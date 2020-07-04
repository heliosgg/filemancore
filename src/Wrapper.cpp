#include "Wrapper.h"

#include "interfaces/FileInfo.h"
#include "Utils.h"

namespace FileManCore {

    namespace NapiWrapper {
        
        Napi::Array listDir(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();
            Napi::Array result = Napi::Array::New(env);

            if (args.Length() != 1) {
                Utils::NapiHelpers::BuildException(env, "listDir: function takes only 1 argument").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            if (!args[0].IsString()) {
                Utils::NapiHelpers::BuildException(env, "listDir: 1st param must be string").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            std::u16string searchMask = args[0].ToString().Utf16Value();
            std::u16string rootDir;

            // Check if it is not a mask
            if (searchMask.rfind(u'*') == std::u16string::npos && searchMask.rfind(u'?') == std::u16string::npos) {
                if (searchMask.back() != u'\\') {
                    searchMask.reserve(searchMask.size() + 2);
                    searchMask.append(u"\\");
                }

                rootDir.assign(searchMask);
                searchMask.append(u"*");
            }
            else {
                rootDir.assign(searchMask.substr(0, searchMask.rfind(u'\\') + 1));
            }

            rootDir = Utils::Path::Canonicalise(rootDir);

            WIN32_FIND_DATAW FindFileData;
            HANDLE hFind = FindFirstFileW((LPWSTR)searchMask.c_str(), &FindFileData);
            FileManCore::FileInfo TempFileInfo;

            if (hFind == INVALID_HANDLE_VALUE) {
                Utils::NapiHelpers::BuildException(env, "listDir: FindFirstFileW failed. Last error: 0x%x", GetLastError()).ThrowAsJavaScriptException();
                return Napi::Array();
            }

            uint16_t iter = 0;

            do {
                // Skip "." (current dir)
                if (FindFileData.cFileName[0] == L'.' && FindFileData.cFileName[0] == L'\0')
                    continue;

                TempFileInfo.ResetFields();

                TempFileInfo.SetPath(rootDir);
                TempFileInfo.SetName((char16_t*)FindFileData.cFileName);
                TempFileInfo.SetSize({ FindFileData.nFileSizeHigh, FindFileData.nFileSizeLow });
                TempFileInfo.SetLastModified(FindFileData.ftLastWriteTime);
                TempFileInfo.SetCreated(FindFileData.ftCreationTime);
                TempFileInfo.SetAttributes(Attributes(FindFileData.dwFileAttributes));

                result.Set(iter, TempFileInfo.ToNapiObject(env));
                iter++;
            } while (FindNextFileW(hFind, &FindFileData) != 0);

            return result;
        }

        Napi::String normalizePath(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            if (args.Length() != 1) {
                Utils::NapiHelpers::BuildException(env, "normalizePath: function takes only 1 argument").ThrowAsJavaScriptException();
                return Napi::String();
            }

            if (!args[0].IsString()) {
                Utils::NapiHelpers::BuildException(env, "normalizePath: 1st param must be string").ThrowAsJavaScriptException();
                return Napi::String();
            }

            return Napi::String::New(env, Utils::Path::Canonicalise(args[0].ToString().Utf16Value()));
        }
        
        Napi::Boolean moveFile(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            if (args.Length() != 2) {
                Utils::NapiHelpers::BuildException(env, "moveFile: function takes 2 arguments").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            if (!args[0].IsString()) {
                Utils::NapiHelpers::BuildException(env, "moveFile: 1st param must be string").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            if (!args[1].IsString()) {
                Utils::NapiHelpers::BuildException(env, "moveFile: 2nd param must be string").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            std::u16string existingFileName = args[0].ToString().Utf16Value();
            std::u16string newFileName = args[1].ToString().Utf16Value();

            if (!MoveFileW((LPWSTR)existingFileName.c_str(), (LPWSTR)newFileName.c_str())) {
                Utils::NapiHelpers::BuildException(env, "moveFile: MoveFileW failed. Last error: 0x%x", GetLastError()).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            return Napi::Boolean::New(env, true);
        }

    }

}
#include "Wrapper.h"

#include "classes/FileInfo.h"
#include "Utils.h"

namespace FileManCore {

    namespace NapiWrapper {
        
        Napi::Array listDir(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

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

            if (Utils::Path::Canonicalise(rootDir, rootDir) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "listDir: Utils::Path::Canonicalise failed").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            WIN32_FIND_DATAW FindFileData;
            HANDLE hFind = FindFirstFileW((LPWSTR)searchMask.c_str(), &FindFileData);
            FileManCore::FileInfo TempFileInfo;

            if (hFind == INVALID_HANDLE_VALUE) {
                Utils::NapiHelpers::BuildException(
                    env,
                    "listDir: FindFirstFileW failed. Last error: %S",
                    Utils::Win::NapiHelpers::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
                return Napi::Array();
            }

            uint16_t iter = 0;

            Napi::Array result = Napi::Array::New(env);
            do {
                // Skip "." (current dir)
                if (FindFileData.cFileName[0] == L'.' && FindFileData.cFileName[1] == L'\0')
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
        
        Napi::Array listDrives(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            if (args.Length() != 0){
                Utils::NapiHelpers::BuildException(env, "listDrives: FindFirstFileW failed. Last error: %S").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            std::vector<std::u16string> vecDrives;
            if (Utils::FileSystem::ListDrives(env, vecDrives) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "listDrives: FileManCore::Utils::FileSystem::ListDrives failed.").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            Napi::Array result = Napi::Array::New(env);
            if (Utils::NapiHelpers::StringVectorToArray(env, vecDrives, result) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "listDrives: FileManCore::Utils::NapiHelpers::StringVectorToArray failed.").ThrowAsJavaScriptException();
                return Napi::Array();
            }

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

            std::u16string path = args[0].ToString().Utf16Value();
            if (Utils::Path::Canonicalise(path, path) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "normalizePath: Utils::Path::Canonicalise failed").ThrowAsJavaScriptException();
                return Napi::String();
            }

            return Napi::String::New(env, path);
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

            if (FileManCore::Utils::FileSystem::MoveFile(env, existingFileName, newFileName) != FMC_OK)
                return Napi::Boolean::New(env, false);

            return Napi::Boolean::New(env, true);
        }

        Napi::Boolean openWithDefaultApp(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            if (args.Length() != 1) {
                Utils::NapiHelpers::BuildException(env, "openWithDefaultApp: function takes only 1 argument").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            if (!args[0].IsString()) {
                Utils::NapiHelpers::BuildException(env, "openWithDefaultApp: 1st param must be string").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            std::u16string filePath = args[0].ToString().Utf16Value();

            if (ShellExecuteW(NULL, L"open", (LPWSTR)filePath.c_str(), NULL, NULL, SW_SHOW) <= (HINSTANCE)32) {
                Utils::NapiHelpers::BuildException(env, "openWithDefaultApp: ShellExecute failed").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            return Napi::Boolean::New(env, true);
        }
    }

}
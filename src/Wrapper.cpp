#include "Wrapper.h"
#include "interfaces/FileInfo.h"

namespace FileManCore {

    namespace NapiWrapper {

        Napi::String helloWorld(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            return Napi::String::New(env, std::string("Hello, world!"));
        }

        Napi::String helloUser(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            Napi::Object user = args[0].ToObject();

            if (!user.Get("name").IsString()) {
                Napi::Error::New(env, "WTF?!").ThrowAsJavaScriptException();
                return Napi::String();
            }

            std::stringstream result;
            result << "Hello, " << user.Get("name").ToString().Utf8Value() << " " << user.Get("surname").ToString().Utf8Value();

            return Napi::String::New(env, result.str());
        }
        
        Napi::Array listDir(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();
            Napi::Array result = Napi::Array::New(env);

            if (!args[0].IsString()) {
                Napi::Error::New(env, "listDir: first param is not string").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            WIN32_FIND_DATAW FindFileData;
            std::u16string root_dir = args[0].ToString().Utf16Value();
            HANDLE hFind = FindFirstFileW((LPWSTR)root_dir.c_str(), &FindFileData);
            FileManCore::FileInfo TempFileInfo;

            if (hFind == INVALID_HANDLE_VALUE) {
                Napi::Error::New(env, "listDir: FindFirstFileW failed").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            uint16_t iter = 0;

            do {
                TempFileInfo.ResetFields();

                TempFileInfo.SetPath(root_dir);
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

    }

}
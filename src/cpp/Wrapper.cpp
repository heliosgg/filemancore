#include "Wrapper.h"

#include "classes/FileInfo.h"
#include "utils/Utils.h"

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
            std::vector<FileManCore::FileInfo> dirVector;
            if (Utils::FileSystem::ListDir(env, searchMask, dirVector) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "listDir: FileManCore::Utils::FileSystem::ListDir failed.").ThrowAsJavaScriptException();
                return Napi::Array();
            }

            Napi::Array result = Napi::Array::New(env);
            if (Utils::NapiHelpers::FileInfoVectorToArray(env, dirVector, result) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "listDir: FileManCore::Utils::NapiHelpers::FileInfoVectorToArray failed.").ThrowAsJavaScriptException();
                return Napi::Array();
            }

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
                Utils::NapiHelpers::BuildException(env, "normalizePath: FileManCore::Utils::Path::Canonicalise failed").ThrowAsJavaScriptException();
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
            if (Utils::OS::OpenWithDefaultApp(env, filePath) != FMC_OK) {
                Utils::NapiHelpers::BuildException(env, "openWithDefaultApp: FileManCore::Utils::OS::OpenWithDefaultApp failed").ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }

            return Napi::Boolean::New(env, true);
        }
    }

}
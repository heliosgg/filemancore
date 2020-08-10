#include "Wrapper.h"

#include "classes/FileInfo.h"
#include "utils/Utils.h"

namespace FileManCore {

    namespace NapiWrapper {
        
        FMC_NAPI_EXPORT(Napi::Array, listDir) {
            FMC_NAPI_ENV_INIT();
            
            FMC_NAPI_VERIFY_ARGS_LENGTH(1, Napi::Array());
            FMC_NAPI_VERIFY_ARG_STRING(0, Napi::Array());

            std::u16string searchMask = args[0].ToString().Utf16Value();
            std::vector<FileManCore::FileInfo> dirVector;
            if (Utils::FileSystem::ListDir(FMC_NAPI_ENV, searchMask, dirVector) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Array();
            }

            Napi::Array result = Napi::Array::New(FMC_NAPI_ENV);
            if (Utils::NapiHelpers::FileInfoVectorToArray(FMC_NAPI_ENV, dirVector, result) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Array();
            }

            return result;
        }
        
        FMC_NAPI_EXPORT(Napi::Array, listDrives) {
            FMC_NAPI_ENV_INIT();

            FMC_NAPI_VERIFY_ARGS_LENGTH(0, Napi::Array());

            std::vector<std::u16string> vecDrives;
            if (Utils::FileSystem::ListDrives(FMC_NAPI_ENV, vecDrives) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Array();
            }

            Napi::Array result = Napi::Array::New(FMC_NAPI_ENV);
            if (Utils::NapiHelpers::StringVectorToArray(FMC_NAPI_ENV, vecDrives, result) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Array();
            }

            return result;
        }

        FMC_NAPI_EXPORT(Napi::String, normalizePath) {
            FMC_NAPI_ENV_INIT();

            FMC_NAPI_VERIFY_ARGS_LENGTH(1, Napi::String());
            FMC_NAPI_VERIFY_ARG_STRING(0, Napi::String());

            std::u16string path = args[0].ToString().Utf16Value();
            if (Utils::Path::Canonicalise(path, path) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::String();
            }

            return Napi::String::New(FMC_NAPI_ENV, path);
        }
        
        FMC_NAPI_EXPORT(Napi::Boolean, moveFile) {
            FMC_NAPI_ENV_INIT();

            FMC_NAPI_VERIFY_ARGS_LENGTH(2, Napi::Boolean::New(FMC_NAPI_ENV, false));
            FMC_NAPI_VERIFY_ARG_STRING(0, Napi::Boolean::New(FMC_NAPI_ENV, false));
            FMC_NAPI_VERIFY_ARG_STRING(1, Napi::Boolean::New(FMC_NAPI_ENV, false));

            std::u16string existingFileName = args[0].ToString().Utf16Value();
            std::u16string newFileName = args[1].ToString().Utf16Value();

            if (FileManCore::Utils::FileSystem::MoveFile(FMC_NAPI_ENV, existingFileName, newFileName) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Boolean::New(FMC_NAPI_ENV, false);
            }

            return Napi::Boolean::New(FMC_NAPI_ENV, true);
        }

        FMC_NAPI_EXPORT(Napi::Boolean, openWithDefaultApp) {
            FMC_NAPI_ENV_INIT();

            FMC_NAPI_VERIFY_ARGS_LENGTH(1, Napi::Boolean::New(FMC_NAPI_ENV, false));
            FMC_NAPI_VERIFY_ARG_STRING(0, Napi::Boolean::New(FMC_NAPI_ENV, false));

            std::u16string filePath = args[0].ToString().Utf16Value();
            if (Utils::OS::OpenWithDefaultApp(FMC_NAPI_ENV, filePath) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::Boolean::New(FMC_NAPI_ENV, false);
            }

            return Napi::Boolean::New(FMC_NAPI_ENV, true);
        }

        FMC_NAPI_EXPORT(Napi::String, getRemoteCWD) {
            FMC_NAPI_ENV_INIT();

            FMC_NAPI_VERIFY_ARGS_LENGTH(1, Napi::String());
            FMC_NAPI_VERIFY_ARG_NUMBER(0, Napi::String());

            uint32_t pid = FMC_NAPI_EXPORT_ARGS[0].ToNumber().Uint32Value();
            std::u16string outPath;
            if (Utils::OS::GetRemoteCWD(FMC_NAPI_ENV, pid, outPath) != FMC_OK) {
                FMC_NAPI_EXCEPTION_REPEAT();
                return Napi::String();
            }

            return Napi::String::New(FMC_NAPI_ENV, outPath);
        }
    }

}
#include "Utils.h"

#include <algorithm>
#include <stdarg.h>
#include <fstream>
#include <sys/stat.h>

namespace FileManCore {
    namespace Utils {
        namespace NapiHelpers {
            Napi::Error BuildException(FMC_NAPI_ENV_ARG, char* fmt, ...) {
                char szErrorMessage[255];
                va_list args;

                va_start(args, fmt);
                vsnprintf(szErrorMessage, sizeof(szErrorMessage), fmt, args);
                va_end(args);

                return Napi::Error::New(FMC_NAPI_ENV, szErrorMessage);
            }

            FMC_ERR StringVectorToArray(FMC_NAPI_ENV_ARG, std::vector<std::u16string>& inputVector, Napi::Array& outArray) {
                size_t iter = 0;

                for (std::u16string& s : inputVector) {
                    outArray.Set(iter++, Napi::String::New(FMC_NAPI_ENV, s));
                }

                return FMC_OK;
            }

            FMC_ERR FileInfoVectorToArray(FMC_NAPI_ENV_ARG, std::vector<FileManCore::FileInfo>& inputVector, Napi::Array& outArray) {
                size_t iter = 0;

                for (FileManCore::FileInfo& fi : inputVector) {
                    outArray.Set(iter++, fi.ToNapiObject(FMC_NAPI_ENV));
                }

                return FMC_OK;
            }
        }

        namespace FileSystem {
            FMC_ERR CreateEmptyFile(FMC_NAPI_ENV_ARG, const std::u16string& fileName, bool eraseIfExist) {
                if (FileExist(FMC_NAPI_ENV, fileName) && !eraseIfExist) {
                    FMC_NAPI_EXCEPTION("File already exists. You can pass eraseIfExist true");
                    return FMC_ALREADYEXIST;
                }

                std::ofstream f((wchar_t*)fileName.c_str());
                if (!f.good()) {
                    FMC_NAPI_EXCEPTION("Failed to create file");
                    return FMC_UNKNOWN;
                }

                f.close();
                return FMC_OK;
            }

            bool FileExist(FMC_NAPI_ENV_ARG, const std::u16string& fileName) {
                struct _stat buffer;
                return (_wstat((wchar_t*)fileName.c_str(), &buffer) == 0); 
            }
        }
    }
}
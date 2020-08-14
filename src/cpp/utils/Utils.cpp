#include "Utils.h"

#include <algorithm>
#include <stdarg.h>

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
    }
}
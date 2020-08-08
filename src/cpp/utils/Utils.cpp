#include "Utils.h"

#include <algorithm>
#include <stdarg.h>

#pragma comment(lib, "Mpr.lib")

namespace FileManCore {
    namespace Utils {
        namespace NapiHelpers {
            Napi::Error BuildException(NAPI_ENV_ARG, char* fmt, ...) {
                char szErrorMessage[255];
                va_list args;

                va_start(args, fmt);
                vsnprintf(szErrorMessage, sizeof(szErrorMessage), fmt, args);
                va_end(args);

                return Napi::Error::New(env, szErrorMessage);
            }

            FMC_ERR StringVectorToArray(NAPI_ENV_ARG, std::vector<std::u16string>& inputVector, Napi::Array& outArray) {
                size_t iter = 0;

                for (std::u16string& s : inputVector) {
                    outArray.Set(iter++, Napi::String::New(env, s));
                }

                return FMC_OK;
            }

            FMC_ERR FileInfoVectorToArray(NAPI_ENV_ARG, std::vector<FileManCore::FileInfo>& inputVector, Napi::Array& outArray) {
                size_t iter = 0;

                for (FileManCore::FileInfo& fi : inputVector) {
                    outArray.Set(iter++, fi.ToNapiObject(env));
                }

                return FMC_OK;
            }
        }
    }
}
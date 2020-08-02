#include "Utils.h"

#include <algorithm>
#include <stdarg.h>

#pragma comment(lib, "Mpr.lib")

namespace FileManCore {
    namespace Utils {
        namespace NapiHelpers {
            Napi::Error BuildException(Napi::Env& env, char* fmt, ...) {
                char szErrorMessage[255];
                va_list args;

                va_start(args, fmt);
                vsnprintf(szErrorMessage, sizeof(szErrorMessage), fmt, args);
                va_end(args);

                return Napi::Error::New(env, szErrorMessage);
            }
        }

        FMC_ERR StringVectorToArray(Napi::Env& env, std::vector<std::u16string>& inputVector, Napi::Array& outArray) {
            size_t iter = 0;

            for (std::u16string& s : inputVector) {
                outArray.Set(iter++, Napi::String::New(env, s));
            }

            return FMC_OK;
        }
    }
}
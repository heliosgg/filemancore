#pragma once

#include <napi.h>
#include <string>
#include <vector>

#include "../FmcDefs.h"

#ifdef _WIN32
    #include "win/Time.h"
    #include "win/NapiHelpers.h"
#endif

namespace FileManCore {
    namespace Utils {
        namespace Path {
            FMC_ERR Canonicalise(const std::u16string& path, std::u16string& resultPath);
        }

        namespace NapiHelpers {
            Napi::Error BuildException(Napi::Env& env, char* fmt, ...);
            FMC_ERR StringVectorToArray(Napi::Env& env, std::vector<std::u16string>& inputVector, Napi::Array& outArray);
        }

        namespace FileSystem {
            FMC_ERR ListDrives(Napi::Env& env, std::vector<std::u16string>& outVector);
            FMC_ERR MoveFile(Napi::Env& env, std::u16string& existingFileName, std::u16string& newFileName);
        }
    }
}
#pragma once

#include <napi.h>
#include <string>
#include <vector>

#include "../FmcDefs.h"
#include "../classes/FileInfo.h"

#ifdef _WIN32
    #include "win/Time.h"
    #include "win/OS.h"
#endif

namespace FileManCore {
    namespace Utils {
        namespace Path {
            FMC_ERR Canonicalise(const std::u16string& path, std::u16string& resultPath);
        }

        namespace NapiHelpers {
            Napi::Error BuildException(NAPI_ENV_ARG, char* fmt, ...);
            FMC_ERR StringVectorToArray(NAPI_ENV_ARG, std::vector<std::u16string>& inputVector, Napi::Array& outArray);
            FMC_ERR FileInfoVectorToArray(NAPI_ENV_ARG, std::vector<FileManCore::FileInfo>& inputVector, Napi::Array& outArray);
        }

        namespace FileSystem {
            FMC_ERR ListDir(NAPI_ENV_ARG, const std::u16string& searchMaskWithRootDir, std::vector<FileManCore::FileInfo>& outVector);
            FMC_ERR ListDrives(NAPI_ENV_ARG, std::vector<std::u16string>& outVector);
            FMC_ERR MoveFile(NAPI_ENV_ARG, std::u16string& existingFileName, std::u16string& newFileName);
        }

        namespace OS {
            FMC_ERR OpenWithDefaultApp(NAPI_ENV_ARG, const std::u16string& filePath);
        }
    }
}
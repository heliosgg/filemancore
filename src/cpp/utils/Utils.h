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
            Napi::Error BuildException(FMC_NAPI_ENV_ARG, char* fmt, ...);
            FMC_ERR StringVectorToArray(FMC_NAPI_ENV_ARG, std::vector<std::u16string>& inputVector, Napi::Array& outArray);
            FMC_ERR FileInfoVectorToArray(FMC_NAPI_ENV_ARG, std::vector<FileManCore::FileInfo>& inputVector, Napi::Array& outArray);
        }

        namespace FileSystem {
            FMC_ERR ListDir(FMC_NAPI_ENV_ARG, const std::u16string& searchMaskWithRootDir, std::vector<FileManCore::FileInfo>& outVector);
            FMC_ERR ListDrives(FMC_NAPI_ENV_ARG, std::vector<std::u16string>& outVector);
            FMC_ERR MoveFile(FMC_NAPI_ENV_ARG, std::u16string& existingFileName, std::u16string& newFileName);
            FMC_ERR CreateEmptyFile(FMC_NAPI_ENV_ARG, const std::u16string& fileName, bool eraseIfExist);
            bool FileExist(FMC_NAPI_ENV_ARG, const std::u16string& fileName);
        }

        namespace OS {
            FMC_ERR OpenWithDefaultApp(FMC_NAPI_ENV_ARG, const std::u16string& filePath);
            FMC_ERR GetRemoteCWD(FMC_NAPI_ENV_ARG, uint32_t pid, std::u16string& outCWD);
        }
    }
}
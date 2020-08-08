#include "../Utils.h"

#include <Windows.h>

using namespace FileManCore;

FMC_ERR FileManCore::Utils::FileSystem::ListDir(NAPI_ENV_ARG, const std::u16string& searchMaskWithRootDir, std::vector<FileManCore::FileInfo>& outVector) {

    std::u16string searchMask = searchMaskWithRootDir;
    std::u16string rootDir;

    // Check if it is not a mask
    if (searchMask.rfind(u'*') == std::u16string::npos && searchMask.rfind(u'?') == std::u16string::npos) {
        if (searchMask.back() != u'\\') {
            searchMask.reserve(searchMask.size() + 2);
            searchMask.append(u"\\");
        }

        rootDir.assign(searchMask);
        searchMask.append(u"*");
    }
    else {
        rootDir.assign(searchMask.substr(0, searchMask.rfind(u'\\') + 1));
    }

    if (Utils::Path::Canonicalise(rootDir, rootDir) != FMC_OK) {
        Utils::NapiHelpers::BuildException(env, "listDir: Utils::Path::Canonicalise failed").ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    WIN32_FIND_DATAW FindFileData;
    HANDLE hFind = FindFirstFileW((LPWSTR)searchMask.c_str(), &FindFileData);
    FileManCore::FileInfo TempFileInfo;

    if (hFind == INVALID_HANDLE_VALUE) {
        Utils::NapiHelpers::BuildException(
            env,
            "listDir: FindFirstFileW failed. Last error: %S",
            Utils::Win::OS::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    outVector.clear();

    do {
        // Skip "." (current dir)
        if (FindFileData.cFileName[0] == L'.' && FindFileData.cFileName[1] == L'\0')
            continue;

        TempFileInfo.ResetFields();

        TempFileInfo.SetPath(rootDir);
        TempFileInfo.SetName((char16_t*)FindFileData.cFileName);
        TempFileInfo.SetSize({ FindFileData.nFileSizeHigh, FindFileData.nFileSizeLow });
        TempFileInfo.SetLastModified(FindFileData.ftLastWriteTime);
        TempFileInfo.SetCreated(FindFileData.ftCreationTime);
        TempFileInfo.SetAttributes(Attributes(FindFileData.dwFileAttributes));

        outVector.push_back(TempFileInfo);
    } while (FindNextFileW(hFind, &FindFileData) != 0);

    return FMC_OK;
}

FMC_ERR FileManCore::Utils::FileSystem::ListDrives(NAPI_ENV_ARG, std::vector<std::u16string>& outVector) {
    DWORD uDriveMask = GetLogicalDrives();

    if(uDriveMask == 0) {
        Utils::NapiHelpers::BuildException(
            env,
            "FileManCore::Utils::FileSystem::ListDrives: GetLogicalDrives failed. Last error: %S",
            Utils::Win::OS::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    char16_t currentDrive[] = u"A:\\";

    while(uDriveMask) {
        if(uDriveMask & 1) {
            outVector.push_back(currentDrive);
        }                    

        ++currentDrive[0];
        uDriveMask >>= 1;
    }

    return FMC_OK;
}

FMC_ERR FileManCore::Utils::FileSystem::MoveFile(NAPI_ENV_ARG, std::u16string& existingFileName, std::u16string& newFileName) {
    if (!::MoveFileW((LPWSTR)existingFileName.c_str(), (LPWSTR)newFileName.c_str())) {
        Utils::NapiHelpers::BuildException(
            env,
            "FileManCore::Utils::FileSystem::MoveFile: MoveFileW failed. Last error: %S",
            Utils::Win::OS::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    return FMC_OK;
}
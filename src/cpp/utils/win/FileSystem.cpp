#include "../Utils.h"

#include <Windows.h>

FMC_ERR FileManCore::Utils::FileSystem::MoveFile(Napi::Env& env, std::u16string& existingFileName, std::u16string& newFileName) {
    if (!::MoveFileW((LPWSTR)existingFileName.c_str(), (LPWSTR)newFileName.c_str())) {
        Utils::NapiHelpers::BuildException(
            env,
            "FileManCore::Utils::FileSystem::MoveFile: MoveFileW failed. Last error: %S",
            Utils::Win::NapiHelpers::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    return FMC_OK;
}

FMC_ERR FileManCore::Utils::FileSystem::ListDrives(Napi::Env& env, std::vector<std::u16string>& outVector) {
    DWORD uDriveMask = GetLogicalDrives();

    if(uDriveMask == 0) {
        FileManCore::Utils::NapiHelpers::BuildException(
            env,
            "FileManCore::Utils::FileSystem::ListDrives: GetLogicalDrives failed. Last error: %S",
            FileManCore::Utils::Win::NapiHelpers::GetLastErrorAsString().c_str()).ThrowAsJavaScriptException();
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
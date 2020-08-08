#include "../Utils.h"

#include <Windows.h>

std::u16string FileManCore::Utils::Win::OS::GetLastErrorAsString() {
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::u16string(); //No error message has been recorded

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&messageBuffer, 0, NULL);

    std::u16string message((char16_t*)messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

FMC_ERR FileManCore::Utils::OS::OpenWithDefaultApp(NAPI_ENV_ARG, const std::u16string& filePath) {

    if (ShellExecuteW(NULL, L"open", (LPWSTR)filePath.c_str(), NULL, NULL, SW_SHOW) <= (HINSTANCE)32) {
        Utils::NapiHelpers::BuildException(env, "openWithDefaultApp: ShellExecute failed").ThrowAsJavaScriptException();
        return FMC_UNKNOWN;
    }

    return FMC_OK;
}
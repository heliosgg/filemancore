#include "../Utils.h"

#include <shellapi.h>
#include <phnt_windows.h>
#include <phnt.h>

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

FMC_ERR FileManCore::Utils::OS::OpenWithDefaultApp(FMC_NAPI_ENV_ARG, const std::u16string& filePath) {

    if (ShellExecuteW(NULL, L"open", (LPWSTR)filePath.c_str(), NULL, NULL, SW_SHOW) <= (HINSTANCE)32) {
        FMC_NAPI_EXCEPTION("ShellExecute failed");
        return FMC_UNKNOWN;
    }

    return FMC_OK;
}

FMC_ERR FileManCore::Utils::OS::GetRemoteCWD(FMC_NAPI_ENV_ARG, uint32_t pid, std::u16string& outCWD) {

    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!handle) {
        FMC_NAPI_EXCEPTION("OpenProcess failed. Last error: %S", Utils::Win::OS::GetLastErrorAsString().c_str());
        return FMC_UNKNOWN;
    }

    NTSTATUS ntStatus = 0;
    PROCESS_BASIC_INFORMATION pbi = { 0 };
    SIZE_T len = 0;
    if (!NT_SUCCESS(ntStatus = NtQueryInformationProcess(handle, ProcessBasicInformation, &pbi, sizeof (pbi), (PULONG)&len))) {
        FMC_NAPI_EXCEPTION("NtQueryInformationProcess failed. NTSTATUS: 0x%x", ntStatus);
        return FMC_UNKNOWN;
    }

    PEB peb = { 0 };
    if (!ReadProcessMemory(handle, pbi.PebBaseAddress, &peb, sizeof(PEB), &len)) {
        FMC_NAPI_EXCEPTION("ReadProcessMemory failed. Last error: %S", Utils::Win::OS::GetLastErrorAsString().c_str());
        return FMC_UNKNOWN;
    }

    RTL_USER_PROCESS_PARAMETERS upp = { 0 };
    if (!ReadProcessMemory (handle, peb.ProcessParameters, &upp, sizeof(RTL_USER_PROCESS_PARAMETERS), &len)) {
        FMC_NAPI_EXCEPTION("ReadProcessMemory failed. Last error: %S", Utils::Win::OS::GetLastErrorAsString().c_str());
        return FMC_UNKNOWN;
    }

    DWORD pathLength = upp.CurrentDirectory.DosPath.Length / 2 + 1;
    std::vector<char16_t> pathBuffer(pathLength);
    if (!ReadProcessMemory(handle, upp.CurrentDirectory.DosPath.Buffer, pathBuffer.data(), upp.CurrentDirectory.DosPath.Length, &len)) {
        FMC_NAPI_EXCEPTION("ReadProcessMemory failed. Last error: %S", Utils::Win::OS::GetLastErrorAsString().c_str());
        return FMC_UNKNOWN;
    }

    // null terminate
    pathBuffer.at(pathLength - 1) = u'\0';

    outCWD.assign(pathBuffer.data());

    return FMC_OK;
}
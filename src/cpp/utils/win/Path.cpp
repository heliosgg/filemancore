#include "../Utils.h"

#include <Windows.h>

FMC_ERR FileManCore::Utils::Path::Canonicalise(const std::u16string& path, std::u16string& resultPath)
{
    std::u16string utf16(path);

    char16_t canon[MAX_PATH];

    /** Note that PathCanonicalize does NOT do what we want here -- it's a
     * purely textual operation that eliminates /./ and /../ only.
     */
    DWORD rc = ::GetFullPathNameW((LPWSTR)utf16.c_str(), MAX_PATH, (LPWSTR)canon, NULL);
    if (!rc)
        return FMC_UNKNOWN;

    utf16.assign((char16_t*)canon);

    if (utf16.length() >= 6)
    {
        /** Get rid of \\?\ and \\.\ prefixes on drive-letter paths */
        if (utf16.rfind(u"\\\\?\\", 0) != std::u16string::npos && utf16[5] == u':')
            utf16.erase(0,4);
        else if (utf16.rfind(u"\\\\.\\", 0) != std::u16string::npos && utf16[5] == u':')
            utf16.erase(0,4);
    }

    if (utf16.length() >= 10)
    {
        /** Get rid of \\?\UNC on drive-letter and UNC paths */
        if (utf16.rfind(u"\\\\?\\UNC\\", 0) != std::u16string::npos)
        {
            if (utf16[9] == u':' && utf16[10] == u'\\')
                utf16.erase(0,8);
            else
            {
                utf16.erase(0,7);
                utf16 = u"\\" + utf16;
            }
        }
    }

    /** Anything other than UNC and drive-letter is something we don't
     * understand
     */
    if (utf16[0] == L'\\' && utf16[1] == L'\\')
    {
        if (utf16[2] == '?' || utf16[2] == '.')
            return FMC_UNKNOWN; // Not understood

        /** OK -- UNC */
    }
    else if (((utf16[0] >= 'A' && utf16[0] <= 'Z')
            || (utf16[0] >= 'a' && utf16[0] <= 'z'))
            && utf16[1] == ':')
    {
        /** OK -- drive letter -- unwind subst'ing */
        for (;;)
        {
            wchar_t drive[3];
            drive[0] = (wchar_t)toupper(utf16[0]);
            drive[1] = L':';
            drive[2] = L'\0';
            canon[0] = L'\0';
            rc = ::QueryDosDeviceW(drive, (LPWSTR)canon, MAX_PATH);
            if (!rc)
                break;
            if (!wcsncmp((LPWSTR)canon, L"\\??\\", 4))
            {
                utf16 = std::u16string(canon + 4) + std::u16string(utf16, 2);
            }
            else // Not subst'd
                break;
        }

        wchar_t drive[4];
        drive[0] = (wchar_t)toupper(utf16[0]);
        drive[1] = ':';
        drive[2] = '\\';
        drive[3] = '\0';

        rc = ::GetDriveTypeW(drive);

        if (rc == DRIVE_REMOTE)
        {
            DWORD bufsize = MAX_PATH;

            /* QueryDosDevice and WNetGetConnection FORBID the
            * trailing slash; GetDriveType REQUIRES it.
            */
            drive[2] = '\0';

            rc = ::WNetGetConnectionW(drive, (LPWSTR)canon, &bufsize);
            if (rc == NO_ERROR)
                utf16 = std::u16string(canon) + std::u16string(utf16, 2);
        }
    }
    else
    {
        // Not understood
        return FMC_UNKNOWN;
    }

    /** Canonicalise case and 8.3-ness */
    rc = ::GetLongPathNameW((LPWSTR)utf16.c_str(), (LPWSTR)canon, MAX_PATH);
    if (!rc)
        return FMC_UNKNOWN;

    std::replace(utf16.begin(), utf16.end(), u'/', u'\\');

    resultPath.swap(utf16);
    return FMC_OK;
}
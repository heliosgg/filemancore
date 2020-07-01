#include "Utils.h"

#pragma comment(lib, "Mpr.lib")

namespace FileManCore {
    namespace Utils {
        namespace Time {
            bool FileTime2TimePoint(const FILETIME& ft, std::chrono::system_clock::time_point& result) {
                SYSTEMTIME st = {0};
                if (!FileTimeToSystemTime(&ft, &st)) {
                    // Should
                    return false;
                }

                // number of seconds 
                ULARGE_INTEGER ull;
                ull.LowPart = ft.dwLowDateTime;
                ull.HighPart = ft.dwHighDateTime;

                time_t secs = ull.QuadPart / 10000000ULL - 11644473600ULL;
                std::chrono::milliseconds ms((ull.QuadPart / 10000ULL) % 1000);

                result = std::chrono::system_clock::from_time_t(secs);
                result += ms;
                
                return true;
            }

            bool TimePoint2FileTime(const std::chrono::system_clock::time_point& tp, FILETIME& result)
            {
                using namespace std;
                using namespace std::chrono;

                SYSTEMTIME st = { 0 };
                auto s = duration_cast<seconds>(tp.time_since_epoch());
                auto t = (time_t)(s.count());
                tm utc_tm;
                if (!gmtime_s(&utc_tm, &t)) {
                    st.wSecond = static_cast<WORD>(utc_tm.tm_sec);
                    st.wMinute = static_cast<WORD>(utc_tm.tm_min);
                    st.wHour = static_cast<WORD>(utc_tm.tm_hour);
                    st.wDay = static_cast<WORD>(utc_tm.tm_mday);
                    st.wMonth = static_cast<WORD>(utc_tm.tm_mon + 1);
                    st.wYear = static_cast<WORD>(utc_tm.tm_year + 1900);
                    st.wMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;

                    if (!SystemTimeToFileTime(&st, &result)) {
                        result.dwHighDateTime = 0;
                        result.dwLowDateTime = 0;
                        
                        return false;
                    }
                }
                else {
                    return false;
                }
                
                return true;
            }
        }

        namespace Path {
            std::u16string Canonicalise(const std::u16string& path)
            {
                std::u16string utf16(path);

                char16_t canon[MAX_PATH];

                /** Note that PathCanonicalize does NOT do what we want here -- it's a
                 * purely textual operation that eliminates /./ and /../ only.
                 */
                DWORD rc = ::GetFullPathNameW((LPWSTR)utf16.c_str(), MAX_PATH, (LPWSTR)canon, NULL);
                if (!rc)
                    return path;

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
                        return path; // Not understood

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
                    return path;
                }

                /** Canonicalise case and 8.3-ness */
                rc = ::GetLongPathNameW((LPWSTR)utf16.c_str(), (LPWSTR)canon, MAX_PATH);
                if (!rc)
                    return path;

                std::replace(utf16.begin(), utf16.end(), u'/', u'\\');

                return utf16;
            }
        }
    }
}
#include "Utils.h"

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
    }
}
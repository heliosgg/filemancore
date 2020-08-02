#pragma once

#include <Windows.h>
#include <chrono>

namespace FileManCore {
    namespace Utils {
        namespace Win {
            namespace Time {
                /**
                 * FILETIME to std::chrono::system_clock::time_point (including milliseconds)
                 * Paste from internet not tested
                 */
                bool FileTime2TimePoint(const FILETIME& ft, std::chrono::system_clock::time_point& result);
                /**
                * Convert a std::chrono::system_clock::time_point to FILETIME. 
                * Paste from internet not tested
                */ 
                bool TimePoint2FileTime(const std::chrono::system_clock::time_point& tp, FILETIME& result);
            }
        }
    }
}
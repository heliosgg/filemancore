#pragma once

#include <napi.h>

#include <Windows.h>
#include <string>
#include <chrono>
#include <algorithm>

namespace FileManCore {

    namespace Utils {
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

        namespace Path {
            /**
             * Path normalizer
             * Src: http://pdh11.blogspot.com/2009/05/pathcanonicalize-versus-what-it-says-on.html
             * Not tested :D
             */
            std::u16string Canonicalise(const std::u16string& path);
        }

        namespace NapiHelpers {
            Napi::Error BuildException(Napi::Env env, LPCSTR fmt, ...);
            std::u16string GetLastErrorAsString();
        }
    }
}
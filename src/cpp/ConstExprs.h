#pragma once

#ifdef _WIN32
    const char pathSlash = '\\';
#else
    const char pathSlash = '/';
#endif

constexpr const char* str_end(const char *str) {
    return *str ? str_end(str + 1) : str;
}

constexpr bool str_slant(const char *str) {
    return *str == pathSlash ? true : (*str ? str_slant(str + 1) : false);
}

constexpr const char* r_slant(const char* str) {
    return *str == pathSlash ? (str + 1) : r_slant(str - 1);
}
constexpr const char* file_name(const char* str) {
    return str_slant(str) ? r_slant(str_end(str)) : str;
}
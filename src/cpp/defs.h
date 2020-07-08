#pragma once

#ifndef NAPI_CB_ARGS
    #define NAPI_CB_ARGS const Napi::CallbackInfo& args
#endif

    struct Attributes {
        Attributes() :
            hidden(false),
            directory(false),
            readonly(false),
            system(false) {}

        Attributes(DWORD attr) :
            hidden(attr & FILE_ATTRIBUTE_HIDDEN),
            directory(attr & FILE_ATTRIBUTE_DIRECTORY),
            readonly(attr & FILE_ATTRIBUTE_READONLY),
            system(attr & FILE_ATTRIBUTE_SYSTEM) {}

        bool hidden;
        bool directory;
        bool readonly;
        bool system;
    };

    struct SIZETYPE {
        DWORD nFileSizeHigh;
        DWORD nFileSizeLow;
    };

    template<typename T>
    class OptionalParam {
    private:
        T m_value;
        bool m_isSet;
    public:
        OptionalParam(): m_isSet(false) {};

        void Set(T value) { this->m_value = value; m_isSet = true; }
        T Get() { return m_value; }

        bool IsSet() { return m_isSet; }
        void Reset() { m_isSet = false; }
    };
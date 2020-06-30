#pragma once

#include <Windows.h>
#include <string>
#include <napi.h>

namespace FileManCore {

    template<typename T>
    class OptionalParam {
    private:
        T m_value;
        bool m_isSet;
    public:
        OptionalParam(): m_isSet(false) {};

        void Set(T value) { value = this->m_value; m_isSet = true; }
        T Get() { return m_value; }

        bool IsSet() { return m_isSet; }
    };

    class FileInfo {
    private:
        typedef DWORD SIZETYPE;
        typedef DWORD LASTMODIFIEDTYPE;
        typedef DWORD CREATEDTYPE;

    public:
        struct Attributes {
            bool hidden = false;
            bool directory = false;
            bool readonly = false;
            bool system = false;
        };
        
    private:
        bool m_init;

        std::string                     m_path;
        std::string                     m_name;
        bool                            m_accessible;
        OptionalParam<SIZETYPE>         m_size;
        OptionalParam<LASTMODIFIEDTYPE> m_lastModified;   // TODO: fixme
        OptionalParam<CREATEDTYPE>      m_created;        // TODO: fixme
        Attributes m_attributes;

    public:
        FileInfo(): m_init(false) {}

        bool InitFromNapiObject(Napi::Env env, Napi::Object obj) { return false; }; // TODO: implement this
        Napi::Object ToNapiObject(Napi::Env env);

        void SetInit(bool value = true) { m_init = value; }
        bool GetInit() { return m_init; }

        void SetPath(std::string value)                 { m_path.assign(value); }
        void SetName(std::string value)                 { m_name.assign(value); }
        void SetAccessible(bool value)                  { m_accessible = value; }
        void SetSize(SIZETYPE value)                    { m_size.Set(value); }
        void SetLastModified(LASTMODIFIEDTYPE value)    { m_lastModified.Set(value); }
        void SetCreated(CREATEDTYPE value)              { m_created.Set(value); }
        void SetAttributes(Attributes value)            { m_attributes = value; }

        std::string GetPath()                               { return m_path; }
        std::string GetName()                               { return m_name; }
        bool GetAccessible()                                { return m_accessible; }
        OptionalParam<SIZETYPE> GetSize()                   { return m_size; }
        OptionalParam<LASTMODIFIEDTYPE> GetLastModified()   { return m_lastModified; }
        OptionalParam<CREATEDTYPE> GetCreated()             { return m_created; }
        Attributes GetAttributes()                          { return m_attributes; }
        
    };

}
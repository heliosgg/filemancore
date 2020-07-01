#pragma once

#include <Windows.h>
#include <string>
#include <napi.h>

#include "../defs.h"

namespace FileManCore {

    class FileInfo {        
    private:
        std::u16string                  m_path;
        std::u16string                  m_name;
        OptionalParam<SIZETYPE>         m_size;
        OptionalParam<FILETIME>         m_lastModified;
        OptionalParam<FILETIME>         m_created;
        Attributes m_attributes;

    public:
        FileInfo():
            m_path(u""),
            m_name(u""),
            m_size(),
            m_lastModified(),
            m_created() {}

        bool InitFromNapiObject(Napi::Env env, Napi::Object obj) { return false; }; // TODO: implement this
        Napi::Object ToNapiObject(Napi::Env env);

        void SetPath(const std::u16string& value)   { m_path.assign(value); }
        void SetName(const std::u16string& value)   { m_name.assign(value); }
        void SetSize(SIZETYPE value)                { m_size.Set(value); }
        void SetLastModified(FILETIME value)        { m_lastModified.Set(value); }
        void SetCreated(FILETIME value)             { m_created.Set(value); }
        void SetAttributes(Attributes value)        { m_attributes = value; }

        void ResetFields();

        std::u16string GetPath()                            { return m_path; }
        std::u16string GetName()                            { return m_name; }
        OptionalParam<SIZETYPE> GetSize()                   { return m_size; }
        OptionalParam<FILETIME> GetLastModified()           { return m_lastModified; }
        OptionalParam<FILETIME> GetCreated()                { return m_created; }
        Attributes GetAttributes()                          { return m_attributes; }
        
    };

}
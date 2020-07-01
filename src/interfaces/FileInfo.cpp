#include "FileInfo.h"
#include "../Utils.h"

namespace FileManCore {

    Napi::Object FileInfo::ToNapiObject(Napi::Env env) {
        Napi::Object result = Napi::Object::New(env);
        Napi::Object attributes = Napi::Object::New(env);

        result.Set("path", m_path);
        result.Set("name", m_name);

        // Optional params
        if (m_size.IsSet()) {
            SIZETYPE size = m_size.Get();
            result.Set("size", (DWORD64)((DWORD64)size.nFileSizeHigh << 32 | (DWORD64)size.nFileSizeLow));
        }
        
        if (m_lastModified.IsSet())
        {
            std::chrono::system_clock::time_point tp =
                std::chrono::system_clock::time_point((std::chrono::system_clock::time_point::min)());
            FILETIME ft = m_lastModified.Get();

            if (Utils::Time::FileTime2TimePoint(ft, tp))
            {
                
                result.Set("lastModified", Napi::Date::New(env,
                    std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count()));
            }
        }

        if (m_created.IsSet())
        {
            std::chrono::system_clock::time_point tp =
                std::chrono::system_clock::time_point((std::chrono::system_clock::time_point::min)());
            FILETIME ft = m_created.Get();

            if (Utils::Time::FileTime2TimePoint(ft, tp))
            {
                
                result.Set("created", Napi::Date::New(env,
                    std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count()));
            }
        }
        
        // Attributes
        attributes.Set("hidden", m_attributes.hidden);
        attributes.Set("directory", m_attributes.directory);
        attributes.Set("readonly", m_attributes.readonly);
        attributes.Set("system", m_attributes.system);

        result.Set("attributes", attributes);

        return result;
    }

    void FileInfo::ResetFields() {
        m_path.assign(u"");
        m_name.assign(u"");
        m_size.Reset();
        m_lastModified.Reset();
        m_created.Reset();
        ZeroMemory(&m_attributes, sizeof(m_attributes));
    }

}
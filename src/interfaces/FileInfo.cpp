#include "FileInfo.h"

namespace FileManCore {

    Napi::Object FileType::ToNapiObject() {
        Napi::Object result;
        Napi::Object attributes;

        result.Set("path", m_path);
        result.Set("name", m_name);
        result.Set("accessible", m_accessible);
        result.Set("accessible", m_accessible);

        // Optional params
        if (m_size.IsSet())
            result.Set("size", m_size.Get());
        
        // TODO: convert to napi date
        if (m_lastModified.IsSet())
            result.Set("lastModified", m_lastModified.Get());

        // TODO: convert to napi date
        if (m_created.IsSet())
            result.Set("created", m_created.Get());
        
        // Attributes
        attributes.Set("hidden", m_attributes.hidden);
        attributes.Set("directory", m_attributes.directory);
        attributes.Set("readonly", m_attributes.readonly);
        attributes.Set("system", m_attributes.system);

        result.Set("attributes", attributes);

        return result;
    }

}
#pragma once

#include <napi.h>
#include <sstream>
#include <Windows.h>
#include <algorithm>
#include <shellapi.h>

#include "defs.h"

namespace FileManCore {

    namespace NapiWrapper {
        
        Napi::Array listDir(NAPI_CB_ARGS);
        Napi::Array listDrives(NAPI_CB_ARGS);
        Napi::String normalizePath(NAPI_CB_ARGS);
        Napi::Boolean moveFile(NAPI_CB_ARGS);
        Napi::Boolean openWithDefaultApp(NAPI_CB_ARGS);

    };

}
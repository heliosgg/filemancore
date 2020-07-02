#pragma once

#include <napi.h>
#include <sstream>
#include <Windows.h>
#include <algorithm>

#include "defs.h"

namespace FileManCore {

    namespace NapiWrapper {
        
        Napi::Array listDir(NAPI_CB_ARGS);
        Napi::String normalizePath(NAPI_CB_ARGS);

    };

}
#pragma once

#include <napi.h>
#include <sstream>
#include <Windows.h>

#include "defs.h"

namespace FileManCore {

    namespace NapiWrapper {

        // TODO: remove it
        Napi::String helloWorld(NAPI_CB_ARGS);
        Napi::String helloUser(NAPI_CB_ARGS);
        
        Napi::Array listDir(NAPI_CB_ARGS);

    };

}
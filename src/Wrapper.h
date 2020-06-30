#pragma once

#include <napi.h>
#include <sstream>

#ifndef NAPI_CB_ARGS
    #define NAPI_CB_ARGS const Napi::CallbackInfo& args
#endif

namespace FileManCore {

    namespace NapiWrapper {

        Napi::String helloWorld(NAPI_CB_ARGS);
        Napi::String helloUser(NAPI_CB_ARGS);

    };

}
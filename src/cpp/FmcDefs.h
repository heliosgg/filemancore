#pragma once

#ifndef NAPI_CB_ARGS
    #define NAPI_CB_ARGS const Napi::CallbackInfo& args
#endif

#ifndef NAPI_ENV_ARG
    #define NAPI_ENV_ARG Napi::Env& env
#endif

enum FMC_ERR {
    FMC_OK = 0,
    FMC_UNKNOWN = 1,
    FMC_BADPARAM = 2,
};
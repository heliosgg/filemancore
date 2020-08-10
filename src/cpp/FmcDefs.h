#pragma once

#include "ConstExprs.h"

#ifdef _WIN32
    #define FMC_FUNC_NAME __FUNCTION__
#else
    #define FMC_FUNC_NAME __FUNCTION__
#endif

#define __FILENAME__ file_name(__FILE__)

#define FMC_NAPI_EXPORT_ARGS_ARG const Napi::CallbackInfo& args
#define FMC_NAPI_EXPORT_ARGS args

#define FMC_NAPI_ENV_ARG Napi::Env& env
#define FMC_NAPI_ENV_INIT() FMC_NAPI_ENV_ARG = FMC_NAPI_EXPORT_ARGS.Env()
#define FMC_NAPI_ENV env

#define FMC_NAPI_EXPORT(type, name) type name(FMC_NAPI_EXPORT_ARGS_ARG)

// Exception definitions
#define FMC_NAPI_EXCEPTION(msg, ...) FileManCore::Utils::NapiHelpers::BuildException(FMC_NAPI_ENV,\
            "%s - %s(%u): " msg, __FILENAME__, FMC_FUNC_NAME, __LINE__, __VA_ARGS__).ThrowAsJavaScriptException()
#define FMC_NAPI_EXCEPTION_REPEAT() if (!FMC_NAPI_ENV.IsExceptionPending()) FMC_NAPI_EXCEPTION("Previous function failed.")

#define FMC_MSG_TAKES_N(n) "Function takes " #n " argument(s)"
#define FMC_MSG_ARG_MUST_BE(n, type) "Argument " #n "must be " type

#define FMC_NAPI_VERIFY_ARGS_LENGTH(lenght, fail_ret_val)\
    if (FMC_NAPI_EXPORT_ARGS.Length() != lenght) {\
        FMC_NAPI_EXCEPTION(FMC_MSG_TAKES_N(lenght));\
        return fail_ret_val;\
    }
#define FMC_NAPI_VERIFY_ARG_STRING(n, fail_ret_val)\
    if (!FMC_NAPI_EXPORT_ARGS[n]) {\
        FMC_NAPI_EXCEPTION(FMC_MSG_ARG_MUST_BE(n, "string"));\
        return fail_ret_val;\
    }
// -- Exception definitions

enum FMC_ERR {
    FMC_OK = 0,
    FMC_UNKNOWN = 1,
    FMC_BADPARAM = 2,
};
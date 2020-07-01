#include <napi.h>

#include "Wrapper.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {

#define ADD_EXPORT(x) exports.Set(  Napi::String::New(env, #x),\
                                    Napi::Function::New(env, FileManCore::NapiWrapper::x))

    ADD_EXPORT(listDir);

#undef ADD_EXPORT

    return exports;
}

NODE_API_MODULE(greet, Init);
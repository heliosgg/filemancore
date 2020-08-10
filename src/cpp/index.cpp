#include <napi.h>

#include "Wrapper.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

#define ADD_EXPORT(x) exports.Set(  Napi::String::New(FMC_NAPI_ENV, #x),\
                                    Napi::Function::New(FMC_NAPI_ENV, FileManCore::NapiWrapper::x))

    ADD_EXPORT(listDir);
    ADD_EXPORT(listDrives);
    ADD_EXPORT(normalizePath);
    ADD_EXPORT(moveFile);
    ADD_EXPORT(openWithDefaultApp);

#undef ADD_EXPORT

    return exports;
}

NODE_API_MODULE(greet, Init);
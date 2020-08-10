#pragma once

#include <napi.h>

#include "utils/Utils.h"
#include "FmcDefs.h"

namespace FileManCore {

    namespace NapiWrapper {
        FMC_NAPI_EXPORT(Napi::Array, listDir);
        FMC_NAPI_EXPORT(Napi::Array, listDrives);
        FMC_NAPI_EXPORT(Napi::String, normalizePath);
        FMC_NAPI_EXPORT(Napi::Boolean, moveFile);
        FMC_NAPI_EXPORT(Napi::Boolean, openWithDefaultApp);
        FMC_NAPI_EXPORT(Napi::String, getRemoteCWD);
    };

}
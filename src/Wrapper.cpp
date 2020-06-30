#include "Wrapper.h"

namespace FileManCore {

    namespace NapiWrapper {

        Napi::String helloWorld(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            return Napi::String::New(env, std::string("Hello, world!"));
        }

        Napi::String helloUser(NAPI_CB_ARGS) {
            Napi::Env env = args.Env();

            Napi::Object user = args[0].ToObject();

            if (!user.Get("name").IsString()) {
                Napi::Error::New(env, "WTF?!").ThrowAsJavaScriptException();
                return Napi::String();
            }

            std::stringstream result;
            result << "Hello, " << user.Get("name").ToString().Utf8Value() << " " << user.Get("surname").ToString().Utf8Value();

            return Napi::String::New(env, result.str());
        }

    }

}
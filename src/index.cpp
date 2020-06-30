#include <napi.h>
#include <sstream>

Napi::String helloWorld(const Napi::CallbackInfo& args) {
    Napi::Env env = args.Env();

    return Napi::String::New(env, std::string("Hello, world!"));
}

Napi::String helloUser(const Napi::CallbackInfo& args) {
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

Napi::Object Init(Napi::Env env, Napi::Object exports) {

#define ADD_EXPORT(x) exports.Set(Napi::String::New(env, #x), Napi::Function::New(env, x))

    ADD_EXPORT(helloWorld);
    ADD_EXPORT(helloUser);

#undef ADD_EXPORT

    return exports;
}

NODE_API_MODULE(greet, Init);
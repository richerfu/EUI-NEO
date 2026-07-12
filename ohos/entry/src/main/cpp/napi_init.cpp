#include <hilog/log.h>
#include <napi/native_api.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace {

constexpr unsigned int kLogDomain = 0x3201;
constexpr const char* kLogTag = "EUI-NEO";
constexpr const char* kModuleName = "entry";

void setMainLibraryHint()
{
    SDL_SetHintWithPriority("SDL_OHOS_MAIN_LIBRARY", "libentry.so", SDL_HINT_OVERRIDE);
}

napi_value initModule(napi_env env, napi_value exports)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, kLogDomain, kLogTag, "EUI-NEO native module init");
    setMainLibraryHint();

    struct ModuleInfo {
        void* env;
        void* exports;
    } info { reinterpret_cast<void*>(env), reinterpret_cast<void*>(exports) };
    SDL_ModuleInit(&info);
    return exports;
}

napi_module module = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = initModule,
    .nm_modname = kModuleName,
    .nm_priv = nullptr,
    .reserved = { 0 },
};

} // namespace

extern "C" __attribute__((constructor)) void registerEuiNeoModule()
{
    napi_module_register(&module);
}

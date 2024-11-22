// added by ZFFramework
// tricks to build static in ZF_impl and export to other libs
#ifndef ZF_ENV_EXPORT
    #ifdef _WIN32
        #define ZF_ENV_EXPORT __declspec(dllexport)
    #else
        #define ZF_ENV_EXPORT __attribute__((visibility("default")))
    #endif
#endif
#ifndef ZF_ENV_IMPORT
    #ifdef _WIN32
        #define ZF_ENV_IMPORT __declspec(dllimport)
    #else
        #define ZF_ENV_IMPORT
    #endif
#endif
#ifndef DECLSPEC
    #define DECLSPEC ZF_ENV_IMPORT
#endif


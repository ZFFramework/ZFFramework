
// we need dll
#ifdef _WIN32
    #define LUA_BUILD_AS_DLL
    #define LUA_CORE
    #define LUA_LIB
#endif

// Android's NDK does not support this
#if defined(ANDROID) || defined(__ANDROID__)
    #undef lua_getlocaledecpoint
    #define lua_getlocaledecpoint() ('.')
#endif

// use longjmp instead of exception by default
#if !ZF_ENV_ZFLUA_USE_EXCEPTION
    #define LUA_USE_LONGJMP 1
#endif

// no jump table support
#define LUA_USE_JUMPTABLE 0

// ignore MSVC warnings
#if defined(_MSC_VER)
    // unsafe APIs
    #pragma warning(disable:4996)
#endif


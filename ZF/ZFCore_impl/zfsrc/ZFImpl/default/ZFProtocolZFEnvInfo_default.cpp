#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFEnvInfo.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Qt
    #include <QSysInfo>
    #include <QString>
#endif

#if ZF_ENV_sys_SDL
    #include "SDL3/SDL.h"
#endif

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#endif

#if ZF_ENV_sys_Posix
    #include <sys/utsname.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_systemInfoImpl_default, ZFEnvInfo_systemInfo, v_ZFProtocolLevel::e_Default)
public:
    virtual zfstring systemName(void) {
        #if ZF_ENV_sys_Qt
        {
            return QSysInfo::productType().toStdString().c_str();
        }
        #endif

        #if ZF_ENV_sys_SDL
        {
            return SDL_GetPlatform();
        }
        #endif

        #if ZF_ENV_sys_Windows
        {
            return zftext("Windows");
        }
        #endif

        #if ZF_ENV_sys_Posix
        {
            struct utsname buf;
            zfmemset(&buf, 0, sizeof(struct utsname));
            uname(&buf);
            return buf.sysname;
        }
        #endif

        return zfnull;
    }
    virtual zfstring systemVersion(void) {
        #if ZF_ENV_sys_Qt
        {
            return QSysInfo::productVersion().toStdString().c_str();
        }
        #endif

        #if ZF_ENV_sys_Windows
        {
            OSVERSIONINFO osvi;
            zfmemset(&osvi, 0, sizeof(OSVERSIONINFO));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            GetVersionEx(&osvi);

            zfstring tmp;
            ZFVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwMajorVersion);
            ZFVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwMinorVersion);
            ZFVersionSetInt(tmp, ZFVERSION_MAIN(), osvi.dwBuildNumber);
            return tmp;
        }
        #endif

        #if ZF_ENV_sys_Posix
        {
            struct utsname buf;
            zfmemset(&buf, 0, sizeof(struct utsname));
            uname(&buf);
            return buf.release;
        }
        #endif

        return zfnull;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_systemInfoImpl_default)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_frameworkInfoImpl_default, ZFEnvInfo_frameworkInfo, v_ZFProtocolLevel::e_Default)
public:
    virtual zfstring frameworkName(void) {
        #if ZF_ENV_sys_Qt
        {
            return zftext("Qt");
        }
        #endif

        #if ZF_ENV_sys_SDL
        {
            return zftext("SDL");
        }
        #endif

        return zfnull;
    }
    virtual zfstring frameworkVersion(void) {
        #if ZF_ENV_sys_Qt
        {
            return QT_VERSION_STR;
        }
        #endif

        #if ZF_ENV_sys_SDL
        {
            int v = SDL_GetVersion();
            return zfstr("%s.%s.%s"
                    , (zfint)SDL_VERSIONNUM_MAJOR(v)
                    , (zfint)SDL_VERSIONNUM_MINOR(v)
                    , (zfint)SDL_VERSIONNUM_MICRO(v)
                    );
        }
        #endif

        return zfnull;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_frameworkInfoImpl_default)

ZF_NAMESPACE_GLOBAL_END


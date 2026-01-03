#include "ZFImpl_default_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

#include "ZFImpl/ZFImpl_env.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_deviceUIInfoImpl_default, ZFEnvInfo_deviceUIInfo, v_ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual ZFEnvDeviceUIType deviceUIType(ZF_IN_OPT ZFEnvDeviceUIType defaultValue = v_ZFEnvDeviceUIType::e_Desktop) {
        #if ZF_ENV_sys_Qt || ZF_ENV_sys_SDL
            return v_ZFEnvDeviceUIType::e_Desktop;
        #else // #if ZF_ENV_sys_Qt
            return v_ZFEnvDeviceUIType::e_Phone;
        #endif // #if ZF_ENV_sys_Qt #else
        return defaultValue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_deviceUIInfoImpl_default)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeInfoImpl_default, ZFEnvInfo_localeInfo, v_ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual void localeInfo(ZF_IN_OUT zfstring &ret) {
        #if ZF_ENV_sys_Windows
            WCHAR wszLocale[LOCALE_NAME_MAX_LENGTH] = {0};
            if(GetUserDefaultLocaleName(wszLocale, LOCALE_NAME_MAX_LENGTH)) {
                int len = WideCharToMultiByte(CP_UTF8, 0, wszLocale, -1, NULL, 0, NULL, NULL);
                if(len > 0) {
                    zfstring tmp;
                    tmp.capacity(len + 1);
                    WideCharToMultiByte(CP_UTF8, 0, wszLocale, -1, tmp.zfunsafe_buffer(), len, NULL, NULL);
                    tmp.zfunsafe_length(len);
                    tmp.zfunsafe_buffer()[len] = '\0';
                    zfindex pos = zfstringFind(tmp, "-");
                    if(pos != zfindexMax()) {
                        zfstringToUpperT(ret, tmp + pos + 1);
                    }
                }
            }
        #elif defined(__APPLE__) && defined(__MACH__)
            // zh-Hans-CN
            FILE *fp = popen("defaults read -g AppleLanguages | head -n2 | tail -n1 | tr -d ' \t\",'", "r");
            if(fp) {
                zfchar buf[64] = {0};
                if(fgets(buf, sizeof(buf), fp)) {
                    zfindex pos = zfstringFindReversely(buf, "-");
                    if(pos != zfindexMax()) {
                        zfindex posEnd = zfslen(buf);
                        while(posEnd != zfindexMax() && (buf[posEnd - 1] == '\r' || buf[posEnd - 1] == '\n')) {
                            --posEnd;
                        }
                        if(posEnd != zfindexMax()) {
                            zfstringToUpperT(ret, buf + pos + 1, posEnd - (pos + 1));
                        }
                    }
                }
                pclose(fp);
            }
        #else // #if ZF_ENV_sys_Windows
            // en_US.UTF-8
            const zfchar *env = getenv("LANG");
            if(!zfstringIsEmpty(env)) {
                zfindex dotPos = zfstringFind(env, ".");
                zfindex sepPos = zfstringFind(env, "_");
                if(dotPos != zfindexMax() && sepPos != zfindexMax() && dotPos > sepPos) {
                    zfstringToUpperT(ret, env + sepPos + 1, dotPos - (sepPos + 1));
                }
            }
        #endif // #if ZF_ENV_sys_Windows
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeInfoImpl_default)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeLangInfoImpl_default, ZFEnvInfo_localeLangInfo, v_ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual void localeLangInfo(ZF_IN_OUT zfstring &ret) {
        #if ZF_ENV_sys_Windows
            WCHAR wszLocale[LOCALE_NAME_MAX_LENGTH] = {0};
            if(GetUserDefaultLocaleName(wszLocale, LOCALE_NAME_MAX_LENGTH)) {
                int len = WideCharToMultiByte(CP_UTF8, 0, wszLocale, -1, NULL, 0, NULL, NULL);
                if(len > 0) {
                    zfstring tmp;
                    tmp.capacity(len + 1);
                    WideCharToMultiByte(CP_UTF8, 0, wszLocale, -1, tmp.zfunsafe_buffer(), len, NULL, NULL);
                    tmp.zfunsafe_length(len);
                    tmp.zfunsafe_buffer()[len] = '\0';
                    zfindex pos = zfstringFind(tmp, "-");
                    if(pos != zfindexMax()) {
                        zfstringToLowerT(ret, tmp, pos);
                    }
                }
            }
        #elif defined(__APPLE__) && defined(__MACH__)
            // zh-Hans-CN
            FILE *fp = popen("defaults read -g AppleLanguages | head -n2 | tail -n1 | tr -d ' \t\",'", "r");
            if(fp) {
                zfchar buf[64] = {0};
                if(fgets(buf, sizeof(buf), fp)) {
                    zfindex pos = zfstringFind(buf, "-");
                    if(pos != zfindexMax()) {
                        zfstringToLowerT(ret, buf, pos);
                    }
                }
                pclose(fp);
            }
        #else // #if ZF_ENV_sys_Windows
            // en_US.UTF-8
            const zfchar *env = getenv("LANG");
            if(!zfstringIsEmpty(env)) {
                zfindex dotPos = zfstringFind(env, ".");
                zfindex sepPos = zfstringFind(env, "_");
                if(dotPos != zfindexMax() && sepPos != zfindexMax() && dotPos > sepPos) {
                    zfstringToLowerT(ret, env, sepPos);
                }
            }
        #endif // #if ZF_ENV_sys_Windows
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeLangInfoImpl_default)

ZF_NAMESPACE_GLOBAL_END


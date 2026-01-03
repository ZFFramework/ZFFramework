#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFEnvInfo_ZFUIKit.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeInfoImpl_sys_iOS, ZFEnvInfo_localeInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void localeInfo(ZF_IN_OUT zfstring &ret) {
        NSString *info = [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
        if(info != nil) {
            ZFImpl_sys_iOS_zfstringFromNSString(ret, info);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeInfoImpl_sys_iOS)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEnvInfo_localeLangInfoImpl_sys_iOS, ZFEnvInfo_localeLangInfo, v_ZFProtocolLevel::e_SystemNormal)
public:
    zfoverride
    virtual void localeLangInfo(ZF_IN_OUT zfstring &ret) {
        NSString *info = [[NSLocale preferredLanguages].firstObject componentsSeparatedByString:@"-"].firstObject;
        if(info != nil) {
            ZFImpl_sys_iOS_zfstringFromNSString(ret, info);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEnvInfo_localeLangInfoImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


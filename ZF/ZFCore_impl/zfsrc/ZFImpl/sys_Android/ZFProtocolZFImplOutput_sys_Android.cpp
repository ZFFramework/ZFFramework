#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFImplOutput.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLog_sys_Android, ZFLevelZFFrameworkEssential) {
    // Android's log already has time
    ZFLogHeaderDefault_logTime(zffalse);
}
ZF_GLOBAL_INITIALIZER_END(ZFLog_sys_Android)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFImplOutputImpl_sys_Android, ZFImplOutput, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Logcat")
public:
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        if(this->savedString.length() > 0) {
            this->doOutput(this->savedString, 0, this->savedString.length());
        }
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void outputCoreLog(ZF_IN const zfchar *s) {
        zfstring tmp = s;
        zfsynchronize(this->syncObj);
        this->checkOutput(tmp);
    }
    virtual void outputLog(
            ZF_IN const zfchar *s
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) {
        zfsynchronize(this->syncObj);
        this->savedString.append(s, count);
        this->checkOutput(this->savedString);
    }
private:
    zfobj<ZFObject> syncObj;
    zfstring savedString;
    void checkOutput(ZF_IN_OUT zfstring &s) {
        if(s.isEmpty()) {
            return;
        }

        zfindex pL = 0;
        do {
            zfindex p = zfstringFind(s + pL, s.length() - pL, "\n");
            if(p == zfindexMax()) {
                break;
            }
            s.set(pL + p, '\0');
            this->doOutput(s, pL, pL + p);
            pL = pL + p + 1;
        } while(zftrue);
        if(pL != 0) {
            s.remove(0, pL);
        }
    }
    void doOutput(ZF_IN_OUT zfstring &s, ZF_IN zfindex pL, ZF_IN zfindex pR) {
        static const zfindex maxLen = 1000;
        if(pR - pL > maxLen) {
            do {
                zfindex p = pL + maxLen;
                zfchar c = s[p];
                s.set(p, '\0');
                this->implOutput(s + pL);
                pL += maxLen;
                s.set(p, c);
            } while(pR - pL > maxLen);
            if(pR > pL) {
                this->implOutput(s + pL);
            }
        }
        else {
            this->implOutput(s + pL);
        }
    }
    inline void implOutput(ZF_IN const zfchar *s) {
        AndroidLogDetail(AndroidLogLevelE, AndroidLogTagDefault(), "%s", s);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFImplOutputImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android


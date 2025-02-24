#include "ZFLua_test.h"
#include "ZFUIWidget.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_app_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFLua_app_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFLua);
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        zfauto luaResult = ZFLuaExecute(ZFInputForRes("test_ZFLua_app/main.lua"));
        ZFUIPageManager *pm = luaResult;
        ZFCoreAssert(pm != zfnull);
        pm->observerAdd(
            ZFUIPageManager::E_ManagerOnDestroy(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, managerOnDestroy)));
    }

private:
    ZFMETHOD_INLINE_1(void, managerOnDestroy
            , ZFMP_IN(const ZFArgs &, zfargs)
            ) {
        this->stop(ZFResultType::e_Success);
        ZFLuaGC();
    }
};
ZFOBJECT_REGISTER(ZFLua_app_test)

ZF_NAMESPACE_GLOBAL_END


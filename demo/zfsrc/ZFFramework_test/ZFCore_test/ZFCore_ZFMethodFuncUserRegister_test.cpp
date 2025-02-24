#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFMethodFuncUserRegister_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFMethodFuncUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        {
            this->outputSeparator();
            this->output("try execute dynamically registered method");

            ZFMethodFuncUserRegisterDetail_1(method, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_0(method0, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic0"
                );

            ZFMethodFuncUserRegisterDetail_1(method1, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic1"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_2(method2, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic2"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                );

            ZFMethodFuncUserRegisterDetail_3(method3, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic3"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                );

            ZFMethodFuncUserRegisterDetail_4(method4, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic4"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                );

            ZFMethodFuncUserRegisterDetail_5(method5, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic5"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                );

            ZFMethodFuncUserRegisterDetail_6(method6, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic6"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                );

            ZFMethodFuncUserRegisterDetail_7(method7, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic7"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                );

            ZFMethodFuncUserRegisterDetail_8(method8, {
                    ZFLog();
                    return "returnValue";
                }, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic8"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                , ZFMP_IN(zfstring const &, param7)
                );

            zfstring ret = ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic")
                ->executeExact<zfstring, zfstring const &>(this, "paramValue");
            this->output(zfstr("execute result: %s", ret));

            ZFMethodFuncUserUnregister(ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic"));

            ZFLogTrim() << "after unregister, method: "
                << ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic");

            this->output("all list:");
            for(zfindex i = 0; i <= 8; ++i) {
                this->output(zfstr("  %s",
                            ZFMethodForName(
                                "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                                zfstr("funcDynamic%s", i)
                                )->objectInfo()
                                ));
            }

            for(zfindex i = 0; i <= 8; ++i) {
                ZFMethodFuncUserUnregister(ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace",
                        zfstr("funcDynamic%s", i))
                    );
            }
        }

        {
            this->outputSeparator();
            this->output("try execute statically registered method");

            zfstring ret = ZFMethodForName("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcStatic")
                ->executeExact<zfstring, zfstring const &>(this, "paramValue");
            this->output(zfstr("execute result: %s", ret));

            this->output("all list:");
            for(zfindex i = 0; i <= ZFMETHOD_MAX_PARAM; ++i) {
                this->output(zfstr("  %s",
                            ZFMethodForName(
                                "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                                zfstr("funcStatic%s", i)
                                )->objectInfo()
                                ));
            }
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodFuncUserRegister_test)

ZF_NAMESPACE_BEGIN_REGISTER(ZFCore_ZFMethodFuncUserRegister_testNamespace, ZF_NAMESPACE_GLOBAL)

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_0({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic0
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic1
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_2({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic2
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_3({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic3
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_4({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic4
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_5({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic5
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_6({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic6
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_7({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic7
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_8({
        ZFLog();
        return "returnValue";
    }, ZF_NAMESPACE_CURRENT(), ZF_CALLER_LINE,
    zfstring, funcStatic8
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    , ZFMP_IN(zfstring const &, param7)
    )

ZF_NAMESPACE_END(ZFCore_ZFMethodFuncUserRegister_testNamespace)

ZF_NAMESPACE_GLOBAL_END


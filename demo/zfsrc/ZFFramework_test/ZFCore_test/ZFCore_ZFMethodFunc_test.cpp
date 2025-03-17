#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(void, ZFCore_ZFMethodFunc_test_func) {
    ZFLog();
}

ZF_NAMESPACE_BEGIN_REGISTER(ZFCore_ZFMethodFunc_test_namespace, ZF_NAMESPACE_GLOBAL)
ZFMETHOD_FUNC_DEFINE_0(void, ZFCore_ZFMethodFunc_test_func) {
    ZFLog();
}
ZF_NAMESPACE_END(ZFCore_ZFMethodFunc_test_namespace)

ZFMETHOD_FUNC_DEFINE_0(zfstring, ZFCore_ZFMethodFunc_test_func0) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFCore_ZFMethodFunc_test_func1
        , ZFMP_IN(zfstring, param0)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFCore_ZFMethodFunc_test_func2
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_3(zfstring, ZFCore_ZFMethodFunc_test_func3
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_4(zfstring, ZFCore_ZFMethodFunc_test_func4
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        , ZFMP_IN(zfstring, param3)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_5(zfstring, ZFCore_ZFMethodFunc_test_func5
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        , ZFMP_IN(zfstring, param3)
        , ZFMP_IN(zfstring, param4)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_6(zfstring, ZFCore_ZFMethodFunc_test_func6
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        , ZFMP_IN(zfstring, param3)
        , ZFMP_IN(zfstring, param4)
        , ZFMP_IN(zfstring, param5)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_7(zfstring, ZFCore_ZFMethodFunc_test_func7
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        , ZFMP_IN(zfstring, param3)
        , ZFMP_IN(zfstring, param4)
        , ZFMP_IN(zfstring, param5)
        , ZFMP_IN(zfstring, param6)
        ) {
    ZFLog();
    return "ReturnValue";
}

ZFMETHOD_FUNC_DEFINE_8(zfstring, ZFCore_ZFMethodFunc_test_func8
        , ZFMP_IN(zfstring, param0)
        , ZFMP_IN(zfstring, param1)
        , ZFMP_IN(zfstring, param2)
        , ZFMP_IN(zfstring, param3)
        , ZFMP_IN(zfstring, param4)
        , ZFMP_IN(zfstring, param5)
        , ZFMP_IN(zfstring, param6)
        , ZFMP_IN(zfstring, param7)
        ) {
    ZFLog();
    return "ReturnValue";
}

// ============================================================
zfclass ZFCore_ZFMethodFunc_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFMethodFunc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        ZFCoreArray<const ZFMethod *> methods;
        ZFMethodFuncGetAllT(methods);
        const zfchar *prefix = "ZFCore_ZFMethodFunc_test_";
        const zfindex prefixLen = zfslen(prefix);
        for(zfindex i = methods.count() - 1; i != zfindexMax(); --i) {
            if(zfsncmp(methods[i]->methodName(), prefix, prefixLen) != 0) {
                methods.remove(i);
            }
        }

        this->output(methods.objectInfoOfContent(
                zfindexMax(),
                ZFTokenForContainerPlainList()
            ));

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodFunc_test)

ZF_NAMESPACE_GLOBAL_END


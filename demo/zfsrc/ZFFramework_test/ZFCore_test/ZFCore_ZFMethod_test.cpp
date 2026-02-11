#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_ZFCore_ZFMethod_test_TestBase : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFMethod_test_TestBase, ZFObject)
public:
    zfstring s;

public:
    ZFMETHOD_INLINE_0(void, methodNormal) {
        ZFLog();
    }
    ZFMETHOD_DECLARE_0(void, methodDefinedInCppFile)
    ZFMETHOD_INLINE_0(void, methodOverrided) {
        ZFLog();
    }
    ZFMETHOD_INLINE_DETAIL_0(protected, ZFMethodTypeVirtual, ZF_CALLER_LINE,
            void, methodProtected
            ) {
        ZFLog();
    }
    ZFMETHOD_INLINE_DETAIL_0(private, ZFMethodTypeVirtual, ZF_CALLER_LINE,
            void, methodPrivate
            ) {
        ZFLog();
    }
    ZFMETHOD_INLINE_DETAIL_0(public, ZFMethodTypeStatic, ZF_CALLER_LINE,
            void, methodStatic
            ) {
        ZFLog();
    }
    ZFMETHOD_INLINE_1(void, methodOverloaded
            , ZFMP_IN(zfint, param0)
            ) {
        ZFLog() << param0;
    }
    ZFMETHOD_INLINE_1(void, methodOverloaded
            , ZFMP_IN(zffloat, param0)
            ) {
        ZFLog() << param0;
    }
    ZFMETHOD_INLINE_1(void, methodForDefaultParam
            , ZFMP_IN_OPT(const zfstring &, param0, "DefaultParam")
            ) {
        ZFLog() << param0;
    }
    ZFMETHOD_INLINE_0(zfstring &, returnReference) {
        ZFLog();
        return this->s;
    }
    ZFMETHOD_INLINE_1(void, methodParamIsReference
            , ZFMP_IN(zfstring &, param0)
            ) {
        ZFLog();
        param0 = "newString";
    }
    ZFMETHOD_INLINE_2(void, methodFor2Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            ) {
        ZFLog() << param0 << " " << param1;
    }
    ZFMETHOD_INLINE_3(void, methodFor3Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2;
    }
    ZFMETHOD_INLINE_4(void, methodFor4Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            , ZFMP_IN(const zfchar *, param3)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2 << " " << param3;
    }
    ZFMETHOD_INLINE_5(void, methodFor5Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            , ZFMP_IN(const zfchar *, param3)
            , ZFMP_IN(const zfchar *, param4)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2 << " " << param3 << " " << param4;
    }
    ZFMETHOD_INLINE_6(void, methodFor6Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            , ZFMP_IN(const zfchar *, param3)
            , ZFMP_IN(const zfchar *, param4)
            , ZFMP_IN(const zfchar *, param5)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2 << " " << param3 << " " << param4 << " " << param5;
    }
    ZFMETHOD_INLINE_7(void, methodFor7Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            , ZFMP_IN(const zfchar *, param3)
            , ZFMP_IN(const zfchar *, param4)
            , ZFMP_IN(const zfchar *, param5)
            , ZFMP_IN(const zfchar *, param6)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2 << " " << param3 << " " << param4 << " " << param5 << " " << param6;
    }
    ZFMETHOD_INLINE_8(void, methodFor8Param
            , ZFMP_IN(const zfchar *, param0)
            , ZFMP_IN(const zfchar *, param1)
            , ZFMP_IN(const zfchar *, param2)
            , ZFMP_IN(const zfchar *, param3)
            , ZFMP_IN(const zfchar *, param4)
            , ZFMP_IN(const zfchar *, param5)
            , ZFMP_IN(const zfchar *, param6)
            , ZFMP_IN(const zfchar *, param7)
            ) {
        ZFLog() << param0 << " " << param1 << " " << param2 << " " << param3 << " " << param4 << " " << param5 << " " << param6 << " " << param7;
    }
};
#if 0 // if opened, all method should be registered statically
    ZFOBJECT_REGISTER(_ZFP_ZFCore_ZFMethod_test_TestBase)
#endif
ZFMETHOD_DEFINE_0(_ZFP_ZFCore_ZFMethod_test_TestBase, void, methodDefinedInCppFile) {
    ZFLog();
}

// ============================================================
zfclass _ZFP_ZFCore_ZFMethod_test_TestChild : zfextend _ZFP_ZFCore_ZFMethod_test_TestBase {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFMethod_test_TestChild, _ZFP_ZFCore_ZFMethod_test_TestBase)

public:
    ZFMETHOD_INLINE_0(void, methodOverrided) {
        zfsuper::methodOverrided();
        ZFLog();
    }

    ZFMETHOD_INLINE_0(void, methodInChild) {
        ZFLog();
    }
};
#if 0 // if opened, all method should be registered statically
    ZFOBJECT_REGISTER(_ZFP_ZFCore_ZFMethod_test_TestChild)
#endif

// ============================================================
zfclass ZFCore_ZFMethod_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFMethod_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        const ZFClass *clsBase = _ZFP_ZFCore_ZFMethod_test_TestBase::ClassData();
        const ZFClass *clsChild = _ZFP_ZFCore_ZFMethod_test_TestChild::ClassData();

        // ============================================================
        this->outputSeparator();
        this->output("ZFMethod registration");

        this->outputSeparator();
        this->output("method registered before any of instances created:");

        this->outputSeparator();
        this->output("Base:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestBase::ClassData());
        this->output("Child:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestChild::ClassData());

        this->outputSeparator();

        this->prepareInstance();
        this->output("method registered after any of instances created:");
        this->outputSeparator();
        this->output("Base:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestBase::ClassData());
        this->output("Child:");
        this->printMethodInfo(_ZFP_ZFCore_ZFMethod_test_TestChild::ClassData());

        // ============================================================
        this->outputSeparator();
        this->output("ZFMethod directly access");
        this->output(zfstr("methodNormal: %s"
                    , ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodNormal)
                    ));
        this->output(zfstr("methodStatic: %s"
                    , ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodStatic)
                    ));
        this->output(zfstr("methodOverloaded(zfint): %s"
                    , ZFMethodAccessDetail_1(_ZFP_ZFCore_ZFMethod_test_TestBase, methodOverloaded
                        , ZFMP_IN(zfint, param0)
                        )
                        ));
        this->output(zfstr("methodOverloaded(zffloat): %s"
                    , ZFMethodAccessDetail_1(_ZFP_ZFCore_ZFMethod_test_TestBase, methodOverloaded
                        , ZFMP_IN(zffloat, param0)
                        )
                        ));

        this->output("unlike reflected by ZFClass, ZFMethodAccess would comiple error if method not defined");
#if 0 // this should not able to be compiled
        ZFMethodAccess(_ZFP_ZFCore_ZFMethod_test_TestBase, methodNotDefined);
#endif

        // ============================================================
        this->outputSeparator();
        this->output("ZFMethod reflection");
        this->output("call method normally:");
        this->pBase->methodNormal();
        this->pChild->methodNormal();
        this->pBasePointToChild->methodNormal();

        this->outputSeparator();
        this->output("call by reflect:");
        clsBase->methodForNameIgnoreParent("methodNormal")->executeExact<void>(this->pBase);
        clsBase->methodForNameIgnoreParent("methodNormal")->executeExact<void>(this->pChild);
        clsBase->methodForNameIgnoreParent("methodNormal")->executeExact<void>(this->pBasePointToChild);

        this->output(zfstr("should be null since it's in base only: %s"
                    , clsChild->methodForNameIgnoreParent("methodNormal")
                    ));
        this->output(zfstr("should not be null since we search hierarchically: %s"
                    , clsChild->methodForName("methodNormal")
                    ));
        this->output(zfstr("should not be null since it's in child only: %s"
                    , clsChild->methodForNameIgnoreParent("methodInChild")
                    ));

        clsChild->methodForName("methodNormal")->executeExact<void>(this->pBase);
        clsChild->methodForName("methodNormal")->executeExact<void>(this->pChild);
        clsChild->methodForName("methodNormal")->executeExact<void>(this->pBasePointToChild);

        // ============================================================
        this->outputSeparator();
        this->output("ZFMethod overloaded method");

        clsBase->methodForNameIgnoreParent("methodOverloaded", "zfint")->executeExact<void, zfint>(this->pBase, 123);
        clsBase->methodForNameIgnoreParent("methodOverloaded", "zffloat")->executeExact<void, zffloat>(this->pBase, 123.456f);

        // ============================================================
        this->outputSeparator();
        this->output("ZFMethod with default param");

        this->pBase->methodForDefaultParam();
        clsBase->methodForNameIgnoreParent("methodForDefaultParam")->executeExact<void, const zfstring &>(this->pBase,
            "default value not available when reflect");

        // ============================================================
        this->outputSeparator();
        this->output("return reference:");
        this->pBase->s = "oldString";
        this->output(zfstr("before: %s", this->pBase->s));
        clsBase->methodForNameIgnoreParent("returnReference")->executeExact<zfstring &>(this->pBase) = "newString";
        this->output(zfstr("after: %s", this->pBase->s));

        this->outputSeparator();

        zfstring _paramReference;
        zfstring &paramReference = _paramReference;
        this->output("param is reference:");
        paramReference = "oldString";
        this->output(zfstr("before: %s", paramReference));
        clsBase->methodForNameIgnoreParent("methodParamIsReference")->executeExact<void, zfstring &>(this->pBase, paramReference);
        this->output(zfstr("after: %s", paramReference));

        this->outputSeparator();
        this->output("note that you must specify both ReturnType and ParamType, ");
        this->output("if not, no compiler would occurred, but app may crash at runtime");
#if 0
        // invoke method without explicitly specifying the param types
        // this code may or may not crash
        paramReference = "oldString";
        this->output("these code may have wrong result or crash:");
        this->output(zfstr("before: %s", paramReference));
        clsBase->methodForNameIgnoreParent("methodParamIsReference")->executeExact<void>(this->pBase, paramReference);
        this->output(zfstr("after: %s", paramReference));
#endif

        this->stop();
    }

private:
    zfautoT<_ZFP_ZFCore_ZFMethod_test_TestBase> pBase;
    zfautoT<_ZFP_ZFCore_ZFMethod_test_TestChild> pChild;
    _ZFP_ZFCore_ZFMethod_test_TestBase *pBasePointToChild;
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();

        this->pBase = zfnull;
        this->pChild = zfnull;
        this->pBasePointToChild = zfnull;
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        this->destroyInstance();
        zfsuper::objectOnDealloc();
    }
private:
    void prepareInstance(void) {
        this->destroyInstance();

        this->pBase = zfobj<_ZFP_ZFCore_ZFMethod_test_TestBase>();
        this->pChild = zfobj<_ZFP_ZFCore_ZFMethod_test_TestChild>();
        this->pBasePointToChild = this->pChild;
    }
    void destroyInstance(void) {
        this->pBase = zfnull;
        this->pChild = zfnull;

        this->pBase = zfnull;
        this->pChild = zfnull;
        this->pBasePointToChild = zfnull;
    }
    void printMethodInfo(const ZFClass *cls, const zfchar *indent = "  ") {
        for(zfiter it = cls->methodIter(); it; ++it) {
            this->output(zfstr("%s%s", indent, cls->methodIterValue(it)));
        }
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethod_test)

ZF_NAMESPACE_GLOBAL_END


#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFCore_ZFPropertyDynamic_test_Object : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFPropertyDynamic_test_Object, ZFObject)

public:
    zfint myProp(void) const {
        return _ZFP_myProp.zfv;
    }
    void myProp(ZF_IN zfint v) {
        _ZFP_myProp.zfv = v;
    }
private:
    zftValue<zfint> _ZFP_myProp;
};

// ============================================================
zfclass ZFCore_ZFPropertyDynamic_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFPropertyDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        const ZFClass *cls = _ZFP_ZFCore_ZFPropertyDynamic_test_Object::ClassData();

        const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .ownerClass(cls)
                .methodGenericInvoker(_setterGI)
                .methodType(ZFMethodTypeNormal)
                .methodName("myProp")
                .methodParam(ZFTypeId_zfint())
            );
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .ownerClass(cls)
                .methodGenericInvoker(_getterGI)
                .methodType(ZFMethodTypeNormal)
                .methodName("myProp")
                .returnTypeId(ZFTypeId_zfint())
            );

        const ZFProperty *property = ZFPropertyDynamicRegister(ZFPropertyDynamicRegisterParam()
                .ownerClass(cls)
                .propertyTypeId(ZFTypeId_zfint())
                .propertyName("myProp")
                .propertyCustomImpl(setterMethod, getterMethod, _callbackIsValueAccessed, _callbackIsInitValue, _callbackValueReset)
            );

        this->output(zfstr("property: %s", property));

        zfobj<_ZFP_ZFCore_ZFPropertyDynamic_test_Object> obj;
        obj->myProp(123);
        this->output(zfstr("obj: %s", obj));

        ZFPropertyDynamicUnregister(property);
        ZFMethodDynamicUnregister(setterMethod);
        ZFMethodDynamicUnregister(getterMethod);
        this->stop();
    }
private:
    static const zfchar *_valueKey(void) {
        return "ZFCore_ZFPropertyDynamic_test_myProp";
    }
    static void _setterGI(ZF_IN_OUT const ZFArgs &zfargs) {
        if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
            return;
        }
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = zfargs.sender();
        obj->myProp(zfargs.param0()->to<v_zfint *>()->zfv);
        obj->objectTag(_valueKey(), zfargs.param0()->to<ZFCopyable *>()->copy());
    }
    static void _getterGI(ZF_IN_OUT const ZFArgs &zfargs) {
        if(!ZFMethodGenericInvokerParamsCheck(zfargs)) {
            return;
        }
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = zfargs.sender();
        zfauto tag = zfobj<v_zfint>(obj->myProp());
        obj->objectTag(_valueKey(), tag);
        zfargs.result(tag);
    }
    static zfbool _callbackIsValueAccessed(
            ZF_IN const ZFProperty *property
            , ZF_IN zfany const &ownerObj
            ) {
        return !_callbackIsInitValue(property, ownerObj);
    }
    static zfbool _callbackIsInitValue(
            ZF_IN const ZFProperty *property
            , ZF_IN zfany const &ownerObj
            , ZF_OUT_OPT zfauto *outInitValue = zfnull
            ) {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj;
        if(outInitValue != zfnull) {
            *outInitValue = zfobj<v_zfint>();
        }
        return (obj->myProp() == 0);
    }
    static void _callbackValueReset(
            ZF_IN const ZFProperty *property
            , ZF_IN zfany const &ownerObj
            ) {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj;
        obj->objectTagRemove(_valueKey());
        obj->myProp(0);
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPropertyDynamic_test)

ZF_NAMESPACE_GLOBAL_END


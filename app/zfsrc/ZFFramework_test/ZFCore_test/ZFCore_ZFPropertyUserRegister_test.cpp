#include "ZFCore_test.h"

#include "ZFAlgorithm/ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_ZFCore_ZFPropertyUserRegister_test_Object : zfextend ZFObject, zfimplement ZFSerializable {
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable)
public:
    ZFPROPERTY_ASSIGN(zfindex, valueNormal)
};
zfclass ZFCore_ZFPropertyUserRegister_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFPropertyUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        {
            zfobj<_ZFP_ZFCore_ZFPropertyUserRegister_test_Object> obj;
            obj->valueNormal(1);

            this->outputSeparator();
            this->output("try modify and serialize a dynamically registered property, result");

            ZFPropertyUserRegisterAssign(propertyInfo, _ZFP_ZFCore_ZFPropertyUserRegister_test_Object::ClassData(),
                zfstring, "valueDynamic", ZFPropertyNoInitValue,
                public, public);
            obj->classData()->propertyForName("valueDynamic")->setterMethod()
                ->executeExact<void, zfstring const &>(obj, "value");
            ZFObjectToXml(ZFOutputDefault(), obj);
            ZFPropertyUserUnregister(obj->classData()->propertyForName("valueDynamic"));

            this->output("after unregister:");
            ZFObjectToXml(ZFOutputDefault(), obj);
        }

        {
            zfobj<_ZFP_ZFCore_ZFPropertyUserRegister_test_Object> obj;
            obj->valueNormal(1);

            this->outputSeparator();
            this->output("try modify and serialize a statically registered property, result");

            obj->classData()->propertyForName("valueStatic")->setterMethod()
                ->executeExact<void, v_zfstring * const &>(obj, zfobj<v_zfstring>("value"));
            ZFObjectToXml(ZFOutputDefault(), obj);
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPropertyUserRegister_test)

ZFPROPERTY_USER_REGISTER_RETAIN(_ZFP_ZFCore_ZFPropertyUserRegister_test_Object
        , v_zfstring *, valueStatic, ZFPropertyNoInitValue
        , public, public
        )

ZF_NAMESPACE_GLOBAL_END


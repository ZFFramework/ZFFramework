#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// in header
ZF_NAMESPACE_BEGIN_REGISTER(ZFCore_inner_type_test_NS, ZF_NAMESPACE_GLOBAL)

zfclassNotPOD TestType {};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_APP, TestType, TestType)

ZFENUM_BEGIN(ZFLIB_APP, TestEnum)
    ZFENUM_VALUE(T0)
    ZFENUM_VALUE(T1)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(T0)
    ZFENUM_VALUE_REGISTER(T1)
ZFENUM_END_FLAGS(ZFLIB_APP, TestEnum, TestEnumFlags)

zfclass OuterClass : zfextend ZFObject {
    ZFOBJECT_DECLARE(OuterClass, ZFObject)

    zfclassNotPOD TestType {};
    ZFTYPEID_INNER_ACCESS_ONLY_DECLARE(OuterClass, TestType, TestType)

    ZFENUM_INNER_BEGIN(OuterClass, TestEnum)
        ZFENUM_INNER_VALUE(T0)
        ZFENUM_INNER_VALUE(T1)
    ZFENUM_INNER_SEPARATOR()
        ZFENUM_INNER_VALUE_REGISTER(T0)
        ZFENUM_INNER_VALUE_REGISTER(T1)
    ZFENUM_INNER_END_FLAGS(OuterClass, TestEnum, TestEnumFlags)
};

ZF_NAMESPACE_END(ZFCore_inner_type_test_NS)

// ============================================================
// reg in global scope
ZFTYPEID_REG(ZFLIB_APP, TestType, TestType, ZFCore_inner_type_test_NS)
ZFOUTPUT_TYPE(ZFCore_inner_type_test_NS::TestType, {zfsFromPointerT(s, &v);})
ZFENUM_REG_FLAGS(ZFLIB_APP, TestEnum, TestEnumFlags, ZFCore_inner_type_test_NS)
ZFTYPEID_INNER_REG(ZFCore_inner_type_test_NS::OuterClass, TestType, TestType)
ZFOUTPUT_TYPE(ZFCore_inner_type_test_NS::OuterClass::TestType, {zfsFromPointerT(s, &v);})
ZFENUM_INNER_REG_FLAGS(ZFCore_inner_type_test_NS::OuterClass, TestEnum, TestEnumFlags)

// ============================================================
// in source
ZF_NAMESPACE_BEGIN(ZFCore_inner_type_test_NS)

ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(TestType, TestType)
ZFENUM_DEFINE_FLAGS(TestEnum, TestEnumFlags)
ZFTYPEID_INNER_ACCESS_ONLY_DEFINE_UNCOMPARABLE(OuterClass, TestType, TestType)
ZFENUM_INNER_DEFINE_FLAGS(OuterClass, TestEnum, TestEnumFlags)

ZF_NAMESPACE_END(ZFCore_inner_type_test_NS)

// ============================================================
// test
zfclass ZFCore_inner_type_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_inner_type_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        {
            zfobj<ZFCore_inner_type_test_NS::v_TestType> v;
            ZFLogTrim("raw type in NS: %s", v);
        }

        {
            zfobj<ZFCore_inner_type_test_NS::v_TestEnum> v(ZFCore_inner_type_test_NS::v_TestEnum::e_T0);
            ZFLogTrim("enum in NS: %s", v);
        }

        {
            zfobj<ZFCore_inner_type_test_NS::OuterClass::v_TestType> v;
            ZFLogTrim("raw type as inner class: %s", v);
        }

        {
            zfobj<ZFCore_inner_type_test_NS::OuterClass::v_TestEnum> v(ZFCore_inner_type_test_NS::OuterClass::v_TestEnum::e_T0);
            ZFLogTrim("enum as inner class: %s", v);
        }

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_inner_type_test)

ZF_NAMESPACE_GLOBAL_END


#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZFCore_ZFTypeId_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFCore_ZFTypeId_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();

        zfauto v;

        zfobj<v_zfstring> test_zfstring("string type");
        v = test_zfstring;

        // for aliased type, when accessed, a new instance would be created and stored as tag in holder object
        typedef const zfchar * _ZFP_AliasedChar;
        zfLogTrim() << "============================================================";
        zfLogTrim() << "const zfchar *";
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * const &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * const &>::zfvAccess(v);

        // for wrapped type, all should be able to access
        zfLogTrim() << "============================================================";
        zfLogTrim() << "zfstring";
        zfLogTrim() << ZFTypeId<zfstring>::Value<zfstring>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<const zfstring &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<zfstring &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<zfstring *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<const zfstring *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<zfstring * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<zfstring * const &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<const zfstring * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfstring>::Value<const zfstring * const &>::zfvAccess(v);

        // zfauto processed as normal raw type
        zfobj<v_zfstring> test_String("zfauto");
        v = test_String;
        zfLogTrim() << "============================================================";
        zfLogTrim() << "zfauto";
        zfLogTrim() << ZFTypeId<zfauto>::Value<zfauto>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<const zfauto &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<zfauto &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<zfauto *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<const zfauto *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<zfauto * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<zfauto * const &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * const &>::zfvAccess(v);

        // ZFAny can only accessed as value or const ref
        zfobj<v_zfstring> test_ZFAny("ZFAny");
        v = test_ZFAny;
        zfLogTrim() << "============================================================";
        zfLogTrim() << "ZFAny";
        zfLogTrim() << ZFTypeId<ZFAny>::Value<ZFAny>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<const ZFAny &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<ZFAny &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<ZFAny *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<const ZFAny *>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<ZFAny * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<ZFAny * const &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<const ZFAny * &>::zfvAccess(v);
        zfLogTrim() << ZFTypeId<ZFAny>::Value<const ZFAny * const &>::zfvAccess(v);

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTypeId_test)

ZF_NAMESPACE_GLOBAL_END


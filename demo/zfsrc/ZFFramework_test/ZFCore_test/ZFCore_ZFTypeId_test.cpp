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
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "const zfchar *";
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar *>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar *>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * const &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * const &>::zfvAccess(v);

        // for wrapped type, all should be able to access
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "zfstring";
        ZFLogTrim() << ZFTypeId<zfstring>::Value<zfstring>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<const zfstring &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<zfstring &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<zfstring *>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<const zfstring *>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<zfstring * &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<zfstring * const &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<const zfstring * &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfstring>::Value<const zfstring * const &>::zfvAccess(v);

        // zfwrap can be accessed as modifiable value
        zfobj<v_zfstring> test_zfwrap("zfwrap");
        v = test_zfwrap;
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "zfwrap";
        ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto &>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto *>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto *>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto * const &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * const &>::zfvAccess(v);

        // zfauto can only accessed as value or const ref
        zfobj<v_zfstring> test_zfauto("zfauto");
        v = test_zfauto;
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "zfauto";
        ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto *>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto *>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<zfauto * const &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfauto>::Value<const zfauto * const &>::zfvAccess(v);

        // zfany can only accessed as value or const ref
        zfobj<v_zfstring> test_zfany("zfany");
        v = test_zfany;
        ZFLogTrim() << "============================================================";
        ZFLogTrim() << "zfany";
        ZFLogTrim() << ZFTypeId<zfany>::Value<zfany>::zfvAccess(v);
        ZFLogTrim() << ZFTypeId<zfany>::Value<const zfany &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<zfany &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<zfany *>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<const zfany *>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<zfany * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<zfany * const &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<const zfany * &>::zfvAccess(v);
        // ZFLogTrim() << ZFTypeId<zfany>::Value<const zfany * const &>::zfvAccess(v);

        this->stop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTypeId_test)

ZF_NAMESPACE_GLOBAL_END


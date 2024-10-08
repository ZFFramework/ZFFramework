/**
 * @file ZFNamespaceImpl.h
 * @brief namespace impl
 */
#ifndef _ZFI_ZFNamespaceImpl_h_
#define _ZFI_ZFNamespaceImpl_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreArray.h"
#include "ZFCoreStaticRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief register namespace
 */
#define ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    _ZFP_ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace)
#define _ZFP_ZF_NAMESPACE_REGISTER(NameSpace, ParentNameSpace) \
    const zfchar *_ZFP_ZF_NAMESPACE_NOT_REGISTERED(void) { \
        static _ZFP_ZFNamespaceHolder d(ParentNameSpace::_ZFP_ZF_NAMESPACE_NOT_REGISTERED(), #NameSpace); \
        return d.ns; \
    } \
    ZF_STATIC_REGISTER_INIT(_ZFP_NSReg) { \
        _ZFP_ZF_NAMESPACE_NOT_REGISTERED(); \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_NSReg)
extern ZFLIB_ZFCore zfstring _ZFP_ZFNamespaceRegister(
        ZF_IN const zfchar *parent
        , ZF_IN const zfchar *child
        );
extern ZFLIB_ZFCore void _ZFP_ZFNamespaceUnregister(ZF_IN const zfchar *ns);
class _ZFP_ZFNamespaceHolder {
public:
    _ZFP_ZFNamespaceHolder(
            ZF_IN const zfchar *parent
            , ZF_IN const zfchar *child
            )
    : ns(_ZFP_ZFNamespaceRegister(parent, child))
    {
    }
    ~_ZFP_ZFNamespaceHolder(void) {
        _ZFP_ZFNamespaceUnregister(this->ns);
    }
public:
    zfstring ns;
};

/**
 * @brief get current namespace name
 *
 * note:
 * -  global namespace and ZFFramework's main namespace (#ZF_NAMESPACE_GLOBAL) are ensured null
 * -  embeded namespace are supported,
 *   however, you must ensure it's valid declared by #ZF_NAMESPACE_REGISTER or #ZF_NAMESPACE_END_WITH_REGISTER
 * -  for embeded namespace, the namespace text are something looks like "GrandParent.Parent.Child"
 */
#define ZF_NAMESPACE_CURRENT() \
    _ZFP_ZF_NAMESPACE_NOT_REGISTERED()

// ============================================================
/**
 * @brief skip "zf." in "zf.NS0.NS1.funcName"
 *
 * things to skip:
 * -  #ZF_NAMESPACE_GLOBAL_NAME
 * -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *
 * return null if everything are skipped
 */
extern ZFLIB_ZFCore const zfchar *ZFNamespaceSkipGlobal(ZF_IN const zfchar *ns);
/**
 * @brief util to split namespace components, similar to #ZFCoreDataPairSplitString
 *
 * if success, ret ensured to have at least one element even if the namespace is empty,
 * at this case, the only one element would be #ZFIndexRangeZero
 */
extern ZFLIB_ZFCore zfbool ZFNamespaceSplit(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &ret
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        );

// ============================================================
/**
 * @brief get all namespace
 */
extern ZFLIB_ZFCore void ZFNamespaceGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret);
/** @brief see #ZFNamespaceGetAllT */
inline ZFCoreArray<zfstring> ZFNamespaceGetAll(void) {
    ZFCoreArray<zfstring> ret;
    ZFNamespaceGetAllT(ret);
    return ret;
}

/** @brief see #ZFNamespaceGetAllT */
extern ZFLIB_ZFCore void ZFNamespaceGetAllT(
        ZF_IN_OUT ZFCoreArray<zfstring> &ret
        , ZF_IN const zfchar *parent
        , ZF_IN_OPT zfbool recursive = zffalse
        );
/** @brief see #ZFNamespaceGetAllT */
inline ZFCoreArray<zfstring> ZFNamespaceGetAll(
        ZF_IN const zfchar *parent
        , ZF_IN_OPT zfbool recursive = zffalse
        ) {
    ZFCoreArray<zfstring> ret;
    ZFNamespaceGetAllT(ret, parent, recursive);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFNamespaceImpl_h_


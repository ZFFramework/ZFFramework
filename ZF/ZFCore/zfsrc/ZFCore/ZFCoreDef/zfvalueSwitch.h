/**
 * @file zfvalueSwitch.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_zfvalueSwitch_h_
#define _ZFI_zfvalueSwitch_h_

#include "ZFCoreMap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #zfvalueSwitch */
template<typename T_Key, typename T_Hash = ZFCoreMapKeyHash<T_Key>, typename T_Equal = ZFCoreMapKeyEqual<T_Key>>
zfclassLikePOD zfvalueSwitchData {
public:
    /** @brief see #zfvalueSwitch */
    zfvalueSwitchData &v(ZF_IN T_Key const &v) {
        m.set(v, m.count());
        return *this;
    }
private:
    ZFCoreMap<T_Key, zfindex, T_Hash, T_Equal> m;
    template<typename T_KeyV, typename T_Key_, typename T_Hash_, typename T_Equal_>
    friend zfindex zfvalueSwitch(ZF_IN T_KeyV const &v, ZF_IN zfvalueSwitchData<T_Key_, T_Hash_, T_Equal_> const &data);
public:
    /** @cond ZFPrivateDoc */
    zfvalueSwitchData(void) : m() {}
    zfvalueSwitchData(ZF_IN const zfvalueSwitchData<T_Key, T_Hash, T_Equal> &ref) : m(ref.m) {}
    zfvalueSwitchData &operator = (ZF_IN const zfvalueSwitchData<T_Key, T_Hash, T_Equal> &ref) {m = ref.m; return *this;}
    /** @endcond */
};

/**
 * @brief util to switch-case on any value type
 *
 * @code
 *   switch(zfvalueSwitch(v, zfvalueSwitchData<zfstring>()
 *           .v("111")
 *           .v("222")
 *           )) {
 *       case 0:
 *           ZFLog("case 111");
 *           break;
 *       case 1:
 *           ZFLog("case 222");
 *           break;
 *       default:
 *           ZFLog("default");
 *           break;
 *   }
 * @endcode
 *
 * or, store pre initialized switch data for better performance:
 * @code
 *   // the data can be stored for reuse
 *   zfvalueSwitchData<zfstring> m = zfvalueSwitchData<zfstring>()
 *       .v("111")
 *       .v("222")
 *       ;
 *
 *   switch(zfvalueSwitch(v, m)) {
 *       case 0:
 *       ...
 *   }
 * @endcode
 */
template<typename T_KeyV, typename T_Key, typename T_Hash, typename T_Equal>
zfindex zfvalueSwitch(ZF_IN T_KeyV const &v, ZF_IN zfvalueSwitchData<T_Key, T_Hash, T_Equal> const &data) {
    zfiter it = data.m.iterFind(v);
    if(it) {
        return data.m.iterValue(it);
    }
    else {
        return zfindexMax();
    }
}

// ============================================================
zfclassNotPOD _ZFP_zfstringSwitch_Hash {
public:
    /** @brief default key hash function */
    inline zfidentity operator() (ZF_IN const zfchar * const &key) const {
        return zfidentityCalcString(key);
    }
};
zfclassNotPOD _ZFP_zfstringSwitch_Equal {
public:
    inline zfbool operator() (ZF_IN const zfchar * const &key0, ZF_IN const zfchar * const &key1) const {
        return zfscmp(key0, key1) == 0;
    }
};
/** @brief util #zfvalueSwitchData for `const zfchar *` */
typedef zfvalueSwitchData<const zfchar *, _ZFP_zfstringSwitch_Hash, _ZFP_zfstringSwitch_Equal> zfcharConstSwitchData;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfvalueSwitch_h_


/**
 * @file zfstlhashmap.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstlhashmap_h_
#define _ZFI_zfstlhashmap_h_

#include "zfstlpair.h"

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlhashmap
    #define zfstlhashmap std::unordered_map
    #include "zfstl_impl/unordered_map.hpp"
#endif // #ifndef zfstlhashmap


// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Hash = zfstl::hash<T_Key>, typename T_Equal = zfstl::equal_to<T_Key> >
class zfimplhashmap : public zfstlhashmap<T_Key, T_Value, T_Hash, T_Equal> {
private:
    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator it;
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator end;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return it != end;
        }
        zfoverride
        virtual void next(void) {
            ++it;
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->it = it;
            ret->end = end;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return it == t->it;
        }
    };
public:
    zffinal inline zfiter iter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->begin();
        impl->end = this->end();
        return zfiter(impl);
    }

    zffinal inline zfiter iterFind(ZF_IN T_Key const &key) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->find(key);
        impl->end = this->end();
        return zfiter(impl);
    }

    zffinal inline T_Key const &iterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    zffinal inline T_Value &iterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }

    zffinal inline void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN T_Value const &newValue
            ) {
        it.impl<_Iter *>()->it->second = newValue;
    }
    zffinal inline void iterRemove(ZF_IN_OUT zfiter &it) {
        this->erase((it.impl<_Iter *>()->it)++);
    }

    zffinal inline void iterAdd(
            ZF_IN T_Key const &key
            , ZF_IN T_Value const &value
            ) {
        this->insert(zfstl::make_pair<T_Key, T_Value>(key, value));
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlhashmap_h_


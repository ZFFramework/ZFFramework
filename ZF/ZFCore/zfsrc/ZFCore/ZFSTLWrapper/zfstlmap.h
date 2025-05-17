/**
 * @file zfstlmap.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstlmap_h_
#define _ZFI_zfstlmap_h_

#include "zfstlpair.h"
#include <map>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlmap
    #define zfstlmap std::map
#endif

/** @brief stl wrapper */
#ifndef zfstlless
    #define zfstlless std::less
#endif


// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Compare = zfstlless<T_Key> >
class zfimplmap : public zfstlmap<T_Key, T_Value, T_Compare> {
private:
    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator it;
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator end;
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
        this->insert(zfstlpair<T_Key, T_Value>(key, value));
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlmap_h_


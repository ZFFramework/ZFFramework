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


// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Compare = zfstl::less<T_Key> >
class zfimplmap : public zfstlmap<T_Key, T_Value, T_Compare>
{
private:
    static void _ZFP_d(void *data)
    {
        delete ((typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *)data);
    }
    static void *_ZFP_c(void *data)
    {
        return new typename zfimplmap<T_Key, T_Value, T_Compare>::iterator(
            *(typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *)data);
    }

public:
    zffinal inline zfiterator iter(void)
    {
        return zfiterator(
            new typename zfimplmap<T_Key, T_Value, T_Compare>::iterator(this->begin()),
            _ZFP_d,
            _ZFP_c);
    }

    zffinal inline zfiterator iterFind(ZF_IN T_Key const &key)
    {
        return zfiterator(
            new typename zfimplmap<T_Key, T_Value, T_Compare>::iterator(this->find(key)),
            _ZFP_d,
            _ZFP_c);
    }

    zffinal inline zfbool iterValid(ZF_IN const zfiterator &it)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        return (data && *data != this->end());
    }

    zffinal inline void iterNext(ZF_IN_OUT zfiterator &it)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        ++(*data);
    }

    zffinal inline T_Key const &iterKey(ZF_IN const zfiterator &it)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        return (*data)->first;
    }
    zffinal inline T_Value const &iterValue(ZF_IN const zfiterator &it)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        return (*data)->second;
    }

    zffinal inline void iterValue(ZF_IN_OUT zfiterator &it,
                                  ZF_IN T_Value const &newValue)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        (*data)->second = newValue;
    }
    zffinal inline void iterRemove(ZF_IN_OUT zfiterator &it)
    {
        typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *data = it.data<typename zfimplmap<T_Key, T_Value, T_Compare>::iterator *>();
        this->erase((*data)++);
    }

    zffinal inline void iterAdd(ZF_IN T_Key const &key,
                                ZF_IN T_Value const &value)
    {
        this->insert(zfstl::make_pair<T_Key, T_Value>(key, value));
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlmap_h_


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
class zfimplhashmap : public zfstlhashmap<T_Key, T_Value, T_Hash, T_Equal>
{
private:
    static void _ZFP_d(void *data)
    {
        delete ((typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *)data);
    }
    static void *_ZFP_c(void *data)
    {
        return new typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator(
            *(typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *)data);
    }

public:
    zffinal inline zfiterator iter(void)
    {
        return zfiterator(
            new typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator(this->begin()),
            _ZFP_d,
            _ZFP_c);
    }

    zffinal inline zfiterator iterFind(ZF_IN T_Key const &key)
    {
        return zfiterator(
            new typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator(this->find(key)),
            _ZFP_d,
            _ZFP_c);
    }

    zffinal inline zfbool iterValid(ZF_IN const zfiterator &it)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        return (data && *data != this->end());
    }

    zffinal inline void iterNext(ZF_IN_OUT zfiterator &it)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        ++(*data);
    }

    zffinal inline T_Key const &iterKey(ZF_IN const zfiterator &it)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        return (*data)->first;
    }
    zffinal inline T_Value const &iterValue(ZF_IN const zfiterator &it)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        return (*data)->second;
    }

    zffinal inline void iterValue(ZF_IN_OUT zfiterator &it,
                                  ZF_IN T_Value const &newValue)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        (*data)->second = newValue;
    }
    zffinal inline void iterRemove(ZF_IN_OUT zfiterator &it)
    {
        typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *data = it.data<typename zfimplhashmap<T_Key, T_Value, T_Hash, T_Equal>::iterator *>();
        this->erase((*data)++);
    }

    zffinal inline void iterAdd(ZF_IN T_Key const &key,
                                ZF_IN T_Value const &value)
    {
        this->insert(zfstl::make_pair<T_Key, T_Value>(key, value));
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlhashmap_h_


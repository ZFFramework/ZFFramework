/**
 * @file zfstl_hashmap.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_hashmap_h_
#define _ZFI_zfstl_hashmap_h_

#include "zfstl_pair.h"

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlhashmap
    #if __cplusplus >= 201103L
        #define zfstlhashmap std::unordered_map
        #include <unordered_map>
    #elif (defined(_MSC_VER) && ((_MSC_VER < 1500) || !_HAS_TR1)) // old hash_map of MSVC
        #define zfstlhashmap _ZFP_zfstlhashmap

        ZF_ENV_SENSITIVE("old MSVC spec, not for production, for compatibility test only")
        #include <hash_map>
        #include "zfstl_string.h"

        template<typename T>
        class _ZFP_zfstlhashmap_Hash;

        template<typename T>
        class _ZFP_zfstlhashmap_EqualTo;

        // zfstlstringZ
        template<>
        class _ZFP_zfstlhashmap_Hash<zfstlstringZ>
        {
        public:
            size_t operator () (zfstlstringZ const &k) const
            {
                return (size_t)zfidentityCalcString(k.c_str());
            }
        };
        template<>
        class _ZFP_zfstlhashmap_EqualTo<zfstlstringZ>
        {
        public:
            bool operator () (zfstlstringZ const &k0, zfstlstringZ const &k1) const
            {
                return (k0.compare(k1) == 0);
            }
        };

        template<typename T_Key, typename T_Hash, typename T_EqualTo>
        class _ZFP_zfstlhashmap_hashCompare
        {
            public:
                enum {bucket_size = 4, min_buckets = 8};
            public:
                size_t operator () (T_Key const &_Keyval) const
                {
                    long _Quot = _hashFunc(_Keyval) & LONG_MAX;
                    ldiv_t _Qrem = ldiv(_Quot, 127773);
                    _Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
                    if (_Qrem.rem < 0) {_Qrem.rem += LONG_MAX;}
                    return ((size_t)_Qrem.rem);
                }
                bool operator () (T_Key const &_Keyval1, T_Key const &_Keyval2) const
                {
                    return !_equalToFunc(_Keyval1, _Keyval2);
                }
            private:
                T_Hash _hashFunc;
                T_EqualTo _equalToFunc;
        };
        template<typename T_Key, typename T_Value, typename T_Hash = _ZFP_zfstlhashmap_Hash<T_Key>, typename T_EqualTo = _ZFP_zfstlhashmap_EqualTo<T_Key> >
        class _ZFP_zfstlhashmap : public stdext::hash_map<T_Key, T_Value, _ZFP_zfstlhashmap_hashCompare<T_Key, T_Hash, T_EqualTo> >
        {
        };
    #else
        #define zfstlhashmap std::unordered_map

        #include "zfstl_impl/unordered_map.hpp"
    #endif
#endif // #ifndef zfstlhashmap


// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Hash = zfstl::hash<T_Key>, typename T_Equal = zfstl::equal_to<T_Key> >
class ZF_ENV_EXPORT zfimplhashmap : public zfstlhashmap<T_Key, T_Value, T_Hash, T_Equal>
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

#endif // #ifndef _ZFI_zfstl_hashmap_h_


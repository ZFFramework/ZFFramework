/**
 * @file ZFMemPool.h
 * @brief memory pool
 */
#ifndef _ZFI_ZFMemPool_h_
#define _ZFI_ZFMemPool_h_

#include "ZFCoreMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief whether enable memory pool, true by default
 */
#ifndef ZF_ENV_ZFMEMPOOL_ENABLE
    #if _ZFP_ZFMEM_LOG_DISABLE_MEMPOOL
        #define ZF_ENV_ZFMEMPOOL_ENABLE 0
    #else
        #define ZF_ENV_ZFMEMPOOL_ENABLE 1
    #endif
#endif

#define _ZFP_ZFMEMPOOL_DEBUG 0

// ============================================================
/**
 * @def zfpoolNew
 * @brief internal use only, for allocating internal types for performance
 *
 * @warning pointers passed to zfpoolDelete must be the same with the type you create
 * @def zfpoolDelete
 * @brief see #zfpoolNew
 * @def zfpoolDeclareFriend
 * @brief use to declare friend if your type has non-public constructors
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolNew(T_Type, ...) zfnewPlacement((_ZFP_MP_Obj<T_Type >::pNew()), T_Type, ##__VA_ARGS__)
    #define zfpoolDelete(obj) _ZFP_zfpoolDelete(obj)
    #define zfpoolDeclareFriend() \
        friend zfclassFwd _ZFP_MP_Obj<zfself>;
#else
    #define zfpoolNew(T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfpoolDelete(obj) zfdelete(obj)
    #define zfpoolDeclareFriend()
#endif

// ============================================================
// impl
template<int N>
zfclassNotPOD _ZFP_MP_SA { // Size Align
public:
    enum {
        _A = (N <= sizeof(zfuint) * 4 ? sizeof(zfuint) * 2 : sizeof(zfuint) * 8),
        V = ((N % _A) == 0 ? N : ((N / _A) + 1) * _A),
        M = (N <= sizeof(const void *) * 4 ? 32 : N <= sizeof(const void *) * 8 ? 8 : 4),
    };
};
template<int N>
union ZFLIB_ZFCore _ZFP_MP_B { // Block
public:
    zfbyte buf[N];
    _ZFP_MP_B<N> *next;
};
template<int N>
zfclassNotPOD _ZFP_MP_H { // Holder
public:
    static void *pNew(void) {
        _ZFP_MP_H<N> &d = _instance();
        if(d.available) {
            _ZFP_MP_B<N> *t = d.available;
            d.available = d.available->next;
            --d.count;
            return t;
        }
        else {
            return zfmalloc(sizeof(_ZFP_MP_B<N>));
        }
    }
    static void pDel(ZF_IN void *obj) {
        _ZFP_MP_H<N> &d = _instance();
        if(d.count >= _ZFP_MP_SA<N>::M) {
            zffree(obj);
        }
        else {
            ++d.count;
            _ZFP_MP_B<N> *t = (_ZFP_MP_B<N> *)obj;
            t->next = d.available;
            d.available = t;
        }
    }
private:
    _ZFP_MP_H(void)
    : available(zfnull)
    , count(0)
    {
    }
    ~_ZFP_MP_H(void) {
        while(available) {
            _ZFP_MP_B<N> *t = available;
            available = available->next;
            zffree(t);
        }
    }
    static _ZFP_MP_H<N> &_instance(void) {
        static _ZFP_MP_H<N> d;
        return d;
    }
private:
    _ZFP_MP_B<N> *available;
    zfuint count;
};

template<typename T_Type>
zfclassNotPOD _ZFP_MP_Obj {
public:
    static void *pNew(void) {
        zfCoreMutexLocker();
        return _ZFP_MP_H<_ZFP_MP_SA<sizeof(T_Type)>::V>::pNew();
    }
    static void pDel(ZF_IN T_Type *obj) {
        obj->~T_Type();
        _ZFP_MP_H<_ZFP_MP_SA<sizeof(T_Type)>::V>::pDel(obj);
    }
};
template<typename T_Type>
inline void _ZFP_zfpoolDelete(ZF_IN T_Type *obj) {
    if(obj) {
        zfCoreMutexLock();
        _ZFP_MP_Obj<T_Type>::pDel(obj);
        zfCoreMutexUnlock();
    }
}

// ============================================================
#if _ZFP_ZFMEMPOOL_DEBUG
    template<typename T_Type>
    zfclassNotPOD _ZFP_MP_ObjDebug {
    public:
        static T_Type *a(T_Type *obj) {
            if(obj) {
                zfCoreMutexLocker();
                Item *&p_ = p();
                Item *&pEnd_ = pEnd();
                Item *&cEnd_ = cEnd();
                if(pEnd_ == cEnd_) {
                    zfindex count = pEnd_ - p_;
                    zfindex capacity = count ? count * 2 : 8;
                    p_ = (Item *)zfrealloc(p_, capacity * sizeof(Item));
                    pEnd_ = p_ + count;
                    cEnd_ = p_ + capacity;
                }
                *pEnd_ = obj;
                ++pEnd_;
            }
            return obj;
        }
        static T_Type *d(T_Type *obj) {
            if(obj) {
                Item *&p_ = p();
                Item *&pEnd_ = pEnd();
                for(Item *t = p_; t != pEnd_; ++t) {
                    if(*t == obj) {
                        zfmemmove(t, t + 1, pEnd_ - (t + 1));
                        --pEnd_;
                        break;
                    }
                }
            }
            return obj;
        }
    public:
        typedef T_Type *Item;
        static Item *&p(void) {
            static Item *d = zfnull;
            return d;
        }
        static Item *&pEnd(void) {
            static Item *d = zfnull;
            return d;
        }
        static Item *&cEnd(void) {
            static Item *d = zfnull;
            return d;
        }
    };
    template<typename T_Type>
    inline void _ZFP_MP_ObjDebugDelete(ZF_IN T_Type *obj) {
        if(obj) {
            zfCoreMutexLock();
            _ZFP_MP_ObjDebug<T_Type>::d(obj);
            _ZFP_MP_Obj<T_Type>::pDel(obj);
            zfCoreMutexUnlock();
        }
    }

    #undef zfpoolNew
    #undef zfpoolDelete
    #define zfpoolNew(T_Type, ...) _ZFP_MP_ObjDebug<T_Type >::a(zfnewPlacement((_ZFP_MP_Obj<T_Type >::pNew()), T_Type, ##__VA_ARGS__))
    #define zfpoolDelete(obj) _ZFP_MP_ObjDebugDelete(obj)
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFMemPool_h_


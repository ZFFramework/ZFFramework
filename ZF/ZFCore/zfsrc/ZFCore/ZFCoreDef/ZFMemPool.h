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

// ============================================================
zfclassNotPOD ZFLIB_ZFCore _ZFP_MP_T {};
/**
 * @def zfpoolDeclareFriend
 * @brief see #zfnew
 */
#define zfpoolDeclareFriend() zfpoolDeclareFriendT(_ZFP_MP_T)

/**
 * @def zfunsafe_zfpoolNew
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolDelete
 * @brief see #zfnew
 */
#define zfunsafe_zfpoolNew(T_Type, ...) zfunsafe_zfpoolNewT(_ZFP_MP_T, T_Type, ##__VA_ARGS__)
#define zfunsafe_zfpoolDelete(obj) zfunsafe_zfpoolDeleteT(_ZFP_MP_T, (obj))

/**
 * @def zfunsafe_zfpoolMalloc
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolRealloc
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolFree
 * @brief see #zfnew
 */
#define zfunsafe_zfpoolMalloc(size) zfunsafe_zfpoolMallocT(_ZFP_MP_T, (size))
#define zfunsafe_zfpoolRealloc(p, size) zfunsafe_zfpoolReallocT(_ZFP_MP_T, (p), (size))
#define zfunsafe_zfpoolFree(p) zfunsafe_zfpoolFreeT(_ZFP_MP_T, (p))

/**
 * @def zfpoolNew
 * @brief see #zfnew
 *
 * @def zfpoolDelete
 * @brief see #zfnew
 */
#define zfpoolNew(T_Type, ...) zfpoolNewT(_ZFP_MP_T, T_Type, ##__VA_ARGS__)
#define zfpoolDelete(obj) zfpoolDeleteT(_ZFP_MP_T, (obj))

/**
 * @def zfpoolMalloc
 * @brief see #zfnew
 *
 * @def zfpoolRealloc
 * @brief see #zfnew
 *
 * @def zfpoolFree
 * @brief see #zfnew
 */
#define zfpoolMalloc(size) zfpoolMallocT(_ZFP_MP_T, (size))
#define zfpoolRealloc(p, size) zfpoolReallocT(_ZFP_MP_T, (p), (size))
#define zfpoolFree(p) zfpoolFreeT(_ZFP_MP_T, (p))

// ============================================================
/**
 * @def zfpoolDeclareFriendT
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolDeclareFriendT(T) \
        friend zfclassFwd _ZFP_MP_Obj<T, zfself>;
#else
    #define zfpoolDeclareFriendT(T) zfmemDeclareFriend()
#endif

/**
 * @def zfunsafe_zfpoolNewT
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolDeleteT
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfunsafe_zfpoolNewT(T, T_Type, ...) zfnewPlacement((_ZFP_MP_Obj<T, T_Type >::pNew()), T_Type, ##__VA_ARGS__)
    #define zfunsafe_zfpoolDeleteT(T, obj) _ZFP_zfpoolDelete<T>(obj)
#else
    #define zfunsafe_zfpoolNewT(T, T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfunsafe_zfpoolDeleteT(T, obj) zfdelete(obj)
#endif

/**
 * @def zfunsafe_zfpoolMallocT
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolReallocT
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolFreeT
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfunsafe_zfpoolMallocT(T, size) _ZFP_MP_malloc<T>(size)
    #define zfunsafe_zfpoolReallocT(T, p, size) _ZFP_MP_realloc<T>((p), (size))
    #define zfunsafe_zfpoolFreeT(T, p) _ZFP_MP_free<T>(p)
#else
    #define zfunsafe_zfpoolMallocT(T, size) zfmalloc(size)
    #define zfunsafe_zfpoolReallocT(T, p, size) zfrealloc((p), (size))
    #define zfunsafe_zfpoolFreeT(T, p) zffree(p)
#endif

/**
 * @def zfpoolNewT
 * @brief see #zfnew
 *
 * @def zfpoolDeleteT
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolNewT(T, T_Type, ...) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolNewT(T, T_Type, ##__VA_ARGS__))
    #define zfpoolDeleteT(T, obj) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolDeleteT(T, (obj)))
#else
    #define zfpoolNewT(T, T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfpoolDeleteT(T, obj) zfdelete(obj)
#endif

/**
 * @def zfpoolMallocT
 * @brief see #zfnew
 *
 * @def zfpoolReallocT
 * @brief see #zfnew
 *
 * @def zfpoolFreeT
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolMallocT(T, size) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolMallocT(T, (size)))
    #define zfpoolReallocT(T, p, size) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolReallocT(T, (p), (size)))
    #define zfpoolFreeT(T, p) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolFreeT(T, (p)))
#else
    #define zfpoolMallocT(T, size) zfmalloc(size)
    #define zfpoolReallocT(T, p, size) zfrealloc((p), (size))
    #define zfpoolFreeT(T, p) zffree(p)
#endif

// ============================================================
zfclassPOD ZFLIB_ZFCore _ZFP_MP_D {
public:
    void *available;
    zfuint count;
};
extern ZFLIB_ZFCore _ZFP_MP_D &_ZFP_MP_A(ZF_IN zfuint size);
#if ZF_ENV_ZFMEMPOOL_ENABLE
template<int N>
zfclassNotPOD _ZFP_MP_SA { // Size Align
public:
    enum {
        _A = (N <= sizeof(void *) * 4
                ? sizeof(void *) * 2
                : N <= sizeof(void *) * 32
                    ? sizeof(void *) * 4
                    : sizeof(void *) * 32
            ),
        V = ((N % _A) == 0 ? N : ((N / _A) + 1) * _A),
        M = (N <= sizeof(void *) * 4
                ? 32
                : N <= sizeof(void *) * 8
                    ? 16
                    : N <= sizeof(void *) * 32
                        ? 8
                        : N <= sizeof(void *) * 256
                            ? 4
                            : 0
            ),
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
        _ZFP_MP_D &d = _instance();
        if(d.available) {
            _ZFP_MP_B<N> *t = (_ZFP_MP_B<N> *)d.available;
            d.available = t->next;
            --d.count;
            return t;
        }
        else {
            return zfmalloc(sizeof(_ZFP_MP_B<N>));
        }
    }
    static void pDel(ZF_IN void *obj) {
        _ZFP_MP_D &d = _instance();
        if(d.count >= _ZFP_MP_SA<N>::M) {
            zffree(obj);
        }
        else {
            ++d.count;
            _ZFP_MP_B<N> *t = (_ZFP_MP_B<N> *)obj;
            t->next = (_ZFP_MP_B<N> *)d.available;
            d.available = t;
        }
    }
private:
    static _ZFP_MP_D &_instance(void) {
        static _ZFP_MP_D &d = _ZFP_MP_A(N);
        return d;
    }
};

template<typename T, typename T_Type>
zfclassNotPOD _ZFP_MP_Obj {
public:
    static void *pNew(void) {
        return _ZFP_MP_H<_ZFP_MP_SA<sizeof(T_Type)>::V>::pNew();
    }
    static void pDel(ZF_IN T_Type *obj) {
        obj->~T_Type();
        _ZFP_MP_H<_ZFP_MP_SA<sizeof(T_Type)>::V>::pDel(obj);
    }
};
template<typename T, typename T_Type>
inline void _ZFP_zfpoolDelete(ZF_IN T_Type *obj) {
    if(obj) {
        _ZFP_MP_Obj<T, T_Type>::pDel(obj);
    }
}

// ============================================================
template<int N>
zfclassNotPOD _ZFP_MP_mallocSA {
public:
    enum {
        V = _ZFP_MP_SA<_ZFP_MP_mallocSA<N - 1>::V + 1>::V,
    };
};
template<>
zfclassNotPOD _ZFP_MP_mallocSA<1> {
public:
    enum {
        V = _ZFP_MP_SA<1>::V,
    };
};

inline void *_ZFP_MP_mallocFix(ZF_IN void *p, ZF_IN zfindex size) {
    if(p) {
        *(zfindex *)p = size;
        return (((zfbyte *)p) + sizeof(void *));
    }
    else {
        return zfnull;
    }
}
zfclassNotPOD ZFLIB_ZFCore _ZFP_MP_mallocSAMap {
public:
    typedef void *(*Fn_pNew)(void);
    typedef void (*Fn_pDel)(ZF_IN void *p);
public:
    enum {
        N = 10,
    };
    zfindex M_size[N];
    Fn_pNew M_pNew[N];
    Fn_pDel M_pDel[N];
public:
    _ZFP_MP_mallocSAMap(void) {
        M_size[0] = _ZFP_MP_mallocSA<1>::V - sizeof(void *);
        M_size[1] = _ZFP_MP_mallocSA<2>::V - sizeof(void *);
        M_size[2] = _ZFP_MP_mallocSA<3>::V - sizeof(void *);
        M_size[3] = _ZFP_MP_mallocSA<4>::V - sizeof(void *);
        M_size[4] = _ZFP_MP_mallocSA<5>::V - sizeof(void *);
        M_size[5] = _ZFP_MP_mallocSA<6>::V - sizeof(void *);
        M_size[6] = _ZFP_MP_mallocSA<7>::V - sizeof(void *);
        M_size[7] = _ZFP_MP_mallocSA<8>::V - sizeof(void *);
        M_size[8] = _ZFP_MP_mallocSA<9>::V - sizeof(void *);
        M_size[9] = _ZFP_MP_mallocSA<10>::V - sizeof(void *);

        M_pNew[0] = _ZFP_MP_H<_ZFP_MP_mallocSA<1>::V>::pNew;
        M_pNew[1] = _ZFP_MP_H<_ZFP_MP_mallocSA<2>::V>::pNew;
        M_pNew[2] = _ZFP_MP_H<_ZFP_MP_mallocSA<3>::V>::pNew;
        M_pNew[3] = _ZFP_MP_H<_ZFP_MP_mallocSA<4>::V>::pNew;
        M_pNew[4] = _ZFP_MP_H<_ZFP_MP_mallocSA<5>::V>::pNew;
        M_pNew[5] = _ZFP_MP_H<_ZFP_MP_mallocSA<6>::V>::pNew;
        M_pNew[6] = _ZFP_MP_H<_ZFP_MP_mallocSA<7>::V>::pNew;
        M_pNew[7] = _ZFP_MP_H<_ZFP_MP_mallocSA<8>::V>::pNew;
        M_pNew[8] = _ZFP_MP_H<_ZFP_MP_mallocSA<9>::V>::pNew;
        M_pNew[9] = _ZFP_MP_H<_ZFP_MP_mallocSA<10>::V>::pNew;

        M_pDel[0] = _ZFP_MP_H<_ZFP_MP_mallocSA<1>::V>::pDel;
        M_pDel[1] = _ZFP_MP_H<_ZFP_MP_mallocSA<2>::V>::pDel;
        M_pDel[2] = _ZFP_MP_H<_ZFP_MP_mallocSA<3>::V>::pDel;
        M_pDel[3] = _ZFP_MP_H<_ZFP_MP_mallocSA<4>::V>::pDel;
        M_pDel[4] = _ZFP_MP_H<_ZFP_MP_mallocSA<5>::V>::pDel;
        M_pDel[5] = _ZFP_MP_H<_ZFP_MP_mallocSA<6>::V>::pDel;
        M_pDel[6] = _ZFP_MP_H<_ZFP_MP_mallocSA<7>::V>::pDel;
        M_pDel[7] = _ZFP_MP_H<_ZFP_MP_mallocSA<8>::V>::pDel;
        M_pDel[8] = _ZFP_MP_H<_ZFP_MP_mallocSA<9>::V>::pDel;
        M_pDel[9] = _ZFP_MP_H<_ZFP_MP_mallocSA<10>::V>::pDel;
    }
public:
    static _ZFP_MP_mallocSAMap const &I(void) {
        static _ZFP_MP_mallocSAMap d;
        return d;
    }
    static zfindex i(ZF_IN zfindex size) {
        const zfindex *m = I().M_size;
        zfindex l = 0;
        zfindex r = N;
        do {
            zfindex mid = (l + r) / 2;
            if(m[mid] == size) {
                return mid;
            }
            else if(m[mid] < size) {
                l = mid + 1;
            }
            else {
                if(mid > 0) {
                    r = mid;
                }
                else {
                    r = 0;
                }
            }
        } while(l < r);
        return l;
    }
};
template<typename T>
inline void *_ZFP_MP_malloc(ZF_IN zfindex size) {
    zfindex i = _ZFP_MP_mallocSAMap::i(size);
    if(i < _ZFP_MP_mallocSAMap::N) {
        return _ZFP_MP_mallocFix(_ZFP_MP_mallocSAMap::I().M_pNew[i](), size);
    }
    else {
        return _ZFP_MP_mallocFix(zfmalloc(size + sizeof(void *)), size);
    }
}
template<typename T>
inline void _ZFP_MP_free(ZF_IN void *p) {
    if(p == zfnull) {
        return;
    }
    p = ((zfbyte *)p) - sizeof(void *);
    zfindex i = _ZFP_MP_mallocSAMap::i(*(zfindex *)p);
    if(i < _ZFP_MP_mallocSAMap::N) {
        _ZFP_MP_mallocSAMap::I().M_pDel[i](p);
    }
    else {
        zffree(p);
    }
}
template<typename T>
inline void *_ZFP_MP_realloc(ZF_IN void *p, ZF_IN zfindex size) {
    if(p == zfnull) {
        return _ZFP_MP_malloc<T>(size);
    }
    zfindex sizeOld = *(zfindex *)(((zfbyte *)p) - sizeof(void *));
    zfindex i = _ZFP_MP_mallocSAMap::i(sizeOld);
    if(i < _ZFP_MP_mallocSAMap::N) {
        if(size <= _ZFP_MP_mallocSAMap::I().M_size[i]) {
            return p;
        }
    }
    void *pNew = _ZFP_MP_malloc<T>(size);
    if(pNew == zfnull) {
        return zfnull;
    }
    zfmemcpy(pNew, p, sizeOld);
    _ZFP_MP_free<T>(p);
    return pNew;
}
#endif // #if ZF_ENV_ZFMEMPOOL_ENABLE

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFMemPool_h_


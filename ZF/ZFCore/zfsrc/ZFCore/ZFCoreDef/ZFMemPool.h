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
/**
 * @def zfpoolDeclareFriend
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolDeclareFriend() \
        friend zfclassFwd _ZFP_MP_Obj<zfself>;
#else
    #define zfpoolDeclareFriend() zfmemDeclareFriend()
#endif

// ============================================================
/**
 * @def zfunsafe_zfpoolNew
 * @brief see #zfnew
 *
 * @def zfunsafe_zfpoolDelete
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfunsafe_zfpoolNew(T_Type, ...) zfnewPlacement((_ZFP_MP_Obj<T_Type >::pNew()), T_Type, ##__VA_ARGS__)
    #define zfunsafe_zfpoolDelete(obj) _ZFP_zfpoolDelete(obj)
#else
    #define zfunsafe_zfpoolNew(T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfunsafe_zfpoolDelete(obj) zfdelete(obj)
#endif

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
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfunsafe_zfpoolMalloc(size) _ZFP_MP_malloc(size)
    #define zfunsafe_zfpoolRealloc(p, size) _ZFP_MP_realloc((p), (size))
    #define zfunsafe_zfpoolFree(p) _ZFP_MP_free(p)
#else
    #define zfunsafe_zfpoolMalloc(size) zfmalloc(size)
    #define zfunsafe_zfpoolRealloc(p, size) zfrealloc((p), (size))
    #define zfunsafe_zfpoolFree(p) zffree(p)
#endif

// ============================================================
/**
 * @def zfpoolNew
 * @brief see #zfnew
 *
 * @def zfpoolDelete
 * @brief see #zfnew
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolNew(T_Type, ...) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolNew(T_Type, ##__VA_ARGS__))
    #define zfpoolDelete(obj) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolDelete(obj))
#else
    #define zfpoolNew(T_Type, ...) zfnew(T_Type, ##__VA_ARGS__)
    #define zfpoolDelete(obj) zfdelete(obj)
#endif

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
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolMalloc(size) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolMalloc(size))
    #define zfpoolRealloc(p, size) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolRealloc((p), (size)))
    #define zfpoolFree(p) (ZFCoreMutexLockerHolder(), zfunsafe_zfpoolFree(p))
#else
    #define zfpoolMalloc(size) zfmalloc(size)
    #define zfpoolRealloc(p, size) zfrealloc((p), (size))
    #define zfpoolFree(p) zffree(p)
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

template<typename T_Type>
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
template<typename T_Type>
inline void _ZFP_zfpoolDelete(ZF_IN T_Type *obj) {
    if(obj) {
        _ZFP_MP_Obj<T_Type>::pDel(obj);
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
inline void *_ZFP_MP_malloc(ZF_IN zfindex size) {
    if(zffalse){
    }
    else if(size <= _ZFP_MP_mallocSA<1>::V - sizeof(void *)) {
        return _ZFP_MP_mallocFix(_ZFP_MP_H<_ZFP_MP_mallocSA<1>::V>::pNew(), size);
    }
    else if(size <= _ZFP_MP_mallocSA<2>::V - sizeof(void *)) {
        return _ZFP_MP_mallocFix(_ZFP_MP_H<_ZFP_MP_mallocSA<2>::V>::pNew(), size);
    }
    else if(size <= _ZFP_MP_mallocSA<3>::V - sizeof(void *)) {
        return _ZFP_MP_mallocFix(_ZFP_MP_H<_ZFP_MP_mallocSA<3>::V>::pNew(), size);
    }
    else if(size <= _ZFP_MP_mallocSA<4>::V - sizeof(void *)) {
        return _ZFP_MP_mallocFix(_ZFP_MP_H<_ZFP_MP_mallocSA<4>::V>::pNew(), size);
    }
    else if(size <= _ZFP_MP_mallocSA<5>::V - sizeof(void *)) {
        return _ZFP_MP_mallocFix(_ZFP_MP_H<_ZFP_MP_mallocSA<5>::V>::pNew(), size);
    }
    else {
        return _ZFP_MP_mallocFix(zfmalloc(size + sizeof(void *)), size);
    }
}
inline void _ZFP_MP_free(ZF_IN void *p) {
    if(p == zfnull) {
        return;
    }
    p = ((zfbyte *)p) - sizeof(void *);
    zfindex size = *(zfindex *)p;
    if(zffalse){
    }
    else if(size <= _ZFP_MP_mallocSA<1>::V - sizeof(void *)) {
        _ZFP_MP_H<_ZFP_MP_mallocSA<1>::V>::pDel(p);
    }
    else if(size <= _ZFP_MP_mallocSA<2>::V - sizeof(void *)) {
        _ZFP_MP_H<_ZFP_MP_mallocSA<2>::V>::pDel(p);
    }
    else if(size <= _ZFP_MP_mallocSA<3>::V - sizeof(void *)) {
        _ZFP_MP_H<_ZFP_MP_mallocSA<3>::V>::pDel(p);
    }
    else if(size <= _ZFP_MP_mallocSA<4>::V - sizeof(void *)) {
        _ZFP_MP_H<_ZFP_MP_mallocSA<4>::V>::pDel(p);
    }
    else if(size <= _ZFP_MP_mallocSA<5>::V - sizeof(void *)) {
        _ZFP_MP_H<_ZFP_MP_mallocSA<5>::V>::pDel(p);
    }
    else {
        zffree(p);
    }
}
inline void *_ZFP_MP_realloc(ZF_IN void *p, ZF_IN zfindex size) {
    if(p == zfnull) {
        return _ZFP_MP_malloc(size);
    }
    zfindex sizeOld = *(zfindex *)(((zfbyte *)p) - sizeof(void *));
    if(size <= sizeOld) {
        return p;
    }
    else if(sizeOld <= _ZFP_MP_mallocSA<1>::V - sizeof(void *)) {
        if(size <= _ZFP_MP_mallocSA<1>::V - sizeof(void *)) {
            return p;
        }
    }
    else if(sizeOld <= _ZFP_MP_mallocSA<2>::V - sizeof(void *)) {
        if(size <= _ZFP_MP_mallocSA<2>::V - sizeof(void *)) {
            return p;
        }
    }
    else if(sizeOld <= _ZFP_MP_mallocSA<3>::V - sizeof(void *)) {
        if(size <= _ZFP_MP_mallocSA<3>::V - sizeof(void *)) {
            return p;
        }
    }
    else if(sizeOld <= _ZFP_MP_mallocSA<4>::V - sizeof(void *)) {
        if(size <= _ZFP_MP_mallocSA<4>::V - sizeof(void *)) {
            return p;
        }
    }
    else if(sizeOld <= _ZFP_MP_mallocSA<5>::V - sizeof(void *)) {
        if(size <= _ZFP_MP_mallocSA<5>::V - sizeof(void *)) {
            return p;
        }
    }
    void *pNew = _ZFP_MP_malloc(size);
    if(pNew == zfnull) {
        return zfnull;
    }
    zfmemcpy(pNew, p, sizeOld);
    _ZFP_MP_free(p);
    return pNew;
}
#endif // #if ZF_ENV_ZFMEMPOOL_ENABLE

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFMemPool_h_


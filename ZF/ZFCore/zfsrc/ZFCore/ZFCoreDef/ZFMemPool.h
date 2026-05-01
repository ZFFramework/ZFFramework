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
 * @def zfpoolNew
 * @brief internal use only, for allocating internal types for performance
 *
 * @def zfpoolDelete
 * @brief see #zfpoolNew, see #zfnew for for limitation info
 *
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
    #define zfpoolDeclareFriend() zfmemDeclareFriend()
#endif

/**
 * @def zfpoolMalloc
 * @brief internal use only, for allocating internal types for performance
 *
 * @def zfpoolRealloc
 * @brief see #zfpoolMalloc
 *
 * @def zfpoolFree
 * @brief see #zfpoolMalloc
 */
#if ZF_ENV_ZFMEMPOOL_ENABLE
    #define zfpoolMalloc(size) _ZFP_MP_malloc(size)
    #define zfpoolRealloc(p, size) _ZFP_MP_realloc((p), (size))
    #define zfpoolFree(p) _ZFP_MP_free(p)
#else
    #define zfpoolMalloc(size) zfmalloc(size)
    #define zfpoolRealloc(p, size) zfrealloc((p), (size))
    #define zfpoolFree(p) zffree(p)
#endif

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
        ZFCoreMutexLocker();
        return _ZFP_MP_H<_ZFP_MP_SA<sizeof(T_Type)>::V>::pNew();
    }
    static void pDel(ZF_IN T_Type *obj) {
        ZFCoreMutexLocker();
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
    ZFCoreMutexLocker();
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
    ZFCoreMutexLocker();
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
    ZFCoreMutexLocker();
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


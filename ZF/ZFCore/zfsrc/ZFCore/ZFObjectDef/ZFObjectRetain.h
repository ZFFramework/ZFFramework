/**
 * @file ZFObjectRetain.h
 * @brief retain count logic for ZFFramework
 */

#ifndef _ZFI_ZFObjectRetain_h_
#define _ZFI_ZFObjectRetain_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfAlloc
/* ZFMETHOD_MAX_PARAM */
template<typename T_ZFObject, int valid>
zfclassNotPOD _ZFP_Obj_AllocCk;
template<typename T_ZFObject>
zfclassNotPOD _ZFP_Obj_AllocCk<T_ZFObject, 1> {
public:
    static inline T_ZFObject *Alloc(void) {
        ZFObject *obj = T_ZFObject::_ZFP_Obj_ctor();
        obj->_ZFP_ZFObject_objectOnInit();
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return _ZFP_ObjCastInternal(T_ZFObject *, obj);
    }
public:
    static inline T_ZFObject *CanAlloc(void);
    template<typename P0>
    static inline T_ZFObject *CanAlloc(P0 const &p0) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2, p3);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2, p3, p4);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2, p3, p4, p5);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    static inline T_ZFObject *CanAlloc(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6, P7 const &p7) {
        T_ZFObject *obj = _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor());
        obj->_ZFP_ZFObject_objectOnInit();
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6, p7);
        obj->_ZFP_ZFObject_objectOnInitFinish();
        return obj;
    }
};
template<typename T_ZFObject>
zfclassNotPOD _ZFP_Obj_AllocCk<T_ZFObject, 0> {
};
#define _ZFP_zfAlloc(T_ZFObject, ...) \
    _ZFP_Obj_AllocCk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::CanAlloc(__VA_ARGS__)
/**
 * @brief alloc an object, see #ZFObject
 *
 * if your class has declared #ZFALLOC_CACHE_RELEASE,
 * then zfAlloc would automatically has cache logic
 */
#define zfAlloc(T_ZFObject, ...) \
    zfunsafe_zfAlloc(T_ZFObject, ##__VA_ARGS__)
/** @brief no lock version of #zfAlloc, use with caution */
#define zfunsafe_zfAlloc(T_ZFObject, ...) \
    _ZFP_zfAlloc(T_ZFObject, ##__VA_ARGS__)

#define _ZFP_zfAllocInternal(T_ZFObject) \
    _ZFP_Obj_AllocCk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::Alloc()

// ============================================================
// zfRetain
inline void _ZFP_zfRetainAction(ZF_IN ZFObject *obj) {
    if(obj) {
        obj->objectOnRetain();
    }
}
template<typename T_ZFObject>
inline T_ZFObject const &_ZFP_zfRetain(ZF_IN T_ZFObject const &obj) {
    _ZFP_zfRetainAction(zfcast(ZFObject *, obj));
    return obj;
}
/**
 * @brief retain an object, see #ZFObject
 */
#define zfRetain(obj) \
    (ZFCoreMutexLockerHolder(), zfunsafe_zfRetain(obj))
/** @brief no lock version of #zfRetain, use with caution */
#define zfunsafe_zfRetain(obj) \
    _ZFP_zfRetain(obj)

// ============================================================
// zfRelease
inline void _ZFP_zfReleaseAction(ZF_IN ZFObject *obj) {
    if(obj) {
        obj->_ZFP_ZFObjectCheckRelease();
    }
}
template<typename T_ZFObject>
inline void _ZFP_zfRelease(ZF_IN T_ZFObject const &obj) {
    _ZFP_zfReleaseAction(zfcast(ZFObject *, obj));
}
/**
 * @brief release an object, see #ZFObject
 */
#define zfRelease(obj) \
    (ZFCoreMutexLockerHolder(), zfunsafe_zfRelease(obj))
/** @brief no lock version of #zfRelease, use with caution */
#define zfunsafe_zfRelease(obj) \
    _ZFP_zfRelease(obj)

// ============================================================
// ZFALLOC_CACHE_RELEASE
/**
 * @brief mark class that it should apply cache logic when alloc
 *
 * usage:
 * @code
 *   zfclass YourObject : zfextend SomeParent {
 *       ZFOBJECT_DECLARE(YourObject, SomeParent)
 *       ZFALLOC_CACHE_RELEASE({
 *           cache->yourCacheCleanupAction();
 *       })
 *   };
 * @endcode
 *
 * once declared, when you alloc object with no extra param,
 * the cache logic would automatically take effect:
 * @code
 *   ZFObject *obj = zfAlloc(YourObject); // first time alloc
 *   zfRelease(obj); // release, and the object would be cached
 *
 *   obj = zfAlloc(YourObject); // second time alloc, the previously cached object would be returned
 *   zfRelease(obj);
 *
 *   obj = zfAlloc(YourObject, someParam); // alloc with extra param, no cache logic would be applied
 *   zfRelease(obj);
 * @endcode
 *
 * note the alloc cache logic is explicitly attached to class,
 * parent class' #ZFALLOC_CACHE_RELEASE does not make child class cacheable,
 * the child class must also explicitly declare #ZFALLOC_CACHE_RELEASE to enable cache logic
 */
#define ZFALLOC_CACHE_RELEASE(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static ZFObject *_ZFP_zfAllocCacheIvk(void) { \
            return _ZFP_zfAllocCache<zfself>::Alloc(); \
        } \
        static void _ZFP_zfAllocCacheRelease(ZF_IN ZFObject *_obj) { \
            zfsuper::_ZFP_zfAllocCacheRelease(_obj); \
            zfself *cache = zfcast(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/** @brief #ZFALLOC_CACHE_RELEASE for abstract class */
#define ZFALLOC_CACHE_RELEASE_ABSTRACT(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static void _ZFP_zfAllocCacheRelease(ZF_IN ZFObject *_obj) { \
            zfsuper::_ZFP_zfAllocCacheRelease(_obj); \
            zfself *cache = zfcast(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/**
 * @brief remove all object cache, see #ZFALLOC_CACHE_RELEASE
 */
extern ZFLIB_ZFCore void zfAllocCacheRemoveAll(void);

extern ZFLIB_ZFCore void _ZFP_zfAllocCacheImplRegister(
        ZF_IN_OUT zfbool &enableFlag
        , ZF_IN_OUT ZFObject **cache
        , ZF_IN_OUT zfindex &cacheCount
        );
extern ZFLIB_ZFCore void _ZFP_zfAllocCacheImplUnregister(ZF_IN_OUT zfbool &enableFlag);
template<typename T_ZFObject, int T_MaxCache = 16>
zfclassNotPOD _ZFP_zfAllocCache {
private:
    typedef _ZFP_zfAllocCache<T_ZFObject, T_MaxCache> zfself;
public:
    static T_ZFObject *Alloc(void) {
        static RegHolder _dummy;
        ZFUNUSED(_dummy);
        if(enableFlag()) {
            if(cacheCount() > 0) {
                ZFObject *ret = cache()[--(cacheCount())];
                ret->_ZFP_ZFObject_zfAllocCacheRelease(zfself::_ZFP_zfAllocCacheRelease);
                ret->_ZFP_ZFObject_objectOnInitFromCache();
                return zfcast(T_ZFObject *, ret);
            }
            else {
                T_ZFObject *ret = _ZFP_zfAllocInternal(T_ZFObject);
                ret->_ZFP_ZFObject_zfAllocCacheRelease(zfself::_ZFP_zfAllocCacheRelease);
                return ret;
            }
        }
        else {
            return _ZFP_zfAllocInternal(T_ZFObject);
        }
    }
    static void _ZFP_zfAllocCacheRelease(ZF_IN ZFObject *obj) {
        obj->_ZFP_ZFObject_zfAllocCacheRelease(zfnull);
        if(enableFlag() && cacheCount() < T_MaxCache) {
            T_ZFObject::_ZFP_zfAllocCacheRelease(obj);
            cache()[cacheCount()++] = obj;
        }
        else {
            zfunsafe_zfRelease(obj);
        }
    }
    static zfbool &enableFlag(void) {
        static zfbool enableFlag = zffalse;
        return enableFlag;
    }
    static ZFObject **cache(void) {
        static ZFObject *cache[T_MaxCache];
        return cache;
    }
    static zfindex &cacheCount(void) {
        static zfindex cacheCount = 0;
        return cacheCount;
    }
private:
    zfclassNotPOD RegHolder {
    public:
        RegHolder(void) {
            _ZFP_zfAllocCacheImplRegister(enableFlag(), cache(), cacheCount());
        }
        ~RegHolder(void) {
            _ZFP_zfAllocCacheImplUnregister(enableFlag());
        }
    };
};

template<typename T_ZFObject>
T_ZFObject *_ZFP_Obj_AllocCk<T_ZFObject, 1>::CanAlloc(void) {
    if(&T_ZFObject::_ZFP_zfAllocCacheIvk != T_ZFObject::zfsuper::_ZFP_zfAllocCacheIvk) {
        return _ZFP_zfAllocCache<T_ZFObject>::Alloc();
    }
    else {
        return _ZFP_Obj_AllocCk<T_ZFObject, 1>::Alloc();
    }
}

// ============================================================
template<typename T_ZFObject> void _ZFP_zfweakCk(T_ZFObject &t);
/**
 * @brief util to release property's old value, retain new value, then set to property
 *
 * this macro is similar to the retain property in Object-C\n
 * typical usage:
 * @code
 *   ZFObject *property = ...;
 *   ZFObject *newProperty = ...;
 *
 *   // OK, release property, retain newProperty, then set to property
 *   zfRetainChange(property, newProperty);
 *
 *   // OK, use return value of a function as new value
 *   // but keep it in mind, that the new value will be retained
 *   zfRetainChange(property, funcThatReturnZFObject());
 *
 *   // OK, same as release old property and set it to zfnull
 *   zfRetainChange(property, zfnull);
 *
 *   // error, new value must be ZFObject
 *   // zfRetainChange(property, 123);
 *
 *   // error, property must be a variable contains a (ZFObject *)
 *   // zfRetainChange(zfnull, newProperty);
 *   // zfRetainChange(funcThatReturnZFObject(), newProperty);
 * @endcode
 * @see zfRetain, zfRelease, ZFPROPERTY_RETAIN
 */
template<typename T_ZFObject, typename T_ZFObject2>
inline void zfRetainChange(ZF_IN T_ZFObject &obj, ZF_IN T_ZFObject2 const &v) {
    ZFCoreMutexLock();
    _ZFP_zfweakCk(obj);
    ZFObject *tmp = zfcast(ZFObject *, obj);
    zfunsafe_zfRetain(obj = v);
    zfunsafe_zfRelease(tmp);
    ZFCoreMutexUnlock();
}
/** @brief no lock version of #zfRetainChange, use with caution */
template<typename T_ZFObject, typename T_ZFObject2>
inline void zfunsafe_zfRetainChange(ZF_IN T_ZFObject &obj, ZF_IN T_ZFObject2 const &v) {
    _ZFP_zfweakCk(obj);
    ZFObject *tmp = zfcast(ZFObject *, obj);
    zfunsafe_zfRetain(obj = v);
    zfunsafe_zfRelease(tmp);
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectRetain_h_


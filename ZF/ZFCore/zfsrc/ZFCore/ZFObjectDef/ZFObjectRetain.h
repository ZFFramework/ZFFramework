/**
 * @file ZFObjectRetain.h
 * @brief retain count logic for ZFFramework
 */

#ifndef _ZFI_ZFObjectRetain_h_
#define _ZFI_ZFObjectRetain_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfobjAlloc
/* ZFMETHOD_MAX_PARAM */
template<typename T_ZFObject, int valid>
zfclassNotPOD _ZFP_ObjACk; // alloc check
template<typename T_ZFObject>
zfclassNotPOD _ZFP_ObjACk<T_ZFObject, 1> {
public:
    static inline T_ZFObject *A(void) {
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
zfclassNotPOD _ZFP_ObjACk<T_ZFObject, 0> {
};
#define _ZFP_zfobjAlloc(T_ZFObject, ...) \
    _ZFP_ObjACk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::CanAlloc(__VA_ARGS__)
/**
 * @brief alloc an object, see #ZFObject
 *
 * if your class has declared #ZFOBJECT_CACHE_RELEASE,
 * then zfobjAlloc would automatically has cache logic
 */
#define zfobjAlloc(T_ZFObject, ...) \
    zfunsafe_zfobjAlloc(T_ZFObject, ##__VA_ARGS__)
/** @brief no lock version of #zfobjAlloc, use with caution */
#define zfunsafe_zfobjAlloc(T_ZFObject, ...) \
    _ZFP_zfobjAlloc(T_ZFObject, ##__VA_ARGS__)

#define _ZFP_zfobjAllocInternal(T_ZFObject) \
    _ZFP_ObjACk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::A()

// ============================================================
// zfobjRetain
inline void _ZFP_zfobjRetainAction(ZF_IN ZFObject *obj) {
    if(obj) {
        obj->objectOnRetain();
    }
}
template<typename T_ZFObject>
inline T_ZFObject const &_ZFP_zfobjRetain(ZF_IN T_ZFObject const &obj) {
    _ZFP_zfobjRetainAction(zfcast(ZFObject *, obj));
    return obj;
}
/**
 * @brief retain an object, see #ZFObject
 */
#define zfobjRetain(obj) \
    (ZFCoreMutexLockerHolder(), zfunsafe_zfobjRetain(obj))
/** @brief no lock version of #zfobjRetain, use with caution */
#define zfunsafe_zfobjRetain(obj) \
    _ZFP_zfobjRetain(obj)

// ============================================================
// zfobjRelease
inline void _ZFP_zfobjReleaseAction(ZF_IN ZFObject *obj) {
    if(obj) {
        obj->_ZFP_ZFObjectCheckRelease();
    }
}
template<typename T_ZFObject>
inline void _ZFP_zfobjRelease(ZF_IN T_ZFObject const &obj) {
    _ZFP_zfobjReleaseAction(zfcast(ZFObject *, obj));
}
/**
 * @brief release an object, see #ZFObject
 */
#define zfobjRelease(obj) \
    (ZFCoreMutexLockerHolder(), zfunsafe_zfobjRelease(obj))
/** @brief no lock version of #zfobjRelease, use with caution */
#define zfunsafe_zfobjRelease(obj) \
    _ZFP_zfobjRelease(obj)

// ============================================================
// ZFOBJECT_CACHE_RELEASE
/**
 * @brief mark class that it should apply cache logic when alloc
 *
 * usage:
 * @code
 *   zfclass YourObject : zfextend SomeParent {
 *       ZFOBJECT_DECLARE(YourObject, SomeParent)
 *       ZFOBJECT_CACHE_RELEASE({
 *           cache->yourCacheCleanupAction();
 *       })
 *   };
 * @endcode
 *
 * once declared, when you alloc object with no extra param,
 * the cache logic would automatically take effect:
 * @code
 *   ZFObject *obj = zfobjAlloc(YourObject); // first time alloc
 *   zfobjRelease(obj); // release, and the object would be cached
 *
 *   obj = zfobjAlloc(YourObject); // second time alloc, the previously cached object would be returned
 *   zfobjRelease(obj);
 *
 *   obj = zfobjAlloc(YourObject, someParam); // alloc with extra param, no cache logic would be applied
 *   zfobjRelease(obj);
 * @endcode
 *
 * note the alloc cache logic is explicitly attached to class,
 * parent class' #ZFOBJECT_CACHE_RELEASE does not make child class cacheable,
 * the child class must also explicitly declare #ZFOBJECT_CACHE_RELEASE to enable cache logic
 */
#define ZFOBJECT_CACHE_RELEASE(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static ZFObject *_ZFP_ObjACIvk(void) { \
            return _ZFP_ObjAC<zfself>::A(); \
        } \
        static void _ZFP_ObjACR(ZF_IN ZFObject *_obj) { \
            zfsuper::_ZFP_ObjACR(_obj); \
            zfself *cache = zfcast(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/** @brief #ZFOBJECT_CACHE_RELEASE for abstract class */
#define ZFOBJECT_CACHE_RELEASE_ABSTRACT(action) \
    public: \
        /** @cond ZFPrivateDoc */ \
        static void _ZFP_ObjACR(ZF_IN ZFObject *_obj) { \
            zfsuper::_ZFP_ObjACR(_obj); \
            zfself *cache = zfcast(zfself *, _obj); \
            ZFUNUSED(cache); \
            action \
        } \
        /** @endcond */

/**
 * @brief remove all object cache, see #ZFOBJECT_CACHE_RELEASE
 */
extern ZFLIB_ZFCore void zfobjAllocCacheRemoveAll(void);

extern ZFLIB_ZFCore void _ZFP_zfobjAllocCacheImplRegister(
        ZF_IN_OUT zfbool &enableFlag
        , ZF_IN_OUT ZFObject **cache
        , ZF_IN_OUT zfindex &cacheCount
        );
extern ZFLIB_ZFCore void _ZFP_zfobjAllocCacheImplUnregister(ZF_IN_OUT zfbool &enableFlag);
template<typename T_ZFObject, int T_MaxCache = 16>
zfclassNotPOD _ZFP_ObjAC { // alloc cache
private:
    typedef _ZFP_ObjAC<T_ZFObject, T_MaxCache> zfself;
public:
    static T_ZFObject *A(void) {
        static RegH r;
        ZFUNUSED(r);
        if(enableFlag()) {
            if(cacheCount() > 0) {
                ZFObject *ret = cache()[--(cacheCount())];
                ret->_ZFP_ZFObject_zfobjAllocCacheRelease(zfself::_ZFP_ObjACR);
                ret->_ZFP_ZFObject_objectOnInitFromCache();
                return zfcast(T_ZFObject *, ret);
            }
            else {
                T_ZFObject *ret = _ZFP_zfobjAllocInternal(T_ZFObject);
                ret->_ZFP_ZFObject_zfobjAllocCacheRelease(zfself::_ZFP_ObjACR);
                return ret;
            }
        }
        else {
            return _ZFP_zfobjAllocInternal(T_ZFObject);
        }
    }
    static void _ZFP_ObjACR(ZF_IN ZFObject *obj) {
        obj->_ZFP_ZFObject_zfobjAllocCacheRelease(zfnull);
        if(enableFlag() && cacheCount() < T_MaxCache) {
            T_ZFObject::_ZFP_ObjACR(obj);
            cache()[cacheCount()++] = obj;
        }
        else {
            zfunsafe_zfobjRelease(obj);
        }
    }
private:
    static zfbool &enableFlag(void) {
        static zfbool E = zffalse;
        return E;
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
    zfclassNotPOD RegH {
    public:
        RegH(void) {
            _ZFP_zfobjAllocCacheImplRegister(enableFlag(), cache(), cacheCount());
        }
        ~RegH(void) {
            _ZFP_zfobjAllocCacheImplUnregister(enableFlag());
        }
    };
};

template<typename T_ZFObject>
T_ZFObject *_ZFP_ObjACk<T_ZFObject, 1>::CanAlloc(void) {
    if(&T_ZFObject::_ZFP_ObjACIvk != T_ZFObject::zfsuper::_ZFP_ObjACIvk) {
        return _ZFP_ObjAC<T_ZFObject>::A();
    }
    else {
        return _ZFP_ObjACk<T_ZFObject, 1>::A();
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
 *   zfobjRetainChange(property, newProperty);
 *
 *   // OK, use return value of a function as new value
 *   // but keep it in mind, that the new value will be retained
 *   zfobjRetainChange(property, funcThatReturnZFObject());
 *
 *   // OK, same as release old property and set it to zfnull
 *   zfobjRetainChange(property, zfnull);
 *
 *   // error, new value must be ZFObject
 *   // zfobjRetainChange(property, 123);
 *
 *   // error, property must be a variable contains a (ZFObject *)
 *   // zfobjRetainChange(zfnull, newProperty);
 *   // zfobjRetainChange(funcThatReturnZFObject(), newProperty);
 * @endcode
 * @see zfobjRetain, zfobjRelease, ZFPROPERTY_RETAIN
 */
template<typename T_ZFObject, typename T_ZFObject2>
inline void zfobjRetainChange(ZF_IN T_ZFObject &obj, ZF_IN T_ZFObject2 const &v) {
    ZFCoreMutexLock();
    _ZFP_zfweakCk(obj);
    ZFObject *tmp = zfcast(ZFObject *, obj);
    zfunsafe_zfobjRetain(obj = v);
    zfunsafe_zfobjRelease(tmp);
    ZFCoreMutexUnlock();
}
/** @brief no lock version of #zfobjRetainChange, use with caution */
template<typename T_ZFObject, typename T_ZFObject2>
inline void zfunsafe_zfobjRetainChange(ZF_IN T_ZFObject &obj, ZF_IN T_ZFObject2 const &v) {
    _ZFP_zfweakCk(obj);
    ZFObject *tmp = zfcast(ZFObject *, obj);
    zfunsafe_zfobjRetain(obj = v);
    zfunsafe_zfobjRelease(tmp);
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectRetain_h_


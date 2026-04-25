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

template<typename T_ZFObject>
T_ZFObject *_ZFP_ObjACk<T_ZFObject, 1>::CanAlloc(void) {
    return _ZFP_ObjACk<T_ZFObject, 1>::A();
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


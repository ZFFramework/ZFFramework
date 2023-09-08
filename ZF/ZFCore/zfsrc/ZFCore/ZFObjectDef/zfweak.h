/**
 * @file zfweak.h
 * @brief weak reference to ZFObject
 */

#ifndef _ZFI_zfweak_h_
#define _ZFI_zfweak_h_

#include "ZFObjectHolder.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief weak reference to ZFObject
 *
 * usage:
 * @code
 *   ZFArray *obj = xxx;
 *
 *   zfweak p1 = obj;
 *   ZFArray *access1 = p1.get();
 *
 *   zfweakT<ZFArray *> p2 = obj;
 *   if(p2) {
 *       p2.get()->add(xxx);
 *   }
 *   p2.set(xxx);
 *   if(p2.valid()) {
 *       p2.get()->add(xxx);
 *   }
 * @endcode
 *
 * can be used in script with similar rule, lua for example:
 * @code
 *   local a = ZFArray();
 *
 *   local p1 = zfweak(a);
 *   local access1 = p1:get();
 *
 *   local p2 = zfweak(a);
 *   if(p2:get() ~= zfnull) {
 *       p2:get():add(xxx);
 *   }
 *   p2:set(xxx);
 *   if(p2:valid()) {
 *       p2:get():add(xxx);
 *   }
 * @endcode
 *
 * this class is a util wrapper for #ZFObjectHolder
 */
zfclassLikePOD ZFLIB_ZFCore zfweak {
    /** @cond ZFPrivateDoc */
public:
    zfweak(void)
    : _ZFP_obj(zfnull)
    {
    }
    zfweak(ZF_IN const zfweak &obj)
    : _ZFP_obj(zfRetain(obj._ZFP_obj))
    {
    }
    template<typename T_ZFObject>
    zfweak(ZF_IN T_ZFObject *obj)
    : _ZFP_obj(obj ? zfRetain(obj->toObject()->objectHolder()) : zfnull)
    {
    }
    template<typename T_ZFObject>
    zfweak(ZF_IN T_ZFObject const &obj)
    {
        ZFObject *t = _ZFP_ZFAnyCast(T_ZFObject, obj);
        _ZFP_obj = t ? zfRetain(t->objectHolder()) : zfnull;
    }
    ~zfweak(void) {
        zfRelease(_ZFP_obj);
    }

public:
    inline zfweak &operator = (ZF_IN const zfweak &obj) {
        this->set(obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfweak &operator = (ZF_IN T_ZFObject *obj) {
        this->set(obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfweak &operator = (ZF_IN T_ZFObject const &obj) {
        this->set(obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return (this->toObject() == ZFCastZFObjectUnchecked(ZFObject *, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return (this->toObject() != ZFCastZFObjectUnchecked(ZFObject *, obj));
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return (this->toObject() == _ZFP_ZFAnyCast(T_ZFObject, obj));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return (this->toObject() != _ZFP_ZFAnyCast(T_ZFObject, obj));
    }

public:
    inline operator zfbool (void) const {
        return this->toObject() != zfnull;
    }
    /** @endcond */

public:
    /**
     * @brief true if the holded object is not null
     */
    inline zfbool valid(void) const {
        return this->toObject() != zfnull;
    }

    /**
     * @brief get the holded object
     */
    inline ZFAny get(void) const {
        return _ZFP_obj ? _ZFP_obj->objectHolded() : zfnull;
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    void set(ZF_IN T_ZFObject *obj) {
        zfRetainChange(_ZFP_obj, obj ? obj->toObject()->objectHolder() : zfnull);
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    void set(ZF_IN T_ZFObject const &obj) {
        ZFObject *t = _ZFP_ZFAnyCast(T_ZFObject, obj);
        zfRetainChange(_ZFP_obj, t ? t->objectHolder() : zfnull);
    }
    /**
     * @brief set the holded object
     */
    void set(ZF_IN const zfweak &obj) {
        zfRetainChange(_ZFP_obj, obj._ZFP_obj);
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return _ZFP_obj ? _ZFP_obj->objectHolded() : zfnull;
    }
    /**
     * @brief cast by #ZFCastZFObjectUnchecked
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }

protected:
    ZFObjectHolder *_ZFP_obj;
};
ZFOUTPUT_TYPE(zfweak, {
    if(v) {
        v.get()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, zfweak, zfweak)

// ============================================================
/**
 * @brief see #zfweak
 */
template<typename T_ZFObject>
zfclassLikePOD zfweakT : zfextend zfweak {
    /** @cond ZFPrivateDoc */
public:
    zfweakT(void)
    : zfweak()
    {
    }
    zfweakT(ZF_IN const zfweakT<T_ZFObject> &obj)
    : zfweak((const zfweak &)obj)
    {
    }
    zfweakT(ZF_IN T_ZFObject obj)
    : zfweak(obj)
    {
    }

public:
    inline zfweakT<T_ZFObject> &operator = (ZF_IN zfweakT<T_ZFObject> const &obj) {
        this->set(obj);
        return *this;
    }
    inline zfweakT<T_ZFObject> &operator = (ZF_IN T_ZFObject obj) {
        this->set(obj);
        return *this;
    }

public:
    zfbool operator == (ZF_IN T_ZFObject obj) const {
        return (this->toObject() == ZFCastZFObjectUnchecked(ZFObject *, obj));
    }
    zfbool operator != (ZF_IN T_ZFObject obj) const {
        return (this->toObject() != ZFCastZFObjectUnchecked(ZFObject *, obj));
    }
    zfbool operator == (ZF_IN zfweakT<T_ZFObject> const &obj) const {
        return (this->toObject() == obj.toObject());
    }
    zfbool operator != (ZF_IN zfweakT<T_ZFObject> const &obj) const {
        return (this->toObject() != obj.toObject());
    }

public:
    inline ZFObject *toObject(void) const {
        return _ZFP_obj ? _ZFP_obj->objectHolded() : zfnull;
    }
    template<typename T_ZFObjectCast>
    inline T_ZFObjectCast to(void) const {
        return ZFCastZFObjectUnchecked(T_ZFObjectCast, this->toObject());
    }
    /** @endcond */

public:
    /**
     * @brief get the holded object
     */
    inline T_ZFObject get(void) const {
        return _ZFP_obj ? _ZFP_obj->objectHolded() : zfnull;
    }
    /**
     * @brief set the holded object
     */
    void set(ZF_IN T_ZFObject obj) {
        zfweak::set(obj);
    }
    /**
     * @brief set the holded object
     */
    void set(ZF_IN zfweakT<T_ZFObject> const &obj) {
        zfweak::set((const zfweak &)obj);
    }
};
ZFOUTPUT_TYPE_TEMPLATE(typename T_ZFObject, zfweakT<T_ZFObject>, {
    if(v) {
        v.get()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfclassNotPOD ZFTypeId<zfweakT<T_ZFObject>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId<zfweak>::TypeId();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfchar *typeId(void) const {
        return TypeId();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return v_zfweak::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfautoObject &obj
            , ZF_IN zfweakT<T_ZFObject> const &v
            ) {
        return ZFTypeId<zfweak>::ValueStore(obj, v);
    }
    template<typename T_Access = zfweakT<T_ZFObject>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfweakT<T_ZFObject>
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccessAvailable(obj);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfautoObject &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccess(obj);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) {
            ZFTypeId<zfweak>::Value<T_Access>::zfvAccessFinish(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfautoObject &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccessAvailable(obj);
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfautoObject &obj) {
            // zfweakT ensured safe for reinterpret cast
            return (typename zftTraits<T_Access>::TrNoRef)ZFTypeId<zfweak>::Value<T_Access>::zfvAccess(obj);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfautoObject &obj) {
            ZFTypeId<zfweak>::Value<T_Access>::zfvAccessFinish(obj);
        }
    };
};
/** @endcond */

// ============================================================
template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, zfweak, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfweak const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, zfweak, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfweak c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, zfweak, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfweak const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, zfweak, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfweak c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

// ============================================================
template<typename T_ZFObject, typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastHolder<0, T_To, zfweakT<T_ZFObject>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfweakT<T_ZFObject> const &obj) {
        return ZFCastZFObject(T_To, obj.toObject());
    }
};
template<typename T_ZFObject, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastHolder<0, zfweakT<T_ZFObject>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfweakT<T_ZFObject> c(T_From obj) {
        return ZFCastZFObject(ZFObject *, obj);
    }
};

template<typename T_ZFObject, typename T_To, int T_ToType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, T_To, zfweakT<T_ZFObject>, T_ToType, _ZFP_ObjCastTypeUnknown> {
public:
    static inline T_To c(zfweakT<T_ZFObject> const &obj) {
        return ZFCastZFObjectUnchecked(T_To, obj.toObject());
    }
};
template<typename T_ZFObject, typename T_From, int T_FromType>
zfclassNotPOD _ZFP_ObjCastUncheckedHolder<0, zfweakT<T_ZFObject>, T_From, _ZFP_ObjCastTypeUnknown, T_FromType> {
public:
    static inline zfweakT<T_ZFObject> c(T_From obj) {
        return ZFCastZFObjectUnchecked(ZFObject *, obj);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfweak_h_


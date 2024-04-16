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
    zfweak(void) : _ZFP_obj(zfnull) {}
    zfweak(ZF_IN zfweak const &obj) : _ZFP_obj(zfRetain(obj._ZFP_obj)) {}
    zfweak(ZF_IN zfnullT const &) : _ZFP_obj(zfnull) {}
    template<typename T_ZFObject>
    zfweak(ZF_IN T_ZFObject *obj) : _ZFP_obj(obj ? zfRetain(obj->toObject()->objectHolder()) : zfnull) {}
    template<typename T_ZFObject>
    zfweak(ZF_IN T_ZFObject const &obj) {
        ZFObject *t = _ZFP_ZFAnyCast(T_ZFObject, obj);
        _ZFP_obj = t ? zfRetain(t->objectHolder()) : zfnull;
    }
    ~zfweak(void) {
        zfRelease(_ZFP_obj);
    }

public:
    inline zfweak &operator = (ZF_IN zfweak const &obj) {
        this->set(obj);
        return *this;
    }
    inline zfweak &operator = (ZF_IN zfnullT const &) {
        this->set(zfnull);
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
    inline zfbool operator == (ZF_IN zfweak const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfweak const &obj) const {
        return this->toObject() != obj.toObject();
    }
    inline zfbool operator == (ZF_IN zfnullT const &) const {
        return this->toObject() == zfnull;
    }
    inline zfbool operator != (ZF_IN zfnullT const &) const {
        return this->toObject() != zfnull;
    }
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return this->toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return this->toObject() != zfnull;
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
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
        return _ZFP_obj ? _ZFP_obj->objectHolded().toObject() : zfnull;
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
    /**
     * @brief set the holded object
     */
    void set(ZF_IN zfnullT const &) {
        zfRetainChange(_ZFP_obj, zfnull);
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return _ZFP_obj ? _ZFP_obj->objectHolded().toObject() : zfnull;
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
template<typename T_ZFObjectBase>
zfclassLikePOD zfweakT : zfextend zfweak {
    /** @cond ZFPrivateDoc */
public:
    zfweakT(void) : zfweak() {}
    zfweakT(ZF_IN zfnullT const &) : zfweak() {}
    template<typename T_ZFObject>
    zfweakT(ZF_IN T_ZFObject *obj) : zfweak(obj) {}
    template<typename T_ZFObject>
    zfweakT(ZF_IN T_ZFObject const &obj) : zfweak(obj) {}

public:
    inline zfbool operator == (ZF_IN zfweakT<T_ZFObjectBase> const &obj) const {
        return this->toObject() == obj.toObject();
    }
    inline zfbool operator != (ZF_IN zfweakT<T_ZFObjectBase> const &obj) const {
        return this->toObject() != obj.toObject();
    }
    inline zfbool operator == (ZF_IN zfnullT const &) const {
        return this->toObject() == zfnull;
    }
    inline zfbool operator != (ZF_IN zfnullT const &) const {
        return this->toObject() != zfnull;
    }
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject *obj) const {
        return this->toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject *obj) const {
        return this->toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
    }

public:
    inline T_ZFObjectBase operator -> (void) const {
        return ZFCastZFObjectUnchecked(T_ZFObjectBase, this->toObject());
    }
    inline ZFObject *toObject(void) const { // required for _ZFP_ZFAnyCast to work
        return _ZFP_obj ? _ZFP_obj->objectHolded() : zfnull;
    }
    /** @endcond */
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
            ZF_OUT zfauto &obj
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
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccessAvailable(obj);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccess(obj);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            ZFTypeId<zfweak>::Value<T_Access>::zfvAccessFinish(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return ZFTypeId<zfweak>::Value<T_Access>::zfvAccessAvailable(obj);
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            // zfweakT ensured safe for reinterpret cast
            return (typename zftTraits<T_Access>::TrNoRef)ZFTypeId<zfweak>::Value<T_Access>::zfvAccess(obj);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            ZFTypeId<zfweak>::Value<T_Access>::zfvAccessFinish(obj);
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfweakT<T_ZFObject> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<zfweakT<T_ZFObject> >::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(zfweakT<T_ZFObject>, Value<zfweakT<T_ZFObject> >::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfweakT<T_ZFObject> *)v);
        Value<zfweakT<T_ZFObject> >::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfweakT<T_ZFObject> >);
    }
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

/** @cond ZFPrivateDoc */
// ============================================================
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfweak const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfweak const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfweakT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_ZFAnyCast(ZFObject *, obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfweakT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_ZFAnyCast(ZFObject *, obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfweak_h_


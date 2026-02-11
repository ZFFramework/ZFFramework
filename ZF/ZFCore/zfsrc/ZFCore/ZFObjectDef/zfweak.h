/**
 * @file zfweak.h
 * @brief weak reference to ZFObject
 */

#ifndef _ZFI_zfweak_h_
#define _ZFI_zfweak_h_

#include "zfweak_fwd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
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
 *   zfweakT<ZFArray> p2 = obj;
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
 */
zfclassLikePOD ZFLIB_ZFCore zfweak {
    /** @cond ZFPrivateDoc */
public:
    zfweak(void) : _ZFP_obj(zfnull) {}
    zfweak(ZF_IN zfweak const &obj) : _ZFP_obj(zfobjRetain(obj._ZFP_obj)) {}
    template<typename T_ZFObject>
    zfweak(ZF_IN T_ZFObject const &obj) {
        ZFObject *t = _ZFP_zfanyCast(obj);
        _ZFP_obj = t ? zfobjRetain(t->_ZFP_ZFObject_weakHolder()) : zfnull;
    }
    ~zfweak(void) {
        zfobjRelease(_ZFP_obj);
    }

public:
    inline zfweak &operator = (ZF_IN zfweak const &obj) {
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
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() == _ZFP_zfanyCast(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() != _ZFP_zfanyCast(obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return this->toObject() != zfnull;
    }
    inline operator ZFObject * (void) const {
        return this->toObject();
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return zfcast(T_ZFObject *, this->toObject());
    }

public:
    static const ZFClass *ClassData(void) {
        return ZFObject::ClassData();
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
    inline zfany get(void) const {
        return _ZFP_obj ? _ZFP_obj->get().toObject() : zfnull;
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    void set(ZF_IN T_ZFObject const &obj) {
        ZFObject *t = _ZFP_zfanyCast(obj);
        _ZFP_I_zfweak *old = _ZFP_obj;
        _ZFP_obj = (t ? zfobjRetain(t->_ZFP_ZFObject_weakHolder()) : zfnull);
        zfobjRelease(old);
    }
    /**
     * @brief set the holded object
     */
    void set(ZF_IN const zfweak &obj) {
        this->set(obj.toObject());
    }
    /**
     * @brief set the holded object
     */
    void set(ZF_IN zfnullT const &) {
        _ZFP_I_zfweak *old = _ZFP_obj;
        _ZFP_obj = zfnull;
        zfobjRelease(old);
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return _ZFP_obj ? _ZFP_obj->get().toObject() : zfnull;
    }
    /**
     * @brief cast by #zfcast
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return zfcast(T_ZFObject, this->toObject());
    }

    /**
     * @brief access as #zfany
     */
    const zfany &asAny(void) const {
        return _ZFP_obj;
    }

private:
    zfanyT<_ZFP_I_zfweak> _ZFP_obj;
};
ZFOUTPUT_TYPE(zfweak, {
    if(v) {
        s += "[weak]";
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFCOMPARER_DEFAULT_DECLARE(zfweak, zfweak, {
        return ZFObjectCompare(v0.toObject(), v1.toObject());
    })
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, zfweak, zfweak)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, zfweak, zfweak)

// ============================================================
/**
 * @brief see #zfweak
 */
template<typename T_ZFObjectBase>
zfclassLikePOD zfweakT : zfextend zfweak {
    /** @cond ZFPrivateDoc */
public:
    zfweakT(void) : zfweak() {}
    zfweakT(ZF_IN zfweak const &obj) : zfweak(obj) {}
    zfweakT(ZF_IN zfweakT<T_ZFObjectBase> const &obj) : zfweak((zfweak const &)obj) {}
    template<typename T_ZFObject>
    zfweakT(ZF_IN T_ZFObject const &obj) : zfweak(obj) {}

public:
    inline zfweakT<T_ZFObjectBase> &operator = (ZF_IN zfweak const &obj) {
        this->set(obj);
        return *this;
    }
    inline zfweakT<T_ZFObjectBase> &operator = (ZF_IN zfweakT<T_ZFObjectBase> const &obj) {
        this->set(obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfweakT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj) {
        this->set(obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() == _ZFP_zfanyCast(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() != _ZFP_zfanyCast(obj);
    }

public:
    inline T_ZFObjectBase *operator -> (void) const {
        return zfcast(T_ZFObjectBase *, this->toObject());
    }

public:
    inline ZFObject *toObject(void) const { // required for _ZFP_zfanyCast to work
        return zfweak::toObject();
    }

public:
    static const ZFClass *ClassData(void) {
        return T_ZFObjectBase::ClassData();
    }
    /** @endcond */
};
ZFOUTPUT_TYPE_TEMPLATE(typename T_ZFObject, zfweakT<T_ZFObject>, {
    if(v) {
        s += "[weak]";
        v.toObject()->objectInfoT(s);
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
    static inline const zfstring &TypeId(void) {
        return ZFTypeId<zfweak>::TypeId();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return v_zfweak::ClassData();
    }
    zfoverride
    virtual zfbool typeIdSerializable(void) const {
        return TypeIdSerializable;
    }
    zfoverride
    virtual const zfstring &typeId(void) const {
        return TypeId();
    }
    zfoverride
    virtual const ZFClass *typeIdClass(void) const {
        return TypeIdClass();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN zfweakT<T_ZFObject> const &v
            ) {
        return ZFTypeId<zfweak>::ValueStore(obj, v);
    }
    template<typename T_Access = zfweakT<T_ZFObject>
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfweakT<T_ZFObject> >::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfweakT<T_ZFObject> >::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) {
            return ZFTypeId<zfweak>::Value<zfweak &>::zfvAccessAvailable(obj);
        }
        static T_Access zfvAccess(ZF_IN const zfauto &obj) {
            // zfweakT ensured safe for reinterpret cast
            return (T_Access)ZFTypeId<zfweak>::Value<zfweak &>::zfvAccess(obj);
        }
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) {
            return ZFTypeId<zfweak>::Value<zfweak &>::zfvAccessFinish(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) {
            return ZFTypeId<zfweak>::Value<zfweak *>::zfvAccessAvailable(obj);
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN const zfauto &obj) {
            // zfweakT ensured safe for reinterpret cast
            return (typename zftTraits<T_Access>::TrNoRef)ZFTypeId<zfweak>::Value<zfweak *>::zfvAccess(obj);
        }
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) {
            return ZFTypeId<zfweak>::Value<zfweak *>::zfvAccessFinish(obj);
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfweakT<T_ZFObject> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN const zfauto &obj) const {
        if(!Value<zfweakT<T_ZFObject> >::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        else {
            return _ZFP_genericAccessWrap<zfweakT<T_ZFObject> >(Value<zfweakT<T_ZFObject> >::zfvAccess(obj));
        }
    }
    zfoverride
    virtual zfauto genericAccessFinish(ZF_IN const zfauto &obj, ZF_IN void *v) const {
        return _ZFP_genericAccessFinishWrap(Value<zfweakT<T_ZFObject> >::zfvAccessFinish(obj), v, _ZFP_genericAccessFinish<zfweakT<T_ZFObject> >);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return ZFCoreArray<zfweakT<T_ZFObject> >().refNew();
    }
};
/** @endcond */

/** @cond ZFPrivateDoc */
// ============================================================
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfweak const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfweak const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}

// ============================================================
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfweakT<T_ZFObjectBase> const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject, typename T_ZFObjectBase>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfweakT<T_ZFObjectBase> const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}
/** @endcond */

// ============================================================
template<typename T_ZFObject>
zfclassNotPOD _ZFP_zfweakCkT {
public:
    static inline void zfweakNotSupport(void) {}
};
template<> zfclassNotPOD _ZFP_zfweakCkT<zfweak> {};
template<typename T_ZFObject> zfclassNotPOD _ZFP_zfweakCkT<zfweakT<T_ZFObject> > {};
template<typename T_ZFObject>
inline void _ZFP_zfweakCk(T_ZFObject &t) {
    _ZFP_zfweakCkT<typename zftTraits<T_ZFObject>::TrNoRef>::zfweakNotSupport();
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfweak_h_


/**
 * @file ZFTypeId_spec.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_spec_h_
#define _ZFI_ZFTypeId_spec_h_

#include "ZFTypeIdCore.h"
#include "ZFIOCallback.h"
#include "zfauto.h"
#include "ZFObjectAutoPtr.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// void
/** @brief see #ZFTYPEID_DECLARE */
inline const zfstring &ZFTypeId_void(void) {
    static ZFSigName d(zftext("void"));
    return d;
}

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<void> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 0,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId_void();
    }
    virtual const ZFClass *TypeIdClass(void) const {
        return zfnull;
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
    template<typename T_Access = zfint>
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static typename zftTraits<T_Access>::TrType zfvAccess(ZF_IN_OUT zfauto &obj) {
            return typename zftTraits<T_Access>::TrType();
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return zffalse;
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        return zfnull;
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnull;
    }
};
/** @endcond */

// ============================================================
// const void * / void *
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, zfptr, const void *)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, zfptr, const void *)

ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, zfptr, const void *, zfptrW, void *)
#define _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_zfptr(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
    template<typename T_Access = Type \
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, Type >::Value) ? 1 \
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, Type >::Value \
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0)) \
        , typename T_Fix = void \
        > \
    zfclassNotPOD Value { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
            return (zfcast(v_zfptr *, obj) != zfnull); \
        } \
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
            return const_cast<T_Access>(zfcast(v_zfptr *, obj)->zfv); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access, 1> { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
            return obj == zfnull || (zfcast(v_zfptr *, obj) != zfnull); \
        } \
        static typename zftTraits<T_Access>::TrNoRef const &zfvAccess(ZF_IN_OUT zfauto &obj) { \
            return obj == zfnull ? zfnull : const_cast<typename zftTraits<T_Access>::TrNoRef>(zfcast(v_zfptr *, obj)->zfv); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access, 2> { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
            return obj == zfnull || (zfcast(v_zfptr *, obj) != zfnull); \
        } \
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
            return obj == zfnull ? zfnull : const_cast<T_Access>(zfcast(v_zfptr *, obj)->zfv); \
        } \
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
        } \
    };
ZFTYPEID_ALIAS_REG_CUSTOM(ZFLIB_ZFCore, zfptr, const void *, zfptrW, void *, _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_zfptr)
#undef _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_zfptr

// ============================================================
// ZFObject
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type, 1, 1> : zfextend ZFTypeInfo {
private:
    typedef typename zftTraits<T_Type>::TrType _ZFP_T_ZFObject;
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfstring &TypeId(void) {
        return zftTraits<T_Type>::TrType::ClassData()->classNameFull();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return _ZFP_T_ZFObject::ClassData();
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
            , ZF_IN T_Type const &v
            ) {
        obj = zfcast(ZFObject *, v);
        return zftrue;
    }
    template<typename T_Access = T_Type
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, T_Type>::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, T_Type>::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return (obj == zfnull || zfcast(T_Type, obj) != zfnull);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                static T_Type _d = zfnull;
                return _d;
            }
            T_Type *holder = zfnew(T_Type, zfcast(T_Type, obj));
            _ZFP_PropAliasAttach(obj, holder,
                zfstr("_ZFP_PropAlias:%s:%s",
                    zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj) {
                _ZFP_PropAliasDetach(obj,
                    zfstr("_ZFP_PropAlias:%s:%s",
                        zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                        zftTraits<T_Access>::ModifierName())
                    );
            }
        }
    private:
        static void _ZFP_PropAliasOnDetach(
                ZF_IN ZFObject *obj
                , ZF_IN void *v
                ) {
            T_Type *vTmp = (T_Type *)v;
            zfdelete(vTmp);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    private:
        typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef;
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return (obj == zfnull || zfcast(T_Type, obj) != zfnull);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                return zfnull;
            }
            _TrNoRef *holder = zfnew(_TrNoRef);
            *holder = zfcast(T_Type, obj);
            _ZFP_PropAliasAttach(obj, holder,
                zfstr("_ZFP_PropAlias:%s:%s",
                    _ZFP_T_ZFObject::ClassData()->classNameFull(),
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj) {
                _ZFP_PropAliasDetach(obj,
                    zfstr("_ZFP_PropAlias:%s:%s",
                        _ZFP_T_ZFObject::ClassData()->classNameFull(),
                        zftTraits<T_Access>::ModifierName())
                    );
            }
        }
    private:
        static void _ZFP_PropAliasOnDetach(
                ZF_IN ZFObject *obj
                , ZF_IN void *v
                ) {
            _TrNoRef *vTmp = (_TrNoRef *)v;
            zfdelete(vTmp);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 2> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return (obj == zfnull || zfcast(T_Type, obj) != zfnull);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const T_Type *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<T_Type>::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(T_Type, Value<T_Type>::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((T_Type *)v);
        Value<T_Type>::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<T_Type>);
    }
};
/** @endcond */

// ============================================================
// zfauto
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFCore, zfauto)

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<zfauto> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfstring &TypeId(void) {
        return ZFObject::ClassData()->classNameFull();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return ZFObject::ClassData();
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
            , ZF_IN zfauto const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfauto
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfauto>::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfauto>::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return &obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfauto *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<zfauto>::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(zfauto, Value<zfauto>::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfauto *)v);
        Value<zfauto>::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfauto>);
    }
};
/** @endcond */

// ============================================================
// zfautoT
ZFOUTPUT_TYPE_TEMPLATE(typename T_ZFObject, zfautoT<T_ZFObject>, {
    if(v) {
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})

/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfclassNotPOD ZFTypeId<zfautoT<T_ZFObject>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId<T_ZFObject *>::TypeId();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return T_ZFObject::ClassData();
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
            , ZF_IN zfautoT<T_ZFObject> const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfautoT<T_ZFObject>
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfautoT<T_ZFObject> >::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfautoT<T_ZFObject> >::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return (T_Access)obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            /* ZFTAG_TRICKS: zfautoT ensured safe for reinterpret cast */
            return (typename zftTraits<T_Access>::TrNoRef)&obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfautoT<T_ZFObject> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<zfautoT<T_ZFObject> >::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(zfautoT<T_ZFObject>, Value<zfautoT<T_ZFObject> >::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfautoT<T_ZFObject> *)v);
        Value<zfautoT<T_ZFObject> >::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfautoT<T_ZFObject> >);
    }
};
/** @endcond */

// ============================================================
// zfobj
ZFOUTPUT_TYPE_TEMPLATE(typename T_ZFObject, zfobj<T_ZFObject>, {
    if(v) {
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})

// ============================================================
// zfany
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFCore, zfany)

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<zfany> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfstring &TypeId(void) {
        return ZFObject::ClassData()->classNameFull();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return ZFObject::ClassData();
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
            , ZF_IN zfany const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfany
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfany>::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfany>::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return const_cast<zfany &>(obj.asAny());
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return zfnull;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfany *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        return (void *)zfnew(zfany, obj);
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfany *)v);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfany>);
    }
};
/** @endcond */

// ============================================================
// zfanyT
ZFOUTPUT_TYPE_TEMPLATE(typename T_ZFObject, zfanyT<T_ZFObject>, {
    if(v) {
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})

/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfclassNotPOD ZFTypeId<zfanyT<T_ZFObject>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId<T_ZFObject *>::TypeId();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return T_ZFObject::ClassData();
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
            , ZF_IN zfanyT<T_ZFObject> const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfanyT<T_ZFObject>
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfanyT<T_ZFObject> >::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, zfanyT<T_ZFObject> >::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            /* ZFTAG_TRICKS: zfanyT ensured safe for reinterpret cast */
            return (T_Access)const_cast<zfany &>(obj.asAny());
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return zfnull;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfanyT<T_ZFObject> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        return (void *)zfnew(zfanyT<T_ZFObject>, obj);
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfanyT<T_ZFObject> *)v);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfanyT<T_ZFObject> >);
    }
};
/** @endcond */

// ============================================================
// pointer type
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type *, 0, 1> : zfextend ZFTypeInfo {
public:
    typedef T_Type * T_TypePtr;
    typedef typename zftTraits<T_Type *>::TrType T_Type_;
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type_>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type_>::TypeIdSerializable,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId<T_Type_>::TypeId();
    }
    static inline const ZFClass *TypeIdClass(void) {
        return ZFTypeId<T_Type_>::TypeIdClass();
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
    static zfbool ValueStore(ZF_OUT zfauto &obj, T_Type * const &v) {
        if(v == zfnull) {
            obj = zfnull;
            return zftrue;
        }
        else {
            return ZFTypeId<T_Type_>::ValueStore(obj, *v);
        }
    }
    static zfbool ValueStore(ZF_OUT zfauto &obj, T_Type_ const &v) {
        return ZFTypeId<T_Type_>::ValueStore(obj, *v);
    }
    template<typename T_Access = T_Type *
        , int T_Mode = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && !zftIsSame<typename zftTraits<T_Access>::TrNoRef, T_Type *>::Value) ? 1
            : ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
                && zftIsSame<typename zftTraits<T_Access>::TrNoRef, T_Type *>::Value
                && !zftTraits<T_Access>::TrIsRef) ? 2 : 0))
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessAvailable(obj);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccess(obj);
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessFinish(obj);
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return ((obj == zfnull) || ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessAvailable(obj));
        }
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return ((obj == zfnull) ? zfnull : ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccess(obj));
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj != zfnull) {
                ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessFinish(obj);
            }
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 2> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return ((obj == zfnull) || ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessAvailable(obj));
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            return ((obj == zfnull) ? zfnull : ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccess(obj));
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj != zfnull) {
                ZFTypeId<T_Type_>::template Value<T_Access>::zfvAccessFinish(obj);
            }
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const T_TypePtr *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        if(!Value<T_TypePtr>::zfvAccessAvailable(obj)) {
            return zfnull;
        }
        return (void *)zfnew(T_TypePtr, Value<T_TypePtr>::zfvAccess(obj));
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((T_TypePtr *)v);
        Value<T_TypePtr>::zfvAccessFinish(obj);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<T_TypePtr>);
    }
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_spec_h_


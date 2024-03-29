/**
 * @file ZFTypeId_spec.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeId_spec_h_
#define _ZFI_ZFTypeId_spec_h_

#include "ZFTypeIdCore.h"
#include "ZFIOCallback.h"
#include "zfauto.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// void
/** @brief see #ZFTYPEID_DECLARE */
#define ZFTypeId_void() "void"

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<void> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 0,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId_void();
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
        return zfnull;
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
};
/** @endcond */

// ============================================================
// ZFObject
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type, 1, 1> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void) {
        return zftTraits<T_Type>::TrType::ClassData()->classNameFull();
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
        return typename zftTraits<T_Type>::TrType::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN T_Type const &v
            ) {
        obj = ZFCastZFObjectUnchecked(ZFObject *, v);
        return zftrue;
    }
    template<typename T_Access = T_Type
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    T_Type
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return (obj == zfnull || ZFCastZFObject(T_Type, obj) != zfnull);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                static T_Type _d = zfnull;
                return _d;
            }
            T_Type *holder = zfnew(T_Type, ZFCastZFObject(T_Type, obj));
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(
                    zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                    ":",
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                return;
            }
            _ZFP_PropAliasDetach(obj,
                zfsConnectLineFree(
                    zftTraits<T_Type>::TrType::ClassData()->classNameFull(),
                    ":",
                    zftTraits<T_Access>::ModifierName())
                );
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
            return (obj == zfnull || ZFCastZFObject(T_Type, obj) != zfnull);
        }
        static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                static T_Type *_d = zfnull;
                return _d;
            }
            _TrNoRef *holder = zfnew(_TrNoRef);
            *holder = ZFCastZFObject(T_Type, obj);
            _ZFP_PropAliasAttach(obj, holder,
                zfsConnectLineFree(
                    typename zftTraits<_TrNoRef>::TrType::ClassData()->classNameFull(),
                    ":",
                    zftTraits<T_Access>::ModifierName()),
                _ZFP_PropAliasOnDetach);
            return *holder;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
            if(obj == zfnull) {
                return;
            }
            _ZFP_PropAliasAttach(obj,
                zfsConnectLineFree(
                    typename zftTraits<_TrNoRef>::TrType::ClassData()->classNameFull(),
                    ":",
                    zftTraits<T_Access>::ModifierName())
                );
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
    static inline const zfchar *TypeId(void) {
        return ZFObject::ClassData()->classNameFull();
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
        return ZFObject::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN zfauto const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfauto
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfauto
                >::TypeIsSame != 1)
            ? 1 : 0)
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
};
/** @endcond */

// ============================================================
// zfautoT
/** @cond ZFPrivateDoc */
template<typename T_ZFObject>
zfclassNotPOD ZFTypeId<zfautoT<T_ZFObject>, 0, 0> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId<T_ZFObject>::TypeId();
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
        return typename zftTraits<T_ZFObject>::TrType::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN zfautoT<T_ZFObject> const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfautoT<T_ZFObject>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfautoT<T_ZFObject>
                >::TypeIsSame != 1)
            ? 1 : 0)
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
            // zfautoT ensured safe for reinterpret cast
            return (typename zftTraits<T_Access>::TrNoRef)&obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
};
/** @endcond */

// ============================================================
// ZFAny
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFCore, ZFAny)

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<ZFAny> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 1,
    };
    static inline const zfchar *TypeId(void) {
        return ZFObject::ClassData()->classNameFull();
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
        return ZFObject::ClassData();
    }
    static zfbool ValueStore(
            ZF_OUT zfauto &obj
            , ZF_IN ZFAny const &v
            ) {
        obj = v.toObject();
        return zftrue;
    }
    template<typename T_Access = ZFAny
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    ZFAny
                >::TypeIsSame != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static ZFAny zfvAccess(ZF_IN_OUT zfauto &obj) {
            return obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Access>
    zfclassNotPOD Value<T_Access, 1> {
    private:
        typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef;
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static const ZFAny *zfvAccess(ZF_IN_OUT zfauto &obj) {
            return zfnull;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
};
/** @endcond */

// ============================================================
// pointer type
/** @cond ZFPrivateDoc */
template<typename T_Type>
zfclassNotPOD ZFTypeId<T_Type *, 0, 1> : zfextend ZFTypeInfo {
public:
    typedef typename zftTraits<T_Type *>::TrType T_Type_;
public:
    enum {
        TypeIdRegistered = ZFTypeId<T_Type_>::TypeIdRegistered,
        TypeIdSerializable = ZFTypeId<T_Type_>::TypeIdSerializable,
    };
    static inline const zfchar *TypeId(void) {
        return ZFTypeId<T_Type_>::TypeId();
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
        ZFTypeId<T_Type_> t;
        return t.typeIdClass();
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
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftTypeIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    T_Type_
                >::TypeIsSame != 1)
            ? 1 : 0)
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
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_spec_h_


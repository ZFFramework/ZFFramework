/**
 * @file zfweak.h
 * @brief weak reference to ZFObject
 */

#ifndef _ZFI_zfweak_h_
#define _ZFI_zfweak_h_

#include "zfweak_def.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #zfweak */
inline const zfstring &ZFTypeId_zfweak(void) {
    static ZFSigName d("zfweak");
    return d;
}

// ============================================================
// zfweak
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

/** @cond ZFPrivateDoc */
template<>
zfclassNotPOD ZFTypeId<zfweak> : zfextend ZFTypeInfo {
public:
    enum {
        TypeIdRegistered = 1,
        TypeIdSerializable = 0,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId_zfweak();
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
            , ZF_IN zfweak const &v
            ) {
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfweak
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfweak
                >::Value != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static zfweak zfvAccess(ZF_IN_OUT zfauto &obj) {
            return obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Fix>
    zfclassNotPOD Value<zfweak &, 0, T_Fix> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static zfweak &zfvAccess(ZF_IN_OUT zfauto &obj) {
            static zfweak _d;
            return _d;
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
        static _TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return zfnull;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfweak *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        return (void *)zfnew(zfweak, obj);
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfweak *)v);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfweak>);
    }
};
/** @endcond */

// ============================================================
// zfweakT
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
        TypeIdSerializable = 0,
    };
    static inline const zfstring &TypeId(void) {
        return ZFTypeId_zfweak();
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
        obj = v;
        return zftrue;
    }
    template<typename T_Access = zfweakT<T_ZFObject>
        , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr
            && zftIsSame<
                    typename zftTraits<T_Access>::TrNoRef,
                    zfweakT<T_ZFObject>
                >::Value != 1)
            ? 1 : 0)
        , typename T_Fix = void
        >
    zfclassNotPOD Value {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zftrue;
        }
        static zfweakT<T_ZFObject> zfvAccess(ZF_IN_OUT zfauto &obj) {
            return obj;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
    template<typename T_Fix>
    zfclassNotPOD Value<zfweakT<T_ZFObject> &, 0, T_Fix> {
    public:
        static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) {
            return zffalse;
        }
        static zfweakT<T_ZFObject> &zfvAccess(ZF_IN_OUT zfauto &obj) {
            static zfweakT<T_ZFObject> _d;
            return _d;
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
        static _TrNoRef zfvAccess(ZF_IN_OUT zfauto &obj) {
            return zfnull;
        }
        static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) {
        }
    };
public:
    zfoverride
    virtual zfbool genericValueStore(ZF_OUT zfauto &obj, ZF_IN const void *v) const {
        return ValueStore(obj, *(const zfweakT<T_ZFObject> *)v);
    }
    zfoverride
    virtual void *genericAccess(ZF_IN_OUT zfauto &obj) const {
        return (void *)zfnew(zfweakT<T_ZFObject>, obj);
    }
    zfoverride
    virtual void genericAccessFinish(ZF_IN_OUT zfauto &obj, ZF_IN void *v) const {
        zfdelete((zfweakT<T_ZFObject> *)v);
    }
    zfoverride
    virtual ZFCoreArrayBase *genericArrayNew(void) const {
        return zfnew(ZFCoreArray<zfweakT<T_ZFObject> >);
    }
};
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfweak_h_


/**
 * @file ZFEnumDeclarePropType.h
 * @brief ZFEnum declare impl
 */

#ifndef _ZFI_ZFEnumDeclarePropType_h_
#define _ZFI_ZFEnumDeclarePropType_h_

#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// normal enum
#define _ZFP_ZFENUM_PROP_TYPE_DECLARE(ZFLIB_, EnumName) \
    ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, EnumName, EnumName##Enum) \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<EnumName##Enum> : zfextend ZFTypeInfo { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfchar *TypeId(void) { \
            return ZFTypeId_##EnumName(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfchar *typeId(void) const { \
            return TypeId(); \
        } \
        zfoverride \
        virtual const ZFClass *typeIdClass(void) const { \
            return EnumName::ClassData(); \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN zfuint const &v \
                ) { \
            zfCoreMutexLock(); \
            EnumName *t = zfunsafe_zfAlloc(EnumName, v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            zfCoreMutexUnlock(); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN EnumName##Enum const &v \
                ) { \
            zfCoreMutexLock(); \
            EnumName *t = zfunsafe_zfAlloc(EnumName, v); \
            obj.zfunsafe_assign(t); \
            zfunsafe_zfRelease(t); \
            zfCoreMutexUnlock(); \
            return zftrue; \
        } \
        template<typename T_Access = EnumName##Enum \
            , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        EnumName##Enum \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (ZFCastZFObject(EnumName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                return *(typename zftTraits<T_Access>::TrNoRef *)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        private: \
             typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (ZFCastZFObject(EnumName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                EnumName *t = ZFCastZFObject(EnumName *, obj); \
                _TrNoRef *holder = zfnew(_TrNoRef); \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                *holder = (_TrNoRef)(&(t->_ZFP_ZFEnum_value)); \
                _ZFP_PropAliasAttach(obj, holder \
                    , zfsConnectLineFree(ZFM_TOSTRING(EnumName), ":", zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                return *holder; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
                _ZFP_PropAliasDetach(obj \
                    , zfsConnectLineFree(ZFM_TOSTRING(EnumName), ":", zftTraits<T_Access>::ModifierName()) \
                    ); \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach( \
                    ZF_IN ZFObject *obj \
                    , ZF_IN void *v \
                    ) { \
                _TrNoRef *vTmp = (_TrNoRef *)v; \
                zfdelete(vTmp); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFENUM_PROP_TYPE_DEFINE(EnumName) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(EnumName, EnumName##Enum, { \
            if(zfsncmp(src, ZFEnumNameInvalid(), srcLen) == 0) { \
                v = (EnumName##Enum)ZFEnumInvalid(); \
                return zftrue; \
            } \
            v = (EnumName##Enum)EnumName::EnumValueForName( \
                    (srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString() \
                ); \
            return ((zfuint)v != ZFEnumInvalid()); \
        }, { \
            s += EnumName::EnumNameForValue(v); \
            return zftrue; \
        }) \
    const zfchar *EnumName::wrappedValueTypeId(void) { \
        return ZFTypeId_##EnumName(); \
    }


// ============================================================
// enum flags
#define _ZFP_ZFENUM_FLAGS_PROP_TYPE_DECLARE(ZFLIB_, EnumName, EnumFlagsName) \
    ZFTYPEID_DECLARE_WITH_CUSTOM_WRAPPER(ZFLIB_, EnumFlagsName, EnumFlagsName) \
    /** @brief type wrapper for #ZFTypeId::Value */ \
    zfclass ZFLIB_ v_##EnumFlagsName : zfextend EnumName { \
        ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(v_##EnumFlagsName, EnumName) \
        ZFALLOC_CACHE_RELEASE({ \
            cache->wrappedValueReset(); \
        }) \
    public: \
        zfoverride \
        virtual const zfchar *wrappedValueTypeId(void) { \
            return ZFTypeId_##EnumFlagsName(); \
        } \
    public: \
        /** @brief the value, see #ZFTypeId::Value */ \
        zfuint &zfv; \
    protected: \
       v_##EnumFlagsName(void) : zfv(_ZFP_ZFEnum_value) {} \
    }; \
    /** @cond ZFPrivateDoc */ \
    template<> \
    zfclassNotPOD ZFTypeId<EnumFlagsName> : zfextend ZFTypeInfo { \
    public: \
        enum { \
            TypeIdRegistered = 1, \
            TypeIdSerializable = 1, \
        }; \
        static inline const zfchar *TypeId(void) { \
            return ZFTypeId_##EnumFlagsName(); \
        } \
        zfoverride \
        virtual zfbool typeIdSerializable(void) const { \
            return TypeIdSerializable; \
        } \
        zfoverride \
        virtual const zfchar *typeId(void) const { \
            return TypeId(); \
        } \
        zfoverride \
        virtual const ZFClass *typeIdClass(void) const { \
            return v_##EnumFlagsName::ClassData(); \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN zfuint const &v \
                ) { \
            zfCoreMutexLock(); \
            v_##EnumFlagsName *t = zfunsafe_zfAlloc(v_##EnumFlagsName); \
            t->zfv = v; \
            obj = t; \
            zfunsafe_zfRelease(t); \
            zfCoreMutexUnlock(); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN EnumName##Enum const &v \
                ) { \
            EnumName *t = zfAlloc(EnumName, (zfuint)v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        static zfbool ValueStore( \
                ZF_OUT zfauto &obj \
                , ZF_IN EnumFlagsName const &v \
                ) { \
            EnumName *t = zfAlloc(EnumName, (zfuint)v); \
            obj = t; \
            zfRelease(t); \
            return zftrue; \
        } \
        template<typename T_Access = EnumFlagsName \
            , int T_IsPointer = ((zftTraits<typename zftTraits<T_Access>::TrNoRef>::TrIsPtr \
                && zftTypeIsSame< \
                        typename zftTraits<T_Access>::TrNoRef, \
                        EnumFlagsName \
                    >::TypeIsSame != 1) \
                ? 1 : 0) \
            , typename T_Fix = void \
            > \
        zfclassNotPOD Value { \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (ZFCastZFObject(EnumName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                return *(typename zftTraits<T_Access>::TrNoRef *)(&(ZFCastZFObject(EnumName *, obj)->_ZFP_ZFEnum_value)); \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
            } \
        }; \
        template<typename T_Access> \
        zfclassNotPOD Value<T_Access, 1> { \
        private: \
            typedef typename zftTraits<T_Access>::TrNoRef _TrNoRef; \
        public: \
            static zfbool zfvAccessAvailable(ZF_IN_OUT zfauto &obj) { \
                return (ZFCastZFObject(EnumName *, obj) != zfnull); \
            } \
            static T_Access zfvAccess(ZF_IN_OUT zfauto &obj) { \
                EnumName *t = ZFCastZFObject(EnumName *, obj); \
                _TrNoRef *holder = zfnew(_TrNoRef); \
                /* ZFTAG_TRICKS: EnumReinterpretCast */ \
                *holder = (_TrNoRef)(&(t->_ZFP_ZFEnum_value)); \
                _ZFP_PropAliasAttach(obj, holder \
                    , zfsConnectLineFree(ZFM_TOSTRING(EnumName), ":", zftTraits<T_Access>::ModifierName()) \
                    , _ZFP_PropAliasOnDetach \
                    ); \
                return *holder; \
            } \
            static void zfvAccessFinish(ZF_IN_OUT zfauto &obj) { \
                _ZFP_PropAliasDetach(obj \
                    , zfsConnectLineFree(ZFM_TOSTRING(EnumName), ":", zftTraits<T_Access>::ModifierName()) \
                    ); \
            } \
        private: \
            static void _ZFP_PropAliasOnDetach( \
                    ZF_IN ZFObject *obj \
                    , ZF_IN void *v \
                    ) { \
                _TrNoRef *vTmp = (_TrNoRef *)v; \
                zfdelete(vTmp); \
            } \
        }; \
    }; \
    /** @endcond */

#define _ZFP_ZFENUM_FLAGS_PROP_TYPE_DEFINE(EnumName, EnumFlagsName) \
    ZFTYPEID_DEFINE_BY_STRING_CONVERTER_WITH_CUSTOM_WRAPPER(EnumFlagsName, EnumFlagsName, { \
            zfflags flags = zfflagsZero(); \
            if(!zfflagsFromString(flags, \
                        EnumName::ClassData(), \
                        src, srcLen)) { \
                return zffalse; \
            } \
            v.enumValue((zfuint)flags); \
            return zftrue; \
        }, { \
            return zfflagsToString(s, EnumName::ClassData(), (zfflags)v.enumValue()); \
        }) \
    ZFOBJECT_REGISTER(v_##EnumFlagsName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDeclarePropType_h_


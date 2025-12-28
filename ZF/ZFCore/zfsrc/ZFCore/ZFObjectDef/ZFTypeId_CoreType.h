/**
 * @file ZFTypeId_CoreType.h
 * @brief type define for #ZFTYPEID_DECLARE
 */

#ifndef _ZFI_ZFTypeId_CoreType_h_
#define _ZFI_ZFTypeId_CoreType_h_

#include "ZFTypeId_spec.h"
#include "ZFMethodDynamicRegister.h"
#include "ZFPropertyDynamicRegister.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// string conversion spec
/** @cond ZFPrivateDoc */
inline zfbool zfstringToStringT(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    s += v;
    return zftrue;
}
inline zfstring zfstringToString(
        ZF_IN const zfchar *v
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    return zfstring(v);
}
inline zfbool zfstringFromStringT(
        ZF_OUT const zfchar *&v
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        ) {
    if(srcLen != zfindexMax() && src[srcLen] != '\0') {
        return zffalse;
    }
    v = src;
    return zftrue;
}

extern ZFLIB_ZFCore zfbool zfstringFromDataT(
        ZF_OUT const zfchar * &propertyValue
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint = zfnull
        );
extern ZFLIB_ZFCore zfbool zfstringToDataT(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const zfchar * const &propertyValue
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        );
/** @endcond */

/**
 * @brief convert flags to string using flagList and nameList
 *
 * result would be like "Flag0|Flag1|0x123ABC"\n
 * if flags is 0 and there's no 0 value in flagList, or error occurred, "0x0" would be returned
 * params:
 * -  includeNotConverted
 *   if true, values not recognized would be appended to result as HEX value
 * -  exclusiveMode if true, when a flag recognized,
 *   flags would be unset and can't be recognized any more\n
 *   for example, if we have two flag 0101 and 0011 in flagList which shares 0001 bit,
 *   then if we are converting 0111, since 0101 would be recognized first
 *   and leave 0010, 0011 won't be recognized any more
 * -  notConverted if all value recognized, notConverted would be set to 0,
 *   otherwise, the not converted value would be set
 */
extern ZFLIB_ZFCore zfbool zfflagsToStringT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfflags *flagList
        , ZF_IN const zfchar **nameList
        , ZF_IN zfindex listCount
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted = zftrue
        , ZF_IN_OPT zfbool exclusiveMode = zffalse
        , ZF_OUT_OPT zfflags *notConverted = zfnull
        , ZF_IN_OPT zfchar separatorToken = '|'
        );
/** @brief see #zfflagsToStringT */
inline zfstring zfflagsToString(
        ZF_IN const zfflags *flagList
        , ZF_IN const zfchar **nameList
        , ZF_IN zfindex listCount
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted = zftrue
        , ZF_IN_OPT zfbool exclusiveMode = zffalse
        , ZF_OUT_OPT zfflags *notConverted = zfnull
        , ZF_IN_OPT zfchar separatorToken = '|'
        ) {
    zfstring ret;
    zfflagsToStringT(ret, flagList, nameList, listCount, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}

/**
 * @brief convert string to zfflags
 *
 * note, a non converted format is not regarded as error
 */
extern ZFLIB_ZFCore zfbool zfflagsFromStringT(
        ZF_OUT zfflags &ret
        , ZF_IN const zfflags *flagList
        , ZF_IN const zfchar **nameList
        , ZF_IN zfindex listCount
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfchar separatorToken = '|'
        );

// ============================================================
// common types
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbool value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfbool, zfbool)
ZFTYPEID_REG(ZFLIB_ZFCore, zfbool, zfbool)

/**
 * @brief a dummy class that wraps #v_zfbool
 *
 * for script language, #v_zfbool may (or may not) be automatically converted to native boolean type,
 * which make it not posible to achieve "out param" for #zfbool,
 * for example, `void func(zfbool *outParam);`\n
 * this dummy class is designed to solve this problem, an example for lua:
 * @code
 *   -- assume you have a reflectable cpp method with this proto type:
 *   --     void func(zfbool *p) {
 *   --         *p = zffalse;
 *   --     }
 *   -- this code would print:
 *   --     p0 = true
 *   --     p1 = true
 *   --     p2 = false
 *   -- note: only zfboolHolder can holds result passed from out param
 *   local p0 = zftrue
 *   local p1 = zfbool(zftrue)
 *   local p2 = zfboolHolder(zftrue)
 *   func(p0); ZFLogTrim('p0 = %s', p0);
 *   func(p1); ZFLogTrim('p1 = %s', p1);
 *   func(p2); ZFLogTrim('p2 = %s', p2);
 *
 *   -- however, since zfboolHolder is ensured object type now,
 *   -- you must use `p2:zfv()` to access it's boolean value:
 *   if p2:zfv() then
 *       ZFLogTrim('p2 = true')
 *   else
 *       ZFLogTrim('p2 = false')
 *   end
 * @endcode
 */
zfclass ZFLIB_ZFCore v_zfboolHolder : zfextend v_zfbool {
    ZFOBJECT_DECLARE(v_zfboolHolder, v_zfbool)
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfbyte value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfbyte, zfbyte)
ZFTYPEID_REG(ZFLIB_ZFCore, zfbyte, zfbyte)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfchar value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfchar, zfchar)
ZFTYPEID_REG(ZFLIB_ZFCore, zfchar, zfchar)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfstring value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfstring, zfstring)
ZFTYPEID_REG(ZFLIB_ZFCore, zfstring, zfstring)

ZFTYPEID_ALIAS_DECLARE(ZFLIB_ZFCore, zfstring, zfstring, cString, const zfchar *)
#define _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_cString(ZFLIB_, AliasToTypeName, AliasToType, TypeName, Type) \
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
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
            return obj == zfnull || (zfcast(v_zfstring *, obj) != zfnull); \
        } \
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN const zfauto &obj) { \
            return obj == zfnull ? zfnull : zfcast(v_zfstring *, obj)->zfv.cString(); \
        } \
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
            return zfnull; \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access, 1> { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
            return zffalse; \
        } \
        static typename zftTraits<T_Access>::TrNoRef zfvAccess(ZF_IN const zfauto &obj) { \
            return zfnull; \
        } \
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
            return zfnull; \
        } \
    }; \
    template<typename T_Access> \
    zfclassNotPOD Value<T_Access, 2> { \
    public: \
        static zfbool zfvAccessAvailable(ZF_IN const zfauto &obj) { \
            return obj == zfnull || (zfcast(v_zfstring *, obj) != zfnull); \
        } \
        static T_Access zfvAccess(ZF_IN const zfauto &obj) { \
            return obj == zfnull ? zfnull : zfcast(v_zfstring *, obj)->zfv.cString(); \
        } \
        static zfauto zfvAccessFinish(ZF_IN const zfauto &obj) { \
            return zfnull; \
        } \
    };
ZFTYPEID_ALIAS_REG_CUSTOM(ZFLIB_ZFCore, zfstring, zfstring, cString, const zfchar *, _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_cString)
#undef _ZFP_ZFTYPEID_ALIAS_VALUE_ACCESS_cString

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfint value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfint, zfint)
ZFTYPEID_REG(ZFLIB_ZFCore, zfint, zfint)
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfuint value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfuint, zfuint)
ZFTYPEID_REG(ZFLIB_ZFCore, zfuint, zfuint)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zflong value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zflong, zflong)
ZFTYPEID_REG(ZFLIB_ZFCore, zflong, zflong)
/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfulong value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfulong, zfulong)
ZFTYPEID_REG(ZFLIB_ZFCore, zfulong, zfulong)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfindex value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfindex, zfindex)
ZFTYPEID_REG(ZFLIB_ZFCore, zfindex, zfindex)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zffloat value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zffloat, zffloat)
ZFTYPEID_REG(ZFLIB_ZFCore, zffloat, zffloat)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfdouble value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfdouble, zfdouble)
ZFTYPEID_REG(ZFLIB_ZFCore, zfdouble, zfdouble)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zftimet value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zftimet, zftimet)
ZFTYPEID_REG(ZFLIB_ZFCore, zftimet, zftimet)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfflags value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfflags, zfflags)
ZFTYPEID_REG(ZFLIB_ZFCore, zfflags, zfflags)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <zfidentity value="value" />
 * @endcode
 * @note for portability of unsigned types,
 *   we'll use "-1" to store max unsigned value
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, zfidentity, zfidentity)
ZFTYPEID_REG(ZFLIB_ZFCore, zfidentity, zfidentity)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCompareResult value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFCompareResult, ZFCompareResult)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFCompareResult, ZFCompareResult)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSeekPos value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFSeekPos, ZFSeekPos)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFSeekPos, ZFSeekPos)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFIndexRange
 *       value="(0, 0)" // optional, 0 by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFIndexRange, ZFIndexRange)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFIndexRange, ZFIndexRange)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFLevel value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFLevel, ZFLevel)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFLevel, ZFLevel)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFFrameworkState value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFFrameworkState, ZFFrameworkState)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFFrameworkState, ZFFrameworkState)

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFFuncAddrType, ZFFuncAddrType)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFFuncAddrType, ZFFuncAddrType)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, zfiter, zfiter)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, zfiter, zfiter)
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFCallerInfo, ZFCallerInfo)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFCallerInfo, ZFCallerInfo)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFIdentityGenerator, ZFIdentityGenerator)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFIdentityGenerator, ZFIdentityGenerator)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFPathInfo value="pathType!!pathData" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFPathInfo, ZFPathInfo)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFPathInfo, ZFPathInfo)
ZFOUTPUT_TYPE(ZFPathInfo, {ZFPathInfoToStringT(s, v);})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFPathInfo_separator "!!"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTokenForContainer // all tokens are optional
 *       tokenLeft=""
 *       tokenRight=""
 *       tokenSeparator=""
 *       tokenValueLeft=""
 *       tokenValueRight=""
 *       tokenEtc=""
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainer)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFTokenForContainer, ZFTokenForContainer)
ZFOUTPUT_TYPE(ZFTokenForContainer, {ZFTokenForContainerToStringT(s, v);})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenLeft "tokenLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenRight "tokenRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator "tokenSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft "tokenValueLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight "tokenValueRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForContainer_tokenEtc "tokenEtc"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTokenForKeyValueContainer // all tokens are optional
 *       tokenLeft=""
 *       tokenRight=""
 *       tokenSeparator=""
 *       tokenPairLeft=""
 *       tokenPairRight=""
 *       tokenPairSeparator=""
 *       tokenKeyLeft=""
 *       tokenKeyRight=""
 *       tokenValueLeft=""
 *       tokenValueRight=""
 *       tokenEtc=""
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainer)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainer)
ZFOUTPUT_TYPE(ZFTokenForKeyValueContainer, {ZFTokenForKeyValueContainerToStringT(s, v);})
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft "tokenLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight "tokenRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator "tokenSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft "tokenPairLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight "tokenPairRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator "tokenPairSeparator"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft "tokenKeyLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight "tokenKeyRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft "tokenValueLeft"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight "tokenValueRight"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc "tokenEtc"

// ============================================================
// ZFObject types
/**
 * @brief see #ZFTYPEID_DECLARE
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFClassDataUpdateType, ZFClassDataUpdateType)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFClassDataUpdateType, ZFClassDataUpdateType)
ZFOUTPUT_TYPE(ZFClassDataUpdateType, {ZFClassDataUpdateTypeToStringT(s, v);})
/**
 * @brief see #ZFTYPEID_DECLARE
 */
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFClassDataUpdateData, ZFClassDataUpdateData)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFClassDataUpdateData, ZFClassDataUpdateData)
ZFOUTPUT_TYPE(ZFClassDataUpdateData, {v.objectInfoT(s);})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethodAccessType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFMethodAccessType, ZFMethodAccessType)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFMethodAccessType, ZFMethodAccessType)
ZFOUTPUT_TYPE(ZFMethodAccessType, {ZFMethodAccessTypeToStringT(s, v);})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFMethodType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFMethodType, ZFMethodType)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFMethodType, ZFMethodType)
ZFOUTPUT_TYPE(ZFMethodType, {ZFMethodTypeToStringT(s, v);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFArgs, ZFArgs)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFArgs, ZFArgs)
ZFOUTPUT_TYPE(ZFArgs, {v.objectInfoT(s);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFObserver, ZFObserver)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFObserver, ZFObserver)
ZFOUTPUT_TYPE(ZFObserver, {v.objectInfoT(s);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFMethodGenericInvoker, ZFMethodGenericInvoker)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFMethodGenericInvoker, ZFMethodGenericInvoker)

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFMethodDynamicRegisterParam, ZFMethodDynamicRegisterParam)
ZFOUTPUT_TYPE(ZFMethodDynamicRegisterParam, {v.objectInfoT(s);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFMP, ZFMP)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFMP, ZFMP)
ZFOUTPUT_TYPE(ZFMP, {v.objectInfoT(s);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFPropertyCallbackDynamicRegisterInitValueGetter, ZFPropertyCallbackDynamicRegisterInitValueGetter)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFPropertyCallbackDynamicRegisterInitValueGetter, ZFPropertyCallbackDynamicRegisterInitValueGetter)
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam)
ZFOUTPUT_TYPE(ZFPropertyDynamicRegisterParam, {v.objectInfoT(s);})

ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFComparer_ZFObject, ZFComparer<ZFObject *>::Comparer)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFComparer_ZFObject, ZFComparer<ZFObject *>::Comparer)

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFObjectInstanceState value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFObjectInstanceState, ZFObjectInstanceState)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFObjectInstanceState, ZFObjectInstanceState)
ZFOUTPUT_TYPE(ZFObjectInstanceState, {ZFObjectInstanceStateToStringT(s, v);})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFCallbackType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFCallbackType, ZFCallbackType)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFCallbackType, ZFCallbackType)
ZFOUTPUT_TYPE(ZFCallbackType, {ZFCallbackTypeToStringT(s, v);})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSerializablePropertyType value="value" />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFSerializablePropertyType, ZFSerializablePropertyType)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFSerializablePropertyType, ZFSerializablePropertyType)
ZFOUTPUT_TYPE(ZFSerializablePropertyType, {ZFSerializablePropertyTypeToStringT(s, v);})

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFSerializableData />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFCore, ZFSerializableData, ZFSerializableData)
ZFTYPEID_REG(ZFLIB_ZFCore, ZFSerializableData, ZFSerializableData)
ZFOUTPUT_TYPE_DECLARE(ZFLIB_ZFCore, ZFSerializableData)

/**
 * @brief see #ZFTYPEID_DECLARE
 */
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFSDOutputToken, ZFSDOutputToken)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFSDOutputToken, ZFSDOutputToken)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeId_CoreType_h_


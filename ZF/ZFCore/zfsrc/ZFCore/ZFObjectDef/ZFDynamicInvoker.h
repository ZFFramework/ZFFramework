/**
 * @file ZFDynamicInvoker.h
 * @brief dynamic invoker util for advanced dynamic impl
 */

#ifndef _ZFI_ZFDynamicInvoker_h_
#define _ZFI_ZFDynamicInvoker_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief wrapper for unknown types for #ZFDI_invoke
 */
zfabstract ZFLIB_ZFCore ZFDI_WrapperBase : zfextend ZFObject {
    ZFOBJECT_DECLARE_ABSTRACT(ZFDI_WrapperBase, ZFObject)
    ZFALLOC_CACHE_RELEASE_ABSTRACT({
        cache->zfv(zfnull);
    })

public:
    /**
     * @brief the data
     *
     * the data must store strings that can be converted by:
     * -  #ZFTypeIdWrapper::wrappedValueFromString
     * -  #ZFSerializeFromString
     */
    virtual void zfv(ZF_IN const zfchar *zfv) zfpurevirtual;
    /** @brief see #zfv */
    virtual const zfchar *zfv(void) zfpurevirtual;

public:
    zfoverride
    virtual zfidentity objectHash(void) {
        return zfidentityCalcString(this->zfv());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj) {
        zfself *ref = ZFCastZFObject(zfself *, anotherObj);
        if(ref != zfnull) {
            return ZFComparerDefault(this->zfv(), ref->zfv());
        }
        else {
            return ZFCompareUncomparable;
        }
    }
    zfoverride
    virtual inline zfbool objectIsInternal(void) {return zftrue;}
    zfoverride
    virtual inline zfbool objectIsInternalPrivate(void) {return zftrue;}
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) {
        ret += this->zfv();
    }
protected:
    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN const zfchar *zfv) {this->objectOnInit(); this->zfv(zfv);}
};
/** @brief see #ZFDI_WrapperBase */
zfclass ZFLIB_ZFCore ZFDI_Wrapper : zfextend ZFDI_WrapperBase {
    ZFOBJECT_DECLARE(ZFDI_Wrapper, ZFDI_WrapperBase)
public:
    zfoverride
    virtual void zfv(ZF_IN const zfchar *zfv) {
        this->_ZFP_zfv = zfv;
    }
    zfoverride
    virtual const zfchar *zfv(void) {return this->_ZFP_zfv;}
private:
    zfstring _ZFP_zfv;
};
/** @brief see #ZFDI_WrapperBase */
zfclass ZFLIB_ZFCore ZFDI_WrapperRaw : zfextend ZFDI_WrapperBase {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFDI_WrapperRaw, ZFDI_WrapperBase)
public:
    zfoverride
    virtual void zfv(ZF_IN const zfchar *zfv) {
        if(zfv) {
            this->_ZFP_zfv = zfv;
        }
        else {
            this->_ZFP_zfv = "";
        }
    }
    zfoverride
    virtual const zfchar *zfv(void) {return this->_ZFP_zfv;}
private:
    const zfchar *_ZFP_zfv;
protected:
    ZFDI_WrapperRaw(void) : _ZFP_zfv("") {}
};

// ============================================================
/**
 * @brief util method to convert to string
 *
 * support these types:
 * -  #v_zfstring
 * -  #ZFDI_WrapperBase
 */
extern ZFLIB_ZFCore const zfchar *ZFDI_toString(ZF_IN ZFObject *obj);

/**
 * @brief util method to find class as well as type id wrapper class
 *
 * we will try to find class by #ZFClass::classForName,
 * if unable to find,
 * we will append #ZFTypeIdWrapperPrefixName to find again, e.g.
 * "ClassName" to "v_ClassName" or "NS.ClassName" to "NS.v_ClassName"
 */
extern ZFLIB_ZFCore const ZFClass *ZFDI_classForName(
        ZF_IN const zfchar *className
        , ZF_IN_OPT const zfchar *NS = zfnull
        );
/**
 * @brief util to print param info
 *
 * output format:
 *   (Class0)param0, (Class1)param1, ...
 */
extern ZFLIB_ZFCore void ZFDI_paramInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OPT ZFObject *param0 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
        );

/** @brief util method to calc param count */
inline zfindex ZFDI_paramCount(
        ZF_IN ZFObject * const *paramList
        , ZF_IN_OPT zfindex paramListCount = zfindexMax()
        ) {
    zfindex paramCount = 0;
    while(paramCount < ZFMETHOD_MAX_PARAM && paramCount < paramListCount && paramList[paramCount] != ZFMethodGenericInvokerDefaultParam()) {
        ++paramCount;
    }
    return paramCount;
}
/** @brief util method to calc param count */
inline zfindex ZFDI_paramCount(
        ZF_IN zfauto const *paramList
        , ZF_IN_OPT zfindex paramListCount = zfindexMax()
        ) {
    zfindex paramCount = 0;
    while(paramCount < ZFMETHOD_MAX_PARAM && paramCount < paramListCount && paramList[paramCount] != ZFMethodGenericInvokerDefaultParam()) {
        ++paramCount;
    }
    return paramCount;
}

/**
 * @brief perform advanced dynamic invoke
 *
 * note: when mentioned `string`, we means any type
 * that can be converted by #ZFDI_toString
 *
 * name can be:
 * -# "methodName" : only when obj is not null, call class member method
 * -# "NS.methodName" : call static method
 * -# "NS.ClassName.methodName" : call class static member method
 * -# "NS.v_ClassName.methodName" : call class static member method
 * -# "NS.ClassName" : use #ZFDI_alloc to alloc object
 * -# "NS.v_ClassName" : use #ZFDI_alloc to alloc object
 *
 * params can be:
 * -  #ZFObject type for ZFObject type
 * -  #ZFTypeIdWrapper for non-ZFObject type
 * -  #ZFMethodGenericInvokerDefaultParam for default param
 * -  #ZFDI_WrapperBase, we will try to convert to desired type if possible
 *
 *
 * note: only public methods are allowed to be called by this method,
 * non-public method would result to fail,
 * but you can still explicitly find the method by #ZFMethodForName
 * and invoke it by #ZFMethod::methodGenericInvoke
 */
extern ZFLIB_ZFCore zfbool ZFDI_invoke(ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT ZFObject *obj
        , ZF_IN const zfchar *name
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        );
/**
 * @brief perform advanced dynamic invoke
 */
extern ZFLIB_ZFCore zfbool ZFDI_invoke(ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN_OPT ZFObject *obj
        , ZF_IN const ZFCoreArray<const ZFMethod *> &methodList
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        );

/**
 * @brief perform advanced dynamic class alloc
 *
 * use #ZFDI_classForName to find class,
 * for the params, see #ZFDI_invoke for more info
 */
extern ZFLIB_ZFCore zfbool ZFDI_alloc(ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT zfindex paramCount
        , ZF_IN_OUT zfauto (&paramList)[ZFMETHOD_MAX_PARAM]
        );

// ============================================================
/**
 * @brief util to convert object from string
 */
extern ZFLIB_ZFCore zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *s
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief util to convert object from string
 */
extern ZFLIB_ZFCore zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const zfchar *typeId
        , ZF_IN const zfchar *s
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

// ============================================================
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief util to #ZFDI_invoke/#ZFDI_alloc for static method or object allocation
 *
 * cases:
 * @code
 *   NS.ClassName(...)             // object allcation
 *   NS.ClassName.MethodName(...)  // static member method
 *   NS.MethodName(...)            // static method
 *
 *   // type wrapper specials
 *   NS.v_zfstring(...)            // type wrapper allcation
 *   NS.zfstring(...)              // type wrapper allcation
 *   NS.v_zfstring.MethodName(...) // static member method of type wrapper
 *   NS.zfstring.MethodName(...)   // static member method of type wrapper
 * @endcode
 */
extern ZFLIB_ZFCore zfauto ZFInvoke(
        ZF_IN const zfchar *name
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvoke(
        ZF_IN const zfchar *name
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
        , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvokeDetail(
        ZF_IN const zfchar *name
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success = zfnull
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvoke(
        ZF_IN const zfchar *name
        , ZF_IN const zfchar *param0
        , ZF_IN_OPT const zfchar *param1 = zfnull
        , ZF_IN_OPT const zfchar *param2 = zfnull
        , ZF_IN_OPT const zfchar *param3 = zfnull
        , ZF_IN_OPT const zfchar *param4 = zfnull
        , ZF_IN_OPT const zfchar *param5 = zfnull
        , ZF_IN_OPT const zfchar *param6 = zfnull
        , ZF_IN_OPT const zfchar *param7 = zfnull
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvokeDetail(
        ZF_IN const zfchar *name
        , ZF_IN const ZFCoreArray<zfstring> &params
        , ZF_OUT_OPT zfbool *success = zfnull
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicInvoker_h_


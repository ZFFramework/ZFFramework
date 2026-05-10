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
 *
 * note, for internal use only, you should not store this object
 * for further use
 */
zfclass ZFLIB_ZFCore ZFDI_Wrapper : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFDI_Wrapper, ZFObject)

public:
    /** @brief value type */
    typedef enum {
        TypeString, /**< @brief string type */
        TypeInt, /**< @brief int type */
        TypeFloat, /**< @brief float type */
    } ValueType;
private:
    zfstring _zfvString;
    ValueType _zfvType;
    zfflags _zfvValid;
    zflong _zfvInt;
    zfdouble _zfvFloat;

public:
    /** @brief valueType */
    ValueType valueType(void) {return _zfvType;}
public:
    /**
     * @brief access string value
     *
     * note the returned string may be #zfstring::shared,
     * use #zfstring::sharedCopy if necessary
     */
    const zfstring &valueString(void) {
        switch(_zfvType) {
            case TypeString:
                return _zfvString;
            case TypeInt:
                if(!_zfvString) {
                    zfsFromIntT(_zfvString, _zfvInt);
                }
                return _zfvString;
            case TypeFloat:
                if(!_zfvString) {
                    zfsFromFloatT(_zfvString, _zfvFloat);
                }
                return _zfvString;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return _zfvString;
        }
    }
    /** @brief helper method */
    zfbool valueIntT(ZF_OUT zflong &ret) {
        switch(_zfvType) {
            case TypeString:
                if(!ZFBitTest(_zfvValid, 1 << TypeInt)) {
                    if(!zfsToIntT(_zfvInt, _zfvString, _zfvString.length())) {
                        return zffalse;
                    }
                    ZFBitSet(_zfvValid, 1 << TypeInt);
                }
                ret = _zfvInt;
                return zftrue;
            case TypeInt:
                ret = _zfvInt;
                return zftrue;
            case TypeFloat:
                if(!ZFBitTest(_zfvValid, 1 << TypeInt)) {
                    if(!zffloatIsInt(_zfvFloat)) {
                        return zffalse;
                    }
                    _zfvInt = (zflong)_zfvFloat;
                    ZFBitSet(_zfvValid, 1 << TypeInt);
                }
                ret = _zfvInt;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    /** @brief helper method */
    zflong valueInt(void) {
        zflong ret = 0;
        this->valueIntT(ret);
        return ret;
    }
    /** @brief helper method */
    zfbool valueFloatT(ZF_OUT zfdouble &ret) {
        switch(_zfvType) {
            case TypeString:
                if(!ZFBitTest(_zfvValid, 1 << TypeFloat)) {
                    if(!zfsToFloatT(_zfvFloat, _zfvString, _zfvString.length())) {
                        return zffalse;
                    }
                    ZFBitSet(_zfvValid, 1 << TypeFloat);
                }
                ret = _zfvFloat;
                return zftrue;
            case TypeInt:
                if(!ZFBitTest(_zfvValid, 1 << TypeFloat)) {
                    _zfvFloat = (zfdouble)_zfvInt;
                    ZFBitSet(_zfvValid, 1 << TypeFloat);
                }
                ret = _zfvFloat;
                return zftrue;
            case TypeFloat:
                ret = _zfvFloat;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    /** @brief helper method */
    zfdouble valueFloat(void) {
        zfdouble ret = 0;
        this->valueFloatT(ret);
        return ret;
    }
public:
    /** @brief change value */
    void value(ZF_IN const zfstring &v) {
        _zfvType = TypeString;
        _zfvString = v;
        _zfvValid = 0;
    }
    /** @brief change value */
    void value(ZF_IN const zflong &v) {
        _zfvType = TypeInt;
        _zfvInt = v;
        _zfvValid = 0;
    }
    /** @brief change value */
    void value(ZF_IN const zfdouble &v) {
        _zfvType = TypeFloat;
        _zfvFloat = v;
        _zfvValid = 0;
    }

public:
    zfoverride
    virtual zfidentity objectHashImpl(void) {
        switch(_zfvType) {
            case TypeString:
                return zfidentityHash((zfidentity)_zfvType, zfidentityCalcString(_zfvString));
            case TypeInt:
                return zfidentityHash((zfidentity)_zfvType, zfidentityCalc(_zfvInt));
            case TypeFloat:
                return zfidentityHash((zfidentity)_zfvType, zfidentityCalc(_zfvFloat));
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zfidentityInvalid();
        }
    }
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj) {
        zfself *ref = zfcast(zfself *, anotherObj);
        if(ref != zfnull && _zfvType == ref->_zfvType) {
            switch(_zfvType) {
                case TypeString:
                    return _zfvString == ref->_zfvString ? ZFCompareEqual : ZFCompareUncomparable;
                case TypeInt:
                    return _zfvInt == ref->_zfvInt ? ZFCompareEqual : ZFCompareUncomparable;
                case TypeFloat:
                    return _zfvFloat == ref->_zfvFloat ? ZFCompareEqual : ZFCompareUncomparable;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return ZFCompareUncomparable;
            }
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
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        if(ret.capacity() != 0) {
            ret += this->valueString();
        }
        else {
            ret = this->valueString().sharedCopy();
        }
    }
protected:
    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN const zfstring &v) {this->value(v);}
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN const zflong &v) {this->value(v);}
    /** @brief init with value */
    virtual void objectOnInit(ZF_IN const zfdouble &v) {this->value(v);}
};

// ============================================================
/**
 * @brief util method to convert to string
 *
 * support these types:
 * -  #v_zfstring
 * -  #ZFDI_Wrapper
 */
extern ZFLIB_ZFCore zfstring ZFDI_toString(ZF_IN ZFObject *obj);

/**
 * @brief util method to convert to number
 *
 * support these types:
 * -  #v_zfbool
 * -  #v_zfboolHolder
 * -  #v_zfindex
 * -  #v_zfint
 * -  #v_zfuint
 * -  #v_zflong
 * -  #v_zfulong
 * -  #v_zffloat
 * -  #v_zfdouble
 * -  #v_zfbyte
 * -  #v_zftimet
 * -  #v_zfflags
 * -  #v_zfidentity
 * -  all #ZFEnum types
 * -  #v_zfstring
 * -  #ZFDI_Wrapper
 *
 * note, for all unsigned type (zfuint for example),
 * ensured converted as `-1` for max unsigned value for compatibility
 */
extern ZFLIB_ZFCore zfbool ZFDI_toNumberT(ZF_OUT zfdouble &ret, ZF_IN ZFObject *obj);
/**
 * @brief see #ZFDI_toNumberT, return -1 if fail
 */
inline zfdouble ZFDI_toNumber(ZF_IN ZFObject *obj) {
    zfdouble ret = -1;
    if(ZFDI_toNumberT(ret, obj)) {
        return ret;
    }
    else {
        return -1;
    }
}

/**
 * @brief util to print param info
 *
 * output format:
 *   (Class0)param0, (Class1)param1, ...
 */
extern ZFLIB_ZFCore void ZFDI_paramInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const ZFArgs &zfargs
        );
/** @brief see #ZFDI_paramInfoT */
inline zfstring ZFDI_paramInfo(ZF_IN const ZFArgs &zfargs) {
    zfstring ret;
    ZFDI_paramInfoT(ret, zfargs);
    return ret;
}

/** @brief util method to calc param count */
inline zfindex ZFDI_paramCount(ZF_IN const ZFArgs &zfargs) {
    zfindex paramCount = 0;
    while(paramCount < ZFMETHOD_MAX_PARAM && zfargs.paramAt(paramCount) != ZFMP_DEF()) {
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
 * -  #ZFMP_DEF for default param
 * -  #ZFDI_Wrapper (or #v_zfstring when convStr=true), we will try to convert to desired type if possible
 *
 * note: null param is also a valid param, you must fill #ZFMP_DEF to indicate param end
 *
 * note: only public methods are allowed to be called by this method,
 * non-public method would result to fail,
 * but you can still explicitly find the method by #ZFMethodForName
 * and invoke it by #ZFMethod::methodInvoke
 */
extern ZFLIB_ZFCore void ZFDI_invoke(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const zfstring &name
        , ZF_IN_OPT zfbool convStr = zffalse
        );

/**
 * @brief perform advanced dynamic class alloc
 *
 * use #ZFClass::classForName to find class,
 * for the params, see #ZFDI_invoke for more info
 */
extern ZFLIB_ZFCore void ZFDI_alloc(
        ZF_IN_OUT const ZFArgs &zfargs
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT zfbool convStr = zffalse
        );

// ============================================================
/**
 * @brief util to convert object from string
 */
extern ZFLIB_ZFCore zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );
/**
 * @brief util to convert object from string
 */
extern ZFLIB_ZFCore zfbool ZFDI_objectFromString(
        ZF_OUT zfauto &ret
        , ZF_IN const zfstring &typeId
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
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
        ZF_IN const zfstring &name
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvoke(
        ZF_IN const zfstring &name
        , ZF_IN ZFObject *param0
        , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfbool ZFInvokeT(
        ZF_OUT zfauto &ret
        , ZF_OUT_OPT zfstring *errorHint
        , ZF_IN const zfstring &name
        , ZF_IN_OPT ZFObject *param0 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
        , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
        );
/** @brief see #ZFInvoke */
extern ZFLIB_ZFCore zfauto ZFInvokeDetail(
        ZF_IN const zfstring &name
        , ZF_IN const ZFCoreArray<zfauto> &params
        , ZF_OUT_OPT zfbool *success = zfnull
        , ZF_OUT_OPT zfstring *errorHint = zfnull
        );

// ============================================================
zfclassFwd ZFOutput;
/**
 * @brief callbacks when any dynamic invoker fails
 *
 * note: adding anything to this would cause great performance issue,
 * use only if necessary
 */
extern ZFLIB_ZFCore ZFCoreArray<ZFOutput> &ZFDI_errorCallbacks(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFDynamicInvoker_h_


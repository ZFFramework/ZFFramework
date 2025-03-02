/**
 * @file ZFTypeIdCore.h
 * @brief reflectable type define
 */

#ifndef _ZFI_ZFTypeIdCore_h_
#define _ZFI_ZFTypeIdCore_h_

#include "ZFStyleable.h"
#include "ZFProgressable.h"
#include "ZFMethodUserRegister.h"
#include "ZFMethodFuncUserRegister.h"
#include "ZFMethodFuncDeclare.h"
#include "ZFObjectUtil.h"
#include "ZFSerializableUtil.h"
#include "ZFSerializableDataSerializableConverter.h"
#include "ZFObjectRetain.h"
#include "ZFTypeIdDeclare.h"
#include "ZFTypeIdDeclare_inner.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief dummy base for all wrapper types,
 *   see #ZFTypeId::Value
 *
 * serializable data:
 * @code
 *   <WrapperTypeClass
 *       value="string content" // optional, empty by default
 *   />
 * @endcode
 */
zfabstract ZFLIB_ZFCore ZFTypeIdWrapper : zfextend ZFStyle, zfimplement ZFProgressable {
    ZFOBJECT_DECLARE_ABSTRACT(ZFTypeIdWrapper, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFProgressable)
    ZFALLOC_CACHE_RELEASE_ABSTRACT({
        cache->zfvReset();
    })

public:
    /**
     * @brief copy internal value
     */
    zffinal ZFTypeIdWrapper *zfvAssign(ZF_IN ZFTypeIdWrapper *ref) {
        this->wrappedValueOnAssign(ref);
        return this;
    }
protected:
    /**
     * @brief see #zfvAssign
     */
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) zfpurevirtual;

public:
    /**
     * @brief type id for this wrapper type
     */
    virtual const zfstring &zfvTypeId(void) zfpurevirtual;
    /**
     * @brief access the value's address
     */
    virtual void *wrappedValue(void) zfpurevirtual;
    /**
     * @brief set the value, no type safe check
     */
    virtual void wrappedValue(ZF_IN const void *v) zfpurevirtual;
    /**
     * @brief get the value, no type safe check
     */
    virtual void wrappedValueCopy(ZF_IN void *v) zfpurevirtual;

    /**
     * @brief reset the value to it's init value
     */
    virtual void zfvReset(void) zfpurevirtual;
    /**
     * @brief true if the value is in init value state
     */
    virtual zfbool zfvIsInit(void) zfpurevirtual;

public:
    /**
     * @brief whether prefer string converter during serialization,
     *   which would result much shorter result
     *
     * @code
     *   // default complex serializable data:
     *   <Owner>
     *       <TypeId prop="PropertyName" value="xxx"/>
     *   </Owner>
     *
     *   // if prefer string converter
     *   <Owner PropertyName="xxx" />
     * @endcode
     */
    virtual zfbool wrappedValuePreferStringConverter(void) {return zffalse;}
    /**
     * @brief convert from serializable data
     */
    virtual zfbool zfvFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert to serializable data
     */
    virtual zfbool zfvToData(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert from string
     */
    virtual zfbool zfvFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;
    /**
     * @brief convert to string
     */
    virtual zfbool zfvToString(
            ZF_IN_OUT zfstring &s
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;

public:
    zfoverride
    virtual zfbool progressUpdate(
            ZF_IN ZFProgressable *from
            , ZF_IN ZFProgressable *to
            , ZF_IN zffloat progress
            ) {
        return zffalse;
    }

public:
    zfoverride
    virtual zfbool objectIsInternal(void) {
        return zftrue;
    }

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable) {
        zfsuper::styleableOnCopyFrom(anotherStyleable);
        this->wrappedValue(anotherStyleable->to<ZFTypeIdWrapper *>()->wrappedValue());
    }

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

        zfstring valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
        if(valueString == zfnull) {
            this->zfvReset();
        }
        else if(!this->zfvFromString(valueString)) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "failed to convert from \"%s\"", valueString);
            return zffalse;
        }
        return zftrue;
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
        zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

        if((ref == zfnull && !this->zfvIsInit())
                || (ref != zfnull && this->objectCompare(ref) != ZFCompareEqual)
                ) {
            if(this->wrappedValuePreferStringConverter()) {
                zfstring valueString;
                if(!this->zfvToString(valueString, outErrorHint)) {
                    return zffalse;
                }
                serializableData.propertyValue(valueString);
            }
            else {
                if(!this->zfvToData(serializableData, outErrorHint)) {
                    return zffalse;
                }
            }
        }

        return zftrue;
    }
protected:
    zfoverride
    virtual inline zfbool serializableOnSerializeFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        return this->zfvFromString(src, srcLen, errorHint);
    }
    zfoverride
    virtual inline zfbool serializableOnSerializeToString(
            ZF_IN_OUT zfstring &ret
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) {
        return this->zfvToString(ret, errorHint);
    }
};

// ============================================================
/**
 * @brief register Type's progress update logic
 *
 * a progress update means,
 * the type can be changed by timer by supply a "from/to" value range\n
 * for example, an float type with range "[0.5, 3.0]" and progress "0.3",
 * would result "0.5 + (3.0 - 0.5) * 0.3", which is 1.25\n
 * this is useful to make a property change with animation\n
 * \n
 * to use this, register your type by this macro, with this proto type:
 * @code
 *   zfbool update(
 *           ZF_OUT Type &ret
 *           , ZF_IN Type const &from
 *           , ZF_IN Type const &to
 *           , ZF_IN zffloat progress
 *           );
 * @endcode
 * then use them by #ZFProgressable::progressUpdate\n
 * \n
 * to register:
 * @code
 *   ZFTYPEID_PROGRESS_DEFINE(YourType, YourType, {
 *           yourProgress(ret, from, to, progress);
 *       })
 * @endcode
 * or use #ZFTYPEID_PROGRESS_DEFINE_BY_VALUE for short
 */
#define ZFTYPEID_PROGRESS_DEFINE(TypeName, Type, progressUpdateAction) \
    ZF_STATIC_REGISTER_INIT(_ZFP_ZFTypeIdProgressReg_##TypeName) { \
        v_##TypeName::_ZFP_ZFTypeId_progressUpdate() = zfself::I; \
    } \
    static zfbool I( \
            ZF_IN_OUT ZFProgressable *_ret \
            , ZF_IN ZFProgressable *_from \
            , ZF_IN ZFProgressable *_to \
            , ZF_IN zffloat progress \
            ) { \
        v_##TypeName *_retH = zfcast(v_##TypeName *, _ret); \
        v_##TypeName *_fromH = zfcast(v_##TypeName *, _from); \
        v_##TypeName *_toH = zfcast(v_##TypeName *, _to); \
        if(_retH == zfnull || _fromH == zfnull || _toH == zfnull) { \
            return zffalse; \
        } \
        Type &ret = _retH->zfv; \
        Type const &from = _fromH->zfv; \
        Type const &to = _toH->zfv; \
        progressUpdateAction \
        return zftrue; \
    } \
    ZF_STATIC_REGISTER_END(_ZFP_ZFTypeIdProgressReg_##TypeName)
/**
 * @brief util macro to declare #ZFTYPEID_PROGRESS_DEFINE by raw value calculating
 */
#define ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(TypeName, Type) \
    ZFTYPEID_PROGRESS_DEFINE(TypeName, Type, { \
            ret = (Type)(from + (Type)((to - from) * progress)); \
        })

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdCore_h_


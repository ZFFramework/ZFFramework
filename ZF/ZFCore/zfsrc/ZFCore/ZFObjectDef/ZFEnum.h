/**
 * @file ZFEnum.h
 * @brief enum utility
 */

#ifndef _ZFI_ZFEnum_h_
#define _ZFI_ZFEnum_h_

#include "ZFTypeId.h"
#include "ZFEnumDeclare.h"
#include "ZFEnumDeclare_inner.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief invalid value for ZFEnum
 */
#define ZFEnumInvalid() ((zfuint)-1)
/**
 * @brief invalid value's name for ZFEnum
 */
#define ZFEnumNameInvalid() _ZFP_ZFEnumNameInvalid()
extern ZFLIB_ZFCore const zfstring &_ZFP_ZFEnumNameInvalid(void);

// ============================================================
/**
 * @brief abstract base class of all enum type of ZFFramework
 *
 * please refer to #ZFENUM_BEGIN to see how to use enum type\n
 * \n
 * serializable data:
 * @code
 *   <EnumClass
 *       value="EmunValueName" // optional, #ZFEnumNameInvalid by default
 *   />
 * @endcode
 */
zfabstract ZFLIB_ZFCore ZFEnum : zfextend ZFTypeIdWrapper {
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFEnum, ZFTypeIdWrapper)

protected:
    /** @cond ZFPrivateDoc */
    ZFEnum(void) : _ZFP_ZFEnum_value(ZFEnumInvalid()) {}
    /** @endcond */

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj) {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->enumValue(zfcast(zfself *, anotherObj)->enumValue());
    }

protected:
    /**
     * @brief init with value or ZFEnumInvalid if invalid
     */
    virtual void objectOnInit(ZF_IN zfuint value);
    /**
     * @brief init with ZFEnum
     */
    virtual void objectOnInit(ZF_IN ZFEnum *another);

    zfoverride
    virtual void objectOnInit(void) {zfsuper::objectOnInit();}

public:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual inline zfidentity objectHashImpl(void) {
        return zfidentityCalcPOD(this->enumValue());
    }
    /**
     * @brief compare result of two enum values as int,
     *   or return ZFCompareUncomparable if any of them are ZFEnumInvalid
     */
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj);

public:
    /* use ZFObject version instead of ZFTypeIdWrapper version */
    zfoverride
    virtual zfbool objectIsInternal(void) {
        return ZFObject::objectIsInternal();
    }
    zfoverride
    virtual zfbool objectIsInternalPrivate(void) {
        return ZFObject::objectIsInternalPrivate();
    }

public:
    zfoverride
    virtual void *wrappedValue(void);
    zfoverride
    virtual void wrappedValue(ZF_IN const void *v);
    zfoverride
    virtual void wrappedValueCopy(ZF_IN void *v);
public:
    zfoverride
    virtual void zfvReset(void);
    zfoverride
    virtual zfbool zfvIsInit(void);
public:
    zfoverride
    virtual zfbool wrappedValuePreferStringConverter(void) {
        return zftrue;
    }
    zfoverride
    virtual zfbool zfvFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool zfvToData(
            ZF_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );
    zfoverride
    virtual zfbool zfvFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );
    zfoverride
    virtual zfbool zfvToString(
            ZF_IN_OUT zfstring &s
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );

public:
    /**
     * @brief get value num
     */
    virtual zfindex enumCount(void) zfpurevirtual;
    /**
     * @brief get the index of enum value, or zfindexMax() if no such enum value
     */
    virtual zfindex enumIndexForValue(ZF_IN zfuint value) zfpurevirtual;
    /**
     * @brief get the value at index, or ZFEnumInvalid if not exist
     */
    virtual zfuint enumValueAt(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief get the name at index, or ZFEnumNameInvalid if not exist
     */
    virtual const zfstring &enumNameAt(ZF_IN zfindex index) zfpurevirtual;
    /**
     * @brief return true if contain the specified value
     */
    virtual zfbool enumValueContain(ZF_IN zfuint value) zfpurevirtual;

    /**
     * @brief get the value with specified name, or ZFEnumInvalid if not exist
     */
    virtual zfuint enumValueForName(ZF_IN const zfstring &name) zfpurevirtual;
    /**
     * @brief get the name with specified value, or ZFEnumNameInvalid if not exist
     */
    virtual const zfstring &enumNameForValue(ZF_IN zfuint value) zfpurevirtual;

    /**
     * @brief true if this enum type is flags type
     *   (which support bit operation)
     */
    virtual zfbool enumIsFlags(void) zfpurevirtual;

    /**
     * @brief default value of this enum
     */
    virtual zfuint enumDefault(void) zfpurevirtual;

    /**
     * @brief get the enum value, or #ZFEnumInvalid if invalid
     */
    virtual zfuint const &enumValue(void) {
        return _ZFP_ZFEnum_value;
    }
    /**
     * @brief get current value's name, or ZFEnumNameInvalid if invalid
     *
     * return invalid if holds flags value,
     * use #zfflagsToString instead
     */
    virtual const zfstring &enumName(void) {
        return this->enumNameForValue(_ZFP_ZFEnum_value);
    }

public:
    /**
     * @brief set the enum value
     *
     * must be the enum's value or ZFEnumInvalid
     */
    virtual void enumValue(ZF_IN zfuint const &value) {
        if(!this->enumIsFlags() && !this->enumValueContain(value)) {
            _ZFP_ZFEnum_value = ZFEnumInvalid();
            return;
        }
        _ZFP_ZFEnum_value = value;
    }

public:
    void _ZFP_enumValue(ZF_IN zfuint value) {
        this->enumValue(value);
    }
public:
    zfuint _ZFP_ZFEnum_value;
public:
    zfoverride
    virtual void wrappedValueOnAssign(ZF_IN ZFTypeIdWrapper *ref) {
        zfself *refTmp = zfcast(zfself *, ref);
        if(refTmp != zfnull) {
            this->_ZFP_ZFEnum_value = refTmp->_ZFP_ZFEnum_value;
        }
    }
};

// ============================================================
// zfflags conversion
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsFromString
 */
extern ZFLIB_ZFCore zfbool zfflagsFromStringT(
        ZF_OUT zfflags &ret
        , ZF_IN const ZFClass *enumClass
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfchar separatorToken = '|'
        , ZF_OUT_OPT const zfchar **outErrorPos = zfnull
        );
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsFromStringT
 */
inline zfflags zfflagsFromString(
        ZF_IN const ZFClass *enumClass
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen = zfindexMax()
        , ZF_IN_OPT zfchar separatorToken = '|'
        , ZF_OUT_OPT const zfchar **outErrorPos = zfnull
        ) {
    zfflags ret = 0;
    zfflagsFromStringT(ret, enumClass, src, srcLen, separatorToken, outErrorPos);
    return ret;
}
/**
 * @brief zfflags conversion using ZFEnum as source, see #zfflagsToString
 */
extern ZFLIB_ZFCore zfbool zfflagsToStringT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const ZFClass *enumClass
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted = zftrue
        , ZF_IN_OPT zfbool exclusiveMode = zffalse
        , ZF_OUT_OPT zfflags *notConverted = zfnull
        , ZF_IN_OPT zfchar separatorToken = '|'
        );
/** @brief see #zfflagsToStringT */
inline zfstring zfflagsToString(
        ZF_IN const ZFClass *enumClass
        , ZF_IN zfflags const &value
        , ZF_IN_OPT zfbool includeNotConverted = zftrue
        , ZF_IN_OPT zfbool exclusiveMode = zffalse
        , ZF_OUT_OPT zfflags *notConverted = zfnull
        , ZF_IN_OPT zfchar separatorToken = '|'
        ) {
    zfstring ret;
    zfflagsToStringT(ret, enumClass, value, includeNotConverted, exclusiveMode, notConverted, separatorToken);
    return ret;
}

// ============================================================
zfclassFwd _ZFP_ZFEnumDataPrivate;
zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFEnumData {
public:
    _ZFP_ZFEnumData(void);
    ~_ZFP_ZFEnumData(void);
public:
    zfbool needInitFlag;
    const ZFClass *ownerClass;
    zfuint enumDefault;
    zfbool enumIsFlags;
    void add(
            ZF_IN zfbool isEnableDuplicateValue
            , ZF_IN zfuint value
            , ZF_IN const zfstring &name
            );
    zfindex enumCount(void) const;
    zfindex enumIndexForValue(ZF_IN zfuint value) const;
    zfuint enumValueAt(ZF_IN zfindex index) const;
    const zfstring &enumNameAt(ZF_IN zfindex index) const;
    zfbool enumValueContain(ZF_IN zfuint value) const;
    zfuint enumValueForName(ZF_IN const zfstring &name) const;
    const zfstring &enumNameForValue(ZF_IN zfuint value) const;
private:
    _ZFP_ZFEnumDataPrivate *d;
};
extern ZFLIB_ZFCore _ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass);
extern ZFLIB_ZFCore void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass);
extern ZFLIB_ZFCore void _ZFP_ZFEnumDataCleanup(ZF_IN const _ZFP_ZFEnumData *d);
zfclassNotPOD _ZFP_ZFEnumDataHolder {
public:
    _ZFP_ZFEnumDataHolder(ZF_IN const _ZFP_ZFEnumData *d)
    : d(d)
    {
    }
    _ZFP_ZFEnumDataHolder(void) {
        _ZFP_ZFEnumDataCleanup(d);
    }
public:
    const _ZFP_ZFEnumData *d;
};
extern ZFLIB_ZFCore const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass);
extern ZFLIB_ZFCore void _ZFP_ZFEnumMethodReg(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const _ZFP_ZFEnumData *d
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnum_h_


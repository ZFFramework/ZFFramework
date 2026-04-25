/**
 * @file ZFValue.h
 * @brief used to hold a non ZFObject type for performance, for internal use only
 */

#ifndef _ZFI_ZFValue_h_
#define _ZFI_ZFValue_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFValueType {
public:
    static void TypePOD(ZF_IN void *value) {
        zffree(value);
    }
    template<typename T_Object>
    static void TypeObject(ZF_IN void *value) {
        zfdelete((T_Object *)value);
    }
    template<typename T_Object>
    static void TypePoolObject(ZF_IN void *value) {
        zfpoolDelete((T_Object *)value);
    }
};
/** @brief see #ZFValueType */
#define ZFValueTypePointerRef() zfnull
/** @brief see #ZFValueType */
#define ZFValueTypePOD() _ZFP_ZFValueType::TypePOD
/** @brief see #ZFValueType */
#define ZFValueTypeObject(T_Object) _ZFP_ZFValueType::TypeObject<T_Object >
/** @brief see #ZFValueType */
#define ZFValueTypePoolObject(T_Object) _ZFP_ZFValueType::TypePoolObject<T_Object >

// ============================================================
// ZFValue
zfclassFwd ZFValue;
/**
 * @brief type for #ZFValue
 *
 * to achieve custom #ZFValue, supply a function with following proto type,
 * which cleanup the internal #ZFValue::value:
 * @code
 *   void type(ZF_IN void *value);
 * @endcode
 */
typedef void (*ZFValueType)(ZF_IN void *value);

/**
 * @brief used to hold a non ZFObject type for performance, for internal use only
 */
zffinal zfclass ZFLIB_ZFCore ZFValue : zfextend ZFObject {
    // ============================================================
public:
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFValue, ZFObject)

protected:
    ZFValue(void) : _value(zfnull), _type(zfnull) {}

private:
    void *_value;
    ZFValueType _type;
    void _cleanup(void) {
        if(_value && _type) {
            void *value = _value;
            ZFValueType type = _type;
            _value = zfnull;
            _type = zfnull;
            type(value);
        }
        else {
            _value = zfnull;
            _type = zfnull;
        }
    }

protected:
    /**
     * @brief init with value and type
     */
    virtual void objectOnInit(
        ZF_IN void *value
        , ZF_IN ZFValueType type
        ) {
        _value = value;
        _type = type;
    }
    /**
     * @brief init and copy value
     */
    template<typename T_Value>
    void objectOnInit(ZF_IN T_Value const &value) {
        _value = zfpoolNew(T_Value, value);
        _type = ZFValueTypePoolObject(T_Value);
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->_cleanup();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief change the value
     */
    ZFValue *value(ZF_IN void *value, ZF_IN ZFValueType type) {
        if(_value && _type) {
            void *valueOld = _value;
            ZFValueType typeOld = _type;
            _value = value;
            _type = type;
            typeOld(valueOld);
        }
        else {
            _value = value;
            _type = type;
        }
        return this;
    }
    /**
     * @brief change the value
     */
    template<typename T_Value>
    ZFValue *value(ZF_IN T_Value const &value) {
        if(_value && _type) {
            void *valueOld = _value;
            ZFValueType typeOld = _type;
            _value = zfpoolNew(T_Value, value);
            _type = ZFValueTypePoolObject(T_Value);
            typeOld(valueOld);
        }
        else {
            _value = zfpoolNew(T_Value, value);
            _type = ZFValueTypePoolObject(T_Value);
        }
        return this;
    }

public:
    /**
     * @brief whether value valid
     */
    zfbool valid(void) {return _value != zfnull;}
    /**
     * @brief used to hold the raw pointer
     */
    void *valuePtr(void) {return _value;}
    /**
     * @brief delete callback to clear the #value
     */
    ZFValueType type(void) {return _type;}

public:
    /**
     * @brief util method to access #value as reference
     *
     * warning: you must make sure the type is exactly same from the type passed from value store
     */
    template<typename T_Value>
    inline T_Value &value(void) {
        return *(T_Value *)_value;
    }
    /**
     * @brief util method to access #value as reference
     *
     * warning: you must make sure the type is exactly same from the type passed from value store
     */
    template<typename T_Value>
    inline T_Value *valuePtr(void) {
        return (T_Value *)_value;
    }

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFValue_h_


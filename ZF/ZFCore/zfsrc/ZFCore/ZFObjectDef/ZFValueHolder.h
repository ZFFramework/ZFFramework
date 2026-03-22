/**
 * @file ZFValueHolder.h
 * @brief utilities for ZFObject
 */

#ifndef _ZFI_ZFValueHolder_h_
#define _ZFI_ZFValueHolder_h_

#include "ZFObjectCore.h"
#include "ZFObjectRetain.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFValueHolderType {
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
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePointerRef() zfnull
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePOD() _ZFP_ZFValueHolderType::TypePOD
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypeObject(T_Object) _ZFP_ZFValueHolderType::TypeObject<T_Object >
/** @brief see #ZFValueHolderType */
#define ZFValueHolderTypePoolObject(T_Object) _ZFP_ZFValueHolderType::TypePoolObject<T_Object >

// ============================================================
// ZFValueHolder
zfclassFwd ZFValueHolder;
/**
 * @brief type for #ZFValueHolder
 *
 * to achieve custom #ZFValueHolder, supply a function with following proto type,
 * which cleanup the internal #ZFValueHolder::value:
 * @code
 *   void type(ZF_IN void *value);
 * @endcode
 */
typedef void (*ZFValueHolderType)(ZF_IN void *value);

/**
 * @brief used to hold a non ZFObject type for performance
 */
zffinal zfclass ZFLIB_ZFCore ZFValueHolder : zfextend ZFObject {
    // ============================================================
public:
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFValueHolder, ZFObject)

public:
    /** @brief util to create */
    template<typename T_Value>
    T_Value &create(void) {
        T_Value *v = zfpoolNew(T_Value);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0>
    T_Value &create(ZF_IN P0 const &p0) {
        T_Value *v = zfpoolNew(T_Value, p0);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1) {
        T_Value *v = zfpoolNew(T_Value, p0, p1);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2, typename P3>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2, ZF_IN P3 const &p3) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2, p3);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2, typename P3, typename P4>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2, ZF_IN P3 const &p3, ZF_IN P4 const &p4) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2, p3, p4);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2, ZF_IN P3 const &p3, ZF_IN P4 const &p4, ZF_IN P5 const &p5) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2, p3, p4, p5);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2, ZF_IN P3 const &p3, ZF_IN P4 const &p4, ZF_IN P5 const &p5, ZF_IN P6 const &p6) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2, p3, p4, p5, p6);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }
    /** @brief util to create */
    template<typename T_Value, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    T_Value &create(ZF_IN P0 const &p0, ZF_IN P1 const &p1, ZF_IN P2 const &p2, ZF_IN P3 const &p3, ZF_IN P4 const &p4, ZF_IN P5 const &p5, ZF_IN P6 const &p6, ZF_IN P7 const &p7) {
        T_Value *v = zfpoolNew(T_Value, p0, p1, p2, p3, p4, p5, p6, p7);
        this->value(v, ZFValueHolderTypePoolObject(T_Value));
        return *v;
    }

protected:
    ZFValueHolder(void) : _value(zfnull), _type(zfnull) {}

    ZFOBJECT_CACHE_RELEASE({
        cache->_cleanup();
    })
private:
    void *_value;
    ZFValueHolderType _type;
    void _cleanup(void) {
        if(_value && _type) {
            void *value = _value;
            ZFValueHolderType type = _type;
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
     * @brief init with object
     */
    virtual void objectOnInit(
        ZF_IN void *value
        , ZF_IN ZFValueHolderType type
        ) {
        _value = value;
        _type = type;
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
    void value(ZF_IN void *value, ZF_IN ZFValueHolderType type) {
        if(_value && _type) {
            void *value = _value;
            ZFValueHolderType type = _type;
            _value = value;
            _type = type;
            type(value);
        }
        else {
            _value = value;
            _type = type;
        }
    }

public:
    /**
     * @brief used to hold the raw pointer
     */
    void *value(void) {return _value;}
    /**
     * @brief delete callback to clear the #value
     */
    ZFValueHolderType type(void) {return _type;}

public:
    /** @brief util method to access #value as reference */
    template<typename T_RawType>
    inline T_RawType valueRef(void) {
        typedef typename zftTraits<T_RawType>::TrPtr T_RawTypePointer;
        return *(T_RawTypePointer)_value;
    }

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
public:
    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFValueHolder_h_


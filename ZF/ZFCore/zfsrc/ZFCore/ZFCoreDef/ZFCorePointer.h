/**
 * @file ZFCorePointer.h
 * @brief light weight smart pointer
 */
#ifndef _ZFI_ZFCorePointer_h_
#define _ZFI_ZFCorePointer_h_

#include "ZFCoreTypeDef.h"
#include "ZFCoreUtilMacro.h"
#include "zfstr.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper;
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper<T_Pointer *> {
public:
    static inline void *toNonConstRaw(ZF_IN T_Pointer *p) {
        return p;
    }
    template<typename T_ZFCorePointer>
    static inline T_Pointer *toPointer(ZF_IN T_ZFCorePointer p) {
        return (T_Pointer *)p->pointerValueAccessNonConst();
    }
};
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerHelper<const T_Pointer *> {
public:
    static inline void *toNonConstRaw(ZF_IN const T_Pointer *p) {
        return zfnull;
    }
    template<typename T_ZFCorePointer>
    static inline const T_Pointer *toPointer(ZF_IN T_ZFCorePointer p) {
        return (const T_Pointer *)p->pointerValueAccess();
    }
};

// ============================================================
/**
 * @brief abstract base class for ZFCorePointerT
 *
 * designed for hiding type from ZFCorePointerT
 */
zfclassLikePOD ZFLIB_ZFCore ZFCorePointer {
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFCorePointer)
public:
    /** @cond ZFPrivateDoc */
    ZFCorePointer(void) {}
    virtual ~ZFCorePointer(void) {}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const {
        if(this->pointerValueAccess() == zfnull) {
            ret += ZFTOKEN_zfnull;
        }
        else {
            this->objectInfoOfContentT(ret);
        }
    }
    /** @brief return object info */
    virtual inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret) const zfpurevirtual;
    /**
     * @brief get content info or #ZFTOKEN_zftToStringNotAvailable if not available
     */
    virtual inline zfstring objectInfoOfContent(void) const {
        zfstring ret;
        this->objectInfoOfContentT(ret);
        return ret;
    }

public:
    /**
     * @brief compare by internal pointer value
     */
    virtual ZFCompareResult objectCompare(ZF_IN const ZFCorePointer &another) const {
        return ((this->pointerValueAccess() == another.pointerValueAccess())
            ? ZFCompareEqual
            : ZFCompareUncomparable);
    }

public:
    /**
     * @brief create a new reference to the smart pointer
     *
     * increase object reference and return a new smart pointer,
     * old one should be deleted manually\n
     * after the newly created instance and the old one both been deleted,
     * the object would be deleted finally
     * @note should be (and recommended to be) paired with #refDelete instead of #zfdelete,
     *   if you override default #zfnew and #zfdelete
     */
    virtual const ZFCorePointer *refNew(void) const zfpurevirtual;
    /**
     * @brief see #refNew
     */
    virtual void refDelete(void) const {
        zfdelete(const_cast<ZFCorePointer *>(this));
    }
    /**
     * @brief get the internal pointer
     */
    virtual const void *pointerValueAccess(void) const zfpurevirtual;
    /**
     * @brief get the internal pointer
     */
    virtual void *pointerValueAccessNonConst(void) const zfpurevirtual;

    /**
     * @brief util method to get and cast to desired type
     */
    template<typename T_PointerDesired>
    inline T_PointerDesired pointerValueT(void) const {
        return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this);
    }

    /**
     * @brief get current retain count
     */
    virtual zfindex objectRetainCount(void) const zfpurevirtual;
};
ZFOUTPUT_TYPE(ZFCorePointer, {v.objectInfoT(s);})

// ============================================================
template<typename T_Pointer>
zfclassNotPOD _ZFP_ZFCorePointerPrivate {
public:
    zfuint refCount;
    T_Pointer pointerValue;
public:
    _ZFP_ZFCorePointerPrivate(void)
    : refCount(1)
    , pointerValue(zfnull)
    {
    }
};
/**
 * @brief pointer container with reference count logic
 *
 * usage:
 * @code
 *   {
 *       // create
 *       YourClass *obj = zfnew(YourClass);
 *       ZFCorePointerForObject<YourClass *> container(obj);
 *
 *       // get
 *       container->pointerValue()->funcInYourClass();
 *       // or cast it
 *       ((YourClass *)container)->funcInYourClass();
 *       // or implicit
 *       container->funcInYourClass();
 *
 *       // or as r-value
 *       YourClass *p = container;
 *       // or set as l-value
 *       container = p;
 *
 *       // or copy it
 *       ZFCorePointerForObject<YourClass *> another0(container);
 *       another0 = container;
 *   } // obj would be automatically deleted after this block,
 *     // i.e. all smart pointer container were deleted
 * @endcode
 *
 * you may store any types in ZFCorePointerT, if you are able to supply a custom delete callback,
 * to declare your own type, see #ZFCOREPOINTER_DECLARE
 */
template<typename T_Pointer, typename T_ZFCorePointerType>
zffinal zfclassLikePOD ZFCorePointerT : zfextend ZFCorePointer {
public:
    /**
     * @brief set the pointer value
     */
    inline void pointerValue(ZF_IN T_Pointer const &value) {
        if(d->pointerValue != value) {
            T_Pointer pointerValueOld = d->pointerValue;
            d->pointerValue = value;
            if(value) {
                T_ZFCorePointerType::pointerOnRetain(d->pointerValue);
            }
            if(pointerValueOld != zfnull) {
                T_ZFCorePointerType::pointerOnDelete(pointerValueOld);
            }
        }
    }
    /**
     * @brief get the pointer value
     */
    inline T_Pointer const &pointerValue(void) const {
        return d->pointerValue;
    }

public:
    /** @cond ZFPrivateDoc */
    inline T_Pointer operator -> (void) const {
        return d->pointerValue;
    }
    inline zfbool operator == (ZF_IN zfnullT const &p) const {
        return (d->pointerValue == zfnull);
    }
    inline zfbool operator != (ZF_IN zfnullT const &p) const {
        return (d->pointerValue != zfnull);
    }
    inline zfbool operator == (ZF_IN T_Pointer const &p) const {
        return (d->pointerValue == p);
    }
    inline zfbool operator != (ZF_IN T_Pointer const &p) const {
        return (d->pointerValue != p);
    }
    inline zfbool operator == (ZF_IN ZFCorePointerT<T_Pointer, T_ZFCorePointerType> const &ref) const {
        return (d->pointerValue == ref.d->pointerValue);
    }
    inline zfbool operator != (ZF_IN ZFCorePointerT<T_Pointer, T_ZFCorePointerType> const &ref) const {
        return (d->pointerValue == ref.d->pointerValue);
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    ZFCorePointerT(void)
    : d(zfnew(_ZFP_ZFCorePointerPrivate<T_Pointer>))
    {
    }
    ZFCorePointerT(ZF_IN zfnullT const &value)
    : d(zfnew(_ZFP_ZFCorePointerPrivate<T_Pointer>))
    {
    }
    ZFCorePointerT(ZF_IN T_Pointer const &value)
    : d(zfnew(_ZFP_ZFCorePointerPrivate<T_Pointer>))
    {
        this->pointerValue(value);
    }
    ZFCorePointerT(ZF_IN const ZFCorePointerT<T_Pointer, T_ZFCorePointerType> &ref)
    : d(ref.d)
    {
        ++(d->refCount);
    }
    virtual ~ZFCorePointerT(void) {
        --(d->refCount);
        if(d->refCount == 0) {
            if(d->pointerValue != zfnull) {
                T_ZFCorePointerType::pointerOnDelete(d->pointerValue);
            }
            zfdelete(d);
        }
    }
    ZFCorePointerT<T_Pointer, T_ZFCorePointerType> &operator = (ZF_IN const ZFCorePointerT<T_Pointer, T_ZFCorePointerType> &ref) {
        _ZFP_ZFCorePointerPrivate<T_Pointer> *dTmp = d;
        d = ref.d;
        ++(ref.d->refCount);
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            if(dTmp->pointerValue != zfnull) {
                T_ZFCorePointerType::pointerOnDelete(dTmp->pointerValue);
            }
            zfdelete(dTmp);
        }
        return *this;
    }
    ZFCorePointerT<T_Pointer, T_ZFCorePointerType> &operator = (ZF_IN T_Pointer const &value) {
        this->pointerValue(value);
        return *this;
    }
    operator T_Pointer (void) const {
        return d->pointerValue;
    }
    /** @endcond */

public:
    zfoverride
    virtual void objectInfoOfContentT(ZF_IN_OUT zfstring &ret) const {
        this->objectInfoOfContentT(ret, zfnull);
    }
    /** @brief see #objectInfoOfContent */
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN typename ZFCoreInfoGetter<T_Pointer>::InfoGetter infoGetter
            ) const {
        if(infoGetter != zfnull) {
            infoGetter(ret, d->pointerValue);
        }
        else {
            zftToStringT(ret, d->pointerValue);
        }
    }

public:
    zfoverride
    virtual const ZFCorePointer *refNew(void) const {
        return zfnew(ZFCorePointerT<T_Pointer ZFM_COMMA() T_ZFCorePointerType>, *this);
    }
    zfoverride
    virtual inline const void *pointerValueAccess(void) const {
        return d->pointerValue;
    }
    zfoverride
    virtual inline void *pointerValueAccessNonConst(void) const {
        return _ZFP_ZFCorePointerHelper<T_Pointer>::toNonConstRaw(d->pointerValue);
    }

    zfoverride
    virtual zfindex objectRetainCount(void) const {
        return d->refCount;
    }

    /** @cond ZFPrivateDoc */
    template<typename T_PointerDesired>
    inline T_PointerDesired pointerValueT(void) const {
        return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this);
    }
    /** @endcond */

private:
    _ZFP_ZFCorePointerPrivate<T_Pointer> *d;
};
ZFOUTPUT_TYPE_TEMPLATE(ZFM_EXPAND(typename T_Pointer, typename T_ZFCorePointerType), ZFM_EXPAND(ZFCorePointerT<T_Pointer, T_ZFCorePointerType>), {v.objectInfoT(s);})

// ============================================================
/**
 * @brief declare your custom #ZFCorePointerT type
 *
 * usage
 * @code
 *   ZFCOREPOINTER_DECLARE(YourCorePointer, {
 *           logRetained(p); // define your own retain action here
 *       }, {
 *           delete p; // define your own delete action here
 *       })
 * @endcode
 * once declared, you can use as normal #ZFCorePointerT
 * @code
 *   YourCorePointer sp(zfnew(YourType));
 * @endcode
 */
#define ZFCOREPOINTER_DECLARE(T_ZFCorePointer, pointerRetainAction, pointerDeleteAction) \
    template<typename T_Pointer> \
    zfclassNotPOD _ZFP_CPT_##T_ZFCorePointer { \
    public: \
        static inline void pointerOnRetain(T_Pointer const &p) \
        pointerRetainAction \
        static inline void pointerOnDelete(T_Pointer const &p) \
        pointerDeleteAction \
    }; \
    /** @brief see #ZFCorePointerT */ \
    template<typename T_Pointer> \
    zfclassLikePOD T_ZFCorePointer : zfextend ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> > { \
    public: \
        /** @cond ZFPrivateDoc */ \
        T_ZFCorePointer(void) \
        : ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >() \
        { \
        } \
        T_ZFCorePointer(ZF_IN T_Pointer const &value) \
        : ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >(value) \
        { \
        } \
        T_ZFCorePointer(ZF_IN zfnullT const &value) \
        : ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >() \
        { \
        } \
        T_ZFCorePointer(ZF_IN ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> > const &ref) \
        : ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >(ref) \
        { \
        } \
        T_ZFCorePointer<T_Pointer> &operator = (ZF_IN T_Pointer const &value) { \
            ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >::operator = (value); \
            return *this; \
        } \
        T_ZFCorePointer<T_Pointer> &operator = (ZF_IN ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> > const &ref) { \
            ZFCorePointerT<T_Pointer, _ZFP_CPT_##T_ZFCorePointer<T_Pointer> >::operator = (ref); \
            return *this; \
        } \
        template<typename T_PointerDesired> \
        inline T_PointerDesired pointerValueT(void) const { \
            return _ZFP_ZFCorePointerHelper<T_PointerDesired>::toPointer(this); \
        } \
        inline T_Pointer operator -> (void) const { \
            return this->pointerValue(); \
        } \
        template<typename T_Ref> \
        inline T_Ref operator * (void) const { \
            return *(this->pointerValue()); \
        } \
        inline zfbool operator == (ZF_IN zfnullT const &p) const { \
            return (this->pointerValue() == zfnull); \
        } \
        inline zfbool operator != (ZF_IN zfnullT const &p) const { \
            return (this->pointerValue() != zfnull); \
        } \
        inline zfbool operator == (ZF_IN T_Pointer const &p) const { \
            return (this->pointerValue() == p); \
        } \
        inline zfbool operator != (ZF_IN T_Pointer const &p) const { \
            return (this->pointerValue() != p); \
        } \
        inline zfbool operator == (ZF_IN T_ZFCorePointer<T_Pointer> const &ref) const { \
            return (this->pointerValue() == ref.pointerValue()); \
        } \
        inline zfbool operator != (ZF_IN T_ZFCorePointer<T_Pointer> const &ref) const { \
            return (this->pointerValue() == ref.pointerValue()); \
        } \
        /** @endcond */ \
    }; \
    ZFOUTPUT_TYPE_TEMPLATE(typename T_Pointer, T_ZFCorePointer<T_Pointer>, {v.objectInfoT(s);})

ZFCOREPOINTER_DECLARE(ZFCorePointerForPointerRef, {}, {})
ZFCOREPOINTER_DECLARE(ZFCorePointerForPOD, {}, {zffree(p);})
ZFCOREPOINTER_DECLARE(ZFCorePointerForObject, {}, {zfdelete(p);})
ZFCOREPOINTER_DECLARE(ZFCorePointerForPoolObject, {}, {zfpoolDelete(p);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCorePointer_h_


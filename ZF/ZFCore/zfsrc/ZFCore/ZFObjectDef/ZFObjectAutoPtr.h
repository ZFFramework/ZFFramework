/**
 * @file ZFObjectAutoPtr.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_ZFObjectAutoPtr_h_
#define _ZFI_ZFObjectAutoPtr_h_

#include "ZFObjectRetain.h"
#include "zfauto.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFCOREPOINTER_DECLARE(ZFCorePointerForZFObject, {
        zfRetain(p);
    }, {
        zfRelease(p);
    })

// ============================================================
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zflineRelease {
public:
    template<typename T_ZFObject>
    T_ZFObject set(T_ZFObject obj) {
        this->obj = zfcast(ZFObject *, obj);
        return obj;
    }
public:
    _ZFP_zflineRelease(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zflineRelease(void) {
        zfRelease(obj);
    }
public:
    ZFObject *obj;
};
zffinal zfclassNotPOD ZFLIB_ZFCore _ZFP_zfunsafe_zflineRelease {
public:
    template<typename T_ZFObject>
    T_ZFObject set(T_ZFObject obj) {
        this->obj = zfcast(ZFObject *, obj);
        return obj;
    }
public:
    _ZFP_zfunsafe_zflineRelease(void)
    : obj(zfnull)
    {
    }
    ~_ZFP_zfunsafe_zflineRelease(void) {
        zfunsafe_zfRelease(obj);
    }
public:
    ZFObject *obj;
};
/**
 * @brief convenient method to release the object after code line end
 *
 * usage:
 * @code
 *   ZFObject *allocObject(void) {
 *       return zfAlloc(ZFObject);
 *   }
 *
 *   ZFArray *array = ...;
 *   array->add( // step 2, array auto retain it, objectRetainCount 2
 *       zflineRelease(
 *               allocObject() // step 1, objectRetainCount 1
 *           )
 *       ); // step 3, zflineRelease auto release it, objectRetainCount 1
 *
 *   // equivalence:
 *   // ZFObject *obj = allocObject();
 *   // array->add(obj);
 *   // zfRelease(obj);
 * @endcode
 * usually, this method is used to save you some code,
 * or designed for chained code
 */
#define zflineRelease(obj) (_ZFP_zflineRelease().set(obj))
/** @brief no lock version of #zflineRelease, use with caution */
#define zfunsafe_zflineRelease(obj) (_ZFP_zfunsafe_zflineRelease().set(obj))

// ============================================================
/**
 * @brief same as zflineRelease(zfAlloc(...)), see #zflineRelease
 */
#define zflineAlloc(T_ZFObject, ...) \
    zflineRelease(zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zflineAlloc, use with caution */
#define zfunsafe_zflineAlloc(T_ZFObject, ...) \
    zfunsafe_zflineRelease(zfunsafe_zfAlloc(T_ZFObject, ##__VA_ARGS__))

// ============================================================
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfblockedReleaseContainer {
public:
    template<typename T_ZFObject>
    _ZFP_zfblockedReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(zfcast(ZFObject *, obj))
    {
    }
    ~_ZFP_zfblockedReleaseContainer(void) {
        zfRelease(this->obj);
    }
private:
    ZFObject *obj;
};
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfunsafe_zfblockedReleaseContainer {
public:
    template<typename T_ZFObject>
    _ZFP_zfunsafe_zfblockedReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(zfcast(ZFObject *, obj))
    {
    }
    ~_ZFP_zfunsafe_zfblockedReleaseContainer(void) {
        zfRelease(this->obj);
    }
private:
    ZFObject *obj;
};
/**
 * @brief release the object after nearest block
 *
 * @code
 *   { // code block
 *       ZFObject *obj = ...;
 *       zfblockedRelease(obj); // mark release, must be single line
 *       return; // safe to return, auto released after exiting code block
 *   } // obj would be released after here
 * @endcode
 */
#define zfblockedRelease(obj) \
    _ZFP_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj)
/** @brief no lock version of #zfblockedRelease, use with caution */
#define zfunsafe_zfblockedRelease(obj) \
    _ZFP_zfunsafe_zfblockedReleaseContainer ZFUniqueName(zfblockedRelease) (obj)

/**
 * @brief util class to alloc and hold ZFObject type
 *
 * usage:
 * @code
 *   zfobj<MyObject> v(param0, param1);
 *
 *   // equivalence
 *   zfautoT<MyObject> v = zflineAlloc(MyObject, param0, param1);
 *   zfautoT<MyObject> v = zfobj<MyObject>(param0, param1);
 * @endcode
 *
 * note, this type is a util class in cpp world,
 * it's not reflectable and can not be used in ZFMethod,
 * use zfauto or zfautoT instead
 */
template<typename T_ZFObjectBase>
zfclassLikePOD zfobj : zfextend zfautoT<T_ZFObjectBase> {
    /** @cond ZFPrivateDoc */
public:
    zfobj(void)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase));}

    template<typename P0>
    zfobj(P0 const &p0)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0));}

    template<typename P0, typename P1>
    zfobj(P0 const &p0, P1 const &p1)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1));}

    template<typename P0, typename P1, typename P2>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2));}

    template<typename P0, typename P1, typename P2, typename P3>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2, p3));}

    template<typename P0, typename P1, typename P2, typename P3, typename P4>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4));}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5));}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5, p6));}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6, P7 const &p7)
    : zfautoT<T_ZFObjectBase>()
    {this->zfunsafe_assign(zfunsafe_zflineAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5, p6, p7));}

public:
    inline zfobj<T_ZFObjectBase> &operator = (ZF_IN zfauto const &obj) {
        zfauto::operator = (obj);
        return *this;
    }
    inline zfobj<T_ZFObjectBase> &operator = (ZF_IN zfautoT<T_ZFObjectBase> const &obj) {
        zfauto::operator = ((zfauto const &)obj);
        return *this;
    }
    inline zfobj<T_ZFObjectBase> &operator = (ZF_IN zfobj<T_ZFObjectBase> const &obj) {
        zfauto::operator = ((zfauto const &)obj);
        return *this;
    }
    template<typename T_ZFObject>
    inline zfautoT<T_ZFObjectBase> &operator = (ZF_IN T_ZFObject const &obj) {
        zfauto::operator = (obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() == _ZFP_zfanyCast(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return this->toObject() != _ZFP_zfanyCast(obj);
    }

public:
    T_ZFObjectBase *operator -> (void) const {
        return zfcast(T_ZFObjectBase *, this->toObject());
    }

public:
    inline ZFObject *toObject(void) const { // required for _ZFP_zfanyCast to work
        return zfautoT<T_ZFObjectBase>::toObject();
    }

public:
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectAutoPtr_h_


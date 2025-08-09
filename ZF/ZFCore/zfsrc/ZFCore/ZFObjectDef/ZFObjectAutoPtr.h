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
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfscopeReleaseContainer {
public:
    template<typename T_ZFObject>
    _ZFP_zfscopeReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(zfcast(ZFObject *, obj))
    {
    }
    ~_ZFP_zfscopeReleaseContainer(void) {
        zfRelease(this->obj);
    }
private:
    ZFObject *obj;
};
zffinal zfclassLikePOD ZFLIB_ZFCore _ZFP_zfunsafe_zfscopeReleaseContainer {
public:
    template<typename T_ZFObject>
    _ZFP_zfunsafe_zfscopeReleaseContainer(ZF_IN T_ZFObject obj)
    : obj(zfcast(ZFObject *, obj))
    {
    }
    ~_ZFP_zfunsafe_zfscopeReleaseContainer(void) {
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
 *       zfscopeRelease(obj); // mark release, must be single line
 *       return; // safe to return, auto released after exiting code block
 *   } // obj would be released after here
 * @endcode
 */
#define zfscopeRelease(obj) \
    _ZFP_zfscopeReleaseContainer ZFUniqueName(zfscopeRelease) (obj)
/** @brief no lock version of #zfscopeRelease, use with caution */
#define zfunsafe_zfscopeRelease(obj) \
    _ZFP_zfunsafe_zfscopeReleaseContainer ZFUniqueName(zfscopeRelease) (obj)

/**
 * @brief util class to alloc and hold ZFObject type
 *
 * usage:
 * @code
 *   zfobj<MyObject> v(param0, param1);
 *   v->func();
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
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0>
    zfobj(P0 const &p0)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1>
    zfobj(P0 const &p0, P1 const &p1)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2, typename P3>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2, p3); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2, typename P3, typename P4>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5, p6); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    zfobj(P0 const &p0, P1 const &p1, P2 const &p2, P3 const &p3, P4 const &p4, P5 const &p5, P6 const &p6, P7 const &p7)
    : zfautoT<T_ZFObjectBase>()
    {T_ZFObjectBase *v = zfunsafe_zfAlloc(T_ZFObjectBase, p0, p1, p2, p3, p4, p5, p6, p7); this->zfunsafe_assign(v); zfunsafe_zfRelease(v);}

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


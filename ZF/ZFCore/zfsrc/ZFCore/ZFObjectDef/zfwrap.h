/**
 * @file zfwrap.h
 * @brief strong reference to ZFObject
 */

#ifndef _ZFI_zfwrap_h_
#define _ZFI_zfwrap_h_

#include "ZFTypeId.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief strong reference to ZFObject
 *
 * compared to #zfweak, this class is designed for holding ZFObject types that can be modified,
 * typically output params that can be modified:
 * @code
 *   // in cpp:
 *   ZFMETHOD_FUNC_DEFINE_1(void, func1, ZFMP_IN_OUT(zfwrap &, ret)) {...}
 *   ZFMETHOD_FUNC_DEFINE_1(void, func2, ZFMP_IN_OUT(zfauto &, ret)) {...}
 *
 *   // in lua:
 *   local ret = zfwrap();
 *
 *   func1(ret);
 *   ZFLog('after func1: %s', ret:get())
 *
 *   func2(ret);
 *   ZFLog('after func2: %s', ret:get())
 * @endcode
 */
zfclassLikePOD ZFLIB_ZFCore zfwrap : zfextend zfauto {
    /** @cond ZFPrivateDoc */
public:
    zfwrap(void) : zfauto() {}
    template<typename T_ZFObject>
    zfwrap(ZF_IN T_ZFObject const &obj) : zfauto(obj) {}

public:
    template<typename T_ZFObject>
    inline zfwrap &operator = (ZF_IN T_ZFObject const &obj) {
        zfauto::operator=(obj);
        return *this;
    }

public:
    template<typename T_ZFObject>
    inline zfbool operator == (ZF_IN T_ZFObject const &obj) const {
        return zfauto::operator==(obj);
    }
    template<typename T_ZFObject>
    inline zfbool operator != (ZF_IN T_ZFObject const &obj) const {
        return zfauto::operator!=(obj);
    }

public:
    inline ZFObject *operator -> (void) const {
        return this->toObject();
    }
    inline operator zfbool (void) const {
        return this->toObject() != zfnull;
    }
    inline operator ZFObject * (void) const {
        return this->toObject();
    }
    template<typename T_ZFObject>
    inline operator T_ZFObject * (void) const {
        return zfcast(T_ZFObject *, this->toObject());
    }

public:
    static const ZFClass *ClassData(void) {
        return zfauto::ClassData();
    }
    /** @endcond */

public:
    /**
     * @brief true if the holded object is not null
     */
    inline zfbool valid(void) const {
        return this->toObject() != zfnull;
    }

    /**
     * @brief get the holded object
     */
    inline zfany get(void) const {
        return this->toObject();
    }
    /**
     * @brief set the holded object
     */
    template<typename T_ZFObject>
    void set(ZF_IN T_ZFObject const &obj) {
        zfauto::operator=(obj);
    }

public:
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const {
        return zfauto::toObject();
    }
    /**
     * @brief cast by #zfcast
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const {
        return zfcast(T_ZFObject, this->toObject());
    }

    /**
     * @brief access as #zfany
     */
    const zfany &asAny(void) const {
        return zfauto::asAny();
    }
};
ZFOUTPUT_TYPE(zfwrap, {
    if(v) {
        s += "[strong]";
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, zfwrap, zfwrap)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, zfwrap, zfwrap)

/** @cond ZFPrivateDoc */
// ============================================================
template<typename T_ZFObject>
inline zfbool operator == (ZF_IN T_ZFObject *obj, ZF_IN zfwrap const &e) {
    return e.toObject() == _ZFP_zfanyCast(obj);
}
template<typename T_ZFObject>
inline zfbool operator != (ZF_IN T_ZFObject *obj, ZF_IN zfwrap const &e) {
    return e.toObject() != _ZFP_zfanyCast(obj);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_zfwrap_h_


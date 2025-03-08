/**
 * @file ZFArgs.h
 * @brief params for #ZFListener
 */

#ifndef _ZFI_ZFArgs_h_
#define _ZFI_ZFArgs_h_

#include "zfany.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFArgs
zfclassFwd _ZFP_ZFArgsPrivate;
zfclassFwd zfauto;
/**
 * @brief listener data used by #ZFListener
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFArgs {
public:
    /**
     * @brief owner object or event sender, may be null
     * @note the sender won't be retained
     */
    zfany const &sender(void) const;
    /** @brief see #sender */
    ZFArgs &sender(ZF_IN zfany const &v);

    /**
     * @brief the result
     * @note the result would be retained
     */
    zfauto &result(void) const;
    /** @brief see #result */
    ZFArgs const &result(ZF_IN zfauto const &v) const;
    /** @brief see #result */
    ZFArgs &result(ZF_IN zfauto const &v);

    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief params, may be null
     * @note the param would be retained
     * @note params would be init as null for performance,
     *   but since null is also a valid param value,
     *   when used in #ZFMethodGenericInvoker or #ZFDI_invoke,
     *   generally you should set param as #ZFMP_DEF
     *   to make param count detect logic work,
     *   use #paramInit for short
     */
    zfauto &param0(void) const;
    /** @brief see #param0 */
    ZFArgs const &param0(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param0(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param1(void) const;
    /** @brief see #param0 */
    ZFArgs const &param1(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param1(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param2(void) const;
    /** @brief see #param0 */
    ZFArgs const &param2(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param2(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param3(void) const;
    /** @brief see #param0 */
    ZFArgs const &param3(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param3(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param4(void) const;
    /** @brief see #param0 */
    ZFArgs const &param4(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param4(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param5(void) const;
    /** @brief see #param0 */
    ZFArgs const &param5(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param5(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param6(void) const;
    /** @brief see #param0 */
    ZFArgs const &param6(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param6(ZF_IN zfauto const &v);

    /** @brief see #param0 */
    zfauto &param7(void) const;
    /** @brief see #param0 */
    ZFArgs const &param7(ZF_IN zfauto const &v) const;
    /** @brief see #param0 */
    ZFArgs &param7(ZF_IN zfauto const &v);

public:
    // ============================================================
    /**
     * @brief whether invoke success
     */
    zfbool success(void) const;
    /** @brief see #success */
    ZFArgs const &success(ZF_IN zfbool v) const;
    /** @brief see #success */
    ZFArgs &success(ZF_IN zfbool v);

    /**
     * @brief error hint
     */
    zfstring const &errorHint(void) const;
    /** @brief see #errorHint */
    ZFArgs const &errorHint(ZF_IN zfstring const &v) const;
    /** @brief see #errorHint */
    ZFArgs &errorHint(ZF_IN zfstring const &v);

    /**
     * @brief a hint flag that tell impl to ignore error,
     *   no #errorHint would be stored for performance
     */
    zfbool ignoreError(void) const;
    /** @brief see #ignoreError */
    ZFArgs &ignoreError(ZF_IN zfbool v);

    /**
     * @brief a hint flag that tell impl do not fire error event,
     *   while #errorHint would still be stored
     */
    zfbool ignoreErrorEvent(void) const;
    /** @brief see #ignoreErrorEvent */
    ZFArgs &ignoreErrorEvent(ZF_IN zfbool v);

public:
    // ============================================================
    /**
     * @brief owner method
     */
    const ZFMethod *ownerMethod(void) const;
    /** @brief see #ownerMethod */
    ZFArgs &ownerMethod(ZF_IN const ZFMethod *v);

    /**
     * @brief owner property
     */
    const ZFProperty *ownerProperty(void) const;
    /** @brief see #ownerProperty */
    ZFArgs &ownerProperty(ZF_IN const ZFProperty *v);

public:
    // ============================================================
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    zfidentity const &eventId(void) const;
    /** @brief see #eventId */
    ZFArgs &eventId(ZF_IN zfidentity const &v);
    /**
     * @brief used to achieve event filter logic
     *
     * to achieve event filter,
     * you may attach an #ZFObserver::observerAdd with higher #ZFLevel,
     * and set #eventFiltered to true,
     * then the event would not be further dispatched
     */
    zfbool eventFiltered(void) const;
    /** @brief see #eventFiltered */
    ZFArgs const &eventFiltered(ZF_IN zfbool v) const;
    /** @brief see #eventFiltered */
    ZFArgs &eventFiltered(ZF_IN zfbool v);

public:
    // ============================================================
    /**
     * @brief util method for dynamic registered method to call parent's method
     */
    zfauto callSuper(void) const;

    /**
     * @brief util for impl to init all params with #ZFMP_DEF
     */
    ZFArgs &paramInit(void);
    /**
     * @brief util for impl to init all params
     */
    ZFArgs &paramInit(
            ZF_IN ZFObject *param0
            , ZF_IN ZFObject *param1
            , ZF_IN ZFObject *param2
            , ZF_IN ZFObject *param3
            , ZF_IN ZFObject *param4
            , ZF_IN ZFObject *param5
            , ZF_IN ZFObject *param6
            , ZF_IN ZFObject *param7
            );
    /**
     * @brief util for impl to init all params
     */
    ZFArgs &paramInit(ZF_IN const ZFCoreArray<zfauto> &params);

    /** @brief get param at index */
    zfauto &paramAt(ZF_IN zfindex index) const;
    /** @brief set param at index */
    ZFArgs const &param(ZF_IN zfindex index, ZF_IN zfauto const &v) const;
    /** @brief set param at index */
    ZFArgs &param(ZF_IN zfindex index, ZF_IN zfauto const &v);

    /** @brief get property value */
    zfauto &propValue(void) const {return this->paramAt(0);}
    /** @brief set property value */
    ZFArgs const &propValue(ZF_IN zfauto const &v) const {return this->param(0, v);}
    /** @brief set property value */
    ZFArgs &propValue(ZF_IN zfauto const &v) {return this->param(0, v);}

    /** @brief get old property value */
    zfauto &propValueOld(void) const {return this->paramAt(1);}
    /** @brief set old property value */
    ZFArgs &propValueOld(ZF_IN zfauto const &v) {return this->param(1, v);}

    // ============================================================
public:
    /** @brief short form to access #sender */
    zfany const &o(void) const {return this->sender();}
    /** @brief short form to access param by name */
    zfauto p(ZF_IN const zfstring &paramName) const;

    // ============================================================
public:
    /**
     * @brief main constructor
     */
    ZFArgs(void);
    /**
     * @brief construct with another data
     */
    ZFArgs(ZF_IN const ZFArgs &ref);

public:
    /** @cond ZFPrivateDoc */
    zffinal ~ZFArgs(void);
    ZFArgs &operator = (ZF_IN const ZFArgs &ref);
    zfbool operator == (ZF_IN const ZFArgs &ref) const;
    inline zfbool operator != (ZF_IN const ZFArgs &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief get a short info of this object */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    ZFArgs &_ZFP_ZFArgs_removeConst(void) const {
        return const_cast<ZFArgs &>(*this);
    }

private:
    _ZFP_ZFArgsPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFArgs_h_


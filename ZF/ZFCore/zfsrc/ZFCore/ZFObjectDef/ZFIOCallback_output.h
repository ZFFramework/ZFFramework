/**
 * @file ZFIOCallback_output.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_output_h_
#define _ZFI_ZFIOCallback_output_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutput
/**
 * @brief general output callback
 *
 * proto type:
 * @code
 *   zfindex output(
 *           ZF_IN const void *src
 *           , ZF_IN_OPT zfindex count = zfindexMax()
 *           );
 * @endcode
 *
 * params:
 * -  (const void *) output buffer
 * -  (zfindex) byte size to write, or zfindexMax() to calculate by src
 *  (at this case, src must be NULL-terminated zfchar string)
 *
 * return:
 * -  byte size written to output,
 *   return a value less than count indicates error state
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFLIB_ZFCore, ZFOutput, ZFIOCallback)
public:
    /** @brief see #ZFOutput */
    inline zfindex execute(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) const {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
    /** @brief see #ZFOutput */
    inline zfindex operator () (
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            ) const {
        return ZFCallback::executeExact<zfindex, const void *, zfindex>(src, count);
    }
    /** @brief see #ZFOutput */
    inline const ZFOutput &output(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex count = zfindexMax()
            , ZF_OUT_OPT zfindex *result = zfnull
            ) const {
        if(result != zfnull) {
            *result = this->execute(src, count);
        }
        else {
            this->execute(src, count);
        }
        return *this;
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFLIB_ZFCore, ZFOutput, ZFIOCallback)

// ============================================================
// custom type output
#define _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    template<> \
    zfclassNotPOD ZFCoreInfoGetter<T_Type> { \
    public: \
        static void InfoGetter( \
                ZF_IN_OUT zfstring &ret \
                , ZF_IN T_Type const &v \
                ) { \
            zftToString(ret, v); \
        } \
    };
/** @brief see #ZFOUTPUT_TYPE */
template<typename T>
void zftToString(
        ZF_IN_OUT zfstring &s
        , ZF_IN T const &v
        );
/** @cond ZFPrivateDoc */
template<typename T>
inline const ZFOutput &operator << (const ZFOutput &output, T const &v) {
    zfstring s;
    zftToString(s, v);
    output.output(s.cString(), s.length() * sizeof(zfchar));
    return output;
}
/** @endcond */
/**
 * @brief declare your custom type conversion to string,
 *   convenient for debug
 *
 * proto type:
 * @code
 *   void zftToString(
 *           ZF_IN_OUT zfstring &s
 *           , ZF_IN YourType const &v
 *           );
 * @endcode
 * usage:
 * @code
 *   ZFOUTPUT_TYPE(YourType, {s += YourConverter(v);})
 * @endcode
 *
 * once declared, you may output your type to #ZFOutput by:
 * @code
 *   output << yourObject;
 * @endcode
 * or, use the declared method:
 * @code
 *   zftToString(s, v);
 * @endcode
 */
#define ZFOUTPUT_TYPE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    inline void zftToString( \
            ZF_IN_OUT zfstring &s \
            , T_Type const &v \
            ) { \
        outputAction \
    } \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   ZFOUTPUT_TYPE_TEMPLATE(
 *           ZFM_EXPAND(typename T0, typename T1)
 *           , ZFM_EXPAND(YourType<T0, T1>)
 *           , {
 *               s += YourConverter(v);
 *           }
 *           )
 * @endcode
 */
#define ZFOUTPUT_TYPE_TEMPLATE(templateList, T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    template<templateList> \
    inline void zftToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ) { \
        outputAction \
    } \
    template<templateList> \
    zfclassNotPOD ZFCoreInfoGetter<T_Type> { \
    public: \
        static void InfoGetter( \
                ZF_IN_OUT zfstring &ret \
                , ZF_IN T_Type const &v \
                ) { \
            zftToString(ret, v); \
        } \
    }; \
    /** @endcond */

/**
 * @brief see #ZFOUTPUT_TYPE
 *
 * usage:
 * @code
 *   // in header file
 *   ZFOUTPUT_TYPE_DECLARE(ZFLIB_APP, YourType)
 *   // in source file
 *   ZFOUTPUT_TYPE_DEFINE(YourType, {doYourStuff();})
 *
 *   // or, the inline version
 *   ZFOUTPUT_TYPE(YourType, {doYourStuff();})
 * @endcode
 */
#define ZFOUTPUT_TYPE_DECLARE(ZFLIB_, T_Type) \
    /** @cond ZFPrivateDoc */ \
    extern ZFLIB_ void zftToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ); \
    _ZFP_ZFOUTPUT_EXPAND(T_Type) \
    /** @endcond */
/** @brief see #ZFOUTPUT_TYPE_DECLARE */
#define ZFOUTPUT_TYPE_DEFINE(T_Type, outputAction) \
    /** @cond ZFPrivateDoc */ \
    void zftToString( \
            ZF_IN_OUT zfstring &s \
            , ZF_IN T_Type const &v \
            ) { \
        outputAction \
    } \
    /** @endcond */

// ============================================================
// common output callbacks
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       // dummy input has no contents
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputDummy "ZFOutputDummy"
/**
 * @brief a dummy output that always output nothing with success state
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputDummy(void);

/**
 * @brief create a output callback to output to a zfstring
 *
 * @note you must ensure the string to output is alive while the callback is still under use
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputForString(ZF_IN_OUT zfstring &s);
/**
 * @brief create a output callback to output to a ZFBuffer
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputForBuffer(ZF_IN_OUT ZFBuffer &buf);
/**
 * @brief create a output callback to output to a buffer
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max buffer size or zfindexMax() for no limit (you must make sure buffer is enough),
 *   if autoAppendNullToken, maxCount should contain the extra '\0' size
 * -  (zfbool) whether auto append '\0' to tail each time write
 */
extern ZFLIB_ZFCore ZFOutput ZFOutputForBufferUnsafe(
        ZF_IN void *buf
        , ZF_IN_OPT zfindex maxCount = zfindexMax()
        , ZF_IN_OPT zfbool autoAppendNullToken = zftrue
        );

// ============================================================
// basic output
ZFOUTPUT_TYPE(const zfchar *, {s += (v ? v : ZFTOKEN_zfnull);})
ZFOUTPUT_TYPE(zfchar *, {s += (const zfchar *)v;})

ZFOUTPUT_TYPE(const void *, {
    if(v) {
        zfsFromPointerT(s, v);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})
ZFOUTPUT_TYPE(void *, {
    if(v) {
        zfsFromPointerT(s, v);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})

/** @cond ZFPrivateDoc */
template<typename T_Type, int T_ZFObject = 0>
zfclassNotPOD _ZFP_zftToStringWrapper {
public:
    static void outputAction(ZF_IN_OUT zfstring &s, ZF_IN T_Type const &v) {
        zftToString(s, *v);
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_zftToStringWrapper<T_Type, 1> {
public:
    static void outputAction(ZF_IN_OUT zfstring &s, ZF_IN T_Type const &v) {
        v->toObject()->objectInfoT(s);
    }
};

template<typename T_Type>
inline void zftToString(ZF_IN_OUT zfstring &s, ZF_IN const T_Type * const &v) {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        zftToString(s, *v);
    }
}
template<typename T_Type>
inline void zftToString(ZF_IN_OUT zfstring &s, ZF_IN T_Type * const &v) {
    if(v == zfnull) {
        s += ZFTOKEN_zfnull;
    }
    else {
        _ZFP_zftToStringWrapper<T_Type *,
                zftIsZFObject(typename zftTraits<T_Type>::TrType)
            >::outputAction(s, v);
    }
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_output_h_


/**
 * @file ZFOutputForFormat.h
 * @brief output callback util
 */

#ifndef _ZFI_ZFOutputForFormat_h_
#define _ZFI_ZFOutputForFormat_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFOutputForFormat */
ZFENUM_BEGIN(ZFLIB_ZFCore, ZFOutputFormatStep)
    /**
     * @brief before anything would be outputed,
     *   ensured called only once for each output
     */
    ZFENUM_VALUE(OnInit)
    /**
     * @brief called each time before any output would be outputed
     */
    ZFENUM_VALUE(OnOutput)
    /**
     * @brief called only once just before finish/destroy the output
     */
    ZFENUM_VALUE(OnOutputEnd)
    /**
     * @brief called only once just before finish/destroy the output
     */
    ZFENUM_VALUE(OnDealloc)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(OnInit)
    ZFENUM_VALUE_REGISTER(OnOutput)
    ZFENUM_VALUE_REGISTER(OnOutputEnd)
    ZFENUM_VALUE_REGISTER(OnDealloc)
ZFENUM_END(ZFLIB_ZFCore, ZFOutputFormatStep)
ZFENUM_REG(ZFLIB_ZFCore, ZFOutputFormatStep)

// ============================================================
/** @brief see #ZFOutputForFormat */
zfinterface ZFLIB_ZFCore ZFOutputFormat : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFOutputFormat, ZFInterface)

public:
    /**
     * @brief try access the output format passed to #ZFOutputForFormat,
     *   return null if not available
     */
    ZFMETHOD_DECLARE_STATIC_1(zfanyT<ZFOutputFormat>, getFormat
            , ZFMP_IN(const ZFCallback &, output)
            )
    /**
     * @brief try access the output passed to #ZFOutputForFormat,
     *   return null callback if not available
     */
    ZFMETHOD_DECLARE_STATIC_1(ZFOutput, getOutput
            , ZFMP_IN(const ZFCallback &, output)
            )

protected:
    /**
     * @brief see #ZFOutputForFormat
     *
     * during output, it's ensured:
     * -  src is valid
     * -  srcLen is the src's length
     */
    virtual void format(
            ZF_IN_OUT zfstring &ret
            , ZF_IN ZFOutputFormatStep outputStep
            , ZF_IN const zfchar *src
            , ZF_IN zfindex srcLen
            ) zfpurevirtual;
public:
    inline void _ZFP_format(
            ZF_IN_OUT zfstring &ret
            , ZF_IN ZFOutputFormatStep outputStep
            , ZF_IN const zfchar *src
            , ZF_IN zfindex srcLen
            ) {
        this->format(ret, outputStep, src, srcLen);
    }
};

// ============================================================
/**
 * @brief see #ZFCALLBACK_SERIALIZE_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <ZFCallback callbackType="ZFOutputForFormat">
 *       <ZFCallback category="output" ... /> // the output
 *       <YourOutputFormat category="format" ... /> // the format object
 *   </ZFCallback>
 * @endcode
 */
#define ZFCallbackSerializeType_ZFOutputForFormat "ZFOutputForFormat"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_output "output"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_format "format"

// ============================================================
/** @brief see #ZFOutputForFormat */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFCore, zfbool, ZFOutputForFormatT
        , ZFMP_OUT(ZFCallback &, ret)
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(ZFOutputFormat *, format)
        )

/**
 * @brief create an output callback with specified format rule
 *
 * each time before anything would be outputed to the original output,
 * it would be processed by the format object to determin the final output\n
 * \n
 * note:
 * -  the original output and the format object would be retained by the result output
 *   until it's destroyed
 * -  the result output is serializable if both the
 *   original output and the format object are serializable
 * -  serialization may cause performance issue,
 *   you may use #ZFOutputForFormatT and #ZFCallback::callbackSerializeDisable
 *   to disable the default serialization step,
 *   see #ZFCallbackSerializeType_ZFInputForPathInfo for more info
 * -  if output success, the original output size would be returned,
 *   instead of the formated size
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, ZFOutput, ZFOutputForFormat
        , ZFMP_IN(const ZFOutput &, output)
        , ZFMP_IN(ZFOutputFormat *, format)
        )

// ============================================================
/**
 * @brief basic output format
 */
zfclass ZFLIB_ZFCore ZFOutputFormatBasic : zfextend ZFStyle, zfimplement ZFOutputFormat {
    ZFOBJECT_DECLARE(ZFOutputFormatBasic, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFOutputFormat)

public:
    /** @brief contents write before entire output */
    ZFPROPERTY_ASSIGN(zfstring, outputPrefix)
    /** @brief contents write before each line */
    ZFPROPERTY_ASSIGN(zfstring, linePrefix)
    /** @brief contents write after each line */
    ZFPROPERTY_ASSIGN(zfstring, linePostfix)
    /** @brief contents write after entire output */
    ZFPROPERTY_ASSIGN(zfstring, outputPostfix)
    /** @brief whether ignore tail endl */
    ZFPROPERTY_ASSIGN(zfbool, removeEndl)

protected:
    zfoverride
    virtual void format(
            ZF_IN_OUT zfstring &ret
            , ZF_IN ZFOutputFormatStep outputStep
            , ZF_IN const zfchar *src
            , ZF_IN zfindex srcLen
            );
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        _ZFP_outputCount = 0;
        _ZFP_needLinePrefix = zftrue;
    }

private:
    zfindex _ZFP_outputCount;
    zfbool _ZFP_needLinePrefix;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForFormat_h_


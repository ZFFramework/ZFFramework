/**
 * @file ZFTextTemplateType.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateType_h_
#define _ZFI_ZFTextTemplateType_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief index output flags for #ZFTextTemplateIndexData
 */
ZFENUM_BEGIN(ZFLIB_ZFUtility, ZFTextTemplateIndexFlag)
    ZFENUM_VALUE(LeadingZero)
    ZFENUM_VALUE(LeadingSpace)
    ZFENUM_VALUE(TailSpace)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(LeadingZero)
    ZFENUM_VALUE_REGISTER(LeadingSpace)
    ZFENUM_VALUE_REGISTER(TailSpace)
ZFENUM_END(ZFLIB_ZFUtility, ZFTextTemplateIndexFlag)

// ============================================================
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZFLIB_ZFUtility ZFTextTemplateIndexData {
public:
    /**
     * @brief start index, 0 by default
     */
    zfindex indexStart;
    /**
     * @brief increase or decrease the index, 1 by default,
     *   must not be 0
     */
    zfint indexOffset;
    /**
     * @brief radix, 10 by default
     */
    zfindex indexRadix;
    /**
     * @brief upper case or not, zftrue by default
     */
    zfbool indexUpperCase;
    /**
     * @brief specify width, 0 to disable, 0 by default
     */
    zfindex indexWidth;
    /**
     * @brief flag to use when output index,
     *   valid only if #indexWidth greater than 0,
     *   #v_ZFTextTemplateIndexFlag::EnumDefault by default
     */
    ZFTextTemplateIndexFlag indexFlag;

public:
    ZFTextTemplateIndexData(void)
    : indexStart(0)
    , indexOffset(1)
    , indexRadix(10)
    , indexUpperCase(zftrue)
    , indexWidth(0)
    , indexFlag(v_ZFTextTemplateIndexFlag::EnumDefault())
    {
    }

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateIndexData &ref) {
        this->indexStart = ref.indexStart;
        this->indexOffset = ref.indexOffset;
        this->indexRadix = ref.indexRadix;
        this->indexUpperCase = ref.indexUpperCase;
        this->indexWidth = ref.indexWidth;
        this->indexFlag = ref.indexFlag;
    }
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFTextTemplateIndexData &ref) const {
        return (zftrue
                && this->indexStart == ref.indexStart
                && this->indexOffset == ref.indexOffset
                && this->indexRadix == ref.indexRadix
                && this->indexUpperCase == ref.indexUpperCase
                && this->indexWidth == ref.indexWidth
                && this->indexFlag == ref.indexFlag
            );
    }
    inline zfbool operator != (ZF_IN const ZFTextTemplateIndexData &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateIndexData
 *       indexStart="0" // optional, 0 by default
 *       indexOffset="1" // optional, 1 by default
 *       indexRadix="10" // optional, 10 by default
 *       indexUpperCase="true" // optional, true by default
 *       indexWidth="0" // optional, 0 by default
 *       indexFlag="LeadingZero" // optional, v_ZFTextTemplateIndexFlag::EnumDefault() by default
 *   />
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUtility, ZFTextTemplateIndexData, ZFTextTemplateIndexData)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart "indexStart"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset "indexOffset"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix "indexRadix"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase "indexUpperCase"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth "indexWidth"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag "indexFlag"

ZFOUTPUT_TYPE(ZFTextTemplateIndexData, {v.objectInfoT(s);})

// ============================================================
zfclassFwd _ZFP_ZFTextTemplateParamPrivate;
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZFLIB_ZFUtility ZFTextTemplateParam {
public:
    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void replaceData(
            ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            );
    /** @brief see #ZFTextTemplateApply */
    const zfstring &replaceData(ZF_IN const zfstring &key) const;
    /** @brief see #ZFTextTemplateApply */
    zfindex replaceDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfstring &replaceDataNameAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    const zfstring &replaceDataAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemove(ZF_IN const zfstring &key);
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemoveAt(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemoveAll(void);

    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void enableDataDefault(ZF_IN zfbool enableDataDefault);
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataDefault(void) const;

    /** @brief see #ZFTextTemplateApply */
    void enableData(
            ZF_IN const zfstring &key
            , ZF_IN zfbool value
            );
    /** @brief see #ZFTextTemplateApply */
    const zfbool *enableData(ZF_IN const zfstring &key) const;
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataValue(ZF_IN const zfstring &key) const;
    /** @brief see #ZFTextTemplateApply */
    zfindex enableDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfstring &enableDataNameAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemove(ZF_IN const zfstring &key);
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemoveAt(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemoveAll(void);

    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void indexDataDefault(ZF_IN const ZFTextTemplateIndexData &indexDataDefault);
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData &indexDataDefault(void) const;

    /** @brief see #ZFTextTemplateApply */
    void indexData(
            ZF_IN const zfstring &key
            , ZF_IN const ZFTextTemplateIndexData &value
            );
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData *indexData(ZF_IN const zfstring &key) const;
    /** @brief see #ZFTextTemplateApply */
    ZFTextTemplateIndexData *indexData(ZF_IN const zfstring &key);
    /** @brief see #ZFTextTemplateApply */
    zfindex indexDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfstring &indexDataNameAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData *indexDataAt(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    ZFTextTemplateIndexData *indexDataAt(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemove(ZF_IN const zfstring &key);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemoveAt(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemoveAll(void);

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateParam &ref);

    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFTextTemplateParam &ref) const;
    inline zfbool operator != (ZF_IN const ZFTextTemplateParam &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFTextTemplateParam(void);
    ZFTextTemplateParam(ZF_IN const ZFTextTemplateParam &ref);
    ~ZFTextTemplateParam(void);
    ZFTextTemplateParam &operator = (ZF_IN const ZFTextTemplateParam &ref);
    /** @endcond */
private:
    _ZFP_ZFTextTemplateParamPrivate *d;
};

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateParam>
 *       <node category="replace"> // optional
 *           <zfstring prop="key0" value="value0" />
 *           <zfstring prop="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <node category="enable"> // optional
 *           <zfbool prop="key0" value="value0" />
 *           <zfbool prop="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <zfbool category="enableDataDefault" ... /> // optional, false by default
 *
 *       <node category="index"> // optional
 *           <ZFTextTemplateIndexData prop="key0" ... />
 *           <ZFTextTemplateIndexData prop="key1" ... />
 *           ...
 *       </node>
 *
 *       <ZFTextTemplateIndexData category="indexDataDefault" ... /> // optional
 *   </ZFTextTemplateParam>
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFUtility, ZFTextTemplateParam, ZFTextTemplateParam)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_replace "replace"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enable "enable"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault "enableDataDefault"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_index "index"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault "indexDataDefault"

ZFOUTPUT_TYPE(ZFTextTemplateParam, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateType_h_


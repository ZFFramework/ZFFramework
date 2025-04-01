/**
 * @file ZFRegExp.h
 * @brief regular expression utils
 */

#ifndef _ZFI_ZFRegExp_h_
#define _ZFI_ZFRegExp_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief compile flags for ZFRegExp
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFRegExpOption)
    /**
     * @brief no flags
     *
     * default is case sensitive,
     * no single line mode,
     * no multi line mode
     */
    ZFENUM_VALUE_WITH_INIT(None, 0)
    /**
     * @brief ignore case
     */
    ZFENUM_VALUE_WITH_INIT(IgnoreCase, 1 << 0)
    /**
     * @brief single line mode
     *
     * if single line mode,
     * '.' matches any chars including '\\n'\n
     * otherwise,
     * '.' doesn't match '\\n' (while '\\r' is also matched)
     */
    ZFENUM_VALUE_WITH_INIT(SingleLine, 1 << 1)
    /**
     * @brief multi line mode
     *
     * if multi line mode,
     * '^' matches string's begin and line's begin (after '\\n'),
     * '$' matches string's end and line's end (before '\\n')\n
     * otherwise,
     * '^' matches string's begin only,
     * '$' matches string's end only
     */
    ZFENUM_VALUE_WITH_INIT(MultiLine, 1 << 2)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(None)
    ZFENUM_VALUE_REGISTER(IgnoreCase)
    ZFENUM_VALUE_REGISTER(SingleLine)
    ZFENUM_VALUE_REGISTER(MultiLine)
ZFENUM_END_FLAGS(ZFLIB_ZFAlgorithm, ZFRegExpOption, ZFRegExpOptionFlags)
ZFENUM_REG_FLAGS(ZFLIB_ZFAlgorithm, ZFRegExpOption, ZFRegExpOptionFlags)

// ============================================================
/**
 * @brief match result info for ZFRegExp
 */
zfclassLikePOD ZFLIB_ZFAlgorithm ZFRegExpResult {
public:
    /**
     * @brief true if matched
     */
    zfbool matched;
    /**
     * @brief first matched index
     *
     * would be #ZFIndexRangeZero if not supported
     */
    ZFIndexRange matchedRange;
    /**
     * @brief matched name groups
     *
     * would be empty if not supported
     */
    ZFCoreArray<ZFIndexRange> namedGroups;

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
    zfbool operator == (ZF_IN const ZFRegExpResult &ref) const {
        return (this->matched == ref.matched
                && this->matchedRange == ref.matchedRange
                && this->namedGroups.objectCompare(ref.namedGroups) == ZFCompareEqual);
    }
    inline zfbool operator != (ZF_IN const ZFRegExpResult &ref) const {return !this->operator == (ref);}
    /** @endcond */
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFRegExpResult_matched "matched"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFRegExpResult_matchedRange "matchedRange"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFRegExpResult_namedGroups "namedGroups"

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFRegExpResult>
 *       <zfbool category="matched" value="xxx" /> // optional, false by default
 *       <ZFIndexRange category="matchedRange" value="xxx" /> // optional, ZFIndexRangeZero by default
 *       <ZFCoreArray category="namedGroups"> // optional, empty by default
 *           <ZFIndexRange /> // each namedGroups
 *       </ZFCoreArray>
 *   </ZFRegExpResult>
 * @endcode
 */
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFRegExpResult, ZFRegExpResult)
ZFTYPEID_REG(ZFLIB_ZFAlgorithm, ZFRegExpResult, ZFRegExpResult)
ZFOUTPUT_TYPE(ZFRegExpResult, {v.objectInfoT(s);})

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFRegExp_pattern "pattern"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFRegExp_flag "flag"

zfclassFwd _ZFP_ZFRegExpPrivate;
/**
 * @brief regular expression utils
 *
 * serializable data:
 * @code
 *   <ZFRegExp
 *       partten="pattern" // optional
 *       flag="ZFRegExpOptionFlags" // optional
 *   />
 * @endcode
 */
zfclass ZFLIB_ZFAlgorithm ZFRegExp : zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable {
    ZFOBJECT_DECLARE(ZFRegExp, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToString(
            ZF_IN_OUT zfstring &ret
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj) {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        ZFRegExp *another = zfcast(zfself *, anotherObj);
        this->pattern(another->pattern(), another->options());
    }

protected:
    /**
     * @brief init with pattern, see #pattern
     */
    ZFOBJECT_ON_INIT_DECLARE_2(
            ZFMP_IN(const zfstring &, pattern)
            , ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault())
            )

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

public:
    zfoverride
    virtual zfidentity objectHashImpl(void);
    /**
     * @brief ZFCompareEqual if pattern and flag both the same,
     *   ZFCompareUncomparable otherwise
     */
    zfoverride
    virtual ZFCompareResult objectCompareImpl(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief for internal use only
     */
    ZFMETHOD_DECLARE_0(void *, nativeRegExp)

public:
    /**
     * @brief get current pattern or null if not set, use #pattern to change
     */
    ZFMETHOD_DECLARE_0(const zfstring &, pattern)
    /**
     * @brief get current flag or ZFRegExpOptionNone if not set, use #pattern to change
     */
    ZFMETHOD_DECLARE_0(ZFRegExpOptionFlags, options)
    /**
     * @brief get named group's number which can be used as "$n" while #replace, or zfindexMax() if no such group
     */
    ZFMETHOD_DECLARE_1(zfindex, namedGroupIndexForName
            , ZFMP_IN(const zfstring &, name)
            )

public:
    /**
     * @brief change the pattern
     */
    ZFMETHOD_DECLARE_2(void, pattern
            , ZFMP_IN(const zfstring &, pattern)
            , ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault())
            )

    /**
     * @brief test the src with this compiled pattern
     */
    ZFMETHOD_DECLARE_3(void, find
            , ZFMP_OUT(ZFRegExpResult &, result)
            , ZFMP_IN(const zfchar *, src)
            , ZFMP_IN_OPT(zfindex, srcLength, zfindexMax())
            )

    /**
     * @brief replace src with this pattern and replacePattern
     */
    ZFMETHOD_DECLARE_6(void, replace
            , ZFMP_OUT(zfstring &, ret)
            , ZFMP_OUT(ZFRegExpResult &, result)
            , ZFMP_IN(const zfstring &, replacePattern)
            , ZFMP_IN(const zfchar *, src)
            , ZFMP_IN_OPT(zfindex, srcLength, zfindexMax())
            , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
            )

private:
    _ZFP_ZFRegExpPrivate *d;
};

/** @brief util to find by regexp */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFIndexRange, ZFRegExpFind
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, pattern)
        )
/** @brief util to find by regexp */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFIndexRange, ZFRegExpFind
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, pattern)
        )

/** @brief util to replace by regexp */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfstring, ZFRegExpReplace
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, patternFrom)
        , ZFMP_IN(const zfstring &, patternTo)
        , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
        )
/** @brief util to replace by regexp */
ZFMETHOD_FUNC_DECLARE_4(ZFLIB_ZFAlgorithm, zfstring, ZFRegExpReplace
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, patternFrom)
        , ZFMP_IN(const zfstring &, patternTo)
        , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
        )

/** @brief util to split by regexp */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFCoreArray<zfstring>, ZFRegExpSplit
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, separatorPattern)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )
/** @brief util to split by regexp */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, ZFCoreArray<zfstring>, ZFRegExpSplit
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, separatorPattern)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFRegExp_h_


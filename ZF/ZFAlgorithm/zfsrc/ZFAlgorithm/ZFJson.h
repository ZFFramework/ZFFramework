/**
 * @file ZFJson.h
 * @brief JSON processing
 */

#ifndef _ZFI_ZFJson_h_
#define _ZFI_ZFJson_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * all types:
 *     Null
 *     Value
 *     Object
 *     Array
 *
 * here is a typical json data:
    "{"
    "    \"k0\" : \"v0\","
    ""
    "    \"object0\" : {},"
    "    \"object1\" : {"
    "        \"k0\" : \"v0\","
    "        \"k1\" : \"v1\""
    "    },"
    "    \"object2\" : {"
    "        \"k0\" : {\"k0\" : \"v0\"}"
    "    },"
    ""
    "    \"array0\" : [],"
    "    \"array1\" : ["
    "        {\"k0\" : \"v0\"},"
    "        {\"k0\" : \"v0\"}"
    "    ],"
    ""
    "    \"k1\" : \"v1\""
    "}"
 */

// ============================================================
// ZFJsonType
/**
 * @brief json item type
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFJsonType)
    ZFENUM_VALUE(Null)
    ZFENUM_VALUE(Value)
    ZFENUM_VALUE(Object)
    ZFENUM_VALUE(Array)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Null)
    ZFENUM_VALUE_REGISTER(Value)
    ZFENUM_VALUE_REGISTER(Object)
    ZFENUM_VALUE_REGISTER(Array)
ZFENUM_END(ZFLIB_ZFAlgorithm, ZFJsonType)
ZFENUM_REG(ZFLIB_ZFAlgorithm, ZFJsonType)

// ============================================================
/**
 * @brief token for output json
 */
zfclassLikePOD ZFLIB_ZFAlgorithm ZFJsonOutputToken {
public:
    zfstring jsonNewLineToken; /**< @brief "\n" by default */
    zfstring jsonIndentToken; /**< @brief "  " by default */

    zfstring jsonValueSeparatorToken; /**< @brief " : " by default */
    zfstring jsonSeparatorToken; /**< @brief "," by default */
    zfstring jsonSeparatorInSameLineToken; /**< @brief ", " by default */

    zfstring jsonKeyTagLeft; /**< @brief "\"" by default */
    zfstring jsonKeyTagRight; /**< @brief "\"" by default */

    zfstring jsonValueTagLeft; /**< @brief "\"" by default */
    zfstring jsonValueTagRight; /**< @brief "\"" by default */

    zfstring jsonObjectTagLeft; /**< @brief "{" by default */
    zfstring jsonObjectTagRight; /**< @brief "}" by default */

    zfstring jsonArrayTagLeft; /**< @brief "[" by default */
    zfstring jsonArrayTagRight; /**< @brief "]" by default */

public:
    /**
     * @brief strings added to head of each new line, empty by default
     */
    zfstring jsonGlobalLineBeginToken;

    /**
     * @brief whether add new line for content of a json object, true by default
     */
    zfbool jsonObjectAddNewLineForContent;
    /**
     * @brief whether to put right tag in same line if no content, true by default
     */
    zfbool jsonObjectTagInSameLineIfNoContent;

    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayAddNewLineForContent;
    /**
     * @brief whether trim element tag in same line for a json array,
     *   valid only if jsonArrayAddNewLineForContent is true,
     *   true by default
     */
    zfbool jsonArrayContentTagInSameLine;
    /**
     * @brief whether add new line for content of a json array, true by default
     */
    zfbool jsonArrayTagInSameLineIfNoContent;

public:
    /** @cond ZFPrivateDoc */
    ZFJsonOutputToken(void)
    : jsonNewLineToken("\n")
    , jsonIndentToken("  ")
    , jsonValueSeparatorToken(" : ")
    , jsonSeparatorToken(",")
    , jsonSeparatorInSameLineToken(", ")
    , jsonKeyTagLeft("\"")
    , jsonKeyTagRight("\"")
    , jsonValueTagLeft("\"")
    , jsonValueTagRight("\"")
    , jsonObjectTagLeft("{")
    , jsonObjectTagRight("}")
    , jsonArrayTagLeft("[")
    , jsonArrayTagRight("]")
    , jsonGlobalLineBeginToken()
    , jsonObjectAddNewLineForContent(zftrue)
    , jsonObjectTagInSameLineIfNoContent(zftrue)
    , jsonArrayAddNewLineForContent(zftrue)
    , jsonArrayContentTagInSameLine(zftrue)
    , jsonArrayTagInSameLineIfNoContent(zftrue)
    {
    }
    zfbool operator == (ZF_IN ZFJsonOutputToken const &ref) const;
    zfbool operator != (ZF_IN ZFJsonOutputToken const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputToken, ZFJsonOutputToken)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFAlgorithm, ZFJsonOutputToken, ZFJsonOutputToken)
/**
 * @brief default output token for #ZFJsonToOutput
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputToken, ZFJsonOutputTokenDefault)
/**
 * @brief trim output token for #ZFJsonToOutput
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputToken, ZFJsonOutputTokenTrim)
/**
 * @brief detailed output token for #ZFJsonToOutput
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputToken, ZFJsonOutputTokenDetail)

// ============================================================
// ZFJson
zfclassFwd _ZFP_ZFJsonPrivate;
/**
 * @brief JSON item
 */
zfclassLikePOD ZFLIB_ZFAlgorithm ZFJson {
    // ============================================================
public:
    /** @cond ZFPrivateDoc */
    ZFJson(void);
    ZFJson(ZF_IN const zfnullT &dummy);
    ZFJson(ZF_IN ZFJsonType type);
    ZFJson(ZF_IN const ZFJson &ref);
    virtual ~ZFJson(void);

    ZFJson &operator = (ZF_IN const ZFJson &ref);
    ZFJson &operator = (ZF_IN const zfnullT &dummy);
    inline zfbool operator == (ZF_IN const zfnullT &ref) const {return !this->valid();}
    inline zfbool operator != (ZF_IN const zfnullT &ref) const {return this->valid();}
    zfbool operator == (ZF_IN const ZFJson &ref) const;
    inline zfbool operator != (ZF_IN const ZFJson &ref) const {return !(this->operator == (ref));}
    /** @endcond */

    // ============================================================
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
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const;

    // ============================================================
public:
    void _ZFP_ZFJson_jsonType(ZF_IN ZFJsonType type);
    /**
     * @brief type of this item
     */
    ZFJsonType type(void) const;
    /**
     * @brief true if #type is not #v_ZFJsonType::e_Null
     */
    inline zfbool valid(void) const {
        return (this->type() != v_ZFJsonType::e_Null);
    }

    /**
     * @brief return a copy of this node as well as all of its content
     *
     * the newly created tree has no parent
     */
    ZFJson copy(void) const;

    /**
     * @brief util to merge from other json
     *
     * -  arrays: would be replaced
     * -  objects: all attr value or array would be replaced,
     *   all attr object would be merged recursively
     */
    ZFJson &merge(ZF_IN const ZFJson &other);

    // ============================================================
    // for value type
public:
    /**
     * @brief value of the node, valid only for #v_ZFJsonType::e_Value type
     */
    ZFJson &value(ZF_IN const zfstring &value);
    /**
     * @brief see #value
     */
    const zfstring &value(void) const;

    // ============================================================
    // for object type
public:
    /**
     * @brief json item count, valid only for #v_ZFJsonType::e_Object
     */
    zfindex attrCount(void) const;

    /**
     * @brief set json item for key, valid only for #v_ZFJsonType::e_Object
     */
    ZFJson &attr(
            ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            );
    /**
     * @brief set json item for key, valid only for #v_ZFJsonType::e_Object
     */
    ZFJson &attr(
            ZF_IN const zfstring &key
            , ZF_IN const ZFJson &item
            );
    /**
     * @brief get json item for key, valid only for #v_ZFJsonType::e_Object
     */
    ZFJson attr(ZF_IN const zfstring &key) const;
    /**
     * @brief util method to access json value, valid only for #v_ZFJsonType::e_Object
     */
    zfstring attrValue(ZF_IN const zfstring &key) const;

    /**
     * @brief remove json item, valid only for #v_ZFJsonType::e_Object
     */
    ZFJson &attrRemove(ZF_IN const zfstring &key);
    /**
     * @brief remove all json item, valid only for #v_ZFJsonType::e_Object
     */
    ZFJson &attrRemoveAll(void);

public:
    /** @brief see #zfiter */
    zfiter attrIter(void) const;
    /** @brief see #zfiter */
    zfiter attrIterFind(ZF_IN const zfstring &key) const;
    /** @brief see #zfiter */
    zfstring attrIterKey(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    ZFJson attrIterValue(ZF_IN const zfiter &it) const;

    /** @brief see #zfiter */
    void attrIterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const ZFJson &item
            );
    /** @brief see #zfiter */
    void attrIterRemove(ZF_IN_OUT zfiter &it);

    // ============================================================
    // for array type
public:
    /**
     * @brief json child count, valid only for #v_ZFJsonType::e_Array
     */
    zfindex childCount(void) const;
    /**
     * @brief get json child at index, valid only for #v_ZFJsonType::e_Array
     */
    ZFJson childAt(ZF_IN zfindex index) const;

    /**
     * @brief add json child to specified index (ranged in [0, count]), valid only for #v_ZFJsonType::e_Array
     */
    ZFJson &child(
            ZF_IN const zfstring &value
            , ZF_IN_OPT zfindex index = zfindexMax()
            );
    /**
     * @brief add json child to specified index (ranged in [0, count]), valid only for #v_ZFJsonType::e_Array
     */
    ZFJson &child(
            ZF_IN const ZFJson &item
            , ZF_IN_OPT zfindex index = zfindexMax()
            );
    /**
     * @brief remove json child at index, valid only for #v_ZFJsonType::e_Array
     */
    ZFJson &childRemoveAt(ZF_IN zfindex index);
    /**
     * @brief remove all json child, valid only for #v_ZFJsonType::e_Array
     */
    ZFJson &childRemoveAll(void);
    /**
     * @brief find json child, valid only for #v_ZFJsonType::e_Array
     */
    zfindex childFind(ZF_IN const ZFJson &item) const;

    // ============================================================
    // quick access
public:
    /** @brief return #ZFJsonToString */
    inline operator zfstring (void) const {return this->toString();}
    /** @brief return #valid */
    inline operator zfbool (void) const {return this->valid();}
    /** @brief access #attr */
    inline ZFJson operator [] (ZF_IN const zfchar *key) const {return this->attr(key);}
    /** @brief access #attr */
    inline ZFJson operator [] (ZF_IN const zfstring &key) const {return this->attr(key);}
    /** @brief access #childAt */
    inline ZFJson operator [] (ZF_IN zfindex const &index) const {return this->childAt(index);}

    /** @brief return #ZFJsonToString */
    zfstring toString(ZF_IN_OPT const ZFJsonOutputToken &token = ZFJsonOutputTokenTrim()) const;

private:
    _ZFP_ZFJsonPrivate *d;
    friend zfclassFwd _ZFP_ZFJsonPrivate;
private:
    ZFJson(ZF_IN _ZFP_ZFJsonPrivate *ref);
};
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFJson, ZFJson)
ZFTYPEID_REG(ZFLIB_ZFAlgorithm, ZFJson, ZFJson)
ZFOUTPUT_TYPE(ZFJson, {v.objectInfoT(s);})

/** @brief util to create #ZFJson */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonObject)
/** @brief util to create #ZFJson */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonArray)

// ============================================================
/**
 * @brief parse json, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonFromInput
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

/**
 * @brief convert json to output
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFJsonToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN_OPT(const ZFJsonOutputToken &, token, ZFJsonOutputTokenDefault())
        )
/**
 * @brief convert json to string
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFJsonToStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN(const ZFJsonOutputToken &, token)
        )
/** @brief see #ZFJsonToStringT */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFJsonToString
        , ZFMP_IN(const ZFJson &, item)
        , ZFMP_IN(const ZFJsonOutputToken &, token)
        )

// ============================================================
// escape chars
/**
 * @brief encode json chars
 *
 * @code
 *   \\b        \b
 *   \\t        \t
 *   \\n        \n
 *   \\f        \f
 *   \\r        \r
 *   \"         "
 *   \\         \
 * @endcode
 *
 * result would be appended to dst
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFJsonEscapeCharEncode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfstring &, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFJsonEscapeCharEncode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )

/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFJsonEscapeCharDecode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )
/**
 * @brief see #ZFJsonEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFJsonEscapeCharDecode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJson_h_


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
 *     JsonNull
 *     JsonValue
 *     JsonObject
 *     JsonArray
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
    ZFENUM_VALUE(JsonNull)
    ZFENUM_VALUE(JsonValue)
    ZFENUM_VALUE(JsonObject)
    ZFENUM_VALUE(JsonArray)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(JsonNull)
    ZFENUM_VALUE_REGISTER(JsonValue)
    ZFENUM_VALUE_REGISTER(JsonObject)
    ZFENUM_VALUE_REGISTER(JsonArray)
ZFENUM_END(ZFLIB_ZFAlgorithm, ZFJsonType)

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
    {
    }
    zfbool operator == (ZF_IN ZFJsonOutputToken const &ref) const;
    zfbool operator != (ZF_IN ZFJsonOutputToken const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
/**
 * @brief flags to output json items
 */
zfclassLikePOD ZFLIB_ZFAlgorithm ZFJsonOutputFlags {
public:
    /**
     * @brief token to output json data
     */
    ZFJsonOutputToken jsonToken;

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
    ZFJsonOutputFlags(void)
    : jsonGlobalLineBeginToken()
    , jsonObjectAddNewLineForContent(zftrue)
    , jsonObjectTagInSameLineIfNoContent(zftrue)
    , jsonArrayAddNewLineForContent(zftrue)
    , jsonArrayContentTagInSameLine(zftrue)
    , jsonArrayTagInSameLineIfNoContent(zftrue)
    {
    }
    zfbool operator == (ZF_IN ZFJsonOutputFlags const &ref) const;
    zfbool operator != (ZF_IN ZFJsonOutputFlags const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputFlags, ZFJsonOutputFlags)
/**
 * @brief default output flags for #ZFJsonToOutput
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputFlags, ZFJsonOutputFlagsDefault)
/**
 * @brief trim output flags for #ZFJsonToOutput
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFJsonOutputFlags, ZFJsonOutputFlagsTrim)

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
    ZFJson(ZF_IN ZFJsonTypeEnum jsonType);
    ZFJson(ZF_IN const ZFJson &ref);
    virtual ~ZFJson(void);

    ZFJson &operator = (ZF_IN const ZFJson &ref);
    ZFJson &operator = (ZF_IN const zfnullT &dummy);
    inline zfbool operator == (ZF_IN const zfnullT &ref) const {return !this->jsonTypeValid();}
    inline zfbool operator != (ZF_IN const zfnullT &ref) const {return this->jsonTypeValid();}
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
    void _ZFP_ZFJson_jsonType(ZF_IN ZFJsonTypeEnum jsonType);
    /**
     * @brief type of this item
     */
    ZFJsonTypeEnum jsonType(void) const;
    /**
     * @brief true if #jsonType is not #ZFJsonType::e_JsonNull
     */
    inline zfbool jsonTypeValid(void) const {
        return (this->jsonType() != ZFJsonType::e_JsonNull);
    }

    /**
     * @brief return a copy of this node as well as all of its content
     *
     * the newly created tree has no parent
     */
    ZFJson copy(void) const;

public:
    zffinal void _ZFP_ZFJson_jsonMemoryPool_jsonValue(
            ZF_IN const zfchar *value
            , ZF_IN void *token
            );
    zffinal void _ZFP_ZFJson_jsonMemoryPool_jsonItem(
            ZF_IN const zfchar *key
            , ZF_IN void *token
            , ZF_IN const ZFJson &jsonItem
            );

    // ============================================================
    // for value type
public:
    /**
     * @brief value of the node, valid only for #ZFJsonType::e_JsonValue type
     */
    ZFJson &jsonValue(ZF_IN const zfchar *value);
    /**
     * @brief see #jsonValue
     */
    const zfchar *jsonValue(void) const;

    // ============================================================
    // for object type
public:
    /**
     * @brief set json item for key, valid only for #ZFJsonType::e_JsonObject
     */
    ZFJson &attr(
            ZF_IN const zfchar *key
            , ZF_IN const zfchar *value
            );
    /**
     * @brief set json item for key, valid only for #ZFJsonType::e_JsonObject
     */
    ZFJson &attr(
            ZF_IN const zfchar *key
            , ZF_IN const ZFJson &jsonItem
            );
    /**
     * @brief get json item for key, valid only for #ZFJsonType::e_JsonObject
     */
    ZFJson attr(ZF_IN const zfchar *key) const;
    /**
     * @brief util method to access json value, valid only for #ZFJsonType::e_JsonObject
     */
    const zfchar *attrValue(ZF_IN const zfchar *key) const;

    /**
     * @brief remove json item, valid only for #ZFJsonType::e_JsonObject
     */
    ZFJson &attrRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove all json item, valid only for #ZFJsonType::e_JsonObject
     */
    ZFJson &attrRemoveAll(void);

    /**
     * @brief json item count, valid only for #ZFJsonType::e_JsonObject
     */
    zfindex attrCount(void) const;

public:
    /** @brief see #zfiterator */
    zfiterator attrIter(void) const;
    /** @brief see #zfiterator */
    zfiterator attrIterFind(ZF_IN const zfchar *key) const;
    /** @brief see #zfiterator */
    zfbool attrIterValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    void attrIterNext(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    const zfchar *attrIterKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    ZFJson attrIterValue(ZF_IN const zfiterator &it) const;

    /** @brief see #zfiterator */
    void attrIterValue(
            ZF_IN_OUT zfiterator &it
            , ZF_IN const ZFJson &jsonItem
            );
    /** @brief see #zfiterator */
    void attrIterRemove(ZF_IN_OUT zfiterator &it);

    // ============================================================
    // for array type
public:
    /**
     * @brief json child count, valid only for #ZFJsonType::e_JsonArray
     */
    zfindex childCount(void) const;
    /**
     * @brief get json child at index, valid only for #ZFJsonType::e_JsonArray
     */
    ZFJson childAt(ZF_IN zfindex index) const;

    /**
     * @brief add json child to specified index (ranged in [0, count]), valid only for #ZFJsonType::e_JsonArray
     */
    ZFJson &childAdd(
            ZF_IN const zfchar *jsonValue
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            );
    /**
     * @brief add json child to specified index (ranged in [0, count]), valid only for #ZFJsonType::e_JsonArray
     */
    ZFJson &childAdd(
            ZF_IN const ZFJson &jsonObject
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            );
    /**
     * @brief remove json child at index, valid only for #ZFJsonType::e_JsonArray
     */
    ZFJson &childRemoveAt(ZF_IN zfindex index);
    /**
     * @brief remove all json child, valid only for #ZFJsonType::e_JsonArray
     */
    ZFJson &childRemoveAll(void);
    /**
     * @brief find json child, valid only for #ZFJsonType::e_JsonArray
     */
    zfindex childFind(ZF_IN const ZFJson &jsonObject) const;

    // ============================================================
    // quick access
public:
    /** @brief return #ZFJsonToString */
    operator zfstring (void) const;
    /** @brief return #jsonTypeValid */
    operator zfbool (void) const {return this->jsonTypeValid();}
    /** @brief access #attr */
    inline ZFJson operator [] (ZF_IN const zfchar *key) const {return this->attr(key);}
    /** @brief access #childAt */
    inline ZFJson operator [] (ZF_IN zfindex const &jsonObjectIndex) const {return this->childAt(jsonObjectIndex);}

private:
    _ZFP_ZFJsonPrivate *d;
    friend zfclassFwd _ZFP_ZFJsonPrivate;
private:
    ZFJson(ZF_IN _ZFP_ZFJsonPrivate *ref);
};
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFJson, ZFJson)
ZFOUTPUT_TYPE(ZFJson, {v.objectInfoT(s);})

/** @brief util to create #ZFJson */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonObject)
/** @brief util to create #ZFJson */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonArray)

// ============================================================
/**
 * @brief parse json, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonFromInput
        , ZFMP_IN(const ZFInput &, input)
        )
/**
 * @brief parse json, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFJson, ZFJsonFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, length, zfindexMax())
        )

/**
 * @brief convert json to output
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFJsonToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN_OPT(const ZFJsonOutputFlags &, outputFlags, ZFJsonOutputFlagsDefault())
        )
/**
 * @brief convert json to string
 *
 * @note result string is not ensured to be a valid json string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFJsonToString
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN(const ZFJsonOutputFlags &, outputFlags)
        )
/** @brief see #ZFJsonToString */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFJsonToString
        , ZFMP_IN(const ZFJson &, jsonItem)
        , ZFMP_IN(const ZFJsonOutputFlags &, outputFlags)
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
        , ZFMP_IN(const zfchar *, src)
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


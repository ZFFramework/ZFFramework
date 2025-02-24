/**
 * @file ZFXml.h
 * @brief DOM XML processing
 */

#ifndef _ZFI_ZFXml_h_
#define _ZFI_ZFXml_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * all types:
 *     Null
 *     Element
 *     Text
 *     Comment
 *     Document
 *     Declaration
 *     DocType
 *     PI
 * has name:
 *     Element, PI
 * has value:
 *     Text, Comment, DocType, PI
 * has attribute:
 *     Element, Declaration
 * has child:
 *     Element, Document
 *
 * a typical xml to test:
    <?xml type="declaration"?>
    <!DOCTYPE TYPE "DOCTYPE">
    <?pi ProcessingInstruction?>
    <element0 attribute0="attribute value0">
        <!-- here is a comment -->
        <element1 attribute0="attribute value0" />
        <element2 attribute0="attribute value0" attribute1="attribute value1"
            attribute2="attribute value2" attribute3="attribute value3" />
        <element3>text0</element3>
        <element4><![CDATA[cdata & < > [ ] text]]></element4>
        <element5><![CDATA[
        cdata & < > [ ] text
        cdata text 2
        ]]></element5>
        <element6>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>
        <element7 attr="attr with escaped chars: &lt; &gt; &amp; &apos; &quot;" />
    </element0>
 */
zfclassFwd ZFXml;

// ============================================================
// ZFXmlType
/**
 * @brief xml item type
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFXmlType)
    ZFENUM_VALUE(Null)
    ZFENUM_VALUE(Element)
    ZFENUM_VALUE(Text)
    ZFENUM_VALUE(Comment)
    ZFENUM_VALUE(Document)
    ZFENUM_VALUE(Declaration)
    ZFENUM_VALUE(DocType)
    ZFENUM_VALUE(PI)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Null)
    ZFENUM_VALUE_REGISTER(Element)
    ZFENUM_VALUE_REGISTER(Text)
    ZFENUM_VALUE_REGISTER(Comment)
    ZFENUM_VALUE_REGISTER(Document)
    ZFENUM_VALUE_REGISTER(Declaration)
    ZFENUM_VALUE_REGISTER(DocType)
    ZFENUM_VALUE_REGISTER(PI)
ZFENUM_END(ZFLIB_ZFAlgorithm, ZFXmlType)

// ============================================================
// ZFXml
zfclassFwd _ZFP_ZFXmlPrivate;
/**
 * @brief xml item that holds the xml DOM
 */
zffinal zfclassLikePOD ZFLIB_ZFAlgorithm ZFXml {
    // ============================================================
public:
    /** @cond ZFPrivateDoc */
    ZFXml(void);
    ZFXml(ZF_IN const zfnullT &dummy);
    ZFXml(ZF_IN ZFXmlType type);
    ZFXml(ZF_IN const ZFXml &ref);
    virtual ~ZFXml(void);

    ZFXml &operator = (ZF_IN const ZFXml &ref);
    ZFXml &operator = (ZF_IN const zfnullT &dummy);
    inline zfbool operator == (ZF_IN const zfnullT &dummy) const {return !this->valid();}
    inline zfbool operator != (ZF_IN const zfnullT &dummy) const {return this->valid();}
    zfbool operator == (ZF_IN const ZFXml &ref) const;
    inline zfbool operator != (ZF_IN const ZFXml &ref) const {return !(this->operator == (ref));}
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
    void _ZFP_ZFXml_xmlType(ZF_IN ZFXmlType type);
    /**
     * @brief type of this item
     */
    ZFXmlType type(void) const;
    /**
     * @brief true if #type is #v_ZFXmlType::e_Null
     */
    inline zfbool valid(void) const {
        return (this->type() != ZFXmlType::e_Null);
    }

public:
    /**
     * @brief name of the node
     *
     * what name stands for depends on the node type:
     * -  Document: always null
     * -  Declaration: always null
     * -  DocType: always null
     * -  PI: pi's name
     * -  Element: tag name
     * -  Text: always null
     * -  Comment: always null
     */
    ZFXml &name(ZF_IN const zfstring &name);
    /**
     * @brief see #name
     */
    const zfstring &name(void) const;

    /**
     * @brief value of the node
     *
     * what value stands for depends on the node type:
     * -  Document: always null
     * -  Declaration: always null
     * -  DocType: the DOCTYPE without tags
     * -  PI: the processing instruction's content
     * -  Element: always null
     * -  Text: the text itself without tags if CDATA
     * -  Comment: the comment itself without tags
     */
    ZFXml &value(ZF_IN const zfstring &value);
    /**
     * @brief see #value
     */
    const zfstring &value(void) const;

    // ============================================================
public:
    /**
     * @brief return a copy of this node as well as all of its children
     *
     * the newly created tree has no parent
     */
    ZFXml copy(void) const;

    // ============================================================
public:
    /**
     * @brief get attr count
     */
    zfindex attrCount(void) const;
    /**
     * @brief add or set attribute
     */
    ZFXml &attr(
            ZF_IN const zfstring &key
            , ZF_IN const zfstring &value
            );
    /**
     * @brief get attribute with key
     */
    zfstring attr(ZF_IN const zfstring &key) const;
    /**
     * @brief true if attribute exist
     */
    zfbool attrExist(ZF_IN const zfstring &key) const;
    /**
     * @brief remove attribute with key
     */
    ZFXml &attrRemove(ZF_IN const zfstring &key);
    /**
     * @brief remove all attribute
     */
    ZFXml &attrRemoveAll(void);

public:
    /** @brief see #zfiter */
    zfiter attrIter(void) const;
    /** @brief see #zfiter */
    zfiter attrIterFind(ZF_IN const zfstring &key) const;
    /** @brief see #zfiter */
    zfstring attrIterKey(ZF_IN const zfiter &it) const;
    /** @brief see #zfiter */
    zfstring attrIterValue(ZF_IN const zfiter &it) const;

    /** @brief see #zfiter */
    void attrIterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN const zfstring &value
            );
    /** @brief see #zfiter */
    void attrIterRemove(ZF_IN_OUT zfiter &it);

    // ============================================================
public:
    /**
     * @brief child count
     */
    zfindex childCount(void) const;
    /**
     * @brief get child at index
     */
    ZFXml childAt(ZF_IN zfindex index) const;
    /**
     * @brief add child at index
     */
    ZFXml &child(
            ZF_IN const ZFXml &item
            , ZF_IN_OPT zfindex index = zfindexMax()
            );
    /**
     * @brief remove child
     */
    ZFXml &childRemoveAt(ZF_IN zfindex index);
    /**
     * @brief remove all child
     */
    ZFXml &childRemoveAll(void);
    /**
     * @brief find child
     */
    zfindex childFind(ZF_IN const ZFXml &item) const;

    /**
     * @brief util to get first element child
     */
    ZFXml childElement(void) const;
    /**
     * @brief util to try get element at index
     */
    ZFXml childElementAt(ZF_IN zfindex index) const;

    // ============================================================
public:
    /**
     * @brief CDATA or not, for #v_ZFXmlType::e_Text
     */
    void CDATA(ZF_IN zfbool CDATA);
    /**
     * @brief see #CDATA
     */
    zfbool CDATA(void) const;

    // ============================================================
    // quick access
public:
    /** @brief return #ZFXmlToString */
    operator zfstring (void) const;
    /** @brief return #valid */
    inline operator zfbool (void) const {return this->valid();}
    /** @brief access #attr */
    inline zfstring operator [] (ZF_IN const zfchar *key) const {return this->attr(key);}
    /** @brief access #attr */
    inline zfstring operator [] (ZF_IN const zfstring &key) const {return this->attr(key);}
    /** @brief access #childAt */
    inline ZFXml operator [] (ZF_IN zfindex index) const {return this->childAt(index);}

private:
    _ZFP_ZFXmlPrivate *d;
private:
    ZFXml(ZF_IN _ZFP_ZFXmlPrivate *ref);
};
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFXml, ZFXml)
ZFOUTPUT_TYPE(ZFXml, {v.objectInfoT(s);})

/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlElement
        , ZFMP_IN(const zfstring &, name)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlText
        , ZFMP_IN(const zfstring &, value)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlComment
        , ZFMP_IN(const zfstring &, value)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDocument)
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDeclaration)
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDocType
        , ZFMP_IN(const zfstring &, value)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlPI
        , ZFMP_IN(const zfstring &, name)
        , ZFMP_IN(const zfstring &, value)
        )

// ============================================================
/**
 * @brief token used to output a xml document
 */
zffinal zfclassLikePOD ZFLIB_ZFAlgorithm ZFXmlOutputToken {
public:
    zfstring xmlNewLineToken; /**< @brief "\n" by default */
    zfstring xmlIndentToken; /**< @brief "    " by default */

    zfstring xmlDeclarationTagLeft; /**< @brief "<?xml" by default */
    zfstring xmlDeclarationTagRight; /**< @brief "?>" by default */
    zfstring xmlDocTypeTagLeft; /**< @brief "<!DOCTYPE" by default */
    zfstring xmlDocTypeTagRight; /**< @brief ">" by default */
    zfstring xmlPITagLeft; /**< @brief "<?" by default */
    zfstring xmlPITagRight; /**< @brief "?>" by default */
    zfstring xmlElementBeginTagLeft; /**< @brief "<" by default */
    zfstring xmlElementBeginTagRight; /**< @brief ">" by default */
    zfstring xmlElementEndTagLeft; /**< @brief "</" by default */
    zfstring xmlElementEndTagRight; /**< @brief ">" by default */
    zfstring xmlElementSingleTagLeft; /**< @brief "<" by default */
    zfstring xmlElementSingleTagRight; /**< @brief "/>" by default */
    zfstring xmlAttrEqualTag; /**< @brief "=" by default */
    zfstring xmlAttrQuoteTagLeft; /**< @brief "\"" by default */
    zfstring xmlAttrQuoteTagRight; /**< @brief "\"" by default */
    zfstring xmlTextCDATATagLeft; /**< @brief "<![CDATA[" by default */
    zfstring xmlTextCDATATagRight; /**< @brief "]]>" by default */
    zfstring xmlCommentTagLeft; /**< @brief "<!--" by default */
    zfstring xmlCommentTagRight; /**< @brief "-->" by default */

public:
    /**
     * @brief strings added to head of each new line, empty by default
     */
    zfstring xmlGlobalLineBeginToken;

    /**
     * @brief how many attributes before add new line, 3 by default
     *
     * e.g. if zfindexMax(), never add new line,
     * if 0, add before every attributes,
     * if 1, add before every attributes except first one
     */
    zfindex xmlElementAttrCountBeforeAddNewLine;
    /**
     * @brief add new line before entering a element if the element isn't single line, zffalse by default
     */
    zfbool xmlElementAddNewLineAtHeadIfNotSingleLine;
    /**
     * @brief trim "<element></element>" to "<element/>" if no children, zffalse by default
     */
    zfbool xmlElementTrimTagIfNoChildren;
    /**
     * @brief whether put element's end tag in a same line
     *   if no child element or only text children, zftrue by default
     */
    zfbool xmlElementEndTagAtSameLineIfNoChildElement;

public:
    /** @cond ZFPrivateDoc */
    ZFXmlOutputToken(void)
    : xmlNewLineToken("\n")
    , xmlIndentToken("    ")
    , xmlDeclarationTagLeft("<?xml")
    , xmlDeclarationTagRight("?>")
    , xmlDocTypeTagLeft("<!DOCTYPE")
    , xmlDocTypeTagRight(">")
    , xmlPITagLeft("<?")
    , xmlPITagRight("?>")
    , xmlElementBeginTagLeft("<")
    , xmlElementBeginTagRight(">")
    , xmlElementEndTagLeft("</")
    , xmlElementEndTagRight(">")
    , xmlElementSingleTagLeft("<")
    , xmlElementSingleTagRight("/>")
    , xmlAttrEqualTag("=")
    , xmlAttrQuoteTagLeft("\"")
    , xmlAttrQuoteTagRight("\"")
    , xmlTextCDATATagLeft("<![CDATA[")
    , xmlTextCDATATagRight("]]>")
    , xmlCommentTagLeft("<!--")
    , xmlCommentTagRight("-->")
    , xmlGlobalLineBeginToken()
    , xmlElementAttrCountBeforeAddNewLine(3)
    , xmlElementAddNewLineAtHeadIfNotSingleLine(zffalse)
    , xmlElementTrimTagIfNoChildren(zftrue)
    , xmlElementEndTagAtSameLineIfNoChildElement(zftrue)
    {
    }
    zfbool operator == (ZF_IN ZFXmlOutputToken const &ref) const;
    zfbool operator != (ZF_IN ZFXmlOutputToken const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputToken, ZFXmlOutputToken)

/**
 * @brief default xml output token
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputToken, ZFXmlOutputTokenDefault)
/**
 * @brief xml output token with trim format
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputToken, ZFXmlOutputTokenTrim)
/**
 * @brief xml output token with detailed format
 */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputToken, ZFXmlOutputTokenDetail)

// ============================================================
/**
 * @brief parse xml document, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlFromInput
        , ZFMP_IN(const ZFInput &, callback)
        , ZFMP_OUT_OPT(zfstring *, errorHint, zfnull)
        )

// ============================================================
/**
 * @brief convert xml to output
 *
 * @note result string is not ensured to be a valid xml string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFXmlToOutput
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN_OPT(const ZFXmlOutputToken &, token, ZFXmlOutputTokenDefault())
        )
/**
 * @brief convert xml to string
 *
 * @note result string is not ensured to be a valid xml string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFXmlToStringT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputToken &, token)
        )
/** @brief see #ZFXmlToStringT */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFXmlToString
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputToken &, token)
        )

// ============================================================
// escape chars
/**
 * @brief encode xml chars, e.g. convert "&" to "&amp;"
 *
 * support:
 * @code
 *   &lt;               <
 *   &gt;               >
 *   &amp;              &
 *   &apos;             '
 *   &quot;             "
 *   &#0; ~ &#65536;    decimal encoded char
 *   &#x0; ~ &#xFFFF;   heximal encoded char
 * @endcode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFXmlEscapeCharEncode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )
/**
 * @brief see #ZFXmlEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFXmlEscapeCharEncode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )

/**
 * @brief see #ZFXmlEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFXmlEscapeCharDecode
        , ZFMP_OUT(zfstring &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )
/**
 * @brief see #ZFXmlEscapeCharEncode
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, void, ZFXmlEscapeCharDecode
        , ZFMP_OUT(const ZFOutput &, dst)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXml_h_


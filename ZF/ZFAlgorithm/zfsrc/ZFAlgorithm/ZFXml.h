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
 *     XmlNull
 *     XmlElement
 *     XmlAttribute
 *     XmlText
 *     XmlComment
 *     XmlDocument
 *     XmlDeclaration
 *     XmlDocType
 *     XmlPI
 * has attribute:
 *     XmlElement, XmlDeclaration
 * has child:
 *     XmlElement, XmlDocument
 *
 * a typical xml to test:
    "<?xml type=\"declaration\"?>"
    "<!DOCTYPE TYPE \"DOCTYPE\">"
    "<?pi ProcessingInstruction?>"
    "<element0 attribute0=\"attribute value0\">"
        "<!-- here is a comment -->"
        "<element1 attribute0=\"attribute value0\" />"
        "<element2 attribute0=\"attribute value0\" attribute1=\"attribute value1\""
        " attribute2=\"attribute value2\" attribute3=\"attribute value3\" />"
        "<element3>text0</element3>"
        "<element4><![CDATA[cdata text]]></element4>"
        "<element5>text with escaped chars: &lt; &gt; &amp; &apos; &quot;</element5>"
    "</element0>"
 */
zfclassFwd ZFXml;

// ============================================================
// ZFXmlType
/**
 * @brief xml item type
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFXmlType)
    ZFENUM_VALUE(XmlNull)
    ZFENUM_VALUE(XmlElement)
    ZFENUM_VALUE(XmlAttribute)
    ZFENUM_VALUE(XmlText)
    ZFENUM_VALUE(XmlComment)
    ZFENUM_VALUE(XmlDocument)
    ZFENUM_VALUE(XmlDeclaration)
    ZFENUM_VALUE(XmlDocType)
    ZFENUM_VALUE(XmlPI)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(XmlNull)
    ZFENUM_VALUE_REGISTER(XmlElement)
    ZFENUM_VALUE_REGISTER(XmlAttribute)
    ZFENUM_VALUE_REGISTER(XmlText)
    ZFENUM_VALUE_REGISTER(XmlComment)
    ZFENUM_VALUE_REGISTER(XmlDocument)
    ZFENUM_VALUE_REGISTER(XmlDeclaration)
    ZFENUM_VALUE_REGISTER(XmlDocType)
    ZFENUM_VALUE_REGISTER(XmlPI)
ZFENUM_END(ZFLIB_ZFAlgorithm, ZFXmlType)

zfclassFwd ZFXmlVisitData;
/**
 * @brief callback to go through all nodes and attributes in a xml node
 *
 * proto type:
 * -  zfbool xmlVisit(const ZFXmlVisitData &);
 *
 * param:
 * -  (const ZFXmlVisitData &) visit data
 *
 * return:
 * -  (zfbool) if visitEnter, shows that whether we should continue to visit the item's attributes and children\n
 *   if not visitEnter, shows that whether we should continue to visit next siblings
 */
typedef ZFCallbackT<zfbool, const ZFXmlVisitData &> ZFXmlVisitCallback;
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlVisitCallback, ZFXmlVisitCallback)

/**
 * @brief default visit callback for visiting a xml item,
 *   you can change it at runtime
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlVisitCallback, ZFXmlVisitCallbackDefault)

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
    ZFXml(ZF_IN ZFXmlTypeEnum xmlType);
    ZFXml(ZF_IN const ZFXml &ref);
    virtual ~ZFXml(void);

    ZFXml &operator = (ZF_IN const ZFXml &ref);
    ZFXml &operator = (ZF_IN const zfnullT &dummy);
    inline zfbool operator == (ZF_IN const zfnullT &dummy) const {return !this->xmlTypeValid();}
    inline zfbool operator != (ZF_IN const zfnullT &dummy) const {return this->xmlTypeValid();}
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
    void _ZFP_ZFXml_xmlType(ZF_IN ZFXmlTypeEnum xmlType);
    /**
     * @brief type of this item
     */
    ZFXmlTypeEnum xmlType(void) const;
    /**
     * @brief true if #xmlType is #ZFXmlType::e_XmlNull
     */
    inline zfbool xmlTypeValid(void) const {
        return (this->xmlType() != ZFXmlType::e_XmlNull);
    }

    /**
     * @brief access parent of this node
     */
    ZFXml xmlParent(void) const;

public:
    /**
     * @brief name of the node
     *
     * what name stands for depends on the node type:
     * -  document: always null
     * -  declaration: always null
     * -  DOCTYPE: always null
     * -  processing instruction: pi's name
     * -  element: tag name
     * -  attribute: attribute name
     * -  text: always null
     * -  comment: always null
     */
    ZFXml &xmlName(ZF_IN const zfchar *name);
    /**
     * @brief see #xmlName
     */
    const zfchar *xmlName(void) const;

    /**
     * @brief value of the node
     *
     * what value stands for depends on the node type:
     * -  document: always null
     * -  declaration: always null
     * -  DOCTYPE: the DOCTYPE without tags
     * -  processing instruction: the processing instruction's content
     * -  element: always null
     * -  attribute: attribute value without quotes
     * -  text: the text itself without tags if CDATA
     * -  comment: the comment itself without tags
     */
    ZFXml &xmlValue(ZF_IN const zfchar *value);
    /**
     * @brief see #xmlValue
     */
    const zfchar *xmlValue(void) const;

    zffinal void _ZFP_ZFXml_xmlMemoryPool_xmlName(
            ZF_IN const zfchar *xmlName
            , ZF_IN void *token
            );
    zffinal void _ZFP_ZFXml_xmlMemoryPool_xmlValue(
            ZF_IN const zfchar *xmlValue
            , ZF_IN void *token
            );

    // ============================================================
public:
    /**
     * @brief visit the xml item and all of its attributes and children
     *
     * you may use this method to print a xml item:
     * @code
     *   xmlItem.xmlVisit(ZFXmlVisitCallbackForOutput());
     * @endcode
     * or declare your own #ZFXmlVisitCallback to go through all items in xml tree
     */
    void xmlVisit(ZF_IN const ZFXmlVisitCallback &callback) const;

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
     * @brief add attribute before beforeThis or add to tail if beforeThis is null
     */
    ZFXml &attrAdd(
            ZF_IN const ZFXml &addThis
            , ZF_IN_OPT const ZFXml *beforeThis = zfnull
            );
    /**
     * @brief util method to add attribute, do nothing if key or value is null
     */
    ZFXml &attr(
            ZF_IN const zfchar *key
            , ZF_IN const zfchar *value
            , ZF_IN_OPT const ZFXml *beforeThis = zfnull
            );
    /**
     * @brief remove attribute with name
     */
    ZFXml &attrRemove(ZF_IN const zfchar *name);
    /**
     * @brief remove attribute or do nothing if not in this node
     */
    ZFXml &attrRemove(ZF_IN const ZFXml &removeThis);
    /**
     * @brief remove all attribute
     */
    ZFXml &attrRemoveAll(void);

    /**
     * @brief get attribute with name
     */
    ZFXml attr(ZF_IN const zfchar *name) const;
    /**
     * @brief get attribute value with name
     */
    const zfchar *attrValue(ZF_IN const zfchar *name) const;

    /**
     * @brief get first attribute
     */
    ZFXml attrFirst(void) const;
    /**
     * @brief get last attribute
     */
    ZFXml attrLast(void) const;

    /**
     * @brief get next attribute
     */
    ZFXml attrNext(void) const;
    /**
     * @brief get prev attribute
     */
    ZFXml attrPrev(void) const;

    // ============================================================
public:
    /**
     * @brief sort all attribute
     */
    ZFXml &attrSort(void);
    /**
     * @brief sort all attribute as well as all descendants' attribute
     */
    ZFXml &attrSortRecursively(void);

    // ============================================================
public:
    /**
     * @brief add child before beforeThis or add to tail if beforeThis is null
     */
    ZFXml &childAdd(
            ZF_IN const ZFXml &addThis
            , ZF_IN_OPT const ZFXml *beforeThis = zfnull
            );
    /**
     * @brief remove child or do nothing if not in this node
     */
    ZFXml &childRemove(ZF_IN const ZFXml &removeThis);
    /**
     * @brief remove all child
     */
    ZFXml &childRemoveAll(void);

    /**
     * @brief get first child with name after afterThis or null if none
     */
    ZFXml childFirst(
            ZF_IN_OPT const zfchar *name = zfnull
            , ZF_IN_OPT const ZFXml *afterThis = zfnull
            ) const;
    /**
     * @brief get last child with name before beforeThis or null if none
     */
    ZFXml childLast(
            ZF_IN_OPT const zfchar *name = zfnull
            , ZF_IN_OPT const ZFXml *beforeThis = zfnull
            ) const;
    /**
     * @brief util function to #childFirst, to get first child element
     */
    ZFXml childFirstElement(
            ZF_IN_OPT const zfchar *name = zfnull
            , ZF_IN_OPT const ZFXml *afterThis = zfnull
            ) const;
    /**
     * @brief util function to #childLast, to get last child element
     */
    ZFXml childLastElement(
            ZF_IN_OPT const zfchar *name = zfnull
            , ZF_IN_OPT const ZFXml *beforeThis = zfnull
            ) const;

    /**
     * @brief get next sibling with name or null if none
     */
    ZFXml siblingNext(ZF_IN const zfchar *name = zfnull) const;
    /**
     * @brief get prev sibling with name or null if none
     */
    ZFXml siblingPrev(ZF_IN const zfchar *name = zfnull) const;

    /**
     * @brief util function to #siblingNext, to get next sibling element
     */
    ZFXml siblingNextElement(ZF_IN const zfchar *name = zfnull) const;
    /**
     * @brief util function to #siblingPrev, to get prev sibling element
     */
    ZFXml siblingPrevElement(ZF_IN const zfchar *name = zfnull) const;

    // ============================================================
public:
    /**
     * @brief CDATA or not, for #ZFXmlType::e_XmlText
     */
    void xmlTextCDATA(ZF_IN zfbool xmlTextCDATA);
    /**
     * @brief see #xmlTextCDATA
     */
    zfbool xmlTextCDATA(void) const;

    // ============================================================
    // quick access
public:
    /** @brief return #ZFXmlToString */
    operator zfstring (void) const;
    /** @brief return #xmlTypeValid */
    operator zfbool (void) const {return this->xmlTypeValid();}
    /** @brief access #attr */
    inline ZFXml operator [] (ZF_IN const zfchar *key) const {return this->attr(key);}

private:
    _ZFP_ZFXmlPrivate *d;
private:
    ZFXml(ZF_IN _ZFP_ZFXmlPrivate *ref);
};
ZFTYPEID_DECLARE(ZFLIB_ZFAlgorithm, ZFXml, ZFXml)
ZFOUTPUT_TYPE(ZFXml, {v.objectInfoT(s);})

/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlElement
        , ZFMP_IN(const zfchar *, xmlName)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlAttribute
        , ZFMP_IN(const zfchar *, xmlName)
        , ZFMP_IN(const zfchar *, xmlValue)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlText
        , ZFMP_IN(const zfchar *, xmlValue)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlComment
        , ZFMP_IN(const zfchar *, xmlValue)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDocument)
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDeclaration)
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlDocType
        , ZFMP_IN(const zfchar *, xmlValue)
        )
/** @brief util to create #ZFXml */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlPI
        , ZFMP_IN(const zfchar *, xmlName)
        , ZFMP_IN(const zfchar *, xmlValue)
        )

// ============================================================
// ZFXmlVisitData
/**
 * @brief visit type for #ZFXml::xmlVisit
 */
ZFENUM_BEGIN(ZFLIB_ZFAlgorithm, ZFXmlVisitType)
    /**
     * @brief entering visit the xml item
     */
    ZFENUM_VALUE(Enter)
    /**
     * @brief exiting visit the xml item
     *
     * for items that have attribute,
     * visit exit would occurred after all attributes visited
     */
    ZFENUM_VALUE(Exit)
    /**
     * @brief exiting visit the xml element after all children visited
     *
     * for xml element only, and the order should like this:\n
     *   1. enter element\n
     *   2.   enter attributes\n
     *   3.   exit attributes\n
     *   4. exit element\n
     *   5.   enter children\n
     *   6.   exit children\n
     *   7. exitChildren element
     */
    ZFENUM_VALUE(ExitChildren)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Enter)
    ZFENUM_VALUE_REGISTER(Exit)
    ZFENUM_VALUE_REGISTER(ExitChildren)
ZFENUM_END(ZFLIB_ZFAlgorithm, ZFXmlVisitType)
/**
 * @brief visit data when visiting a xml item, used in #ZFXmlVisitCallback
 */
zfclassLikePOD ZFLIB_ZFAlgorithm ZFXmlVisitData {
public:
    /**
     * @brief the xml item to visit
     */
    ZFXml xmlItem;
    /**
     * @brief visit type
     */
    ZFXmlVisitTypeEnum xmlVisitType;
    /**
     * @brief depth of the xml item, 0 for root item
     *
     * attributes and children has (parent's depth + 1)'s depth
     */
    zfindex depth;
    /**
     * @brief sibling index
     */
    zfindex siblingIndex;

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zffinal zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFXmlVisitData(void) {}
    ZFXmlVisitData(
            ZF_IN ZFXml const &xmlItem
            , ZF_IN ZFXmlVisitTypeEnum xmlVisitType
            , ZF_IN zfindex depth
            , ZF_IN zfindex siblingIndex
            )
    : xmlItem(xmlItem)
    , xmlVisitType(xmlVisitType)
    , depth(depth)
    , siblingIndex(siblingIndex)
    {
    }
    zfbool operator == (ZF_IN ZFXmlVisitData const &ref) const {
        return (this->xmlItem == ref.xmlItem
                && this->xmlVisitType == ref.xmlVisitType
                && this->depth == ref.depth
                && this->siblingIndex == ref.siblingIndex
            );
    }
    inline zfbool operator != (ZF_IN ZFXmlVisitData const &ref) const {return !this->operator == (ref);}
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlVisitData, ZFXmlVisitData)
ZFOUTPUT_TYPE(ZFXmlVisitData, {v.objectInfoT(s);})

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
    zfstring xmlAttributeEqualTag; /**< @brief "=" by default */
    zfstring xmlAttributeQuoteTagLeft; /**< @brief "\"" by default */
    zfstring xmlAttributeQuoteTagRight; /**< @brief "\"" by default */
    zfstring xmlAttributeSingleQuoteTagLeft; /**< @brief "\'" by default */
    zfstring xmlAttributeSingleQuoteTagRight; /**< @brief "\'" by default */
    zfstring xmlTextCDATATagLeft; /**< @brief "<![CDATA[" by default */
    zfstring xmlTextCDATATagRight; /**< @brief "]]>" by default */
    zfstring xmlCommentTagLeft; /**< @brief "<!--" by default */
    zfstring xmlCommentTagRight; /**< @brief "-->" by default */

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
    , xmlAttributeEqualTag("=")
    , xmlAttributeQuoteTagLeft("\"")
    , xmlAttributeQuoteTagRight("\"")
    , xmlAttributeSingleQuoteTagLeft("\'")
    , xmlAttributeSingleQuoteTagRight("\'")
    , xmlTextCDATATagLeft("<![CDATA[")
    , xmlTextCDATATagRight("]]>")
    , xmlCommentTagLeft("<!--")
    , xmlCommentTagRight("-->")
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
 * @brief output flags for output a xml
 */
zffinal zfclassLikePOD ZFLIB_ZFAlgorithm ZFXmlOutputFlags {
public:
    /**
     * @brief xml token to use to output a xml document
     */
    ZFXmlOutputToken xmlToken;

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
    zfindex xmlElementAttributeCountBeforeAddNewLine;
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

    /**
     * @brief use single quote for attribute's value, zffalse by default
     */
    zfbool xmlAttributeUseSingleQuote;

public:
    /** @cond ZFPrivateDoc */
    ZFXmlOutputFlags(void)
    : xmlToken()
    , xmlGlobalLineBeginToken()
    , xmlElementAttributeCountBeforeAddNewLine(3)
    , xmlElementAddNewLineAtHeadIfNotSingleLine(zffalse)
    , xmlElementTrimTagIfNoChildren(zftrue)
    , xmlElementEndTagAtSameLineIfNoChildElement(zftrue)
    , xmlAttributeUseSingleQuote(zffalse)
    {
    }
    zfbool operator == (ZF_IN ZFXmlOutputFlags const &ref) const;
    zfbool operator != (ZF_IN ZFXmlOutputFlags const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputFlags, ZFXmlOutputFlags)

/**
 * @brief default xml output flags
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputFlags, ZFXmlOutputFlagsDefault)
/**
 * @brief xml output flags with trim format
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputFlags, ZFXmlOutputFlagsTrim)
/**
 * @brief xml output flags with detailed format
 */
ZFEXPORT_VAR_READONLY_DECLARE(ZFLIB_ZFAlgorithm, ZFXmlOutputFlags, ZFXmlOutputFlagsDetailed)

/**
 * @brief create a ZFXmlVisitCallback to output xml DOM tree
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXmlVisitCallback, ZFXmlVisitCallbackForOutput
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        , ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault())
        )

// ============================================================
/**
 * @brief parse xml document, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlFromInput
        , ZFMP_IN(const ZFInput &, callback)
        )
/**
 * @brief parse xml document, or return an item with null type if fail
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, ZFXml, ZFXmlFromString
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, size, zfindexMax())
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
        , ZFMP_IN_OPT(const ZFXmlOutputFlags &, outputFlags, ZFXmlOutputFlagsDefault())
        )
/**
 * @brief convert xml to string
 *
 * @note result string is not ensured to be a valid xml string
 *   if source is not valid
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFAlgorithm, zfbool, ZFXmlToString
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputFlags &, outputFlags)
        )
/** @brief see #ZFXmlToString */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFAlgorithm, zfstring, ZFXmlToString
        , ZFMP_IN(const ZFXml &, xmlItem)
        , ZFMP_IN(const ZFXmlOutputFlags &, outputFlags)
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


#include "ZFXml.h"
#include "protocol/ZFProtocolZFXml.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFXmlType)
ZFENUM_DEFINE(ZFXmlVisitType)

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlVisitData, ZFXmlVisitData)
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlVisitCallback, ZFXmlVisitCallback)
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlVisitCallback, ZFXmlVisitCallbackDefault, ZFXmlVisitCallbackForOutput())

// ============================================================
zfbool ZFXmlOutputToken::operator == (ZF_IN ZFXmlOutputToken const &ref) const
{
    return (zftrue
            && this->xmlNewLineToken == ref.xmlNewLineToken
            && this->xmlIndentToken == ref.xmlIndentToken
            && this->xmlDeclarationTagLeft == ref.xmlDeclarationTagLeft
            && this->xmlDeclarationTagRight == ref.xmlDeclarationTagRight
            && this->xmlDocTypeTagLeft == ref.xmlDocTypeTagLeft
            && this->xmlDocTypeTagRight == ref.xmlDocTypeTagRight
            && this->xmlPITagLeft == ref.xmlPITagLeft
            && this->xmlPITagRight == ref.xmlPITagRight
            && this->xmlElementBeginTagLeft == ref.xmlElementBeginTagLeft
            && this->xmlElementBeginTagRight == ref.xmlElementBeginTagRight
            && this->xmlElementEndTagLeft == ref.xmlElementEndTagLeft
            && this->xmlElementEndTagRight == ref.xmlElementEndTagRight
            && this->xmlElementSingleTagLeft == ref.xmlElementSingleTagLeft
            && this->xmlElementSingleTagRight == ref.xmlElementSingleTagRight
            && this->xmlAttributeEqualTag == ref.xmlAttributeEqualTag
            && this->xmlAttributeQuoteTagLeft == ref.xmlAttributeQuoteTagLeft
            && this->xmlAttributeQuoteTagRight == ref.xmlAttributeQuoteTagRight
            && this->xmlAttributeSingleQuoteTagLeft == ref.xmlAttributeSingleQuoteTagLeft
            && this->xmlAttributeSingleQuoteTagRight == ref.xmlAttributeSingleQuoteTagRight
            && this->xmlTextCDATATagLeft == ref.xmlTextCDATATagLeft
            && this->xmlTextCDATATagRight == ref.xmlTextCDATATagRight
            && this->xmlCommentTagLeft == ref.xmlCommentTagLeft
            && this->xmlCommentTagRight == ref.xmlCommentTagRight
        );
}
zfbool ZFXmlOutputFlags::operator == (ZF_IN ZFXmlOutputFlags const &ref) const
{
    return (zftrue
            && this->xmlToken == ref.xmlToken
            && this->xmlGlobalLineBeginToken == ref.xmlGlobalLineBeginToken
            && this->xmlElementAddNewLineAtHeadIfNotSingleLine == ref.xmlElementAddNewLineAtHeadIfNotSingleLine
            && this->xmlElementAttributeCountBeforeAddNewLine == ref.xmlElementAttributeCountBeforeAddNewLine
            && this->xmlElementTrimTagIfNoChildren == ref.xmlElementTrimTagIfNoChildren
            && this->xmlElementEndTagAtSameLineIfNoChildElement == ref.xmlElementEndTagAtSameLineIfNoChildElement
            && this->xmlAttributeUseSingleQuote == ref.xmlAttributeUseSingleQuote
        );
}
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlags)

// ============================================================
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsDefault, ZFXmlOutputFlags())

static const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsTrimInit(void)
{
    static ZFXmlOutputFlags d;
    d.xmlElementAttributeCountBeforeAddNewLine = zfindexMax();
    d.xmlElementTrimTagIfNoChildren = zftrue;
    d.xmlToken.xmlNewLineToken.removeAll();
    d.xmlToken.xmlIndentToken.removeAll();
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsTrim, _ZFP_ZFXmlOutputFlagsTrimInit())

static const ZFXmlOutputFlags &_ZFP_ZFXmlOutputFlagsDetailedInit(void)
{
    static ZFXmlOutputFlags d;
    d.xmlElementAddNewLineAtHeadIfNotSingleLine = zftrue;
    d.xmlElementAttributeCountBeforeAddNewLine = 1;
    d.xmlElementEndTagAtSameLineIfNoChildElement = zftrue;
    return d;
}
ZFEXPORT_VAR_READONLY_DEFINE(ZFXmlOutputFlags, ZFXmlOutputFlagsDetailed, _ZFP_ZFXmlOutputFlagsDetailedInit())

// ============================================================
static zfbool _ZFP_ZFXmlOutputElementUseSingleTag(ZF_IN const ZFXmlItem &element,
                                                  ZF_IN const ZFXmlOutputFlags &flags,
                                                  ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementTrimTagIfNoChildren && !element.childFirst());
}
static zfbool _ZFP_ZFXmlOutputElementAttributeNeedNewLine(ZF_IN const ZFXmlItem &element,
                                                          ZF_IN const ZFXmlOutputFlags &flags,
                                                          ZF_IN const ZFXmlVisitData &data)
{
    if(!element)
    {
        return zffalse;
    }
    ZFXmlItem xmlAttribute = element.attrFirst();
    if(flags.xmlElementAttributeCountBeforeAddNewLine == zfindexMax())
    {
        return zffalse;
    }
    if(flags.xmlElementAttributeCountBeforeAddNewLine == 0)
    {
        return xmlAttribute;
    }
    zfindex xmlAttributeCount = 0;
    while(xmlAttribute)
    {
        ++xmlAttributeCount;
        if(xmlAttributeCount > flags.xmlElementAttributeCountBeforeAddNewLine)
        {
            return zftrue;
        }
        xmlAttribute = xmlAttribute.attrNext();
    }
    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputElementChildNeedNewLine(ZF_IN const ZFXmlItem &xmlChild,
                                                      ZF_IN const ZFXmlOutputFlags &flags,
                                                      ZF_IN const ZFXmlVisitData &data)
{
    if(flags.xmlElementTrimTagIfNoChildren && !xmlChild)
    {
        return zffalse;
    }
    if(!flags.xmlElementEndTagAtSameLineIfNoChildElement)
    {
        return zftrue;
    }

    if(xmlChild.xmlType() != ZFXmlType::e_XmlText)
    {
        return zftrue;
    }
    for(ZFXmlItem t = xmlChild.siblingNext(); t; t = t.siblingNext())
    {
        if(t.xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }
    for(ZFXmlItem t = xmlChild.siblingPrev(); t; t = t.siblingPrev())
    {
        if(t.xmlType() != ZFXmlType::e_XmlText)
        {
            return zftrue;
        }
    }

    return zffalse;
}
static zfbool _ZFP_ZFXmlOutputAttributeNeedNewLine(ZF_IN const ZFXmlItem &xmlAttribute,
                                                   ZF_IN const ZFXmlOutputFlags &flags,
                                                   ZF_IN const ZFXmlVisitData &data)
{
    return (flags.xmlElementAttributeCountBeforeAddNewLine == 0
        || (flags.xmlElementAttributeCountBeforeAddNewLine != zfindexMax()
            && data.siblingIndex > 0
            && (data.siblingIndex % flags.xmlElementAttributeCountBeforeAddNewLine) == 0));
}
zfclass _ZFP_I_ZFXmlOutputOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFXmlOutputOwner, ZFObject)

public:
    ZFOutput outputCallback;
    ZFXmlOutputFlags flags;

public:
    ZFMETHOD_INLINE_1(zfbool, onVisit, ZFMP_IN(const ZFXmlVisitData &, data))
    {
        switch(data.xmlItem.xmlType())
        {
            case ZFXmlType::e_XmlElement:
                return this->onVisitXmlElement(data);
            case ZFXmlType::e_XmlAttribute:
                return this->onVisitXmlAttribute(data);
            case ZFXmlType::e_XmlText:
                return this->onVisitXmlText(data);
            case ZFXmlType::e_XmlComment:
                return this->onVisitXmlComment(data);
            case ZFXmlType::e_XmlDocument:
                return this->onVisitXmlDocument(data);
            case ZFXmlType::e_XmlDeclaration:
                return this->onVisitXmlDeclaration(data);
            case ZFXmlType::e_XmlDocType:
                return this->onVisitXmlDocType(data);
            case ZFXmlType::e_XmlPI:
                return this->onVisitXmlPI(data);
            default:
                return zffalse;
        }
    }
private:
    zfbool onVisitXmlElement(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                if((data.depth > 0 && data.siblingIndex > 0) && flags.xmlElementAddNewLineAtHeadIfNotSingleLine
                    && (_ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data)
                        || _ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem.childFirst(), flags, data)))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                }
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagLeft);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagLeft);
                }
                this->add(data.xmlItem.xmlName());
                break;
            case ZFXmlVisitType::e_Exit:
                if(data.xmlItem.attrFirst()
                    && _ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(" ");
                }
                if(_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlElementSingleTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlElementBeginTagRight);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
                if(!_ZFP_ZFXmlOutputElementUseSingleTag(data.xmlItem, flags, data))
                {
                    if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem.childFirst(), flags, data)
                        || _ZFP_ZFXmlOutputElementAttributeNeedNewLine(data.xmlItem, flags, data))
                    {
                        this->add(flags.xmlToken.xmlNewLineToken);
                        this->add(flags.xmlGlobalLineBeginToken);
                        this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                    }
                    this->add(flags.xmlToken.xmlElementEndTagLeft);
                    this->add(data.xmlItem.xmlName());
                    this->add(flags.xmlToken.xmlElementEndTagRight);
                }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlAttribute(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(_ZFP_ZFXmlOutputAttributeNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                else if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(" ");
                }
                this->add(data.xmlItem.xmlName());
                this->add(flags.xmlToken.xmlAttributeEqualTag);
                if(flags.xmlAttributeUseSingleQuote)
                {
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem.xmlValue());
                    this->add(flags.xmlToken.xmlAttributeSingleQuoteTagRight);
                }
                else
                {
                    this->add(flags.xmlToken.xmlAttributeQuoteTagLeft);
                    ZFXmlEscapeCharEncode(this->outputCallback, data.xmlItem.xmlValue());
                    this->add(flags.xmlToken.xmlAttributeQuoteTagRight);
                }
            }
                break;
            case ZFXmlVisitType::e_Exit:
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlText(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                }
                if(data.xmlItem.xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagLeft);
                }
                this->addEncoded(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                if(data.xmlItem.xmlTextCDATA())
                {
                    this->add(flags.xmlToken.xmlTextCDATATagRight);
                }
                if(_ZFP_ZFXmlOutputElementChildNeedNewLine(data.xmlItem, flags, data))
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                    this->add(flags.xmlGlobalLineBeginToken);
                    this->addIndent(flags.xmlToken.xmlIndentToken, data.depth - 1);
                }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlComment(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
            {
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlCommentTagLeft);
                this->add(data.xmlItem.xmlValue());
                break;
            }
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlCommentTagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDocument(ZF_IN const ZFXmlVisitData &data)
    {
        return zftrue;
    }
    zfbool onVisitXmlDeclaration(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDeclarationTagLeft);
                break;
            case ZFXmlVisitType::e_Exit:
            {
                if(!data.xmlItem.attrFirst())
                {
                    this->add(" ");
                }
                this->add(flags.xmlToken.xmlDeclarationTagRight);
            }
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlDocType(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlDocTypeTagLeft);
                this->add(" ");
                this->add(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlDocTypeTagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }
    zfbool onVisitXmlPI(ZF_IN const ZFXmlVisitData &data)
    {
        switch(data.xmlVisitType)
        {
            case ZFXmlVisitType::e_Enter:
                if(data.depth > 0 || data.siblingIndex > 0)
                {
                    this->add(flags.xmlToken.xmlNewLineToken);
                }
                this->add(flags.xmlGlobalLineBeginToken);
                this->addIndent(flags.xmlToken.xmlIndentToken, data.depth);
                this->add(flags.xmlToken.xmlPITagLeft);
                this->add(data.xmlItem.xmlName());
                this->add(" ");
                this->add(data.xmlItem.xmlValue());
                break;
            case ZFXmlVisitType::e_Exit:
                this->add(flags.xmlToken.xmlPITagRight);
                break;
            case ZFXmlVisitType::e_ExitChildren:
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return zftrue;
    }

private:
    inline void add(const zfchar *s)
    {
        if(s && *s)
        {
            this->outputCallback.execute(s);
        }
    }
    inline void addEncoded(const zfchar *s)
    {
        if(s && *s)
        {
            ZFXmlEscapeCharEncode(this->outputCallback, s);
        }
    }
    void addIndent(const zfchar *xmlIndentToken, zfindex indentLevel = 1)
    {
        if(xmlIndentToken && *xmlIndentToken)
        {
            for(zfindex i = 0; i < indentLevel; ++i)
            {
                this->outputCallback.execute(xmlIndentToken);
            }
        }
    }
};

ZFMETHOD_FUNC_DEFINE_2(ZFXmlVisitCallback, ZFXmlVisitCallbackForOutput,
                       ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault()),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))
{
    if(!outputCallback)
    {
        return zfnull;
    }
    _ZFP_I_ZFXmlOutputOwner *owner = zfAlloc(_ZFP_I_ZFXmlOutputOwner);
    owner->outputCallback = outputCallback;
    owner->flags = flags;
    ZFXmlVisitCallback callback = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFXmlOutputOwner, onVisit));
    callback.callbackOwnerObjectRetain();
    zfRelease(owner);
    return callback;
}

// ============================================================
zfclassLikePOD _ZFP_ZFXmlMemoryPoolString
{
private:
    void *_token;
    union {
        zfchar *_s;
        const zfchar *_sInPool;
    } _s;

private:
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const _ZFP_ZFXmlMemoryPoolString &ref);
    _ZFP_ZFXmlMemoryPoolString &operator = (ZF_IN const _ZFP_ZFXmlMemoryPoolString &ref);
public:
    _ZFP_ZFXmlMemoryPoolString(void)
    : _token(zfnull)
    , _s()
    {
    }
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const zfchar *s)
    : _token(zfnull)
    , _s()
    {
        _s._s = zfsCopy(s);
    }
    _ZFP_ZFXmlMemoryPoolString(ZF_IN const zfchar *s, ZF_IN void *token)
    : _token(token)
    , _s()
    {
        _s._sInPool = s;
    }
    ~_ZFP_ZFXmlMemoryPoolString(void)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
        }
        else
        {
            zffree(_s._s);
        }
    }

public:
    void value(ZF_IN const zfchar *value)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        zfsChange(_s._s, value);
    }
    void value(ZF_IN const zfchar *value, ZF_IN void *token)
    {
        if(_token)
        {
            ZFPROTOCOL_ACCESS(ZFXml)->xmlMemoryPoolRelease(_token, _s._sInPool);
            _token = zfnull;
            _s._s = zfnull;
        }
        else
        {
            zffree(_s._s);
        }
        _token = token;
        _s._sInPool = value;
    }
    inline const zfchar *value(void) const
    {
        return _s._sInPool;
    }
};
zfclassNotPOD _ZFP_ZFXmlItemPrivate
{
public:
    zfuint refCount;
    ZFXmlTypeEnum xmlType;
    _ZFP_ZFXmlItemPrivate *xmlParent;
    _ZFP_ZFXmlMemoryPoolString xmlName;
    _ZFP_ZFXmlMemoryPoolString xmlValue;

    _ZFP_ZFXmlItemPrivate *attrFirst;
    _ZFP_ZFXmlItemPrivate *attrLast;
    _ZFP_ZFXmlItemPrivate *childFirst;
    _ZFP_ZFXmlItemPrivate *childLast;

    // prev/next attribute or sibling depending on xmlType
    _ZFP_ZFXmlItemPrivate *siblingPrev;
    _ZFP_ZFXmlItemPrivate *siblingNext;

    zfbool attrNeedSort;
    zfbool xmlTextCDATA;

public:
    explicit _ZFP_ZFXmlItemPrivate(ZF_IN ZFXmlTypeEnum xmlType)
    : refCount(1)
    , xmlType(xmlType)
    , xmlParent(zfnull)
    , xmlName()
    , xmlValue()
    , attrFirst(zfnull)
    , attrLast(zfnull)
    , childFirst(zfnull)
    , childLast(zfnull)
    , siblingPrev(zfnull)
    , siblingNext(zfnull)
    , attrNeedSort(zftrue)
    , xmlTextCDATA(zffalse)
    {
    }
    ~_ZFP_ZFXmlItemPrivate(void)
    {
        this->attrRemoveAll();
        this->childRemoveAll();
    }

public:
    void attrRemoveAll(void)
    {
        if(this->attrFirst)
        {
            _ZFP_ZFXmlItemPrivate *xmlAttribute = this->attrFirst;
            this->attrFirst = zfnull;
            this->attrLast = zfnull;
            while(xmlAttribute != zfnull)
            {
                _ZFP_ZFXmlItemPrivate *xmlAttributeTmp = xmlAttribute;
                xmlAttribute = xmlAttribute->siblingNext;

                xmlAttributeTmp->xmlParent = zfnull;
                xmlAttributeTmp->siblingPrev = zfnull;
                xmlAttributeTmp->siblingNext = zfnull;

                --(xmlAttributeTmp->refCount);
                if(xmlAttributeTmp->refCount == 0)
                {
                    zfdelete(xmlAttributeTmp);
                }
            }
        }
    }
    void childRemoveAll(void)
    {
        if(this->childFirst)
        {
            _ZFP_ZFXmlItemPrivate *xmlChild = this->childFirst;
            this->childFirst = zfnull;
            this->childLast = zfnull;
            while(xmlChild != zfnull)
            {
                _ZFP_ZFXmlItemPrivate *xmlChildTmp = xmlChild;
                xmlChild = xmlChild->siblingNext;

                xmlChildTmp->xmlParent = zfnull;
                xmlChildTmp->siblingPrev = zfnull;
                xmlChildTmp->siblingNext = zfnull;

                --(xmlChildTmp->refCount);
                if(xmlChildTmp->refCount == 0)
                {
                    zfdelete(xmlChildTmp);
                }
            }
        }
    }

public:
    void attrAttach(ZF_IN _ZFP_ZFXmlItemPrivate *addThis, ZF_IN _ZFP_ZFXmlItemPrivate *beforeThis)
    {
        this->attrNeedSort = zftrue;
        ++(addThis->refCount);
        addThis->xmlParent = this;

        if(beforeThis == zfnull)
        {
            addThis->siblingPrev = this->attrLast;
            if(this->attrLast == zfnull)
            {
                this->attrLast = addThis;
            }
            else
            {
                this->attrLast->siblingNext = addThis;
                this->attrLast = addThis;
            }
            if(this->attrFirst == zfnull)
            {
                this->attrFirst = addThis;
            }
        }
        else
        {
            if(beforeThis->siblingPrev == zfnull)
            {
                this->attrFirst = addThis;
            }
            else
            {
                beforeThis->siblingPrev->siblingNext = addThis;
            }
            addThis->siblingPrev = beforeThis->siblingPrev;
            beforeThis->siblingPrev = addThis;
            addThis->siblingNext = beforeThis;
        }
    }
    void attrDetach(ZF_IN _ZFP_ZFXmlItemPrivate *removeThis)
    {
        this->attrNeedSort = zftrue;
        --(removeThis->refCount);
        removeThis->xmlParent = zfnull;
        if(this->attrFirst == removeThis)
        {
            this->attrFirst = removeThis->siblingNext;
        }
        if(this->attrLast == removeThis)
        {
            this->attrLast = removeThis->siblingPrev;
        }
        if(removeThis->siblingPrev != zfnull)
        {
            removeThis->siblingPrev->siblingNext = removeThis->siblingNext;
        }
        if(removeThis->siblingNext != zfnull)
        {
            removeThis->siblingNext->siblingPrev = removeThis->siblingPrev;
        }
        removeThis->siblingPrev = zfnull;
        removeThis->siblingNext = zfnull;

        if(removeThis->refCount == 0)
        {
            zfdelete(removeThis);
        }
    }
    void childAttach(ZF_IN _ZFP_ZFXmlItemPrivate *addThis, ZF_IN _ZFP_ZFXmlItemPrivate *beforeThis)
    {
        ++(addThis->refCount);
        addThis->xmlParent = this;

        if(beforeThis == zfnull)
        {
            addThis->siblingPrev = this->childLast;
            if(this->childLast == zfnull)
            {
                this->childLast = addThis;
            }
            else
            {
                this->childLast->siblingNext = addThis;
                this->childLast = addThis;
            }
            if(this->childFirst == zfnull)
            {
                this->childFirst = addThis;
            }
        }
        else
        {
            if(beforeThis->siblingPrev == zfnull)
            {
                this->childFirst = addThis;
            }
            else
            {
                beforeThis->siblingPrev->siblingNext = addThis;
            }
            addThis->siblingPrev = beforeThis->siblingPrev;
            beforeThis->siblingPrev = addThis;
            addThis->siblingNext = beforeThis;
        }
    }
    void childDetach(ZF_IN _ZFP_ZFXmlItemPrivate *removeThis)
    {
        --(removeThis->refCount);
        removeThis->xmlParent = zfnull;
        if(this->childFirst == removeThis)
        {
            this->childFirst = removeThis->siblingNext;
        }
        if(this->childLast == removeThis)
        {
            this->childLast = removeThis->siblingPrev;
        }
        if(removeThis->siblingPrev != zfnull)
        {
            removeThis->siblingPrev->siblingNext = removeThis->siblingNext;
        }
        if(removeThis->siblingNext != zfnull)
        {
            removeThis->siblingNext->siblingPrev = removeThis->siblingPrev;
        }
        removeThis->siblingPrev = zfnull;
        removeThis->siblingNext = zfnull;

        if(removeThis->refCount == 0)
        {
            zfdelete(removeThis);
        }
    }

public:
    static ZFCompareResult _ZFP_attrSortComparer(ZF_IN _ZFP_ZFXmlItemPrivate * const &v0, ZF_IN _ZFP_ZFXmlItemPrivate * const &v1)
    {
        zfint cmpResult = zfscmp(v0->xmlName.value(), v1->xmlName.value());
        if(cmpResult < 0)
        {
            return ZFCompareSmaller;
        }
        else if(cmpResult == 0)
        {
            return ZFCompareTheSame;
        }
        else
        {
            return ZFCompareGreater;
        }
    }
    void attrSort(void)
    {
        if(!this->attrNeedSort)
        {
            return;
        }
        this->attrNeedSort = zffalse;

        if(this->attrFirst != zfnull)
        {
            ZFCoreArrayPOD<_ZFP_ZFXmlItemPrivate *> tmp;
            _ZFP_ZFXmlItemPrivate *xmlAttribute = this->attrFirst;
            while(xmlAttribute != zfnull)
            {
                tmp.add(xmlAttribute);
                xmlAttribute = xmlAttribute->siblingNext;
            }
            tmp.sort(_ZFP_attrSortComparer);

            this->attrFirst = tmp[0];
            this->attrLast = tmp[tmp.count() - 1];

            for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
            {
                xmlAttribute = tmp[i];
                if(i + 1 < tmp.count())
                {
                    xmlAttribute->siblingNext = tmp[i + 1];
                }
                else
                {
                    xmlAttribute->siblingNext = zfnull;
                }
                if(i > 0)
                {
                    xmlAttribute->siblingPrev = tmp[i - 1];
                }
                else
                {
                    xmlAttribute->siblingPrev = zfnull;
                }
            }
        }
    }
    void attrSortRecursively(void)
    {
        this->attrSort();

        if(this->xmlType == ZFXmlType::e_XmlElement || this->xmlType == ZFXmlType::e_XmlDocument)
        {
            _ZFP_ZFXmlItemPrivate *xmlChild = this->childFirst;
            while(xmlChild != zfnull)
            {
                xmlChild->attrSortRecursively();
                xmlChild = xmlChild->siblingNext;
            }
        }
    }
};

static void _ZFP_ZFXmlCopyNode(ZF_IN ZFXmlItem &to, ZF_IN const ZFXmlItem &from)
{
    to.xmlName(from.xmlName());
    to.xmlValue(from.xmlValue());
    switch(from.xmlType())
    {
        case ZFXmlType::e_XmlText:
        {
            to.xmlTextCDATA(from.xmlTextCDATA());
            break;
        }
        case ZFXmlType::e_XmlElement:
        case ZFXmlType::e_XmlDeclaration:
        {
            ZFXmlItem fromAttribute = from.attrFirst();
            while(fromAttribute)
            {
                to.attrAdd(fromAttribute.xmlClone());
                fromAttribute = fromAttribute.attrNext();
            }
            break;
        }
        default:
            break;
    }
}
static void _ZFP_ZFXmlCopyTree(ZF_IN ZFXmlItem &to, ZF_IN const ZFXmlItem &from)
{
    _ZFP_ZFXmlCopyNode(to, from);

    switch(from.xmlType())
    {
        case ZFXmlType::e_XmlElement:
        {
            ZFXmlItem fromChild = from.childFirst();
            while(fromChild)
            {
                to.childAdd(fromChild.xmlCloneTree());
                fromChild = fromChild.siblingNext();
            }
            break;
        }
        case ZFXmlType::e_XmlDocument:
        {
            ZFXmlItem fromChild = from.childFirst();
            while(fromChild)
            {
                to.childAdd(fromChild.xmlCloneTree());
                fromChild = fromChild.siblingNext();
            }
            break;
        }
        default:
            break;
    }
}

#define _ZFP_ZFXmlAssertCanHaveAttribute(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlElement || (item).xmlType() == ZFXmlType::e_XmlDeclaration)
#define _ZFP_ZFXmlAssertCanHaveChild(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlElement || (item).xmlType() == ZFXmlType::e_XmlDocument)
#define _ZFP_ZFXmlAssertCanBeAttribute(item) \
    zfCoreAssert((item).xmlType() == ZFXmlType::e_XmlAttribute)

// ============================================================
// ZFXmlItem
ZFXmlItem::ZFXmlItem(ZF_IN _ZFP_ZFXmlItemPrivate *ref)
: d(ref)
{
    if(d)
    {
        ++(d->refCount);
    }
}

ZFXmlItem::ZFXmlItem(void)
: d(zfnull)
{
}
ZFXmlItem::ZFXmlItem(ZF_IN const zfnullT &dummy)
: d(zfnull)
{
}
ZFXmlItem::ZFXmlItem(ZF_IN ZFXmlTypeEnum xmlType)
{
    if(xmlType != ZFXmlType::e_XmlNull)
    {
        d = zfnew(_ZFP_ZFXmlItemPrivate, xmlType);
    }
    else
    {
        d = zfnull;
    }
}
ZFXmlItem::ZFXmlItem(ZF_IN const ZFXmlItem &ref)
: d(ref.d)
{
    if(d)
    {
        ++(d->refCount);
    }
}
ZFXmlItem::~ZFXmlItem(void)
{
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
    }
}

ZFXmlItem &ZFXmlItem::operator = (ZF_IN const ZFXmlItem &ref)
{
    _ZFP_ZFXmlItemPrivate *dTmp = d;
    d = ref.d;
    if(d)
    {
        ++(d->refCount);
    }
    if(dTmp)
    {
        --(dTmp->refCount);
        if(dTmp->refCount == 0)
        {
            zfdelete(dTmp);
        }
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::operator = (ZF_IN const zfnullT &dummy)
{
    if(d)
    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
        d = zfnull;
    }
    return *this;
}
zfbool ZFXmlItem::operator == (ZF_IN const ZFXmlItem &ref) const
{
    return (d == ref.d);
}

// ============================================================
void ZFXmlItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ZFXmlItemToString(ret, *this, ZFXmlOutputFlagsTrim());
}

zfindex ZFXmlItem::objectRetainCount(void) const
{
    return d ? d->refCount : 0;
}

// ============================================================
void ZFXmlItem::_ZFP_ZFXmlItem_xmlType(ZF_IN ZFXmlTypeEnum xmlType)
{
    if(xmlType != ZFXmlType::e_XmlNull)
    {
        if(d)
        {
            d->xmlType = xmlType;
        }
        else
        {
            d = zfnew(_ZFP_ZFXmlItemPrivate, xmlType);
        }
    }
}
ZFXmlTypeEnum ZFXmlItem::xmlType(void) const
{
    return d ? d->xmlType : ZFXmlType::e_XmlNull;
}

ZFXmlItem ZFXmlItem::xmlParent(void) const
{
    return ZFXmlItem(d ? d->xmlParent : zfnull);
}

ZFXmlItem &ZFXmlItem::xmlName(ZF_IN const zfchar *name)
{
    zfCoreAssert(this->xmlTypeValid());
    d->xmlName.value(name);
    return *this;
}
const zfchar *ZFXmlItem::xmlName(void) const
{
    return d ? d->xmlName.value() : zfnull;
}

ZFXmlItem &ZFXmlItem::xmlValue(ZF_IN const zfchar *value)
{
    zfCoreAssert(this->xmlTypeValid());
    d->xmlValue.value(value);
    return *this;
}
const zfchar *ZFXmlItem::xmlValue(void) const
{
    return d ? d->xmlValue.value() : zfnull;
}

void ZFXmlItem::_ZFP_ZFXml_xmlMemoryPool_xmlName(ZF_IN const zfchar *xmlName, ZF_IN void *token)
{
    zfCoreAssert(this->xmlTypeValid());
    d->xmlName.value(xmlName, token);
}
void ZFXmlItem::_ZFP_ZFXml_xmlMemoryPool_xmlValue(ZF_IN const zfchar *xmlValue, ZF_IN void *token)
{
    zfCoreAssert(this->xmlTypeValid());
    d->xmlValue.value(xmlValue, token);
}

// ============================================================
void ZFXmlItem::xmlVisit(ZF_IN const ZFXmlVisitCallback &callback /* = ZFXmlVisitCallbackForOutput() */) const
{
    if(!callback || this->xmlType() == ZFXmlType::e_XmlNull)
    {
        return ;
    }

    ZFCoreArray<ZFXmlVisitData> datas;
    datas.add(ZFXmlVisitData(*this, ZFXmlVisitType::e_Enter, 0, 0));

    while(datas.count() > 0)
    {
        ZFXmlVisitData data = datas.removeLastAndGet();
        if(data.xmlVisitType == ZFXmlVisitType::e_Enter)
        {
            if(callback.execute(data))
            {
                ZFXmlTypeEnum xmlType = data.xmlItem.xmlType();
                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDocument)
                {
                    if(data.xmlItem.xmlType() == ZFXmlType::e_XmlElement)
                    {
                        datas.add(ZFXmlVisitData(data.xmlItem, ZFXmlVisitType::e_ExitChildren, data.depth, data.siblingIndex));
                    }

                    ZFXmlItem xmlChild = data.xmlItem.childLast();
                    if(xmlChild)
                    {
                        zfindex xmlChildDepth = ((data.xmlItem.xmlType() == ZFXmlType::e_XmlDocument)
                            ? data.depth
                            : data.depth + 1);
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(ZFXmlVisitData(xmlChild, ZFXmlVisitType::e_Enter, xmlChildDepth, 0));
                            xmlChild = xmlChild.siblingPrev();
                        } while(xmlChild);
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }

                datas.add(ZFXmlVisitData(data.xmlItem, ZFXmlVisitType::e_Exit, data.depth, data.siblingIndex));

                if(xmlType == ZFXmlType::e_XmlElement || xmlType == ZFXmlType::e_XmlDeclaration)
                {
                    ZFXmlItem xmlAttribute = data.xmlItem.attrLast();
                    if(xmlAttribute)
                    {
                        zfindex xmlAttributeDepth = data.depth + 1;
                        zfindex startIndex = ((zfindex)datas.count()) - 1;
                        do
                        {
                            datas.add(ZFXmlVisitData(xmlAttribute, ZFXmlVisitType::e_Enter, xmlAttributeDepth, 0));
                            xmlAttribute = xmlAttribute.attrPrev();
                        } while(xmlAttribute);
                        for(zfindex i = ((zfindex)datas.count()) - 1, xmlChildIndex = 0;
                            i != startIndex;
                            --i, ++xmlChildIndex)
                        {
                            datas[i].siblingIndex = xmlChildIndex;
                        }
                    }
                }
            }
        }
        else
        {
            if(!callback.execute(data))
            {
                for(zfindex i = ((zfindex)datas.count()) - 1; i != zfindexMax(); --i)
                {
                    if(datas[i].depth == data.depth)
                    {
                        datas.remove(i);
                    }
                }
            }
        }
    }
}

// ============================================================
ZFXmlItem ZFXmlItem::xmlClone(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyNode(ret, *this);
    }
    return ret;
}
ZFXmlItem ZFXmlItem::xmlCloneTree(void) const
{
    ZFXmlItem ret(this->xmlType());
    if(this->xmlType() != ZFXmlType::e_XmlNull)
    {
        _ZFP_ZFXmlCopyTree(ret, *this);
    }
    return ret;
}

// ============================================================
ZFXmlItem &ZFXmlItem::attrAdd(ZF_IN const ZFXmlItem &addThis,
                              ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveAttribute(*this);
    _ZFP_ZFXmlAssertCanBeAttribute(addThis);
    zfCoreAssertWithMessage(addThis.d->xmlParent == zfnull, "adding a attribute that already has parent, remove it first");

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() == ZFXmlType::e_XmlAttribute)
    {
        d->attrAttach(addThis.d, beforeThis->d);
    }
    else
    {
        d->attrAttach(addThis.d, zfnull);
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::attr(ZF_IN const zfchar *key,
                           ZF_IN const zfchar *value,
                           ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    if(!zfsIsEmpty(key))
    {
        ZFXmlItem xmlAttribute(ZFXmlType::e_XmlAttribute);
        xmlAttribute.xmlName(key);
        xmlAttribute.xmlValue(value);
        this->attrAdd(xmlAttribute, beforeThis);
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::attrRemove(ZF_IN const zfchar *name /* = zfnull */)
{
    if(d == zfnull || d->attrFirst == zfnull || zfsIsEmpty(name))
    {
        return *this;
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->attrFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->siblingNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            d->attrDetach(xmlAttribute);
            break;
        }
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::attrRemove(ZF_IN const ZFXmlItem &removeThis)
{
    if(d == zfnull || removeThis.d == zfnull || removeThis.d->xmlParent != d || removeThis.xmlType() != ZFXmlType::e_XmlAttribute)
    {
        return *this;
    }
    d->attrDetach(removeThis.d);
    return *this;
}
ZFXmlItem &ZFXmlItem::attrRemoveAll(void)
{
    if(d)
    {
        d->attrRemoveAll();
    }
    return *this;
}

ZFXmlItem ZFXmlItem::attr(ZF_IN const zfchar *name) const
{
    if(d == zfnull || d->attrFirst == zfnull || zfsIsEmpty(name))
    {
        return ZFXmlItem();
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->attrFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->siblingNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            return ZFXmlItem(xmlAttribute);
        }
    }
    return ZFXmlItem();
}
const zfchar *ZFXmlItem::attrValue(ZF_IN const zfchar *name) const
{
    if(d == zfnull || d->attrFirst == zfnull || zfsIsEmpty(name))
    {
        return zfnull;
    }

    for(_ZFP_ZFXmlItemPrivate *xmlAttribute = d->attrFirst; xmlAttribute != zfnull; xmlAttribute = xmlAttribute->siblingNext)
    {
        if(zfscmpTheSame(xmlAttribute->xmlName.value(), name))
        {
            return xmlAttribute->xmlValue.value();
        }
    }
    return zfnull;
}
ZFXmlItem ZFXmlItem::attrFirst(void) const
{
    return ZFXmlItem(d ? d->attrFirst : zfnull);
}
ZFXmlItem ZFXmlItem::attrLast(void) const
{
    return ZFXmlItem(d ? d->attrLast : zfnull);
}

ZFXmlItem ZFXmlItem::attrNext(void) const
{
    return ZFXmlItem((d && d->xmlType == ZFXmlType::e_XmlAttribute) ? d->siblingNext : zfnull);
}
ZFXmlItem ZFXmlItem::attrPrev(void) const
{
    return ZFXmlItem((d && d->xmlType == ZFXmlType::e_XmlAttribute) ? d->siblingPrev : zfnull);
}

// ============================================================
ZFXmlItem &ZFXmlItem::attrSort(void)
{
    if(d)
    {
        d->attrSort();
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::attrSortRecursively(void)
{
    if(d)
    {
        d->attrSortRecursively();
    }
    return *this;
}

// ============================================================
ZFXmlItem &ZFXmlItem::childAdd(ZF_IN const ZFXmlItem &addThis,
                               ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */)
{
    _ZFP_ZFXmlAssertCanHaveChild(*this);
    switch(addThis.xmlType())
    {
        case ZFXmlType::e_XmlNull:
        case ZFXmlType::e_XmlAttribute:
        case ZFXmlType::e_XmlDocument:
            zfCoreCriticalMessage("%s can not be added to %s"
                    , ZFXmlTypeToString(addThis.xmlType()).cString()
                    , ZFXmlTypeToString(this->xmlType()).cString()
                );
            break;
        case ZFXmlType::e_XmlDeclaration:
        case ZFXmlType::e_XmlDocType:
        case ZFXmlType::e_XmlPI:
            if(this->xmlType() != ZFXmlType::e_XmlDocument)
            {
                zfCoreCriticalMessage("%s can not be added to %s"
                        , ZFXmlTypeToString(addThis.xmlType()).cString()
                        , ZFXmlTypeToString(this->xmlType()).cString()
                    );
            }
            break;
        default:
            break;
    }
    zfCoreAssertWithMessage(addThis.d->xmlParent == zfnull, "adding a child that already has parent, remove it first");

    if(beforeThis != zfnull && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
    {
        d->childAttach(addThis.d, beforeThis->d);
    }
    else
    {
        d->childAttach(addThis.d, zfnull);
    }
    return *this;
}
ZFXmlItem &ZFXmlItem::childRemove(ZF_IN const ZFXmlItem &removeThis)
{
    if(d == zfnull || removeThis.d == zfnull || removeThis.d->xmlParent != d || removeThis.xmlType() == ZFXmlType::e_XmlAttribute)
    {
        return *this;
    }
    d->childDetach(removeThis.d);
    return *this;
}
ZFXmlItem &ZFXmlItem::childRemoveAll(void)
{
    if(d)
    {
        d->childRemoveAll();
    }
    return *this;
}

ZFXmlItem ZFXmlItem::childFirst(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                ZF_IN_OPT const ZFXmlItem *afterThis /* = zfnull */) const
{
    if(d == zfnull)
    {
        return ZFXmlItem();
    }
    _ZFP_ZFXmlItemPrivate *xmlChild = (afterThis != zfnull && afterThis->d && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? afterThis->d->siblingNext
        : d->childFirst;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->siblingNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::childLast(ZF_IN_OPT const zfchar *name /* = zfnull */,
                               ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */) const
{
    if(d == zfnull)
    {
        return ZFXmlItem();
    }
    _ZFP_ZFXmlItemPrivate *xmlChild = (beforeThis != zfnull && beforeThis->d && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? beforeThis->d->siblingPrev
        : d->childLast;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->siblingPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::childFirstElement(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                       ZF_IN_OPT const ZFXmlItem *afterThis /* = zfnull */) const
{
    if(d == zfnull)
    {
        return ZFXmlItem();
    }
    _ZFP_ZFXmlItemPrivate *xmlChild = (afterThis != zfnull && afterThis->d && afterThis->d->xmlParent == d && afterThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? afterThis->d->siblingNext
        : d->childFirst;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->siblingNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::childLastElement(ZF_IN_OPT const zfchar *name /* = zfnull */,
                                      ZF_IN_OPT const ZFXmlItem *beforeThis /* = zfnull */) const
{
    if(d == zfnull)
    {
        return ZFXmlItem();
    }
    _ZFP_ZFXmlItemPrivate *xmlChild = (beforeThis != zfnull && beforeThis->d && beforeThis->d->xmlParent == d && beforeThis->xmlType() != ZFXmlType::e_XmlAttribute)
        ? beforeThis->d->siblingPrev
        : d->childLast;

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(xmlChild);
    }
    else
    {
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->siblingPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}

ZFXmlItem ZFXmlItem::siblingNext(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d == zfnull || d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->siblingNext);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->siblingNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->siblingNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::siblingPrev(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d == zfnull || d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->siblingPrev);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->siblingPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name))
        {
            xmlChild = xmlChild->siblingPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::siblingNextElement(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d == zfnull || d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->siblingNext);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->siblingNext;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->siblingNext;
        }
        return ZFXmlItem(xmlChild);
    }
}
ZFXmlItem ZFXmlItem::siblingPrevElement(ZF_IN const zfchar *name /* = zfnull */) const
{
    if(d == zfnull || d->xmlType == ZFXmlType::e_XmlAttribute)
    {
        return ZFXmlItem();
    }

    if(zfsIsEmpty(name))
    {
        return ZFXmlItem(d->siblingPrev);
    }
    else
    {
        _ZFP_ZFXmlItemPrivate *xmlChild = d->siblingPrev;
        while(xmlChild != zfnull && !zfscmpTheSame(xmlChild->xmlName.value(), name) && xmlChild->xmlType != ZFXmlType::e_XmlElement)
        {
            xmlChild = xmlChild->siblingPrev;
        }
        return ZFXmlItem(xmlChild);
    }
}

// ============================================================
void ZFXmlItem::xmlTextCDATA(ZF_IN zfbool xmlTextCDATA)
{
    zfCoreAssert(this->xmlType() == ZFXmlType::e_XmlText);
    d->xmlTextCDATA = xmlTextCDATA;
}
zfbool ZFXmlItem::xmlTextCDATA(void) const
{
    return d ? d->xmlTextCDATA : zffalse;
}

// ============================================================
ZFXmlItem::operator zfstring (void) const
{
    return ZFXmlItemToString(*this);
}

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFXmlItem, ZFXmlItem, {
        v = ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, srcLen);
        return v;
    }, {
        return ZFXmlItemToString(s, v, ZFXmlOutputFlagsTrim());
    })

ZFOBJECT_ON_INIT_USER_REGISTER_1({
        invokerObject->to<v_ZFXmlItem *>()->zfv._ZFP_ZFXmlItem_xmlType(xmlType);
    }, v_ZFXmlItem
    , ZFMP_IN(ZFXmlTypeEnum, xmlType)
    )

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, objectInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfstring, objectInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfindex, objectRetainCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlTypeEnum, xmlType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfbool, xmlTypeValid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlParent)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem &, xmlName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, const zfchar *, xmlName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem &, xmlValue, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, const zfchar *, xmlValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlVisit, ZFMP_IN(const ZFXmlVisitCallback &, callback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlClone)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, xmlCloneTree)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem &, attrAdd, ZFMP_IN(const ZFXmlItem &, addThis), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFXmlItem, ZFXmlItem &, attr, ZFMP_IN(const zfchar *, key), ZFMP_IN(const zfchar *, value), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem &, attrRemove, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem &, attrRemove, ZFMP_IN(const ZFXmlItem &, removeThis))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem &, attrRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, attr, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, const zfchar *, attrValue, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, attrFirst)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, attrLast)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, attrNext)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem, attrPrev)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem &, attrSort)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem &, attrSortRecursively)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem &, childAdd, ZFMP_IN(const ZFXmlItem &, addThis), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem &, childRemove, ZFMP_IN(const ZFXmlItem &, removeThis))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, ZFXmlItem &, childRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, childFirst, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, afterThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, childLast, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, childFirstElement, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, afterThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFXmlItem, ZFXmlItem, childLastElement, ZFMP_IN_OPT(const zfchar *, name, zfnull), ZFMP_IN_OPT(const ZFXmlItem *, beforeThis, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, siblingNext, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, siblingPrev, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, siblingNextElement, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, ZFXmlItem, siblingPrevElement, ZFMP_IN_OPT(const zfchar *, name, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFXmlItem, void, xmlTextCDATA, ZFMP_IN(zfbool, xmlTextCDATA))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFXmlItem, zfbool, xmlTextCDATA)

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlElement,
                       ZFMP_IN_OPT(const zfchar *, xmlName, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlElement).xmlName(xmlName);}
ZFMETHOD_FUNC_DEFINE_2(ZFXmlItem, ZFXmlAttribute,
                       ZFMP_IN_OPT(const zfchar *, xmlName, zfnull),
                       ZFMP_IN_OPT(const zfchar *, xmlValue, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlAttribute).xmlName(xmlName).xmlValue(xmlValue);}
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlText,
                       ZFMP_IN_OPT(const zfchar *, xmlValue, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlText).xmlValue(xmlValue);}
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlComment,
                       ZFMP_IN_OPT(const zfchar *, xmlValue, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlComment).xmlValue(xmlValue);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlDocument) {return ZFXmlItem(ZFXmlType::e_XmlDocument);}
ZFMETHOD_FUNC_DEFINE_0(ZFXmlItem, ZFXmlDeclaration) {return ZFXmlItem(ZFXmlType::e_XmlDeclaration);}
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlDocType,
                       ZFMP_IN_OPT(const zfchar *, xmlValue, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlDocType).xmlValue(xmlValue);}
ZFMETHOD_FUNC_DEFINE_2(ZFXmlItem, ZFXmlPI,
                       ZFMP_IN_OPT(const zfchar *, xmlName, zfnull),
                       ZFMP_IN_OPT(const zfchar *, xmlValue, zfnull))
{return ZFXmlItem(ZFXmlType::e_XmlPI).xmlName(xmlName).xmlValue(xmlValue);}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFXmlItem, ZFXmlItemFromInput,
                       ZFMP_IN(const ZFInput &, callback))
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(callback);
}
ZFMETHOD_FUNC_DEFINE_2(ZFXmlItem, ZFXmlItemFromString,
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, size, zfindexMax()))
{
    return ZFPROTOCOL_ACCESS(ZFXml)->xmlParse(src, size);
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlItemToOutput,
                       ZFMP_IN_OUT(const ZFOutput &, output),
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, outputFlags, ZFXmlOutputFlagsDefault()))
{
    if(!output || !xmlItem)
    {
        return zffalse;
    }
    xmlItem.xmlVisit(ZFXmlVisitCallbackForOutput(output, outputFlags));
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFXmlItemToString,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN(const ZFXmlOutputFlags &, outputFlags))
{
    return ZFXmlItemToOutput(ZFOutputForString(ret), xmlItem, outputFlags);
}
ZFMETHOD_FUNC_DEFINE_2(zfstring, ZFXmlItemToString,
                       ZFMP_IN(const ZFXmlItem &, xmlItem),
                       ZFMP_IN(const ZFXmlOutputFlags &, outputFlags))
{
    zfstring ret;
    ZFXmlItemToString(ret, xmlItem, outputFlags);
    return ret;
}

// ============================================================
// escape chars
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFXmlEscapeCharEncode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharEncode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharEncode(dst, src, count);
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode,
                       ZFMP_OUT(zfstring &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFXmlEscapeCharDecode(ZFOutputForString(dst), src, count);
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFXmlEscapeCharDecode,
                       ZFMP_OUT(const ZFOutput &, dst),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    ZFPROTOCOL_ACCESS(ZFXml)->xmlEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END


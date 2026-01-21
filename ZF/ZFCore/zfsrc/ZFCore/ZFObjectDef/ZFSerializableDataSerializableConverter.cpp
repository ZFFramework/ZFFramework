#include "ZFSerializableDataSerializableConverter.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_USER_REGISTER_FOR_FUNC(ZFSDOutputToken, ZFSDOutputTokenDefault)
ZFEXPORT_VAR_USER_REGISTER_FOR_FUNC(ZFSDOutputToken, ZFSDOutputTokenTrim)
ZFEXPORT_VAR_USER_REGISTER_FOR_FUNC(ZFSDOutputToken, ZFSDOutputTokenDetail)

ZFSDOutputToken &ZFSDOutputTokenDefault(void) {
    static ZFSDOutputToken d = ZFSDOutputTokenTrim();
    return d;
}
void ZFSDOutputTokenDefault(ZF_IN const ZFSDOutputToken &v) {
    ZFSDOutputTokenDefault() = v;
}

static ZFSDOutputToken _ZFP_ZFSDOutputTokenTrim(void) {
    ZFSDOutputToken d;
    d.prettyPrint = zffalse;
    return d;
}
ZFSDOutputToken &ZFSDOutputTokenTrim(void) {
    static ZFSDOutputToken d = _ZFP_ZFSDOutputTokenTrim();
    return d;
}
void ZFSDOutputTokenTrim(ZF_IN const ZFSDOutputToken &v) {
    ZFSDOutputTokenTrim() = v;
}

ZFSDOutputToken &ZFSDOutputTokenDetail(void) {
    static ZFSDOutputToken d;
    return d;
}
void ZFSDOutputTokenDetail(ZF_IN const ZFSDOutputToken &v) {
    ZFSDOutputTokenDetail() = v;
}

// ============================================================
/*
 * // '-' for empty ClassName
 *
 * <ClassNameEncoded AttrKey1="AttrValue1" AttrKey2="AttrValue2" [
 *     <ChildElement0>
 *     <ChildElement1 AttrKey3="">
 *     <ChildElement2 []>
 * ]>
 */
#define _ZFP_ZFSD_NullClass '-'
#define _ZFP_ZFSD_IsSpace(c) (zfcharIsSpace(c) || zfcharIsEndl(c))
#define _ZFP_ZFSD_Space ' '
#define _ZFP_ZFSD_ObjBegin '<'
#define _ZFP_ZFSD_ObjEnd '>'
#define _ZFP_ZFSD_AttrAssign '='
#define _ZFP_ZFSD_AttrValuePair '"'
#define _ZFP_ZFSD_AttrValuePairEscape '\\'
#define _ZFP_ZFSD_ChildBegin '['
#define _ZFP_ZFSD_ChildEnd ']'

// ============================================================
// escape char map for serializable data's class name and attribute key name
// everything would be escaped except:
//   0~9
//   a-z
//   A-Z
//   @_
#define _ZFP_ZFSerializableEscapeCharMap() const zfchar charMap[256] = { \
        /* 0x00 ~ 0x0F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x10 ~ 0x1F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x20 ~ 0x2F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x30 ~ 0x3F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, \
        /* 0x40 ~ 0x4F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x50 ~ 0x5F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, \
        /* 0x60 ~ 0x6F */ \
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
        /* 0x70 ~ 0x7F */ \
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, \
        /* 0x80 ~ 0x8F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0x90 ~ 0x9F */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xA0 ~ 0xAF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xB0 ~ 0xBF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xC0 ~ 0xCF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xD0 ~ 0xDF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xE0 ~ 0xEF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* 0xF0 ~ 0xFF */ \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    }

// ============================================================
zfbool ZFSerializableDataFromZFSD(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!input) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "invalid input callback");
        return zffalse;
    }
    zfstring buf;
    if(ZFInputRead(buf, input) == zfindexMax() || buf.isEmpty()) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "unable to load data from input");
        return zffalse;
    }
    zfbool ret = ZFSerializableDataFromZFSD(serializableData, buf.cString(), buf.length(), outErrorHint);
    if(ret) {
        serializableData.pathInfo(input.pathInfo());
    }
    return ret;
}
ZFSerializableData ZFSerializableDataFromZFSD(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromZFSD(ret, input, outErrorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}
zfbool ZFSerializableDataToZFSD(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT const ZFSDOutputToken &token /* = ZFSDOutputTokenDefault() */
        ) {
    if(!output) {
        ZFSerializableUtilErrorOccurred(outErrorHint, "invalid output callback");
        return zffalse;
    }
    zfstring tmp;
    if(!ZFSerializableDataToZFSD(tmp, serializableData, outErrorHint, token)) {
        return zffalse;
    }
    tmp += "\n";
    output.execute(tmp.cString(), tmp.length());
    return zftrue;
}

// ============================================================
static zfbool _ZFP_ZFSD_AttrValueDecode(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OUT const zfchar *&encodedData
        , ZF_IN const zfchar *srcEnd
        ) {
    if(*encodedData != _ZFP_ZFSD_AttrValuePair) {
        return zffalse;
    }
    ++encodedData;
    const zfchar *pLeft = encodedData;
    while(encodedData < srcEnd && *encodedData != _ZFP_ZFSD_AttrValuePair) {
        if(*encodedData == _ZFP_ZFSD_AttrValuePairEscape && encodedData + 1 < srcEnd) {
            ++encodedData;
            if(*encodedData == _ZFP_ZFSD_AttrValuePair) {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += _ZFP_ZFSD_AttrValuePair;
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == _ZFP_ZFSD_AttrValuePairEscape) {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += _ZFP_ZFSD_AttrValuePairEscape;
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 'r') {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\r';
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 'n') {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\n';
                ++encodedData;
                pLeft = encodedData;
            }
            else if(*encodedData == 't') {
                ret.append(pLeft, encodedData - 1 - pLeft);
                ret += '\t';
                ++encodedData;
                pLeft = encodedData;
            }
        }
        else {
            zfcharMoveNext(encodedData);
        }
    }
    ret.append(pLeft, encodedData - pLeft);

    if(*encodedData == _ZFP_ZFSD_AttrValuePair) {
        ++encodedData;
        return zftrue;
    }
    else {
        return zffalse;
    }
}
static void _ZFP_ZFSD_AttrValueEncode(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *value
        ) {
    ret += _ZFP_ZFSD_AttrValuePair;
    const zfchar *pL = value;
    while(*value != '\0') {
        if(*value == _ZFP_ZFSD_AttrValuePair) {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += _ZFP_ZFSD_AttrValuePair;
        }
        else if(*value == _ZFP_ZFSD_AttrValuePairEscape) {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
        }
        else if(*value == '\r') {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 'r';
        }
        else if(*value == '\n') {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 'n';
        }
        else if(*value == '\t') {
            ret.append(pL, value - pL);
            ++value;
            pL = value;
            ret += _ZFP_ZFSD_AttrValuePairEscape;
            ret += 't';
        }
        else {
            zfcharMoveNext(value);
        }
    }
    ret.append(pL, value - pL);
    ret += _ZFP_ZFSD_AttrValuePair;
}
zfbool _ZFP_ZFSerializableDataFromZFSD(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN_OUT const zfchar *&encodedData
        , ZF_IN zfindex encodedDataLen
        , ZF_OUT zfstring *outErrorHint
        , ZF_IN_OPT zfbool validateTail = zffalse
        ) {
    if(encodedData == zfnull) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "invalid param");
        return zffalse;
    }
    const zfchar *srcEnd = (encodedData + ((encodedDataLen == zfindexMax()) ? zfslen(encodedData) : encodedDataLen));
    zfbool ret = zffalse;
    do {
        const zfchar *pLeft = zfnull;
        const zfchar *pRight = zfnull;
        zfstring decodedTmp;

        // begin
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd || *encodedData != _ZFP_ZFSD_ObjBegin) {break;}
        ++encodedData;

        // class name
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        pLeft = encodedData;
        while(encodedData < srcEnd
                && !_ZFP_ZFSD_IsSpace(*encodedData)
                && *encodedData != _ZFP_ZFSD_ChildBegin
                && *encodedData != _ZFP_ZFSD_ObjEnd
                ) {
            zfcharMoveNext(encodedData);
        }
        pRight = encodedData;
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd) {break;}

        if(pRight == pLeft + 1 && *pLeft == _ZFP_ZFSD_NullClass) {
            serializableData.itemClass(zfnull);
        }
        else {
            ZFCoreDataDecode(decodedTmp, pLeft, pRight - pLeft);
            serializableData.itemClass(decodedTmp);
            decodedTmp.removeAll();
        }

        // attributes
        if(*encodedData != _ZFP_ZFSD_ChildBegin && *encodedData != _ZFP_ZFSD_ObjEnd) {
            while(encodedData < srcEnd) {
                if(*encodedData == _ZFP_ZFSD_ChildBegin || *encodedData == _ZFP_ZFSD_ObjEnd) {
                    ret = zftrue;
                    break;
                }

                // name
                pLeft = encodedData;
                while(encodedData < srcEnd
                        && !_ZFP_ZFSD_IsSpace(*encodedData)
                        && *encodedData != _ZFP_ZFSD_AttrAssign
                        ) {
                    zfcharMoveNext(encodedData);
                }
                if(encodedData >= srcEnd) {break;}
                pRight = encodedData;
                if(*encodedData != _ZFP_ZFSD_AttrAssign) {
                    zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                }
                if(*encodedData != _ZFP_ZFSD_AttrAssign) {break;}
                ++encodedData;
                zfcharSkipSpaceAndEndl(encodedData, srcEnd);

                zfstring attrName;
                ZFCoreDataDecode(attrName, pLeft, pRight - pLeft);

                // value
                if(!_ZFP_ZFSD_AttrValueDecode(decodedTmp, encodedData, srcEnd)) {break;}

                // save
                if(!attrName.isEmpty() && !decodedTmp.isEmpty()) {
                    serializableData.attr(attrName, decodedTmp);
                }
                decodedTmp.removeAll();

                zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            }
            if(!ret) {break;}
            ret = zffalse;
        } // if(*encodedData == _ZFP_ZFSD_AttrBegin)

        // elements
        if(*encodedData == _ZFP_ZFSD_ChildBegin) {
            ++encodedData;
            while(encodedData < srcEnd) {
                zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                if(*encodedData == _ZFP_ZFSD_ChildEnd) {
                    ++encodedData;
                    zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                    ret = zftrue;
                    break;
                }

                ZFSerializableData element;
                if(!_ZFP_ZFSerializableDataFromZFSD(element, encodedData, srcEnd - encodedData, outErrorHint)) {
                    return zffalse;
                }
                serializableData.child(element);
            }
            if(!ret) {break;}
            ret = zffalse;
        }

        // tail
        if(encodedData >= srcEnd || *encodedData != _ZFP_ZFSD_ObjEnd) {break;}
        ++encodedData;
        if(validateTail) {
            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            if(encodedData < srcEnd) {break;}
        }

        ret = zftrue;
    } while(zffalse);
    if(!ret) {
        ZFSerializableUtilErrorOccurred(outErrorHint,
            "wrong serializable string format at position: \"%s\"",
            zfstring(encodedData, srcEnd - encodedData));
    }
    return ret;
}

// ============================================================
zfbool ZFSerializableDataFromZFSD(
        ZF_OUT ZFSerializableData &serializableData
        , ZF_IN const zfchar *encodedData
        , ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    return _ZFP_ZFSerializableDataFromZFSD(serializableData, encodedData, encodedDataLen, outErrorHint, ZF_HINT("validateTail")zftrue);
}
ZFSerializableData ZFSerializableDataFromZFSD(
        ZF_IN const zfchar *encodedData
        , ZF_IN zfindex encodedDataLen
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    ZFSerializableData ret;
    if(ZFSerializableDataFromZFSD(ret, encodedData, encodedDataLen, outErrorHint)) {
        return ret;
    }
    else {
        return ZFSerializableData();
    }
}

static zfbool _ZFP_ZFSerializableDataToZFSDPretty(
        ZF_OUT zfstring &result
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT zfstring *outErrorHint
        , ZF_IN zfindex indentLevel
        );
zfbool ZFSerializableDataToZFSD(
        ZF_OUT zfstring &result
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT const ZFSDOutputToken &token /* = ZFSDOutputTokenDefault() */
        ) {
    if(token.prettyPrint) {
        return _ZFP_ZFSerializableDataToZFSDPretty(result, serializableData, outErrorHint, 0);
    }

    _ZFP_ZFSerializableEscapeCharMap();

    result += _ZFP_ZFSD_ObjBegin;

    // serializable class
    if(serializableData.itemClass() == zfnull) {
        result += _ZFP_ZFSD_NullClass;
    }
    else {
        ZFCoreDataEncode(result, serializableData.itemClass(), zfindexMax(), charMap);
    }

    // attributes
    if(serializableData.attrCount() > 0) {
        for(zfiter it = serializableData.attrIter(); it; ++it) {
            result += _ZFP_ZFSD_Space;
            ZFCoreDataEncode(result, serializableData.attrIterKey(it), zfindexMax(), charMap);
            result += _ZFP_ZFSD_AttrAssign;
            _ZFP_ZFSD_AttrValueEncode(result, serializableData.attrIterValue(it));
        }
    }

    // elements
    if(serializableData.childCount() > 0) {
        result += _ZFP_ZFSD_ChildBegin;
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            if(!ZFSerializableDataToZFSD(result, serializableData.childAt(i), outErrorHint, token)) {
                return zffalse;
            }
        }
        result += _ZFP_ZFSD_ChildEnd;
    }

    result += _ZFP_ZFSD_ObjEnd;
    return zftrue;
}
zfstring ZFSerializableDataToZFSD(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT const ZFSDOutputToken &token /* = ZFSDOutputTokenDefault() */
        ) {
    zfstring tmp;
    ZFSerializableDataToZFSD(tmp, serializableData, outErrorHint, token);
    return tmp;
}

// ============================================================
static zfbool _ZFP_ZFSerializableDataToZFSDPretty(
        ZF_OUT zfstring &result
        , ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT zfstring *outErrorHint
        , ZF_IN zfindex indentLevel
        ) {
    _ZFP_ZFSerializableEscapeCharMap();

    zfstringRepeatT(result, "    ", indentLevel);
    result += _ZFP_ZFSD_ObjBegin;

    // serializable class
    if(serializableData.itemClass() == zfnull) {
        result += _ZFP_ZFSD_NullClass;
    }
    else {
        ZFCoreDataEncode(result, serializableData.itemClass(), zfindexMax(), charMap);
    }

    zfbool needBreak = (serializableData.attrCount() > 3);

    // attributes
    if(serializableData.attrCount() > 0) {
        for(zfiter it = serializableData.attrIter(); it; ++it) {
            if(needBreak) {
                result += '\n';
                zfstringRepeatT(result, "    ", indentLevel + 1);
            }
            else {
                result += _ZFP_ZFSD_Space;
            }
            ZFCoreDataEncode(result, serializableData.attrIterKey(it), zfindexMax(), charMap);
            result += _ZFP_ZFSD_AttrAssign;
            _ZFP_ZFSD_AttrValueEncode(result, serializableData.attrIterValue(it));
        }
    }

    // elements
    if(serializableData.childCount() > 0) {
        if(needBreak) {
            result += '\n';
            zfstringRepeatT(result, "    ", indentLevel + 1);
        }
        else {
            result += ' ';
        }
        result += _ZFP_ZFSD_ChildBegin;
        result += '\n';
        for(zfindex i = 0; i < serializableData.childCount(); ++i) {
            if(!_ZFP_ZFSerializableDataToZFSDPretty(result, serializableData.childAt(i), outErrorHint, indentLevel + 1)) {
                return zffalse;
            }
            result += '\n';
        }
        zfstringRepeatT(result, "    ", indentLevel);
        result += _ZFP_ZFSD_ChildEnd;
    }

    if(needBreak) {
        result += ' ';
    }
    result += _ZFP_ZFSD_ObjEnd;
    return zftrue;
}

zfbool ZFObjectFromZFSDT(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    ZFSerializableData data;
    if(ZFSerializableDataFromZFSD(data, input, outErrorHint)) {
        return ZFObjectFromDataT(ret, data, outErrorHint);
    }
    else {
        return zffalse;
    }
}
zfauto ZFObjectFromZFSD(
        ZF_IN const ZFInput &input
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    zfauto ret;
    ZFObjectFromZFSDT(ret, input, outErrorHint);
    return ret;
}

zfbool ZFObjectToZFSD(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN ZFObject *obj
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT const ZFSDOutputToken &token /* = ZFSDOutputTokenDefault() */
        ) {
    ZFSerializableData serializableData;
    if(!ZFObjectToDataT(serializableData, obj, outErrorHint)) {
        return zffalse;
    }
    else {
        return ZFSerializableDataToZFSD(output, serializableData, outErrorHint, token);
    }
}

// ============================================================
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataFromZFSD
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFSerializableData, ZFSerializableDataFromZFSD
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataToZFSD
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFSerializableDataFromZFSD
        , ZFMP_OUT(ZFSerializableData &, serializableData)
        , ZFMP_IN(const zfchar *, encodedData)
        , ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax())
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFSerializableData, ZFSerializableDataFromZFSD
        , ZFMP_IN(const zfchar *, encodedData)
        , ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax())
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFSerializableDataToZFSD
        , ZFMP_OUT(zfstring &, result)
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFSerializableDataToZFSD
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectFromZFSDT
        , ZFMP_OUT(zfauto &, ret)
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfauto, ZFObjectFromZFSD
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectToZFSD
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        , ZFMP_IN_OPT(const ZFSDOutputToken &, token, ZFSDOutputTokenDefault())
        )

ZF_NAMESPACE_GLOBAL_END


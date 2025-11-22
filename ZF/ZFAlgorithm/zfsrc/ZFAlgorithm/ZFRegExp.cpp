#include "ZFRegExp.h"
#include "protocol/ZFProtocolZFRegExp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFRegExpResult::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->matched) {
        ret += "matched";
        if(this->matchedRange != ZFIndexRangeMax()) {
            ret += " in ";
            ZFIndexRangeToStringT(ret, this->matchedRange);
        }
        if(!this->namedGroups.isEmpty()) {
            ret += ", named groups: ";
            for(zfindex i = 0; i < this->namedGroups.count(); ++i) {
                if(i != 0) {
                    ret += ", ";
                }
                ZFIndexRangeToStringT(ret, this->namedGroups[i]);
            }
        }
    }
    else {
        ret += "no match";
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFRegExpResult, ZFRegExpResult, {
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFRegExpResult(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        ZFSerializableData element;

        v.matched = zffalse;
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFRegExpResult_matched, zfbool, v.matched, {
                    return zffalse;
                });

        v.matchedRange = ZFIndexRangeMax();
        ZFSerializableUtilSerializeCategoryFromData(serializableData, outErrorHint, outErrorPos,
                check, ZFSerializableKeyword_ZFRegExpResult_matchedRange, ZFIndexRange, v.matchedRange, {
                    return zffalse;
                });

        v.namedGroups.removeAll();
        element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFRegExpResult_namedGroups);
        if(element != zfnull && !ZFCoreArrayFromDataT(
                    v.namedGroups,
                    serializableData,
                    outErrorHint,
                    outErrorPos
                    )) {
            return zffalse;
        }
        return zftrue;
    }, {
        serializableData.itemClass(ZFTypeId_ZFRegExpResult());

        ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFRegExpResult_matched, zfbool, v.matched, zffalse, {
                    return zffalse;
                });

        ZFSerializableUtilSerializeCategoryToDataNoRef(serializableData, outErrorHint,
                ZFSerializableKeyword_ZFRegExpResult_matchedRange, ZFIndexRange, v.matchedRange, ZFIndexRangeMax(), {
                    return zffalse;
                });

        if(!v.namedGroups.isEmpty()) {
            ZFSerializableData element;
            if(!ZFCoreArrayToDataT(element, v.namedGroups, outErrorHint)) {
                return zffalse;
            }
            element.category(ZFSerializableKeyword_ZFRegExpResult_namedGroups);
            serializableData.child(element);
        }

        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, zfbool, matched)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, ZFIndexRange, matchedRange)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFRegExpResult, ZFCoreArray<ZFIndexRange>, namedGroups)

// ============================================================
// global
ZFENUM_DEFINE_FLAGS(ZFRegExpOption, ZFRegExpOptionFlags)

// ============================================================
// _ZFP_ZFRegExpPrivate
zfclassNotPOD _ZFP_ZFRegExpPrivate {
public:
    void *nativeRegExp;
    zfstring pattern;
    ZFRegExpOptionFlags flag;

public:
    _ZFP_ZFRegExpPrivate(void)
    : nativeRegExp(zfnull)
    , pattern()
    , flag()
    {
    }
};

// ============================================================
// ZFRegExp
ZFOBJECT_REGISTER(ZFRegExp)

zfbool ZFRegExp::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    const zfchar *pattern = zfnull;
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFRegExp_pattern, zfstring, pattern, {
                return zffalse;
            });

    ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault();
    ZFSerializableUtilSerializeAttrFromData(serializableData, outErrorHint, outErrorPos,
            check, ZFSerializableKeyword_ZFRegExp_flag, ZFRegExpOptionFlags, flag, {
                return zffalse;
            });

    this->pattern(pattern ? pattern : "", flag);

    return zftrue;
}
zfbool ZFRegExp::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref,
            ZFSerializableKeyword_ZFRegExp_pattern, zfstring, this->pattern(), ref->pattern(), "", {
                return zffalse;
            });

    ZFSerializableUtilSerializeAttrToData(serializableData, outErrorHint, ref,
            ZFSerializableKeyword_ZFRegExp_flag, ZFRegExpOptionFlags, this->options(), ref->options(), ZFRegExpOptionFlags::EnumDefault(), {
                return zffalse;
            });

    return zftrue;
}

zfbool ZFRegExp::serializableOnSerializeFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    this->pattern(srcLen == zfindexMax()
            ? src : zfstring(src, srcLen).cString()
            );
    return zftrue;
}
zfbool ZFRegExp::serializableOnSerializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    if(zffalse
            || ZFBitTest(this->options(), v_ZFRegExpOption::e_IgnoreCase)
            ) {
        zfstringAppend(errorHint, "only regexp with default options can be serialized to string, got: %s", this->options());
        return zffalse;
    }
    ret += this->pattern();
    return zftrue;
}

ZFOBJECT_ON_INIT_DEFINE_2(ZFRegExp
        , ZFMP_IN(const zfstring &, pattern)
        , ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault())
        ) {
    zfself::pattern(pattern, flag);
}
void ZFRegExp::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFRegExpPrivate);
    d->nativeRegExp = ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpCreate(this);
}
void ZFRegExp::objectOnDealloc(void) {
    ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpDestroy(this);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFRegExp::objectInfoImplAppend(ZF_IN_OUT zfstring &ret) {
    zfsuper::objectInfoImplAppend(ret);
    ZFObjectPropertyInfoT(ret, this);
}

zfidentity ZFRegExp::objectHashImpl(void) {
    ZFRegExpOptionFlags flag = this->options();
    return zfidentityHash(
            zfidentityCalcString(this->pattern())
            , zfidentityCalc(flag)
            );
}
ZFCompareResult ZFRegExp::objectCompareImpl(ZF_IN ZFObject *anotherObj) {
    if(this == anotherObj) {return ZFCompareEqual;}
    zfself *another = zfcast(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zfstringIsEqual(this->pattern(), another->pattern())
            && this->options() == another->options()
            ) {
        return ZFCompareEqual;
    }
    return ZFCompareUncomparable;
}

ZFMETHOD_DEFINE_0(ZFRegExp, void *, nativeRegExp) {
    return d->nativeRegExp;
}

ZFMETHOD_DEFINE_0(ZFRegExp, const zfstring &, pattern) {
    return d->pattern;
}
ZFMETHOD_DEFINE_0(ZFRegExp, ZFRegExpOptionFlags, options) {
    return d->flag;
}
ZFMETHOD_DEFINE_1(ZFRegExp, zfindex, namedGroupIndexForName
        , ZFMP_IN(const zfstring &, name)
        ) {
    if(name == zfnull) {
        return zfindexMax();
    }
    return ZFPROTOCOL_ACCESS(ZFRegExp)->namedGroupIndexForName(this, name);
}

ZFMETHOD_DEFINE_2(ZFRegExp, void, pattern
        , ZFMP_IN(const zfstring &, pattern)
        , ZFMP_IN_OPT(ZFRegExpOptionFlags, flag, ZFRegExpOptionFlags::EnumDefault())
        ) {
    d->pattern = pattern;
    d->flag = flag;
    ZFPROTOCOL_ACCESS(ZFRegExp)->pattern(this, pattern, flag);
}
ZFMETHOD_DEFINE_3(ZFRegExp, void, find
        , ZFMP_OUT(ZFRegExpResult &, result)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLength, zfindexMax())
        ) {
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->find(this, result, src, srcLength);
}
ZFMETHOD_DEFINE_6(ZFRegExp, void, replace
        , ZFMP_OUT(zfstring &, ret)
        , ZFMP_OUT(ZFRegExpResult &, result)
        , ZFMP_IN(const zfstring &, replacePattern)
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLength, zfindexMax())
        , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
        ) {
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->replace(this, ret, result, replacePattern, src, srcLength, maxReplaceCount);
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFRegExpMatch
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, pattern)
        ) {
    zfindex srcLen = zfslen(src);
    ZFRegExpResult result;
    if(pattern) {
        pattern->find(result, src, srcLen);
    }
    return result.matchedRange.start == 0 && result.matchedRange.count == srcLen;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFRegExpMatch
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, pattern)
        ) {
    return ZFRegExpMatch(src, zfobj<ZFRegExp>(pattern));
}

ZFMETHOD_FUNC_DEFINE_2(ZFIndexRange, ZFRegExpFind
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, pattern)
        ) {
    ZFRegExpResult result;
    if(pattern) {
        pattern->find(result, src);
    }
    return result.matchedRange;
}
ZFMETHOD_FUNC_DEFINE_2(ZFIndexRange, ZFRegExpFind
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, pattern)
        ) {
    return ZFRegExpFind(src, zfobj<ZFRegExp>(pattern));
}

ZFMETHOD_FUNC_DEFINE_4(zfstring, ZFRegExpReplace
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, patternFrom)
        , ZFMP_IN(const zfstring &, patternTo)
        , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
        ) {
    if(patternFrom) {
        ZFRegExpResult result;
        zfstring ret;
        patternFrom->replace(ret, result, patternTo, src, zfindexMax(), maxReplaceCount);
        return ret;
    }
    else {
        return src;
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfstring, ZFRegExpReplace
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, patternFrom)
        , ZFMP_IN(const zfstring &, patternTo)
        , ZFMP_IN_OPT(zfindex, maxReplaceCount, zfindexMax())
        ) {
    return ZFRegExpReplace(src, zfobj<ZFRegExp>(patternFrom), patternTo, maxReplaceCount);
}

ZFMETHOD_FUNC_DEFINE_3(ZFCoreArray<zfstring>, ZFRegExpSplit
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(ZFRegExp *, separatorPattern)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        ) {
    ZFCoreArray<zfstring> ret;
    if(src != zfnull && separatorPattern != zfnull) {
        ZFRegExpResult result;
        do {
            separatorPattern->find(result, src);
            if(!result.matched || result.matchedRange == ZFIndexRangeMax()) {
                if(*src || keepEmpty) {
                    ret.add(src);
                }
                break;
            }

            if(result.matchedRange.start > 0 || keepEmpty) {
                ret.add(zfstring(src, result.matchedRange.start));
            }
            src += result.matchedRange.start + result.matchedRange.count;
        } while(zftrue);
    }
    else if(src != zfnull) {
        if(*src || keepEmpty) {
            ret.add(src);
        }
    }
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(ZFCoreArray<zfstring>, ZFRegExpSplit
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN(const zfstring &, separatorPattern)
        , ZFMP_IN_OPT(zfbool, keepEmpty, zffalse)
        ) {
    return ZFRegExpSplit(src, zfobj<ZFRegExp>(separatorPattern), keepEmpty);
}

ZF_NAMESPACE_GLOBAL_END


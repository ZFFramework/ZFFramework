#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFRegExp.h"

#undef min
#undef max
#include "../_repo/deelx/deelx.h"
#undef min
#undef max

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFRegExpImpl_default, ZFRegExp, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("deelx")
public:
    virtual void *nativeRegExpCreate(ZF_IN ZFRegExp *regExp) {
        return zfnew(CRegexpT<zfchar>);
    }
    virtual void nativeRegExpDestroy(
            ZF_IN ZFRegExp *regExp
            , ZF_IN void *nativeRegExp
            ) {
        zfdelete((CRegexpT<zfchar> *)nativeRegExp);
    }

    virtual void pattern(
            ZF_IN ZFRegExp *regExp
            , ZF_IN const zfchar *pattern
            , ZF_IN_OPT ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::EnumDefault()
            ) {
        CRegexpT<zfchar> *regexp = (CRegexpT<zfchar> *)regExp->nativeRegExp();

        int tmp = NO_FLAG;
        if(ZFBitTest(flag, v_ZFRegExpOption::e_IgnoreCase)) {
            ZFBitSet(tmp, IGNORECASE);
        }
        if(ZFBitTest(flag, v_ZFRegExpOption::e_SingleLine)) {
            ZFBitSet(tmp, SINGLELINE);
        }
        if(ZFBitTest(flag, v_ZFRegExpOption::e_MultiLine)) {
            ZFBitSet(tmp, MULTILINE);
        }
        regexp->Compile(pattern, tmp);
    }

    virtual zfindex namedGroupIndexForName(
            ZF_IN ZFRegExp *regExp
            , ZF_IN const zfchar *name
            ) {
        CRegexpT<zfchar> *regexp = (CRegexpT<zfchar> *)regExp->nativeRegExp();

        zfint ret = regexp->GetNamedGroupNumber(name);
        return ((ret < 0) ? zfindexMax() : (zfindex)ret);
    }

    virtual void find(
            ZF_IN ZFRegExp *regExp
            , ZF_OUT ZFRegExpResult &result
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLength = zfindexMax()
            ) {
        CRegexpT<zfchar> *regexp = (CRegexpT<zfchar> *)regExp->nativeRegExp();

        MatchResult regexpResult = ((srcLength == zfindexMax())
            ? regexp->Match(src)
            : regexp->Match(src, (zfint)srcLength, 0, zfnull));
        this->fillResult(result, regexpResult);
    }

    virtual void replace(
            ZF_IN ZFRegExp *regExp
            , ZF_OUT zfstring &ret
            , ZF_OUT ZFRegExpResult &result
            , ZF_IN const zfchar *replacePattern
            , ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLength = zfindexMax()
            , ZF_IN_OPT zfindex maxReplaceCount = zfindexMax()
            ) {
        CRegexpT<zfchar> *regexp = (CRegexpT<zfchar> *)regExp->nativeRegExp();

        zfint dummy;
        MatchResult regexpResult;
        zfchar *pResult = regexp->Replace(
            src, (zfint)((srcLength == zfindexMax()) ? zfslen(src) : srcLength),
            replacePattern, (zfint)zfslen(replacePattern),
            dummy,
            -1,
            (zfint)((maxReplaceCount == zfindexMax()) ? -1 : maxReplaceCount),
            &regexpResult);
        if(pResult != zfnull) {
            ret += pResult;
            regexp->ReleaseString(pResult);
        }

        this->fillResult(result, regexpResult);
    }

private:
    void fillResult(
            ZF_OUT ZFRegExpResult &result
            , ZF_IN const MatchResult &regexpResult
            ) {
        result.matched = (regexpResult.IsMatched() != 0);
        if(regexpResult.GetEnd() > regexpResult.GetStart()) {
            result.matchedRange = ZFIndexRangeCreate(regexpResult.GetStart(), regexpResult.GetEnd() - regexpResult.GetStart());
        }
        else {
            result.matchedRange = ZFIndexRangeMax();
        }
        zfindex n = regexpResult.MaxGroupNumber() + 1;
        for(zfindex i = 1; i < n; ++i) {
            ZFIndexRange indexPair;
            indexPair.start = regexpResult.GetGroupStart((zfint)i);
            indexPair.count = regexpResult.GetGroupEnd((zfint)i) - indexPair.start;
            if(indexPair.start != zfindexMax() && indexPair.count > 0) {
                result.namedGroups.add(indexPair);
            }
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFRegExpImpl_default)

ZF_NAMESPACE_GLOBAL_END


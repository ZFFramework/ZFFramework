#include "ZFVersion.h"
#include "ZFCoreDataPairSplit.h"
#include "ZFCoreStringConvert.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void zfVersionGet(
        ZF_OUT zfstring &ret
        , ZF_IN const zfchar *version
        , ZF_IN zfindex subVersionIndex
        ) {
    ZFCoreArray<ZFIndexRange> pos;
    if(*version != '\0' && !zfCoreDataPairSplitString(pos, zfindexMax(), version, zfindexMax(), ".", zfnull, zfnull)) {
        return;
    }
    if(pos.count() <= subVersionIndex) {
        return;
    }
    ret.append(version + pos[subVersionIndex].start, pos[subVersionIndex].count);
}
void zfVersionGetInt(
        ZF_OUT zfuint &ret
        , ZF_IN const zfchar *version
        , ZF_IN zfindex subVersionIndex
        ) {
    ret = zfsToInt(zfVersionGet(version, subVersionIndex));
}
void zfVersionSet(
        ZF_IN_OUT zfstring &version
        , ZF_IN zfindex subVersionIndex
        , ZF_IN const zfchar *subVersion
        , ZF_IN_OPT const zfchar *emptySubVersion /* = "0" */
        ) {
    ZFCoreArray<ZFIndexRange> pos;
    if(!version.isEmpty() && !zfCoreDataPairSplitString(pos, zfindexMax(), version, zfindexMax(), ".", zfnull, zfnull)) {
        return;
    }

    if(pos.count() <= subVersionIndex) {
        for(zfindex i = pos.count(); i < subVersionIndex; ++i) {
            if(i != 0) {
                version += '.';
            }
            version += emptySubVersion;
        }
        version += '.';
        version += subVersion;
    }
    else {
        version.replace(pos[subVersionIndex].start, pos[subVersionIndex].count, subVersion);
    }
}
void zfVersionSetInt(
        ZF_IN_OUT zfstring &version
        , ZF_IN zfindex subVersionIndex
        , ZF_IN zfuint subVersion
        , ZF_IN_OPT const zfchar *emptySubVersion /* = "0" */
        ) {
    zfVersionSet(version, subVersionIndex, zfsFromInt(subVersion), emptySubVersion);
}

ZFCompareResult _ZFP_ZFComparerForVersion(
        ZF_IN const zfchar *const &v0
        , ZF_IN const zfchar *const &v1
        ) {
    zfindex n0 = 0;
    zfindex n1 = 0;
    if(!zfsToIntT(n0, v0, zfindexMax(), 36, /* allowNegative */zffalse)
            || !zfsToIntT(n1, v1, zfindexMax(), 36, /* allowNegative */zffalse)
            ) {
        return ZFCompareUncomparable;
    }
    return ZFComparerDefault(n0, n1);
}
ZFCompareResult zfVersionCompare(
        ZF_IN const zfchar *version0
        , ZF_IN const zfchar *version1
        , ZF_IN_OPT ZFComparer<const zfchar *>::Comparer subVersionComparer /* = ZFComparerForVersion */
        ) {
    ZFCoreArray<ZFIndexRange> pos0;
    ZFCoreArray<ZFIndexRange> pos1;
    if(!zfCoreDataPairSplitString(pos0, zfindexMax(), version0, zfindexMax(), ".", zfnull, zfnull)
            || !zfCoreDataPairSplitString(pos1, zfindexMax(), version1, zfindexMax(), ".", zfnull, zfnull)
            ) {
        return ZFCompareUncomparable;
    }

    zfindex count = zfmMin(pos0.count(), pos1.count());
    for(zfindex i = 0; i < count; ++i) {
        ZFCompareResult cmp = subVersionComparer(
            zfstring(version0 + pos0[i].start, pos0[i].count)
            ,
            zfstring(version1 + pos1[i].start, pos1[i].count)
            );
        switch(cmp) {
            case ZFCompareUncomparable:
            case ZFCompareSmaller:
            case ZFCompareGreater:
                return cmp;
            case ZFCompareEqual:
                continue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ZFCompareUncomparable;
        }
    }
    if(pos0.count() > pos1.count()) {
        return ZFCompareGreater;
    }
    else if(pos0.count() < pos1.count()) {
        return ZFCompareSmaller;
    }
    else {
        return ZFCompareEqual;
    }
}

ZF_NAMESPACE_GLOBAL_END


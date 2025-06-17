#include "ZFCoreDataPairSplit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFCoreDataPairSplitString(
        ZF_IN_OUT ZFCoreArray<ZFIndexRange> &outData
        , ZF_IN zfindex desiredCountOrIndexMax
        , ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_IN_OPT const zfchar *separatorTokens /* = "," */
        , ZF_IN_OPT const zfchar *leftTokens /* = "(" */
        , ZF_IN_OPT const zfchar *rightTokens /* = ")" */
        , ZF_IN_OPT zfbool allowEmptyItem /* = zffalse */
        ) {
    if(src == zfnull) {
        return zffalse;
    }
    const zfchar *p = src;
    const zfchar *srcEnd = ((srcLen == zfindexMax()) ? (src + zfslen(src)) : (src + srcLen));

    // leftTokens
    zfindex leftTokenMatch = zfindexMax();
    zfcharSkipSpace(p, srcEnd);
    if(leftTokens != zfnull && *leftTokens != '\0') {
        leftTokenMatch = zfstringFind(leftTokens, p, 1);
        if(leftTokenMatch == zfindexMax()) {
            return zffalse;
        }
        ++p;
    }

    // each item
    zfindex count = 0;
    ZFCoreArray<zfindex> itemTokenMatch;
    do {
        // skip item header space
        zfcharSkipSpace(p, srcEnd);

        // find item end pos
        const zfchar *pl = p;
        const zfchar *pr = p;
        zfbool matchEnd = zffalse;
        for( ; pr < srcEnd; ++pr) {
            if(zfstringFind(separatorTokens, pr, 1) != zfindexMax()) {
                if(itemTokenMatch.isEmpty()) {
                    // next item
                    break;
                }
                else {
                    // separator in paired child item
                    pl = pr + 1;
                    continue;
                }
            }

            zfindex matchL = zfstringFind(leftTokens, pr, 1);
            if(matchL != zfindexMax()) {
                // paired child item
                itemTokenMatch.add(matchL);
                pl = pr + 1;
                continue;
            }

            zfindex matchR = zfstringFind(rightTokens, pr, 1);
            if(matchR != zfindexMax()) {
                if(itemTokenMatch.isEmpty()) {
                    if(matchR != leftTokenMatch) {
                        // invalid right token
                        return zffalse;
                    }
                    // reach end
                    matchEnd = zftrue;
                    break;
                }
                else {
                    if(matchR != itemTokenMatch.getLast()) {
                        return zffalse;
                    }
                    itemTokenMatch.removeLast();
                    pl = pr + 1;
                }
            }
            else if(!zfcharIsSpace(*pr)) {
                pl = pr + 1;
            }
        }
        if(!itemTokenMatch.isEmpty()) {
            return zffalse;
        }

        if(!allowEmptyItem && pl <= p) {
            return zffalse;
        }

        // save item
        outData.add(ZFIndexRangeCreate(p - src, pl - p));
        ++count;

        // end item
        if(matchEnd || pr >= srcEnd) {p = pr; break;}
        p = pr + 1;
    } while(count < desiredCountOrIndexMax);

    // count check
    if(desiredCountOrIndexMax != zfindexMax() && count != desiredCountOrIndexMax) {
        return zffalse;
    }

    // rightTokens
    zfcharSkipSpace(p, srcEnd);
    if(rightTokens != zfnull && *rightTokens != '\0') {
        if(p == srcEnd || zfstringFind(rightTokens, p, 1) == zfindexMax()) {
            return zffalse;
        }
        ++p;
    }

    // tail check
    zfcharSkipSpace(p, srcEnd);
    if(p < srcEnd) {
        return zffalse;
    }

    // success
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


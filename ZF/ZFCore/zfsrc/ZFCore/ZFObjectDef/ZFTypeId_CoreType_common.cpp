#include "ZFTypeId_CoreType.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfbool, zfbool, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_zfbool_zftrue,
            ZFTOKEN_zfbool_zffalse,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = zffalse;
        switch(matched) {
            case 0:
                v = zftrue;
                return zftrue;
            case 1:
                v = zffalse;
                return zftrue;
            case 2:
                v = zffalse;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        s += (v ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse);
        return zftrue;
    })

ZFOBJECT_REGISTER(v_zfboolHolder)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfbyte, zfbyte, {
        if(!zfsToIntT(v, src, srcLen, 16, zffalse)) {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        return zftrue;
    }, {
        return zfsFromIntT(s, v, 16);
    })
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfbyte, zfbyte)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfchar, zfchar, {
        if(src == zfnull || srcLen == 0) {
            return zffalse;
        }
        v = *src;
        return zftrue;
    }, {
        s += v;
        return zftrue;
    })
ZFTYPEID_PROGRESS_DEFINE_BY_VALUE(zfchar, zfchar)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(zfstring, zfstring, {
        v.append(src, srcLen);
        return zftrue;
    }, {
        s += v;
        return zftrue;
    })
ZFTYPEID_PROGRESS_DEFINE(zfstring, zfstring, {
    zfindex pos = 0;
    while(from[pos] && from[pos] == to[pos]) {
        ++pos;
    }
    if(pos == from.length() && pos == to.length()) {
        ret = from;
        return zftrue;
    }
    if(progress < 0) {
        ret = from;
    }
    else if(progress > 1) {
        ret = to;
    }
    else {
        zfindex p = (zfindex)((from.length() - pos + to.length() - pos) * progress);
        if(p < from.length() - pos) {
            ret.assign(from, from.length() - p);
        }
        else {
            ret.assign(to, pos + p - (from.length() - pos));
        }
    }
})

ZFOBJECT_ON_INIT_USER_REGISTER_2({
        zfstring &zfv = invokerObject.to<v_zfstring *>()->zfv;
        zfv.assign(src, len);
    }, v_zfstring
    , ZFMP_IN(const zfchar *, src)
    , ZFMP_IN(zfindex, len)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_3({
        zfstring &zfv = invokerObject.to<v_zfstring *>()->zfv;
        zfv.assign(src + pos, len);
    }, v_zfstring
    , ZFMP_IN(const zfchar *, src)
    , ZFMP_IN(zfindex, pos)
    , ZFMP_IN(zfindex, len)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        zfstring &zfv = invokerObject.to<v_zfstring *>()->zfv;
        zfv.assign((const zfchar *)src, len);
    }, v_zfstring
    , ZFMP_IN(const void *, src)
    , ZFMP_IN(zfindex, len)
    )
ZFOBJECT_ON_INIT_USER_REGISTER_3({
        zfstring &zfv = invokerObject.to<v_zfstring *>()->zfv;
        zfv.assign(((const zfchar *)src) + pos, len);
    }, v_zfstring
    , ZFMP_IN(const void *, src)
    , ZFMP_IN(zfindex, pos)
    , ZFMP_IN(zfindex, len)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_0(zfstring, v_zfstring, const zfstring &, Empty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(zfstring, v_zfstring, zfstring, shared
        , ZFMP_IN(const zfchar *, sLiteral)
        , ZFMP_IN_OPT(zfindex, length, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_2(zfstring, v_zfstring, zfstring, shared
        , ZFMP_IN(const void *, sLiteral)
        , ZFMP_IN_OPT(zfindex, length, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, set
        , ZFMP_IN(zfindex, pos)
        , ZFMP_IN(zfchar, c)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, zfchar, get
        , ZFMP_IN(zfindex, pos)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, swap
        , ZFMP_IN_OUT(zfstring &, ref)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, append
        , ZFMP_IN(const zfstring &, s)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, append
        , ZFMP_IN(const zfchar *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, append
        , ZFMP_IN(const void *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, assign
        , ZFMP_IN(const zfstring &, s)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, assign
        , ZFMP_IN(const zfchar *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, assign
        , ZFMP_IN(const void *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, insert
        , ZFMP_IN(zfindex, insertAt)
        , ZFMP_IN(const zfstring &, s)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_zfstring, void, insert
        , ZFMP_IN(zfindex, insertAt)
        , ZFMP_IN(const zfchar *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_zfstring, void, insert
        , ZFMP_IN(zfindex, insertAt)
        , ZFMP_IN(const void *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_zfstring, void, replace
        , ZFMP_IN(zfindex, replacePos)
        , ZFMP_IN(zfindex, replaceLen)
        , ZFMP_IN(const zfstring &, s)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_zfstring, void, replace
        , ZFMP_IN(zfindex, replacePos)
        , ZFMP_IN(zfindex, replaceLen)
        , ZFMP_IN(const zfchar *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_zfstring, void, replace
        , ZFMP_IN(zfindex, replacePos)
        , ZFMP_IN(zfindex, replaceLen)
        , ZFMP_IN(const void *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfindex, length)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfbool, isEmpty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, capacity
        , ZFMP_IN(zfindex, capacity)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfindex, capacity)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, void, capacityTrim)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, void, remove
        , ZFMP_IN_OPT(zfindex, pos, 0)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, zfint, compare
        , ZFMP_IN(const zfstring &, s)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_zfstring, zfint, compare
        , ZFMP_IN(const zfchar *, s)
        , ZFMP_IN_OPT(zfindex, len, zfindexMax())
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, const void *, buffer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, void *, zfunsafe_bufferGiveUp)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_STATIC_1(zfstring, v_zfstring, void, zfunsafe_bufferFree
        , ZFMP_IN(void *, buf)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfstring, zfchar *, zfunsafe_buffer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfstring, void, zfunsafe_length
        , ZFMP_IN(zfindex, length)
        )

ZFTYPEID_ALIAS_DEFINE(zfstring, zfstring, cString, const zfchar *)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCompareResult, ZFCompareResult, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFCompareUncomparable,
            ZFTOKEN_ZFCompareSmaller,
            ZFTOKEN_ZFCompareEqual,
            ZFTOKEN_ZFCompareGreater,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFCompareUncomparable;
        switch(matched) {
            case 0:
                v = ZFCompareUncomparable;
                return zftrue;
            case 1:
                v = ZFCompareSmaller;
                return zftrue;
            case 2:
                v = ZFCompareEqual;
                return zftrue;
            case 3:
                v = ZFCompareGreater;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFCompareUncomparable:
                s += ZFTOKEN_ZFCompareUncomparable;
                return zftrue;
            case ZFCompareSmaller:
                s += ZFTOKEN_ZFCompareSmaller;
                return zftrue;
            case ZFCompareEqual:
                s += ZFTOKEN_ZFCompareEqual;
                return zftrue;
            case ZFCompareGreater:
                s += ZFTOKEN_ZFCompareGreater;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_RAW_ENUM_DEFINE(ZFCompareResult
        , ZFCompareUncomparable
        , ZFCompareSmaller
        , ZFCompareEqual
        , ZFCompareGreater
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFSeekPos, ZFSeekPos, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFSeekPosBegin,
            ZFTOKEN_ZFSeekPosCur,
            ZFTOKEN_ZFSeekPosCurReversely,
            ZFTOKEN_ZFSeekPosEnd,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFSeekPosBegin;
        switch(matched) {
            case 0:
                v = ZFSeekPosBegin;
                return zftrue;
            case 1:
                v = ZFSeekPosCur;
                return zftrue;
            case 2:
                v = ZFSeekPosCurReversely;
                return zftrue;
            case 3:
                v = ZFSeekPosEnd;
                return zftrue;
            case 4:
                v = ZFSeekPosBegin;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFSeekPosBegin:
                s += ZFTOKEN_ZFSeekPosBegin;
                return zftrue;
            case ZFSeekPosCur:
                s += ZFTOKEN_ZFSeekPosCur;
                return zftrue;
            case ZFSeekPosCurReversely:
                s += ZFTOKEN_ZFSeekPosCurReversely;
                return zftrue;
            case ZFSeekPosEnd:
                s += ZFTOKEN_ZFSeekPosEnd;
                return zftrue;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    })
ZFEXPORT_RAW_ENUM_DEFINE(ZFSeekPos
        , ZFSeekPosBegin
        , ZFSeekPosCur
        , ZFSeekPosCurReversely
        , ZFSeekPosEnd
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFIndexRange, ZFIndexRange, {
        v = ZFIndexRangeZero();
        ZFCoreArray<zfindex> pair;
        if(!ZFCoreDataPairSplitInt(pair, 2, src, srcLen)) {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        v.start = pair[0];
        v.count = pair[1];
        return zftrue;
    }, {
        zfstringAppend(s, "(%s, %s)", v.start, v.count);
        return zftrue;
    })
ZFTYPEID_PROGRESS_DEFINE(ZFIndexRange, ZFIndexRange, {
    if(from.start == zfindexMax() || to.start == zfindexMax()) {
        ret.start = zfindexMax();
        ret.count = zfindexMax();
    }
    else {
        ret.start = (zfindex)(from.start + (zfindex)((to.start - from.start) * progress));

        if(from.count == zfindexMax() && to.count == zfindexMax()) {
            ret.count = zfindexMax();
        }
        else {
            zfindex fromCount = (from.count != zfindexMax() ? from.count : to.count);
            zfindex toCount = (to.count != zfindexMax() ? to.count : from.count);
            ret.count = (zfindex)(fromCount + (zfindex)((toCount - fromCount) * progress));
        }
    }
})
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFIndexRange, zfindex, start)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFIndexRange, zfindex, count)

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFLevel, ZFLevel, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFLevelZFFrameworkStatic,
            ZFTOKEN_ZFLevelZFFrameworkEssential,
            ZFTOKEN_ZFLevelZFFrameworkHigh,
            ZFTOKEN_ZFLevelZFFrameworkNormal,
            ZFTOKEN_ZFLevelZFFrameworkLow,

            ZFTOKEN_ZFLevelAppEssential,
            ZFTOKEN_ZFLevelAppHigh,
            ZFTOKEN_ZFLevelAppNormal,
            ZFTOKEN_ZFLevelAppLow,

            ZFTOKEN_ZFLevelZFFrameworkPostLow,
            ZFTOKEN_ZFLevelZFFrameworkPostNormal,
            ZFTOKEN_ZFLevelZFFrameworkPostHigh,
            ZFTOKEN_ZFLevelZFFrameworkPostEssential,
            ZFTOKEN_ZFLevelZFFrameworkPostStatic,

            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFLevelAppLow;
        switch(matched) {
            case 0:
                v = ZFLevelZFFrameworkStatic;
                return zftrue;
            case 1:
                v = ZFLevelZFFrameworkEssential;
                return zftrue;
            case 2:
                v = ZFLevelZFFrameworkHigh;
                return zftrue;
            case 3:
                v = ZFLevelZFFrameworkNormal;
                return zftrue;
            case 4:
                v = ZFLevelZFFrameworkLow;
                return zftrue;

            case 5:
                v = ZFLevelAppEssential;
                return zftrue;
            case 6:
                v = ZFLevelAppHigh;
                return zftrue;
            case 7:
                v = ZFLevelAppNormal;
                return zftrue;
            case 8:
                v = ZFLevelAppLow;
                return zftrue;

            case 9:
                v = ZFLevelZFFrameworkPostLow;
                return zftrue;
            case 10:
                v = ZFLevelZFFrameworkPostNormal;
                return zftrue;
            case 11:
                v = ZFLevelZFFrameworkPostHigh;
                return zftrue;
            case 12:
                v = ZFLevelZFFrameworkPostEssential;
                return zftrue;
            case 13:
                v = ZFLevelZFFrameworkPostStatic;
                return zftrue;

            case 14:
                v = ZFLevelAppNormal;
                return zftrue;

            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFLevelZFFrameworkStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkStatic;
                return zftrue;
            case ZFLevelZFFrameworkEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkEssential;
                return zftrue;
            case ZFLevelZFFrameworkHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkHigh;
                return zftrue;
            case ZFLevelZFFrameworkNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkNormal;
                return zftrue;
            case ZFLevelZFFrameworkLow:
                s += ZFTOKEN_ZFLevelZFFrameworkLow;
                return zftrue;

            case ZFLevelAppEssential:
                s += ZFTOKEN_ZFLevelAppEssential;
                return zftrue;
            case ZFLevelAppHigh:
                s += ZFTOKEN_ZFLevelAppHigh;
                return zftrue;
            case ZFLevelAppNormal:
                s += ZFTOKEN_ZFLevelAppNormal;
                return zftrue;
            case ZFLevelAppLow:
                s += ZFTOKEN_ZFLevelAppLow;
                return zftrue;

            case ZFLevelZFFrameworkPostLow:
                s += ZFTOKEN_ZFLevelZFFrameworkPostLow;
                return zftrue;
            case ZFLevelZFFrameworkPostNormal:
                s += ZFTOKEN_ZFLevelZFFrameworkPostNormal;
                return zftrue;
            case ZFLevelZFFrameworkPostHigh:
                s += ZFTOKEN_ZFLevelZFFrameworkPostHigh;
                return zftrue;
            case ZFLevelZFFrameworkPostEssential:
                s += ZFTOKEN_ZFLevelZFFrameworkPostEssential;
                return zftrue;
            case ZFLevelZFFrameworkPostStatic:
                s += ZFTOKEN_ZFLevelZFFrameworkPostStatic;
                return zftrue;

            default:
                return zffalse;
        }
    })
ZFEXPORT_RAW_ENUM_DEFINE(ZFLevel
    , ZFLevelZFFrameworkStatic
    , ZFLevelZFFrameworkEssential
    , ZFLevelZFFrameworkHigh
    , ZFLevelZFFrameworkNormal
    , ZFLevelZFFrameworkLow
    , ZFLevelAppEssential
    , ZFLevelAppHigh
    , ZFLevelAppNormal
    , ZFLevelAppLow
    , ZFLevelZFFrameworkPostLow
    , ZFLevelZFFrameworkPostNormal
    , ZFLevelZFFrameworkPostHigh
    , ZFLevelZFFrameworkPostEssential
    , ZFLevelZFFrameworkPostStatic
    )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFFrameworkState, ZFFrameworkState, {
        const zfchar *tokens[] = ZFM_EXPAND({
            ZFTOKEN_ZFFrameworkStateNotAvailable,
            ZFTOKEN_ZFFrameworkStateInitRunning,
            ZFTOKEN_ZFFrameworkStateAvailable,
            ZFTOKEN_ZFFrameworkStateCleanupRunning,
            "",
        });
        zfindex matched = zfsCheckMatch(tokens, ZFM_ARRAY_SIZE(tokens), src, srcLen);
        v = ZFFrameworkStateNotAvailable;
        switch(matched) {
            case 0:
                v = ZFFrameworkStateNotAvailable;
                return zftrue;
            case 1:
                v = ZFFrameworkStateInitRunning;
                return zftrue;
            case 2:
                v = ZFFrameworkStateAvailable;
                return zftrue;
            case 3:
                v = ZFFrameworkStateCleanupRunning;
                return zftrue;
            case 4:
                v = ZFFrameworkStateNotAvailable;
                return zftrue;
            default:
                if(errorHint) {
                    zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
                }
                return zffalse;
        }
    }, {
        switch(v) {
            case ZFFrameworkStateNotAvailable:
                s += ZFTOKEN_ZFFrameworkStateNotAvailable;
                return zftrue;
            case ZFFrameworkStateInitRunning:
                s += ZFTOKEN_ZFFrameworkStateInitRunning;
                return zftrue;
            case ZFFrameworkStateAvailable:
                s += ZFTOKEN_ZFFrameworkStateAvailable;
                return zftrue;
            case ZFFrameworkStateCleanupRunning:
                s += ZFTOKEN_ZFFrameworkStateCleanupRunning;
                return zftrue;
            default:
                return zffalse;
        }
    })
ZFEXPORT_RAW_ENUM_DEFINE(ZFFrameworkState
        , ZFFrameworkStateNotAvailable
        , ZFFrameworkStateInitRunning
        , ZFFrameworkStateAvailable
        , ZFFrameworkStateCleanupRunning
        )

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFFuncAddrType, ZFFuncAddrType)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE_UNCOMPARABLE(zfiter, zfiter)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfiter, zfbool, valid)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfiter, void, next)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_zfiter, zfiter, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_zfiter, zfbool, isEqual
        , ZFMP_IN(const zfiter &, ref)
        )

// ============================================================
// ZFSigName
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFSigNameInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(zfstring, ZFSigNameInfo)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfidentity, ZFSigNameToId
        , ZFMP_IN_OUT(const zfstring &, name)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfstring, ZFSigNameFromId
        , ZFMP_IN(zfidentity, sigId)
        )

// ============================================================
// ZFCallerInfo
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFCallerInfo, ZFCallerInfo, {
        do {
            ZFIndexRange pos[3];
            zfmemset(pos, 0, sizeof(pos));
            const zfchar *p = src;
            const zfchar *pEnd = src + ((srcLen == zfindexMax()) ? zfslen(src) : srcLen);

            zfcharSkipSpace(p, pEnd);
            if(p >= pEnd) {
                v = ZFCallerInfo();
                return zftrue;
            }
            if(*p != '[') {
                break;
            }
            ++p;

            if(*p != ' ') {
                pos[0].start = p - src;
                while(*p != ' ' && p < pEnd) {++p;}
                pos[0].count = p - src - pos[0].start;
            }
            ++p;

            if(*p != ' ') {
                pos[1].start = p - src;
                while(*p != ' ' && p < pEnd) {++p;}
                pos[1].count = p - src - pos[1].start;
            }
            ++p;

            if(*p != '(') {
                break;
            }
            ++p;
            pos[2].start = p - src;
            while(*p != ')' && p < pEnd) {++p;}
            pos[2].count = p - src - pos[2].start;

            if(p + 2 > pEnd || *p != ')' || *(p + 1) != ']') {
                break;
            }

            v.callerInfo(
                    (pos[0].count > 0)
                        ? zfstring(src + pos[0].start, pos[0].count).cString()
                        : zfnull,
                    (pos[1].count > 0)
                        ? zfstring(src + pos[1].start, pos[1].count).cString()
                        : zfnull,
                    (zfuint)zfsToInt(zfstring(src + pos[2].start, pos[2].count).cString())
                );
            return zftrue;
        } while(zffalse);

        if(errorHint) {
            zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
        }
        return zffalse;
    }, {
        v.callerInfoT(s);
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFCallerInfo, void, callerInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, zfstring, callerInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, const zfchar *, callerFile)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, const zfchar *, callerFunc)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, zfuint, callerLine)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFCallerInfo, zfbool, valid)

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFIdentityGenerator, ZFIdentityGenerator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFIdentityGenerator, zfidentity, idAcquire)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, zfbool, idRelease
        , ZFMP_IN(zfidentity, identity)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, zfbool, idExist
        , ZFMP_IN(zfidentity, identity)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFIdentityGenerator, void, idExistGetAll
        , ZFMP_IN_OUT(ZFCoreArray<zfidentity> &, ret)
        )

// ============================================================
ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFPathInfo, ZFPathInfo, {
        if(srcLen == zfindexMax()) {
            srcLen = zfslen(src);
        }
        zfindex pos = zfstringFind(src, srcLen, ZFSerializableKeyword_ZFPathInfo_separator);
        if(pos == 0 || pos == zfindexMax()) {
            if(errorHint) {
                zfstringAppend(errorHint, "invalid value: \"%s\"", zfstring(src, srcLen));
            }
            return zffalse;
        }
        v.pathType(zfstring(src, pos));
        pos += zfslen(ZFSerializableKeyword_ZFPathInfo_separator);
        v.pathData(zfstring(src + pos, srcLen - pos));
        return zftrue;
    }, {
        s += v.pathType();
        s += ZFSerializableKeyword_ZFPathInfo_separator;
        s += v.pathData();
        return zftrue;
    })
ZFOBJECT_ON_INIT_USER_REGISTER_2({
        ZFPathInfo &zfv = invokerObject.to<v_ZFPathInfo *>()->zfv;
        zfv.pathType(pathType);
        zfv.pathData(pathData);
    }, v_ZFPathInfo
    , ZFMP_IN(const zfstring &, pathType)
    , ZFMP_IN(const zfstring &, pathData)
    )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPathInfo, ZFPathInfo &, pathType
        , ZFMP_IN(const zfstring &, pathType)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPathInfo, const zfstring &, pathType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPathInfo, ZFPathInfo &, pathData
        , ZFMP_IN(const zfstring &, pathData)
        )
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPathInfo, const zfstring &, pathData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPathInfo, zfbool, isEmpty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPathInfo, void, removeAll)

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTokenForContainer, ZFTokenForContainer, {
        v.tokenLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenLeft);
        v.tokenRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenRight);
        v.tokenSeparator = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator);
        v.tokenValueLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft);
        v.tokenValueRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight);
        v.tokenEtc = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForContainer_tokenEtc);
        return zftrue;
    }, {
        if(!v.tokenLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenLeft, v.tokenLeft);}
        if(!v.tokenRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenRight, v.tokenRight);}
        if(!v.tokenSeparator.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenSeparator, v.tokenSeparator);}
        if(!v.tokenValueLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenValueLeft, v.tokenValueLeft);}
        if(!v.tokenValueRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenValueRight, v.tokenValueRight);}
        if(!v.tokenEtc.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForContainer_tokenEtc, v.tokenEtc);}
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenValueLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenValueRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForContainer, zfstring, tokenEtc)

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFTokenForKeyValueContainer, ZFTokenForKeyValueContainer, {
        v.tokenLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft);
        v.tokenRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight);
        v.tokenSeparator = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator);
        v.tokenPairLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft);
        v.tokenPairRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight);
        v.tokenPairSeparator = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator);
        v.tokenKeyLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft);
        v.tokenKeyRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight);
        v.tokenValueLeft = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft);
        v.tokenValueRight = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight);
        v.tokenEtc = ZFSerializableUtil::checkAttr(serializableData, ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc);
        return zftrue;
    }, {
        if(!v.tokenLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenLeft, v.tokenLeft);}
        if(!v.tokenRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenRight, v.tokenRight);}
        if(!v.tokenSeparator.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenSeparator, v.tokenSeparator);}
        if(!v.tokenPairLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairLeft, v.tokenPairLeft);}
        if(!v.tokenPairRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairRight, v.tokenPairRight);}
        if(!v.tokenPairSeparator.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenPairSeparator, v.tokenPairSeparator);}
        if(!v.tokenKeyLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyLeft, v.tokenKeyLeft);}
        if(!v.tokenKeyRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenKeyRight, v.tokenKeyRight);}
        if(!v.tokenValueLeft.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueLeft, v.tokenValueLeft);}
        if(!v.tokenValueRight.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenValueRight, v.tokenValueRight);}
        if(!v.tokenEtc.isEmpty()) {serializableData.attr(ZFSerializableKeyword_ZFTokenForKeyValueContainer_tokenEtc, v.tokenEtc);}
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenPairSeparator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenKeyLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenKeyRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenValueLeft)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenValueRight)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFTokenForKeyValueContainer, zfstring, tokenEtc)

ZFMETHOD_FUNC_USER_REGISTER_0({ZFCoreMutexLock();}, void, ZFCoreMutexLock)
ZFMETHOD_FUNC_USER_REGISTER_0({ZFCoreMutexUnlock();}, void, ZFCoreMutexUnlock)

ZF_NAMESPACE_GLOBAL_END


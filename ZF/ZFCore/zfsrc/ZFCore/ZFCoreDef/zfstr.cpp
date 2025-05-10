#include "zfstr.h"
#include "ZFCoreStringUtil.h"
#include "ZFCoreUtilMacro.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_zfstrOption {
public:
    enum {
        AlignLeft = 1,
        LeadingZero = 2,
    };

public:
    zfflags flags;
    zfuint width;
    zfint precision; // -1 if none
    zfchar positiveToken; // `+` or `-` or ` `, or `\0` if none
    zfbool success;

public:
    _ZFP_zfstrOption(void)
    : flags(0)
    , width(0)
    , precision(-1)
    , positiveToken('\0')
    , success(zftrue)
    {
    }

public:
    zfbool hasFlag(void) {
        return (zffalse
                || flags != 0
                || width != 0
                || precision != -1
                || positiveToken != '\0'
                );
    }
};

static void _ZFP_zfstringAppendAction(
        ZF_IN_OUT zfstring &s
        , ZF_IN_OUT _ZFP_zfstrOption &option
        , ZF_IN_OUT const zfchar *&p
        , ZF_IN const void *param
        , ZF_IN _ZFP_zfstrFn Fn
        ) {
    switch(*p) {
        case 'b': {
            zfstring tmp;
            Fn(tmp, param);
            if(zffalse
                    || !tmp
                    || zfstringIsEqual(tmp, ZFTOKEN_zfnull)
                    || zfstringIsEqual(tmp, "0")
                    || zfstringIsEqual(tmp, ZFTOKEN_zfbool_zffalse)
                    ) {
                s += ZFTOKEN_zfbool_zffalse;
            }
            else {
                s += ZFTOKEN_zfbool_zftrue;
            }
            break;
        }
        case 'z':
            if(*(p+1) == 'i') {
                ++p;
                Fn(s, param);
            }
            else {
                option.success = zffalse;
            }
            break;
        case 'd':
        case 'i':
            if(option.hasFlag()) {
                zfstring tmp;
                Fn(tmp, param);
                zfint v = 0;
                if(zfsToIntT(v, tmp)) {
                    if(v < 0) {
                        option.positiveToken = '-';
                        v = -v;
                    }
                    zfsFromIntT(s, v);
                }
                else {
                    s += tmp;
                }
            }
            else {
                Fn(s, param);
            }
            break;
        case 'u':
            Fn(s, param);
            break;
        case 'o': {
            zfstring tmp;
            Fn(tmp, param);
            zfuint v = 0;
            if(zfsToIntT(v, tmp, zfindexMax(), 10, zffalse)) {
                zfsFromIntT(s, v, 8);
            }
            else {
                s += tmp;
            }
            break;
        }
        case 'x':
        case 'X': {
            zfstring tmp;
            Fn(tmp, param);
            zfuint v = 0;
            if(zfsToIntT(v, tmp, zfindexMax(), 10, zffalse)) {
                zfsFromIntT(s, v, 16, *p == 'X');
            }
            else {
                s += tmp;
            }
            break;
        }
        case 'f':
            if(option.hasFlag()) {
                zfstring tmp;
                Fn(tmp, param);
                zffloat v = 0;
                if(zfsToFloatT(v, tmp)) {
                    if(v < 0) {
                        option.positiveToken = '-';
                        v = -v;
                    }
                    zfsFromFloatT(s, v);
                }
                else {
                    s += tmp;
                }
            }
            else {
                Fn(s, param);
            }
            break;
        case 'p':
        case 'c':
        case 'C':
        case 's':
        case 'S':
            Fn(s, param);
            break;
        default:
            option.success = zffalse;
            break;
    }
    ++p;
}

void _ZFP_zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN zfindex paramCount
        , ZF_IN const void * const *param
        , ZF_IN _ZFP_zfstrFn *Fn
        ) {
    if(fmt == zfnull || *fmt == '\0') {
        return;
    }

    const zfchar *p = fmt;
    zfindex paramIndex = 0;
    while(*p) {
        if(*p != '%') {
            zfcharAppendAndMoveNext(s, p);
            continue;
        }

        ++p;
        if(*p == '%') {
            s += '%';
            ++p;
            continue;
        }

        const zfchar *savedPos = p;
        _ZFP_zfstrOption option;
        do {
            while(*p == '-' || *p == '0' || *p == '+' || *p == ' ') {
                if(*p == '-') {
                    if(ZFBitTest(option.flags, _ZFP_zfstrOption::AlignLeft)) {option.success = zffalse; break;}
                    else {ZFBitSet(option.flags, _ZFP_zfstrOption::AlignLeft);}
                }
                else if(*p == '0') {
                    if(ZFBitTest(option.flags, _ZFP_zfstrOption::LeadingZero)) {option.success = zffalse; break;}
                    else {ZFBitSet(option.flags, _ZFP_zfstrOption::LeadingZero);}
                }
                else if(*p == '+') {
                    if(option.positiveToken != '\0') {option.success = zffalse; break;}
                    else {option.positiveToken = '+';}
                }
                else if(*p == ' ') {
                    if(option.positiveToken != '\0') {option.success = zffalse; break;}
                    else {option.positiveToken = ' ';}
                }
                ++p;
            }
            if(!option.success) {break;}

            while(*p >= '0' && *p <= '9') {
                option.width = option.width * 10 + (*p - '0');
                ++p;
            }
            if(*p == '.') {
                ++p;
                option.precision = 0;
                while(*p >= '0' && *p <= '9') {
                    option.precision = option.precision * 10 + (*p - '0');
                    ++p;
                }
            }
        } while(zffalse);
        if(!option.success) {
            s += '%';
            p = savedPos;
            continue;
        }
        if(!option.hasFlag()) {
            _ZFP_zfstringAppendAction(s, option, p
                    , paramIndex < paramCount ? param[paramIndex] : (const void *)&(zfstring::Empty())
                    , paramIndex < paramCount ? Fn[paramIndex] : _ZFP_zfstrIvk<zfstring>
                    );
            ++paramIndex;
            continue;
        }

        zfstring tmp;
        _ZFP_zfstringAppendAction(tmp, option, p
                , paramIndex < paramCount ? param[paramIndex] : (const void *)&(zfstring::Empty())
                , paramIndex < paramCount ? Fn[paramIndex] : _ZFP_zfstrIvk<zfstring>
                );
        ++paramIndex;

        if(option.precision != -1) {
            zfindex pos = zfstringFind(tmp, '.');
            if(pos != zfindexMax()) {
                zfint decimalLen = (zfint)(tmp.length() - pos - 1);
                if(decimalLen > option.precision) {
                    zfindex newLen = pos + option.precision + 1;
                    if(tmp[newLen] >= '5') {
                        tmp.set(newLen - 1, (zfchar)(tmp[newLen - 1] + 1));
                    }
                    tmp.remove(newLen);
                }
                else if(decimalLen < option.precision) {
                    for(zfint i = decimalLen; i < option.precision; ++i) {
                        tmp += '0';
                    }
                }
            }
        }

        if(option.positiveToken != '\0') {
            ++option.width;
        }

        if(ZFBitTest(option.flags, _ZFP_zfstrOption::AlignLeft)) {
            zfuint writtenLen = 0;
            if(option.positiveToken != '\0') {
                s += option.positiveToken;
                ++writtenLen;
            }
            s += tmp;
            writtenLen += (zfuint)tmp.length();
            for(zfuint i = writtenLen; i < option.width; ++i) {
                s += ' ';
            }
        }
        else {
            zfindex leftTokenLen = ((option.positiveToken == '\0') ? 0 : 1);
            if((zfindex)option.width > tmp.length() + leftTokenLen) {
                zfindex spaceLen = option.width - tmp.length() - leftTokenLen;
                if(ZFBitTest(option.flags, _ZFP_zfstrOption::LeadingZero)) {
                    if(option.positiveToken != '\0') {
                        s += option.positiveToken;
                    }
                    for(zfindex i = 0; i < spaceLen; ++i) {
                        s += '0';
                    }
                }
                else {
                    for(zfindex i = 0; i < spaceLen; ++i) {
                        s += ' ';
                    }
                    if(option.positiveToken != '\0') {
                        s += option.positiveToken;
                    }
                }
                s += tmp;
            }
            else {
                if(option.positiveToken != '\0') {
                    s += option.positiveToken;
                }
                s += tmp;
            }
        }
    } // while(*p)
}

ZF_NAMESPACE_GLOBAL_END


#include "zfstr.h"
#include "ZFCoreStringUtil.h"
#include "ZFCoreUtilMacro.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_zfstrFlag {
public:
    enum {
        AlignLeft = 1,
        LeadingZero = 2,
    };

public:
    zfflags flags;
    zfindex width;
    zfindex precision; // zfindexMax if none
    zfchar positiveToken; // `+` or `-` or ` `, or `\0` if none
    zfbool success;

public:
    _ZFP_zfstrFlag(void)
    : flags(0)
    , width(0)
    , precision(zfindexMax())
    , positiveToken('\0')
    , success(zftrue)
    {
    }

public:
    zfbool hasFlag(void) {
        return (zffalse
                || flags != 0
                || width != 0
                || precision != zfindexMax()
                || positiveToken != '\0'
                );
    }
};

static void _ZFP_zfstringAppendAction(
        ZF_IN_OUT zfstring &s
        , ZF_IN_OUT _ZFP_zfstrFlag &flags
        , ZF_IN_OUT const zfchar *&p
        , ZF_IN const zfchar *param
        ) {
    switch(*p) {
        case 'b':
            if(zffalse
                    || zfstringIsEmpty(param)
                    || zfstringIsEqual(param, ZFTOKEN_zfnull)
                    || zfstringIsEqual(param, "0")
                    || zfstringIsEqual(param, ZFTOKEN_zfbool_zffalse)
                    ) {
                s += ZFTOKEN_zfbool_zffalse;
            }
            else {
                s += ZFTOKEN_zfbool_zftrue;
            }
            break;
        case 'z':
            if(*(p+1) == 'i') {
                ++p;
                s += param;
            }
            else {
                flags.success = zffalse;
            }
            break;
        case 'd':
        case 'i': {
            zfint v = 0;
            if(flags.hasFlag() && zfsToIntT(v, param)) {
                if(v < 0) {
                    flags.positiveToken = '-';
                    v = -v;
                }
                zfsFromIntT(s, v);
            }
            else {
                s += param;
            }
            break;
        }
        case 'u':
            s += param;
            break;
        case 'o': {
            zfuint v = 0;
            if(zfsToIntT(v, param, zfindexMax(), 10, zffalse)) {
                zfsFromIntT(s, v, 8);
            }
            else {
                s += param;
            }
            break;
        }
        case 'x': {
            zfuint v = 0;
            if(zfsToIntT(v, param, zfindexMax(), 10, zffalse)) {
                zfsFromIntT(s, v, 16, zffalse);
            }
            else {
                s += param;
            }
            break;
        }
        case 'X': {
            zfuint v = 0;
            if(zfsToIntT(v, param, zfindexMax(), 10, zffalse)) {
                zfsFromIntT(s, v, 16, zftrue);
            }
            else {
                s += param;
            }
            break;
        }
        case 'f': {
            zffloat v = 0;
            if(flags.hasFlag() && zfsToFloatT(v, param)) {
                if(v < 0) {
                    flags.positiveToken = '-';
                    v = -v;
                }
                zfsFromFloatT(s, v);
            }
            else {
                s += param;
            }
            break;
        }
        case 'p':
            s += param;
            break;
        case 'c':
        case 'C':
            s += param;
            break;
        case 's':
        case 'S': {
            if(!zfstringIsEqual(param, ZFTOKEN_zfnull)) {
                if(flags.precision != zfindexMax()) {
                    zfindex len = zfslen(param);
                    if(flags.precision < len) {
                        len = flags.precision;
                    }
                    s.append(param, len);
                }
                else {
                    s.append(param);
                }
            }
            else {
                s += ZFTOKEN_zfnull;
            }
        }
            break;
        default:
            flags.success = zffalse;
            break;
    }
    ++p;
}

void _ZFP_zfstringAppend(
        ZF_IN_OUT zfstring &s
        , ZF_IN const zfchar *fmt
        , ZF_IN const zfstring *param
        , ZF_IN zfindex paramCount
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
        _ZFP_zfstrFlag flags;
        do {
            while(*p == '-' || *p == '0' || *p == '+' || *p == ' ') {
                if(*p == '-') {
                    if(ZFBitTest(flags.flags, _ZFP_zfstrFlag::AlignLeft)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstrFlag::AlignLeft);}
                }
                else if(*p == '0') {
                    if(ZFBitTest(flags.flags, _ZFP_zfstrFlag::LeadingZero)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstrFlag::LeadingZero);}
                }
                else if(*p == '+') {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = '+';}
                }
                else if(*p == ' ') {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = ' ';}
                }
                ++p;
            }
            if(!flags.success) {break;}

            while(*p >= '0' && *p <= '9') {
                flags.width = flags.width * 10 + (*p - '0');
                ++p;
            }
            if(*p == '.') {
                ++p;
                flags.precision = 0;
                while(*p >= '0' && *p <= '9') {
                    flags.precision = flags.precision * 10 + (*p - '0');
                    ++p;
                }
            }
        } while(zffalse);
        if(!flags.success) {
            s += '%';
            p = savedPos;
            continue;
        }
        if(!flags.hasFlag()) {
            _ZFP_zfstringAppendAction(s, flags, p, paramIndex < paramCount ? param[paramIndex].cString() : "");
            ++paramIndex;
            continue;
        }

        zfstring tmp;
        _ZFP_zfstringAppendAction(tmp, flags, p, paramIndex < paramCount ? param[paramIndex].cString() : "");
        ++paramIndex;

        if(flags.precision != zfindexMax()) {
            zfindex pos = zfstringFind(tmp, '.');
            if(pos != zfindexMax()) {
                zfindex decimalLen = tmp.length() - pos - 1;
                if(decimalLen > flags.precision) {
                    zfindex newLen = pos + flags.precision + 1;
                    if(tmp[newLen] >= '5') {
                        tmp[newLen - 1] = tmp[newLen - 1] + 1;
                    }
                    tmp.remove(newLen);
                }
                else if(decimalLen < flags.precision) {
                    for(zfindex i = decimalLen; i < flags.precision; ++i) {
                        tmp += '0';
                    }
                }
            }
        }

        if(flags.positiveToken != '\0') {
            ++flags.width;
        }

        if(ZFBitTest(flags.flags, _ZFP_zfstrFlag::AlignLeft)) {
            zfindex writtenLen = 0;
            if(flags.positiveToken != '\0') {
                s += flags.positiveToken;
                ++writtenLen;
            }
            s += tmp;
            writtenLen += tmp.length();
            for(zfindex i = writtenLen; i < flags.width; ++i) {
                s += ' ';
            }
        }
        else {
            zfindex leftTokenLen = ((flags.positiveToken == '\0') ? 0 : 1);
            if(flags.width > tmp.length() + leftTokenLen) {
                zfindex spaceLen = flags.width - tmp.length() - leftTokenLen;
                if(ZFBitTest(flags.flags, _ZFP_zfstrFlag::LeadingZero)) {
                    if(flags.positiveToken != '\0') {
                        s += flags.positiveToken;
                    }
                    for(zfindex i = 0; i < spaceLen; ++i) {
                        s += '0';
                    }
                }
                else {
                    for(zfindex i = 0; i < spaceLen; ++i) {
                        s += ' ';
                    }
                    if(flags.positiveToken != '\0') {
                        s += flags.positiveToken;
                    }
                }
                s += tmp;
            }
            else {
                if(flags.positiveToken != '\0') {
                    s += flags.positiveToken;
                }
                s += tmp;
            }
        }
    } // while(*p)
}

ZF_NAMESPACE_GLOBAL_END


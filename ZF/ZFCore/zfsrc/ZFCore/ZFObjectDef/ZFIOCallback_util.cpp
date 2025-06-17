#include "ZFIOCallback_util.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfindex ZFInputReadChar(
        ZF_OUT zfchar *p
        , ZF_IN_OUT const ZFInput &input
        ) {
    if(input.execute(p, 1) != 1) {
        p[0] = '\0';
        return 0;
    }
    if((zfbyte)p[0] < 0x7F) {
        p[1] = '\0';
        return 1;
    }

    if(input.execute(p + 1, 1) != 1) {
        p[1] = '\0';
        return 1;
    }
    if((zfbyte)p[0] <= 0xDF && (zfbyte)p[1] <= 0xBF) {
        p[2] = '\0';
        return 2;
    }

    if(input.execute(p + 2, 1) != 1) {
        p[2] = '\0';
        return 2;
    }
    if((zfbyte)p[0] <= 0xEF && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF) {
        p[3] = '\0';
        return 3;
    }

    if(input.execute(p + 3, 1) != 1) {
        p[3] = '\0';
        return 3;
    }
    if((zfbyte)p[0] <= 0xF7 && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF) {
        p[4] = '\0';
        return 4;
    }

    if(input.execute(p + 4, 1) != 1) {
        p[4] = '\0';
        return 4;
    }
    if((zfbyte)p[0] <= 0xFB && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF) {
        p[5] = '\0';
        return 5;
    }

    if(input.execute(p + 5, 1) != 1) {
        p[5] = '\0';
        return 5;
    }
    if((zfbyte)p[0] <= 0xFD && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF) {
        p[6] = '\0';
        return 6;
    }

    if(input.execute(p + 6, 1) != 1) {
        p[6] = '\0';
        return 6;
    }
    if((zfbyte)p[0] <= 0xFE && (zfbyte)p[1] <= 0xBF && (zfbyte)p[2] <= 0xBF && (zfbyte)p[3] <= 0xBF && (zfbyte)p[4] <= 0xBF && (zfbyte)p[5] <= 0xBF && (zfbyte)p[6] <= 0xBF) {
        p[7] = '\0';
        return 7;
    }

    if(input.execute(p + 7, 1) != 1) {
        p[7] = '\0';
        return 7;
    }

    p[8] = '\0';
    return 8;
}
zfindex ZFInputReadChar(
        ZF_IN_OUT zfstring &buf
        , ZF_IN_OUT const ZFInput &input
        ) {
    zfchar tmp[9] = {0};
    zfindex ret = ZFInputReadChar(tmp, input);
    buf += tmp;
    return ret;
}

zfindex ZFInputSkipChars(
        ZF_OUT zfchar *buf
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT const zfchar *charSet /* = " \t\r\n" */
        ) {
    zfindex charSetCount = zfslen(charSet);
    zfbool matched = zffalse;
    do {
        zfindex t = ZFInputReadChar(buf, input);
        switch(t) {
            case 0:
                return 0;
            case 1:
                break;
            default:
                return t;
        }
        matched = zffalse;
        for(zfindex i = 0; i < charSetCount; ++i) {
            if(charSet[i] == buf[0]) {
                matched = zftrue;
                break;
            }
        }
        if(!matched) {
            return t;
        }
    } while(zftrue);
    return 0;
}
zfindex ZFInputReadUntil(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT const zfchar *charSet /* = " \t\r\n" */
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_OUT_OPT zfchar *firstMatchedChar /* = zfnull */
        ) {
    zfindex readCount = 0;
    if(firstMatchedChar != zfnull) {
        *firstMatchedChar = '\0';
    }
    if(input) {
        zfchar buf[9] = {0};
        zfindex charSetCount = zfslen(charSet);
        zfbool matched = zffalse;
        while(readCount < maxCount) {
            zfindex t = ZFInputReadChar(buf, input);
            if(t == 0) {
                break;
            }
            else if(t == 1) {
                matched = zffalse;
                for(zfindex i = 0; i < charSetCount; ++i) {
                    if(charSet[i] == buf[0]) {
                        matched = zftrue;
                        break;
                    }
                }
                if(matched) {
                    if(firstMatchedChar != zfnull) {
                        *firstMatchedChar = buf[0];
                    }
                    break;
                }
            }

            ret += buf;
            readCount += t;
        }
    }
    return readCount;
}
zfindex ZFInputCheckMatch(
        ZF_IN const zfchar **tokens
        , ZF_IN zfindex tokenCount
        , ZF_IN_OUT const ZFInput &input
        ) {
    zfindex ret = zfindexMax();
    if(input) {
        zfindex saved = input.ioTell();
        zfindex maxLen = 0;
        for(zfindex i = 0; i < tokenCount; ++i) {
            maxLen = zfmMax(maxLen, zfslen(tokens[i]));
        }
        zfchar *buf = (zfchar *)zfmalloc(maxLen + 1);
        zfblockedFree(buf);

        zfbool matched = zffalse;
        zfindex firstCharLen = ZFInputSkipChars(buf, input);
        zfindex savedFix = input.ioTell() - firstCharLen;
        if(firstCharLen < maxLen) {
            zfindex read = input.execute(buf + firstCharLen, maxLen - firstCharLen);
            for(zfindex i = 0; i < tokenCount; ++i) {
                zfindex tokenLen = zfslen(tokens[i]);
                if(zfstringIsEqual(tokens[i], tokenLen, buf, read + firstCharLen)) {
                    saved = savedFix + tokenLen;
                    matched = zftrue;
                    ret = i;
                    break;
                }
            }
        }

        input.ioSeek(saved, ZFSeekPosBegin);
    }
    return ret;
}

// ============================================================
#define _ZFP_ZFInputRead_blockSize 1024
zfindex ZFInputRead(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT zfindex size /* = zfindexMax() */
        ) {
    zfindex read = 0;
    if(input && output) {
        zfchar buf[_ZFP_ZFInputRead_blockSize];
        zfindex readCount = 0;
        zfindex writeCount = 0;
        zfindex toRead = 0;
        do {
            if(read + _ZFP_ZFInputRead_blockSize <= size) {
                toRead = _ZFP_ZFInputRead_blockSize;
            }
            else {
                toRead = size - read;
            }
            readCount = input.execute(buf, toRead);
            writeCount = output.execute(buf, readCount);
            read += writeCount;
            if(readCount < toRead || writeCount < readCount || read >= size) {
                break;
            }
        } while(zftrue);
    }
    return read;
}

zfindex ZFInputRead(
        ZF_IN_OUT zfstring &ret
        , ZF_IN_OUT const ZFInput &input
        , ZF_IN_OPT zfindex size /* = zfindexMax() */
        ) {
    zfindex read = 0;
    if(input) {
        zfindex readCount = 0;
        zfindex toRead = 0;
        do {
            if(read + _ZFP_ZFInputRead_blockSize <= size) {
                toRead = _ZFP_ZFInputRead_blockSize;
            }
            else {
                toRead = size - read;
            }
            ret.capacity(ret.length() + (read + toRead));
            readCount = input.execute(ret.zfunsafe_buffer() + ret.length(), toRead);
            read += readCount;
            ret.zfunsafe_length(ret.length() + readCount);
            if(readCount < toRead) {
                ret.zfunsafe_buffer()[ret.length()] = '\0';
                break;
            }
        } while(zftrue);
    }
    return read;
}
#undef _ZFP_ZFInputRead_blockSize

// ============================================================
zfindex ZFInputReadLine(
        ZF_IN_OUT const ZFOutput &output
        , ZF_IN_OUT const ZFInput &input
        ) {
    if(!output || !input) {
        return zfindexMax();
    }
    zfindex count = 0;
    zfchar c[9];
    do {
        zfindex readCount = ZFInputReadChar(c, input);
        if(readCount == 0) {
            return count == 0 ? zfindexMax() : count;
        }
        else {
            if(*c == '\n') {
                return count;
            }
            if(output.execute(c, readCount) < readCount) {
                return count == 0 ? zfindexMax() : count;
            }
            ++count;
        }
    } while(zftrue);
}
zfindex ZFInputReadLine(
        ZF_IN_OUT zfstring &output
        , ZF_IN_OUT const ZFInput &input
        ) {
    if(!input) {
        return zfindexMax();
    }
    zfindex count = 0;
    zfchar c[9];
    do {
        zfindex readCount = ZFInputReadChar(c, input);
        if(readCount == 0) {
            return count == 0 ? zfindexMax() : count;
        }
        else {
            if(*c == '\n') {
                return count;
            }
            output.append(c, readCount);
            ++count;
        }
    } while(zftrue);
}

// ============================================================
ZFOutput ZFOutputChain(
        ZF_IN const ZFOutput &o0
        , ZF_IN const ZFOutput &o1
        , ZF_IN_OPT const ZFOutput &o2 /* = zfnull */
        , ZF_IN_OPT const ZFOutput &o3 /* = zfnull */
        , ZF_IN_OPT const ZFOutput &o4 /* = zfnull */
        , ZF_IN_OPT const ZFOutput &o5 /* = zfnull */
        , ZF_IN_OPT const ZFOutput &o6 /* = zfnull */
        , ZF_IN_OPT const ZFOutput &o7 /* = zfnull */
        ) {
    ZFCoreArray<ZFOutput> o;
    if(o0) {o.add(o0);}
    if(o1) {o.add(o1);}
    if(o2) {o.add(o2);}
    if(o3) {o.add(o3);}
    if(o4) {o.add(o4);}
    if(o5) {o.add(o5);}
    if(o6) {o.add(o6);}
    if(o7) {o.add(o7);}
    return ZFOutputChain(o);
}
zfclass _ZFP_I_ZFOutputChain : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputChain, ZFObject)
    ZFCoreArray<ZFOutput> o;
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_OUT(const void *, src)
            , ZFMP_IN_OPT(zfindex, count, zfindexMax())
            )
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputChain, zfindex, onOutput
        , ZFMP_OUT(const void *, src)
        , ZFMP_IN_OPT(zfindex, count, zfindexMax())
        ) {
    for(zfindex i = 0; i < o.count(); ++i) {
        o[i].execute(src, count);
    }
    return count;
}
ZFOutput ZFOutputChain(
        ZF_IN const ZFCoreArray<ZFOutput> &o
        ) {
    if(o.isEmpty()) {return zfnull;}
    if(o.count() == 1) {return o[0];}
    zfobj<_ZFP_I_ZFOutputChain> owner;
    owner->o.addFrom(o);
    ZFOutput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFOutputChain, onOutput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, ZFInputReadChar
        , ZFMP_IN_OUT(zfstring &, buf)
        , ZFMP_IN_OUT(const ZFInput &, input)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, ZFInputRead
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN_OUT(const ZFInput &, input)
        , ZFMP_IN_OPT(zfindex, size, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfindex, ZFInputRead
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN_OUT(const ZFInput &, input)
        , ZFMP_IN_OPT(zfindex, size, zfindexMax())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, ZFInputReadLine
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN_OUT(const ZFInput &, input)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfindex, ZFInputReadLine
        , ZFMP_IN_OUT(zfstring &, output)
        , ZFMP_IN_OUT(const ZFInput &, input)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFOutputRepeat
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const zfchar *, token)
        , ZFMP_IN(zfindex, count)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(ZFOutput, ZFOutputChain
        , ZFMP_IN(const ZFOutput &, o0)
        , ZFMP_IN(const ZFOutput &, o1)
        , ZFMP_IN_OPT(const ZFOutput &, o2, zfnull)
        , ZFMP_IN_OPT(const ZFOutput &, o3, zfnull)
        , ZFMP_IN_OPT(const ZFOutput &, o4, zfnull)
        , ZFMP_IN_OPT(const ZFOutput &, o5, zfnull)
        , ZFMP_IN_OPT(const ZFOutput &, o6, zfnull)
        , ZFMP_IN_OPT(const ZFOutput &, o7, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFOutput, ZFOutputChain
        , ZFMP_IN(const ZFCoreArray<ZFOutput> &, o)
        )

ZF_NAMESPACE_GLOBAL_END
#endif


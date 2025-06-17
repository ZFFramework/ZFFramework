#include "ZFCoreArg.h"
#include "ZFCoreStringUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFCoreArgSplit(
        ZF_IN_OUT ZFCoreArray<zfstring> &result
        , ZF_IN const zfchar *cmdLine
        ) {
    if(cmdLine == zfnull) {return;}

    const zfchar *p = cmdLine;
    zfbool startFlag = zffalse;
    zfbool isInQuote = zffalse;
    zfstring arg;
    while(*p != '\0') {
        if(!startFlag) {
            while(*p != '\0' && *p == ' ') {
                ++p;
            }
            if(*p == '\0') {
                break;
            }
            startFlag = zftrue;
        } // if(!startFlag)

        if(*p == '\\') {
            ++p;
            switch(*p) {
                case ' ':
                case '\\':
                case '"':
                    arg += *p;
                    break;
                default:
                    --p;
                    arg += '\\';
                    break;
            } // switch(*p)
        }
        else if(*p == '"') {
            if(!isInQuote) {
                isInQuote = zftrue;
                if(!arg.isEmpty()) {
                    result.add(arg);
                    arg.removeAll();
                    startFlag = zffalse;
                }
            }
            else {
                isInQuote = zffalse;
                result.add(arg);
                arg.removeAll();
                startFlag = zffalse;
            }
        }
        else if(*p == ' ') {
            if(isInQuote) {
                arg += ' ';
            }
            else {
                result.add(arg);
                arg.removeAll();
                startFlag = zffalse;
            }
        }
        else {
            zfcharAppendAndMoveNext(arg, p);
            continue;
        }

        ++p;
    }

    if(!arg.isEmpty()) {
        result.add(arg);
    }
}
ZFCoreArray<zfstring> ZFCoreArgSplit(ZF_IN const zfchar *cmdLine) {
    ZFCoreArray<zfstring> result;
    ZFCoreArgSplit(result, cmdLine);
    return result;
}

void ZFCoreArgMerge(
        ZF_OUT zfstring &result
        , ZF_IN const ZFCoreArray<zfstring> &argList
        ) {
    for(zfindex i = 0; i < argList.count(); ++i) {
        const zfstring &s = argList.get(i);

        if(i != 0) {
            result += ' ';
        }

        zfbool containSpace = (zfstringFind(s, " ") != zfindexMax());
        if(containSpace) {
            result += '\"';
        }

        const zfchar *p = s;
        while(*p != '\0') {
            if(*p == '\\') {
                result += "\\\\";
            }
            else if(*p == '\"') {
                result += "\\\"";
            }
            else {
                zfcharAppendAndMoveNext(result, p);
                continue;
            }
            ++p;
        }

        if(containSpace) {
            result += '\"';
        }
    }
}
zfstring ZFCoreArgMerge(ZF_IN const ZFCoreArray<zfstring> &argList) {
    zfstring result;
    ZFCoreArgMerge(result, argList);
    return result;
}

ZF_NAMESPACE_GLOBAL_END


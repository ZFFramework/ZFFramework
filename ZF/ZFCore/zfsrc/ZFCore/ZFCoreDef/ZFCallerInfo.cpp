#include "ZFCallerInfo.h"
#include "ZFCoreStringUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZF_CALLER_FILE_TO_NAME(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *filePath
        ) {
    if(filePath == zfnull) {
        return;
    }
    const zfchar *p = filePath + zfslen(filePath);
    while(p > filePath) {
        --p;
        if(*p == '/' || *p == '\\') {
            ++p;
            break;
        }
    }
    ret += p;
}
zfstring _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN const zfchar *filePath) {
    zfstring ret;
    _ZFP_ZF_CALLER_FILE_TO_NAME(ret, filePath);
    return ret;
}

const ZFCallerInfo &_ZFP_ZFCallerInfoEmpty(void) {
    static ZFCallerInfo d;
    return d;
}

// ============================================================
static zfchar *_ZFP_ZFCallerInfo_copy(ZF_IN const zfchar *s) {
    zfindex len = zfslen(s);
    zfchar *ret = (zfchar *)zfmalloc(sizeof(zfchar) * (len + 1));
    zfmemcpy(ret, s, sizeof(zfchar) * (len + 1));
    return ret;
}

ZFCallerInfo::ZFCallerInfo(void)
: _callerFile(zfnull)
, _callerFunc(zfnull)
, _callerLine(0)
, _needFree(zffalse)
{
}
ZFCallerInfo::ZFCallerInfo(
        ZF_IN const zfchar *callerFile
        , ZF_IN const zfchar *callerFunc
        , ZF_IN zfuint callerLine
        )
{
    if(!zfstringIsEmpty(callerFile) && !zfstringIsEmpty(callerFunc)) {
        _callerFile = _ZFP_ZFCallerInfo_copy(callerFile);
        _callerFunc = _ZFP_ZFCallerInfo_copy(callerFunc);
        _callerLine = callerLine;
        _needFree = zftrue;
    }
    else {
        _callerFile = zfnull;
        _callerFunc = zfnull;
        _callerLine = callerLine;
        _needFree = zffalse;
    }
}
ZFCallerInfo::ZFCallerInfo(ZF_IN const ZFCallerInfo &ref)
{
    if(ref._needFree) {
        _callerFile = _ZFP_ZFCallerInfo_copy(ref._callerFile);
        _callerFunc = _ZFP_ZFCallerInfo_copy(ref._callerFunc);
        _callerLine = ref._callerLine;
        _needFree = zftrue;
    }
    else {
        _callerFile = ref._callerFile;
        _callerFunc = ref._callerFunc;
        _callerLine = ref._callerLine;
        _needFree = zffalse;
    }
}
ZFCallerInfo::~ZFCallerInfo(void) {
    if(_needFree) {
        zffree(const_cast<zfchar *>(_callerFile));
        zffree(const_cast<zfchar *>(_callerFunc));
    }
}
ZFCallerInfo &ZFCallerInfo::operator = (ZF_IN const ZFCallerInfo &ref) {
    if(this != &ref) {
        if(_needFree) {
            zffree(const_cast<zfchar *>(_callerFile));
            zffree(const_cast<zfchar *>(_callerFunc));
        }
        if(ref._needFree) {
            _callerFile = _ZFP_ZFCallerInfo_copy(ref._callerFile);
            _callerFunc = _ZFP_ZFCallerInfo_copy(ref._callerFunc);
            _callerLine = ref._callerLine;
            _needFree = zftrue;
        }
        else {
            _callerFile = ref._callerFile;
            _callerFunc = ref._callerFunc;
            _callerLine = ref._callerLine;
            _needFree = zffalse;
        }
    }
    return *this;
}
zfbool ZFCallerInfo::operator == (ZF_IN const ZFCallerInfo &ref) const {
    return (zfstringIsEqual(this->callerFile(), ref.callerFile())
        && zfstringIsEqual(this->callerFunc(), ref.callerFunc())
        && this->callerLine() == ref.callerLine());
}

void ZFCallerInfo::callerInfo(
        ZF_IN const zfchar *callerFile
        , ZF_IN const zfchar *callerFunc
        , ZF_IN zfuint callerLine
        ) {
    if(_needFree) {
        zffree(const_cast<zfchar *>(_callerFile));
        zffree(const_cast<zfchar *>(_callerFunc));
    }
    _callerFile = _ZFP_ZFCallerInfo_copy(callerFile);
    _callerFunc = _ZFP_ZFCallerInfo_copy(callerFunc);
    _callerLine = callerLine;
    _needFree = zftrue;
}
zfbool ZFCallerInfo::callerInfoT(ZF_IN_OUT zfstring &ret) const {
    if(this->callerFile() != zfnull) {
        ret += "[";
        ZF_CALLER_FILE_TO_NAME_REF(ret, this->callerFile());
        ret += " ";
        ret += this->callerFunc();
        ret += " (";
        zfsFromIntT(ret, this->callerLine());
        ret += ")]";
        return zftrue;
    }
    else {
        return zffalse;
    }
}

ZF_NAMESPACE_GLOBAL_END


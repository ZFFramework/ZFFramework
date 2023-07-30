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
ZFCallerInfo::ZFCallerInfo(void)
: _callerFileH(zfnull)
, _callerFuncH(zfnull)
, _callerFile(zfnull)
, _callerFunc(zfnull)
, _callerLine(0)
{
}
ZFCallerInfo::ZFCallerInfo(
        ZF_IN const zfchar *callerFile
        , ZF_IN const zfchar *callerFunc
        , ZF_IN zfuint callerLine
        )
: _callerFileH(zfsCopy(callerFile))
, _callerFuncH(zfsCopy(callerFunc))
, _callerFile(_callerFileH)
, _callerFunc(_callerFuncH)
, _callerLine(callerLine)
{
}
ZFCallerInfo::ZFCallerInfo(ZF_IN const ZFCallerInfo &ref)
: _callerFileH(zfsCopy(ref._callerFileH))
, _callerFuncH(zfsCopy(ref._callerFuncH))
, _callerFile(_callerFileH)
, _callerFunc(_callerFuncH)
, _callerLine(ref._callerLine)
{
}
ZFCallerInfo::~ZFCallerInfo(void) {
    zfsChange(this->_callerFileH, zfnull);
    zfsChange(this->_callerFuncH, zfnull);
}
ZFCallerInfo &ZFCallerInfo::operator = (ZF_IN const ZFCallerInfo &ref) {
    zfsChange(this->_callerFileH, ref._callerFileH);
    zfsChange(this->_callerFuncH, ref._callerFuncH);
    this->_callerFile = this->_callerFileH;
    this->_callerFunc = this->_callerFuncH;
    this->_callerLine = ref._callerLine;
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
    zfsChange(this->_callerFileH, callerFile);
    zfsChange(this->_callerFuncH, callerFunc);
    this->_callerFile = this->_callerFileH;
    this->_callerFunc = this->_callerFuncH;
    this->_callerLine = callerLine;
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


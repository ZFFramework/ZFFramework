#include "ZFPathInfo.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFPathInfoPrivate {
public:
    zfuint refCount;
    zfstring pathType;
    zfstring pathData;
public:
    _ZFP_ZFPathInfoPrivate(void)
    : refCount(1)
    , pathType()
    , pathData()
    {
    }
    _ZFP_ZFPathInfoPrivate(
            ZF_IN const zfstring &pathType
            , ZF_IN const zfstring &pathData
            )
    : refCount(1)
    , pathType(pathType)
    , pathData(pathData)
    {
    }
public:
    static void prepareWrite(ZF_IN_OUT _ZFP_ZFPathInfoPrivate *&d) {
        if(d == zfnull) {
            d = zfpoolNew(_ZFP_ZFPathInfoPrivate);
        }
        else if(d->refCount > 1) {
            _ZFP_ZFPathInfoPrivate *dTmp = d;
            --(dTmp->refCount);
            d = zfpoolNew(_ZFP_ZFPathInfoPrivate);
            d->pathType = dTmp->pathType;
            d->pathData = dTmp->pathData;
        }
    }
};

ZFPathInfo &ZFPathInfo::pathType(ZF_IN const zfstring &pathType) {
    _ZFP_ZFPathInfoPrivate::prepareWrite(d);
    d->pathType = pathType;
    return *this;
}
const zfstring &ZFPathInfo::pathType(void) const {
    return d ? d->pathType : zfstring::Empty();
}

ZFPathInfo &ZFPathInfo::pathData(ZF_IN const zfstring &pathData) {
    _ZFP_ZFPathInfoPrivate::prepareWrite(d);
    d->pathData = pathData;
    return *this;
}
const zfstring &ZFPathInfo::pathData(void) const {
    return d ? d->pathData : zfstring::Empty();
}

zfbool ZFPathInfo::isEmpty(void) const {
    return d == zfnull || (d->pathType.isEmpty() && d->pathData.isEmpty());
}
void ZFPathInfo::removeAll(void) {
    if(d) {
        if(d->refCount == 1) {
            d->pathType.removeAll();
            d->pathData.removeAll();
        }
        else {
            --(d->refCount);
            d = zfnull;
        }
    }
}

ZFPathInfo::ZFPathInfo(void) : d(zfnull) {
}
ZFPathInfo::ZFPathInfo(ZF_IN const zfnullT &dummy) : d(zfnull) {
}
ZFPathInfo::ZFPathInfo(ZF_IN const ZFPathInfo &ref) : d(ref.d) {
    if(d) {
        ++(d->refCount);
    }
}
ZFPathInfo::ZFPathInfo(ZF_IN const zfchar *pathInfoString) : d(zfnull) {
    ZFPathInfoFromStringT(*this, pathInfoString);
}
ZFPathInfo::ZFPathInfo(ZF_IN const zfstring &pathType, ZF_IN const zfstring &pathData) : d(zfpoolNew(_ZFP_ZFPathInfoPrivate, pathType, pathData)) {
}
ZFPathInfo::~ZFPathInfo(void) {
    if(d) {
        --(d->refCount);
        if(d->refCount == 0) {
            zfpoolDelete(d);
        }
    }
}
ZFPathInfo &ZFPathInfo::operator = (ZF_IN const ZFPathInfo &ref) {
    _ZFP_ZFPathInfoPrivate *dTmp = d;
    d = ref.d;
    if(d) {
        ++(d->refCount);
    }
    if(dTmp) {
        --(dTmp->refCount);
        if(dTmp->refCount == 0) {
            zfpoolDelete(dTmp);
        }
    }
    return *this;
}
ZFPathInfo &ZFPathInfo::operator = (ZF_IN const zfnullT &dummy) {
    this->removeAll();
    return *this;
}
zfbool ZFPathInfo::operator == (ZF_IN const ZFPathInfo &ref) const {
    if(d == ref.d) {
        return zftrue;
    }
    else if(d) {
        if(ref.d) {
            return d->pathType == ref.d->pathType && d->pathData == ref.d->pathData;
        }
        else {
            return d->pathType.isEmpty() && d->pathData.isEmpty();
        }
    }
    else {
        if(ref.d) {
            return ref.d->pathType.isEmpty() && ref.d->pathData.isEmpty();
        }
        else {
            return zftrue;
        }
    }
}

ZF_NAMESPACE_GLOBAL_END


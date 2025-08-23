#include "ZFIO_common.h"
#include "ZFIO_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFIO_DEFINE_COMMON(pathType, basePath) \
    zfclass _ZFP_I_ZFIOImpl_##pathType : zfextend _ZFP_I_ZFIOImpl_file { \
        ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_##pathType, _ZFP_I_ZFIOImpl_file) \
    protected: \
        virtual zfstring pathConvert(ZF_IN const zfstring &pathData) { \
            return zfstr("%s/%s", basePath, pathData); \
        } \
    public: \
        zfoverride \
        virtual zfstring pathType(void) {return ZFPathType_##pathType();} \
    }; \
    ZFIO_DEFINE(pathType, _ZFP_I_ZFIOImpl_##pathType)

_ZFP_ZFIO_DEFINE_COMMON(modulePath, ZFPathForModule())
_ZFP_ZFIO_DEFINE_COMMON(settingPath, ZFPathForSetting())
_ZFP_ZFIO_DEFINE_COMMON(storagePath, ZFPathForStorage())
_ZFP_ZFIO_DEFINE_COMMON(storageSharedPath, ZFPathForStorageShared())
_ZFP_ZFIO_DEFINE_COMMON(cachePath, ZFPathForCache())

// ============================================================
zfclass _ZFP_I_ZFIOToken_text : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_text, ZFIOToken)
public:
    zfstring _pathData;
    zfindex _pos; // zfindexMax when closed
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _pos = zfindexMax();
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_text();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return _pathData;
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return v_ZFIOOpenOption::e_Read;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(_pos == zfindexMax()) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        _pos = zfindexMax();
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(_pos >= _pathData.length() || buf == zfnull) {
            return 0;
        }
        maxByteSize = zfmMin<zfindex>(maxByteSize, _pathData.length() - _pos);
        zfmemcpy(buf, _pathData.cString() + _pos, maxByteSize);
        _pos += maxByteSize;
        return maxByteSize;
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {return zfindexMax();}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {return _pos = ZFIOCallbackCalcSeek(0, _pathData.length(), _pos, byteSize, seekPos);}
    zfoverride
    virtual zfindex ioTell(void) {return _pos;}
    zfoverride
    virtual zfindex ioSize(void) {return _pathData.length();}
};
zfclass _ZFP_I_ZFIOImpl_text : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_text, ZFIOImpl)
public:
    zfoverride
    virtual zfstring pathType(void) {return ZFPathType_text();}

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {return zffalse;}
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {return zffalse;}
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {return zffalse;}
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {}
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        if(ZFBitTest(flags, 0
                    | v_ZFIOOpenOption::e_Write
                    | v_ZFIOOpenOption::e_Modify
                    )) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_text> ioToken;
        ioToken->_pathData = pathData;
        return ioToken;
    }
};
ZFIO_DEFINE(text, _ZFP_I_ZFIOImpl_text)

// ============================================================
zfclass _ZFP_I_ZFIOToken_bin : zfextend ZFIOToken {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOToken_bin, ZFIOToken)
public:
    zfstring _bin; // decoded binary data
    zfindex _pos; // zfindexMax when closed
protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        _pos = zfindexMax();
    }
public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_bin();
    }
    zfoverride
    virtual zfstring pathData(void) {
        zfstring ret;
        ZFCoreDataEncode(ret, _bin, _bin.length());
        return ret;
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return v_ZFIOOpenOption::e_Read;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(_pos == zfindexMax()) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        _bin = zfnull;
        _pos = zfindexMax();
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(_pos >= _bin.length() || buf == zfnull) {
            return 0;
        }
        maxByteSize = zfmMin<zfindex>(maxByteSize, _bin.length() - _pos);
        zfmemcpy(buf, _bin.cString() + _pos, maxByteSize);
        _pos += maxByteSize;
        return maxByteSize;
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {return zfindexMax();}
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {return _pos = ZFIOCallbackCalcSeek(0, _bin.length(), _pos, byteSize, seekPos);}
    zfoverride
    virtual zfindex ioTell(void) {return _pos;}
    zfoverride
    virtual zfindex ioSize(void) {return _bin.length();}
};
zfclass _ZFP_I_ZFIOImpl_bin : zfextend ZFIOImpl {
    ZFOBJECT_DECLARE(_ZFP_I_ZFIOImpl_bin, ZFIOImpl)
public:
    zfoverride
    virtual zfstring pathType(void) {return ZFPathType_bin();}

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) {return zffalse;}
    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) {return zffalse;}
    zfoverride
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataFrom
            , ZF_IN const zfstring &pathDataTo
            , ZF_IN_OPT zfbool isForce = zftrue
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) {return zffalse;}
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {return zffalse;}
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {}
    zfoverride
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) {
        if(ZFBitTest(flags, 0
                    | v_ZFIOOpenOption::e_Write
                    | v_ZFIOOpenOption::e_Modify
                    )) {
            return zfnull;
        }
        zfobj<_ZFP_I_ZFIOToken_bin> ioToken;
        ZFCoreDataDecode(ioToken->_bin, pathData, pathData.length());
        return ioToken;
    }
};
ZFIO_DEFINE(bin, _ZFP_I_ZFIOImpl_bin)

// ============================================================
zfclass _ZFP_I_ZFIOToken_ZFIOOpenCache : zfextend ZFIOToken {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFIOToken_ZFIOOpenCache, ZFIOToken)
public:
    zfautoT<ZFIOToken> _impl;
    ZFIOOpenOptionFlags _flags;
    zfbool _autoRemove;
protected:
    _ZFP_I_ZFIOToken_ZFIOOpenCache(void) : _impl(), _flags(v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write), _autoRemove(zftrue) {}
public:
    zfoverride
    virtual zfstring pathType(void) {
        if(_impl) {
            return _impl->pathType();
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual zfstring pathData(void) {
        if(_impl) {
            return _impl->pathData();
        }
        else {
            return zfnull;
        }
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return _flags;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_impl) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        if(_autoRemove) {
            ZFPathInfo tmp = _impl->pathInfo();
            _impl->ioClose();
            _impl = zfnull;
            ZFIORemove(tmp);
        }
        else {
            _impl->ioClose();
            _impl = zfnull;
        }
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return zftrue;
    }
    zfoverride
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) {
        if(_impl) {
            return _impl->ioRead(buf, maxByteSize);
        }
        else {
            return 0;
        }
    }
    zfoverride
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) {
        if(_impl) {
            return _impl->ioWrite(src, maxByteSize);
        }
        else {
            return 0;
        }
    }
    zfoverride
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) {
        if(_impl) {
            return _impl->ioSeek(byteSize, seekPos);
        }
        else {
            return zffalse;
        }
    }
    zfoverride
    virtual zfindex ioTell(void) {
        if(_impl) {
            return _impl->ioTell();
        }
        else {
            return zfindexMax();
        }
    }
    zfoverride
    virtual zfindex ioSize(void) {
        if(_impl) {
            return _impl->ioSize();
        }
        else {
            return zfindexMax();
        }
    }
};
ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFIOToken>, ZFIOOpenCache
        , ZFMP_IN_OPT(const ZFPathInfo &, pathInfo, zfnull)
        , ZFMP_IN_OPT(ZFIOOpenOptionFlags, flags, v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write)
        , ZFMP_IN_OPT(zfbool, autoRemove, zftrue)
        ) {
    zfautoT<ZFIOToken> impl = ZFIOOpen(pathInfo ? pathInfo : ZFIO_cachePathGen(), flags);
    if(!impl) {
        return zfnull;
    }

    zfobj<_ZFP_I_ZFIOToken_ZFIOOpenCache> ret;
    ret->_impl = impl;
    ret->_flags = flags;
    ret->_autoRemove = autoRemove;
    return ret;
}
ZFMETHOD_FUNC_DEFINE_0(ZFPathInfo, ZFIO_cachePathGen) {
    static zfindex _counter = 0;
    return ZFPathInfo(ZFPathType_cachePath(), zfstr(
                "ZFIOBuffer/%s_%s"
                , ++_counter
                , zfmRand()
                ));
}

ZF_NAMESPACE_GLOBAL_END


#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFCompress.h"

#include "ZFAlgorithm.h"

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "../_repo/miniz/miniz.h"

#include "ZFCore/ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFCompressToken_default : zfextend ZFCompressToken {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFCompressToken_default, ZFCompressToken)
private:
    zfautoT<ZFIOToken> _refIOToken;
    ZFPathInfo _refPathInfo;
    ZFIOOpenOptionFlags _refOpenFlags;
    mz_zip_archive _impl; // readonly
    mz_uint _compressLevel;

    // for modify
    zfbool _modified;
    zfbool _itemLoaded;
    zfstlhashmap<zfstring, zfbool> _itemToKeep; // <itemPath, isDir>, items to keep from source, also used for ioFindFirst
    zfstlhashmap<zfstring, ZFPathInfo> _itemToAdd; // <itemPath, pathInfo(null for dir)>, items to add

protected:
    _ZFP_I_ZFCompressToken_default(void)
    : _refIOToken()
    , _refPathInfo()
    , _refOpenFlags(v_ZFIOOpenOption::e_Read)
    , _impl()
    , _compressLevel(MZ_DEFAULT_COMPRESSION)
    , _modified(zffalse)
    , _itemLoaded(zffalse)
    , _itemToKeep()
    , _itemToAdd()
    {
    }

public:
    zfbool open(
            ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN ZFCompressLevel compressLevel
            ) {
        _refIOToken = ZFIOOpen(pathInfo, flags);
        if(!_refIOToken) {
            return zffalse;
        }
        zfindex fileSize = _refIOToken->ioSize();
        if(fileSize == zfindexMax()) {
            _refIOToken = zfnull;
            return zffalse;
        }

        _refPathInfo = pathInfo;
        _refOpenFlags = flags;

        switch(compressLevel) {
            case v_ZFCompressLevel::e_NoCompress:
                _compressLevel = MZ_NO_COMPRESSION;
                break;
            case v_ZFCompressLevel::e_BestSpeed:
                _compressLevel = MZ_BEST_SPEED;
                break;
            case v_ZFCompressLevel::e_GoodSpeed:
                _compressLevel = 3;
                break;
            case v_ZFCompressLevel::e_Normal:
                _compressLevel = MZ_DEFAULT_LEVEL;
                break;
            case v_ZFCompressLevel::e_GoodCompress:
                _compressLevel = 7;
                break;
            case v_ZFCompressLevel::e_BestCompress:
                _compressLevel = MZ_BEST_COMPRESSION;
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                break;
        }

        zfmemset(&_impl, 0, sizeof(mz_zip_archive));
        _impl.m_pIO_opaque = (ZFIOToken *)_refIOToken;
        _impl.m_pRead = zfself::_implRead_ZFIOToken;
        _impl.m_pWrite = zfself::_implWrite_ZFIOToken;
        if(ZFBitTest(flags, 0
                    | v_ZFIOOpenOption::e_Modify
                    | v_ZFIOOpenOption::e_Read
                    )) {
            if(fileSize != 0 && !mz_zip_reader_init(&_impl, (mz_uint64)fileSize, MZ_ZIP_FLAG_CASE_SENSITIVE)) {
                this->ioClose();
                return zffalse;
            }
        }
        return zftrue;
    }

private:
    static size_t _implRead_ZFIOToken(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n) {
        ZFIOToken *p = (ZFIOToken *)pOpaque;
        p->ioSeek((zfindex)file_ofs);
        return (size_t)p->ioRead(pBuf, (zfindex)n);
    }
    static size_t _implWrite_ZFIOToken(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n) {
        ZFIOToken *p = (ZFIOToken *)pOpaque;
        p->ioSeek((zfindex)file_ofs);
        return (size_t)p->ioWrite(pBuf, (zfindex)n);
    }
    static size_t _implRead_ZFInput(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n) {
        ZFInput &input = *(ZFInput *)pOpaque;
        input.ioSeek(file_ofs);
        return (size_t)input.execute(pBuf, (zfindex)n);
    }
    static size_t _implWrite_ZFOutput(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n) {
        ZFOutput &output = *(ZFOutput *)pOpaque;
        output.ioSeek((zfindex)file_ofs);
        return (size_t)output.execute(pBuf, (zfindex)n);
    }

private:
    static zfstring _itemPathFormat(ZF_IN const zfstring &itemPath) {
        const zfchar *p = itemPath;
        while(*p == '/') {++p;}
        return ZFPathFormat(p, itemPath.cString() + itemPath.length() - p);
    }
    static zfbool _isSameOrChildOf(ZF_IN const zfstring &maybeChild, ZF_IN const zfstring &maybeParent) {
        return zfstringBeginWith(maybeChild, maybeParent)
            && (maybeChild.length() == maybeParent.length()
                    || maybeChild[maybeParent.length()] == '/'
                    );
    }
    static zfbool _isChildOf(ZF_IN const zfstring &maybeChild, ZF_IN const zfstring &maybeParent) {
        return zfstringBeginWith(maybeChild, maybeParent)
            && maybeChild[maybeParent.length()] == '/'
            ;
    }
    static zfbool _isDirectChildOf(ZF_IN const zfstring &maybeChild, ZF_IN const zfstring &maybeParent) {
        return zfstringBeginWith(maybeChild, maybeParent)
            && maybeChild[maybeParent.length()] == '/'
            && zfstringFind(maybeChild.cString() + maybeParent.length() + 1, maybeChild.length() - maybeParent.length() - 1, "/") == zfindexMax()
            ;
    }

private:
    void _itemLoadCheck(void) {
        if(_itemLoaded) {
            return;
        }
        _itemLoaded = zftrue;
        if(_impl.m_zip_mode != MZ_ZIP_MODE_INVALID) {
            for(zfindex i = 0, itemCount = this->itemCount(); i < itemCount; ++i) {
                _itemToKeep[this->itemPath(i)] = this->ioIsDirAt(i);
            }
        }
    }

public:
    zfoverride
    virtual zfstring pathType(void) {
        return ZFPathType_compress();
    }
    zfoverride
    virtual zfstring pathData(void) {
        return ZFPathInfoChainEncode(_refPathInfo, "");
    }
    zfoverride
    virtual ZFIOOpenOptionFlags ioFlags(void) {
        return _refOpenFlags;
    }

private:
    zfbool _ioCloseImpl(void) {
        zfbool ret = zffalse;;
        ZFPathInfo cachePath;
        zfautoT<ZFIOToken> cacheIO;
        if(_modified) {
            cachePath.pathType(_refPathInfo.pathType());

            zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(_refPathInfo.pathType());
            ZFCoreAssert(ioImpl);

            zfstring cachePathData;
            ioImpl->ioToParent(cachePathData, _refPathInfo.pathData());
            zfstring fileName;
            ioImpl->ioToFileName(fileName, _refPathInfo.pathData());
            ioImpl->ioToChild(cachePathData, cachePathData, zfstr("_ZFCompress_%s", fileName));
            cachePath.pathData(cachePathData);
        }

        mz_zip_archive implCache;
        zfmemset(&implCache, 0, sizeof(mz_zip_archive));

        do {
            if(!_modified) {
                ret = zftrue;
                break;
            }

            cacheIO = ZFIOOpen(cachePath, v_ZFIOOpenOption::e_Write);
            if(!cacheIO) {
                break;
            }

            implCache.m_pIO_opaque = (ZFIOToken *)cacheIO;
            implCache.m_pRead = zfself::_implRead_ZFIOToken;
            implCache.m_pWrite = zfself::_implWrite_ZFIOToken;
            if(!mz_zip_writer_init_v2(&implCache, 0, MZ_ZIP_FLAG_CASE_SENSITIVE)) {
                break;
            }

            zfstlhashmap<zfstring, zfbool> dirToCreate;

            ret = zftrue;
            for(zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.begin(); itExist != _itemToKeep.end(); ++itExist) {
                if(itExist->second) {
                    dirToCreate[itExist->first] = zftrue;
                    continue;
                }

                zfobj<ZFIOBuffer> buf;
                ZFOutput bufOutput = buf->output();
                zfbool success = mz_zip_reader_extract_file_to_callback(
                        &_impl
                        , itExist->first
                        , zfself::_implWrite_ZFOutput
                        , &bufOutput
                        , MZ_ZIP_FLAG_CASE_SENSITIVE
                        );
                if(!success) {
                    ret = zffalse;
                    break;
                }
                buf->input().ioSeek(0);
                ZFInput bufInput = buf->input();
                zfindex fileSize = bufInput.ioSize();
                if(fileSize == zfindexMax()) {
                    ret = zffalse;
                    break;
                }
                success = mz_zip_writer_add_read_buf_callback(
                        &implCache
                        , itExist->first
                        , zfself::_implRead_ZFInput
                        , &bufInput
                        , (mz_uint64)fileSize
                        , zfnull
                        , zfnull
                        , 0
                        , _compressLevel
                        , 0
                        , zfnull
                        , 0
                        , zfnull
                        , 0
                        );
                if(!success) {
                    ret = zffalse;
                    break;
                }
            }
            if(!ret) {
                break;
            }

            for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.begin(); itMod != _itemToAdd.end(); ++itMod) {
                if(!(itMod->second)) {
                    dirToCreate[itMod->first] = zftrue;
                    continue;
                }

                ZFInput bufInput = ZFInputForPathInfo(itMod->second);
                zfindex fileSize = bufInput.ioSize();
                if(fileSize == zfindexMax()) {
                    ret = zffalse;
                    break;
                }
                zfbool success = mz_zip_writer_add_read_buf_callback(
                        &implCache
                        , itMod->first
                        , zfself::_implRead_ZFInput
                        , &bufInput
                        , (mz_uint64)fileSize
                        , zfnull
                        , zfnull
                        , 0
                        , _compressLevel
                        , 0
                        , zfnull
                        , 0
                        , zfnull
                        , 0
                        );
                if(!success) {
                    ret = zffalse;
                    break;
                }
            }

            for(zfstlhashmap<zfstring, zfbool>::iterator it = dirToCreate.begin(); it != dirToCreate.end(); ++it) {
                zfbool hasChild = zffalse;
                for(zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.begin(); itExist != _itemToKeep.end(); ++itExist) {
                    if(_isChildOf(itExist->first, it->first)) {
                        hasChild = zftrue;
                        break;
                    }
                }
                if(!hasChild) {
                    for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.begin(); itMod != _itemToAdd.end(); ++itMod) {
                        if(_isChildOf(itMod->first, it->first)) {
                            hasChild = zftrue;
                            break;
                        }
                    }
                }
                if(!hasChild) {
                    ret = mz_zip_writer_add_mem(&implCache, zfstr("%s/", it->first), zfnull, 0, 0);
                    if(!ret) {
                        break;
                    }
                }
            }
        } while(zffalse);

        if(_impl.m_zip_mode != MZ_ZIP_MODE_INVALID) {
            mz_zip_end(&_impl);
        }
        _refIOToken->ioClose();
        _refIOToken = zfnull;

        if(implCache.m_zip_mode != MZ_ZIP_MODE_INVALID) {
            ret = ret && mz_zip_writer_finalize_archive(&implCache);
            ret = ret && mz_zip_end(&implCache);
        }

        if(cacheIO) {
            cacheIO = zfnull;
            ret = ret && ZFIOMove(_refPathInfo.pathData(), cachePath);
        }

        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.begin(); itMod != _itemToAdd.end(); ++itMod) {
            if(itMod->second) {
                ZFIORemove(itMod->second);
            }
        }
        _refPathInfo = zfnull;
        _modified = zffalse;
        _itemLoaded = zffalse;
        _itemToKeep.clear();
        _itemToAdd.clear();
        return ret;
    }
public:
    zfoverride
    virtual zfbool ioClose(void) {
        if(!_refIOToken) {
            return zftrue;
        }
        this->observerNotify(zfself::E_IOCloseOnPrepare());
        zfbool ret = _ioCloseImpl();
        this->observerNotify(zfself::E_IOCloseOnFinish());
        return ret;
    }
    zfoverride
    virtual zfbool ioRead(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const zfstring &itemPath
            ) {
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            return zffalse;
        }

        if(!_modified) {
            if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
                return zffalse;
            }
            ZFOutput outputTmp = output;
            return mz_zip_reader_extract_file_to_callback(
                    &_impl
                    , itemPathTmp
                    , zfself::_implWrite_ZFOutput
                    , &outputTmp
                    , MZ_ZIP_FLAG_CASE_SENSITIVE
                    );
        }
        zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.find(itemPathTmp);
        if(itExist != _itemToKeep.end()) {
            if(itExist->second) {
                return zffalse;
            }
            if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
                return zffalse;
            }
            ZFOutput outputTmp = output;
            return mz_zip_reader_extract_file_to_callback(
                    &_impl
                    , itemPathTmp
                    , zfself::_implWrite_ZFOutput
                    , &outputTmp
                    , MZ_ZIP_FLAG_CASE_SENSITIVE
                    );
        }
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.find(itemPathTmp);
        if(itMod != _itemToAdd.end()) {
            return ZFInputRead(output, ZFInputForPathInfo(itMod->second)) != zfindexMax();
        }
        return zffalse;
    }
    zfoverride
    virtual zfbool ioWrite(
            ZF_IN const zfstring &itemPath
            , ZF_IN const ZFInput &input
            ) {
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            return zffalse;
        }

        _itemLoadCheck();
        _modified = zftrue;

        zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.find(itemPathTmp);
        if(itExist != _itemToKeep.end()) {
            _itemToKeep.erase(itExist);
        }

        zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.find(itemPathTmp);
        if(itMod != _itemToAdd.end()) {
            if(!itMod->second) {
                itMod->second = ZFIO_cachePathGen();
            }
            return ZFInputRead(ZFOutputForPathInfo(itMod->second), input) != zfindexMax();
        }
        else {
            return ZFInputRead(ZFOutputForPathInfo(_itemToAdd[itemPathTmp] = ZFIO_cachePathGen()), input) != zfindexMax();
        }
    }

    zfoverride
    virtual zfbool ioPathCreate(ZF_IN const zfstring &itemPath) {
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            return zffalse;
        }
        _itemLoadCheck();

        zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.find(itemPathTmp);
        if(itExist != _itemToKeep.end()) {
            return itExist->second;
        }

        zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.find(itemPathTmp);
        if(itMod != _itemToAdd.end()) {
            return !(itMod->second);
        }

        _modified = zftrue;
        _itemToAdd[itemPathTmp] = zfnull;
        return zftrue;
    }
    zfoverride
    virtual zfbool ioRemove(ZF_IN const zfstring &itemPath) {
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            return zffalse;
        }

        _itemLoadCheck();

        for(zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.begin(); itExist != _itemToKeep.end(); ) {
            if(_isSameOrChildOf(itExist->first, itemPathTmp)) {
                _modified = zftrue;
                _itemToKeep.erase(itExist++);
            }
            else {
                ++itExist;
            }
        }

        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.begin(); itMod != _itemToAdd.end(); ) {
            if(_isSameOrChildOf(itMod->first, itemPathTmp)) {
                _modified = zftrue;
                _itemToAdd.erase(itMod++);
            }
            else {
                ++itMod;
            }
        }
        return zftrue;
    }
    zfoverride
    virtual zfbool ioMove(
            ZF_IN const zfstring &itemPathTo
            , ZF_IN const zfstring &itemPathFrom
            ) {
        if(!itemPathFrom || !itemPathTo || itemPathFrom == itemPathTo) {
            return zffalse;
        }

        _itemLoadCheck();
        zfbool moved = zffalse;

        zfstlhashmap<zfstring, zfbool> existToMove; // <itemPath, isDir>
        zfstlhashmap<zfstring, ZFPathInfo> modToMove; // <itemPath, cachePath>
        zfstlhashmap<zfstring, zfbool> existToRemove; // <itemPath, dummy>
        zfstlhashmap<zfstring, zfbool> modToRemove; // <itemPath, dummy>

        for(zfstlhashmap<zfstring, zfbool>::iterator itExist = _itemToKeep.begin(); itExist != _itemToKeep.end(); ++itExist) {
            if(_isSameOrChildOf(itExist->first, itemPathFrom)) {
                moved = zftrue;
                existToMove[itExist->first] = itExist->second;
            }
            else if(_isSameOrChildOf(itExist->first, itemPathTo)) {
                existToRemove[itExist->first] = zftrue;
            }
        }
        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.begin(); itMod != _itemToAdd.end(); ++itMod) {
            if(_isSameOrChildOf(itMod->first, itemPathFrom)) {
                moved = zftrue;
                modToMove[itMod->first] = itMod->second;
            }
            else if(_isSameOrChildOf(itMod->first, itemPathTo)) {
                modToRemove[itMod->first] = zftrue;
            }
        }
        if(!moved) {
            return zffalse;
        }

        // remove conflict
        for(zfstlhashmap<zfstring, zfbool>::iterator it = existToRemove.begin(); it != existToRemove.end(); ++it) {
            _itemToKeep.erase(it->first);
        }
        for(zfstlhashmap<zfstring, zfbool>::iterator it = modToRemove.begin(); it != modToRemove.end(); ++it) {
            zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.find(it->first);
            if(itMod != _itemToAdd.end() && itMod->second) {
                ZFIORemove(itMod->second);
                _itemToAdd.erase(itMod);
            }
        }

        // move
        for(zfstlhashmap<zfstring, zfbool>::iterator it = existToMove.begin(); it != existToMove.end(); ++it) {
            zfstring itemPath = (it->first.length() == itemPathFrom.length()
                    ? itemPathTo
                    : zfstr("%s/%s", itemPathTo, it->first.cString() + itemPathFrom.length() + 1)
                    );
            if(it->second) {
                _itemToAdd[itemPath] = zfnull;
            }
            else {
                ZFInputRead(ZFOutputForPathInfo(_itemToAdd[itemPath] = ZFIO_cachePathGen()), this->input(it->first));
            }
            _itemToKeep.erase(it->first);
        }
        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator it = modToMove.begin(); it != modToMove.end(); ++it) {
            zfstring itemPath = (it->first.length() == itemPathFrom.length()
                    ? itemPathTo
                    : zfstr("%s/%s", itemPathTo, it->first.cString() + itemPathFrom.length() + 1)
                    );
            if(it->second) {
                zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod = _itemToAdd.find(it->first);
                ZFCoreAssert(itMod != _itemToAdd.end());
                _itemToAdd[itemPath] = itMod->second;
                _itemToAdd.erase(itMod);
            }
            else {
                _itemToAdd[itemPath] = zfnull;
                _itemToAdd.erase(it->first);
            }
        }
        return zftrue;
    }

private:
    zfclassNotPOD _FindData {
    public:
        zfstring itemPath;
        zfstlhashmap<zfstring, zfbool>::iterator itExist;
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itMod;
    };
    zfbool _itemFindNext(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN_OUT _FindData *d
            ) {
        while(d->itExist != _itemToKeep.end()) {
            if(_isDirectChildOf(d->itExist->first, d->itemPath)) {
                fd.impl().name = ZFFileNameOf(d->itExist->first);
                fd.impl().isDir = d->itExist->second;
                ++(d->itExist);
                return zftrue;
            }
            ++(d->itExist);
        }
        while(d->itMod != _itemToAdd.end()) {
            if(_isDirectChildOf(d->itMod->first, d->itemPath)) {
                fd.impl().name = ZFFileNameOf(d->itMod->first);
                fd.impl().isDir = !(d->itMod->second);
                ++(d->itMod);
                return zftrue;
            }
            ++(d->itMod);
        }
        return zffalse;
    }
public:
    zfoverride
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &itemPath
            ) {
        _itemLoadCheck();
        _FindData *d = zfpoolNew(_FindData);
        d->itemPath = _itemPathFormat(itemPath);
        d->itExist = _itemToKeep.begin();
        d->itMod = _itemToAdd.begin();
        if(this->_itemFindNext(fd, d)) {
            fd.implAttach("ZFCompressToken_default", d);
            return zftrue;
        }
        else {
            zfpoolDelete(d);
            return zffalse;
        }
    }
    zfoverride
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        _FindData *d = (_FindData *)fd.implUserData();
        return this->_itemFindNext(fd, d);
    }
    zfoverride
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        _FindData *d = (_FindData *)fd.implUserData();
        fd.implDetach();
        zfpoolDelete(d);
    }

    zfoverride
    virtual zfbool ioIsExist(ZF_IN const zfstring &itemPath) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return zffalse;
        }
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            // empty is valid root dir
            return zftrue;
        }
        zfstring buf;
        for(mz_uint itemIndex = 0; itemIndex < _impl.m_total_files; ++itemIndex) {
            zfindex size = (zfindex)mz_zip_reader_get_filename(&_impl, itemIndex, zfnull, 0);
            if(size == 0) {
                continue;
            }
            buf.capacity(size);
            mz_zip_reader_get_filename(&_impl, itemIndex, buf.zfunsafe_buffer(), (mz_uint)size);
            buf.zfunsafe_length(size - 1);
            if(_isSameOrChildOf(buf, itemPathTmp)) {
                return zftrue;
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &itemPath) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return zffalse;
        }
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            // empty is valid root dir
            return zftrue;
        }
        zfstring buf;
        for(mz_uint itemIndex = 0; itemIndex < _impl.m_total_files; ++itemIndex) {
            zfindex size = (zfindex)mz_zip_reader_get_filename(&_impl, itemIndex, zfnull, 0);
            if(size == 0) {
                continue;
            }
            buf.capacity(size);
            mz_zip_reader_get_filename(&_impl, itemIndex, buf.zfunsafe_buffer(), (mz_uint)size);
            buf.zfunsafe_length(size - 1);
            if(zfstringBeginWith(buf, itemPathTmp)) {
                if(buf.length() == itemPathTmp.length()) {
                    mz_zip_archive_file_stat stat;
                    return (mz_zip_reader_file_stat(&_impl, itemIndex, &stat) && stat.m_is_directory);
                }
                else if(buf[itemPathTmp.length()] == '/') {
                    return zftrue;
                }
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfbool ioIsDirAt(ZF_IN zfindex itemIndex) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return zffalse;
        }
        mz_zip_archive_file_stat stat;
        return (mz_zip_reader_file_stat(&_impl, (mz_uint)itemIndex, &stat) && stat.m_is_directory);
    }

    zfoverride
    virtual zfindex itemCount(void) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return 0;
        }
        return (zfindex)mz_zip_reader_get_num_files(&_impl);
    }
    zfoverride
    virtual zfindex itemIndex(ZF_IN const zfstring &itemPath) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return zfindexMax();
        }
        int ret = mz_zip_reader_locate_file(&_impl, itemPath, zfnull, MZ_ZIP_FLAG_CASE_SENSITIVE);
        if(ret < 0) {
            return zfindexMax();
        }
        else {
            return (zfindex)ret;
        }
    }
    zfoverride
    virtual zfbool itemPathT(
            ZF_IN_OUT zfstring &itemPath
            , ZF_IN zfindex itemIndex
            ) {
        if(_impl.m_zip_mode == MZ_ZIP_MODE_INVALID) {
            return zffalse;
        }
        // note the size contains '\0'
        zfindex size = (zfindex)mz_zip_reader_get_filename(&_impl, (mz_uint)itemIndex, zfnull, 0);
        if(size == 0) {
            return zffalse;
        }
        itemPath.capacity(itemPath.length() + size);
        mz_zip_reader_get_filename(&_impl, (mz_uint)itemIndex, itemPath.zfunsafe_buffer() + itemPath.length(), (mz_uint)size);
        itemPath.zfunsafe_length(itemPath.length() + size - 1);
        return zftrue;
    }
};

/*
 * note:
 * -  compressed as zip, which should compatible to most compress tools
 * -  for file contents, any encoding should work
 * -  for file and path names, UTF-8 encoding would be used,
 *   would work on:
 *   -  ZFCompress for any platform
 *   -  any compress tools under Mac and Linux (which also use UTF-8)
 *
 *   would not work on:
 *   -  any compress tools under Windows (which use UTF-16)
 *
 *   so, it's recommended to use ASCII chars only for file and path names
 */
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFCompressImpl_default, ZFCompress, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("miniz")
public:
    /** @brief see #ZFCompressOpen */
    virtual zfautoT<ZFCompressToken> compressOpen(
            ZF_IN const ZFPathInfo &pathInfo
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN ZFCompressLevel compressLevel
            ) {
        zfobj<_ZFP_I_ZFCompressToken_default> ret;
        if(ret->open(pathInfo, flags, compressLevel)) {
            return ret;
        }
        else {
            return zfnull;
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFCompressImpl_default)

ZF_NAMESPACE_GLOBAL_END


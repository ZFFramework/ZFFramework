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
            for(mz_uint itemIndex = 0, itemCount = mz_zip_reader_get_num_files(&_impl); itemIndex < itemCount; ++itemIndex) {
                // note the size contains '\0'
                zfindex size = (zfindex)mz_zip_reader_get_filename(&_impl, itemIndex, zfnull, 0);
                if(size == 0) {
                    continue;
                }
                zfstring itemPath;
                itemPath.capacity(itemPath.length() + size);
                mz_zip_reader_get_filename(&_impl, itemIndex, itemPath.zfunsafe_buffer() + itemPath.length(), (mz_uint)size);
                itemPath.zfunsafe_length(itemPath.length() + size - 1);

                mz_zip_archive_file_stat stat;
                _itemToKeep[itemPath] = (mz_zip_reader_file_stat(&_impl, itemIndex, &stat) && stat.m_is_directory);
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
    mz_uint32 _fileAttr(void) {
        // UNX_IFREG | 0644
        return (mz_uint32)(0100644 << 16);
    }
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
            for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ++itToKeep) {
                if(itToKeep->second) {
                    dirToCreate[itToKeep->first] = zftrue;
                    continue;
                }

                zfobj<ZFIOBuffer> buf;
                ZFOutput bufOutput = buf->output();
                zfbool success = mz_zip_reader_extract_file_to_callback(
                        &_impl
                        , itToKeep->first
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
                        , itToKeep->first
                        , zfself::_implRead_ZFInput
                        , &bufInput
                        , (mz_uint64)fileSize
                        , zfnull
                        , zfnull
                        , 0
                        , _compressLevel
                        , _fileAttr()
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

            for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
                if(!(itToAdd->second)) {
                    dirToCreate[itToAdd->first] = zftrue;
                    continue;
                }

                ZFInput bufInput = ZFInputForPathInfo(itToAdd->second);
                zfindex fileSize = bufInput.ioSize();
                if(fileSize == zfindexMax()) {
                    ret = zffalse;
                    break;
                }
                zfbool success = mz_zip_writer_add_read_buf_callback(
                        &implCache
                        , itToAdd->first
                        , zfself::_implRead_ZFInput
                        , &bufInput
                        , (mz_uint64)fileSize
                        , zfnull
                        , zfnull
                        , 0
                        , _compressLevel
                        , _fileAttr()
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
                for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ++itToKeep) {
                    if(_isChildOf(itToKeep->first, it->first)) {
                        hasChild = zftrue;
                        break;
                    }
                }
                if(!hasChild) {
                    for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
                        if(_isChildOf(itToAdd->first, it->first)) {
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

        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
            if(itToAdd->second) {
                ZFIORemove(itToAdd->second);
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
        zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.find(itemPathTmp);
        if(itToKeep != _itemToKeep.end()) {
            if(itToKeep->second) {
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
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.find(itemPathTmp);
        if(itToAdd != _itemToAdd.end()) {
            return ZFInputRead(output, ZFInputForPathInfo(itToAdd->second)) != zfindexMax();
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

        zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.find(itemPathTmp);
        if(itToKeep != _itemToKeep.end()) {
            _itemToKeep.erase(itToKeep);
        }

        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.find(itemPathTmp);
        if(itToAdd != _itemToAdd.end()) {
            if(!itToAdd->second) {
                itToAdd->second = ZFIO_cachePathGen();
            }
            return ZFInputRead(ZFOutputForPathInfo(itToAdd->second), input) != zfindexMax();
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

        zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.find(itemPathTmp);
        if(itToKeep != _itemToKeep.end()) {
            return itToKeep->second;
        }

        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.find(itemPathTmp);
        if(itToAdd != _itemToAdd.end()) {
            return !(itToAdd->second);
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

        for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ) {
            if(_isSameOrChildOf(itToKeep->first, itemPathTmp)) {
                _modified = zftrue;
                _itemToKeep.erase(itToKeep++);
            }
            else {
                ++itToKeep;
            }
        }

        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ) {
            if(_isSameOrChildOf(itToAdd->first, itemPathTmp)) {
                _modified = zftrue;
                _itemToAdd.erase(itToAdd++);
            }
            else {
                ++itToAdd;
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

        for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ++itToKeep) {
            if(_isSameOrChildOf(itToKeep->first, itemPathFrom)) {
                moved = zftrue;
                existToMove[itToKeep->first] = itToKeep->second;
            }
            else if(_isSameOrChildOf(itToKeep->first, itemPathTo)) {
                existToRemove[itToKeep->first] = zftrue;
            }
        }
        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
            if(_isSameOrChildOf(itToAdd->first, itemPathFrom)) {
                moved = zftrue;
                modToMove[itToAdd->first] = itToAdd->second;
            }
            else if(_isSameOrChildOf(itToAdd->first, itemPathTo)) {
                modToRemove[itToAdd->first] = zftrue;
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
            zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.find(it->first);
            if(itToAdd != _itemToAdd.end() && itToAdd->second) {
                ZFIORemove(itToAdd->second);
                _itemToAdd.erase(itToAdd);
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
                zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.find(it->first);
                ZFCoreAssert(itToAdd != _itemToAdd.end());
                _itemToAdd[itemPath] = itToAdd->second;
                _itemToAdd.erase(itToAdd);
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
        zfstlhashmap<zfstring, zfbool>::iterator itToKeep;
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd;
    };
    zfbool _itemFindNext(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN_OUT _FindData *d
            ) {
        while(d->itToKeep != _itemToKeep.end()) {
            if(_isDirectChildOf(d->itToKeep->first, d->itemPath)) {
                fd.impl().name = ZFFileNameOf(d->itToKeep->first);
                fd.impl().isDir = d->itToKeep->second;
                ++(d->itToKeep);
                return zftrue;
            }
            ++(d->itToKeep);
        }
        while(d->itToAdd != _itemToAdd.end()) {
            if(_isDirectChildOf(d->itToAdd->first, d->itemPath)) {
                fd.impl().name = ZFFileNameOf(d->itToAdd->first);
                fd.impl().isDir = !(d->itToAdd->second);
                ++(d->itToAdd);
                return zftrue;
            }
            ++(d->itToAdd);
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
        d->itToKeep = _itemToKeep.begin();
        d->itToAdd = _itemToAdd.begin();
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
        _itemLoadCheck();
        for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ++itToKeep) {
            if(_isSameOrChildOf(itToKeep->first, itemPath)) {
                return zftrue;
            }
        }
        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
            if(_isSameOrChildOf(itToAdd->first, itemPath)) {
                return zftrue;
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &itemPath) {
        _itemLoadCheck();
        for(zfstlhashmap<zfstring, zfbool>::iterator itToKeep = _itemToKeep.begin(); itToKeep != _itemToKeep.end(); ++itToKeep) {
            if(_isChildOf(itToKeep->first, itemPath)) {
                return zftrue;
            }
            else if(itToKeep->first == itemPath) {
                return itToKeep->second;
            }
        }
        for(zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd = _itemToAdd.begin(); itToAdd != _itemToAdd.end(); ++itToAdd) {
            if(_isChildOf(itToAdd->first, itemPath)) {
                return zftrue;
            }
            else if(itToAdd->first == itemPath) {
                return itToAdd->second == zfnull;
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfindex itemCount(void) {
        _itemLoadCheck();
        return (zfindex)(_itemToKeep.size() + _itemToAdd.size());
    }

    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        zfstlhashmap<zfstring, zfbool>::iterator itToKeep;
        zfstlhashmap<zfstring, zfbool>::iterator itToKeepEnd;
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAdd;
        zfstlhashmap<zfstring, ZFPathInfo>::iterator itToAddEnd;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return itToKeep != itToKeepEnd || itToAdd != itToAddEnd;
        }
        zfoverride
        virtual void next(void) {
            if(itToKeep != itToKeepEnd) {
                ++itToKeep;
            }
            else if(itToAdd != itToAddEnd) {
                ++itToAdd;
            }
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->itToKeep = itToKeep;
            ret->itToKeepEnd = itToKeepEnd;
            ret->itToAdd = itToAdd;
            ret->itToAddEnd = itToAddEnd;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return itToKeep == t->itToKeep && itToAdd == t->itToAdd;
        }
    };
    zfoverride
    virtual zfiter itemIter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->itToKeep = _itemToKeep.begin();
        impl->itToKeepEnd = _itemToKeep.end();
        impl->itToAdd = _itemToAdd.begin();
        impl->itToAddEnd = _itemToAdd.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfbool itemIsDir(ZF_IN const zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        if(impl->itToKeep != impl->itToKeepEnd) {
            return impl->itToKeep->second;
        }
        if(impl->itToAdd != impl->itToAddEnd) {
            return impl->itToAdd->second == zfnull;
        }
        return zffalse;
    }
    zfoverride
    virtual zfstring itemPath(ZF_IN const zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        if(impl->itToKeep != impl->itToKeepEnd) {
            return impl->itToKeep->first;
        }
        if(impl->itToAdd != impl->itToAddEnd) {
            return impl->itToAdd->first;
        }
        return zfnull;
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


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
    typedef enum {
        ItemType_FileToKeep, // normal file in source zip file
        ItemType_FileToAdd, // normal file to create in new zip file
    } ItemType;
    zfclassLikePOD ItemInfo {
    public:
        ItemType itemType;
        ZFPathInfo fileToAdd; // store cache path info for ItemType_FileToAdd
    };
    typedef zfstlhashmap<zfstring, ItemInfo> ItemMapType;
private:
    zfautoT<ZFIOToken> _refIOToken;
    ZFPathInfo _refPathInfo;
    ZFIOOpenOptionFlags _refOpenFlags;
    mz_zip_archive _impl; // readonly
    mz_uint _compressLevel;

    // for modify
    zfbool _modified;
    zfbool _itemLoaded;
    ItemMapType _itemMap; // <itemPath, ItemInfo>

protected:
    _ZFP_I_ZFCompressToken_default(void)
    : _refIOToken()
    , _refPathInfo()
    , _refOpenFlags(v_ZFIOOpenOption::e_Read)
    , _impl()
    , _compressLevel(MZ_DEFAULT_COMPRESSION)
    , _modified(zffalse)
    , _itemLoaded(zffalse)
    , _itemMap()
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
    // return:
    // * 0 : not child of
    // * 1 : is child of
    // * 2 : is direct child of
    static zfint _directChildNameOf(ZF_OUT zfstring &ret, ZF_IN const zfstring &maybeChild, ZF_IN const zfstring &maybeParent) {
        if(_isChildOf(maybeChild, maybeParent)) {
            zfindex pEnd = zfstringFind(maybeChild.cString() + maybeParent.length() + 1, "/");
            if(pEnd != zfindexMax()) {
                ret.assign(maybeChild.cString() + maybeParent.length() + 1, pEnd);
                return 1;
            }
            else {
                ret.assign(maybeChild.cString() + maybeParent.length() + 1);
                return 2;
            }
        }
        else {
            return 0;
        }
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
                if(mz_zip_reader_file_stat(&_impl, itemIndex, &stat) && stat.m_is_directory) {
                    // dir, discard
                }
                else {
                    _itemMap[itemPath].itemType = ItemType_FileToKeep;
                }
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

            ret = zftrue;
            for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ++itItem) {
                const zfstring &itemPath = itItem->first;
                const ItemInfo &itemInfo = itItem->second;
                switch(itemInfo.itemType) {
                    case ItemType_FileToAdd: {
                        zfobj<ZFIOBuffer> buf;
                        ZFOutput bufOutput = buf->output();
                        zfbool success = mz_zip_reader_extract_file_to_callback(
                                &_impl
                                , itemPath
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
                                , itemPath
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
                        break;
                    }
                    case ItemType_FileToKeep: {
                        ZFInput bufInput = ZFInputForPathInfo(itemInfo.fileToAdd);
                        zfindex fileSize = bufInput.ioSize();
                        if(fileSize == zfindexMax()) {
                            ret = zffalse;
                            break;
                        }
                        zfbool success = mz_zip_writer_add_read_buf_callback(
                                &implCache
                                , itemPath
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
                        break;
                    }
                    default:
                        ZFCoreCriticalShouldNotGoHere();
                        break;
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

        for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ++itItem) {
            const ItemInfo &itemInfo = itItem->second;
            if(itemInfo.itemType == ItemType_FileToAdd) {
                ZFIORemove(itemInfo.fileToAdd);
            }
        }
        _refPathInfo = zfnull;
        _modified = zffalse;
        _itemLoaded = zffalse;
        _itemMap.clear();
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
        _itemLoadCheck();
        ItemMapType::iterator itItem = _itemMap.find(itemPathTmp);
        if(itItem == _itemMap.end()) {
            return zffalse;
        }

        switch(itItem->second.itemType) {
            case ItemType_FileToKeep: {
                ZFOutput outputTmp = output;
                return mz_zip_reader_extract_file_to_callback(
                        &_impl
                        , itemPathTmp
                        , zfself::_implWrite_ZFOutput
                        , &outputTmp
                        , MZ_ZIP_FLAG_CASE_SENSITIVE
                        );
            }
            case ItemType_FileToAdd: {
                return ZFInputRead(output, ZFInputForPathInfo(itItem->second.fileToAdd)) != zfindexMax();
            }
            default:
                ZFCoreCriticalShouldNotGoHere();
                return zffalse;
        }
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

        ZFPathInfo cachePath = ZFIO_cachePathGen();
        if(ZFInputRead(ZFOutputForPathInfo(cachePath), input) == zfindexMax()) {
            ZFIORemove(cachePath);
            return zffalse;
        }

        _modified = zftrue;
        ItemMapType::iterator itItem = _itemMap.find(itemPathTmp);
        if(itItem == _itemMap.end()) {
            ItemInfo &itemInfo = _itemMap[itemPathTmp];
            itemInfo.itemType = ItemType_FileToAdd;
            itemInfo.fileToAdd = cachePath;
        }
        else {
            ItemInfo &itemInfo = itItem->second;
            if(itemInfo.itemType == ItemType_FileToAdd) {
                ZFIORemove(itemInfo.fileToAdd);
            }
            itemInfo.itemType = ItemType_FileToAdd;
            itemInfo.fileToAdd = cachePath;
        }
        return zftrue;
    }

    zfoverride
    virtual zfbool ioPathCreate(ZF_IN const zfstring &itemPath) {
        return zftrue;
    }
    zfoverride
    virtual zfbool ioRemove(ZF_IN const zfstring &itemPath) {
        zfstring itemPathTmp = _itemPathFormat(itemPath);
        if(!itemPathTmp) {
            return zffalse;
        }
        _itemLoadCheck();

        for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ) {
            const zfstring &itemPath = itItem->first;
            const ItemInfo &itemInfo = itItem->second;
            if(_isSameOrChildOf(itemPath, itemPathTmp)) {
                _modified = zftrue;
                if(itemInfo.itemType == ItemType_FileToAdd) {
                    ZFIORemove(itemInfo.fileToAdd);
                }
                _itemMap.erase(itItem++);
            }
            else {
                ++itItem;
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

        ItemMapType toMove;
        ItemMapType toRemove;
        for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ++itItem) {
            const zfstring &itemPath = itItem->first;
            ItemInfo &itemInfo = itItem->second;
            if(_isSameOrChildOf(itemPath, itemPathFrom)) {
                toMove[itemPath] = itemInfo;
            }
            else if(_isSameOrChildOf(itemPath, itemPathTo)) {
                toRemove[itemPath] = itemInfo;
            }
        }
        if(toMove.empty()) {
            return zffalse;
        }

        for(ItemMapType::iterator itToRemove = toRemove.begin(); itToRemove != toRemove.end(); ++itToRemove) {
            if(itToRemove->second.itemType == ItemType_FileToAdd) {
                ZFIORemove(itToRemove->second.fileToAdd);
            }
            _itemMap.erase(itToRemove->first);
        }
        for(ItemMapType::iterator itToMove = toMove.begin(); itToMove != toMove.end(); ++itToMove) {
            zfstring itemPath = (itToMove->first.length() == itemPathFrom.length()
                    ? itemPathTo
                    : zfstr("%s/%s", itemPathTo, itToMove->first.cString() + itemPathFrom.length() + 1)
                    );
            ZFPathInfo cachePath = ZFIO_cachePathGen();
            switch(itToMove->second.itemType) {
                case ItemType_FileToKeep: {
                    ZFOutput outputTmp = ZFOutputForPathInfo(cachePath);
                    if(!mz_zip_reader_extract_file_to_callback(
                            &_impl
                            , itemPath
                            , zfself::_implWrite_ZFOutput
                            , &outputTmp
                            , MZ_ZIP_FLAG_CASE_SENSITIVE
                            )) {
                        return zffalse;
                    }
                    ItemInfo &itemInfo = _itemMap[itemPath];
                    itemInfo.itemType = ItemType_FileToAdd;
                    itemInfo.fileToAdd = cachePath;
                    _itemMap.erase(itToMove->first);
                    break;
                }
                case ItemType_FileToAdd: {
                    _itemMap[itemPath] = itToMove->second;
                    _itemMap.erase(itToMove->first);
                    break;
                }
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }
        }
        return zftrue;
    }

private:
    zfclassNotPOD _FindData {
    public:
        zfstring itemPath;
        ItemMapType::iterator itItem;
        zfstlhashmap<zfstring, zfbool> processed;
    };
    zfbool _itemFindNext(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN_OUT _FindData &d
            ) {
        zfstring itemName;
        while(d.itItem != _itemMap.end()) {
            const zfstring &itemPath = d.itItem->first;
            ++(d.itItem);
            switch(_directChildNameOf(itemName, itemPath, d.itemPath)) {
                case 1: // grand child with direct child dir
                    if(d.processed.find(itemName) == d.processed.end()) {
                        d.processed[itemName] = zftrue;
                        fd.impl().name = itemName;
                        fd.impl().isDir = zftrue;
                        return zftrue;
                    }
                    break;
                case 2: // direct child file
                    if(d.processed.find(itemName) == d.processed.end()) {
                        d.processed[itemName] = zftrue;
                        fd.impl().name = itemName;
                        fd.impl().isDir = zffalse;
                        return zftrue;
                    }
                    break;
                case 0: // not child of
                default:
                    break;
            }
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
        d->itItem = _itemMap.begin();
        if(this->_itemFindNext(fd, *d)) {
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
        return this->_itemFindNext(fd, *d);
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
        for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ++itItem) {
            if(_isSameOrChildOf(itItem->first, itemPath)) {
                return zftrue;
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfbool ioIsDir(ZF_IN const zfstring &itemPath) {
        _itemLoadCheck();
        for(ItemMapType::iterator itItem = _itemMap.begin(); itItem != _itemMap.end(); ++itItem) {
            if(_isSameOrChildOf(itItem->first, itemPath)) {
                if(itItem->first.length() == itemPath.length()) {
                    return zffalse;
                }
                else {
                    return zftrue;
                }
            }
        }
        return zffalse;
    }

    zfoverride
    virtual zfindex itemCount(void) {
        _itemLoadCheck();
        return (zfindex)_itemMap.size();
    }

    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        ItemMapType::iterator itItem;
        ItemMapType::iterator itItemEnd;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return itItem != itItemEnd;
        }
        zfoverride
        virtual void next(void) {
            if(itItem != itItemEnd) {
                ++itItem;
            }
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->itItem = itItem;
            ret->itItemEnd = itItemEnd;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return itItem == t->itItem;
        }
    };
    zfoverride
    virtual zfiter itemIter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->itItem = _itemMap.begin();
        impl->itItemEnd = _itemMap.end();
        return zfiter(impl);
    }
    zfoverride
    virtual zfbool itemIsDir(ZF_IN const zfiter &it) {
        return zffalse;
    }
    zfoverride
    virtual zfstring itemPath(ZF_IN const zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        return impl->itItem->first;
    }
    zfoverride
    virtual void itemRemove(ZF_IN_OUT zfiter &it) {
        _Iter *impl = it.impl<_Iter *>();
        if(impl->itItem->second.itemType == ItemType_FileToAdd) {
            ZFIORemove(impl->itItem->second.fileToAdd);
        }
        _itemMap.erase((impl->itItem)++);
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


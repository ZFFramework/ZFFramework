#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFCompress.h"

#include "ZFAlgorithm.h"

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "../_repo/miniz/miniz.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFCompressToken_default : zfextend ZFCompressToken {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFCompressToken_default, ZFCompressToken)
private:
    zfautoT<ZFIOToken> _refIOToken;
    ZFIOOpenOptionFlags _refOpenFlags;
    mz_uint _compressLevel;
    mz_zip_archive _impl;
protected:
    _ZFP_I_ZFCompressToken_default(void)
    : _refIOToken()
    , _refOpenFlags(v_ZFIOOpenOption::e_Read)
    , _compressLevel(MZ_DEFAULT_COMPRESSION)
    , _impl()
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
        _impl.m_pIO_opaque = this;
        _impl.m_pRead = zfself::_implRead;
        _impl.m_pWrite = zfself::_implWrite;

        do {
            if(ZFBitTest(flags, v_ZFIOOpenOption::e_Modify)) {
                if(!mz_zip_reader_init(&_impl, (mz_uint64)fileSize, MZ_ZIP_FLAG_CASE_SENSITIVE)) {
                    break;
                }
                if(!mz_zip_writer_init_from_reader(&_impl, zfnull)) {
                    break;
                }
                return zftrue;
            }
            else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Write)) {
                if(!mz_zip_writer_init(&_impl, 0)) {
                    break;
                }
                return zftrue;
            }
            else if(ZFBitTest(flags, v_ZFIOOpenOption::e_Read)) {
                if(!mz_zip_reader_init(&_impl, (mz_uint64)fileSize, MZ_ZIP_FLAG_CASE_SENSITIVE)) {
                    break;
                }
                return zftrue;
            }
            else {
                break;
            }
        } while(zffalse);

        // fail, cleanup
        this->close();
        return zffalse;
    }

private:
    static size_t _implRead(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n) {
        _ZFP_I_ZFCompressToken_default *p = (_ZFP_I_ZFCompressToken_default *)pOpaque;
        if(!p || !p->_refIOToken) {
            return 0;
        }
        p->_refIOToken->ioSeek((zfindex)file_ofs);
        return (size_t)p->_refIOToken->ioRead(pBuf, (zfindex)n);
    }
    static size_t _implWrite(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n) {
        _ZFP_I_ZFCompressToken_default *p = (_ZFP_I_ZFCompressToken_default *)pOpaque;
        if(!p || !p->_refIOToken) {
            return 0;
        }
        p->_refIOToken->ioSeek((zfindex)file_ofs);
        return (size_t)p->_refIOToken->ioWrite(pBuf, (zfindex)n);
    }

    static size_t _itemRead(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n) {
        ZFOutput &output = *(ZFOutput *)pOpaque;
        output.ioSeek((zfindex)file_ofs);
        return (size_t)output.execute(pBuf, (zfindex)n);
    }
    static size_t _itemWrite(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n) {
        ZFInput &input = *(ZFInput *)pOpaque;
        input.ioSeek(file_ofs);
        return (size_t)input.execute(pBuf, (zfindex)n);
    }

public:
    zfoverride
    virtual zfbool close(void) {
        zfbool ret = zftrue;
        if(_impl.m_zip_mode != MZ_ZIP_MODE_INVALID) {
            mz_zip_writer_finalize_archive(&_impl);
            ret = mz_zip_end(&_impl);
        }
        _refIOToken = zfnull;
        return ret;
    }

    zfoverride
    virtual zfbool read(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const zfstring &itemPath
            ) {
        ZFOutput outputTmp = output;
        return mz_zip_reader_extract_file_to_callback(
                &_impl
                , itemPath
                , zfself::_itemRead
                , &outputTmp
                , MZ_ZIP_FLAG_CASE_SENSITIVE
                );
    }
    zfoverride
    virtual zfbool write(
            ZF_IN const zfstring &itemPath
            , ZF_IN const ZFInput &input
            ) {
        zfindex fileSize = input.ioSize();
        if(fileSize == zfindexMax()) {
            zfstring buf;
            ZFInputRead(buf, input);
            return mz_zip_writer_add_mem(
                    &_impl
                    , itemPath
                    , buf
                    , buf.length()
                    , _compressLevel
                    );
        }
        ZFInput inputTmp = input;
        return mz_zip_writer_add_read_buf_callback(
                &_impl
                , itemPath
                , zfself::_itemWrite
                , &inputTmp
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
    }

    zfoverride
    virtual zfbool pathCreate(ZF_IN const zfstring &itemPath) {
        if(!itemPath) {
            return zffalse;
        }
        if(itemPath[itemPath.length() - 1] == '/') {
            return mz_zip_writer_add_mem(&_impl, itemPath, zfnull, 0, 0);
        }
        else {
            zfstring tmp = itemPath;
            tmp += '/';
            return mz_zip_writer_add_mem(&_impl, tmp, zfnull, 0, 0);
        }
    }
    zfoverride
    virtual zfbool remove(ZF_IN const zfstring &itemPath) {
        // not implemented yet
        return zffalse;
    }
    zfoverride
    virtual zfbool move(
            ZF_IN const zfstring &itemPathFrom
            , ZF_IN const zfstring &itemPathTo
            ) {
        // not implemented yet
        return zffalse;
    }

private:
    zfclassNotPOD _FindData {
    public:
        zfstring itemPath; // ensured no '/' at head or tail
        zfindex lastIndex; // last found index
    };
    zfbool _itemFindNext(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN_OUT _FindData *d
            ) {
        zfindex itemCount = this->itemCount();
        zfstring tmp;
        for(zfindex i = d->lastIndex + 1; i < itemCount; ++i) {
            tmp.removeAll();
            if(!this->itemPathT(tmp, i)) {
                continue;
            }
            const zfchar *path = tmp;
            zfindex pathLen = tmp.length();
            while(path[0] == '/') {
                ++path;
                --pathLen;
            }
            while(pathLen > 0 && path[pathLen - 1] == '/') {
                --pathLen;
            }
            if(pathLen == 0) {
                continue;
            }
            if(!d->itemPath.isEmpty()) {
                if(pathLen == d->itemPath.length() || !zfstringBeginWith(path, d->itemPath)) {
                    continue;
                }
                path += d->itemPath.length() + 1;
                pathLen -= d->itemPath.length() + 1;
                if(pathLen == 0) {
                    // itemPath itself, skip
                    continue;
                }
            }
            zfindex pos = zfstringFind(path, pathLen, "/");
            if(pos != zfindexMax()) {
                // only needs to find direct child, but got grand child
                continue;
            }

            // found
            d->lastIndex = i;
            fd.impl().name.assign(path, pos == zfindexMax() ? pathLen : pos);
            fd.impl().isDir = this->itemIsDirAt(i);
            return zftrue;
        }
        return zffalse;
    }
public:
    zfoverride
    virtual zfbool itemFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &itemPath
            ) {
        _FindData *d = zfpoolNew(_FindData);
        d->lastIndex = zfindexMax();
        ZFPathFormatT(d->itemPath, itemPath);
        while(!d->itemPath.isEmpty() && d->itemPath[0] == '/') {
            d->itemPath.remove(0, 1);
        }
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
    virtual zfbool itemFindNext(ZF_IN_OUT ZFIOFindData &fd) {
        _FindData *d = (_FindData *)fd.implUserData();
        return this->_itemFindNext(fd, d);
    }
    zfoverride
    virtual void itemFindClose(ZF_IN_OUT ZFIOFindData &fd) {
        _FindData *d = (_FindData *)fd.implUserData();
        fd.implDetach();
        zfpoolDelete(d);
    }

    zfoverride
    virtual zfbool itemIsDir(ZF_IN const zfstring &itemPath) {
        mz_zip_archive_file_stat stat;
        return (mz_zip_reader_file_stat(&_impl, (mz_uint)this->itemIndex(itemPath), &stat) && stat.m_is_directory);
    }

    zfoverride
    virtual zfbool itemIsDirAt(ZF_IN zfindex itemIndex) {
        mz_zip_archive_file_stat stat;
        return (mz_zip_reader_file_stat(&_impl, (mz_uint)itemIndex, &stat) && stat.m_is_directory);
    }

    zfoverride
    virtual zfindex itemCount(void) {
        return (zfindex)mz_zip_reader_get_num_files(&_impl);
    }
    zfoverride
    virtual zfindex itemIndex(ZF_IN const zfstring &itemPath) {
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
        // note the size contains '\0'
        zfindex size = (zfindex)mz_zip_reader_get_filename(&_impl, (mz_uint)itemIndex, zfnull, 0);
        if(size == 0) {
            return zffalse;
        }
        itemPath.capacity(itemPath.length() + size);
        mz_zip_reader_get_filename(&_impl, (mz_uint)itemIndex, itemPath.zfunsafe_buffer() + itemPath.length(), size);
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


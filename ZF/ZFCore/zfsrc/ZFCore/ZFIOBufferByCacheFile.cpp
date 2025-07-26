#include "ZFIOBufferByCacheFile.h"
#include "ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFIOBufferByCacheFile {
public:
    zfuint refCount;
    void *token;
    zfstring tmpFilePath;
    zfindex outputIndex;
    zfindex inputIndex;
    zfindex fileSize;
public:
    _ZFP_ZFIOBufferByCacheFile(void)
    : refCount(1)
    , token(zfnull)
    , tmpFilePath()
    , outputIndex(0)
    , inputIndex(0)
    , fileSize(0)
    {
        zfstringAppend(this->tmpFilePath, "%s%sZFIOBufferByCacheFile_%s",
            ZFPathForCache(),
            '/',
            zfidentityCalc(this));
        this->token = ZFFileOpen(this->tmpFilePath,
            v_ZFFileOpenOption::e_Create | v_ZFFileOpenOption::e_Read | v_ZFFileOpenOption::e_Write);
    }
    ~_ZFP_ZFIOBufferByCacheFile(void) {
        if(this->token != zfnull) {
            ZFFileClose(this->token);
            this->token = zfnull;
        }
        ZFFileRemove(this->tmpFilePath, ZF_HINT("recursive")zffalse, ZF_HINT("force")zftrue);
    }
};

zfclass _ZFP_ZFIOBufferByCacheFile_input : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByCacheFile_input, ZFObject)
public:
    _ZFP_ZFIOBufferByCacheFile *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
        zfsuper::objectOnDealloc();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput
            , ZFMP_IN(void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

zfclass _ZFP_ZFIOBufferByCacheFile_output : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferByCacheFile_output, ZFObject)
public:
    _ZFP_ZFIOBufferByCacheFile *d;
protected:
    zfoverride
    virtual void objectOnDealloc() {
        --(d->refCount);
        if(d->refCount == 0) {
            zfdelete(d);
        }
        zfsuper::objectOnDealloc();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput
            , ZFMP_IN(const void *, buf)
            , ZFMP_IN(zfindex, count)
            )

    ZFMETHOD_DECLARE_2(zfbool, ioSeek
            , ZFMP_IN(zfindex, byteSize)
            , ZFMP_IN(ZFSeekPos, pos)
            )
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};

// ============================================================
ZFOBJECT_REGISTER(ZFIOBufferByCacheFile)

void ZFIOBufferByCacheFile::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFIOBufferByCacheFile);

    zfobj<_ZFP_ZFIOBufferByCacheFile_input> iOwner;
    iOwner->d = d;
    ++(d->refCount);
    this->_ZFP_input = ZFCallbackForMemberMethod(iOwner, ZFMethodAccess(_ZFP_ZFIOBufferByCacheFile_input, onInput));
    this->_ZFP_input.ioOwner(iOwner);

    zfobj<_ZFP_ZFIOBufferByCacheFile_output> oOwner;
    oOwner->d = d;
    ++(d->refCount);
    this->_ZFP_output = ZFCallbackForMemberMethod(oOwner, ZFMethodAccess(_ZFP_ZFIOBufferByCacheFile_output, onOutput));
    this->_ZFP_output.ioOwner(oOwner);
}
void ZFIOBufferByCacheFile::objectOnDealloc(void) {
    --(d->refCount);
    if(d->refCount == 0) {
        zfdelete(d);
    }
    zfsuper::objectOnDealloc();
}

void ZFIOBufferByCacheFile::implRemoveAll(void) {
    d->inputIndex = 0;
    d->outputIndex = 0;
    d->fileSize = 0;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_input, zfindex, onInput
        , ZFMP_IN(void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(d->token == zfnull) {
        return 0;
    }
    if(buf == zfnull) {
        return d->fileSize - d->inputIndex;
    }
    else {
        ZFFileSeek(d->token, d->inputIndex);
        zfindex read = ZFFileRead(d->token, buf, zfmMin(count, d->fileSize - d->inputIndex));
        d->inputIndex += read;
        return read;
    }
}
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_input, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    if(d->token == zfnull) {
        return zffalse;
    }
    d->inputIndex = ZFIOCallbackCalcSeek(0, d->fileSize, d->inputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_input, zfindex, ioTell) {
    return d->inputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_input, zfindex, ioSize) {
    return d->fileSize;
}

// ============================================================
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_output, zfindex, onOutput
        , ZFMP_IN(const void *, buf)
        , ZFMP_IN(zfindex, count)
        ) {
    if(d->token == zfnull) {
        return 0;
    }
    ZFFileSeek(d->token, d->outputIndex);
    zfindex written = ZFFileWrite(d->token, buf, count);
    d->outputIndex += written;
    if(d->outputIndex > d->fileSize) {
        d->fileSize = d->outputIndex;
    }
    return written;
}

ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferByCacheFile_output, zfbool, ioSeek
        , ZFMP_IN(zfindex, byteSize)
        , ZFMP_IN(ZFSeekPos, pos)
        ) {
    if(d->token == zfnull) {
        return zffalse;
    }
    d->outputIndex = ZFIOCallbackCalcSeek(0, d->fileSize, d->outputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_output, zfindex, ioTell) {
    return d->outputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferByCacheFile_output, zfindex, ioSize) {
    return d->fileSize;
}

ZF_NAMESPACE_GLOBAL_END


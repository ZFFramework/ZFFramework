#include "ZFImpl_sys_SDL_ZF_impl.h"

#if ZF_ENV_sys_SDL

#include "ZFMainEntry_sys_SDL.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static Sint64 _ZFP_ZFImpl_sys_SDL_IOStream_size_ZFInput(void *userdata) {
    ZFInput const &callback = *(ZFInput *)userdata;
    zfindex size = callback.ioSize();
    return size == zfindexMax() ? (Sint64)-1 : (Sint64)size;
}

static Sint64 _ZFP_ZFImpl_sys_SDL_IOStream_seek_ZFInput(void *userdata, Sint64 offset, SDL_IOWhence whence) {
    ZFInput const &callback = *(ZFInput *)userdata;
    ZFSeekPos seekPos = ZFSeekPosBegin;
    switch(whence) {
        case SDL_IO_SEEK_SET:
            seekPos = ZFSeekPosBegin;
            break;
        case SDL_IO_SEEK_CUR:
            if(offset >= 0) {
                seekPos = ZFSeekPosCur;
            }
            else {
                seekPos = ZFSeekPosCurReversely;
            }
            break;
        case SDL_IO_SEEK_END:
            seekPos = ZFSeekPosEnd;
            break;
        default:
            return -1;
    }
    if(!callback.ioSeek(zfmAbs(offset), seekPos)) {
        return -1;
    }
    zfindex cur = callback.ioTell();
    return cur != zfindexMax() ? (Sint64)cur : (Sint64)-1;
}

static size_t _ZFP_ZFImpl_sys_SDL_IOStream_read_ZFInput(void *userdata, void *ptr, size_t size, SDL_IOStatus *status) {
    ZFInput const &callback = *(ZFInput *)userdata;
    size_t ret = (size_t)callback.execute(ptr, size);
    if(ret < size) {
        *status = SDL_IO_STATUS_EOF;
    }
    return ret;
}

static size_t _ZFP_ZFImpl_sys_SDL_IOStream_write_ZFInput(void *userdata, const void *ptr, size_t size, SDL_IOStatus *status) {
    *status = SDL_IO_STATUS_READONLY;
    return 0;
}

static bool _ZFP_ZFImpl_sys_SDL_IOStream_close_ZFInput(void *userdata) {
    zfpoolDelete((ZFInput *)userdata);
    return true;
}

SDL_IOStream *ZFImpl_sys_SDL_ZFInputToSDL_IOStream(ZF_IN const ZFInput &callback) {
    if(!callback) {
        return zfnull;
    }
    SDL_IOStreamInterface impl;
    SDL_INIT_INTERFACE(&impl);
    impl.size = _ZFP_ZFImpl_sys_SDL_IOStream_size_ZFInput;
    impl.seek = _ZFP_ZFImpl_sys_SDL_IOStream_seek_ZFInput;
    impl.read = _ZFP_ZFImpl_sys_SDL_IOStream_read_ZFInput;
    impl.write = _ZFP_ZFImpl_sys_SDL_IOStream_write_ZFInput;
    impl.close = _ZFP_ZFImpl_sys_SDL_IOStream_close_ZFInput;
    return SDL_OpenIO(&impl, zfpoolNew(ZFInput, callback));
}

// ============================================================
static Sint64 _ZFP_ZFImpl_sys_SDL_IOStream_size_ZFOutput(void *userdata) {
    return -1;
}

static Sint64 _ZFP_ZFImpl_sys_SDL_IOStream_seek_ZFOutput(void *userdata, Sint64 offset, SDL_IOWhence whence) {
    ZFOutput const &callback = *(ZFOutput *)userdata;
    ZFSeekPos seekPos = ZFSeekPosBegin;
    switch(whence) {
        case SDL_IO_SEEK_SET:
            seekPos = ZFSeekPosBegin;
            break;
        case SDL_IO_SEEK_CUR:
            if(offset >= 0) {
                seekPos = ZFSeekPosCur;
            }
            else {
                seekPos = ZFSeekPosCurReversely;
            }
            break;
        case SDL_IO_SEEK_END:
            seekPos = ZFSeekPosEnd;
            break;
        default:
            return -1;
    }
    if(!callback.ioSeek(zfmAbs(offset), seekPos)) {
        return -1;
    }
    zfindex cur = callback.ioTell();
    return cur != zfindexMax() ? (Sint64)cur : (Sint64)-1;
}

static size_t _ZFP_ZFImpl_sys_SDL_IOStream_read_ZFOutput(void *userdata, void *ptr, size_t size, SDL_IOStatus *status) {
    *status = SDL_IO_STATUS_WRITEONLY;
    return 0;
}

static size_t _ZFP_ZFImpl_sys_SDL_IOStream_write_ZFOutput(void *userdata, const void *ptr, size_t size, SDL_IOStatus *status) {
    ZFOutput const &callback = *(ZFOutput *)userdata;
    size_t ret = (size_t)callback.execute(ptr, size);
    if(ret < size) {
        *status = SDL_IO_STATUS_ERROR;
    }
    return ret;
}

static bool _ZFP_ZFImpl_sys_SDL_IOStream_close_ZFOutput(void *userdata) {
    zfpoolDelete((ZFOutput *)userdata);
    return true;
}

SDL_IOStream *ZFImpl_sys_SDL_ZFOutputToSDL_IOStream(ZF_IN const ZFOutput &callback) {
    if(!callback) {
        return zfnull;
    }
    SDL_IOStreamInterface impl;
    SDL_INIT_INTERFACE(&impl);
    impl.size = _ZFP_ZFImpl_sys_SDL_IOStream_size_ZFOutput;
    impl.seek = _ZFP_ZFImpl_sys_SDL_IOStream_seek_ZFOutput;
    impl.read = _ZFP_ZFImpl_sys_SDL_IOStream_read_ZFOutput;
    impl.write = _ZFP_ZFImpl_sys_SDL_IOStream_write_ZFOutput;
    impl.close = _ZFP_ZFImpl_sys_SDL_IOStream_close_ZFOutput;
    return SDL_OpenIO(&impl, zfpoolNew(ZFOutput, callback));
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL


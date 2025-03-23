#include "ZFImpl_sys_SDL_ZF_impl.h"

#if ZF_ENV_sys_SDL

#include "ZFMainEntry_sys_SDL.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static Sint64 _ZFP_ZFImpl_sys_SDL_RWops_size_ZFInput(struct SDL_RWops *context) {
    ZFInput const &callback = *(ZFInput *)context->hidden.unknown.data1;
    zfindex size = callback.ioSize();
    return size == zfindexMax() ? (Sint64)-1 : (Sint64)size;
}

static Sint64 _ZFP_ZFImpl_sys_SDL_RWops_seek_ZFInput(struct SDL_RWops *context, Sint64 offset,
                                                     int whence) {
    ZFInput const &callback = *(ZFInput *)context->hidden.unknown.data1;
    ZFSeekPos seekPos = ZFSeekPosBegin;
    switch(whence) {
        case RW_SEEK_SET:
            seekPos = ZFSeekPosBegin;
            break;
        case RW_SEEK_CUR:
            if(offset >= 0) {
                seekPos = ZFSeekPosCur;
            }
            else {
                seekPos = ZFSeekPosCurReversely;
            }
            break;
        case RW_SEEK_END:
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

static size_t _ZFP_ZFImpl_sys_SDL_RWops_read_ZFInput(struct SDL_RWops *context, void *ptr,
                                                     size_t size, size_t maxnum) {
    ZFInput const &callback = *(ZFInput *)context->hidden.unknown.data1;
    return (size_t)callback.execute(ptr, size * maxnum);
}

static size_t _ZFP_ZFImpl_sys_SDL_RWops_write_ZFInput(struct SDL_RWops *context, const void *ptr,
                                                      size_t size, size_t num) {
    return 0;
}

static int _ZFP_ZFImpl_sys_SDL_RWops_close_ZFInput(struct SDL_RWops *context) {
    zfdelete((ZFInput *)context->hidden.unknown.data1);
    SDL_FreeRW(context);
    return 0;
}

SDL_RWops *ZFImpl_sys_SDL_ZFInputToSDL_RWops(ZF_IN const ZFInput &callback) {
    if(!callback) {
        return zfnull;
    }
    SDL_RWops *ret = SDL_AllocRW();
    ret->hidden.unknown.data1 = zfnew(ZFInput, callback);
    ret->size = _ZFP_ZFImpl_sys_SDL_RWops_size_ZFInput;
    ret->seek = _ZFP_ZFImpl_sys_SDL_RWops_seek_ZFInput;
    ret->read = _ZFP_ZFImpl_sys_SDL_RWops_read_ZFInput;
    ret->write = _ZFP_ZFImpl_sys_SDL_RWops_write_ZFInput;
    ret->close = _ZFP_ZFImpl_sys_SDL_RWops_close_ZFInput;
    return ret;
}

// ============================================================
static Sint64 _ZFP_ZFImpl_sys_SDL_RWops_size_ZFOutput(struct SDL_RWops *context) {
    return -1;
}

static Sint64 _ZFP_ZFImpl_sys_SDL_RWops_seek_ZFOutput(struct SDL_RWops *context, Sint64 offset,
                                                      int whence) {
    ZFOutput const &callback = *(ZFOutput *)context->hidden.unknown.data1;
    ZFSeekPos seekPos = ZFSeekPosBegin;
    switch(whence) {
        case RW_SEEK_SET:
            seekPos = ZFSeekPosBegin;
            break;
        case RW_SEEK_CUR:
            if(offset >= 0) {
                seekPos = ZFSeekPosCur;
            }
            else {
                seekPos = ZFSeekPosCurReversely;
            }
            break;
        case RW_SEEK_END:
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

static size_t _ZFP_ZFImpl_sys_SDL_RWops_read_ZFOutput(struct SDL_RWops *context, void *ptr,
                                                      size_t size, size_t maxnum) {
    return 0;
}

static size_t _ZFP_ZFImpl_sys_SDL_RWops_write_ZFOutput(struct SDL_RWops *context, const void *ptr,
                                                       size_t size, size_t num) {
    ZFOutput const &callback = *(ZFOutput *)context->hidden.unknown.data1;
    return (size_t)callback.execute(ptr, size * num);
}

static int _ZFP_ZFImpl_sys_SDL_RWops_close_ZFOutput(struct SDL_RWops *context) {
    zfdelete((ZFOutput *)context->hidden.unknown.data1);
    SDL_FreeRW(context);
    return 0;
}

SDL_RWops *ZFImpl_sys_SDL_ZFOutputToSDL_RWops(ZF_IN const ZFOutput &callback) {
    if(!callback) {
        return zfnull;
    }
    SDL_RWops *ret = SDL_AllocRW();
    ret->hidden.unknown.data1 = zfnew(ZFOutput, callback);
    ret->size = _ZFP_ZFImpl_sys_SDL_RWops_size_ZFOutput;
    ret->seek = _ZFP_ZFImpl_sys_SDL_RWops_seek_ZFOutput;
    ret->read = _ZFP_ZFImpl_sys_SDL_RWops_read_ZFOutput;
    ret->write = _ZFP_ZFImpl_sys_SDL_RWops_write_ZFOutput;
    ret->close = _ZFP_ZFImpl_sys_SDL_RWops_close_ZFOutput;
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_SDL


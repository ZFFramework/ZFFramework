#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_iOS

#import <AVFoundation/AVFoundation.h>

@interface _ZFP_ZFAudioImpl_sys_iOS_Audio : NSObject<AVAudioPlayerDelegate>
@property (nonatomic, assign) ZFAudio *owner;
@property (nonatomic, strong) AVAudioPlayer *audio;
@property (nonatomic, assign) zfautoT<ZFTaskId> loadTaskId;
@end
@implementation _ZFP_ZFAudioImpl_sys_iOS_Audio
- (instancetype)init {
    self = [super init];
    self.loadTaskId = zfnull;
    return self;
}
- (void)audioAttach {
    if(self.audio == nil) {
        return;
    }

    self.audio.delegate = self;
}
- (void)audioDetach {
    if(self.audio == nil) {
        return;
    }
    self.audio.delegate = nil;
    AVAudioPlayer *tmp = self.audio;
    self.audio = nil;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [tmp stop];
        });
}

// protocol
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag {
    if(self.owner != zfnull) {
        if(flag) {
            ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop(self.owner, zftrue, zfnull);
        }
        else {
            zfobj<v_zfstring> errorHint;
            errorHint->zfv = "play failed";
            ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop(self.owner, zffalse, errorHint);
        }
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_iOS, ZFAudio, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:AVAudioPlayer")
public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = [_ZFP_ZFAudioImpl_sys_iOS_Audio new];
        nativeAudio.owner = audio;
        return (__bridge_retained void *)nativeAudio;
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge_transfer _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio audioDetach];
        nativeAudio = nil;
    }

    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const ZFInput &input
            ) {
        zfself *owner = this;
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();

        ZFLISTENER_2(onLoad
                , ZFAudio *, audio
                , ZFInput, input
                ) {
            _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();

            zfstring buf;
            ZFInputRead(buf, input);
            if(buf.length() == 0) {
                zfobj<v_zfstring> errorHint;
                errorHint->zfv = "unable to load from input";
                zfargs.result(errorHint);
                return;
            }

            NSData *data = [[NSData alloc] initWithBytesNoCopy:buf.zfunsafe_bufferGiveUp() length:(NSUInteger)buf.length() deallocator:^(void *bytes, NSUInteger length) {
                zfstring::zfunsafe_bufferFree(bytes);
            }];
            NSError *error = nil;
            nativeAudio.audio = [[AVAudioPlayer alloc] initWithData:data error:&error];
            if(error != nil) {
                zfobj<v_zfstring> errorHint;
                ZFImpl_sys_iOS_zfstringFromNSString(errorHint->zfv, error.description);
                nativeAudio.audio = nil;
                zfargs.result(errorHint);
                return;
            }
            [nativeAudio audioAttach];
            if(![nativeAudio.audio play]) {
                [nativeAudio audioDetach];
                zfobj<v_zfstring> errorHint;
                errorHint->zfv = "unable to play audio";
                zfargs.result(errorHint);
                return;
            }
            [nativeAudio.audio pause];
            zfargs.result(zfnull);
        } ZFLISTENER_END()

        ZFLISTENER_2(onLoadFinish
                , zfself *, owner
                , ZFAudio *, audio
                ) {
            _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
            nativeAudio.loadTaskId = zfnull;

            v_zfstring *errorHint = zfargs.param0();
            owner->notifyAudioOnLoad(audio, errorHint == zfnull, errorHint);
        } ZFLISTENER_END()

        nativeAudio.loadTaskId = zfasync(onLoad, onLoadFinish);
    }
    virtual void nativeAudioLoad(
            ZF_IN ZFAudio *audio
            , ZF_IN const zfstring &url
            ) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();

        NSError *error = nil;
        nativeAudio.audio = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL URLWithString:ZFImpl_sys_iOS_zfstringToNSString(url)] error:&error];
        if(error != nil) {
            zfobj<v_zfstring> errorHint;
            ZFImpl_sys_iOS_zfstringFromNSString(errorHint->zfv, error.description);
            nativeAudio.audio = nil;
            this->notifyAudioOnLoad(audio, zffalse, errorHint);
            return;
        }
        [nativeAudio audioAttach];
        if(![nativeAudio.audio play]) {
            [nativeAudio audioDetach];
            zfobj<v_zfstring> errorHint;
            errorHint->zfv = "unable to play audio";
            this->notifyAudioOnLoad(audio, zffalse, errorHint);
            return;
        }
        [nativeAudio.audio pause];
        this->notifyAudioOnLoad(audio, zftrue, zfnull);
    }
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        if(nativeAudio.loadTaskId) {
            nativeAudio.loadTaskId->stop();
            nativeAudio.loadTaskId = zfnull;
        }
        [nativeAudio audioDetach];
    }

    virtual void nativeAudioStart(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio.audio.currentTime = 0;
        if([nativeAudio.audio play]) {
            this->notifyAudioOnResume(audio);
        }
        else {
            zfobj<v_zfstring> errorHint;
            errorHint->zfv = "unable to play audio";
            this->notifyAudioOnStop(audio, zffalse, errorHint);
        }
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio.audio pause];
        nativeAudio.audio.currentTime = 0;
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio.audio play];
        this->notifyAudioOnResume(audio);
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio.audio pause];
        this->notifyAudioOnPause(audio);
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        return (zftimet)(nativeAudio.audio.duration * 1000);
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        return (zftimet)(nativeAudio.audio.currentTime * 1000);
    }
    virtual void nativeAudioPosition(
            ZF_IN ZFAudio *audio
            , ZF_IN zftimet position
            ) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio.audio.currentTime = position;
    }

    virtual void nativeAudioVolume(
            ZF_IN ZFAudio *audio
            , ZF_IN zffloat volume
            ) {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio.audio.volume = volume;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


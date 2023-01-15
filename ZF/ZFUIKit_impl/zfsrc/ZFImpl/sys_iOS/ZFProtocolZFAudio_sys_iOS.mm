#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAudio.h"

#if ZF_ENV_sys_iOS

#import <AVFoundation/AVFoundation.h>

@interface _ZFP_ZFAudioImpl_sys_iOS_Audio : NSObject<AVAudioPlayerDelegate>
@property (nonatomic, assign) ZFAudio *owner;
@property (nonatomic, strong) AVAudioPlayer *audio;
@end
@implementation _ZFP_ZFAudioImpl_sys_iOS_Audio
- (void)audioAttach
{
    if(self.audio == nil)
    {
        return;
    }

    self.audio.delegate = self;
}
- (void)audioDetach
{
    if(self.audio == nil)
    {
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
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    if(self.owner != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFAudio)->notifyAudioOnStop(self.owner, (zfbool)flag);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAudioImpl_sys_iOS, ZFAudio, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:AVAudioPlayer")
public:
    virtual void *nativeAudioCreate(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = [_ZFP_ZFAudioImpl_sys_iOS_Audio new];
        nativeAudio.owner = audio;
        return (__bridge_retained void *)nativeAudio;
    }
    virtual void nativeAudioDestroy(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge_transfer _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio = nil;
    }

    virtual void nativeAudioLoad(ZF_IN ZFAudio *audio,
                                 ZF_IN const ZFInput &input)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();

        ZFBuffer buf;
        ZFInputReadAll(buf, input);
        if(buf.bufferSize() == 0)
        {
            this->notifyAudioOnLoad(audio, zffalse);
            return;
        }

        NSData *data = [NSData dataWithBytesNoCopy:buf.buffer() length:buf.bufferSize() freeWhenDone:YES];
        buf.bufferGiveUp();
        NSError *error = nil;
        nativeAudio.audio = [[AVAudioPlayer alloc] initWithData:data error:&error];
        if(error != nil)
        {
            nativeAudio.audio = nil;
            this->notifyAudioOnLoad(audio, zffalse);
            return;
        }
        [nativeAudio audioAttach];
        if(![nativeAudio.audio play])
        {
            [nativeAudio audioDetach];
            this->notifyAudioOnLoad(audio, zffalse);
            return;
        }
        [nativeAudio.audio pause];
        this->notifyAudioOnLoad(audio, zftrue);
    }
    virtual void nativeAudioLoad(ZF_IN ZFAudio *audio,
                                 ZF_IN const zfchar *url)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();

        NSError *error = nil;
        nativeAudio.audio = [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL URLWithString:ZFImpl_sys_iOS_zfstringToNSString(url)] error:&error];
        if(error != nil)
        {
            nativeAudio.audio = nil;
            this->notifyAudioOnLoad(audio, zffalse);
            return;
        }
        [nativeAudio audioAttach];
        if(![nativeAudio.audio play])
        {
            [nativeAudio audioDetach];
            this->notifyAudioOnLoad(audio, zffalse);
            return;
        }
        [nativeAudio.audio pause];
        this->notifyAudioOnLoad(audio, zftrue);
    }
    virtual void nativeAudioLoadCancel(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio audioDetach];
    }

    virtual void nativeAudioStart(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        if([nativeAudio.audio play])
        {
            this->notifyAudioOnResume(audio);
        }
        else
        {
            [nativeAudio audioDetach];
            this->notifyAudioOnStop(audio, zffalse);
        }
    }
    virtual void nativeAudioStop(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio audioDetach];
    }
    virtual void nativeAudioResume(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio.audio play];
        this->notifyAudioOnResume(audio);
    }
    virtual void nativeAudioPause(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        [nativeAudio.audio pause];
        this->notifyAudioOnPause(audio);
    }

    virtual zftimet nativeAudioDuration(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        return (zftimet)(nativeAudio.audio.duration * 1000);
    }
    virtual zftimet nativeAudioPosition(ZF_IN ZFAudio *audio)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        return (zftimet)(nativeAudio.audio.currentTime * 1000);
    }
    virtual void nativeAudioPosition(ZF_IN ZFAudio *audio,
                                     ZF_IN zftimet position)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio.audio.currentTime = position;
    }

    virtual void nativeAudioVolume(ZF_IN ZFAudio *audio,
                                   ZF_IN zffloat volume)
    {
        _ZFP_ZFAudioImpl_sys_iOS_Audio *nativeAudio = (__bridge _ZFP_ZFAudioImpl_sys_iOS_Audio *)audio->nativeAudio();
        nativeAudio.audio.volume = volume;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAudioImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAudioImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


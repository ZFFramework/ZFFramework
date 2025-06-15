#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_iOS
#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageImpl_sys_iOS, ZFUIImage, v_ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIImage")
public:
    virtual void *nativeImageFromInput(ZF_IN const ZFInput &inputCallback) {
        zfstring dataBuf;
        ZFInputRead(dataBuf, inputCallback);
        if(dataBuf.buffer() == zfnull) {
            return zfnull;
        }
        NSData *nsData = [[NSData alloc] initWithBytesNoCopy:dataBuf.zfunsafe_bufferGiveUp() length:(NSUInteger)dataBuf.length() deallocator:^(void *bytes, NSUInteger length) {
            zfstring::zfunsafe_bufferFree(bytes);
        }];
        UIImage *uiImage = [UIImage imageWithData:nsData scale:ZFUIGlobalStyle::DefaultStyle()->imageScale()];
        if(uiImage == nil) {
            return zfnull;
        }

        return (__bridge_retained void *)uiImage;
    }
    virtual zfbool nativeImageToOutput(
            ZF_IN void *nativeImage
            , ZF_OUT const ZFOutput &outputCallback
            ) {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        NSData *nsData = UIImagePNGRepresentation(uiImage);
        if(nsData == nil) {
            return zffalse;
        }

        const zfbyte *buf = (const zfbyte *)nsData.bytes;
        zfindex size = nsData.length;
        static const zfindex blockSize = 10240;
        const zfbyte *bufEnd = buf + size;
        const zfbyte *bufEndTmp = buf + size - blockSize;
        while(buf < bufEndTmp) {
            if(outputCallback.execute(buf, blockSize) != blockSize) {
                return zffalse;
            }
            buf += blockSize;
        }
        if(buf < bufEnd) {
            zfindex tmpSize = bufEnd - buf;
            if(outputCallback.execute(buf, tmpSize) != tmpSize) {
                return zffalse;
            }
        }

        return zftrue;
    }

    virtual void *nativeImageCopy(ZF_IN void *nativeImage) {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        CGImageRef cgImageNew = CGImageCreateCopy(uiImage.CGImage);
        UIImage *uiImageNew = [UIImage imageWithCGImage:cgImageNew scale:uiImage.scale orientation:uiImage.imageOrientation];
        CGImageRelease(cgImageNew);
        return (__bridge_retained void *)uiImageNew;
    }

    virtual void *nativeImageRetain(ZF_IN void *nativeImage) {
        UIImage *tmp = (__bridge UIImage *)nativeImage;
        return (__bridge_retained void *)tmp;
    }
    virtual void nativeImageRelease(ZF_IN void *nativeImage) {
        UIImage *tmp = (__bridge_transfer UIImage *)nativeImage;
        tmp = nil;
    }

    virtual ZFUISize nativeImageSize(ZF_IN void *nativeImage) {
        UIImage *uiImage = (__bridge UIImage *)nativeImage;
        return ZFUISizeCreate((zffloat)(uiImage.size.width * uiImage.scale), (zffloat)(uiImage.size.height * uiImage.scale));
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


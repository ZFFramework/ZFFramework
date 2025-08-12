#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFEncrypt.h"

#include "../_repo/tiny-AES-c/aes.hpp"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEncryptImpl_default, ZFEncrypt, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFEncrypt")
public:
    virtual zfbool encrypt(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const ZFInput &input
            , ZF_IN const zfstring &key
            ) {
        struct AES_ctx ctx;
        zfbyte iv[AES_BLOCKLEN];
        zfmemset(iv, 0, sizeof(iv));
        AES_init_ctx_iv(&ctx, (const uint8_t *)key.cString(), iv);

        zfbyte buf[AES_BLOCKLEN * 256];
        zfindex read;
        // the first two bytes always store current block's actual size
        while((read = input.execute(buf + 2, AES_BLOCKLEN * 256 - 2)) > 0) {
            buf[0] = ((read >> 8) & 0xFF);
            buf[1] = ((read >> 0) & 0xFF);
            read += 2;
            if((read % AES_BLOCKLEN) != 0) {
                zfindex align = AES_BLOCKLEN - (read % AES_BLOCKLEN);
                zfmemset(buf + read, 0, align);
                read += align;
            }
            AES_CBC_encrypt_buffer(&ctx, buf, read);
            output.execute(buf, read);
        }
        return zftrue;
    }
    virtual zfbool decrypt(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const ZFInput &input
            , ZF_IN const zfstring &key
            ) {
        struct AES_ctx ctx;
        zfbyte iv[AES_BLOCKLEN];
        zfmemset(iv, 0, sizeof(iv));
        AES_init_ctx_iv(&ctx, (const uint8_t *)key.cString(), iv);

        zfbyte buf[AES_BLOCKLEN * 256];
        zfindex read;
        while((read = input.execute(buf, AES_BLOCKLEN * 256)) > 0) {
            if(read <= 2 || (read % AES_BLOCKLEN) != 0) {
                return zffalse;
            }
            AES_CBC_decrypt_buffer(&ctx, buf, read);
            zfindex blockSize = 0
                | (buf[0] << 8) & 0xFF00
                | (buf[1] << 0) & 0xFF
                ;
            if(blockSize == 0 || blockSize > read - 2) {
                return zffalse;
            }
            output.execute(buf + 2, blockSize);
        }
        return zftrue;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEncryptImpl_default)

ZF_NAMESPACE_GLOBAL_END


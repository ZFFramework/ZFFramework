#include "ZFImpl_default_ZFAlgorithm_impl.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFEncrypt.h"

#include "ZFAlgorithm.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFEncryptImpl_default, ZFEncrypt, v_ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFEncrypt")
public:
    virtual zfbool encrypt(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const ZFInput &input
            , ZF_IN const zfstring &key
            ) {
        zfindex inputSize = input.ioSize();
        if(inputSize != zfindexMax()) {
            zfstring sizeCheck = zfstr("%s+", zfsFromInt(inputSize, 16));
            output.execute(sizeCheck.cString(), sizeCheck.length());
            return ZFBase64Encode(output, input, zfnull, this->tableForKey(key));
        }
        else {
            zfstring inputBuf;
            ZFInputRead(inputBuf, input);
            if(inputBuf.buffer() == zfnull) {
                return zffalse;
            }
            zfstring sizeCheck = zfstr("%s+", zfsFromInt(inputBuf.length(), 16));
            output.execute(sizeCheck.cString(), sizeCheck.length());
            return ZFBase64Encode(output, ZFInputForBuffer(inputBuf), zfnull, this->tableForKey(key));
        }
    }
    virtual zfbool decrypt(
            ZF_IN_OUT const ZFOutput &output
            , ZF_IN const ZFInput &input
            , ZF_IN const zfstring &key
            ) {
        zfchar sizeCheckBuf[16] = {0};
        for(zfindex i = 0; i < 10; ++i) {
            if(input.execute(sizeCheckBuf + i, 1) != 1) {
                return zffalse;
            }
            if(sizeCheckBuf[i] == '+') {
                sizeCheckBuf[i] = '\0';
                break;
            }
        }
        zfindex sizeCheck = 0;
        if(!zfsToIntT(sizeCheck, sizeCheckBuf, zfindexMax(), 16)) {
            return zffalse;
        }
        zfindex size = 0;
        if(!ZFBase64Decode(output, input, &size, this->tableForKey(key))) {
            return zffalse;
        }
        return (size == sizeCheck);
    }
private:
    zfstring tableForKey(ZF_IN const zfstring &key) {
        zfstring table = ZFBase64TableDefault();
        zfstring keyTmp = zfstr("%s%s", key, key.length());
        keyTmp = ZFMd5(keyTmp, keyTmp.length());
        const zfbyte *p = (const zfbyte *)keyTmp.cString();
        const zfbyte *pEnd = p + keyTmp.length();
        for( ; p < pEnd; p += 2) {
            zfindex pos0 = (zfindex)(p[0] % 64);
            zfindex pos1 = (zfindex)(p[1] % 64);
            zfchar t = table[pos0];
            table.set(pos0, table[pos1]);
            table.set(pos1, t);
        }
        return table;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFEncryptImpl_default)

ZF_NAMESPACE_GLOBAL_END


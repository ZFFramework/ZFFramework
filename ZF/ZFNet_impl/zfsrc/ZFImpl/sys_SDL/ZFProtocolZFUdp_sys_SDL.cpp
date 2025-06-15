#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFUdp.h"

#if ZF_ENV_sys_SDL

#include "SDL3_net/SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUdpImpl_sys_SDL, ZFUdp, v_ZFProtocolLevel::e_SystemHigh)
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        NET_Init();
    }
    zfoverride
    virtual void protocolOnDealloc(void) {
        NET_Quit();
        zfsuper::protocolOnDealloc();
    }
public:
    virtual void *open(
            ZF_IN ZFUdp *owner
            , ZF_IN zfuint port
            ) {
        return NET_CreateDatagramSocket(zfnull, (Uint16)port);
    }
    virtual void close(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            ) {
        NET_DestroyDatagramSocket((NET_DatagramSocket *)nativeSocket);
    }

public:
    virtual void *addrResolve(
            ZF_IN const zfstring &host
            ) {
        NET_Address *nativeAddr = NET_ResolveHostname(host);
        NET_WaitUntilResolved(nativeAddr, 2000);
        return nativeAddr;
    }
    virtual void addrRelease(ZF_IN void *nativeAddr) {
        NET_UnrefAddress((NET_Address *)nativeAddr);
    }

public:
    virtual zfbool remoteInfo(
            ZF_IN void *nativeAddr
            , ZF_IN_OUT zfstring &remoteAddr
            ) {
        NET_Address *addr = (NET_Address *)nativeAddr;
        const char *result = NET_GetAddressString(addr);
        if(result == zfnull) {
            return zffalse;
        }
        remoteAddr += result;
        return zftrue;
    }

public:
    virtual zfbool send(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN void *nativeAddr
            , ZF_IN zfuint port
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        return NET_SendDatagram(
                (NET_DatagramSocket *)nativeSocket
                , (NET_Address *)nativeAddr
                , (Uint16)port
                , data
                , (int)size
                );
    }
    virtual zfstring recv(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *&nativeAddr
            , ZF_OUT zfuint &port
            , ZF_IN_OPT zftimet timeout
            ) {
        NET_DatagramSocket *socket = (NET_DatagramSocket *)nativeSocket;
        int n = NET_WaitUntilInputAvailable((void **)&socket, 1, (Sint32)timeout);
        if(n <= 0) {
            return zfnull;
        }
        NET_Datagram *recv = zfnull;
        if(!NET_ReceiveDatagram((NET_DatagramSocket *)nativeSocket, &recv)
                || recv == zfnull
                ) {
            return zfnull;
        }
        nativeAddr = (void *)NET_RefAddress(recv->addr);
        port = (zfuint)recv->port;
        zfstring ret((const zfchar *)recv->buf, (zfindex)recv->buflen);
        NET_DestroyDatagram(recv);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUdpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


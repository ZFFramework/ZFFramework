#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFUdp.h"

#if ZF_ENV_sys_SDL

#include "SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUdpImpl_sys_SDL, ZFUdp, ZFProtocolLevel::e_SystemHigh)
public:
    virtual void *open(
            ZF_IN ZFUdp *owner
            , ZF_IN zfuint port
            ) {
        return SDLNet_UDP_Open((Uint16)port);
    }
    virtual void close(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            ) {
        SDLNet_UDP_Close((UDPsocket)nativeSocket);
    }

public:
    virtual void *hostResolve(
            ZF_IN const zfstring &host
            , ZF_IN zfuint port
            ) {
        IPaddress *hostAddr = zfnew(IPaddress);
        SDLNet_ResolveHost(hostAddr, host ? host.cString() : NULL, (Uint16)port);
        return hostAddr;
    }
    virtual void hostRelease(ZF_IN void *hostAddr) {
        zfdelete((IPaddress *)hostAddr);
    }

public:
    virtual zfbool remoteInfo(
            ZF_IN void *hostAddr
            , ZF_OUT zfstring &remoteAddr
            , ZF_OUT zfuint &remotePort
            ) {
        IPaddress *nativeIp = (IPaddress *)hostAddr;
        const zfbyte *pHost = (const zfbyte *)&(nativeIp->host);
        zfstringAppend(remoteAddr, "%s.%s.%s.%s"
            , (zfint)*pHost
            , (zfint)*(pHost + 1)
            , (zfint)*(pHost + 2)
            , (zfint)*(pHost + 3)
            );
        Uint16 nativePort = nativeIp->port;
        if(SDL_BYTEORDER != SDL_BIG_ENDIAN) {
            nativePort = SDL_Swap16(nativePort);
        }
        remotePort = (zfuint)nativePort;
        return zftrue;
    }

public:
    virtual zfbool send(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN void *hostAddr
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        UDPsocket nativeSocketTmp = (UDPsocket)nativeSocket;
        UDPpacket *sdlPacket = SDLNet_AllocPacket((int)size);
        zfmemcpy(&(sdlPacket->address), hostAddr, sizeof(IPaddress));
        zfmemcpy(sdlPacket->data, data, size);
        sdlPacket->len = (int)size;
        zfbool ret = (0 != SDLNet_UDP_Send(nativeSocketTmp, -1, sdlPacket));
        SDLNet_FreePacket(sdlPacket);
        return ret;
    }
    virtual zfindex recv(
            ZF_IN ZFUdp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *&hostAddr
            , ZF_OUT void *data
            , ZF_IN zfindex maxSize
            , ZF_IN_OPT zftimet timeout
            ) {
        UDPsocket nativeSocketTmp = (UDPsocket)nativeSocket;
        if(timeout >= 0) {
            SDLNet_SocketSet ss = SDLNet_AllocSocketSet(1);
            SDLNet_UDP_AddSocket(ss, nativeSocketTmp);
            int canRead = SDLNet_CheckSockets(ss, (Uint32)timeout);
            SDLNet_FreeSocketSet(ss);
            if(canRead == 0) {
                return 0;
            }
        }

        UDPpacket sdlPacket;
        zfmemset(&sdlPacket, 0, sizeof(UDPpacket));
        sdlPacket.data = (Uint8 *)data;
        sdlPacket.len = 0;
        sdlPacket.maxlen = (int)maxSize;
        zfindex recvSize = (zfindex)SDLNet_UDP_Recv(nativeSocketTmp, &sdlPacket);
        IPaddress *hostAddrTmp = zfnew(IPaddress);
        hostAddrTmp->host = sdlPacket.address.host;
        hostAddrTmp->port = sdlPacket.address.port;
        hostAddr = hostAddrTmp;
        return recvSize;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUdpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


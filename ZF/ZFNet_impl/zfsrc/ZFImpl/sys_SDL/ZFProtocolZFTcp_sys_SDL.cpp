#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFTcp.h"

#if ZF_ENV_sys_SDL

#include "SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTcpImpl_sys_SDL, ZFTcp, v_ZFProtocolLevel::e_SystemHigh)
public:
    virtual void *open(
            ZF_IN ZFTcp *owner
            , ZF_IN const zfstring &host
            , ZF_IN zfuint port
            ) {
        IPaddress sdlIp;
        if(0 != SDLNet_ResolveHost(&sdlIp, host ? host.cString() : NULL, (Uint16)port)) {
            return zfnull;
        }
        return SDLNet_TCP_Open(&sdlIp);
    }
    virtual void close(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            ) {
        SDLNet_TCP_Close((TCPsocket)nativeSocket);
    }
    virtual zfbool remoteInfo(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT zfstring &remoteAddr
            , ZF_OUT zfuint &remotePort
            ) {
        IPaddress *nativeIp = SDLNet_TCP_GetPeerAddress((TCPsocket)nativeSocket);
        if(nativeIp != zfnull) {
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
        else {
            return zffalse;
        }
    }
    virtual void *accept(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            ) {
        return SDLNet_TCP_Accept((TCPsocket)nativeSocket);
    }
    virtual zfbool send(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        return ((int)size == SDLNet_TCP_Send((TCPsocket)nativeSocket, data, (int)size));
    }
    virtual zfindex recv(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *data
            , ZF_IN zfindex maxSize
            , ZF_IN_OPT zftimet timeout
            ) {
        if(timeout >= 0) {
            SDLNet_SocketSet ss = SDLNet_AllocSocketSet(1);
            SDLNet_TCP_AddSocket(ss, (TCPsocket)nativeSocket);
            int canRead = SDLNet_CheckSockets(ss, (Uint32)timeout);
            SDLNet_FreeSocketSet(ss);
            if(canRead == 0) {
                return 0;
            }
        }
        return (zfindex)SDLNet_TCP_Recv((TCPsocket)nativeSocket, data, (int)maxSize);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTcpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


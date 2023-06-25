#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFTcp.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/_repo/SDL/SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTcpImpl_sys_SDL, ZFTcp, ZFProtocolLevel::e_SystemHigh)
public:
    virtual void *open(ZF_IN ZFTcp *owner,
                       ZF_IN const zfchar *host,
                       ZF_IN zfuint port,
                       ZF_OUT void *&nativeTcp)
    {
        IPaddress sdlIp;
        if(0 != SDLNet_ResolveHost(&sdlIp, host, (Uint16)port))
        {
            return zfnull;
        }
        return SDLNet_TCP_Open(&sdlIp);
    }
    virtual void destroy(ZF_IN ZFTcp *owner,
                         ZF_IN void *nativeTcp)
    {
        // nothing to do
    }
    virtual void close(ZF_IN ZFTcp *owner,
                       ZF_IN void *socket)
    {
        SDLNet_TCP_Close((TCPsocket)socket);
    }
    virtual void *accept(ZF_IN ZFTcp *owner,
                         ZF_IN void *socket)
    {
        return SDLNet_TCP_Accept((TCPsocket)socket);
    }
    virtual zfbool send(ZF_IN ZFTcp *owner,
                        ZF_IN void *socket,
                        ZF_IN const void *data,
                        ZF_IN zfindex size)
    {
        return ((int)size == SDLNet_TCP_Send((TCPsocket)socket, data, (int)size));
    }
    virtual zfindex recv(ZF_IN ZFTcp *owner,
                         ZF_IN void *socket,
                         ZF_OUT void *data,
                         ZF_IN zfindex maxSize)
    {
        return (zfindex)SDLNet_TCP_Recv((TCPsocket)socket, data, (int)maxSize);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTcpImpl_sys_SDL)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTcpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


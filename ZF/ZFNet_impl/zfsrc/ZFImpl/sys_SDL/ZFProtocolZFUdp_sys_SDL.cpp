#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFUdp.h"

#if ZF_ENV_sys_SDL

#include "ZFImpl/_repo/SDL/SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFUdpImpl_sys_SDL_NativeUdp
{
public:
    UDPsocket nativeUdp;
    UDPpacket *recvPacketCache;
public:
    _ZFP_ZFUdpImpl_sys_SDL_NativeUdp(void)
    : nativeUdp(zfnull)
    , recvPacketCache(zfnull)
    {
    }
    ~_ZFP_ZFUdpImpl_sys_SDL_NativeUdp(void)
    {
        if(this->recvPacketCache != zfnull)
        {
            SDLNet_FreePacket(this->recvPacketCache);
        }
        if(this->nativeUdp != zfnull)
        {
            SDLNet_UDP_Close(this->nativeUdp);
        }
    }
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUdpImpl_sys_SDL, ZFUdp, ZFProtocolLevel::e_SystemHigh)
public:
    virtual zfbool open(ZF_IN ZFUdp *owner,
                        ZF_IN_OUT zfuint &port,
                        ZF_OUT void *&nativeUdp)
    {
        UDPsocket tmp = zfnull;
        if(port != 0)
        {
            tmp = SDLNet_UDP_Open(port);
        }
        else
        {
            for(zfindex i = 0; i < 10; ++i)
            {
                port = zfmRand(1024, 65536);
                tmp = SDLNet_UDP_Open(port);
                if(tmp != zfnull)
                {
                    break;
                }
            }
        }
        if(tmp == zfnull)
        {
            return zffalse;
        }
        else
        {
            _ZFP_ZFUdpImpl_sys_SDL_NativeUdp *nativeUdpTmp = zfnew(_ZFP_ZFUdpImpl_sys_SDL_NativeUdp);
            nativeUdpTmp->nativeUdp = tmp;
            nativeUdp = nativeUdpTmp;
            return zftrue;
        }
    }
    virtual void close(ZF_IN ZFUdp *owner,
                       ZF_IN void *nativeUdp)
    {
        _ZFP_ZFUdpImpl_sys_SDL_NativeUdp *nativeUdpTmp = (_ZFP_ZFUdpImpl_sys_SDL_NativeUdp *)nativeUdp;
        zfdelete(nativeUdpTmp);
    }
    virtual zfbool send(ZF_IN ZFUdp *owner,
                        ZF_IN ZFUdpPacket *packet)
    {
        _ZFP_ZFUdpImpl_sys_SDL_NativeUdp *nativeUdpTmp = (_ZFP_ZFUdpImpl_sys_SDL_NativeUdp *)owner->nativeUdp();
        UDPpacket *sdlPacket = SDLNet_AllocPacket((int)packet->data.bufferSize());
        if(0 != SDLNet_ResolveHost(&sdlPacket->address, packet->host.cString(), (Uint16)packet->port))
        {
            SDLNet_FreePacket(sdlPacket);
            return zffalse;
        }
        zfmemcpy(sdlPacket->data, packet->data.buffer(), packet->data.bufferSize());
        sdlPacket->len = (int)packet->data.bufferSize();
        return 0 != SDLNet_UDP_Send(nativeUdpTmp->nativeUdp, -1, sdlPacket);
    }
    virtual zfautoObjectT<ZFUdpPacket *> recv(ZF_IN ZFUdp *owner)
    {
        _ZFP_ZFUdpImpl_sys_SDL_NativeUdp *nativeUdpTmp = (_ZFP_ZFUdpImpl_sys_SDL_NativeUdp *)owner->nativeUdp();
        if(nativeUdpTmp->recvPacketCache == zfnull)
        {
            nativeUdpTmp->recvPacketCache = SDLNet_AllocPacket(4096);
        }
        if(1 != SDLNet_UDP_Recv(nativeUdpTmp->nativeUdp, nativeUdpTmp->recvPacketCache))
        {
            return zfnull;
        }
        zfblockedAlloc(ZFUdpPacket, packet);
        unsigned char *pHost = (unsigned char *)&(nativeUdpTmp->recvPacketCache->address.host);
        char buffer[20] = { 0 };
        zfstringAppend(packet->host, "%d.%d.%d.%d"
            , (zfint)*pHost
            , (zfint)*(pHost + 1)
            , (zfint)*(pHost + 2)
            , (zfint)*(pHost + 3)
            );
        Uint16 port = nativeUdpTmp->recvPacketCache->address.port;
        if(SDL_BYTEORDER != SDL_BIG_ENDIAN)
        {
            port = SDL_Swap16(port);
        }
        packet->port = (zfuint)port;
        packet->data.bufferCapacity((zfindex)nativeUdpTmp->recvPacketCache->len);
        packet->data.bufferAppend(nativeUdpTmp->recvPacketCache->data, nativeUdpTmp->recvPacketCache->len);
        return packet;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUdpImpl_sys_SDL)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUdpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


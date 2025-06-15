#include "ZFImpl_sys_SDL_ZFNet_impl.h"
#include "ZFNet/protocol/ZFProtocolZFTcp.h"

#if ZF_ENV_sys_SDL

#include "SDL3_net/SDL_net.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTcpImpl_sys_SDL, ZFTcp, v_ZFProtocolLevel::e_SystemHigh)
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
    zfclassNotPOD Token {
    public:
        // server
        //     * remoteAddr : null
        //     * server : server
        //     * client : null
        // client
        //     * remoteAddr : server's addr
        //     * server : null
        //     * client : client
        // server accept
        //     * remoteAddr : client's addr
        //     * server : null
        //     * client : recv client
        NET_Address *remoteAddr;
        NET_Server *server;
        NET_StreamSocket *client;
    };

public:
    virtual void *open(
            ZF_IN ZFTcp *owner
            , ZF_IN const zfstring &host
            , ZF_IN zfuint port
            , ZF_IN zftimet timeout
            ) {
        if(!host) {
            NET_Server *impl = NET_CreateServer(zfnull, (Uint16)port);
            if(impl == zfnull) {
                return zfnull;
            }
            Token *token = zfpoolNew(Token);
            token->remoteAddr = zfnull;
            token->server = impl;
            token->client = zfnull;
            return token;
        }
        else {
            NET_Address *addr = NET_ResolveHostname(host);
            if(addr == zfnull) {
                return zfnull;
            }
            NET_WaitUntilResolved(addr, timeout > 0 ? (Sint32)timeout : (Sint32)-1);
            NET_StreamSocket *impl = NET_CreateClient(addr, (Uint16)port);
            if(impl == zfnull) {
                NET_UnrefAddress(addr);
                return zfnull;
            }
            if(timeout > 0) {
                if(NET_WaitUntilConnected(impl, (Sint32)timeout) != 1) {
                    NET_DestroyStreamSocket(impl);
                    NET_UnrefAddress(addr);
                    return zfnull;
                }
            }
            Token *token = zfpoolNew(Token);
            token->remoteAddr = addr;
            token->server = zfnull;
            token->client = impl;
            return token;
        }
    }
    virtual void close(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            ) {
        Token *token = (Token *)nativeSocket;
        if(owner->type() == v_ZFTcpType::e_Server) {
            if(token->server) {
                NET_DestroyServer(token->server);
            }
        }
        else {
            if(token->client) {
                NET_DestroyStreamSocket(token->client);
            }
        }
        if(token->remoteAddr) {
            NET_UnrefAddress(token->remoteAddr);
        }
        zfpoolDelete(token);
    }
    virtual zfbool remoteInfo(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN_OUT zfstring &remoteAddr
            ) {
        Token *token = (Token *)nativeSocket;
        if(token->remoteAddr == zfnull) {
            return zffalse;
        }
        const char *result = NET_GetAddressString(token->remoteAddr);
        if(result == zfnull) {
            return zffalse;
        }
        remoteAddr += result;
        return zftrue;
    }
    virtual void *accept(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN_OPT zftimet timeout = -1
            ) {
        Token *token = (Token *)nativeSocket;
        if(timeout != 0) {
            int n = NET_WaitUntilInputAvailable(
                    (void **)&(token->server)
                    , 1
                    , timeout > 0 ? (Sint32)timeout : (Sint32)-1
                    );
            if(n <= 0) {
                return zfnull;
            }
        }
        NET_StreamSocket *client = zfnull;
        if(!NET_AcceptClient(token->server, &client)) {
            return zfnull;
        }
        Token *clientToken = zfpoolNew(Token);
        clientToken->remoteAddr = NET_GetStreamSocketAddress(client);
        clientToken->server = zfnull;
        clientToken->client = client;
        return clientToken;
    }
    virtual zfbool send(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_IN const void *data
            , ZF_IN zfindex size
            ) {
        Token *token = (Token *)nativeSocket;
        return NET_WriteToStreamSocket(token->client, data, (int)size);
    }
    virtual zfindex recv(
            ZF_IN ZFTcp *owner
            , ZF_IN void *nativeSocket
            , ZF_OUT void *data
            , ZF_IN zfindex maxSize
            , ZF_IN_OPT zftimet timeout
            ) {
        Token *token = (Token *)nativeSocket;
        int result = NET_ReadFromStreamSocket(token->client, data, (int)maxSize);
        if(result < 0) {
            return zfindexMax();
        }
        else if(result > 0) {
            return (zfindex)result;
        }
        if(timeout == 0) {
            return 0;
        }

        zftimet wait = 0;
        do {
            ZFThread::sleep(100);
            wait += 100;
            result = NET_ReadFromStreamSocket(token->client, data, (int)maxSize);
            if(result < 0) {
                return zfindexMax();
            }
            else if(result > 0) {
                return (zfindex)result;
            }
        } while(timeout < 0 || wait < timeout);
        return 0;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTcpImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


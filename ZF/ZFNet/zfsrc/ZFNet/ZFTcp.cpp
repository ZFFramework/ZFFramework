#include "ZFTcp.h"
#include "protocol/ZFProtocolZFTcp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFTcpPrivate
{
public:
    void *nativeTcp;
    zfstring host;
    zfuint port;
    void *openSocket;
    ZFCoreArrayPOD<void *> socketList;
public:
    _ZFP_ZFTcpPrivate(void)
    : nativeTcp(zfnull)
    , host()
    , port(0)
    , openSocket(zfnull)
    , socketList()
    {
    }
};

void ZFTcp::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    ret += "tcp:";
    zfstringAppend(ret, "%s:%u", this->host(), this->port());
}

void ZFTcp::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTcpPrivate);
}
void ZFTcp::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_2(ZFTcp, void *, open,
                  ZFMP_IN(const zfchar *, host),
                  ZFMP_IN(zfuint, port))
{
    if(d->port != 0)
    {
        this->close(zfnull);
    }
    if(zfstringIsEmpty(host))
    {
        host = zfnull;
        d->host = "localhost";
    }
    else
    {
        d->host = host;
    }
    d->port = port;
    d->nativeTcp = zfnull;
    d->openSocket = ZFPROTOCOL_ACCESS(ZFTcp)->open(this, host, port, d->nativeTcp);
    if(d->openSocket == zfnull)
    {
        d->host.removeAll();
        d->port = 0;
        d->nativeTcp = zfnull;
        return zfnull;
    }
    zfRetain(this);
    return d->openSocket;
}
ZFMETHOD_DEFINE_1(ZFTcp, void, close,
                  ZFMP_IN(void *, socket))
{
    if(d->openSocket != zfnull)
    {
        if(socket == zfnull)
        {
            socket = d->openSocket;
        }
        if(socket != d->openSocket)
        {
            zfCoreMutexLocker();
            zfindex index = d->socketList.find(socket);
            if(index == zfindexMax())
            {
                return;
            }
            d->socketList.remove(index);
        }

        ZFPROTOCOL_ACCESS(ZFTcp)->close(this, socket);

        if(socket == d->openSocket)
        {
            do {
                zfCoreMutexLock();
                void *toClose = zfnull;
                if(!d->socketList.isEmpty())
                {
                    toClose = d->socketList.removeLastAndGet();
                }
                zfCoreMutexUnlock();

                if(toClose == zfnull)
                {
                    break;
                }
                ZFPROTOCOL_ACCESS(ZFTcp)->close(this, toClose);
            } while(zftrue);

            d->nativeTcp = zfnull;
            d->host.removeAll();
            d->port = 0;
            d->openSocket = zfnull;
            zfRelease(this);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFTcp, const zfchar *, host)
{
    return d->host.isEmpty() ? zfnull : d->host.cString();
}
ZFMETHOD_DEFINE_0(ZFTcp, zfuint, port)
{
    return d->port;
}

ZFMETHOD_DEFINE_0(ZFTcp, void *, accept)
{
    void *socket = ZFPROTOCOL_ACCESS(ZFTcp)->accept(this, d->openSocket);
    if(socket != zfnull)
    {
        zfCoreMutexLocker();
        d->socketList.add(socket);
    }
    return socket;
}

ZFMETHOD_DEFINE_3(ZFTcp, zfbool, send,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN(const void *, data),
                  ZFMP_IN(zfindex, size))
{
    if(socket == zfnull)
    {
        socket = d->openSocket;
    }
    return ZFPROTOCOL_ACCESS(ZFTcp)->send(this, socket, data, size);
}
ZFMETHOD_DEFINE_3(ZFTcp, zfbool, send,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN(const zfchar *, data),
                  ZFMP_IN_OPT(zfindex, size, zfindexMax()))
{
    return this->send(socket, (const void *)data, (size == zfindexMax() ? zfslen(data) : size) * sizeof(zfchar));
}
ZFMETHOD_DEFINE_2(ZFTcp, zfbool, send,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN(const ZFBuffer &, data))
{
    return this->send(socket, data.buffer(), data.bufferSize());
}
ZFMETHOD_DEFINE_2(ZFTcp, zfbool, send,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN(const ZFInput &, input))
{
    ZFBuffer buf;
    zfindex size = input.ioSize();
    if(size != zfindexMax() && size <= 4096)
    {
        buf.bufferCapacity(size);
        buf.bufferSize(input.execute(buf.buffer(), size));
        if(buf.bufferSize() != size)
        {
            return zffalse;
        }
        return this->send(socket, buf.buffer(), buf.bufferSize());
    }

    const zfindex bufSize = 1024;
    buf.bufferCapacity(bufSize);
    do {
        buf.bufferSize(input.execute(buf.buffer(), bufSize));
        if(!this->send(socket, buf.buffer(), buf.bufferSize()))
        {
            return zffalse;
        }
    } while(buf.bufferSize() == bufSize);
    return zftrue;
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN_OUT(ZFBuffer &, data),
                  ZFMP_IN(zfindex, maxSize))
{
    if(socket == zfnull)
    {
        socket = d->openSocket;
    }
    if(maxSize == zfindexMax())
    {
        maxSize = 4096;
    }
    data.bufferCapacity(data.bufferSize() + maxSize);
    return ZFPROTOCOL_ACCESS(ZFTcp)->recv(this, socket, data.bufferT<zfbyte *>() + data.bufferSize(), maxSize);
}
ZFMETHOD_DEFINE_3(ZFTcp, zfindex, recv,
                  ZFMP_IN(void *, socket),
                  ZFMP_IN_OUT(const ZFOutput &, output),
                  ZFMP_IN(zfindex, maxSize))
{
    const zfindex bufSize = 1024;
    if(maxSize <= bufSize)
    {
        ZFBuffer buf;
        buf.bufferCapacity(maxSize);
        zfindex size = this->recv(socket, buf, maxSize);
        return output.execute(buf.buffer(), size);
    }

    ZFBuffer buf;
    buf.bufferCapacity(bufSize);
    zfindex sizeRead = 0;
    zfindex size;
    zfindex sizeToRead;
    do
    {
        sizeToRead = zfmMin(bufSize, maxSize);
        buf.bufferSize(0);
        size = this->recv(socket, buf, sizeToRead);
        size = output.execute(buf.buffer(), size);
        sizeRead += size;
        if(size < sizeToRead || sizeRead >= maxSize)
        {
            break;
        }
    } while(zftrue);
    return sizeRead;
}

ZFMETHOD_DEFINE_0(ZFTcp, void *, nativeTcp)
{
    return d->nativeTcp;
}

ZF_NAMESPACE_GLOBAL_END


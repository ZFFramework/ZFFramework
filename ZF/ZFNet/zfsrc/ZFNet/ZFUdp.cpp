#include "ZFUdp.h"
#include "protocol/ZFProtocolZFUdp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUdpPacket)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUdpPacket, zfstring, host)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUdpPacket, zfuint, port)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUdpPacket, ZFBuffer, data)

void ZFUdpPacket::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    ret += "UdpPacket:";
    ret += this->host;
    ret += ":";
    zfstringAppend(ret, "%u", this->port);
    ret += "[";
    zfstringAppend(ret, "%zi", this->data.bufferSize());
    if(this->data.bufferSize() > 0)
    {
        ret += ":";
        const zfindex limit = 16;
        for(zfindex i = 0; i < limit && i < this->data.bufferSize(); ++i)
        {
            zfstringAppend(ret, "%02x", this->data.bufferT<zfbyte *>()[i]);
        }
        if(this->data.bufferSize() > limit)
        {
            ret += " ..";
        }
    }
    ret += "]";
}

// ============================================================
zfclassNotPOD _ZFP_ZFUdpPrivate
{
public:
    void *nativeUdp;
    zfuint port;
};

void ZFUdp::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    ret += "udp:";
    zfstringAppend(ret, "%u", this->port());
}

void ZFUdp::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUdpPrivate);
    d->nativeUdp = zfnull;
    d->port = 0;
}
void ZFUdp::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_1(ZFUdp, zfbool, open,
                  ZFMP_IN_OPT(zfuint, port, 0))
{
    if(d->port != 0)
    {
        this->close();
    }
    d->port = port;
    d->nativeUdp = zfnull;
    if(!ZFPROTOCOL_ACCESS(ZFUdp)->open(this, d->port, d->nativeUdp))
    {
        d->port = 0;
        d->nativeUdp = zfnull;
        return zffalse;
    }
    zfCoreAssert(d->port != 0);
    zfRetain(this);
    return zftrue;
}
ZFMETHOD_DEFINE_0(ZFUdp, void, close)
{
    if(d->port != 0)
    {
        ZFPROTOCOL_ACCESS(ZFUdp)->close(this, d->nativeUdp);
        d->nativeUdp = zfnull;
        d->port = 0;
        zfRelease(this);
    }
}
ZFMETHOD_DEFINE_0(ZFUdp, zfuint, port)
{
    return d->port;
}

ZFMETHOD_DEFINE_1(ZFUdp, zfbool, send,
                  ZFMP_IN(ZFUdpPacket *, packet))
{
    if(packet == zfnull || packet->host.isEmpty() || packet->port == 0)
    {
        return zffalse;
    }
    return ZFPROTOCOL_ACCESS(ZFUdp)->send(this, packet);
}
ZFMETHOD_DEFINE_0(ZFUdp, zfautoObjectT<ZFUdpPacket *>, recv)
{
    zfCoreAssertWithMessageTrim(d->port != 0, "socket not opened");
    return ZFPROTOCOL_ACCESS(ZFUdp)->recv(this);
}

ZFMETHOD_DEFINE_0(ZFUdp, void *, nativeUdp)
{
    return d->nativeUdp;
}

ZF_NAMESPACE_GLOBAL_END


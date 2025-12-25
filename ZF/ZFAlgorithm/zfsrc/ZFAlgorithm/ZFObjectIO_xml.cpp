#include "ZFObjectIO_xml.h"
#include "ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(xml, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "xml";
    }, {
        return ZFObjectFromXmlT(ret, input, outErrorHint);
    }, {
        return ZFObjectToXml(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END


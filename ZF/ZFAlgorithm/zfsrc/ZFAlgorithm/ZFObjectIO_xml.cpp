#include "ZFObjectIO_xml.h"
#include "ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(xml, ZFLevelZFFrameworkPostNormal, {
        return fileExt == "xml";
    }, {
        return ZFObjectFromXmlT(ret, input, errorHint);
    }, {
        return ZFObjectToXml(output, obj, errorHint);
    })

ZF_NAMESPACE_GLOBAL_END


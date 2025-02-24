#include "ZFObjectIO_xml.h"
#include "ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(xml, {
        zfstring fileExt = ZFFileExtOf(ZFPathInfoToFileName(pathInfo));
        if(!fileExt) {
            return zffalse;
        }
        zfstringToLowerT(fileExt);
        return fileExt == "xml";
    }, {
        return ZFObjectFromXml(ret, input, outErrorHint);
    }, {
        return ZFObjectToXml(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END


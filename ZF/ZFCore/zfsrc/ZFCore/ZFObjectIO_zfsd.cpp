#include "ZFObjectIO_zfsd.h"
#include "ZFFile_util.h"
#include "ZFFile_pathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, {
        zfstring fileExt = ZFFileExtOf(ZFPathInfoToFileName(pathInfo));
        if(!fileExt) {
            return zffalse;
        }
        zfstringToLowerT(fileExt);
        return fileExt == "zfsd";
    }, {
        return ZFObjectFromZfsd(ret, input, outErrorHint);
    }, {
        return ZFObjectToZfsd(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END


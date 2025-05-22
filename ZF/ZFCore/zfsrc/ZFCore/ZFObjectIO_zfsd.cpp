#include "ZFObjectIO_zfsd.h"
#include "ZFFile_util.h"
#include "ZFFile_pathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, {
        zfstring fileExt;
        if(!ZFPathInfoToFileName(fileExt, pathInfo)
                || !ZFFileExtOfT(fileExt, fileExt)
                || !fileExt
                ) {
            return zffalse;
        }
        zfstringToLowerT(fileExt);
        return fileExt == "zfsd";
    }, {
        return ZFObjectFromZFSD(ret, input, outErrorHint);
    }, {
        return ZFObjectToZFSD(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END


#include "ZFIOUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_3(zfautoT<ZFTaskId>, ZFIORemoveOutdate
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        , ZFMP_IN_OPT(zftimet, outdate, zftimetOneWeek())
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zfnull;
    }
    ZFLISTENER_4(impl
            , zfautoT<ZFIOImpl>, ioImpl
            , ZFPathInfo, pathInfo
            , ZFListener, filter
            , zftimet, outdate
            ) {
        if(!zfargs.param0()) {
            return;
        }
        zftimet curTime = ZFTime::currentTime();
        ZFArgs zfargsOwner = zfargs;
        ZFLISTENER_5(onFile
                , zfautoT<ZFIOImpl>, ioImpl
                , ZFArgs, zfargsOwner
                , ZFListener, filter
                , zftimet, outdate
                , zftimet, curTime
                ) {
            if(!zfargsOwner.param0()) {
                zfargs.eventFiltered(zftrue);
                return;
            }
            const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
            if(curTime - outdate > ioImpl->ioModTime(pathInfo.pathData())) {
                filter.execute(zfargs);
                if(zfargs.result() == zfnull) {
                    ioImpl->ioRemove(pathInfo.pathData());
                }
                else {
                    zfargs.result(zfnull);
                }
            }
        } ZFLISTENER_END()
        ZFIOForEachFile(pathInfo, onFile);
    } ZFLISTENER_END()
    return zfasync(impl);
}

ZFMETHOD_FUNC_DEFINE_2(zfautoT<ZFTaskId>, ZFIORemoveEmptyDir
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, filter, zfnull)
        ) {
    zfautoT<ZFIOImpl> ioImpl = ZFIOImplForPathType(pathInfo.pathType());
    if(!ioImpl) {
        return zfnull;
    }
    ZFLISTENER_3(impl
            , zfautoT<ZFIOImpl>, ioImpl
            , ZFPathInfo, pathInfo
            , ZFListener, filter
            ) {
        if(!zfargs.param0()) {
            return;
        }
        ZFArgs zfargsOwner = zfargs;
        ZFLISTENER_3(onFile
                , zfautoT<ZFIOImpl>, ioImpl
                , ZFArgs, zfargsOwner
                , ZFListener, filter
                ) {
            if(!zfargsOwner.param0()) {
                zfargs.eventFiltered(zftrue);
                return;
            }
            const ZFPathInfo &pathInfo = zfargs.param0().to<v_ZFPathInfo *>()->zfv;
            ZFIOFindData fd;
            if(!ioImpl->ioFindFirst(fd, pathInfo.pathData())) {
                filter.execute(zfargs);
                if(zfargs.result() == zfnull) {
                    ioImpl->ioRemove(pathInfo.pathData());
                    zfargs.param0(zfnull);
                }
                else {
                    zfargs.result(zfnull);
                }
            }
            else {
                ioImpl->ioFindClose(fd);
            }
        } ZFLISTENER_END()
        ZFIOForEachDir(pathInfo, onFile);
    } ZFLISTENER_END()
    return zfasync(impl);
}

ZF_NAMESPACE_GLOBAL_END


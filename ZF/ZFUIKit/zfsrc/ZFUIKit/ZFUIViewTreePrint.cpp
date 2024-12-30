#include "ZFUIViewTreePrint.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTreeNative.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_3(void, ZFUIViewTreePrintAfterDelay
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        ) {
    ZFLISTENER_2(action
            , zfweakT<ZFUIView>, view
            , ZFOutput, outputCallback
            ) {
        if(view != zfnull) {
            ZFUIViewTreePrint(view, outputCallback);
        }
    } ZFLISTENER_END()
    ZFTimerOnce(delay, action);
}

// ============================================================
zfclassPOD _ZFP_ZFUIViewTreePrintData {
public:
    const ZFClass *viewClass;
    ZFListener viewInfoGetter;
};

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewTreePrintDataHolder, ZFLevelZFFrameworkEssential) {
}
public:
    // ordered by:
    // ZFUIView, classBaseA, classChildA, classBaseB, classChildB
    ZFCoreArray<_ZFP_ZFUIViewTreePrintData> datas;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewTreePrintDataHolder)

zfclassPOD _ZFP_ZFUIViewTreePrintPrintData {
public:
    ZFUIView *view;
    zfindex depth;
    zfindex siblingIndex;
    ZFUIViewChildLayer layer;
};

ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreePrint
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        ) {
    if(view == zfnull || !outputCallback) {
        return;
    }

    ZFOutput outputCallbackNoEndl = ZFOutputForFormat(outputCallback, zfobj<ZFOutputFormatBasic>()->c_removeEndl(zftrue));

    ZFCoreArray<_ZFP_ZFUIViewTreePrintPrintData> printDatas;
    _ZFP_ZFUIViewTreePrintPrintData rootPrintData;
    rootPrintData.view = view;
    rootPrintData.depth = 0;
    rootPrintData.siblingIndex = 0;
    rootPrintData.layer = ZFUIViewChildLayer::e_Normal;
    printDatas.add(rootPrintData);

    outputCallback.execute("====================== view tree begin =====================\n");
    do {
        _ZFP_ZFUIViewTreePrintPrintData printData = printDatas.removeLastAndGet();

        // all children
        ZFCoreArray<zfautoT<ZFUIView> > implViews = printData.view->internalImplViewArray();
        ZFCoreArray<zfautoT<ZFUIView> > bgViews = printData.view->internalBgViewArray();
        ZFCoreArray<zfautoT<ZFUIView> > normalViews = printData.view->childArray();
        ZFCoreArray<zfautoT<ZFUIView> > fgViews = printData.view->internalFgViewArray();
        for(zfindex i = fgViews.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = fgViews.get(i);
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalFg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = normalViews.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = normalViews.get(i);
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_Normal;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = bgViews.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = bgViews.get(i);
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalBg;
            printDatas.add(printDataTmp);
        }
        for(zfindex i = implViews.count() - 1; i != zfindexMax(); --i) {
            _ZFP_ZFUIViewTreePrintPrintData printDataTmp;
            printDataTmp.view = implViews.get(i);
            printDataTmp.depth = printData.depth + 1;
            printDataTmp.siblingIndex = i;
            printDataTmp.layer = ZFUIViewChildLayer::e_InternalImpl;
            printDatas.add(printDataTmp);
        }

        outputCallback.execute("|");
        outputCallback.execute(zfstr("%2s", printData.siblingIndex));
        for(zfindex i = printData.depth - 1; i != zfindexMax(); --i) {
            outputCallback.execute(" |");
        }

        switch(printData.layer) {
            case ZFUIViewChildLayer::e_Normal:
                outputCallback.execute(" ");
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                outputCallback.execute(" impl ");
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                outputCallback.execute(" bg ");
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                outputCallback.execute(" fg ");
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }

        ZFCoreArray<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
        zfbool exist = zffalse;
        for(zfindex i = datas.count() - 1; i != zfindexMax(); --i) {
            if(printData.view->classData()->classIsTypeOf(datas[i].viewClass)) {
                datas[i].viewInfoGetter.execute(ZFArgs()
                        .sender(printData.view)
                        .param0(zfobj<v_ZFOutput>(outputCallbackNoEndl))
                        );
                exist = zftrue;
                break;
            }
        }
        if(!exist) {
            zfstring tmp;
            printData.view->objectInfoT(tmp);
            outputCallbackNoEndl.execute(tmp, tmp.length());
        }
        outputCallback.execute("\n");
    } while(!printDatas.isEmpty());
    outputCallback.execute("====================== view tree  end  =====================\n");
}

void ZFUIViewTreePrintInfoGetterForClass(
        ZF_IN const ZFClass *viewClass
        , ZF_IN const ZFListener &viewInfoGetter
        ) {
    ZFCoreAssert(viewClass != zfnull && viewClass->classIsTypeOf(ZFUIView::ClassData()));
    ZFCoreArray<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    if(viewInfoGetter == zfnull) {
        for(zfindex i = 0; i < datas.count(); ++i) {
            const ZFClass *cls = datas[i].viewClass;
            if(viewClass == cls) {
                datas.remove(i);
                break;
            }
        }
        return;
    }

    zfindex indexAddTo = zfindexMax();
    for(zfindex i = 0; i < datas.count(); ++i) {
        const ZFClass *cls = datas[i].viewClass;
        if(viewClass == cls) {
            datas[i].viewInfoGetter = viewInfoGetter;
            break;
        }
        else if(cls->classIsTypeOf(viewClass)) {
            indexAddTo = i;
            break;
        }
        else if(viewClass->classIsTypeOf(cls)) {
            indexAddTo = i + 1;
        }
    }

    _ZFP_ZFUIViewTreePrintData data;
    data.viewClass = viewClass;
    data.viewInfoGetter = viewInfoGetter;
    datas.add(data, indexAddTo);
}
ZFListener ZFUIViewTreePrintInfoGetterForClass(ZF_IN const ZFClass *viewClass) {
    ZFCoreArray<_ZFP_ZFUIViewTreePrintData> &datas = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewTreePrintDataHolder)->datas;
    for(zfindex i = datas.count() - 1; i != zfindexMax(); --i) {
        if(datas[i].viewClass == viewClass) {
            return datas[i].viewInfoGetter;
        }
    }
    return zfnull;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_2(void, ZFUIViewTreeNativePrint
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        ) {
    if(!outputCallback) {
        return;
    }
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTreeNative) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTreeNative);
    if(impl != zfnull) {
        impl->viewTreeNative(view, outputCallback);
    }
    else {
        outputCallback
            << "========== ZFUIViewTreeNativePrint begin ==========\n"
            << "| not available\n"
            << "========== ZFUIViewTreeNativePrint  end  ==========\n"
            ;
    }
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFUIViewTreeNativePrintAfterDelay
        , ZFMP_IN(zftimet, delay)
        , ZFMP_IN(ZFUIView *, view)
        , ZFMP_IN_OPT(const ZFOutput &, outputCallback, ZFOutputDefault())
        ) {
    ZFLISTENER_2(action
            , zfweakT<ZFUIView>, view
            , ZFOutput, outputCallback
            ) {
        if(view != zfnull) {
            ZFUIViewTreeNativePrint(view, outputCallback);
        }
    } ZFLISTENER_END()
    ZFTimerOnce(delay, action);
}

ZF_NAMESPACE_GLOBAL_END


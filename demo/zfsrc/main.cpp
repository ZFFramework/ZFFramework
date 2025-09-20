#include "ZFFramework_test_global.h"
#include "ZFFramework_test/ZFUIKit_test/ZFUIKit_test.h"
#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"
#include "ZFUIWebKit.h"
#include "ZFNet.h"
#include "ZFLua.h"
#include "ZFAppUtil.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"

static zfbool _ZFP_ZFFramework_test_luaTest(void);
static zfbool _ZFP_ZFFramework_test_protocolCheck(void);
static zfauto _ZFP_ZFFramework_test_containerViewPrepare(void);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView);

ZFMAIN_ENTRY() {
    ZFLISTENER(onLoad) {
        {
            ZFPathInfo custom;
            if(ZFPathInfoFromStringT(custom, ZFState::instance()->get("ZFCustomResExt"))) {
                ZFResExtPathAdd(custom);
            }
        }
        if(!_ZFP_ZFFramework_test_luaTest() && _ZFP_ZFFramework_test_protocolCheck()) {
            zfauto containerView = _ZFP_ZFFramework_test_containerViewPrepare();
            _ZFP_ZFFramework_test_prepareTestCase(containerView);
        }
    } ZFLISTENER_END()
    ZFState::instance()->load(onLoad);
}

static zfbool _ZFP_ZFFramework_test_luaTest(void) {
    ZFResExtPathAdd(ZFPathInfo(ZFPathType_file(), ZFPathForStorage()));
    ZFResExtPathAdd(ZFPathInfo(ZFPathType_file(), ZFPathForStorageShared()));
    ZFLogTrim() << "external res: " << ZFResExtPathList();

    ZFInput src = ZFInputForRes("zf.lua");
    if(src) {
        ZFLogTrim("redirect to %s", src.callbackId());
        ZFLuaExecute(src);
        ZFLuaGC();
    }
    return src;
}

static zfbool _ZFP_ZFFramework_test_protocolCheck(void) {
    {
        ZFCoreArray<ZFProtocolImplInfo> implDatas = ZFProtocolImplInfoGetAllNotImplemented();
        if(!implDatas.isEmpty()) {
            zfclassNotPOD _ZFP_main_ZFProtocolImplInfo_sort {
            public:
                static ZFCompareResult action(
                        ZF_IN ZFProtocolImplInfo const &v0
                        , ZF_IN ZFProtocolImplInfo const &v1
                        ) {
                    return ZFComparerDefault(v0.protocolName, v1.protocolName);
                }
            };
            implDatas.sort(0, zfindexMax(), _ZFP_main_ZFProtocolImplInfo_sort::action);

            ZFLogTrim() << "note, these protocol has not been implemented:";
            for(zfindex i = 0; i < implDatas.count(); ++i) {
                ZFOutput output = ZFLogTrim();
                output << "    ";
                ZFProtocolImplInfoPrint(implDatas[i], output);
            }
        }
    }
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIView)
       || !ZFPROTOCOL_IS_AVAILABLE(ZFUISysWindow)
    ) {
        ZFTestCaseRunAllStart();
        return zffalse;
    }
    return zftrue;
}
static zfauto _ZFP_ZFFramework_test_containerViewPrepare(void) {
    zfobj<ZFUIWindow> window;
    window->show();

    zfobj<ZFUIKit_test_ListView> containerView;
    window->child(containerView)->c_sizeFill();

    ZFUIViewFocusNextMove(window);
    return containerView;
}

static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(
        ZF_IN ZFUIView *containerView
        , ZF_IN const zfstring &subModuleName
        , ZF_IN ZFCoreArray<const ZFClass *> const &testCases
        );
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(
        ZF_IN ZFUIView *containerView
        , ZF_IN const zfstring &subModuleName
        , ZF_IN const ZFClass *testCase
        );
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView) {
    ZFCoreArray<const ZFClass *> allTestCase = ZFTestCaseGetAll();
    zfclassNotPOD _ZFP_main_class_sort {
    public:
        static ZFCompareResult action(
                ZF_IN const ZFClass * const &cls0
                , ZF_IN const ZFClass * const &cls1
                ) {
            return ZFComparerDefault(cls0->classNameFull(), cls1->classNameFull());
        }
    };
    allTestCase.sort(0, zfindexMax(), _ZFP_main_class_sort::action);

    while(!allTestCase.isEmpty()) {
        ZFCoreArray<const ZFClass *> subModule;
        zfstring subModuleName = allTestCase[0]->classNameFull();
        {
            zfindex t = zfstringFind(subModuleName, "_");
            if(t == zfindexMax()) {
                subModuleName.removeAll();
            }
            else {
                subModuleName = zfstring(subModuleName, t);
            }
        }
        for(zfindex i = 0; i < allTestCase.count(); ++i) {
            if(zfstringBeginWith(allTestCase[i]->classNameFull(), subModuleName)) {
                subModule.add(allTestCase[i]);
                allTestCase.remove(i);
                --i;
            }
        }
        _ZFP_ZFFramework_test_prepareTestCaseSubModule(containerView, subModuleName, subModule);
    }
}
zfclass _ZFP_ZFFramework_test_TestCaseSubModuleData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_ZFFramework_test_TestCaseSubModuleData, ZFObject)

    zfstring subModuleName;
    ZFCoreArray<const ZFClass *> testCases;
};
static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(
        ZF_IN ZFUIView *containerView
        , ZF_IN const zfstring &subModuleName
        , ZF_IN ZFCoreArray<const ZFClass *> const &testCases
        ) {
    zfobj<ZFUIKit_test_Button> button;
    containerView->child(button);

    zfobj<_ZFP_ZFFramework_test_TestCaseSubModuleData> subModuleData;
    subModuleData->subModuleName = subModuleName;
    subModuleData->testCases = testCases;

    ZFLISTENER_1(onClickButton
            , zfautoT<_ZFP_ZFFramework_test_TestCaseSubModuleData>, subModuleData
            ) {
        zfobj<ZFUIWindow> subModuleWindow;
        subModuleWindow->bgColor(ZFUIColorWhite());
        subModuleWindow->show();
        zfobj<ZFUIKit_test_ListView> containerView;
        subModuleWindow->child(containerView)->c_sizeFill();

        {
            zfobj<ZFUIKit_test_Button> closeButton;
            containerView->child(closeButton);
            closeButton->label()->text("back");
            ZFLISTENER_1(closeButtonOnClick
                    , ZFUIWindow *, subModuleWindow
                    ) {
                subModuleWindow->hide();
            } ZFLISTENER_END()
            closeButton->observerAdd(ZFUIButton::E_ButtonOnClick(), closeButtonOnClick);
            closeButton->bg()->bgColor(ZFUIColorRed());

            zfobj<ZFUIView> separator;
            containerView->child(separator);
            separator->viewSizeMin(ZFUISizeCreate(0, 5));
            separator->viewSizeMax(ZFUISizeCreate(-1, 5));
            separator->bgColor(ZFUIColorGray());
        }

        for(zfindex i = 0; i < subModuleData->testCases.count(); ++i) {
            _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(containerView, subModuleData->subModuleName, subModuleData->testCases[i]);
        }
        ZFUIViewFocusNextMove(subModuleWindow);
    } ZFLISTENER_END()
    button->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickButton);
    button->label()->text(subModuleName);
}
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(
        ZF_IN ZFUIView *containerView
        , ZF_IN const zfstring &subModuleName
        , ZF_IN const ZFClass *testCase
        ) {
    zfobj<ZFUIKit_test_Button> button;
    containerView->child(button);

    ZFLISTENER_2(onClickButton
            , const ZFClass *, testCase
            , ZFUIView *, containerView
            ) {
        containerView->viewUIEnableTree(zffalse);
        zfautoT<ZFTestCase> running = ZFTestCaseRun(testCase);
        if(running != zfnull) {
            ZFLISTENER_1(testCaseOnStop
                    , ZFUIView *, containerView
                    ) {
                containerView->viewUIEnableTree(zftrue);
            } ZFLISTENER_END()
            running->observerAdd(ZFTestCase::E_TestCaseOnStop(), testCaseOnStop);
        }
        else {
            containerView->viewUIEnableTree(zftrue);
        }
    } ZFLISTENER_END()
    button->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickButton);
    button->label()->text(zfstring(testCase->classNameFull() + zfslen(subModuleName) + 1));
}

// ============================================================
ZFMAIN_PARAM_DISPATCH(LuaRunner) {
    if(ZFApp::appParams().isEmpty() || !ZFRegExpMatch(ZFApp::appParams()[0], ".*\\.lua$")) {
        return;
    }
    zfargs.eventFiltered(zftrue);

    const ZFCoreArray<zfstring> &appParams = ZFApp::appParams();
    ZFPathInfo pathInfo;
    if(!ZFPathInfoFromStringT(pathInfo, appParams[0])) {
        pathInfo.pathType(ZFPathType_file());
        pathInfo.pathData(appParams[0]);
    }
    ZFCoreArray<zfauto> luaParams;
    for(zfindex i = 1; i < appParams.count(); ++i) {
        luaParams.add(zfobj<v_zfstring>(appParams[i]));
    }
    ZFLuaExecuteDetail(ZFInputForPathInfo(pathInfo), luaParams);
}


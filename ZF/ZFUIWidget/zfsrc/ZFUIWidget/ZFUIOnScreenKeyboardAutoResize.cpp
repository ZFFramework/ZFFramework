#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData;
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(
        ZF_IN ZFUIWindow *window
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(
        ZF_IN ZFUIWindow *window
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_rootWindowOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        );

zfclass _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData : zfextend ZFObject {
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->ownerWindow = zfnull;
        this->startCount = 1;
        this->marginSaved = ZFUIMarginZero();
        this->marginHasStored = zffalse;

        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = this;

        ZFLISTENER_1(onScreenKeyboardStateOnUpdateListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateOnUpdate(zfargs, taskData);
        } ZFLISTENER_END()
        this->onScreenKeyboardStateOnUpdateListener = onScreenKeyboardStateOnUpdateListener;

        ZFLISTENER_1(windowOnUpdateLayoutListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(zfargs, taskData);
        } ZFLISTENER_END()
        this->windowOnUpdateLayoutListener = windowOnUpdateLayoutListener;

        ZFLISTENER_1(rootWindowOnUpdateListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_rootWindowOnUpdate(zfargs, taskData);
        } ZFLISTENER_END()
        this->rootWindowOnUpdateListener = rootWindowOnUpdateListener;

        ZFLISTENER_1(windowLayoutMarginOnUpdateListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginOnUpdate(zfargs, taskData);
        } ZFLISTENER_END()
        this->windowLayoutMarginOnUpdateListener = windowLayoutMarginOnUpdateListener;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(this->ownerWindow, this);
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFUIWindow *ownerWindow;
    zfindex startCount;
    ZFUIMargin marginSaved;
    zfbool marginHasStored;

    ZFListener onScreenKeyboardStateOnUpdateListener;
    ZFListener windowOnUpdateLayoutListener;
    ZFListener rootWindowOnUpdateListener;
    ZFListener windowLayoutMarginOnUpdateListener;
};

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStart
        , ZFMP_IN(ZFUIWindow *, window)
        ) {
    if(window == zfnull) {
        return;
    }

    ZFCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = zfany(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData != zfnull) {
        taskData->startCount += 1;
        return;
    }
    zfobj<_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData> taskDataTmp;
    taskData = taskDataTmp;
    taskData->ownerWindow = window;
    window->objectTag(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull(), taskDataTmp);

    _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(window, taskData);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStop
        , ZFMP_IN(ZFUIWindow *, window)
        ) {
    if(window == zfnull) {
        return;
    }

    ZFCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = zfany(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData == zfnull) {
        return;
    }
    if(taskData->startCount > 1) {
        taskData->startCount -= 1;
        return;
    }
    window->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeDataHolder, ZFLevelZFFrameworkEssential) {
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeDataHolder) {
    ZFCoreAssertWithMessageTrim(this->windowList.isEmpty(),
        "ZFUIOnScreenKeyboardAutoResizeStart/ZFUIOnScreenKeyboardAutoResizeStop mismatch, have you forgot to stop?");
}
public:
    ZFCoreArray<ZFUIWindow *> windowList;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeAutoStop, ZFLevelZFFrameworkEssential) {
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeAutoStop) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    while(!d->windowList.isEmpty()) {
        d->windowList.getLast()->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeAutoStop)

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(
        ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        , ZF_IN ZFUIWindow *window
        , ZF_IN ZFUIOnScreenKeyboardState *state
        ) {
    const ZFUIMargin &marginOld = (taskData->marginHasStored ? taskData->marginSaved : window->layoutParam()->margin());
    if(state->keyboardShowing() && window->showing()) {
        ZFUIMargin margin = marginOld;

        ZFUIRect windowFrame = ZFUIRectZero();
        ZFUIViewPositionOnScreen(windowFrame, window->rootWindow()->rootView());
        ZFUIRectApplyMarginT(windowFrame, windowFrame, margin);
        ZFUIRectApplyMarginT(windowFrame, windowFrame, window->rootWindow()->windowMargin());

        ZFUIRect clientFrame = ZFUIRectZero();
        state->keyboardFixClientFrameT(clientFrame);

        if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(windowFrame)) {
            margin.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(windowFrame);
        }
        if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(windowFrame)) {
            margin.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(windowFrame);
        }
        if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(windowFrame)) {
            margin.right += ZFUIRectGetRight(windowFrame) - ZFUIRectGetRight(clientFrame);
        }
        if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(windowFrame)) {
            margin.bottom += ZFUIRectGetBottom(windowFrame) - ZFUIRectGetBottom(clientFrame);
        }

        if(!taskData->marginHasStored) {
            taskData->marginHasStored = zftrue;
            taskData->marginSaved = marginOld;
        }

        window->layoutParam()->observerRemove(
            ZFObject::E_ObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginOnUpdateListener);
        window->layoutParam()->margin(margin);
        window->layoutParam()->observerAdd(
            ZFObject::E_ObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginOnUpdateListener);
    }
    else {
        window->layoutParam()->observerRemove(
            ZFObject::E_ObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginOnUpdateListener);
        taskData->marginHasStored = zffalse;
        window->layoutParam()->margin(marginOld);
    }
}

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(
        ZF_IN ZFUIWindow *window
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    if(d->windowList.isEmpty()) {
        ZFGlobalObserver().observerAdd(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), taskData->onScreenKeyboardStateOnUpdateListener);
    }
    d->windowList.add(window);

    window->rootWindow()->rootView()->observerAdd(
        ZFUIView::E_ViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->observerAdd(
        ZFUIWindow::E_RootWindowOnUpdate(),
        taskData->rootWindowOnUpdateListener);
    _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(
        ZF_IN ZFUIWindow *window
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);

    window->rootWindow()->rootView()->observerRemove(
        ZFUIView::E_ViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->observerRemove(
        ZFUIWindow::E_RootWindowOnUpdate(),
        taskData->rootWindowOnUpdateListener);
    window->layoutParam()->observerRemove(
        ZFObject::E_ObjectPropertyValueOnUpdate(),
        taskData->windowLayoutMarginOnUpdateListener);
    window->layoutRequest();

    if(taskData->marginHasStored) {
        taskData->marginHasStored = zffalse;
        window->layoutParam()->margin(taskData->marginSaved);
    }

    d->windowList.removeElement(window);
    if(d->windowList.isEmpty()) {
        ZFGlobalObserver().observerRemove(ZFUIOnScreenKeyboardState::E_KeyboardStateOnUpdate(), taskData->onScreenKeyboardStateOnUpdateListener);
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    ZFUIOnScreenKeyboardState *state = zfargs.sender();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    for(zfindex i = 0; i < d->windowList.count(); ++i) {
        ZFUIWindow *window = d->windowList[i];
        if(window->rootWindow() != state->rootWindow()) {
            continue;
        }

        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, state);
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    ZFUIRootView *rootView = zfargs.sender();
    ZFUIWindow *window = taskData->ownerWindow;
    if(rootView->viewFrame() != rootView->viewFramePrev()) {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, ZFUIOnScreenKeyboardState::instanceForView(window));
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_rootWindowOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    ZFUIWindow *window = zfargs.sender();
    ZFUIRootView *rootViewOld = zfargs.param0()->to<ZFUIRootWindow *>()->rootView();
    rootViewOld->observerRemove(
        ZFUIView::E_ViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->rootWindow()->rootView()->observerAdd(
        ZFUIView::E_ViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginOnUpdate(
        ZF_IN const ZFArgs &zfargs
        , ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData
        ) {
    const ZFProperty *property = zfargs.param0().to<v_ZFProperty *>()->zfv;
    if(property != ZFPropertyAccess(ZFUILayoutParam, margin)) {
        return;
    }

    ZFUILayoutParam *layoutParam = zfargs.sender();
    if(taskData->marginHasStored) {
        taskData->marginSaved = layoutParam->margin();
    }
}

ZF_NAMESPACE_GLOBAL_END


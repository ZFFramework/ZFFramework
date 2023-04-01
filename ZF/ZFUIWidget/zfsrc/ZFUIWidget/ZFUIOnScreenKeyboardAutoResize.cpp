#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData;
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);

zfclass _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerWindow = zfnull;
        this->startCount = 1;
        this->layoutMarginSaved = ZFUIMarginZero();
        this->layoutMarginHasStored = zffalse;

        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = this;

        ZFLISTENER_1(onScreenKeyboardStateChangeListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange(zfargs, taskData);
        } ZFLISTENER_END()
        this->onScreenKeyboardStateChangeListener = onScreenKeyboardStateChangeListener;

        ZFLISTENER_1(windowOnUpdateLayoutListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(zfargs, taskData);
        } ZFLISTENER_END()
        this->windowOnUpdateLayoutListener = windowOnUpdateLayoutListener;

        ZFLISTENER_1(windowOnSysWindowChangeListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange(zfargs, taskData);
        } ZFLISTENER_END()
        this->windowOnSysWindowChangeListener = windowOnSysWindowChangeListener;

        ZFLISTENER_1(windowLayoutMarginChangeListener
                , _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *, taskData
                ) {
            _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange(zfargs, taskData);
        } ZFLISTENER_END()
        this->windowLayoutMarginChangeListener = windowLayoutMarginChangeListener;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(this->ownerWindow, this);
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFUIWindow *ownerWindow;
    zfindex startCount;
    ZFUIMargin layoutMarginSaved;
    zfbool layoutMarginHasStored;

    ZFListener onScreenKeyboardStateChangeListener;
    ZFListener windowOnUpdateLayoutListener;
    ZFListener windowOnSysWindowChangeListener;
    ZFListener windowLayoutMarginChangeListener;
};

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStart,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window == zfnull)
    {
        return ;
    }

    zfCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData != zfnull)
    {
        taskData->startCount += 1;
        return ;
    }
    zfblockedAlloc(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData, taskDataTmp);
    taskData = taskDataTmp;
    taskData->ownerWindow = window;
    window->objectTag(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull(), taskDataTmp);

    _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(window, taskData);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStop,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window == zfnull)
    {
        return ;
    }

    zfCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData == zfnull)
    {
        return ;
    }
    if(taskData->startCount > 1)
    {
        taskData->startCount -= 1;
        return ;
    }
    window->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeDataHolder, ZFLevelZFFrameworkEssential)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeDataHolder)
{
    zfCoreAssertWithMessageTrim(this->windowList.isEmpty(),
        "ZFUIOnScreenKeyboardAutoResizeStart/ZFUIOnScreenKeyboardAutoResizeStop mismatch, have you forgot to stop?");
}
public:
    ZFCoreArrayPOD<ZFUIWindow *> windowList;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeAutoStop, ZFLevelZFFrameworkEssential)
{
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeAutoStop)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    while(!d->windowList.isEmpty())
    {
        d->windowList.getLast()->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeAutoStop)

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData, ZF_IN ZFUIWindow *window, ZF_IN ZFUIOnScreenKeyboardState *state)
{
    const ZFUIMargin &layoutMarginOld = (taskData->layoutMarginHasStored ? taskData->layoutMarginSaved : window->windowLayoutParam()->layoutMargin());
    if(state->keyboardShowing() && window->windowShowing())
    {
        ZFUIMargin margin = layoutMarginOld;

        ZFUIRect windowFrame = ZFUIRectZero();
        ZFUIViewPositionOnScreen(windowFrame, window->windowOwnerSysWindow()->rootView());
        ZFUIRectApplyMargin(windowFrame, windowFrame, margin);
        ZFUIRectApplyMargin(windowFrame, windowFrame, window->windowOwnerSysWindow()->sysWindowMargin());

        ZFUIRect clientFrame = ZFUIRectZero();
        state->keyboardFixClientFrameT(clientFrame);

        if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(windowFrame))
        {
            margin.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(windowFrame);
        }
        if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(windowFrame))
        {
            margin.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(windowFrame);
        }
        if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(windowFrame))
        {
            margin.right += ZFUIRectGetRight(windowFrame) - ZFUIRectGetRight(clientFrame);
        }
        if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(windowFrame))
        {
            margin.bottom += ZFUIRectGetBottom(windowFrame) - ZFUIRectGetBottom(clientFrame);
        }

        if(!taskData->layoutMarginHasStored)
        {
            taskData->layoutMarginHasStored = zftrue;
            taskData->layoutMarginSaved = layoutMarginOld;
        }

        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginChangeListener);
        window->windowLayoutParam()->layoutMargin(margin);
        window->windowLayoutParam()->observerAdd(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginChangeListener);
    }
    else
    {
        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            taskData->windowLayoutMarginChangeListener);
        taskData->layoutMarginHasStored = zffalse;
        window->windowLayoutParam()->layoutMargin(layoutMarginOld);
    }
}

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    if(d->windowList.isEmpty())
    {
        ZFGlobalObserver().observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), taskData->onScreenKeyboardStateChangeListener);
    }
    d->windowList.add(window);

    window->windowOwnerSysWindow()->rootView()->observerAdd(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->observerAdd(
        ZFUIWindow::EventWindowOwnerSysWindowOnChange(),
        taskData->windowOnSysWindowChangeListener);
    _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);

    window->windowOwnerSysWindow()->rootView()->observerRemove(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->observerRemove(
        ZFUIWindow::EventWindowOwnerSysWindowOnChange(),
        taskData->windowOnSysWindowChangeListener);
    window->windowLayoutParam()->observerRemove(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        taskData->windowLayoutMarginChangeListener);
    window->layoutRequest();

    if(taskData->layoutMarginHasStored)
    {
        taskData->layoutMarginHasStored = zffalse;
        window->windowLayoutParam()->layoutMargin(taskData->layoutMarginSaved);
    }

    d->windowList.removeElement(window);
    if(d->windowList.isEmpty())
    {
        ZFGlobalObserver().observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), taskData->onScreenKeyboardStateChangeListener);
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZFUIOnScreenKeyboardState *state = zfargs.senderT();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    for(zfindex i = 0; i < d->windowList.count(); ++i)
    {
        ZFUIWindow *window = d->windowList[i];
        if(window->windowOwnerSysWindow() != state->keyboardOwnerSysWindow())
        {
            continue;
        }

        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, state);
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZFUIRootView *rootView = zfargs.senderT();
    ZFUIWindow *window = taskData->ownerWindow;
    if(rootView->viewFrame() != rootView->viewFramePrev())
    {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(taskData, window, ZFUIOnScreenKeyboardState::instanceForView(window));
    }
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZFUIWindow *window = zfargs.senderT();
    ZFUIRootView *rootViewOld = zfargs.param0()->to<ZFUISysWindow *>()->rootView();
    rootViewOld->observerRemove(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
    window->windowOwnerSysWindow()->rootView()->observerAdd(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        taskData->windowOnUpdateLayoutListener);
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange(ZF_IN const ZFArgs &zfargs, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    const ZFProperty *property = zfargs.param0()->to<v_ZFProperty *>()->zfv;
    if(property != ZFPropertyAccess(ZFUILayoutParam, layoutMargin))
    {
        return ;
    }

    ZFUILayoutParam *layoutParam = zfargs.senderT();
    if(taskData->layoutMarginHasStored)
    {
        taskData->layoutMarginSaved = layoutParam->layoutMargin();
    }
}

ZF_NAMESPACE_GLOBAL_END


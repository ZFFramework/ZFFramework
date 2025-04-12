#include "ZFUIPage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFUIPageResumeReason)
ZFENUM_DEFINE(ZFUIPagePauseReason)

ZFOBJECT_REGISTER(ZFUIPage)

ZFEVENT_REGISTER(ZFUIPage, PageOnCreate)
ZFEVENT_REGISTER(ZFUIPage, PageOnResume)
ZFEVENT_REGISTER(ZFUIPage, PageOnPause)
ZFEVENT_REGISTER(ZFUIPage, PageOnDestroy)
ZFEVENT_REGISTER(ZFUIPage, PageOrientationOnUpdate)

ZFEVENT_REGISTER(ZFUIPage, PageAniOnPrepare)
ZFEVENT_REGISTER(ZFUIPage, PageAniOnStart)
ZFEVENT_REGISTER(ZFUIPage, PageAniOnStop)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIPage, zfanyT<ZFUIPageManager>, pageManager)
ZFMETHOD_DEFINE_0(ZFUIPage, ZFUIView *, pageView) {
    return _ZFP_ZFUIPage_pageView;
}

ZFMETHOD_DEFINE_0(ZFUIPage, zfbool, pageCreated) {
    return this->_ZFP_ZFUIPage_pageCreated;
}
ZFMETHOD_DEFINE_0(ZFUIPage, zfbool, pageResumed) {
    return _ZFP_ZFUIPage_pageResumed;
}

ZFMETHOD_DEFINE_2(ZFUIPage, void, pageCreate
        , ZFMP_IN(ZFUIPage *, page)
        , ZFMP_IN_OPT(const ZFListener &, pageResultCallback, zfnull)
        ) {
    if(page && pageResultCallback) {
        zfself *owner = this;
        ZFLISTENER_2(impl
                , zfweak, owner
                , ZFListener, pageResultCallback
                ) {
            ZFUIPage *page = zfargs.sender();
            pageResultCallback.execute(ZFArgs()
                    .sender(owner)
                    .param0(page->pageResult())
                    );
        } ZFLISTENER_END()
        page->observerAddForOnce(ZFUIPage::E_PageOnDestroy(), impl);
    }
    this->pageManager()->pageCreate(page);
}
ZFMETHOD_DEFINE_1(ZFUIPage, void, pageReplace
        , ZFMP_IN(ZFUIPage *, page)
        ) {
    this->pageManager()->pageReplace(page);
}

ZFMETHOD_DEFINE_0(ZFUIPage, void, pageResume) {
    this->pageManager()->pageResume(this);
}

ZFMETHOD_DEFINE_0(ZFUIPage, void, pageDestroy) {
    this->pageManager()->pageDestroy(this);
}

ZFMETHOD_DEFINE_0(ZFUIPage, zfbool, isLandscape) {
    return _ZFP_ZFUIPage_pageOrientation == (zfbyte)1;
}

void ZFUIPage::pageOnCreate(void) {
    this->_ZFP_ZFUIPage_pageCreated = zftrue;
    zfautoT<ZFUIView> pageView = (this->pageViewClass() != zfnull ? this->pageViewClass() : ZFUIView::ClassData())->newInstance();
    this->_ZFP_ZFUIPage_pageView = pageView;
    ZFCoreAssertWithMessage(this->_ZFP_ZFUIPage_pageView != zfnull, "pageViewClass must be type of %s", ZFUIView::ClassData()->className());
    zfRetain(this->_ZFP_ZFUIPage_pageView);

    zfweakT<ZFUIPage> weakSelf = this;
    ZFLISTENER_1(impl
            , zfweakT<ZFUIPage>, weakSelf
            ) {
        ZFUIPage *p = weakSelf;
        if(p == zfnull) {
            return;
        }
        const ZFUIRect &viewFrame = p->pageView()->viewFrame();
        zfbyte tmp = (zfbyte)(viewFrame.width > viewFrame.height ? 1 : 0);
        if(p->_ZFP_ZFUIPage_pageOrientation != tmp) {
            p->_ZFP_ZFUIPage_pageOrientation = tmp;
            p->pageOrientationOnUpdate();
            p->observerNotify(ZFUIPage::E_PageOrientationOnUpdate());
            p->pageManager()->observerNotifyWithSender(p, ZFUIPageManager::E_PageOrientationOnUpdate());
        }
    } ZFLISTENER_END()
    ZFObserverGroup(this, pageView).observerAdd(ZFUIView::E_ViewLayoutOnLayoutPrepare(), impl);
}
void ZFUIPage::pageOnResume(ZF_IN ZFUIPageResumeReason reason) {
    this->_ZFP_ZFUIPage_pageResumed = zftrue;
}
void ZFUIPage::pageOnPause(ZF_IN ZFUIPagePauseReason reason) {
    this->_ZFP_ZFUIPage_pageResumed = zffalse;
}
void ZFUIPage::pageOnDestroy(void) {
    ZFObserverGroupRemove(this);
    zfRetainChange(this->_ZFP_ZFUIPage_pageView, zfnull);
    zfRetainChange(this->_ZFP_ZFUIPage_pageAni, zfnull);
    this->_ZFP_ZFUIPage_pageManager = zfnull;
    this->_ZFP_ZFUIPage_pageCreated = zffalse;
}

ZFMETHOD_DEFINE_1(ZFUIPage, void, pageAni
        , ZFMP_IN(ZFAnimation *, pageAni)
        ) {
    zfRetainChange(this->_ZFP_ZFUIPage_pageAni, pageAni);
}
ZFMETHOD_DEFINE_0(ZFUIPage, ZFAnimation *, pageAni) {
    return this->_ZFP_ZFUIPage_pageAni;
}

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManager)

ZFEVENT_REGISTER(ZFUIPageManager, ManagerOnCreate)
ZFEVENT_REGISTER(ZFUIPageManager, ManagerOnResume)
ZFEVENT_REGISTER(ZFUIPageManager, ManagerOnPause)
ZFEVENT_REGISTER(ZFUIPageManager, ManagerOnDestroy)
ZFEVENT_REGISTER(ZFUIPageManager, ManagerOrientationOnUpdate)
ZFEVENT_REGISTER(ZFUIPageManager, ManagerUIBlockedOnUpdate)

ZFEVENT_REGISTER(ZFUIPageManager, PageOnCreate)
ZFEVENT_REGISTER(ZFUIPageManager, PageOnResume)
ZFEVENT_REGISTER(ZFUIPageManager, PageOnPause)
ZFEVENT_REGISTER(ZFUIPageManager, PageOnDestroy)
ZFEVENT_REGISTER(ZFUIPageManager, PageOrientationOnUpdate)

ZFEVENT_REGISTER(ZFUIPageManager, PageAniOnPrepare)
ZFEVENT_REGISTER(ZFUIPageManager, PageAniOnStart)
ZFEVENT_REGISTER(ZFUIPageManager, PageAniOnStop)

// ============================================================
// _ZFP_ZFUIPageManagerPrivate
zfclassNotPOD _ZFP_ZFUIPageAniOverrideData {
public:
    zfautoT<ZFAnimation> resumeAni;
    zfautoT<ZFAnimation> pauseAni;
    zfbool pauseAniHigherPriority;
};
zfclassNotPOD _ZFP_ZFUIPageManagerPrivate {
public:
    ZFUIWindow *managerOwnerWindow; // no auto retain
    zfbool managerCreated;
    zfbool managerResumed;
    zfbool managerDestroyRunning;
    zfbyte managerOrientation; // -1: not initialized, 0: port, 1: land
    zfbool scheduleResumeRunning;
    zfbool pageRequestRunningFlag;
    ZFCoreArray<ZFListener> pageRequestQueue;
    ZFUIView *managerContainer;
    ZFUIView *pageContainer;
    ZFCoreArray<ZFUIPage *> pageList;
    ZFCoreArray<ZFUIPage *> pageDestroyList;
    ZFCoreArray<_ZFP_ZFUIPageAniOverrideData> pageAniOverrideList;
    zfautoT<ZFAnimation> resumeAni;
    zfautoT<ZFAnimation> pauseAni;
    ZFUIPage *pageMoveLastResumePage;
    zfint managerUIBlocked;
    zfint pageMoveFlag;
public:
    _ZFP_ZFUIPageManagerPrivate(void)
    : managerOwnerWindow(zfnull)
    , managerCreated(zffalse)
    , managerResumed(zffalse)
    , managerDestroyRunning(zffalse)
    , managerOrientation((zfbyte)-1)
    , scheduleResumeRunning(zffalse)
    , pageRequestRunningFlag(zffalse)
    , pageRequestQueue()
    , managerContainer(zfnull)
    , pageContainer(zfnull)
    , pageList()
    , pageDestroyList()
    , pageAniOverrideList()
    , resumeAni()
    , pauseAni()
    , pageMoveLastResumePage(zfnull)
    , managerUIBlocked(0)
    , pageMoveFlag(0)
    {
    }

public:
    void pageRequestAdd(
            ZF_IN ZFUIPageManager *owner
            , ZF_IN const ZFListener &callback
            ) {
        if(this->pageRequestRunningFlag || !this->pageRequestQueue.isEmpty()) {
            this->pageRequestQueue.add(callback);
            return;
        }
        if(this->pageRequestQueue.isEmpty()) {
            this->pageRequestRunningFlag = zftrue;
            callback.execute(ZFArgs()
                    .sender(owner)
                );
            this->pageRequestRunningFlag = zffalse;
        }
        this->pageRequestResolve(owner);
    }
    void pageRequestResolve(ZF_IN ZFUIPageManager *owner) {
        while(!this->pageRequestQueue.isEmpty()) {
            ZFListener callback = this->pageRequestQueue[0];
            this->pageRequestQueue.remove(0);
            this->pageRequestRunningFlag = zftrue;
            callback.execute(ZFArgs()
                    .sender(owner)
                );
            this->pageRequestRunningFlag = zffalse;
        }
    }

    void scheduleResume(
            ZF_IN ZFUIPageManager *owner
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *pausePage
            , ZF_IN ZFUIPagePauseReason pauseReason
            ) {
        if(this->scheduleResumeRunning) {
            return;
        }
        this->scheduleResumeRunning = zftrue;
        this->scheduleResumeAction(owner, resumeReason, pausePage, pauseReason);
    }
    void scheduleResumeAction(
            ZF_IN zfweakT<ZFUIPageManager> const &owner
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *pausePage
            , ZF_IN ZFUIPagePauseReason pauseReason
            ) {
        ZFLISTENER_4(action
                , zfweakT<ZFUIPageManager>, owner
                , ZFUIPageResumeReason, resumeReason
                , zfautoT<ZFUIPage>, pausePage
                , ZFUIPagePauseReason, pauseReason
                ) {
            if(!owner) {
                return;
            }
            _ZFP_ZFUIPageManagerPrivate *d = owner->d;
            if(d->pageRequestQueue.isEmpty()) {
                d->scheduleResumeRunning = zffalse;
                ZFUIPage *resumePage = zfnull;
                if(!d->pageList.isEmpty() && d->managerResumed) {
                    resumePage = d->pageList.getLast();
                    pageOnResume(resumePage, resumeReason, pausePage);
                }
                pageAniUpdate(
                        resumePage, resumeReason
                        , pausePage, pauseReason
                        );
            }
            else {
                d->scheduleResume(owner, resumeReason, pausePage, pauseReason);
            }
        } ZFLISTENER_END()
        owner->pageRequest(action);
    }

public:
    void pageDestroyAction(
            ZF_IN ZFUIPageManager *owner
            , ZF_IN zfindex pageIndex
            ) {
        ZFCoreAssert(this->pageMoveFlag == 0);
        if(pageIndex >= this->pageList.count()) {
            return;
        }
        ZFUIPage *pausePage = this->pageList.removeAndGet(pageIndex);
        ZFCoreAssert(this->pageDestroyList.find(pausePage) == zfindexMax());
        this->pageDestroyList.add(pausePage);
        if(pageIndex < this->pageList.count() - 1) {
            _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, v_ZFUIPagePauseReason::e_BeforeDestroy, zfnull);
        }
        else {
            ZFUIPage *resumePage = this->pageList.isEmpty() ? zfnull : this->pageList.getLast();
            _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, v_ZFUIPagePauseReason::e_BeforeDestroy, resumePage);
        }
        this->scheduleResume(owner, v_ZFUIPageResumeReason::e_FromBackground, pausePage, v_ZFUIPagePauseReason::e_BeforeDestroy);
    }

public:
    static void pageOnCreate(ZF_IN ZFUIPage *page) {
        if(!page->pageCreated()) {
            zfRetain(page);
            page->pageOnCreate();
            page->observerNotify(ZFUIPage::E_PageOnCreate());
            page->pageManager()->observerNotifyWithSender(page, ZFUIPageManager::E_PageOnCreate());
        }
    }
    static void pageOnResume(
            ZF_IN ZFUIPage *page
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *siblingPage
            ) {
        pageOnCreate(page);
        if(!page->pageResumed()) {
            page->pageOnResume(resumeReason);
            zfobj<v_ZFUIPageResumeReason> resumeReasonHolder(resumeReason);
            page->observerNotify(ZFUIPage::E_PageOnResume(), resumeReasonHolder, siblingPage);
            page->pageManager()->observerNotifyWithSender(page, ZFUIPageManager::E_PageOnResume(), resumeReasonHolder, siblingPage);
        }
    }
    static void pageOnPause(
            ZF_IN ZFUIPage *page
            , ZF_IN ZFUIPagePauseReason pauseReason
            , ZF_IN ZFUIPage *siblingPage
            ) {
        if(page->pageResumed()) {
            zfobj<v_ZFUIPagePauseReason> pauseReasonHolder(pauseReason);
            page->pageManager()->observerNotifyWithSender(page, ZFUIPageManager::E_PageOnPause(), pauseReasonHolder, siblingPage);
            page->observerNotify(ZFUIPage::E_PageOnPause(), pauseReasonHolder, siblingPage);
            page->pageOnPause(pauseReason);
        }
    }
    static void pageOnDestroy(ZF_IN ZFUIPage *page) {
        pageOnPause(page, v_ZFUIPagePauseReason::e_BeforeDestroy, zfnull);
        if(page->pageCreated()) {
            page->pageManager()->observerNotifyWithSender(page, ZFUIPageManager::E_PageOnDestroy());
            page->observerNotify(ZFUIPage::E_PageOnDestroy());
            page->pageOnDestroy();
            zfRelease(page);
        }
    }

    static void pageAniOnPrepare(
            ZF_IN ZFUIPage *page
            , ZF_IN ZFEnum *resumeOrPauseReason
            , ZF_IN ZFUIPage *siblingPage
            ) {
        page->pageAniOnPrepare(resumeOrPauseReason, siblingPage);
        page->pageManager()->observerNotifyWithSender(page, ZFUIPageManager::E_PageAniOnPrepare(), resumeOrPauseReason, siblingPage);
        page->observerNotify(ZFUIPage::E_PageAniOnPrepare(), resumeOrPauseReason, siblingPage);
    }

public:
    static void pageAniUpdate(
            ZF_IN ZFUIPage *resumePage
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *pausePage
            , ZF_IN ZFUIPagePauseReason pauseReason
            ) {
        ZFUIPageManager *manager = zfnull;
        if(resumePage != zfnull) {
            resumePage->pageAni(zfnull);
            if(manager == zfnull) {
                manager = resumePage->pageManager();
            }
        }
        if(pausePage != zfnull) {
            pausePage->pageAni(zfnull);
            if(manager == zfnull) {
                manager = pausePage->pageManager();
            }
        }
        if(manager == zfnull) {
            return;
        }

        // stop prev
        if(manager->d->resumeAni != zfnull) {
            manager->d->resumeAni->stop();
        }
        if(manager->d->pauseAni != zfnull) {
            manager->d->pauseAni->stop();
        }

        // add resume page view
        if(resumePage != zfnull && resumePage->pageView()->parent() == zfnull) {
            resumePage->pageManager()->pageContainer()->child(resumePage->pageView())->c_sizeFill();
        }

        if(manager->d->managerDestroyRunning) {
            // nothing to do
        }
        else if(!manager->d->pageAniOverrideList.isEmpty()) {
            manager->d->resumeAni = manager->d->pageAniOverrideList[0].resumeAni;
            manager->d->pauseAni = manager->d->pageAniOverrideList[0].pauseAni;
            if(manager->d->pageAniOverrideList[0].pauseAniHigherPriority) {
                if(pausePage != zfnull) {
                    manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                }
            }
            manager->d->pageAniOverrideList.removeFirst();
            if(pausePage == zfnull) {
                manager->d->pauseAni = zfnull;
            }
            if(resumePage == zfnull) {
                manager->d->resumeAni = zfnull;
            }
        }
        else {
            switch(resumeReason) {
                case v_ZFUIPageResumeReason::e_ByManagerResume:
                    break;
                case v_ZFUIPageResumeReason::e_ByRequest:
                    if(resumePage == zfnull || (pausePage != zfnull && pausePage->pageAniPriority() > resumePage->pageAniPriority())) {
                        pageAniOnPrepare(pausePage, zfobj<v_ZFUIPagePauseReason>(pauseReason), resumePage);
                        manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                    }
                    else {
                        pageAniOnPrepare(resumePage, zfobj<v_ZFUIPageResumeReason>(resumeReason), pausePage);
                    }
                    break;
                case v_ZFUIPageResumeReason::e_FromBackground:
                    if(pausePage == zfnull || (resumePage != zfnull && resumePage->pageAniPriority() > pausePage->pageAniPriority())) {
                        pageAniOnPrepare(resumePage, zfobj<v_ZFUIPageResumeReason>(resumeReason), pausePage);
                    }
                    else {
                        pageAniOnPrepare(pausePage, zfobj<v_ZFUIPagePauseReason>(pauseReason), resumePage);
                        manager->pageContainer()->childMove(pausePage->pageView(), zfindexMax());
                    }
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    break;
            }

            manager->d->resumeAni = resumePage ? resumePage->pageAni() : zfnull;
            manager->d->pauseAni = pausePage ? pausePage->pageAni() : zfnull;
        }
        ZFCoreAssertWithMessageTrim(!(manager->d->resumeAni != zfnull && manager->d->resumeAni == manager->d->pauseAni),
            "[ZFUIPage] same ani instance is not allowed");

        pageAniOnStart(manager, resumePage, resumeReason, pausePage, pauseReason);
        if(manager->d->pauseAni == zfnull && manager->d->resumeAni == zfnull) {
            pageAniOnStop(manager, resumePage, resumeReason, pausePage, pauseReason);
            return;
        }

        if(manager->d->pauseAni != zfnull) {
            manager->d->pauseAni->target(pausePage->pageView());
            ZFLISTENER_4(aniOnStop
                    , ZFUIPage *, pausePage
                    , ZFUIPagePauseReason, pauseReason
                    , ZFUIPage *, resumePage
                    , ZFUIPageResumeReason, resumeReason
                    ) {
                pausePage->pageManager()->d->pauseAni = zfnull;
                pageAniOnStop(pausePage->pageManager(), resumePage, resumeReason, pausePage, pauseReason);
            } ZFLISTENER_END()
            manager->d->pauseAni->observerAddForOnce(
                    ZFAnimation::E_AniOnStop(),
                    aniOnStop);
            manager->d->pauseAni->start();
        }

        if(manager->d->resumeAni != zfnull) {
            manager->d->resumeAni->target(resumePage->pageView());
            ZFLISTENER_4(aniOnStop
                    , ZFUIPage *, resumePage
                    , ZFUIPageResumeReason, resumeReason
                    , ZFUIPage *, pausePage
                    , ZFUIPagePauseReason, pauseReason
                    ) {
                resumePage->pageManager()->d->resumeAni = zfnull;
                pageAniOnStop(resumePage->pageManager(), resumePage, resumeReason, pausePage, pauseReason);
            } ZFLISTENER_END()
            manager->d->resumeAni->observerAddForOnce(
                ZFAnimation::E_AniOnStop(),
                aniOnStop);
            manager->d->resumeAni->start();
        }
    }
    static void pageAniOnStart(
            ZF_IN ZFUIPageManager *manager
            , ZF_IN ZFUIPage *resumePage
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *pausePage
            , ZF_IN ZFUIPagePauseReason pauseReason
            ) {
        if(pausePage != zfnull) {
            zfobj<v_ZFUIPagePauseReason> pauseReasonHolder(pauseReason);
            pausePage->observerNotify(ZFUIPage::E_PageAniOnStart(), pauseReasonHolder);
            manager->observerNotifyWithSender(pausePage, ZFUIPageManager::E_PageAniOnStart(), pauseReasonHolder);
        }
        if(resumePage != zfnull) {
            zfobj<v_ZFUIPageResumeReason> resumeReasonHolder(resumeReason);
            resumePage->observerNotify(ZFUIPage::E_PageAniOnStart(), resumeReasonHolder);
            manager->observerNotifyWithSender(resumePage, ZFUIPageManager::E_PageAniOnStart(), resumeReasonHolder);
        }
    }
    static void pageAniOnStop(
            ZF_IN ZFUIPageManager *manager
            , ZF_IN ZFUIPage *resumePage
            , ZF_IN ZFUIPageResumeReason resumeReason
            , ZF_IN ZFUIPage *pausePage
            , ZF_IN ZFUIPagePauseReason pauseReason
            ) {
        if(manager->d->resumeAni != zfnull || manager->d->pauseAni != zfnull) {
            return;
        }
        if(pausePage != zfnull) {
            manager->pageContainer()->childRemove(pausePage->pageView());

            zfobj<v_ZFUIPagePauseReason> pauseReasonHolder(pauseReason);
            pausePage->observerNotify(ZFUIPage::E_PageAniOnStop(), pauseReasonHolder);
            manager->observerNotifyWithSender(pausePage, ZFUIPageManager::E_PageAniOnStop(), pauseReasonHolder);
        }
        if(resumePage != zfnull) {
            zfobj<v_ZFUIPageResumeReason> resumeReasonHolder(resumeReason);
            resumePage->observerNotify(ZFUIPage::E_PageAniOnStop(), resumeReasonHolder);
            manager->observerNotifyWithSender(resumePage, ZFUIPageManager::E_PageAniOnStop(), resumeReasonHolder);
        }
        if(!manager->d->pageDestroyList.isEmpty()) {
            ZFCoreArray<ZFUIPage *> pageDestroyList = manager->d->pageDestroyList;
            manager->d->pageDestroyList = ZFCoreArray<ZFUIPage *>();
            for(zfindex i = 0; i < pageDestroyList.count(); ++i) {
                pageOnDestroy(pageDestroyList[i]);
            }
        }
    }
};

void ZFUIPageManager::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIPageManagerPrivate);
}
void ZFUIPageManager::objectOnDealloc(void) {
    zfRetainChange(d->pageContainer, zfnull);
    zfRetainChange(d->managerContainer, zfnull);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIPageManager::managerOnCreate(void) {
    zfautoT<ZFUIView> managerContainer = (this->managerContainerClass() != zfnull ? this->managerContainerClass() : ZFUIView::ClassData())->newInstance();
    zfautoT<ZFUIView> pageContainer = (this->pageContainerClass() != zfnull ? this->pageContainerClass() : ZFUIView::ClassData())->newInstance();
    d->managerContainer = managerContainer;
    d->pageContainer = pageContainer;
    ZFCoreAssertWithMessage(d->managerContainer != zfnull, "managerContainerClass must be type of %s", ZFUIView::ClassData()->className());
    ZFCoreAssertWithMessage(d->pageContainer != zfnull, "pageContainerClass must be type of %s", ZFUIView::ClassData()->className());
    zfRetain(d->managerContainer);
    zfRetain(d->pageContainer);

    d->managerContainer->child(d->pageContainer)->c_sizeFill();

    zfweakT<ZFUIPageManager> weakSelf = this;
    ZFLISTENER_1(impl
            , zfweakT<ZFUIPageManager>, weakSelf
            ) {
        ZFUIPageManager *p = weakSelf;
        if(p == zfnull) {
            return;
        }
        const ZFUIRect &viewFrame = p->managerContainer()->viewFrame();
        zfbyte tmp = (zfbyte)(viewFrame.width > viewFrame.height ? 1 : 0);
        if(p->d->managerOrientation != tmp) {
            p->d->managerOrientation = tmp;
            p->managerOrientationOnUpdate();
            p->observerNotify(ZFUIPageManager::E_ManagerOrientationOnUpdate());
        }
    } ZFLISTENER_END()
    ZFObserverGroup(this, d->managerContainer).observerAdd(ZFUIView::E_ViewLayoutOnLayoutPrepare(), impl);
}
void ZFUIPageManager::managerOnDestroy(void) {
    ZFObserverGroupRemove(this);
    zfRetainChange(d->pageContainer, zfnull);
    zfRetainChange(d->managerContainer, zfnull);
}

// ============================================================
// manager creation
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerCreate) {
    ZFCoreAssert(!d->managerCreated);
    zfRetain(this);
    d->managerCreated = zftrue;
    this->managerOnCreate();
    this->observerNotify(ZFUIPageManager::E_ManagerOnCreate());
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerResume) {
    ZFCoreAssert(d->managerCreated);
    ZFCoreAssert(!d->managerResumed);
    d->managerResumed = zftrue;
    this->managerOnResume();
    this->observerNotify(ZFUIPageManager::E_ManagerOnResume());
    d->scheduleResume(this, v_ZFUIPageResumeReason::e_ByManagerResume, zfnull, v_ZFUIPagePauseReason::e_ToBackground);
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerPause) {
    ZFCoreAssert(d->managerResumed);

    ZFUIPage *page = this->currentPage();
    if(page != zfnull) {
        _ZFP_ZFUIPageManagerPrivate::pageOnPause(page, v_ZFUIPagePauseReason::e_ByManagerPause, zfnull);
    }

    this->managerOnPause();
    d->managerResumed = zffalse;
    this->observerNotify(ZFUIPageManager::E_ManagerOnPause());
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, managerDestroy) {
    ZFCoreAssert(d->managerCreated);
    d->managerDestroyRunning = zftrue;
    if(d->managerResumed) {
        this->managerPause();
    }

    d->pageRequestResolve(this);
    while(!d->pageList.isEmpty()) {
        d->pageDestroyAction(this, d->pageList.count() - 1);
    }
    d->pageRequestResolve(this);

    if(d->managerOwnerWindow != zfnull) {
        ZFUIWindow *managerOwnerWindowTmp = d->managerOwnerWindow;
        d->managerOwnerWindow = zfnull;
        ZFObserverGroupRemove(this);
        if(managerOwnerWindowTmp->showing()) {
            managerOwnerWindowTmp->hide();
        }
    }

    this->managerOnDestroy();
    d->managerCreated = zffalse;
    d->managerDestroyRunning = zffalse;
    d->pageAniOverrideList.removeAll();
    this->observerNotify(ZFUIPageManager::E_ManagerOnDestroy());
    zfRelease(this);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerCreated) {
    return d->managerCreated;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerResumed) {
    return d->managerResumed;
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, isLandscape) {
    return d->managerOrientation == (zfbyte)1;
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, ZFUIWindow *, managerCreateForWindow
        , ZFMP_IN_OPT(ZFUISysWindow *, ownerSysWindow, zfnull)
        ) {
    zfobj<ZFUIWindow> window(ownerSysWindow);
    d->managerOwnerWindow = window;

    this->managerCreate();
    window->child(this->managerContainer())->c_sizeFill();

    ZFUIPageManager *owner = this;

    ZFLISTENER_2(viewTreeVisibleOnUpdate
            , ZFUIWindow *, window
            , ZFUIPageManager *, owner
            ) {
        if(window->viewTreeVisible()) {
            if(!owner->managerResumed()) {
                owner->managerResume();
            }
        }
        else {
            if(owner->managerResumed()) {
                owner->managerPause();
            }
        }
    } ZFLISTENER_END()

    ZFLISTENER_1(onDestroy
            , ZFUIPageManager *, owner
            ) {
        owner->managerDestroy();
    } ZFLISTENER_END()

    ZFObserverGroup(this, window)
        .observerAdd(ZFUIWindow::E_ViewTreeVisibleOnUpdate(), viewTreeVisibleOnUpdate)
        .observerAdd(ZFUIWindow::E_ObjectBeforeDealloc(), onDestroy)
        ;

    ZFLISTENER_1(sysWindowOnPause
            , ZFUIPageManager *, owner
            ) {
        if(owner->managerResumed()) {
            owner->managerPause();
        }
    } ZFLISTENER_END()
    ZFLISTENER_1(sysWindowOnResume
            , ZFUIPageManager *, owner
            ) {
        if(owner->managerCreated() && !owner->managerResumed()) {
            owner->managerResume();
        }
    } ZFLISTENER_END()
    ZFObserverGroup(this, window->ownerSysWindow())
        .observerAdd(ZFUISysWindow::E_SysWindowOnPause(), sysWindowOnPause)
        .observerAdd(ZFUISysWindow::E_SysWindowOnResume(), sysWindowOnResume)
        ;

    window->show();
    if(window->ownerSysWindow()->nativeWindowIsResumed()
            && !this->managerResumed()
            ) {
        this->managerResume();
    }
    return d->managerOwnerWindow;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIWindow *, managerOwnerWindow) {
    return d->managerOwnerWindow;
}

// ============================================================
// container
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIView *, managerContainer) {
    return d->managerContainer;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFUIView *, pageContainer) {
    return d->pageContainer;
}

// ============================================================
// manager control
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, managerUIBlocked
        , ZFMP_IN(zfbool, value)
        ) {
    if(value) {
        ++d->managerUIBlocked;
        if(d->managerUIBlocked == 1) {
            this->managerUIBlockedOnUpdate();
            this->observerNotify(ZFUIPageManager::E_ManagerUIBlockedOnUpdate());
        }
    }
    else {
        --d->managerUIBlocked;
        if(d->managerUIBlocked == 0) {
            this->managerUIBlockedOnUpdate();
            this->observerNotify(ZFUIPageManager::E_ManagerUIBlockedOnUpdate());
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerUIBlocked) {
    return d->managerUIBlocked != 0;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, managerUIBlockedCount) {
    return (zfindex)d->managerUIBlocked;
}

// ============================================================
// page access
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, pageCount) {
    return this->pageList().count();
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, zfanyT<ZFUIPage>, pageAt
        , ZFMP_IN(zfindex, index)
        ) {
    return this->pageList().get(index);
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfanyT<ZFUIPage>, currentPage) {
    if(!this->pageList().isEmpty()) {
        return this->pageList().getLast();
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, zfindex, pageIndex
        , ZFMP_IN(ZFUIPage *, page)
        ) {
    return this->pageList().find(page);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFCoreArray<ZFUIPage *> &, pageList) {
    return d->pageList;
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, ZFCoreArray<ZFUIPage *>, pageListForGroupId
        , ZFMP_IN(const zfstring &, pageGroupId)
        ) {
    ZFCoreArray<ZFUIPage *> ret;
    for(zfindex i = 0; i < d->pageList.count(); ++i) {
        if(zfstringIsEqual(d->pageList[i]->pageGroupId(), pageGroupId)) {
            ret.add(d->pageList[i]);
        }
    }
    return ret;
}

// ============================================================
// page management
ZFMETHOD_DEFINE_3(ZFUIPageManager, void, pageAniOverride
        , ZFMP_IN(ZFAnimation *, resumeAni)
        , ZFMP_IN(ZFAnimation *, pauseAni)
        , ZFMP_IN_OPT(zfbool, pauseAniHigherPriority, zffalse)
        ) {
    _ZFP_ZFUIPageAniOverrideData data;
    data.resumeAni = resumeAni;
    data.pauseAni = pauseAni;
    data.pauseAniHigherPriority = pauseAniHigherPriority;
    d->pageAniOverrideList.add(data);
}

ZFMETHOD_DEFINE_2(ZFUIPageManager, void, pageCreate
        , ZFMP_IN(ZFUIPage *, page)
        , ZFMP_IN_OPT(const ZFListener &, pageResultCallback, zfnull)
        ) {
    if(page == zfnull) {
        return;
    }

    if(pageResultCallback) {
        zfself *owner = this;
        ZFLISTENER_2(impl
                , zfweak, owner
                , ZFListener, pageResultCallback
                ) {
            ZFUIPage *page = zfargs.sender();
            pageResultCallback.execute(ZFArgs()
                    .sender(owner)
                    .param0(page->pageResult())
                    );
        } ZFLISTENER_END()
        page->observerAddForOnce(ZFUIPage::E_PageOnDestroy(), impl);
    }

    ZFUIPageManager *pm = this;
    ZFLISTENER_2(callback
            , ZFUIPageManager *, pm
            , ZFUIPage *, page
            ) {
        ZFCoreAssert(page->_ZFP_ZFUIPage_pageManager == zfnull);
        ZFCoreAssert(pm->d->pageMoveFlag == 0);

        ZFUIPage *resumePage = page;
        ZFUIPage *pausePage = pm->currentPage();

        pm->d->pageList.add(resumePage);
        resumePage->_ZFP_ZFUIPage_pageManager = pm;

        if(pausePage != zfnull) {
            _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, v_ZFUIPagePauseReason::e_ToBackground, resumePage);
        }
        _ZFP_ZFUIPageManagerPrivate::pageOnCreate(resumePage);
        pm->d->scheduleResume(pm, v_ZFUIPageResumeReason::e_ByRequest, pausePage, v_ZFUIPagePauseReason::e_ToBackground);
    } ZFLISTENER_END()
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageReplace
        , ZFMP_IN(ZFUIPage *, page)
        ) {
    ZFUIPage *cur = this->currentPage();
    this->pageCreate(page);
    if(cur) {
        this->pageDestroy(cur);
    }
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResume
        , ZFMP_IN(ZFUIPage *, page)
        ) {
    ZFUIPageManager *pm = this;
    ZFLISTENER_2(callback
            , ZFUIPageManager *, pm
            , ZFUIPage *, page
            ) {
        pm->pageResume(pm->pageIndex(page));
    } ZFLISTENER_END()
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResume
        , ZFMP_IN(zfindex, pageIndex)
        ) {
    ZFUIPageManager *pm = this;
    ZFLISTENER_2(callback
            , ZFUIPageManager *, pm
            , zfindex, pageIndex
            ) {
        if(pageIndex < pm->pageCount()) {
            pm->pageMoveBegin();
            pm->d->pageList.move(pageIndex, zfindexMax());
            pm->pageMoveEnd();
        }
    } ZFLISTENER_END()
    d->pageRequestAdd(this, callback);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageResumeForGroupId
        , ZFMP_IN(const zfstring &, pageGroupId)
        ) {
    ZFUIPageManager *pm = this;
    ZFLISTENER_2(callback
            , ZFUIPageManager *, pm
            , zfstring, pageGroupId
            ) {
        pm->pageMoveBegin();
        zfindex moveTo = pm->d->pageList.count() - 1;
        for(zfindex i = pm->d->pageList.count() - 1; i != zfindexMax(); --i) {
            if(zfstringIsEqual(pm->d->pageList[i]->pageGroupId(), pageGroupId)) {
                pm->d->pageList.move(i, moveTo);
                --moveTo;
            }
        }
        pm->pageMoveEnd();
    } ZFLISTENER_END()
    d->pageRequestAdd(this, callback);
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageDestroy
        , ZFMP_IN(ZFUIPage *, page)
        ) {
    ZFUIPageManager *pm = this;
    ZFLISTENER_2(callback
            , ZFUIPageManager *, pm
            , ZFUIPage *, page
            ) {
        pm->d->pageDestroyAction(pm, pm->pageIndex(page));
    } ZFLISTENER_END()
    d->pageRequestAdd(this, callback);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, void, pageMoveBegin) {
    ++d->pageMoveFlag;
    if(d->pageMoveFlag == 1) {
        d->pageMoveLastResumePage = d->pageList.isEmpty() ? zfnull : d->pageList.getLast();
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, pageMoveEnd) {
    --d->pageMoveFlag;
    if(d->pageMoveFlag != 0 || d->pageMoveLastResumePage == zfnull || d->pageMoveLastResumePage == d->pageList.getLast()) {
        return;
    }
    ZFUIPage *resumePage = d->pageList.getLast();
    ZFUIPage *pausePage = d->pageMoveLastResumePage;
    d->pageMoveLastResumePage = zfnull;

    _ZFP_ZFUIPageManagerPrivate::pageOnPause(pausePage, v_ZFUIPagePauseReason::e_ToBackground, resumePage);
    d->scheduleResume(this, v_ZFUIPageResumeReason::e_ByRequest, pausePage, v_ZFUIPagePauseReason::e_ToBackground);
}

ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageRequest
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    if(callback) {
        d->pageRequestAdd(this, callback);
    }
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, pageRequestCancel
        , ZFMP_IN(const ZFListener &, callback)
        ) {
    d->pageRequestQueue.removeElement(callback);
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, pageRequestCount) {
    return d->pageRequestQueue.count();
}

ZF_NAMESPACE_GLOBAL_END


/**
 * @file ZFUIPage.h
 * @brief abstract page manage logic
 */

#ifndef _ZFI_ZFUIPage_h_
#define _ZFI_ZFUIPage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief page resume reason
 */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIPageResumeReason)
    ZFENUM_VALUE(ByManagerResume)
    ZFENUM_VALUE(ByRequest)
    ZFENUM_VALUE(FromBackground)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(ByManagerResume)
    ZFENUM_VALUE_REGISTER(ByRequest)
    ZFENUM_VALUE_REGISTER(FromBackground)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIPageResumeReason)
ZFENUM_REG(ZFLIB_ZFUIWidget, ZFUIPageResumeReason)

/**
 * @brief page pause reason
 */
ZFENUM_BEGIN(ZFLIB_ZFUIWidget, ZFUIPagePauseReason)
    ZFENUM_VALUE(ByManagerPause)
    ZFENUM_VALUE(ToBackground)
    ZFENUM_VALUE(BeforeDestroy)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(ByManagerPause)
    ZFENUM_VALUE_REGISTER(ToBackground)
    ZFENUM_VALUE_REGISTER(BeforeDestroy)
ZFENUM_END(ZFLIB_ZFUIWidget, ZFUIPagePauseReason)
ZFENUM_REG(ZFLIB_ZFUIWidget, ZFUIPagePauseReason)

// ============================================================
zfclassFwd ZFUIPageManager;
/**
 * @brief abstract page similar to Android's Activity
 *
 * a page is an abstract object with those life cycle:
 * -  #ZFUIPage::pageOnCreate, create necessary contents
 * -  #ZFUIPage::pageOnResume, before making the page active
 * -  #ZFUIPage::pageOnPause, before making the page deactive
 * -  #ZFUIPage::pageOnDestroy, to destroy the page
 *
 * and a #ZFUIPageManager is the holder to manage multiple pages\n
 * typical usage:
 * @code
 *   zfobj<ZFUIPageManager> pm;
 *   pm->observerAdd(ZFUIPageManager::E_ManagerOnCreate, myPMCreateCallback);
 *   pm->managerCreateForWindow();
 *   ZFUIPageManager::instance(pm);
 *
 *   zfobj<MyCustomPage> page1;
 *   ZFUIPageManager::instance()->pageCreate(page1);
 *   ...
 *   zfobj<ZFUIPage> page2;
 *   page2->observerAdd(ZFUIPage::E_PageOnCreate(), myCreateCallback);
 *   ZFUIPageManager::instance()->pageCreate(page2);
 *   ...
 *   page1->pageDestroy();
 * @endcode
 * at most one page would being resumed,
 * when new page created,
 * the old one would be paused to background,
 * with a page switch animation implemented by #pageAniOnPrepare\n
 * \n
 * to implement your own page, by any of these methods:
 * -  inherit #ZFUIPage and override #pageOnCreate series,
 *   construct your page view and add to #pageView
 * -  inherit #ZFUIPage and supply dynamic methods named `pageOnCreateImpl` series,
 *   with same method proto type with the original #pageOnCreate series
 * -  alloc a ZFUIPage directly, #ZFObject::observerAdd to #E_PageOnCreate series
 */
zfclass ZFLIB_ZFUIWidget ZFUIPage : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFUIPage, ZFObject)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created
     */
    ZFEVENT(PageOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason,
     * param1 is the sibling pause page or null
     */
    ZFEVENT(PageOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason,
     * param1 is the sibling resume page or null
     */
    ZFEVENT(PageOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed
     */
    ZFEVENT(PageOnDestroy)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when first time to layout page view and each time #isLandscape really changed
     */
    ZFEVENT(PageOrientationOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when this page resume or pause (only called to page who has higher #pageAniPriority),
     * to prepare #pageAni for this page and sibling page,
     * param0 is the #ZFUIPageResumeReason or #ZFUIPagePauseReason,
     * param1 is the sibling page\n
     * see #pageAniOnPrepare for more info
     */
    ZFEVENT(PageAniOnPrepare)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page ani start,
     * called once each for pause and resume page,
     * even if no actual animation to start,
     * param0 is the #ZFUIPagePauseReason/#ZFUIPageResumeReason
     */
    ZFEVENT(PageAniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page ani start,
     * called once each for pause and resume page,
     * even if no actual animation to start,
     * param0 is the #ZFUIPagePauseReason/#ZFUIPageResumeReason
     */
    ZFEVENT(PageAniOnStop)

    // ============================================================
    // page manager
public:
    /** @brief owner page manager */
    virtual zfanyT<ZFUIPageManager> pageManager(void) {
        return this->_ZFP_ZFUIPage_pageManager;
    }

    /** @brief the page's container view, ensured not null */
    ZFMETHOD_DECLARE_0(ZFUIView *, pageView)
    /**
     * @brief #pageView's class, must set before base page's #pageOnCreate
     */
    ZFPROPERTY_ASSIGN(const ZFClass *, pageViewClass)

    // ============================================================
    // page property
public:
    /**
     * @brief page group id, see #ZFUIPageManager::pageResumeForGroupId for more info
     */
    ZFPROPERTY_ASSIGN(zfstring, pageGroupId)
    /**
     * @brief store page result, passed to pageResultCallback of #pageCreate
     */
    ZFPROPERTY_RETAIN(zfany, pageResult)

    // ============================================================
    // page state
public:
    /**
     * @brief true if page created (true after #pageOnCreate called)
     */
    ZFMETHOD_DECLARE_0(zfbool, pageCreated)
    /**
     * @brief true if page resumed (true after #pageOnResume called)
     */
    ZFMETHOD_DECLARE_0(zfbool, pageResumed)

public:
    /**
     * @brief util to #ZFUIPageManager::pageCreate
     *
     * the optional pageResultCallback would be called when page destroyed,
     * sender would be the owner page calling pageCreate,
     * and param0 would be the #pageResult of the new page
     */
    ZFMETHOD_DECLARE_2(void, pageCreate
            , ZFMP_IN(ZFUIPage *, page)
            , ZFMP_IN_OPT(const ZFListener &, pageResultCallback, zfnull)
            )
    /** @brief util to #ZFUIPageManager::pageReplace */
    ZFMETHOD_DECLARE_1(void, pageReplace
            , ZFMP_IN(ZFUIPage *, page)
            )
    /** @brief util to #ZFUIPageManager::pageResume */
    ZFMETHOD_DECLARE_0(void, pageResume)
    /** @brief util to #ZFUIPageManager::pageDestroy */
    ZFMETHOD_DECLARE_0(void, pageDestroy)

    /**
     * @brief whether the page is considered landscape
     *
     * page is considered as landscape only if page view's width larger than height
     */
    ZFMETHOD_DECLARE_0(zfbool, isLandscape)

    // ============================================================
    // page life cycle
protected:
    /** @brief see #ZFUIPage */
    virtual void pageOnCreate(void);
    /** @brief see #ZFUIPage */
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReason reason);
    /** @brief see #ZFUIPage */
    virtual void pageOnPause(ZF_IN ZFUIPagePauseReason reason);
    /** @brief see #ZFUIPage */
    virtual void pageOnDestroy(void);

    /** @brief see #E_PageOrientationOnUpdate */
    virtual inline void pageOrientationOnUpdate(void) {}

    // ============================================================
    // page ani
public:
    /**
     * @brief page priority to detect for #pageAni, see #pageAniOnPrepare for more info
     */
    ZFPROPERTY_ASSIGN(zfint, pageAniPriority)

    /** @brief see #pageAniOnPrepare */
    ZFMETHOD_DECLARE_1(void, pageAni
            , ZFMP_IN(ZFAnimation *, pageAni)
            )
    /** @brief see #pageAniOnPrepare */
    ZFMETHOD_DECLARE_0(ZFAnimation *, pageAni)

protected:
    /**
     * @brief called to prepare page animation
     *
     * when switch pages, we would perform animations automatically:
     * -# detect page priority by #pageAniPriority
     * -# pageWithHighPriority->pageAniOnPrepare(xxx);
     *
     * page with high priority has the chance to setup animations
     * for both itself and its sibling page,
     * and store the result animation to #pageAni and siblingPage->pageAni()
     */
    virtual void pageAniOnPrepare(
            ZF_IN ZFEnum *resumeOrPauseReason
            , ZF_IN ZFUIPage *siblingPage
            ) {
    }

    // ============================================================
private:
    ZFUIPageManager *_ZFP_ZFUIPage_pageManager;
    ZFUIView *_ZFP_ZFUIPage_pageView;
    zfbool _ZFP_ZFUIPage_pageCreated;
    zfbool _ZFP_ZFUIPage_pageResumed;
    zfbyte _ZFP_ZFUIPage_pageOrientation; // -1: not initialized, 0: port, 1: land
    ZFAnimation *_ZFP_ZFUIPage_pageAni;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPage(void)
    : _ZFP_ZFUIPage_pageManager(zfnull)
    , _ZFP_ZFUIPage_pageView(zfnull)
    , _ZFP_ZFUIPage_pageCreated(zffalse)
    , _ZFP_ZFUIPage_pageResumed(zffalse)
    , _ZFP_ZFUIPage_pageOrientation((zfbyte)-1)
    , _ZFP_ZFUIPage_pageAni(zfnull)
    {
    }
    /** @endcond */
    zfoverride
    virtual void objectOnDealloc(void) {
        zfRetainChange(this->_ZFP_ZFUIPage_pageView, zfnull);
        zfRetainChange(this->_ZFP_ZFUIPage_pageAni, zfnull);
        zfsuper::objectOnDealloc();
    }
private:
    friend zfclassFwd ZFUIPageManager;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

// ============================================================
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/** @brief see #ZFUIPage */
zfclass ZFLIB_ZFUIWidget ZFUIPageManager : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIPageManager, ZFObject)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager created
     */
    ZFEVENT(ManagerOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFEVENT(ManagerOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFEVENT(ManagerOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager destroyed
     */
    ZFEVENT(ManagerOnDestroy)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when first time to layout page view and each time #isLandscape really changed
     */
    ZFEVENT(ManagerOrientationOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #managerUIBlockedOnUpdate,
     * see #managerUIBlocked
     */
    ZFEVENT(ManagerUIBlockedOnUpdate)

    // ============================================================
    // page observers
public:
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageOnCreate */
    ZFEVENT(PageOnCreate)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageOnResume */
    ZFEVENT(PageOnResume)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageOnPause */
    ZFEVENT(PageOnPause)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageOnDestroy */
    ZFEVENT(PageOnDestroy)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageOrientationOnUpdate */
    ZFEVENT(PageOrientationOnUpdate)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageAniOnPrepare */
    ZFEVENT(PageAniOnPrepare)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageAniOnStart */
    ZFEVENT(PageAniOnStart)
    /** @brief see #ZFObject::observerNotify, #ZFUIPage::E_PageAniOnStop */
    ZFEVENT(PageAniOnStop)

    // ============================================================
    // manager creation
public:
    /**
     * @brief manually create the manager
     *
     * for most case, it's recommended to use #managerCreateForWindow
     * to create with a #ZFUIWindow for short\n
     * if you want to embed page manager logic to other container,
     * you may manually use these methods to manage the life cycle:
     * -  #managerCreate
     * -  #managerResume
     * -  #managerPause
     * -  #managerDestroy
     */
    ZFMETHOD_DECLARE_0(void, managerCreate)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerResume)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerPause)
    /** @brief see #managerCreate */
    ZFMETHOD_DECLARE_0(void, managerDestroy)

protected:
    /**
     * @brief for subclass to do actual works
     *
     * similar to #ZFUIPage, you may implement your own page manager, by any of these methods:
     * -  inherit #ZFUIPageManager and override #managerOnCreate series
     * -  inherit #ZFUIPageManager and supply dynamic methods named `managerOnCreateImpl` series,
     *   with same method proto type with the original #managerOnCreate series
     * -  alloc a ZFUIPageManager directly, #ZFObject::observerAdd to #E_ManagerOnCreate series
     */
    virtual void managerOnCreate(void);
    /** @brief see #managerOnCreate */
    virtual void managerOnResume(void) {}
    /** @brief see #managerOnCreate */
    virtual void managerOnPause(void) {}
    /** @brief see #managerOnCreate */
    virtual void managerOnDestroy(void);

    /** @brief see #E_ManagerOrientationOnUpdate */
    virtual inline void managerOrientationOnUpdate(void) {}

public:
    /** @brief true if #managerCreate called */
    ZFMETHOD_DECLARE_0(zfbool, managerCreated)
    /** @brief true if #managerResume called */
    ZFMETHOD_DECLARE_0(zfbool, managerResumed)

    /**
     * @brief whether the manager is considered landscape
     *
     * manager is considered as landscape only if container view's width larger than height\n
     * note manager's landscape state is not always same as #ZFUIPage::isLandscape
     */
    ZFMETHOD_DECLARE_0(zfbool, isLandscape)

public:
    /**
     * @brief util method to create manager and a #ZFUIWindow
     *   to manage life cycle for most case
     *
     * this method would:
     * -# create a #ZFUIWindow
     * -# attach #managerContainer to the window
     * -# automatically call #managerCreate series according to window's life cycle
     * -# when you want to destroy the manager,
     *   simply call #managerDestroy
     */
    ZFMETHOD_DECLARE_1(ZFUIWindow *, managerCreateForWindow
            , ZFMP_IN_OPT(ZFUISysWindow *, ownerSysWindow, zfnull)
            )
    /** @brief valid only for #managerCreateForWindow */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, managerOwnerWindow)

    // ============================================================
    // container
public:
    /**
     * @brief container view of this page manager
     *
     * view tree:
     * @code
     *   managerContainer
     *     yourExtraView (title view for example)
     *     pageContainer
     *       page1.pageView
     *     yourExtraView (bottom tab for example)
     * @endcode
     *
     * the class of the container view can be changed by
     * #managerContainerClass/#pageContainerClass,
     * but must be set before base manager's #managerOnCreate
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, managerContainer)
    /** @brief see #managerContainer */
    ZFMETHOD_DECLARE_0(ZFUIView *, pageContainer)

    /** @brief see #managerContainer */
    ZFPROPERTY_ASSIGN(const ZFClass *, managerContainerClass)
    /** @brief see #managerContainer */
    ZFPROPERTY_ASSIGN(const ZFClass *, pageContainerClass)

    // ============================================================
    // manager control
public:
    /**
     * @brief enable or disable manager's user interaction
     *
     * it's useful to disable user interaction when page is doing some heavy works,
     * such as animating switching pages\n
     * this method can be embeded but must be paired,
     * last time you call this method to restore enable state would finally restore enable state
     */
    ZFMETHOD_DECLARE_1(void, managerUIBlocked
            , ZFMP_IN(zfbool, value)
            )
    /** @brief see #managerUIBlocked */
    ZFMETHOD_DECLARE_0(zfbool, managerUIBlocked)
    /** @brief see #managerUIBlocked */
    ZFMETHOD_DECLARE_0(zfindex, managerUIBlockedCount)
protected:
    /** @brief see #E_ManagerUIBlockedOnUpdate */
    virtual inline void managerUIBlockedOnUpdate(void) {
        if(this->pageContainer()) {
            this->pageContainer()->viewUIEnableTree(!this->managerUIBlocked());
        }
    }

    // ============================================================
    // page access
public:
    /**
     * @brief page count of this page manager
     */
    ZFMETHOD_DECLARE_0(zfindex, pageCount)
    /**
     * @brief directly access the page list
     *
     * you must not store the list for future use,
     * since its content may change when resolving page request\n
     * you must not modify the list\n
     * you must not reorder the list,
     * except when using with #pageMoveBegin
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<ZFUIPage *> &, pageList)
    /**
     * @brief page at index
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFUIPage>, pageAt
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief return foreground page or null if no page
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIPage>, currentPage)
    /**
     * @brief find page index
     */
    ZFMETHOD_DECLARE_1(zfindex, pageIndex
            , ZFMP_IN(ZFUIPage *, page)
            )

    /**
     * @brief find page by ZFUIPage::pageGroupId
     */
    ZFMETHOD_DECLARE_1(ZFCoreArray<ZFUIPage *>, pageListForGroupId
            , ZFMP_IN(const zfstring &, pageGroupId)
            )

    // ============================================================
    // page management
public:
    /**
     * @brief override animation for next page management,
     *   can be called more than once to override sequently,
     *   can not undo
     */
    ZFMETHOD_DECLARE_3(void, pageAniOverride
            , ZFMP_IN(ZFAnimation *, resumeAni)
            , ZFMP_IN(ZFAnimation *, pauseAni)
            , ZFMP_IN_OPT(zfbool, pauseAniHigherPriority, zffalse)
            )

    /**
     * @brief create and resume page
     *
     * the optional pageResultCallback would be called when page destroyed,
     * sender would be the owner manager,
     * and param0 would be the #ZFUIPage::pageResult of the new page
     */
    ZFMETHOD_DECLARE_2(void, pageCreate
            , ZFMP_IN(ZFUIPage *, page)
            , ZFMP_IN_OPT(const ZFListener &, pageResultCallback, zfnull)
            )
    /** @brief util to destroy current and create new page */
    ZFMETHOD_DECLARE_1(void, pageReplace
            , ZFMP_IN(ZFUIPage *, page)
            )

    /** @brief resume page */
    ZFMETHOD_DECLARE_1(void, pageResume
            , ZFMP_IN(ZFUIPage *, page)
            )
    /** @brief resume page */
    ZFMETHOD_DECLARE_1(void, pageResume
            , ZFMP_IN(zfindex, pageIndex)
            )
    /** @brief resume multiple page for group id */
    ZFMETHOD_DECLARE_1(void, pageResumeForGroupId
            , ZFMP_IN(const zfstring &, pageGroupId)
            )

    /** @brief destroy page */
    ZFMETHOD_DECLARE_1(void, pageDestroy
            , ZFMP_IN(ZFUIPage *, page)
            )

    /**
     * @brief advanced page stack management
     *
     * usage:
     * @code
     *   pm->pageMoveBegin();
     *
     *   pageList = pm->pageList();
     *   ... // change page stack order by modifying the pageList directly
     *
     *   pm->pageMoveEnd();
     * @endcode
     */
    ZFMETHOD_DECLARE_0(void, pageMoveBegin)
    /** @brief see #pageMoveBegin */
    ZFMETHOD_DECLARE_0(void, pageMoveEnd)

    /**
     * @brief add a page request to queue,
     *   used to ensure execute order when mixed with page management
     *   (#pageCreate, #pageDestroy, etc)
     */
    ZFMETHOD_DECLARE_1(void, pageRequest
            , ZFMP_IN(const ZFListener &, callback)
            )
    /**
     * @brief remove callback for #pageRequest
     */
    ZFMETHOD_DECLARE_1(void, pageRequestCancel
            , ZFMP_IN(const ZFListener &, callback)
            )
    /**
     * @brief return count of request in queue
     */
    ZFMETHOD_DECLARE_0(zfindex, pageRequestCount)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
private:
    _ZFP_ZFUIPageManagerPrivate *d;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPage_h_


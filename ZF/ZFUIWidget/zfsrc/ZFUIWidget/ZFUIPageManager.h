/**
 * @file ZFUIPageManager.h
 * @brief abstract page manager
 */

#ifndef _ZFI_ZFUIPageManager_h_
#define _ZFI_ZFUIPageManager_h_

#include "ZFUIPage.h"
#include "ZFUIPageRequest.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/**
 * @brief abstract page manager to manage multiple #ZFUIPage
 *
 * a page is an abstract object with those life cycle:
 * -  #ZFUIPage::pageOnCreate, create necessary contents
 * -  #ZFUIPage::pageOnResume, before making the page active
 * -  #ZFUIPage::pageOnPause, before making the page deactive
 * -  #ZFUIPage::pageOnDestroy, to destroy the page
 *
 * and a page manager is an abstract object to manage multiple pages\n
 * \n
 * pages is managed by life cycle request, you must use #ZFUIPageManager's method to manage pages,
 * including creating pages or showing pages\n
 * a page manager can hold many pages, however, only one page at most is resumed at one time,
 * resuming another page would cause current page to be paused,
 * pausing page manager would also causing current page to be paused\n
 * page's life cycle is requested by app code,
 * and is ensured to be queued to run at future\n
 * \n
 * typically, the page manager and page are not serializable,
 * but it's recommended to make the page param serializable,
 * so that pages can be created by reflection,
 * to achieve advanced dynamic logic\n
 * \n
 * ADVANCED:\n
 * typically, a page is a normal view to hold UI elements to interact with user,
 * and a page manager is a native window or UI container to hold the pages,
 * however, you may embeded a page manager to another container,
 * even another page manager, for how to, please refer to #embededCreate
 */
zfinterface ZF_ENV_EXPORT ZFUIPageManager : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUIPageManager, ZFInterface)

    // ============================================================
    // observers
public:
    // ============================================================
    // manager impl
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager created
     */
    ZFOBSERVER_EVENT(ManagerOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager destroyed
     */
    ZFOBSERVER_EVENT(ManagerOnDestroy)

    // ============================================================
    // manager life cycle
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager created
     */
    ZFOBSERVER_EVENT(ManagerAfterCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerAfterResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager resumed
     */
    ZFOBSERVER_EVENT(ManagerBeforePause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager destroyed
     */
    ZFOBSERVER_EVENT(ManagerBeforeDestroy)

    // ============================================================
    // manager other state change
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page manager's UIEnable changed,
     * see #managerUIBlocked
     */
    ZFOBSERVER_EVENT(ManagerUIBlockedOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #requestBlocked setting changed
     */
    ZFOBSERVER_EVENT(RequestBlockedOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called before resolve the request,
     * you may change the #ZFUIPageRequest::requestResolved state to override the request
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolvePrepare)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called to resolve the request
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolve)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIPageRequest,
     * called when a request resolved
     * @note this event would also be fired to the #ZFUIPageRequest
     */
    ZFOBSERVER_EVENT(RequestOnResolveFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when all request resolved
     * @note it's allowed to post new request during activating this event,
     *   care about dead loop
     */
    ZFOBSERVER_EVENT(RequestOnResolveFinishAll)

    // ============================================================
    // page ani
    /**
     * @brief see #ZFObject::observerNotify
     *
     * see #pageAniOnUpdate\n
     * called to update impl's priority setting
     * (typically page's view tree order)\n
     * param0 is the page that has higher priority,
     * param1 is the other one page
     */
    ZFOBSERVER_EVENT(PageAniOnUpdatePriority)

    // ============================================================
    // embeded logic
public:
    /**
     * @brief called by implementation to achieve page manager's life cycle, see #ZFUIPageManager
     *
     * a page manager can be a window or native UI container,
     * or even embeded to a child page of another page manager,
     * so we supply direct life cycle manage method for implementation to achieve life cycle,
     * you must ensure it's logical valid while handling it
     */
    ZFMETHOD_DECLARE_0(void, embededCreate)
    /** @brief see #embededCreate */
    ZFMETHOD_DECLARE_0(void, embededResume)
    /** @brief see #embededCreate */
    ZFMETHOD_DECLARE_0(void, embededPause)
    /** @brief see #embededCreate */
    ZFMETHOD_DECLARE_0(void, embededDestroy)

public:
    /**
     * @brief true if manager's #embededCreate / #managerOnCreate called
     */
    ZFMETHOD_DECLARE_0(zfbool, managerCreated)
    /**
     * @brief true if manager's #embededResume / #managerOnResume called
     */
    ZFMETHOD_DECLARE_0(zfbool, managerResumed)

    // ============================================================
    // manager control
public:
    /**
     * @brief enable or disable manager's user interaction
     *
     * it's useful to disable user interaction when manager is doing some heavy works,
     * such as animating switching pages\n
     * subclass should override #managerUIBlockedOnChange to achieve actual state change\n
     * this method can be embeded but must be paired,
     * last time you call this method to restore enable state would finally restore enable state
     */
    ZFMETHOD_DECLARE_1(void, managerUIBlocked,
                       ZFMP_IN(zfbool, value))
    /**
     * @brief see #managerUIBlocked
     */
    ZFMETHOD_DECLARE_0(zfindex, managerUIBlocked)
protected:
    /** @brief see #EventManagerUIBlockedOnChange, subclass should implement and achieve actual action */
    virtual void managerUIBlockedOnChange(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerUIBlockedOnChange());
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
     * you must not modify the list,
     * except when using with #movePageBegin
     */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<ZFUIPage *> &, pageList)
    /**
     * @brief page at index
     */
    ZFMETHOD_INLINE_1(ZFUIPage *, pageAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->pageList().get(index);
    }
    /**
     * @brief page at index
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageAtIndex(ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageAtIndex(index));
    }
    /**
     * @brief return foreground page or null if no page
     */
    ZFMETHOD_INLINE_0(ZFUIPage *, pageForeground)
    {
        if(!this->pageList().isEmpty())
        {
            return this->pageList().getLast();
        }
        return zfnull;
    }
    /**
     * @brief return foreground page or null if no page
     */
    template<typename T_ZFUIPage>
    inline T_ZFUIPage pageForeground(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPage, this->pageForeground());
    }

    // ============================================================
    // page request
public:
    /**
     * @brief request create page, see #ZFUIPageManager
     */
    ZFMETHOD_DECLARE_1(void, requestPageCreate,
                       ZFMP_IN(const ZFUIPageRequestPageCreateParam &, createParam))
    /**
     * @brief request create page, see #ZFUIPageManager
     */
    ZFMETHOD_DECLARE_3(void, requestPageCreate,
                       ZFMP_IN(ZFUIPage *, page),
                       ZFMP_IN_OPT(ZFObject *, pageCreateParam, zfnull),
                       ZFMP_IN_OPT(zfbool, pageAutoResume, zftrue))
    /**
     * @brief request resume page
     */
    ZFMETHOD_DECLARE_1(void, requestPageResume,
                       ZFMP_IN(ZFUIPage *, page))
    /**
     * @brief request resume a group of page
     *
     * used to resume multiple pages,
     * all page with same group id would be reordered to page stack's front,
     * keeping their original relative stack order\n
     * null group id belongs to nothing
     */
    ZFMETHOD_DECLARE_1(void, requestPageGroupResume,
                       ZFMP_IN(const zfchar *, pageGroupId))
    /**
     * @brief request destroy page
     */
    ZFMETHOD_DECLARE_1(void, requestPageDestroy,
                       ZFMP_IN(ZFUIPage *, page))
    /**
     * @brief post a custom request to queue, which would be resolved by #requestOnResolve
     */
    ZFMETHOD_DECLARE_1(void, requestPost,
                       ZFMP_IN(ZFUIPageRequest *, request))
    /**
     * @brief util method to post a listener to be execute at current request queue's end
     */
    ZFMETHOD_INLINE_4(void, requestPostCustom,
                      ZFMP_IN(const ZFListener &, listener),
                      ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                      ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                      ZFMP_IN_OPT(ZFObject *, param1, zfnull))
    {
        zfblockedAlloc(ZFUIPageRequestCustom, request);
        request->listener(listener);
        request->userData(userData);
        request->param0(param0);
        request->param1(param1);
        this->requestPost(request);
    }

public:
    /**
     * @brief block request to prevent multiple request running at same time
     *
     * newly posted request would be queued, until restored\n
     * #requestBlocked can be embeded with more than one time,
     * but must be paired
     */
    ZFMETHOD_DECLARE_1(void, requestBlocked,
                       ZFMP_IN(zfbool, value))
    /**
     * @brief see #requestBlocked
     */
    ZFMETHOD_DECLARE_0(zfindex, requestBlocked)
protected:
    /** @brief see #EventRequestBlockedOnChange */
    virtual inline void requestBlockedOnChange(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestBlockedOnChange());
    }

    // ============================================================
    // request resolve
protected:
    /** @brief see #EventRequestOnResolvePrepare */
    virtual inline void requestOnResolvePrepare(ZF_IN ZFUIPageRequest *pageRequest)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolvePrepare(), pageRequest);
        pageRequest->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolvePrepare(), pageRequest);
    }
    /**
     * @brief resolve request, #ZFUIPageRequest::requestResolved must be set to true
     *   if you have resolved the request
     *
     * builtin request would be dispatched by this method to its proper resolve method,
     * such as #requestOnResolvePageCreate\n
     * for custom request, you should check the request's class type, and do your own work
     */
    virtual void requestOnResolve(ZF_IN ZFUIPageRequest *request);
    /** @brief see #EventRequestOnResolveFinish */
    virtual inline void requestOnResolveFinish(ZF_IN ZFUIPageRequest *pageRequest)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolveFinish(), pageRequest);
        pageRequest->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolveFinish(), pageRequest);
    }
    /** @brief see #EventRequestOnResolveFinishAll */
    virtual inline void requestOnResolveFinishAll(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolveFinishAll());
    }

protected:
    /**
     * @brief resolve #requestPageCreate
     *
     * by default, this method would call #resolvePageCreate to create page,
     * you may override and change the behavior, such as delay some time then invoke #resolvePageCreate
     */
    virtual void requestOnResolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageResume(ZF_IN ZFUIPageRequestPageResume *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageResume(ZF_IN ZFUIPageRequestPageResume *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request);

    /** @brief see #requestOnResolvePageCreate */
    virtual void requestOnResolveCustom(ZF_IN ZFUIPageRequestCustom *request);
    /** @brief see #requestOnResolvePageCreate */
    zffinal void resolveCustom(ZF_IN ZFUIPageRequestCustom *request);

    // ============================================================
    // pageDelayDetach
protected:
    /**
     * @brief check whether the page need delay to detach, do nothing by default
     *
     * for some case, page need to process additional works after pause,
     * and should not be detached immediately,
     * to achieve this, you may override this method and call #pageDelayDetachEnable,
     * then finally notify finish by #pageDelayDetachNotifyFinish
     * to finally detach the page\n
     * #pageDelayDetachEnable can be called multiple times,
     * bust must be paired with #pageDelayDetachNotifyFinish\n
     * all delay detach task must be finished before next attach task,
     * otherwise, assert fail\n
     * delaying detach page would also cause #pageDelayDestroyEnable to be set automatically
     * @see pageDelayDestroyOnCheck
     */
    virtual void pageDelayDetachOnCheck(ZF_IN ZFUIPage *page,
                                        ZF_IN ZFUIPagePauseReasonEnum reason);
    /** @brief see #pageDelayDetachOnCheck */
    zffinal void pageDelayDetachEnable(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDetachOnCheck */
    zffinal void pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page);
private:
    zffinal void _ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page);

    // ============================================================
    // pageDelayDestroy
protected:
    /**
     * @brief check whether the page need delay to destroy, do nothing by default
     *
     * for some case, page need to process additional works after pause,
     * and should not be destroyed immediately,
     * to achieve this, you may override this method and call #pageDelayDestroyEnable,
     * then finally notify finish by #pageDelayDestroyNotifyFinish
     * to finally destroy the page\n
     * #pageDelayDestroyEnable can be called multiple times,
     * bust must be paired with #pageDelayDestroyNotifyFinish\n
     * page that delaying to destroy would be removed from page stack,
     * and can't be accessed anymore
     * @see pageDelayDetachOnCheck
     */
    virtual void pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyEnable(ZF_IN ZFUIPage *page);
    /** @brief see #pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page);
private:
    zffinal void _ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page);

    // ============================================================
    // page move
protected:
    /**
     * @brief for advanced page manage only, directly move the page stack,
     *   must be paired with #movePageEnd
     *
     * usage:
     * @code
     *   // mark to begin move page
     *   manager->movePageBegin();
     *
     *   // directly access the page list
     *   ZFCoreArrayPOD<ZFUIPage *> &pageList = manager->pageList();
     *   // modify pageList
     *   // you can only change the order of the pageList contents
     *   // adding or removing is not allowed
     *
     *   // finally mark move end, page's resume state would be updated directly
     *   manager->movePageEnd();
     * @endcode
     */
    zffinal void movePageBegin(void);
    /** @brief see #movePageBegin */
    zffinal void movePageEnd(void);

    // ============================================================
    // page ani
public:
    /**
     * @brief manually override default page animation for once
     *
     * set null to disable animation\n
     * overrided animation would be reset after animation stop,
     * or, you may cancel manually by #pageAniOverrideForOnceCancel
     * newly overrided animation would override old settings
     */
    ZFMETHOD_DECLARE_3(void, pageAniOverrideForOnce,
                       ZFMP_IN(ZFAnimation *, pageAniResume),
                       ZFMP_IN(ZFAnimation *, pageAniPause),
                       ZFMP_IN_OPT(zfbool, pageAniPauseHasHigherPriority, zffalse))
    /**
     * @brief see #pageAniOverrideForOnceCancel
     */
    ZFMETHOD_DECLARE_0(void, pageAniOverrideForOnceCancel)
protected:
    /**
     * @brief used by #pageAniOnUpdate to check update overrided animation
     */
    zffinal zfbool pageAniOverrideForOnceCheckUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                                                     ZF_IN ZFUIPage *pausePageOrNull,
                                                     ZF_IN ZFUIPageResumeReasonEnum reason);
protected:
    /**
     * @brief called to achieve actual page animation
     *
     * by default, this method would:
     * -  if #pageAniOverrideForOnce has been set, use the overrided animation
     * -  otherwise, use page's default animation setting
     *
     * you may override without calling super to override default animation\n
     * \n
     * \n
     * the default impl:
     * @code
     *   ZFUIPage *pausePage = xxx; // page to pause
     *   ZFUIPage *resumePage = xxx; // page to resume
     *   resumePage->pageAniPriorityNeedHigher = zffalse;
     *   pausePage->pageAniPriorityNeedHigher = zffalse;
     *   pausePage->pageAniOnUpdate(resumePage, pausePage, reason);
     *   resumePage->pageAniOnUpdate(resumePage, pausePage, reason);
     *   zfbool pausePageHasHigherPriority = (!resumePage->pageAniPriorityNeedHigher && pausePage->pageAniPriorityNeedHigher);
     *   pageManager->pageAniOnUpdatePriority(resumePage, pausePage, pausePageHasHigherPriority);
     * @endcode
     * by default, #pageAniOnUpdate would update page animation
     * according to page's resume and pause reason,
     * and page requested by user would have higher animation priority (request resume or request destroy)\n
     * the page's animation:
     * -  #ZFUIPage::pageAniResumeByRequest
     * -  #ZFUIPage::pageAniResumeFromBackground
     * -  #ZFUIPage::pageAniPauseToBackground
     * -  #ZFUIPage::pageAniPauseBeforeDestroy
     */
    virtual void pageAniOnUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                                 ZF_IN ZFUIPage *pausePageOrNull,
                                 ZF_IN ZFUIPageResumeReasonEnum reason);
    /**
     * @brief called for subclass to do extra update according the priority
     *
     * typically, for page with higher priority, its page view should be placed at top of view tree
     */
    virtual inline void pageAniOnUpdatePriority(ZF_IN ZFUIPage *resumePageOrNull,
                                                ZF_IN ZFUIPage *pausePageOrNull,
                                                ZF_IN zfbool pausePageHasHigherPriority)
    {
    }

    // ============================================================
    // page manager life cycle
protected:
    /** @brief for subclass to do actual works */
    virtual void managerOnCreate(void)
    {
        this->toObject()->observerNotify(zfself::EventManagerOnCreate());
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnResume(void)
    {
        this->toObject()->observerNotify(zfself::EventManagerOnResume());
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnPause(void)
    {
        this->toObject()->observerNotify(zfself::EventManagerOnPause());
    }
    /** @brief for subclass to do actual works */
    virtual void managerOnDestroy(void)
    {
        this->toObject()->observerNotify(zfself::EventManagerOnDestroy());
    }

    // ============================================================
    // page life cycle
protected:
    /** @brief see #ZFUIPage::EventPageOnCreate */
    virtual void pageOnCreate(ZF_IN ZFUIPage *page,
                              ZF_IN const ZFUIPageRequestPageCreateParam &createParam)
    {
        this->toObject()->observerNotifyWithCustomSender(page->toObject(), ZFUIPage::EventPageOnCreate());
    }
    /** @brief see #ZFUIPage::EventPageOnResume */
    virtual void pageOnResume(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageOnResume(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageOnAttach */
    virtual void pageOnAttach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageOnAttach(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageOnDetach */
    virtual void pageOnDetach(ZF_IN ZFUIPage *page,
                              ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageOnDetach(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /**
     * @brief called to check whether needs to #pageOnDetach page from manager
     *
     * for managers that needs to keep more than one page being attached,
     * you may simply override this method and return false\n
     * page would ensured to be detached before destroy\n
     * \n
     * typical usage:
     * -  always return false, to ensure all page attached, useful for tab-based manager
     * -  return false if current page count greater than 2, useful to implement "swipe to go back"
     */
    virtual zfbool pageOnDetachCheckRequire(ZF_IN ZFUIPage *page,
                                            ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        return zftrue;
    }
    /** @brief see #ZFUIPage::EventPageOnPause */
    virtual void pageOnPause(ZF_IN ZFUIPage *page,
                             ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageOnPause(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageOnDestroy */
    virtual void pageOnDestroy(ZF_IN ZFUIPage *page)
    {
        this->toObject()->observerNotifyWithCustomSender(page->toObject(), ZFUIPage::EventPageOnDestroy());
    }

    // ============================================================
    // page manager events
protected:
    /** @brief see #EventManagerAfterCreate */
    virtual void managerAfterCreate(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerAfterCreate());
    }
    /** @brief see #EventManagerAfterResume */
    virtual void managerAfterResume(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerAfterResume());
    }
    /** @brief see #EventManagerBeforePause */
    virtual void managerBeforePause(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerBeforePause());
    }
    /** @brief see #EventManagerBeforeDestroy */
    virtual void managerBeforeDestroy(void)
    {
        this->toObject()->observerNotify(ZFUIPageManager::EventManagerBeforeDestroy());
    }

    // ============================================================
    // page events
protected:
    /** @brief see #ZFUIPage::EventPageAfterCreate */
    virtual void pageAfterCreate(ZF_IN ZFUIPage *page)
    {
        this->toObject()->observerNotifyWithCustomSender(page->toObject(), ZFUIPage::EventPageAfterCreate());
    }
    /** @brief see #ZFUIPage::EventPageBeforeResume */
    virtual void pageBeforeResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageBeforeResume(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageAfterResume */
    virtual void pageAfterResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageAfterResume(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageBeforePause */
    virtual void pageBeforePause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageBeforePause(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageAfterPause */
    virtual void pageAfterPause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotifyWithCustomSender(
            page->toObject(), ZFUIPage::EventPageAfterPause(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /** @brief see #ZFUIPage::EventPageBeforeDestroy */
    virtual void pageBeforeDestroy(ZF_IN ZFUIPage *page)
    {
        this->toObject()->observerNotifyWithCustomSender(page->toObject(), ZFUIPage::EventPageBeforeDestroy());
    }

private:
    _ZFP_ZFUIPageManagerPrivate *d;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
    friend zfclassFwd ZFUIPage;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPageManager(void)
    : d(zfnull)
    {
    }
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageManager_h_


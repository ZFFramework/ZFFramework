#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIPage_test_Page : zfextends ZFUIPage
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_Page, ZFUIPage)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _titleView, zflineAlloc(ZFUILinearLayout))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _titleLeftView, zflineAlloc(ZFUIButtonBasic))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUITextView *, _titleCenterView, zflineAlloc(ZFUITextView))
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIButtonBasic *, _contentView, zflineAlloc(ZFUIButtonBasic))

protected:
    zfoverride
    virtual void pageOnCreate(void)
    {
        this->pageViewClass(ZFUILinearLayout::ClassData());
        zfsuper::pageOnCreate();
        this->pageView()->to<ZFUILinearLayout *>()->layoutOrientation(ZFUIOrientation::e_Top);

        this->pageView()->childAdd(this->_titleView())->c_widthFill();
        this->_titleView()->viewBackgroundColor(ZFUIColorRandom());
        this->_titleView()->layoutOrientation(ZFUIOrientation::e_Left);
        this->_titleView()->viewSizeMin(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()));

        this->_titleView()->childAdd(this->_titleLeftView())->c_layoutWeight(1);
        this->_titleLeftView()->label()->text("back");
        this->_titleLeftView()->viewBackgroundColor(ZFUIColorRandom());
        {
            ZFUIPage *page = this;
            ZFLISTENER_1(_titleLeftViewOnClick
                    , ZFUIPage *, page
                    ) {
                page->pageDestroy();
            } ZFLISTENER_END(_titleLeftViewOnClick)
            this->_titleLeftView()->observerAdd(ZFUIButton::EventButtonOnClick(), _titleLeftViewOnClick);
        }

        this->_titleView()->childAdd(this->_titleCenterView())->c_layoutWeight(3);
        this->_titleCenterView()->text(zfstringWithFormat("page %p", this));
        this->_titleCenterView()->textAlign(ZFUIAlign::e_Center);

        this->pageView()->childAdd(this->_contentView())->c_sizeFill();
        this->_contentView()->viewBackgroundColor(ZFUIColorRandom());
        this->_contentView()->label()->text(zfstringWithFormat("belong to %s", this->pageGroupId().cString()));
        {
            ZFUIPage *page = this;
            ZFLISTENER_1(_contentViewOnClick
                    , ZFUIPage *, page
                    ) {
                zfblockedAlloc(ZFUIWidget_ZFUIPage_test_Page, newPage);
                newPage->pageGroupId(page->pageGroupId());
                page->pageManager()->pageCreate(newPage);
                page->pageView()->viewId(zfstringWithFormat("%s:%p", page->pageGroupId().cString(), page));
            } ZFLISTENER_END(_contentViewOnClick)
            this->_contentView()->observerAdd(ZFUIButton::EventButtonOnClick(), _contentViewOnClick);
        }
    }
    static ZFCompareResult pageFindFirstByPageGroupId(ZF_IN ZFUIPage * const &e0,
                                                      ZF_IN const zfchar * const &e1)
    {
        if(zfscmpTheSame(e0->pageGroupId().cString(), e1))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        zfsuper::pageOnResume(reason);
        zfindex index = this->pageManager()->pageList().find(this->pageGroupId().cString(), zfself::pageFindFirstByPageGroupId);
        if(index != zfindexMax() && this->pageManager()->pageAt(index) != this)
        {
            this->_titleLeftView()->viewVisible(zftrue);
        }
        else
        {
            this->_titleLeftView()->viewVisible(zffalse);
        }
    }
    zfoverride
    virtual void pageAniOnPrepare(ZF_IN ZFEnum *resumeOrPauseReason, ZF_IN ZFUIPage *siblingPage)
    {
        ZFUIPageAniPrepareForSlide(this, resumeOrPauseReason, siblingPage);
    }
};
zfclass ZFUIWidget_ZFUIPage_test_PageManager : zfextends ZFUIPageManager
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_PageManager, ZFUIPageManager)

    ZFPROPERTY_RETAIN_WITH_INIT(ZFUILinearLayout *, _buttonLayout, zflineAlloc(ZFUILinearLayout))

protected:
    zfoverride
    virtual void managerOnCreate(void)
    {
        this->managerContainerClass(ZFUILinearLayout::ClassData());
        zfsuper::managerOnCreate();

        this->managerContainer()->to<ZFUILinearLayout *>()->layoutOrientation(ZFUIOrientation::e_Top);
        this->pageContainer()->layoutParam<ZFUILinearLayoutParam *>()->layoutWeight(1);

        this->managerContainer()->childAdd(this->_buttonLayout())->c_widthFill();
        this->_buttonLayout()->layoutOrientation(ZFUIOrientation::e_Left);

        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(v_zfstring, pageGroupId, zfstringWithFormat("pageGroup %zi", i));

            zfblockedAlloc(ZFUIButtonBasic, button);
            this->_buttonLayout()->childAdd(button)->c_layoutWeight(1);
            button->label()->text(zfstringWithFormat("tab %zi", i));
            button->checkable(zftrue);
            ZFUIWidget_ZFUIPage_test_PageManager *pageManager = this;
            ZFLISTENER_2(buttonOnClick
                    , ZFUIWidget_ZFUIPage_test_PageManager *, pageManager
                    , zfautoObjectT<v_zfstring *>, pageGroupId
                    ) {
                if(!zfargs.sender()->to<ZFUIButton *>()->checked())
                { // click checked tab
                    zfargs.sender()->to<ZFUIButton *>()->checked(zftrue);

                    ZFCoreArrayPOD<ZFUIPage *> pageToDestroy;
                    for(zfindex i = pageManager->pageCount() - 1; i != zfindexMax(); --i)
                    {
                        if(pageManager->pageAt(i)->pageGroupId().compare(pageGroupId->zfv) == 0)
                        {
                            pageToDestroy.add(pageManager->pageAt(i));
                        }
                    }
                    if(pageToDestroy.count() > 1)
                    {
                        for(zfindex i = pageToDestroy.count() - 2; i != zfindexMax(); --i)
                        {
                            pageToDestroy[i]->pageDestroy();
                        }
                    }
                }
                else
                {
                    for(zfindex i = 0; i < pageManager->_buttonLayout()->childCount(); ++i)
                    {
                        ZFUIButton *button = pageManager->_buttonLayout()->childAt(i)->toAny();
                        button->checked(zffalse);
                    }
                    zfargs.sender()->to<ZFUIButton *>()->checked(zftrue);

                    zfblockedAlloc(ZFAnimationNativeView, resumeAni);
                    resumeAni->aniScaleXFrom(0.8f);
                    resumeAni->aniScaleYFrom(0.8f);
                    resumeAni->aniAlphaFrom(0.5f);
                    zfblockedAlloc(ZFAnimationNativeView, pauseAni);
                    pauseAni->aniAlphaTo(0);
                    pageManager->pageAniOverride(resumeAni, pauseAni);

                    if(pageManager->pageListForGroupId(pageGroupId->zfv).isEmpty())
                    {
                        zfblockedAlloc(ZFUIWidget_ZFUIPage_test_Page, page);
                        page->pageGroupId(pageGroupId->zfv);
                        pageManager->pageCreate(page);
                        page->pageView()->viewId(zfstringWithFormat("%s:%p", page->pageGroupId().cString(), page));
                    }
                    else
                    {
                        pageManager->pageResumeForGroupId(pageGroupId->zfv);
                    }
                }
            } ZFLISTENER_END(buttonOnClick)
            button->observerAdd(ZFUIButton::EventButtonOnClick(), buttonOnClick);
            zffloat r = zfmRand(255) / 255.0f;
            zffloat g = zfmRand(255) / 255.0f;
            zffloat b = zfmRand(255) / 255.0f;
            button->backgroundNormal()->image(ZFUIImageLoadFromColor(ZFUIColorMake(r, g, b, 0.25f)));
            button->backgroundHighlighted()->image(ZFUIImageLoadFromColor(ZFUIColorMake(r, g, b)));
        }
        this->_buttonLayout()->childAt(0)->to<ZFUIButton *>()->simulateClick();
    }
};

zfclass ZFUIWidget_ZFUIPage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test, ZFFramework_test_TestCase)

private:
    ZFUIPageManager *_pageManager;

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFAnimationNativeView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->_pageManager = zfAlloc(ZFUIWidget_ZFUIPage_test_PageManager);
        this->_pageManager->managerCreate();
        container->childAdd(this->_pageManager->managerContainer())->c_sizeFill()->c_margin(20);
        this->_pageManager->managerResume();

        this->prepareSettingButton(window, this->_pageManager);
    }
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
    {
        if(this->_pageManager != zfnull)
        {
            this->_pageManager->managerPause();
            this->_pageManager->managerDestroy();
            zfRelease(this->_pageManager);
            this->_pageManager = zfnull;
        }
        zfsuper::testCaseOnStop(testCaseResult);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIPageManager *pageManager)
    {
        zfblockedAlloc(ZFArray, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIPage_test)

ZF_NAMESPACE_GLOBAL_END


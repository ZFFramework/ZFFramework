#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIPage_test_Page : zfextend ZFUIPage {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_Page, ZFUIPage)

    ZFPROPERTY_RETAIN(zfanyT<ZFUILinearLayout>, _titleView, zfobj<ZFUILinearLayout>())
    ZFPROPERTY_RETAIN(zfanyT<ZFUIButtonBasic>, _titleLeftView, zfobj<ZFUIButtonBasic>())
    ZFPROPERTY_RETAIN(zfanyT<ZFUITextView>, _titleCenterView, zfobj<ZFUITextView>())
    ZFPROPERTY_RETAIN(zfanyT<ZFUIButtonBasic>, _contentView, zfobj<ZFUIButtonBasic>())

protected:
    zfoverride
    virtual void pageOnCreate(void) {
        this->pageViewClass(ZFUILinearLayout::ClassData());
        zfsuper::pageOnCreate();
        this->pageView()->to<ZFUILinearLayout *>()->orientation(v_ZFUIOrientation::e_Top);

        this->pageView()->child(this->_titleView())->c_widthFill();
        this->_titleView()->bgColor(ZFUIColorRandom());
        this->_titleView()->orientation(v_ZFUIOrientation::e_Left);
        this->_titleView()->viewSizeMin(ZFUISizeCreate(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()));

        this->_titleView()->child(this->_titleLeftView())->c_weight(1);
        this->_titleLeftView()->label()->text("back");
        this->_titleLeftView()->bgColor(ZFUIColorRandom());
        {
            ZFUIPage *page = this;
            ZFLISTENER_1(_titleLeftViewOnClick
                    , ZFUIPage *, page
                    ) {
                page->pageDestroy();
            } ZFLISTENER_END()
            this->_titleLeftView()->observerAdd(ZFUIButton::E_ButtonOnClick(), _titleLeftViewOnClick);
        }

        this->_titleView()->child(this->_titleCenterView())->c_weight(3);
        this->_titleCenterView()->text(zfstr("page %s", (const void *)this));
        this->_titleCenterView()->textAlign(v_ZFUIAlign::e_Center);

        this->pageView()->child(this->_contentView())->c_sizeFill();
        this->_contentView()->bgColor(ZFUIColorRandom());
        this->_contentView()->label()->text(zfstr("belong to %s", this->pageGroupId()));
        {
            ZFUIPage *page = this;
            ZFLISTENER_1(_contentViewOnClick
                    , ZFUIPage *, page
                    ) {
                zfobj<ZFUIWidget_ZFUIPage_test_Page> newPage;
                newPage->pageGroupId(page->pageGroupId());
                page->pageManager()->pageCreate(newPage);
                page->pageView()->viewId(zfstr("%s:%s", page->pageGroupId(), (const void *)page));
            } ZFLISTENER_END()
            this->_contentView()->observerAdd(ZFUIButton::E_ButtonOnClick(), _contentViewOnClick);
        }
    }
    static ZFCompareResult pageFindFirstByPageGroupId(
            ZF_IN ZFUIPage * const &e0
            , ZF_IN const zfchar * const &e1
            ) {
        if(zfstringIsEqual(e0->pageGroupId(), e1)) {
            return ZFCompareEqual;
        }
        return ZFCompareUncomparable;
    }
    zfoverride
    virtual void pageOnResume(ZF_IN ZFUIPageResumeReason reason) {
        zfsuper::pageOnResume(reason);
        zfindex index = this->pageManager()->pageList().find(this->pageGroupId().cString(), zfself::pageFindFirstByPageGroupId);
        if(index != zfindexMax() && this->pageManager()->pageAt(index) != this) {
            this->_titleLeftView()->visible(zftrue);
        }
        else {
            this->_titleLeftView()->visible(zffalse);
        }
    }
    zfoverride
    virtual void pageAniOnPrepare(
            ZF_IN ZFEnum *resumeOrPauseReason
            , ZF_IN ZFUIPage *siblingPage
            ) {
        ZFUIPageAniPrepareForSlide(this, resumeOrPauseReason, siblingPage);
    }
};
zfclass ZFUIWidget_ZFUIPage_test_PageManager : zfextend ZFUIPageManager {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test_PageManager, ZFUIPageManager)

    ZFPROPERTY_RETAIN(zfanyT<ZFUILinearLayout>, _buttonLayout, zfobj<ZFUILinearLayout>())

protected:
    zfoverride
    virtual void managerOnCreate(void) {
        this->managerContainerClass(ZFUILinearLayout::ClassData());
        zfsuper::managerOnCreate();

        this->managerContainer()->to<ZFUILinearLayout *>()->orientation(v_ZFUIOrientation::e_Top);
        this->pageContainer()->layoutParam().to<ZFUILinearLayoutParam *>()->weight(1);

        this->managerContainer()->child(this->_buttonLayout())->c_widthFill();
        this->_buttonLayout()->orientation(v_ZFUIOrientation::e_Left);

        for(zfindex i = 0; i < 4; ++i) {
            zfobj<v_zfstring> pageGroupId(zfstr("pageGroup %s", i));

            zfobj<ZFUIButtonBasic> button;
            this->_buttonLayout()->child(button)->c_weight(1);
            button->label()->text(zfstr("tab %s", i));
            button->checkable(zftrue);
            ZFUIWidget_ZFUIPage_test_PageManager *pageManager = this;
            ZFLISTENER_2(buttonOnClick
                    , ZFUIWidget_ZFUIPage_test_PageManager *, pageManager
                    , zfautoT<v_zfstring>, pageGroupId
                    ) {
                if(!zfargs.sender()->to<ZFUIButton *>()->checked()) { // click checked tab
                    zfargs.sender()->to<ZFUIButton *>()->checked(zftrue);

                    ZFCoreArray<ZFUIPage *> pageToDestroy;
                    for(zfindex i = pageManager->pageCount() - 1; i != zfindexMax(); --i) {
                        if(pageManager->pageAt(i)->pageGroupId().compare(pageGroupId->zfv) == 0) {
                            pageToDestroy.add(pageManager->pageAt(i));
                        }
                    }
                    if(pageToDestroy.count() > 1) {
                        for(zfindex i = pageToDestroy.count() - 2; i != zfindexMax(); --i) {
                            pageToDestroy[i]->pageDestroy();
                        }
                    }
                }
                else {
                    for(zfindex i = 0; i < pageManager->_buttonLayout()->childCount(); ++i) {
                        ZFUIButton *button = pageManager->_buttonLayout()->childAt(i);
                        button->checked(zffalse);
                    }
                    zfargs.sender()->to<ZFUIButton *>()->checked(zftrue);

                    pageManager->pageAniOverride(
                            zfobj<ZFAniGroup>()
                            ->c_child(ZFAni("scaleX", zfobj<v_zffloat>(0.8f), zfobj<v_zffloat>(1)))
                            ->c_child(ZFAni("scaleY", zfobj<v_zffloat>(0.8f), zfobj<v_zffloat>(1)))
                            ->c_child(ZFAni("alpha", zfobj<v_zffloat>(0.5f), zfobj<v_zffloat>(1)))
                            ,

                            ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0))
                            );

                    if(pageManager->pageListForGroupId(pageGroupId->zfv).isEmpty()) {
                        zfobj<ZFUIWidget_ZFUIPage_test_Page> page;
                        page->pageGroupId(pageGroupId->zfv);
                        pageManager->pageCreate(page);
                        page->pageView()->viewId(zfstr("%s:%s", page->pageGroupId(), (const void *)page.toObject()));
                    }
                    else {
                        pageManager->pageResumeForGroupId(pageGroupId->zfv);
                    }
                }
            } ZFLISTENER_END()
            button->observerAdd(ZFUIButton::E_ButtonOnClick(), buttonOnClick);
            zffloat r = zfmRandFloat();
            zffloat g = zfmRandFloat();
            zffloat b = zfmRandFloat();
            button->bgNormal()->image(ZFUIImageFromColor(ZFUIColorCreate(r, g, b, 0.25f)));
            button->bgHighlighted()->image(ZFUIImageFromColor(ZFUIColorCreate(r, g, b)));
        }
        this->_buttonLayout()->childAt(0)->to<ZFUIButton *>()->simulateClick();
    }
};

zfclass ZFUIWidget_ZFUIPage_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIPage_test, ZFFramework_test_TestCase)

private:
    zfautoT<ZFUIPageManager> _pageManager;

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        zfautoT<ZFUIWindow> window;
        zfautoT<ZFUIView> container;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->_pageManager = zfobj<ZFUIWidget_ZFUIPage_test_PageManager>();
        this->_pageManager->managerCreate();
        container->child(this->_pageManager->managerContainer())->c_sizeFill()->c_margin(20);
        this->_pageManager->managerResume();

        this->prepareSettingButton(window, this->_pageManager);
    }
    zfoverride
    virtual void testCaseOnStop(ZF_IN ZFResultType testCaseResult) {
        if(this->_pageManager != zfnull) {
            this->_pageManager->managerPause();
            this->_pageManager->managerDestroy();
            this->_pageManager = zfnull;
        }
        zfsuper::testCaseOnStop(testCaseResult);
    }

    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->_pageManager = zfnull;
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIPageManager *pageManager
            ) {
        zfobj<ZFArray> settings;

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIPage_test)

ZF_NAMESPACE_GLOBAL_END


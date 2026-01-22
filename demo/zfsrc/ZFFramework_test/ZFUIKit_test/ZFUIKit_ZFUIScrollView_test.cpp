#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIScrollView_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIScrollView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIScrollView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIScrollView> scrollView;
        container->child(scrollView)->c_sizeFill()->c_margin(40);
        scrollView->bgColor(ZFUIColorCreateRGB(0xFF0000));

        this->setupScrollListener(scrollView);
        this->setupScrollContent(scrollView, ZFUISizeCreate(120), ZFUISizeCreate(60), ZFUISizeCreate(10));

#if 1 // test embeded scroll view
        zfobj<ZFUIScrollView> embededScrollView;
        scrollView->child(embededScrollView)->c_sizeFill(200, 100)->c_margin(80);
        embededScrollView->bgColor(ZFUIColorCreate(0, 0, 1, 0.75f));

        this->setupScrollListener(embededScrollView);
        this->setupScrollContent(embededScrollView, ZFUISizeCreate(60), ZFUISizeCreate(30), ZFUISizeCreate(10));
#endif

        this->prepareSettingButton(window, scrollView);
    }

private:
    void setupScrollListener(ZF_IN ZFUIScrollView *scrollView) {
#if 1 // output logs during scroll event may cause performance issue
        ZFLISTENER(onDragBegin) {
            ZFLogTrim() << "onDragBegin   " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnDragBegin(), onDragBegin);

        ZFLISTENER(onDrag) {
            ZFLogTrim() << "onDrag        " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnDrag(), onDrag);

        ZFLISTENER(onDragEnd) {
            ZFLogTrim() << "onDragEnd     " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnDragEnd(), onDragEnd);

        ZFLISTENER(onScrollBegin) {
            ZFLogTrim() << "onScrollBegin " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnScrollBegin(), onScrollBegin);

        ZFLISTENER(onScroll) {
            ZFLogTrim() << "onScroll      " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnScroll(), onScroll);

        ZFLISTENER(onScrollEnd) {
            ZFLogTrim() << "onScrollEnd   " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIScrollView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        scrollView->observerAdd(ZFUIScrollView::E_ScrollOnScrollEnd(), onScrollEnd);
#endif
    }
    void setupScrollContent(
            ZF_IN ZFUIScrollView *scrollView
            , ZF_IN const ZFUISize &itemSize
            , ZF_IN const ZFUISize &itemSpace
            , ZF_IN const ZFUISize &itemCount
            , ZF_IN_OPT const ZFUIMargin &contentMargin = ZFUIMarginZero()
            ) {
        scrollView->scrollContentFrame(ZFUIRectCreate(
            0,
            0,
            ZFUIMarginGetWidth(contentMargin) + itemSize.width + (itemSize.width + itemSpace.width) * itemCount.width,
            ZFUIMarginGetHeight(contentMargin) + itemSize.height + (itemSize.height + itemSpace.height) * itemCount.height));

        for(zffloat y = contentMargin.top;
                y <= scrollView->scrollContentFrame().height - itemSize.height - contentMargin.bottom;
                y += itemSize.height + itemSpace.height
                ) {
            for(zffloat x = contentMargin.left;
                    x <= scrollView->scrollContentFrame().height - itemSize.width - contentMargin.right;
                    x += itemSize.width + itemSpace.width
                    ) {
                zfobj<ZFUIButton> btn;
                scrollView->child(btn)->c_sizeFill(itemSize)->c_margin(x, y, 0, 0);
                btn->bgColor(ZFUIColorRandom(0.75f));

                ZFLISTENER(onClick) {
                    ZFLogTrim() << "clicked " << zfargs.sender();
                } ZFLISTENER_END()
                btn->onClick(onClick);
            }
        }
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIScrollView *scrollView
            ) {
        zfobj<ZFArray> settings;
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToAxis));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVertical));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceHorizontalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollBounceVerticalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, scrollView, ZFPropertyAccess(ZFUIScrollView, scrollAlignToPageVertical));

#define _ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed 100
        { // auto scroll x
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIScrollView *, scrollView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("autoScrollSpeedX: %s", scrollView->autoScrollSpeedX());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIScrollView *, scrollView
                    ) {
                if(scrollView->autoScrollSpeedX() == 0) {
                    scrollView->autoScrollStartX(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else {
                    scrollView->autoScrollStopX();
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }
        { // auto scroll y
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIScrollView *, scrollView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("autoScrollSpeedY: %s", scrollView->autoScrollSpeedY());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIScrollView *, scrollView
                    ) {
                if(scrollView->autoScrollSpeedY() == 0) {
                    scrollView->autoScrollStartY(_ZFP_ZFUIKit_ZFUIScrollView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else {
                    scrollView->autoScrollStopY();
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        { // scroll child to visible
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0();
                text->zfv = "scrollChildToVisible";
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIScrollView *, scrollView
                    ) {
                zfindex index = scrollView->childCount() / 2;
                scrollView->scrollChildToVisible(scrollView->childAt(index));
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        { // scrollAreaMargin
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIScrollView *, scrollView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("scrollAreaMargin: %s", scrollView->scrollAreaMargin());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIScrollView *, scrollView
                    ) {
                if(scrollView->scrollAreaMargin().left >= 40) {
                    scrollView->scrollAreaMarginRemove(scrollView->scrollAreaMargin());
                }
                else {
                    scrollView->scrollAreaMarginAdd(ZFUIMarginCreate(20));
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIScrollView_test)

ZF_NAMESPACE_GLOBAL_END


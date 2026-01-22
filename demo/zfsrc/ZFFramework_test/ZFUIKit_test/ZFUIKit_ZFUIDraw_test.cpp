#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUIDraw_test_DrawableView : zfextend ZFUIDrawableView {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIDraw_test_DrawableView, ZFUIDrawableView)

public:
    static void testDraw(
            ZF_IN void *context
            , ZF_IN const ZFUISize &ownerSize
            ) {
        ZFUIDraw::drawColor(context,
            ZFUIColorCreateRGB(0x0000FF),
            ZFUIRectCreate(0, 0, ownerSize.width, ownerSize.height));
        ZFUIDraw::drawImage(context,
            zfres("test_normal.png"),
            ZFUIRectZero(),
            ZFUIRectCreate(ownerSize.width, ownerSize.height, ownerSize.width, ownerSize.height));
    }

protected:
    zfoverride
    virtual void onDraw(void) {
        zfsuper::onDraw();
        void *context = ZFUIDraw::beginForView(this);
        ZFUIKit_ZFUIDraw_test_DrawableView::testDraw(
            context,
            ZFUISizeApplyScale(ZFUIRectGetSize(this->viewFrame()), this->UIScaleFixed()));
        ZFUIDraw::endForView(context);
    }
};

zfclass ZFUIKit_ZFUIDraw_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIDraw_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_protocolCheck(ZFUIDraw);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        container->bgColor(ZFUIColorRandom(1));

        zfobj<ZFUIKit_ZFUIDraw_test_DrawableView> view0;
        container->child(view0)->c_sizeFill(100, 100)->c_alignLeft();

        zfobj<ZFUIImageView> view1;
        container->child(view1)->c_sizeFill(100, 100)->c_alignRight();
        void *context = ZFUIDraw::beginForImage(ZFUISizeApplyScale(ZFUISizeCreate(100), view1->UIScaleFixed()));
        ZFUIKit_ZFUIDraw_test_DrawableView::testDraw(
            context,
            ZFUISizeApplyScale(ZFUISizeCreate(100), view1->UIScaleFixed()));
        zfautoT<ZFUIImage> image = ZFUIDraw::endForImage(context);
        view1->image(image);

        zfobj<ZFArray> views;
        views->add(view0);
        views->add(view1);
        this->prepareSettingButton(window, views);
    }

private:
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFArray *views
            ) {
        zfobj<ZFArray> settings;

        { // imageScaleType
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);
            ZFLISTENER(buttonTextGetter) {
                v_zfstring *text = zfargs.param0();
                text->zfv = "change layout";
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);
            ZFLISTENER_1(buttonClickListener
                    , zfautoT<ZFArray>, views
                    ) {
                ZFUIView *first = views->getFirst();
                ZFUISize sizeHint = first->layoutParam()->sizeHint();
                if(sizeHint.height == 100) {
                    sizeHint.height = 150;
                }
                else {
                    sizeHint.height = 100;
                }
                for(zfindex i = 0; i < views->count(); ++i) {
                    ZFUIView *child = views->get(i);
                    child->layoutParam()->sizeHint(sizeHint);
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIDraw_test)

ZF_NAMESPACE_GLOBAL_END


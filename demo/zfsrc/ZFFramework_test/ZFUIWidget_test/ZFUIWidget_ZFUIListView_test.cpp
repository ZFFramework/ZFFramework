#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIListView_test_CellAdapter : zfextend ZFObject, zfimplement ZFUICellAdapter {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIListView_test_CellAdapter, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFUICellAdapter)

public:
    virtual zfindex cellCount(void) {
        return 2000;
    }
    virtual zffloat cellSizeAt(
            ZF_IN zfindex index
            , ZF_IN ZFUICell *cell
            ) {
        #if 1
            return this->cellSizeHint() + zfmRand(100);
        #else
            return zfsuperI(ZFUICellAdapter)::cellSizeAt(index, cell);
        #endif
    }
    virtual zfautoT<ZFUICell> cellAt(ZF_IN zfindex index) {
        zfobj<ZFUICell> ret;
        ret->cellView(zfobj<ZFUICellViewBasic>());
        return ret;
    }
    virtual inline void cellOnUpdate(
            ZF_IN zfindex atIndex
            , ZF_IN ZFUICell *cell
            ) {
        zfsuperI(ZFUICellAdapter)::cellOnUpdate(atIndex, cell);

        ZFUICellViewBasic *tmp = cell->cellView();
        tmp->cellLabelMain()->text(zfstr("main %s", atIndex));
        if(zfmRand(3) == 0) {
            tmp->cellLabelSub()->text(zfstr("sub %s", atIndex));
        }
        if(zfmRand(3) == 0) {
            tmp->cellIcon()->image(ZFUIImageFromColor(ZFUIColorRandom(), ZFUISizeCreate(40)));
        }
        tmp->bgColor(ZFUIColorRandom());
    }

    #if 1 // whether use cache
        virtual inline zfauto cellCacheOnAccess(ZF_IN zfindex index) {
            return this->cellCacheDefaultAccess(zfnull);
        }
        virtual inline void cellCacheOnRecycle(ZF_IN ZFUICell *cell) {
            this->cellCacheDefaultRecycle(zfnull, cell);
        }
    #endif
};

zfclass ZFUIWidget_ZFUIListView_test : zfextend ZFFramework_test_TestCase {
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIListView_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN(zfanyT<ZFUICellAdapter>, cellAdapter, zfobj<ZFUIWidget_ZFUIListView_test_CellAdapter>())

protected:
    zfoverride
    virtual void testCaseOnStart(void) {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFFramework_test_asyncTestCheck();

        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfobj<ZFUIListView> listView;
        container->child(listView)->c_sizeFill()->c_margin(40);
        listView->bgColor(ZFUIColorCreateRGB(0xFF0000));
        listView->cellAdapter(this->cellAdapter());
        {
            zfobj<ZFUIListViewCellUpdater> cellUpdater;
            listView->cellUpdater()->add(cellUpdater);
            cellUpdater->separatorSize(5);
            cellUpdater->separatorColor(ZFUIColorCreateRGB(0xFF0000));
        }

        this->setupScrollListener(listView);

        this->prepareSettingButton(window, listView);
    }

private:
    void setupScrollListener(ZF_IN ZFUIListView *listView) {
#if 0 // output logs during scroll event may cause performance issue
        ZFLISTENER(onDragBegin) {
            ZFLogTrim() << "onDragBegin   " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnDragBegin(), onDragBegin);

        ZFLISTENER(onDrag) {
            ZFLogTrim() << "onDrag        " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnDrag(), onDrag);

        ZFLISTENER(onDragEnd) {
            ZFLogTrim() << "onDragEnd     " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnDragEnd(), onDragEnd);

        ZFLISTENER(onScrollBegin) {
            ZFLogTrim() << "onScrollBegin " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnScrollBegin(), onScrollBegin);

        ZFLISTENER(onScroll) {
            ZFLogTrim() << "onScroll      " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnScroll(), onScroll);

        ZFLISTENER(onScrollEnd) {
            ZFLogTrim() << "onScrollEnd   " << zfargs.sender()->objectInfoOfInstance() << " " << zfargs.sender()->to<ZFUIListView *>()->scrollContentFrame();
        } ZFLISTENER_END()
        listView->observerAdd(ZFUIListView::E_ScrollOnScrollEnd(), onScrollEnd);
#endif
    }
    void prepareSettingButton(
            ZF_IN ZFUIWindow *window
            , ZF_IN ZFUIListView *listView
            ) {
        zfobj<ZFArray> settings;
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollAlignToAxis));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceHorizontal));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceVertical));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceHorizontalAlways));
        ZFUIKit_test_prepareSettingForBoolProperty(settings, listView, ZFPropertyAccess(ZFUIListView, scrollBounceVerticalAlways));
        ZFUIKit_test_prepareSettingForNormalProperty(settings, listView, ZFUIOrientation, ZFPropertyAccess(ZFUIListView, orientation),
            ZFCoreArrayCreate(ZFUIOrientation
                , v_ZFUIOrientation::e_Top
                , v_ZFUIOrientation::e_Left
                , v_ZFUIOrientation::e_Right
                , v_ZFUIOrientation::e_Bottom
                ));
        ZFUIKit_test_prepareSettingForNormalProperty(settings, listView->cellAdapter(), zffloat, ZFPropertyAccess(ZFUICellAdapter, cellSizeHint), ZFCoreArrayCreate(zffloat, 56, 100, 44));

#define _ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed 100
        { // auto scroll x
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIListView *, listView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("autoScrollSpeedX: %s", listView->autoScrollSpeedX());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIListView *, listView
                    ) {
                if(listView->autoScrollSpeedX() == 0) {
                    listView->autoScrollStartX(_ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else {
                    listView->autoScrollStopX();
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }
        { // auto scroll y
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIListView *, listView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("autoScrollSpeedY: %s", listView->autoScrollSpeedY());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIListView *, listView
                    ) {
                if(listView->autoScrollSpeedY() == 0) {
                    listView->autoScrollStartY(_ZFP_ZFUIWidget_ZFUIListView_test_autoScrollSpeed * ((zfmRand(2) == 0) ? 1 : -1));
                }
                else {
                    listView->autoScrollStopY();
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        { // scroll random cell to random position
            ZFLISTENER_1(buttonClickListener
                    , ZFUIListView *, listView
                    ) {
                zfbool toHead = (zfmRand(2) == 0);
                zfindex toIndex = zfmRand(listView->cellAdapter()->cellCount());
                zffloat toOffset = (ZFUIOrientationIsHorizontal(listView->orientation())
                    ? zfmRand((zfint)listView->viewFrame().width)
                    : zfmRand((zfint)listView->viewFrame().height));
                zfbool animated = (zfmRand(5) != 0);
                ZFUIHintShow(zfstr("%s\nindex: %s\noffset: %s\nanimated: %s",
                            toHead ? "scrollCellToHead" : "scrollCellToTail",
                            toIndex, toOffset, animated));
                if(toHead) {
                    listView->scrollCellToHead(toIndex, toOffset, animated);
                }
                else {
                    listView->scrollCellToTail(toIndex, toOffset, animated);
                }
            } ZFLISTENER_END()
            zfobj<ZFUIKit_test_Button> randomScrollButton;
            window->child(randomScrollButton);
            randomScrollButton->label()->text("random scroll");
            randomScrollButton->observerAdd(ZFUIButton::E_ButtonOnClick(), buttonClickListener);
        }

        { // scrollAreaMargin
            zfobj<ZFUIKit_test_SettingData> setting;
            settings->add(setting);

            ZFLISTENER_1(buttonTextGetter
                    , ZFUIListView *, listView
                    ) {
                v_zfstring *text = zfargs.param0();
                text->zfv = zfstr("scrollAreaMargin: %s", listView->scrollAreaMargin());
            } ZFLISTENER_END()
            setting->buttonTextGetter(buttonTextGetter);

            ZFLISTENER_1(buttonClickListener
                    , ZFUIListView *, listView
                    ) {
                if(listView->scrollAreaMargin().left >= 40) {
                    listView->scrollAreaMarginRemove(listView->scrollAreaMargin());
                }
                else {
                    listView->scrollAreaMarginAdd(ZFUIMarginCreate(20));
                }
            } ZFLISTENER_END()
            setting->buttonClickListener(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIListView_test)

ZF_NAMESPACE_GLOBAL_END


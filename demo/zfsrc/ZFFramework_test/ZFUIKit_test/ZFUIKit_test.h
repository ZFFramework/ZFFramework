#ifndef _ZFI_ZFUIKit_test_h_
#define _ZFI_ZFUIKit_test_h_

#include "../../ZFFramework_test_global.h"
#include "ZFUIKit.h"
#include "ZFUIWidget/ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_test_Window : zfextend ZFUIWindow {
    ZFOBJECT_DECLARE(ZFUIKit_test_Window, ZFUIWindow)

    ZFPROPERTY_ON_INIT_INLINE(ZFUIColor, bgColor) {
        propertyValue = ZFUIColorWhite();
    }
};

zfclass ZFUIKit_test_Button : zfextend ZFUIButtonBasic {
    ZFOBJECT_DECLARE(ZFUIKit_test_Button, ZFUIButtonBasic)

    ZFPROPERTY_ON_INIT_INLINE(zfanyT<ZFUIImageView>, bgNormal) {
        propertyValue.to<ZFUIImageView *>()->bgColor(ZFUIColorGreen());
    }
    ZFPROPERTY_ON_INIT_INLINE(zfanyT<ZFUIImageView>, bgHighlighted) {
        propertyValue.to<ZFUIImageView *>()->bgColor(ZFUIColorBlue());
    }
};

zfclass ZFUIKit_test_ListView : zfextend ZFUIScrollView {
    ZFOBJECT_DECLARE(ZFUIKit_test_ListView, ZFUIScrollView)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        this->scrollBounceVerticalAlways(zftrue);
    }

    zfoverride
    virtual void layoutParamOnUpdate(ZF_IN ZFUILayoutParam *layoutParam) {
        zfsuper::layoutParamOnUpdate(layoutParam);

        layoutParam->sizeParam(ZFUISizeParamFillWrap());
        layoutParam->align(v_ZFUIAlign::e_Top);
    }
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
        zfsuper::layoutOnLayoutPrepare(bounds);

        zffloat contentHeight = 0;
        zffloat space = 4;
        ZFUISize childMeasureSizeHint = ZFUISizeCreate(this->scrollArea().width, -1);
        ZFUISizeParam childMeasureSizeParam = ZFUISizeParamFillWrap();

        for(zfindex i = 0; i < this->childCount(); ++i) {
            contentHeight += space;

            ZFUIView *child = this->childAt(i);
            child->layoutMeasure(childMeasureSizeHint, childMeasureSizeParam);
            child->layoutParam()->margin(ZFUIMarginCreate(0, contentHeight, 0, 0));

            contentHeight += child->layoutMeasuredSize().height;
        }
        contentHeight += space;

        this->scrollContentFrameUpdate(ZFUIRectCreate(
            this->scrollContentFrame().x, this->scrollContentFrame().y,
            this->scrollArea().width, contentHeight));
    }
};

extern void ZFUIKit_test_prepareTestWindow(
        ZF_OUT ZFUIWindow *&window
        , ZF_OUT ZFUIView *&container
        , ZF_IN ZFTestCase *testCaseToStop
        );

zfclass ZFUIKit_test_SettingData : zfextend ZFObject {
    ZFOBJECT_DECLARE(ZFUIKit_test_SettingData, ZFObject)

public:
    ZFEVENT(SettingOnUpdate)

public:
    /**
     * param0 is a #v_zfstring to get the button's text
     */
    ZFPROPERTY_ASSIGN(ZFListener, buttonTextGetter)
    ZFPROPERTY_ASSIGN(ZFListener, buttonClickListener)

public:
    zffinal void settingUpdate(void) {
        this->observerNotify(zfself::E_SettingOnUpdate());
    }

protected:
    virtual void objectOnInit(
            ZF_IN const ZFListener &buttonTextGetter
            , ZF_IN const ZFListener &buttonClickListener
            ) {
        this->objectOnInit();
        this->buttonTextGetter(buttonTextGetter);
        this->buttonClickListener(buttonClickListener);
    }
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
    }
};
extern zfauto ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings);
extern void ZFUIKit_test_prepareSettingButtonWithTestWindow(
        ZF_IN ZFUIWindow *window
        , ZF_IN ZFArray *settings
        );
extern void ZFUIKit_test_prepareSettingForProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFListener &nextCallback
        );

// ============================================================
extern void ZFUIKit_test_prepareSettingForBoolProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFProperty *property
        );

zfclass _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder : zfextend ZFObject {
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder, ZFObject)
public:
    ZFObject *obj;
    const ZFProperty *property;
    ZFCoreArrayBase *propertyValues;
    zfindex index;
public:
    _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder(void)
    : obj(zfnull)
    , property(zfnull)
    , propertyValues(zfnull)
    , index(0)
    {
    }
protected:
    virtual void objectOnDeallocPrepare(void) {
        if(this->propertyValues) {
            this->propertyValues->refDelete();
        }
        zfsuper::objectOnDeallocPrepare();
    }
};
#define ZFUIKit_test_prepareSettingForNormalProperty(settings, obj_, PropertyType, property_, propertyValues_) \
    do { \
        zfobj<_ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder> taskData; \
        taskData->obj = obj_; \
        taskData->property = property_; \
        taskData->propertyValues = propertyValues_.refNew(); \
        ZFLISTENER_1(nextCallback \
                , zfautoT<_ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder>, taskData \
                ) { \
            _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder *holder = taskData; \
            ZFCoreArray<PropertyType> const &propertyValues = *(const ZFCoreArray<PropertyType> *)holder->propertyValues; \
            ++(holder->index); \
            if(holder->index >= propertyValues.count()) { \
                holder->index = 0; \
            } \
            holder->property->setterMethod()->executeExact<void, PropertyType const &>( \
                holder->obj, propertyValues[holder->index]); \
        } ZFLISTENER_END() \
        ZFUIKit_test_prepareSettingForProperty(settings, taskData->obj, taskData->property, nextCallback); \
    } while(zffalse)

extern void ZFUIKit_test_prepareSettingForLayoutRequest(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFUIView *view
        );

extern void ZFUIKit_test_prepareSettingForResetProperty(
        ZF_IN_OUT ZFArray *settings
        , ZF_IN ZFObject *obj
        , ZF_IN const ZFCoreArray<const ZFProperty *> &propertyList
        );

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFUIKit_test_h_


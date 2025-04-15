#include "ZFUIButtonGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIButtonGroupType)

ZFOBJECT_REGISTER(ZFUIButtonGroup)

ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonOnAdd)
ZFEVENT_REGISTER(ZFUIButtonGroup, ButtonOnRemove)
ZFEVENT_REGISTER(ZFUIButtonGroup, TabOnUpdate)
ZFEVENT_REGISTER(ZFUIButtonGroup, TabOnClickChecked)

zfclassNotPOD _ZFP_ZFUIButtonGroupPrivate {
public:
    ZFArray *buttons;
    ZFListener buttonOnEventListener;
    ZFListener buttonOnClickListener;
    ZFUIView *container;
public:
    _ZFP_ZFUIButtonGroupPrivate(void)
    : buttons(zfnull)
    , buttonOnEventListener()
    , buttonOnClickListener()
    , container(zfnull)
    {
    }

public:
    void setup_common(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        if(!this->buttonOnEventListener) {
            ZFLISTENER_1(buttonEvent
                    , ZFUIButtonGroup *, owner
                    ) {
                ZFUIButton *button = zfargs.sender();
                zfindex buttonIndex = owner->buttonFind(button);
                ZFCoreAssert(buttonIndex != zfindexMax());
                owner->buttonOnEvent(button, buttonIndex, zfargs.eventId());
            } ZFLISTENER_END()
            this->buttonOnEventListener = buttonEvent;
        }

        button->observerAdd(ZFUIButton::E_ButtonOnClick(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonStateOnUpdate(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnDown(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveEnter(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveExit(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveInside(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnMoveOutside(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnUpInside(), this->buttonOnEventListener);
        button->observerAdd(ZFUIButton::E_ButtonMouseOnUp(), this->buttonOnEventListener);
    }
    void cleanup_common(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        button->observerRemove(ZFUIButton::E_ButtonOnClick(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonStateOnUpdate(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnDown(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveEnter(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveExit(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveInside(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnMoveOutside(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnUpInside(), this->buttonOnEventListener);
        button->observerRemove(ZFUIButton::E_ButtonMouseOnUp(), this->buttonOnEventListener);
    }
    void setup_Normal(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        this->setup_common(owner, button);
    }
    void cleanup_Normal(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        this->cleanup_common(owner, button);
    }
    void setup_Tab(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        this->setup_common(owner, button);

        button->checkable(zftrue);
        button->checked(zffalse);

        if(!this->buttonOnClickListener) {
            ZFLISTENER_1(buttonOnClick
                    , ZFUIButtonGroup *, owner
                    ) {
                ZFUIButton *button = zfargs.sender();
                zfindex buttonIndex = owner->buttonFind(button);
                ZFCoreAssert(buttonIndex != zfindexMax());
                if(buttonIndex == owner->tabChecked()) {
                    if(owner->tabAllowUnchecked()) {
                        owner->tabChecked(zfindexMax());
                        owner->tabOnUpdate(button, buttonIndex);
                    }
                    else {
                        button->checked(zftrue);
                    }
                    owner->tabOnClickChecked(button, buttonIndex);
                }
                else {
                    zfindex buttonIndexPrev = owner->tabChecked();
                    owner->tabChecked(buttonIndex);
                    owner->tabOnUpdate(button, buttonIndexPrev);
                }
            } ZFLISTENER_END()
            this->buttonOnClickListener = buttonOnClick;
        }
        button->observerAdd(ZFUIButton::E_ButtonOnClick(), this->buttonOnClickListener);

        if(!owner->tabAllowUnchecked() && owner->tabChecked() == zfindexMax()) {
            owner->tabChecked(owner->buttonFind(button));
        }
    }
    void cleanup_Tab(
            ZF_IN ZFUIButtonGroup *owner
            , ZF_IN ZFUIButton *button
            ) {
        this->cleanup_common(owner, button);
        button->observerRemove(ZFUIButton::E_ButtonOnClick(), this->buttonOnClickListener);
    }
};

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfbool, tabAllowUnchecked) {
    if(!this->tabAllowUnchecked() && this->buttonCount() > 0 && this->tabChecked() == zfindexMax()) {
        this->tabChecked(0);
    }
}
ZFPROPERTY_ON_UPDATE_DEFINE(ZFUIButtonGroup, zfindex, tabChecked) {
    propertyValue = ((propertyValue >= this->buttonCount()) ? zfindexMax() : propertyValue);
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, zfindex, tabChecked) {
    if(propertyValue != propertyValueOld) {
        if(!this->tabAllowUnchecked() && this->tabChecked() >= this->buttonCount()) {
            return;
        }
        if(propertyValueOld != zfindexMax()) {
            this->buttonAt(propertyValueOld)->checked(zffalse);
        }
        if(this->tabChecked() != zfindexMax()) {
            this->buttonAt(this->tabChecked())->checked(zftrue);
        }
    }
}

// ============================================================
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIButtonGroup, ZFUIButtonGroupType, type) {
    if(this->type() == propertyValueOld) {
        return;
    }
    switch(propertyValueOld) {
        case v_ZFUIButtonGroupType::e_Normal:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->cleanup_Normal(this, this->buttonAt(i));
            }
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->cleanup_Tab(this, this->buttonAt(i));
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
    switch(propertyValue) {
        case v_ZFUIButtonGroupType::e_Normal:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->setup_Normal(this, this->buttonAt(i));
            }
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->setup_Tab(this, this->buttonAt(i));
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
}

ZFMETHOD_DEFINE_0(ZFUIButtonGroup, zfindex, buttonCount) {
    return d->buttons->count();
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, zfindex, buttonFind
        , ZFMP_IN(ZFUIButton *, button)
        ) {
    return d->buttons->find(button);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, zfanyT<ZFUIButton>, buttonAt
        , ZFMP_IN(zfindex, buttonIndex)
        ) {
    return d->buttons->get(buttonIndex);
}
ZFMETHOD_DEFINE_2(ZFUIButtonGroup, void, button
        , ZFMP_IN(ZFUIButton *, button)
        , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
        ) {
    if(atIndex == zfindexMax()) {
        atIndex = this->buttonCount();
    }
    else {
        ZFCoreAssertIndexRange(atIndex, this->buttonCount());
    }
    d->buttons->add(button, atIndex);
    switch(this->type()) {
        case v_ZFUIButtonGroupType::e_Normal:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->setup_Normal(this, this->buttonAt(i));
            }
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->setup_Tab(this, this->buttonAt(i));
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
    this->buttonOnAdd(button, atIndex);
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemove
        , ZFMP_IN(ZFUIButton *, button)
        ) {
    this->buttonRemoveAt(d->buttons->find(button));
}
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, buttonRemoveAt
        , ZFMP_IN(zfindex, buttonIndex)
        ) {
    if(buttonIndex == zfindexMax()) {
        return;
    }
    ZFCoreAssertIndexRange(buttonIndex, this->buttonCount());
    ZFUIButton *button = this->buttonAt(buttonIndex);
    zfRetain(button);
    switch(this->type()) {
        case v_ZFUIButtonGroupType::e_Normal:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->cleanup_Normal(this, this->buttonAt(i));
            }
            break;
        case v_ZFUIButtonGroupType::e_Tab:
            for(zfindex i = 0; i < this->buttonCount(); ++i) {
                d->cleanup_Tab(this, this->buttonAt(i));
            }
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            break;
    }
    d->buttons->remove(buttonIndex);
    this->buttonOnRemove(button, buttonIndex);
    zfRelease(button);
}
ZFMETHOD_DEFINE_0(ZFUIButtonGroup, void, buttonRemoveAll) {
    while(this->buttonCount() > 0) {
        this->buttonRemoveAt(this->buttonCount() - 1);
    }
}

void ZFUIButtonGroup::objectOnInit(void) {
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonGroupPrivate);
    d->buttons = zfAlloc(ZFArray);
}
void ZFUIButtonGroup::objectOnDealloc(void) {
    this->containerDetach();
    zfRelease(d->buttons);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

// ============================================================
ZFMETHOD_DEFINE_1(ZFUIButtonGroup, void, containerAttach
        , ZFMP_IN(ZFUIView *, container)
        ) {
    if(container == d->container) {
        return;
    }
    this->containerDetach();
    if(container == zfnull) {
        return;
    }
    this->buttonRemoveAll();
    for(zfindex i = 0; i < container->childCount(); ++i) {
        ZFUIButton *child = container->childAt(i);
        if(child) {
            this->button(child);
        }
    }
    d->container = container;
    ZFUIButtonGroup *owner;

    ZFLISTENER_1(containerOnDealloc
            , ZFUIButtonGroup *, owner
            ) {
        owner->containerDetach();
    } ZFLISTENER_END()

    ZFLISTENER_1(containerChildOnAdd
            , ZFUIButtonGroup *, owner
            ) {
        ZFUIButton *child = zfargs.param0();
        if(child) {
            owner->button(child);
        }
    } ZFLISTENER_END()

    ZFLISTENER_1(containerChildOnRemove
            , ZFUIButtonGroup *, owner
            ) {
        ZFUIButton *child = zfargs.param0();
        if(child) {
            owner->buttonRemove(child);
        }
    } ZFLISTENER_END()

    ZFObserverGroup(d->buttons, container)
        .observerAdd(ZFObject::E_ObjectBeforeDealloc(), containerOnDealloc)
        .observerAdd(ZFUIView::E_ViewChildOnAdd(), containerChildOnAdd)
        .observerAdd(ZFUIView::E_ViewChildOnRemove(), containerChildOnRemove)
        ;
}
ZFMETHOD_DEFINE_0(ZFUIButtonGroup, void, containerDetach) {
    if(!d->container) {
        return;
    }
    this->buttonRemoveAll();
    d->container = zfnull;
    ZFObserverGroupRemove(d->buttons);
}
ZFMETHOD_DEFINE_0(ZFUIButtonGroup, ZFUIView *, container) {
    return d->container;
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIButtonGroup
        , ZFMP_IN(ZFUIView *, container)
        ) {
    this->objectOnInit();
    this->containerAttach(container);
}

ZF_NAMESPACE_GLOBAL_END


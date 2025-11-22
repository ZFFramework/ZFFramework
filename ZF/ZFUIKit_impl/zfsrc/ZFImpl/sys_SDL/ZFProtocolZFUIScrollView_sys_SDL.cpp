#include "ZFImpl_sys_SDL_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#if ZF_ENV_sys_SDL

// ============================================================
zfclassNotPOD _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView : zfextend ZFImpl_sys_SDL_View, zfextend ZFUIScrollViewImplHelperProtocol {
public:
    ZFUIScrollViewImplHelper _ZFP_scrollViewImplHelper;

public:
    zfoverride
    virtual const void *implType(void) {
        return ZFUIScrollView::ClassData();
    }

public:
    virtual zftimet nativeTime(void) {
        return ZFTime::timestamp();
    }
    virtual void mouseEventPos(
            ZF_OUT ZFUIPoint &ret
            , ZF_IN void *nativeMouseEvent
            ) {
        // relative to root renderer
        SDL_Event *sdlEvent = (SDL_Event *)nativeMouseEvent;
        ret.x = sdlEvent->motion.x;
        ret.y = sdlEvent->motion.y;
    }
    virtual void *mouseEventClone(
            ZF_IN void *nativeMouseEvent
            , ZF_IN_OPT zfbool changeMouseAction = zffalse
            , ZF_IN_OPT ZFUIMouseAction mouseAction = v_ZFUIMouseAction::e_Cancel
            ) {
        SDL_Event *ret = zfnew(SDL_Event);
        zfmemcpy(ret, nativeMouseEvent, sizeof(SDL_Event));
        if(changeMouseAction) {
            switch(mouseAction) {
                case v_ZFUIMouseAction::e_Down:
                    ret->type = SDL_EVENT_MOUSE_BUTTON_DOWN;
                    break;
                case v_ZFUIMouseAction::e_Move:
                    ret->type = SDL_EVENT_MOUSE_MOTION;
                    break;
                case v_ZFUIMouseAction::e_Up:
                    ret->type = SDL_EVENT_MOUSE_BUTTON_UP;
                    break;
                case v_ZFUIMouseAction::e_Cancel:
                    ret->type = SDL_EVENT_MOUSE_BUTTON_UP;
                    ret->button.x = ZFImpl_sys_SDL_View::MouseCancel;
                    ret->button.y = ZFImpl_sys_SDL_View::MouseCancel;
                    break;
                default:
                    break;
            }
        }
        return ret;
    }
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent) {
        zfdelete((SDL_Event *)nativeMouseEvent);
    }
    virtual void mouseEventForward(
            ZF_IN void *nativeChild
            , ZF_IN void *nativeMouseEvent
            ) {
        ZFImpl_sys_SDL_View *sdlView = (ZFImpl_sys_SDL_View *)nativeChild;
        SDL_Event *sdlEvent = (SDL_Event *)nativeMouseEvent;
        sdlView->dispatchMouseEvent(sdlEvent);
    }

    static zfbool _ZFP_contains(
            ZF_IN const SDL_FRect &rect
            , ZF_IN zffloat x
            , ZF_IN zffloat y
            ) {
        return x >= rect.x && x <= rect.x + rect.w
            && y >= rect.y && y <= rect.y + rect.h;
    }
    ZFImpl_sys_SDL_View *_ZFP_findChildRecursive(
            ZF_IN ZFImpl_sys_SDL_View *parent
            , ZF_IN zffloat x
            , ZF_IN zffloat y
            , ZF_IN zfbool findScrollView
            ) {
        if(findScrollView && parent->implType() == ZFUIScrollView::ClassData()) {
            return parent;
        }
        for(zfindex i = parent->children.count() - 1; i != zfindexMax(); --i) {
            ZFImpl_sys_SDL_View *t = parent->children[i];
            if((t->ownerZFUIView == zfnull || t->ownerZFUIView->viewUIEnableTree())
                    && _ZFP_contains(t->rect, x, y)
                    ) {
                if(t->implType() == ZFUIScrollView::ClassData()) {
                    return t;
                }
                ZFImpl_sys_SDL_View *inner = _ZFP_findChildRecursive(t, x - t->rect.x, y - t->rect.y, findScrollView);
                if(inner != zfnull) {
                    return inner;
                }
                if(!findScrollView) {
                    return t;
                }
            }
        }
        return zfnull;
    }
    ZFImpl_sys_SDL_View *_ZFP_findChild(
            ZF_IN_OUT zfindex &beforeIndex
            , ZF_IN zffloat x
            , ZF_IN zffloat y
            ) {
        --beforeIndex;
        for( ; beforeIndex != zfindexMax(); --beforeIndex) {
            ZFImpl_sys_SDL_View *t = this->children[beforeIndex];
            if((t->ownerZFUIView == zfnull || t->ownerZFUIView->viewUIEnableTree())
                && _ZFP_contains(t->rect, x, y)
            ) {
                return t;
            }
        }
        return zfnull;
    }
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(
            ZF_IN zffloat x
            , ZF_IN zffloat y
            ) {
        // x/y relative to scroll content frame
        this->posFromGlobal(x, y);

        zfindex beforeIndex = this->children.count();
        do {
            ZFImpl_sys_SDL_View *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull) {
                ZFImpl_sys_SDL_View *innerChild = _ZFP_findChildRecursive(
                    t,
                    x - t->rect.x,
                    y - t->rect.y,
                    zftrue);
                if(innerChild != zfnull) {
                    return &(((_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)innerChild)->_ZFP_scrollViewImplHelper);
                }
            }
        } while(beforeIndex != zfindexMax());
        return zfnull;
    }
    virtual void findTouchedChildScrollViewCleanup(ZF_IN void *nativeChild) {
        // nothing to do
    }
    virtual void *findTouchedChild(
            ZF_IN zffloat x
            , ZF_IN zffloat y
            ) {
        // x/y relative to scroll content frame
        this->posFromGlobal(x, y);

        zfindex beforeIndex = this->children.count();
        do {
            ZFImpl_sys_SDL_View *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull) {
                if(t->implType() == ZFUIScrollView::ClassData()) {
                    return t;
                }
                ZFImpl_sys_SDL_View *innerChild = _ZFP_findChildRecursive(
                    t,
                    x - t->rect.x,
                    y - t->rect.y,
                    zffalse);
                if(innerChild != zfnull) {
                    return innerChild;
                }
                return t;
            }
        } while(beforeIndex != zfindexMax());
        return zfnull;
    }
    virtual void findTouchedChildCleanup(ZF_IN void *nativeChild) {
        // nothing to do
    }

    static zfbool _ZFP_childOffset(
            ZF_OUT int &xOffset
            , ZF_OUT int &yOffset
            , ZF_IN ZFImpl_sys_SDL_View *parent
            , ZF_IN ZFImpl_sys_SDL_View *child
            ) {
        while(child != zfnull && child != parent) {
            xOffset += child->rect.x;
            yOffset += child->rect.y;
            child = child->parent;
        }
        return (child == parent);
    }
    virtual void *translateFromParentToChild(
            ZF_IN void *nativeChild
            , ZF_IN_OUT void *nativeMouseEvent
            , ZF_IN zffloat const &xInParent
            , ZF_IN zffloat const &yInParent
            ) {
        // x/y relative to root renderer, no need to translate
        return nativeMouseEvent;
    }
    virtual void *translateFromChildToParent(
            ZF_IN void *nativeChild
            , ZF_IN_OUT void *nativeMouseEvent
            , ZF_IN zffloat const &xInChild
            , ZF_IN zffloat const &yInChild
            ) {
        // x/y relative to root renderer, no need to translate
        return nativeMouseEvent;
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_SDL, ZFUIScrollView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "ZFImpl_sys_SDL_View")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

private:
    static zfbool sdlMouseGrabCallback(
            ZF_IN ZFImpl_sys_SDL_View *owner
            , ZF_IN SDL_Event *sdlEvent
            , ZF_IN zfint x
            , ZF_IN zfint y
            ) {
        if(sdlEvent == zfnull) {
            return (x >= 0 && x <= owner->rect.w && y >= 0 && y <= owner->rect.h);
        }

        ZFUIMouseAction mouseAction = v_ZFUIMouseAction::e_Down;
        switch(sdlEvent->type) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mouseAction = v_ZFUIMouseAction::e_Down;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                mouseAction = v_ZFUIMouseAction::e_Move;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouseAction = v_ZFUIMouseAction::e_Up;
                break;
            default:
                return zffalse;
        }

        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeScrollView = (_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)owner;
        nativeScrollView->_ZFP_scrollViewImplHelper.interceptMouse(sdlEvent, mouseAction);
        return zftrue;
    }
public:
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *scrollView) {
        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeScrollView = zfnew(_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView);
        nativeScrollView->_ZFP_scrollViewImplHelper.scrollView = scrollView;
        nativeScrollView->_ZFP_scrollViewImplHelper.implProtocol = nativeScrollView;
        nativeScrollView->sdlMouseGrabCallback = zfself::sdlMouseGrabCallback;

        return (void *)(ZFImpl_sys_SDL_View *)nativeScrollView;
    }
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *scrollView) {
        zfdelete((_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)scrollView->nativeImplView());
    }

public:
    virtual void scrollEnable(
            ZF_IN ZFUIScrollView *scrollView
            , ZF_IN zfbool scrollEnable
            ) {
        // nothing to do, scroll impl helper would solve this
    }
    virtual void scrollBounce(
            ZF_IN ZFUIScrollView *scrollView
            , ZF_IN zfbool scrollBounceHorizontal
            , ZF_IN zfbool scrollBounceVertical
            , ZF_IN zfbool scrollBounceHorizontalAlways
            , ZF_IN zfbool scrollBounceVerticalAlways
            ) {
        // nothing to do, scroll impl helper would solve this
    }
    virtual void scrollContentFrame(
            ZF_IN ZFUIScrollView *scrollView
            , ZF_IN const ZFUIRect &frame
            ) {
        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeScrollView = (_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)scrollView->nativeImplView();
        ZFUIRectRound(
                nativeScrollView->rect.x
                , nativeScrollView->rect.y
                , nativeScrollView->rect.w
                , nativeScrollView->rect.h
                , frame
                );
        nativeScrollView->layoutRequest();
    }

public:
    virtual void scrollChildAdd(
            ZF_IN ZFUIScrollView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex atIndex
            ) {
        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeParent = (_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)parent->nativeImplView();
        ZFImpl_sys_SDL_View *nativeChild = (ZFImpl_sys_SDL_View *)child->nativeView();
        nativeParent->childAttach(atIndex, nativeChild);
        nativeParent->layoutRequest();
    }
    virtual void scrollChildRemove(
            ZF_IN ZFUIScrollView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex atIndex
            ) {
        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeParent = (_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)parent->nativeImplView();
        nativeParent->childDetach(atIndex);
        nativeParent->layoutRequest();
    }
    virtual void scrollChildRemoveAllForDealloc(ZF_IN ZFUIScrollView *parent) {
        _ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *nativeParent = (_ZFP_ZFUIScrollViewImpl_sys_SDL_ScrollView *)(ZFImpl_sys_SDL_View *)parent->nativeImplView();
        for(zfindex i = 0; i < nativeParent->children.count(); ++i) {
            ZFImpl_sys_SDL_View *nativeChild = nativeParent->children[i];
            nativeChild->parent = zfnull;
            if(nativeChild->rootWindow != zfnull) {
                nativeChild->rootWindowDetach();
            }
        }
        nativeParent->children.removeAll();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIScrollViewImpl_sys_SDL)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_SDL


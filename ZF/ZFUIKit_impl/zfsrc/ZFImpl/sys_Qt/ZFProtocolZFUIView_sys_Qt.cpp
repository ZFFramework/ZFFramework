#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Qt

#include "ZFProtocolZFUIView_sys_Qt.h"
#include <QGraphicsOpacityEffect>
#include <QGraphicsProxyWidget>
#include <QKeyEvent>

// ============================================================
extern ZFUIKeyCode ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(ZF_IN int qKeyCode);
zfbool (*ZFUIViewImpl_sys_Qt_isMouseCancel)(ZF_IN QGraphicsSceneMouseEvent *event) = zfnull;

QGraphicsSceneMouseEvent *ZFUIViewImpl_sys_Qt_mouseEventClone(
        ZF_IN QGraphicsSceneMouseEvent *event
        , ZF_IN QEvent::Type type
        , ZF_IN QPointF const &localPos
        ) {
    QGraphicsSceneMouseEvent *ret = new QGraphicsSceneMouseEvent(type);

    ret->setWidget(event->widget());

    ret->setPos(localPos);
    ret->setScenePos(event->scenePos());
    ret->setScreenPos(event->screenPos());
    ret->setButtonDownPos(Qt::LeftButton, event->buttonDownPos(Qt::LeftButton));
    ret->setButtonDownPos(Qt::RightButton, event->buttonDownPos(Qt::RightButton));
    ret->setButtonDownPos(Qt::MiddleButton, event->buttonDownPos(Qt::MiddleButton));
    ret->setButtonDownScenePos(Qt::LeftButton, event->buttonDownScenePos(Qt::LeftButton));
    ret->setButtonDownScenePos(Qt::RightButton, event->buttonDownScenePos(Qt::RightButton));
    ret->setButtonDownScenePos(Qt::MiddleButton, event->buttonDownScenePos(Qt::MiddleButton));
    ret->setButtonDownScreenPos(Qt::LeftButton, event->buttonDownScreenPos(Qt::LeftButton));
    ret->setButtonDownScreenPos(Qt::RightButton, event->buttonDownScreenPos(Qt::RightButton));
    ret->setButtonDownScreenPos(Qt::MiddleButton, event->buttonDownScreenPos(Qt::MiddleButton));
    ret->setLastPos(ret->pos());
    ret->setLastScenePos(ret->scenePos());
    ret->setLastScreenPos(ret->screenPos());
    ret->setButtons(event->buttons());
    ret->setButton(event->button());
    ret->setModifiers(event->modifiers());
    ret->setSource(event->source());
    ret->setFlags(event->flags());

    return ret;
}

class _ZFP_ZFUIViewImpl_sys_Qt_Layout : public ZFImpl_sys_Qt_Layout {
public:
    ZFUIView *_ZFP_ownerZFUIView;
    QSizeF _ZFP_layoutedSize;
public:
    _ZFP_ZFUIViewImpl_sys_Qt_Layout(void)
    : ZFImpl_sys_Qt_Layout()
    , _ZFP_ownerZFUIView(zfnull)
    , _ZFP_layoutedSize(QSizeF(0, 0))
    {
    }
protected:
    virtual void onLayout(const QRectF &rect) {
        if(_ZFP_ownerZFUIView != zfnull && _ZFP_layoutedSize != rect.size()
            && (_ZFP_ownerZFUIView->parent() == zfnull || !_ZFP_ownerZFUIView->parent()->layoutRequested())
        ) {
            _ZFP_layoutedSize = rect.size();
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutView(
                _ZFP_ownerZFUIView,
                ZFImpl_sys_Qt_ZFUIRectFromQRectF(rect));
        }
    }
};

// ============================================================
// focus
extern void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(
        ZF_IN ZFUIView *ownerZFUIView
        , ZF_IN QGraphicsWidget *nativeOwner
        , ZF_IN QGraphicsWidget *nativeImplViewOrNull
        , ZF_IN_OPT void *tokenOld = zfnull
        );
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_detach(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusable(
        ZF_IN void *token
        , ZF_IN zfbool v
        );
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusRequest(
        ZF_IN void *token
        , ZF_IN zfbool v
        );
extern zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focused(ZF_IN void *token);

// ============================================================
// native view
class _ZFP_ZFUIViewImpl_sys_Qt_View : public ZFImpl_sys_Qt_View {
    Q_OBJECT

public:
    _ZFP_ZFUIViewImpl_sys_Qt_Layout *_ZFP_layoutProxy;
    void *_ZFP_focusProxyToken;
    QGraphicsWidget *_ZFP_nativeImplView;
    zfbool _ZFP_viewUIEnable;
    zfbool _ZFP_viewUIEnableTree;
    zfbool _ZFP_mousePressed;
    ZFUIPoint _ZFP_mouseMoveLastPoint;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_View(void)
    : ZFImpl_sys_Qt_View()
    , _ZFP_layoutProxy(new _ZFP_ZFUIViewImpl_sys_Qt_Layout())
    , _ZFP_focusProxyToken(zfnull)
    , _ZFP_nativeImplView(zfnull)
    , _ZFP_viewUIEnable(zftrue)
    , _ZFP_viewUIEnableTree(zftrue)
    , _ZFP_mousePressed(zffalse)
    , _ZFP_mouseMoveLastPoint(ZFUIPointZero())
    {
        this->setLayout(_ZFP_layoutProxy);

        QPalette palette;
        palette.setColor(QPalette::Window, QColor(0, 0, 0, 0));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
        this->setAcceptTouchEvents(true);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_View(void) {
        ZFCoreAssert(this->_ZFP_nativeImplView == zfnull);
    }

public:
    bool event(QEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull) {
            return QGraphicsWidget::event(event);
        }
        switch(event->type()) {
            // mouse
            case QEvent::GraphicsSceneMousePress:
            case QEvent::GraphicsSceneMouseDoubleClick:
            case QEvent::GraphicsSceneMouseMove:
            case QEvent::GraphicsSceneMouseRelease:
            // hover
            case QEvent::GraphicsSceneHoverEnter:
            case QEvent::GraphicsSceneHoverMove:
            case QEvent::GraphicsSceneHoverLeave:
            // key
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            // wheel
            case QEvent::GraphicsSceneWheel:
                if(!this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable) {
                    return false;
                }
                return QGraphicsWidget::event(event);

            // default
            default:
                return QGraphicsWidget::event(event);
        }
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        this->_ZFP_mousePressed = zftrue;
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::mousePressEvent(event);
            return;
        }
        this->mouseEventResolve(event, v_ZFUIMouseAction::e_Down);
    }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        this->_ZFP_mouseMoveLastPoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::mouseMoveEvent(event);
            return;
        }
        if(this->_ZFP_mousePressed) {
            this->mouseEventResolve(event, v_ZFUIMouseAction::e_Move);
        }
    }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        this->_ZFP_mousePressed = zffalse;
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::mouseReleaseEvent(event);
            return;
        }
        if(ZFUIViewImpl_sys_Qt_isMouseCancel != zfnull && ZFUIViewImpl_sys_Qt_isMouseCancel(event)) {
            this->mouseEventResolve(event, v_ZFUIMouseAction::e_Cancel);
        }
        else {
            this->mouseEventResolve(event, v_ZFUIMouseAction::e_Up);
        }
    }
private:
    void mouseEventResolve(QGraphicsSceneMouseEvent *event, ZFUIMouseAction mouseAction) {
        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = (zfidentity)event->button();
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        ev->mouseButton = v_ZFUIMouseButton::e_Left;
        switch(event->button()) {
            case Qt::RightButton:
                ev->mouseButton = v_ZFUIMouseButton::e_Right;
                break;
            case Qt::MiddleButton:
                ev->mouseButton = v_ZFUIMouseButton::e_Center;
                break;
            case Qt::LeftButton:
            default:
                ev->mouseButton = v_ZFUIMouseButton::e_Left;
                break;
        }

        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
        event->setAccepted(ev->eventResolved());
    }

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::hoverEnterEvent(event);
            return;
        }
        this->mouseHoverEventResolve(event, v_ZFUIMouseAction::e_HoverEnter);
    }
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::hoverMoveEvent(event);
            return;
        }
        this->mouseHoverEventResolve(event, v_ZFUIMouseAction::e_Hover);
    }
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::hoverLeaveEvent(event);
            return;
        }
        this->mouseHoverEventResolve(event, v_ZFUIMouseAction::e_HoverExit);
    }
private:
    void mouseHoverEventResolve(QGraphicsSceneHoverEvent *event, ZFUIMouseAction mouseAction) {
        zfobj<ZFUIMouseEvent> ev;
        ev->eventResolved(zffalse);
        ev->mouseId = 0;
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIPointFromQPointF(event->pos());
        ev->mouseButton = v_ZFUIMouseButton::e_Left;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
        if(event != NULL) {
            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual void keyPressEvent(QKeyEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::keyPressEvent(event);
            return;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? v_ZFUIKeyAction::e_Repeat : v_ZFUIKeyAction::e_Down);
    }
    virtual void keyReleaseEvent(QKeyEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::keyReleaseEvent(event);
            return;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? v_ZFUIKeyAction::e_Repeat : v_ZFUIKeyAction::e_Up);
    }
private:
    void keyEventResolve(QKeyEvent *event, ZFUIKeyAction keyAction) {
        if(this->_ZFP_viewUIEnableTree && this->_ZFP_viewUIEnable) {
            zfobj<ZFUIKeyEvent> ev;
            ev->eventResolved(zffalse);
            ev->keyId = (zfidentity)event->key();
            ev->keyAction = keyAction;
            ev->keyCode = ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(event->key());
            ev->keyCodeRaw = (zfflags)event->key();
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, ev);
            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event) {
        if(_ZFP_layoutProxy->_ZFP_ownerZFUIView == zfnull
                || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable
                ) {
            QGraphicsWidget::wheelEvent(event);
            return;
        }
        this->wheelEventResolve(event);
    }
private:
    void wheelEventResolve(QGraphicsSceneWheelEvent *event) {
        zffloat eventSteps = event->delta() / 8;

        zfobj<ZFUIWheelEvent> wheelEvent;
        wheelEvent->eventResolved(zffalse);
        if(event->orientation() == Qt::Horizontal) {
            wheelEvent->wheelX = eventSteps;
            wheelEvent->wheelY = 0;
        }
        else {
            wheelEvent->wheelX = 0;
            wheelEvent->wheelY = eventSteps;
        }
        if(wheelEvent->wheelX != 0 || wheelEvent->wheelY != 0) {
            ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(_ZFP_layoutProxy->_ZFP_ownerZFUIView, wheelEvent);
        }
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Qt, ZFUIView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")

public:
    zfoverride
    virtual void protocolOnInitFinish(void) {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = new _ZFP_ZFUIViewImpl_sys_Qt_View();
        nativeView->_ZFP_layoutProxy->_ZFP_ownerZFUIView = view;
        nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(
            nativeView->_ZFP_layoutProxy->_ZFP_ownerZFUIView, nativeView, zfnull);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusable(nativeView->_ZFP_focusProxyToken, zffalse);
        return nativeView;
    }
    virtual void nativeViewDestroy(ZF_IN void *nativeView) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)nativeView;
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(nativeViewTmp->_ZFP_focusProxyToken);
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_ownerZFUIView = zfnull;
        delete nativeViewTmp;
    }

    virtual void nativeImplView(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeImplViewOld
            , ZF_IN void *nativeImplView
            , ZF_IN zfindex virtualIndex
            , ZF_IN zfbool nativeImplViewRequireVirtualIndex
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        QGraphicsWidget *v = (QGraphicsWidget *)nativeImplView;

        if(nativeView->_ZFP_nativeImplView != zfnull) {
            nativeView->_ZFP_layoutProxy->childRemoveAt(virtualIndex);
        }
        nativeView->_ZFP_nativeImplView = v;

        if(nativeImplViewRequireVirtualIndex && nativeView->_ZFP_nativeImplView != zfnull) {
            nativeView->_ZFP_layoutProxy->child(nativeView->_ZFP_nativeImplView, virtualIndex);
            nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_attach(
                view, nativeView, nativeView->_ZFP_nativeImplView, nativeView->_ZFP_focusProxyToken);
            _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusable(nativeView->_ZFP_focusProxyToken, view->focusable());
            this->_updateNativeImplViewMouseSetting(view);
        }
    }
    virtual void nativeImplViewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        QRectF frame = ZFImpl_sys_Qt_ZFUIRectToQRectF(rect);
        ZFImpl_sys_Qt_BaseView::ForceGeometry(nativeView->_ZFP_nativeImplView, frame);
        nativeView->_ZFP_layoutProxy->invalidate();
    }
    virtual zffloat UIScaleForImpl(ZF_IN void *nativeView) {
        return 1;
    }
    virtual zffloat UIScaleForPixel(ZF_IN void *nativeView) {
        return 1;
    }

    // ============================================================
    // properties
public:
    virtual void visible(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool visible
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View  *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        nativeView->setVisible(visible);
    }
    virtual void alpha(
            ZF_IN ZFUIView *view
            , ZF_IN zffloat alpha
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        if(alpha == 1) {
            nativeViewTmp->setGraphicsEffect(zfnull);
        }
        else {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(nativeViewTmp->graphicsEffect());
            if(effect == zfnull) {
                effect = new QGraphicsOpacityEffect(nativeViewTmp);
                nativeViewTmp->setGraphicsEffect(effect);
            }
            effect->setOpacity(alpha);
        }
    }
    virtual void viewUIEnable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnable
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        nativeViewTmp->_ZFP_viewUIEnable = viewUIEnable;
    }
    virtual void viewUIEnableTree(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool viewUIEnableTree
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        nativeViewTmp->_ZFP_viewUIEnableTree = viewUIEnableTree;
        nativeViewTmp->setAcceptTouchEvents(viewUIEnableTree);
        nativeViewTmp->setAcceptedMouseButtons(viewUIEnableTree ? Qt::AllButtons : Qt::NoButton);
        this->_updateNativeImplViewMouseSetting(view);
    }
    virtual void mouseHoverEnable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool mouseHoverEnable
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        nativeViewTmp->setAcceptHoverEvents(mouseHoverEnable);
    }
    virtual void bgColor(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIColor &bgColor
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        QPalette palette = nativeViewTmp->palette();
        palette.setColor(QPalette::Window, ZFImpl_sys_Qt_ZFUIColorToQColor(bgColor));
        nativeViewTmp->setPalette(palette);
    }

public:
    virtual void child(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)parent->nativeView();
        QGraphicsWidget *nativeChildView = (QGraphicsWidget *)child->nativeView();
        nativeView->_ZFP_layoutProxy->child(nativeChildView, virtualIndex);
    }
    virtual void childRemove(
            ZF_IN ZFUIView *parent
            , ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)parent->nativeView();
        nativeView->_ZFP_layoutProxy->childRemoveAt(virtualIndex);
    }
    virtual void childRemoveAllForDealloc(ZF_IN ZFUIView *parent) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)parent->nativeView();
        nativeView->_ZFP_layoutProxy->childRemoveAll();
        if(nativeView->_ZFP_nativeImplView != NULL) {
            nativeView->_ZFP_layoutProxy->child(nativeView->_ZFP_nativeImplView, 0);
        }
    }

public:
    virtual void viewFrame(
            ZF_IN ZFUIView *view
            , ZF_IN const ZFUIRect &rect
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        QRectF frame = ZFImpl_sys_Qt_ZFUIRectToQRectF(rect);
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_layoutedSize = frame.size();
        ZFImpl_sys_Qt_BaseView::ForceGeometry(nativeViewTmp, frame);
        nativeViewTmp->_ZFP_layoutProxy->invalidate();
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        nativeViewTmp->_ZFP_layoutProxy->_ZFP_layoutedSize = QSize(-1, -1);
        nativeViewTmp->_ZFP_layoutProxy->invalidate();
    }

    virtual void measureNativeView(
            ZF_OUT ZFUISize &ret
            , ZF_IN void *nativeView
            , ZF_IN const ZFUISize &sizeHint
            ) {
        QGraphicsWidget *nativeViewTmp = (QGraphicsWidget *)nativeView;
        QSizeF minSizeSaved = nativeViewTmp->minimumSize();
        QSizeF maxSizeSaved = nativeViewTmp->maximumSize();
        nativeViewTmp->setMinimumSize(0, 0);
        nativeViewTmp->setMaximumSize(
                sizeHint.width >= 0 ? sizeHint.width : (zffloat)QWIDGETSIZE_MAX,
                sizeHint.height >= 0 ? sizeHint.height : (zffloat)QWIDGETSIZE_MAX
            );
        QSizeF t = nativeViewTmp->effectiveSizeHint(Qt::MinimumDescent);
        nativeViewTmp->setMinimumSize(minSizeSaved);
        nativeViewTmp->setMaximumSize(maxSizeSaved);
        ZFImpl_sys_Qt_ZFUISizeFromQSizeFT(ret, t);
    }
private:
    void _updateNativeImplViewMouseSetting(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        if(nativeViewTmp->_ZFP_nativeImplView != NULL) {
            nativeViewTmp->_ZFP_nativeImplView->setAcceptTouchEvents(view->viewUIEnableTree());
            nativeViewTmp->_ZFP_nativeImplView->setAcceptedMouseButtons(view->viewUIEnableTree() ? Qt::AllButtons : Qt::NoButton);
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Qt)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_Qt, ZFUIViewFocus, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void focusable(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focusable
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusable(nativeView->_ZFP_focusProxyToken, focusable);
    }
    virtual zfbool focused(ZF_IN ZFUIView *view) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        return _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focused(nativeView->_ZFP_focusProxyToken);
    }
    virtual void focusRequest(
            ZF_IN ZFUIView *view
            , ZF_IN zfbool focus
            ) {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = (_ZFP_ZFUIViewImpl_sys_Qt_View *)view->nativeView();
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_focusRequest(nativeView->_ZFP_focusProxyToken, focus);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt


#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIRootWindow.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"
#include <QEvent>
#include <QGuiApplication>
#include <QScreen>

static void _ZFP_ZFUIRootWindowImpl_sys_Qt_updateWindowLayout(
        ZF_IN ZFUIRootWindow *rootWindow
        , ZF_IN QGraphicsWidget *nativeWindow
        ) {
    QRect screenRect = QGuiApplication::screens().at(0)->geometry();
    ZFUIRect frame = ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyMeasureWindow(
            rootWindow,
            ZFUIRectCreate(0, 0, screenRect.width(), screenRect.height()),
            ZFUIMarginZero()
        );
    if(nativeWindow->scene() != NULL) {
        QList<QGraphicsView *> views = nativeWindow->scene()->views();
        QRect frameTmp = ZFImpl_sys_Qt_ZFUIRectToQRect(frame);
        for(QList<QGraphicsView *>::iterator it = views.begin(); it != views.end(); ++it) {
            (*it)->setFixedSize(frameTmp.width(), frameTmp.height());
            (*it)->setGeometry(frameTmp);
        }
    }
    QRectF bounds(0, 0, frame.width, frame.height);
    ZFImpl_sys_Qt_BaseView::ForceGeometry(nativeWindow, bounds);
}
class _ZFP_ZFUIRootWindowImpl_sys_Qt_EventWrapper : public QObject {
    Q_OBJECT

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) {
        v_zfweak *ownerHolder = ZFImpl_sys_Qt_QObjectZFObjectTag(obj, "_ZFP_ZFUIRootWindowImpl_sys_Qt_ownerZFUIRootWindow");
        if(ownerHolder == zfnull) {
            return false;
        }
        ZFUIRootWindow *owner = ownerHolder->zfv;
        if(owner == zfnull) {
            return false;
        }
        switch(event->type()) {
            case QEvent::WindowActivate:
                ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnResume(owner);
                break;
            case QEvent::WindowDeactivate:
                ZFPROTOCOL_ACCESS(ZFUIRootWindow)->notifyOnPause(owner);
                #if ZF_ENV_DEBUG && 0
                    ZFLogTrim() << ZFImpl_sys_Qt_viewTreePrint(ZFImpl_sys_Qt_rootWindow());
                #endif
                break;
            default:
                break;
        }
        return false;
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIRootWindowImpl_sys_Qt, ZFUIRootWindow, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void) {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUIRootWindow *mainWindow(void) {
        if(this->_mainWindow == zfnull) {
            this->_mainWindow = zfRetain(ZFUIRootWindow::ClassData()->newInstance().to<ZFUIRootWindow *>());
            QGraphicsWidget *nativeWindow = ZFImpl_sys_Qt_rootWindow();
            ZFImpl_sys_Qt_QObjectZFObjectTag(nativeWindow, "_ZFP_ZFUIRootWindowImpl_sys_Qt_ownerZFUIRootWindow", zfobj<v_zfweak>(this->_mainWindow));
            nativeWindow->installEventFilter(&_eventWrapper);

            this->notifyOnCreate(this->_mainWindow, nativeWindow);

            _ZFP_ZFUIRootWindowImpl_sys_Qt_updateWindowLayout(this->_mainWindow, nativeWindow);
            nativeWindow->show();
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void) {
        if(this->_mainWindow != zfnull) {
            if(this->_mainWindow->windowResumed()) {
                this->notifyOnPause(this->_mainWindow);
            }
            zfscopeRelease(this->_mainWindow);
            QGraphicsWidget *nativeWindow = ZFImpl_sys_Qt_rootWindow();
            nativeWindow->removeEventFilter(&_eventWrapper);
            ZFImpl_sys_Qt_QObjectZFObjectTag(nativeWindow, "_ZFP_ZFUIRootWindowImpl_sys_Qt_ownerZFUIRootWindow", zfnull);

            this->notifyOnDestroy(this->_mainWindow);
        }
    }
    virtual void mainWindowOnDestroy(void) {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUIRootWindow *rootWindow) {
    }

    virtual void nativeWindowRootViewOnAdd(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_OUT_OPT void *&nativeParentView
            ) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        QGraphicsWidget *nativeWindow = (QGraphicsWidget *)rootWindow->nativeWindow();

        QGraphicsWidget *nativeRootView = (QGraphicsWidget *)rootWindow->rootView()->nativeView();
        ZFCoreAssertWithMessageTrim(nativeWindow->layout() != NULL,
            "[ZFUIRootWindow] window's QGraphicsWidget::layout not set");
        ZFImpl_sys_Qt_BaseLayout *l = (ZFImpl_sys_Qt_BaseLayout *)(nativeWindow->layout());
        l->child(nativeRootView);
        nativeParentView = (void *)nativeWindow;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        QGraphicsWidget *nativeWindow = (QGraphicsWidget *)rootWindow->nativeWindow();
        QGraphicsWidget *nativeRootView = (QGraphicsWidget *)rootWindow->rootView()->nativeView();
        ZFImpl_sys_Qt_BaseLayout *l = (ZFImpl_sys_Qt_BaseLayout *)(nativeWindow->layout());
        l->childRemove(nativeRootView);
    }

    virtual zfauto modalWindowShow(ZF_IN ZFUIRootWindow *owner) {
        zfauto modalWindow = zfRetain(ZFUIRootWindow::ClassData()->newInstance().to<ZFUIRootWindow *>());
        ZFImpl_sys_Qt_Window *nativeModalWindow = new ZFImpl_sys_Qt_Window();
        ZFImpl_sys_Qt_QObjectZFObjectTag(nativeModalWindow, "_ZFP_ZFUIRootWindowImpl_sys_Qt_ownerZFUIRootWindow", zfobj<v_zfweak>(modalWindow));
        nativeModalWindow->installEventFilter(&_eventWrapper);
        this->notifyOnCreate(modalWindow, nativeModalWindow);

        _ZFP_ZFUIRootWindowImpl_sys_Qt_updateWindowLayout(modalWindow, nativeModalWindow);
        nativeModalWindow->show();

        return modalWindow;
    }
    virtual void modalWindowHide(
            ZF_IN ZFUIRootWindow *owner
            , ZF_IN ZFUIRootWindow *toHide
            ) {
        if(toHide->nativeWindow() == zfnull) {
            return;
        }
        QGraphicsWidget *nativeModalWindow = (QGraphicsWidget *)owner->nativeWindow();
        nativeModalWindow->hide();
        nativeModalWindow->removeEventFilter(&_eventWrapper);
        this->notifyOnDestroy(toHide);
        ZFImpl_sys_Qt_QObjectZFObjectTag(nativeModalWindow, "_ZFP_ZFUIRootWindowImpl_sys_Qt_ownerZFUIRootWindow", zfnull);
        delete nativeModalWindow;
    }

    virtual void layoutParamOnInit(ZF_IN ZFUIRootWindow *rootWindow) {
        // centered by default
        rootWindow->layoutParam()->align(v_ZFUIAlign::e_Center);
        rootWindow->layoutParam()->sizeHint(ZFUISizeCreate(480, 640));
    }
    virtual void layoutParamOnUpdate(ZF_IN ZFUIRootWindow *rootWindow) {
        if(rootWindow->nativeWindow() == zfnull) {
            return;
        }
        _ZFP_ZFUIRootWindowImpl_sys_Qt_updateWindowLayout(rootWindow, (QGraphicsWidget *)rootWindow->nativeWindow());
    }

    virtual ZFUIOrientation windowOrientation(ZF_IN ZFUIRootWindow *rootWindow) {
        // Qt don't support rotate
        return v_ZFUIOrientation::e_Top;
    }
    virtual void windowOrientationFlags(
            ZF_IN ZFUIRootWindow *rootWindow
            , ZF_IN const ZFUIOrientationFlags &flags
            ) {
        // Qt don't support rotate
    }

private:
    ZFUIRootWindow *_mainWindow;
    _ZFP_ZFUIRootWindowImpl_sys_Qt_EventWrapper _eventWrapper;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIRootWindowImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIRootWindow_sys_Qt.moc"

#endif // #if ZF_ENV_sys_Qt


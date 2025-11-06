#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFImpl_sys_Qt_viewInfoT(
        ZF_IN_OUT zfstring &s
        , ZF_IN QObject *view
        ) {
    // class name
    s += view->metaObject()->className();

    // instance
    zfstringAppend(s, " %s", (const void *)view);

    // basic widget info
    if(qobject_cast<QWidget *>(view) != NULL) {
        QWidget *tmp = qobject_cast<QWidget *>(view);

        const QRect &frame = tmp->geometry();
        zfstringAppend(s, " (%s, %s, %s, %s)"
                , (zfint)zfmRound(frame.x())
                , (zfint)zfmRound(frame.y())
                , (zfint)zfmRound(frame.width())
                , (zfint)zfmRound(frame.height())
                );

        if(tmp->focusPolicy() != Qt::NoFocus) {
            zfstringAppend(s, " (focusable:%s)", (int)tmp->focusPolicy());
        }
        if(!tmp->isVisible()) {
            s += " (hidden)";
        }
        if(!tmp->isEnabled()) {
            s += " (disabled)";
        }
    }
    else if(qobject_cast<QGraphicsLayoutItem *>(view) != NULL) {
        QGraphicsLayoutItem *tmp = qobject_cast<QGraphicsLayoutItem *>(view);

        QRectF frame = tmp->geometry();
        zfstringAppend(s, " (%s, %s, %s, %s)"
                , (zfint)zfmRound(frame.x())
                , (zfint)zfmRound(frame.y())
                , (zfint)zfmRound(frame.width())
                , (zfint)zfmRound(frame.height())
                );

        QGraphicsWidget *tmpWidget = qobject_cast<QGraphicsWidget *>(view);
        if(tmpWidget != NULL && tmpWidget->focusPolicy() != Qt::NoFocus) {
            zfstringAppend(s, " (focusable:%s)", (int)tmpWidget->focusPolicy());
        }

        if(tmp->graphicsItem() != NULL) {
            if(!tmp->graphicsItem()->isVisible()) {
                s += " (hidden)";
            }
            if(!tmp->graphicsItem()->isEnabled()) {
                s += " (all disabled)";
            }
            else if(!tmp->graphicsItem()->acceptTouchEvents()) {
                s += " (disabled)";
            }
        }
    }

    // text
    {
        QLabel *tmp = qobject_cast<QLabel *>(view);
        if(tmp != NULL) {
            zfstringAppend(s, " %s", tmp->text().toStdString().c_str());
        }
    }
    {
        QTextEdit *tmp = qobject_cast<QTextEdit *>(view);
        if(tmp != NULL) {
            zfstringAppend(s, " %s", tmp->toPlainText().toStdString().c_str());
        }
    }
}

void ZFImpl_sys_Qt_viewInfoT(
        ZF_OUT zfstring &ret
        , ZF_IN QObject *view
        ) {
    if(view == zfnull) {
        ret = ZFTOKEN_zfnull;
    }
    else {
        _ZFP_ZFImpl_sys_Qt_viewInfoT(ret, view);
    }
}

static void _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ZF_IN_OUT zfstring &s, QObject *view, zfindex depth, zfindex siblingIndex) {
    zfstringAppend(s, "|%2s ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i) {
        s += "| ";
    }

    _ZFP_ZFImpl_sys_Qt_viewInfoT(s, view);

    s += "\n";

    if(qobject_cast<QWidget *>(view) != NULL) {
        QLayout *l = qobject_cast<QWidget *>(view)->layout();
        if(l != zfnull) {
            for(int i = 0; i < l->count(); ++i) {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->itemAt(i)->widget(), depth + 1, i);
            }
        }
    }
    else if(qobject_cast<QGraphicsWidget *>(view) != NULL) {
        QGraphicsLayout *l = qobject_cast<QGraphicsWidget *>(view)->layout();
        if(l != zfnull) {
            for(int i = 0; i < l->count(); ++i) {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->itemAt(i)->graphicsItem()->toGraphicsObject(), depth + 1, i);
            }
        }
        else if(qobject_cast<QGraphicsProxyWidget *>(view) != NULL) {
            QGraphicsProxyWidget *l = qobject_cast<QGraphicsProxyWidget *>(view);
            if(l->widget() != NULL) {
                _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(s, l->widget(), depth + 1, 0);
            }
        }
    }
}
void ZFImpl_sys_Qt_viewTreePrintT(
        ZF_OUT zfstring &ret
        , ZF_IN QObject *view
        ) {
    ret += "======================= Qt view tree =======================\n";
    if(view != zfnull) {
        _ZFP_ZFImpl_sys_Qt_viewTreePrint_recursive(ret, view, 0, 0);
    }
    ret += "----------------------- Qt view tree -----------------------\n";
}

// convert utility
void ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignmentT(
        ZF_OUT Qt::Alignment &ret
        , ZF_IN const ZFUIAlignFlags &align
        ) {
    ret = (Qt::Alignment)0;

    if(ZFBitTest(align, v_ZFUIAlign::e_Left)) {
        ZFBitSet(ret, Qt::AlignLeft);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_Right)) {
        ZFBitSet(ret, Qt::AlignRight);
    }
    else {
        ZFBitSet(ret, Qt::AlignHCenter);
    }

    if(ZFBitTest(align, v_ZFUIAlign::e_Top)) {
        ZFBitSet(ret, Qt::AlignTop);
    }
    else if(ZFBitTest(align, v_ZFUIAlign::e_Bottom)) {
        ZFBitSet(ret, Qt::AlignBottom);
    }
    else {
        ZFBitSet(ret, Qt::AlignVCenter);
    }
}
void ZFImpl_sys_Qt_ZFUIAlignFlagsFromQAlignmentT(
        ZF_OUT ZFUIAlignFlags &ret
        , ZF_IN const Qt::Alignment qAlign
        ) {
    ret = v_ZFUIAlign::e_Center;

    if(ZFBitTest(qAlign, Qt::AlignLeft)) {
        ZFBitSet(ret, v_ZFUIAlign::e_Left);
    }
    else if(ZFBitTest(qAlign, Qt::AlignRight)) {
        ZFBitSet(ret, v_ZFUIAlign::e_Right);
    }

    if(ZFBitTest(qAlign, Qt::AlignTop)) {
        ZFBitSet(ret, v_ZFUIAlign::e_Top);
    }
    else if(ZFBitTest(qAlign, Qt::AlignBottom)) {
        ZFBitSet(ret, v_ZFUIAlign::e_Bottom);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if 0
    #include "ZFUIKit/ZFUIRootWindow.h"
    ZF_GLOBAL_INITIALIZER_INIT(ZFImpl_sys_Qt_autoPrintViewTree) {
        if(!ZFProtocolIsAvailable("ZFUIView")) {
            return;
        }
        ZFLISTENER(windowOnPause) {
            ZFUIRootWindow *rootWindow = zfargs.sender();
            zfstring s;
            ZFImpl_sys_Qt_viewTreePrintT(s, (QGraphicsWidget *)rootWindow->rootView()->nativeView());
            ZFLogTrim() << s;
        } ZFLISTENER_END()
        this->windowOnPauseListener = windowOnPause;
        ZFGlobalObserver().observerAdd(
            ZFUIRootWindow::E_WindowOnPause(), this->windowOnPauseListener);
    }
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Qt_autoPrintViewTree) {
        ZFGlobalObserver().observerRemove(
            ZFUIRootWindow::E_WindowOnPause(), this->windowOnPauseListener);
    }
    private:
        ZFListener windowOnPauseListener;
    ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Qt_autoPrintViewTree)
#endif

#endif // #if ZF_ENV_sys_Qt


#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_Qt

#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"
#include <QLabel>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsProxyWidget>

class _ZFP_ZFUITextViewImpl_sys_Qt_TextView : public QLabel {
    Q_OBJECT

public:
    _ZFP_ZFUITextViewImpl_sys_Qt_TextView(void)
    : QLabel()
    {
        this->_ZFP_textColor(ZFUIColorBlack());
        this->_ZFP_textSize(14);
        this->setWordWrap(false);
        this->setGraphicsEffect(zfnull);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Qt::transparent);
        this->setPalette(palette);
    }

public:
    void _ZFP_textSize(ZF_IN zffloat v) {
        QFont font = this->font();
        font.setPixelSize((int)v);
        this->setFont(font);
    }
    void _ZFP_textColor(ZF_IN ZFUIColor v) {
        QPalette palette = this->palette();
        palette.setColor(QPalette::WindowText, ZFImpl_sys_Qt_ZFUIColorToQColor(v));
        this->setPalette(palette);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_Qt, ZFUITextView, v_ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsProxyWidget:QLabel")

public:
    virtual void *nativeTextViewCreate(
            ZF_IN ZFUITextView *textView
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
        proxy->setWidget(new _ZFP_ZFUITextViewImpl_sys_Qt_TextView());
        return proxy;
    }
    virtual void nativeTextViewDestroy(
            ZF_IN ZFUITextView *textView
            , ZF_IN void *nativeTextView
            ) {
        QGraphicsProxyWidget *proxy = (QGraphicsProxyWidget *)nativeTextView;
        delete proxy;
    }
private:
    _ZFP_ZFUITextViewImpl_sys_Qt_TextView *getNativeImplView(ZF_IN ZFUITextView *textView) {
        QGraphicsProxyWidget *proxy = (QGraphicsProxyWidget *)textView->nativeImplView();
        return qobject_cast<_ZFP_ZFUITextViewImpl_sys_Qt_TextView *>(proxy->widget());
    }

// ============================================================
// properties
public:
    virtual void text(
            ZF_IN ZFUITextView *textView
            , ZF_IN const zfstring &text
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setText(ZFImpl_sys_Qt_zfstringToQString(text));
    }
    virtual void textAppearance(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        QFont font = nativeImplView->font();
        switch(textAppearance) {
            case v_ZFUITextAppearance::e_Normal:
                font.setBold(false);
                font.setItalic(false);
                break;
            case v_ZFUITextAppearance::e_Bold:
                font.setBold(true);
                font.setItalic(false);
                break;
            case v_ZFUITextAppearance::e_Italic:
                font.setBold(false);
                font.setItalic(true);
                break;
            case v_ZFUITextAppearance::e_BoldItalic:
                font.setBold(true);
                font.setItalic(true);
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
        nativeImplView->setFont(font);
    }
    virtual void textAlign(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setAlignment(ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignment(textAlign));
    }
    virtual void textColor(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUIColor const &textColor
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->_ZFP_textColor(textColor);
    }
    virtual void textSize(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSize
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMin(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMin
            ) {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoMax(
            ZF_IN ZFUITextView *textView
            , ZF_IN zffloat textSizeAutoMax
            ) {
        // changed during layoutNativeTextView
    }
    virtual void singleLine(
            ZF_IN ZFUITextView *textView
            , ZF_IN zfbool singleLine
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        nativeImplView->setWordWrap(!singleLine);
    }
    virtual void textTruncateMode(
            ZF_IN ZFUITextView *textView
            , ZF_IN ZFUITextTruncateMode const &textTruncateMode
            ) {
#if 0 // not supported for now
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = (_ZFP_ZFUITextViewImpl_sys_Qt_TextView *)textView->nativeImplView();
        switch(textTruncateMode) {
            case v_ZFUITextTruncateMode::e_Disable:
                break;
            case v_ZFUITextTruncateMode::e_Head:
                break;
            case v_ZFUITextTruncateMode::e_Middle:
                break;
            case v_ZFUITextTruncateMode::e_Tail:
                break;
            default:
                ZFCoreCriticalShouldNotGoHere();
                return;
        }
#endif
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        QRect geometrySaved = nativeImplView->geometry();
        int textSizeSaved = nativeImplView->font().pixelSize();
        if(textSizeSaved == -1) {
            textSizeSaved = QFontMetrics(nativeImplView->font()).height();
        }
        QSize minimumSizeSaved = nativeImplView->minimumSize();
        QSize maximumSizeSaved = nativeImplView->maximumSize();

        nativeImplView->_ZFP_textSize(textSize);
        nativeImplView->setMinimumSize(0, 0);
        nativeImplView->setMaximumSize(
            (sizeHint.width <= 0) ? (zffloat)30000 : sizeHint.width,
            (sizeHint.height <= 0) ? (zffloat)30000 : sizeHint.height);
        nativeImplView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        QSize ret = nativeImplView->minimumSizeHint();

        nativeImplView->_ZFP_textSize(textSizeSaved);
        nativeImplView->setMinimumSize(minimumSizeSaved);
        nativeImplView->setMaximumSize(maximumSizeSaved);
        nativeImplView->setGeometry(geometrySaved);

        return ZFUISizeCreate(ret.width(), ret.height());
    }

    virtual zffloat textSizeCurrent(ZF_IN ZFUITextView *textView) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        return (zffloat)nativeImplView->font().pixelSize();
    }

    virtual void layoutNativeTextView(
            ZF_IN ZFUITextView *textView
            , ZF_IN const ZFUISize &viewSize
            ) {
        _ZFP_ZFUITextViewImpl_sys_Qt_TextView *nativeImplView = getNativeImplView(textView);
        zffloat fixedTextSize = this->calcTextSizeAuto(textView, viewSize);
        nativeImplView->_ZFP_textSize(fixedTextSize);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUITextView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt


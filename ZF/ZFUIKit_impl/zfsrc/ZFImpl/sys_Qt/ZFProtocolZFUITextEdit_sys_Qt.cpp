#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_Qt

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsProxyWidget>

class _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit : public QLineEdit {
    Q_OBJECT

public:
    ZFUITextEdit *ownerZFUITextEdit;
    zfindex textEditEventOverrideFlag;
    zfbool editSecured;
    QLineEdit::EchoMode textEditEchoModeSaved;
    Qt::InputMethodHints textEditInputMethodHintsSaved;
    zfstring textEditTextSaved;

public:
    _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit(ZF_IN ZFUITextEdit *ownerZFUITextEdit)
    : QLineEdit()
    , ownerZFUITextEdit(ownerZFUITextEdit)
    , textEditEventOverrideFlag(0)
    , editSecured(zffalse)
    , textEditEchoModeSaved(this->echoMode())
    , textEditInputMethodHintsSaved(this->inputMethodHints())
    , textEditTextSaved()
    {
        this->connect(this, SIGNAL(textChanged(QString)), this, SLOT(_ZFP_textOnUpdate(QString)));
        this->connect(this, SIGNAL(cursorPositiOnChanged(int,int)), this, SLOT(_ZFP_selectedRangeOnUpdate(int,int)));
        this->_ZFP_textColor(ZFUIColorBlack());
        this->setFrame(false);
        this->setAttribute(Qt::WA_MacShowFocusRect, false);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, Qt::transparent);
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
        palette.setColor(QPalette::Text, ZFImpl_sys_Qt_ZFUIColorToQColor(v));
        this->setPalette(palette);
    }
    void _ZFP_editSecured(ZF_IN zfbool editSecured) {
        if(this->editSecured == editSecured) {
            return;
        }
        this->editSecured = editSecured;
        if(this->editSecured) {
            this->textEditEchoModeSaved = this->echoMode();
            this->textEditInputMethodHintsSaved = this->inputMethodHints();

            this->setEchoMode(QLineEdit::Password);
            this->setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
        }
        else {
            this->setEchoMode(this->textEditEchoModeSaved);
            this->setInputMethodHints(this->textEditInputMethodHintsSaved);
        }
    }
    void _ZFP_text(const zfstring &text, zfbool needNotify) {
        if(this->textEditTextSaved.compare(text) == 0) {
            return;
        }
        int cursor = this->cursorPosition();
        if(ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldUpdate(this->ownerZFUITextEdit, text)) {
            int positionSaved = this->cursorPosition();
            int textLengthOld = this->text().length();
            this->textEditTextSaved = text;
            this->setText(ZFImpl_sys_Qt_zfstringToQString(text));
            zfindex len = zfslen(text);
            if(len >= textLengthOld) {
                cursor = positionSaved + len - textLengthOld;
            }
            this->setCursorPosition(cursor);
        }
        else {
            this->setText(ZFImpl_sys_Qt_zfstringToQString(this->textEditTextSaved));
            this->setCursorPosition(cursor);
        }
        if(needNotify) {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextUpdate(this->ownerZFUITextEdit, text);
        }
    }

public:
    virtual bool event(QEvent *event) {
        if(this->textEditEventOverrideFlag > 0) {
            return true;
        }
        bool ret = false;
        switch(event->type()) {
            case QEvent::FocusIn:
                ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(this->ownerZFUITextEdit);
                ret = QLineEdit::event(event);
                break;
            case QEvent::FocusOut:
                ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(this->ownerZFUITextEdit);
                ret = QLineEdit::event(event);
                break;
            case QEvent::KeyPress: {
                QKeyEvent *keyEvent = (QKeyEvent *)event;
                if(keyEvent->key() == Qt::Key_Return) {
                    ret = true;
                }
                else {
                    ret = QLineEdit::event(event);
                }
            }
                break;
            case QEvent::KeyRelease: {
                QKeyEvent *keyEvent = (QKeyEvent *)event;
                if(keyEvent->key() == Qt::Key_Return) {
                    ret = true;
                    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(this->ownerZFUITextEdit);
                }
                else {
                    ret = QLineEdit::event(event);
                }
            }
                break;
            default:
                ret = QLineEdit::event(event);
                break;
        }

        return ret;
    }
public slots:
    void _ZFP_textOnUpdate(const QString &text) {
        if(this->textEditEventOverrideFlag > 0) {
            return;
        }
        ++(this->textEditEventOverrideFlag);
        this->_ZFP_text(ZFImpl_sys_Qt_zfstringFromQString(text), zftrue);
        --(this->textEditEventOverrideFlag);
    }
    void _ZFP_selectedRangeOnUpdate(int posOld, int posNew) {
        ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextSelectRangeOnUpdate(this->ownerZFUITextEdit);
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_Qt, ZFUITextEdit, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsProxyWidget:QLineEdit")
public:
    virtual void *nativeTextEditCreate(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT zfbool &nativeImplViewRequireVirtualIndex
            ) {
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
        proxy->setWidget(new _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit(textEdit));
        return proxy;
    }
    virtual void nativeTextEditDestroy(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN void *nativeTextEdit
            ) {
        QGraphicsProxyWidget *proxy = (QGraphicsProxyWidget *)nativeTextEdit;
        delete proxy;
    }
private:
    _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *getNativeImplView(ZF_IN ZFUITextEdit *textEdit) {
        QGraphicsProxyWidget *proxy = (QGraphicsProxyWidget *)textEdit->nativeImplView();
        return qobject_cast<_ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *>(proxy->widget());
    }

// ============================================================
// properties
public:
    virtual void editEnable(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editEnable
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->setReadOnly(!editEnable);
    }
    virtual void textEditSecure(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zfbool editSecured
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->_ZFP_editSecured(editSecured);
    }
    virtual void keyboardType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardType keyboardType
            ) {
        // not supported
    }
    virtual void keyboardReturnType(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextEditKeyboardReturnType keyboardReturnType
            ) {
        // not supported
    }

    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_OUT ZFIndexRange &selectedRange
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        if(nativeImplView->hasSelectedText()) {
            selectedRange.start = nativeImplView->selectionStart();
            selectedRange.count = nativeImplView->selectedText().length();
        }
        else {
            selectedRange.start = nativeImplView->cursorPosition();
            selectedRange.count = 0;
        }
    }
    virtual void selectedRange(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFIndexRange &selectedRange
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        if(selectedRange.count != 0) {
            nativeImplView->setSelection(selectedRange.start, selectedRange.count);
        }
        else {
            nativeImplView->setCursorPosition(selectedRange.start);
        }
    }

public:
    virtual void text(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const zfstring &text
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->_ZFP_text(text, zffalse);
    }
    virtual void textAppearance(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUITextAppearance const &textAppearance
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        QFont font = nativeImplView->font();
        switch(textAppearance) {
            case ZFUITextAppearance::e_Normal:
                font.setBold(false);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Bold:
                font.setBold(true);
                font.setItalic(false);
                break;
            case ZFUITextAppearance::e_Italic:
                font.setBold(false);
                font.setItalic(true);
                break;
            case ZFUITextAppearance::e_BoldItalic:
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
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIAlignFlags const &textAlign
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->setAlignment(ZFImpl_sys_Qt_ZFUIAlignFlagsToQAlignment(textAlign));
    }
    virtual void textColor(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN ZFUIColor const &textColor
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->_ZFP_textColor(textColor);
    }
    virtual void textSize(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->_ZFP_textSize(textSize);
    }

// ============================================================
// layout
public:
    virtual ZFUISize measureNativeTextEdit(
            ZF_IN ZFUITextEdit *textEdit
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN zffloat textSize
            ) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        QFont font = nativeImplView->font();
        font.setPixelSize(textSize);
        QFontMetrics fm(font);
        QRect rect = fm.boundingRect(nativeImplView->text());
        int padding = 8;
        return ZFUISizeCreate(rect.width() + padding, rect.height() + padding);
    }

public:
    virtual void editBegin(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->setFocus();
    }
    virtual void editEnd(ZF_IN ZFUITextEdit *textEdit) {
        _ZFP_ZFUITextEditImpl_sys_Qt_TextEdit *nativeImplView = getNativeImplView(textEdit);
        nativeImplView->clearFocus();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUITextEdit_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt


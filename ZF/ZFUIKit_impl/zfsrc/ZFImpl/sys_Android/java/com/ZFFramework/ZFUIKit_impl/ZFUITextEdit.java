package com.ZFFramework.ZFUIKit_impl;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

import com.ZFFramework.NativeUtil.ZFAndroidLog;
import com.ZFFramework.NativeUtil.ZFAndroidUI;
import com.ZFFramework.ZF_impl.ZFMainEntry;

import java.lang.ref.WeakReference;

@SuppressLint("RtlHardcoded")
public final class ZFUITextEdit extends EditText {
    public static Object native_nativeTextEditCreate(long zfjniPointerOwnerZFUITextEdit) {
        ZFUITextEdit ret = new ZFUITextEdit(ZFMainEntry.appContext());
        ret.zfjniPointerOwnerZFUITextEdit = zfjniPointerOwnerZFUITextEdit;
        return ret;
    }

    public static void native_nativeTextEditDestroy(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.removeTextChangedListener(nativeTextEditTmp._textWatcher);
        nativeTextEditTmp._textWatcher = null;
        nativeTextEditTmp.setText(null);
        nativeTextEditTmp.setOnEditorActionListener(null);
        nativeTextEditTmp.zfjniPointerOwnerZFUITextEdit = 0;
    }

    // ============================================================
    public static void native_editEnable(Object nativeTextEdit, boolean editEnable) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.setEnabled(editEnable);
    }

    public static void native_textEditSecure(Object nativeTextEdit, boolean editSecured) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if (editSecured) {
            nativeTextEditTmp.setTransformationMethod(PasswordTransformationMethod.getInstance());
        } else {
            nativeTextEditTmp.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
        }
    }

    public static void native_keyboardType(Object nativeTextEdit, int keyboardType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if (keyboardType == ZFUITextEditKeyboardType.e_Normal) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        } else if (keyboardType == ZFUITextEditKeyboardType.e_CharBased) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        } else if (keyboardType == ZFUITextEditKeyboardType.e_PhonePad) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_PHONE);
        } else if (keyboardType == ZFUITextEditKeyboardType.e_NumberPad) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_NUMBER);
        } else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }

    public static void native_keyboardReturnType(Object nativeTextEdit, int keyboardReturnType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Normal) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NONE);
        } else if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Next) {
            nativeTextEditTmp.setImeActionLabel(null, 0);
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NEXT);
        } else if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Search) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
        } else if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Done) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_DONE);
        } else if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Go) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_GO);
        } else if (keyboardReturnType == ZFUITextEditKeyboardReturnType.e_Send) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEND);
        } else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }

    public static int native_selectedRange_start(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        return nativeTextEditTmp.getSelectionStart();
    }

    public static int native_selectedRange_count(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        return nativeTextEditTmp.getSelectionEnd() - nativeTextEditTmp.getSelectionStart();
    }

    public static void native_selectedRange(Object nativeTextEdit, int start, int count) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if (count == 0) {
            nativeTextEditTmp.setSelection(start);
        } else {
            nativeTextEditTmp.setSelection(start, start + count);
        }
    }

    // ============================================================
    public static void native_text(Object nativeTextEdit, Object nativeText) {
        ((ZFUITextEdit) nativeTextEdit).setText((String) nativeText);
    }

    public static void native_textAppearance(Object nativeTextEdit, int textAppearance) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if (textAppearance == ZFUITextAppearance.e_Normal) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        } else if (textAppearance == ZFUITextAppearance.e_Bold) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        } else if (textAppearance == ZFUITextAppearance.e_Italic) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.ITALIC));
        } else if (textAppearance == ZFUITextAppearance.e_BoldItalic) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD_ITALIC));
        } else {
            ZFAndroidLog.shouldNotGoHere();
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
    }

    public static void native_textAlign(Object nativeTextEdit, int textAlign) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        if ((textAlign & ZFUIAlign.e_Left) == ZFUIAlign.e_Left) {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        } else if ((textAlign & ZFUIAlign.e_Right) == ZFUIAlign.e_Right) {
            nativeTextEditTmp.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);
        } else if (textAlign == ZFUIAlign.e_Center) {
            nativeTextEditTmp.setGravity(Gravity.CENTER);
        } else {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }

    public static void native_textColor(Object nativeTextEdit, int textColor) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.setTextColor(textColor);
    }

    public static void native_textSize(Object nativeTextEdit, int textSize) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
    }

    private static int[] _native_measureNativeTextEdit_sizeCache = new int[2]; // width, height

    public static int[] native_measureNativeTextEdit(
            Object nativeTextEdit
            , int maxWidthOrNegative
            , int maxHeightOrNegative
            , int textSize
    ) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        float savedTextSize = nativeTextEditTmp.getTextSize();
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);

        int widthOld = nativeTextEditTmp.getMeasuredWidth();
        int heightOld = nativeTextEditTmp.getMeasuredHeight();

        nativeTextEditTmp.measure(
                (maxWidthOrNegative >= 0)
                        ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                        : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
                (maxHeightOrNegative >= 0)
                        ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                        : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));

        int padding = (int) (2 * ZFAndroidUI.screenDensity(nativeTextEditTmp.getContext()));
        _native_measureNativeTextEdit_sizeCache[0] = nativeTextEditTmp.getMeasuredWidth() + padding;
        _native_measureNativeTextEdit_sizeCache[1] = nativeTextEditTmp.getMeasuredHeight() + padding;
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, savedTextSize);

        // measured size must be restored, otherwise text's draw step would cause strange error
        nativeTextEditTmp.setMeasuredDimension(widthOld, heightOld);

        return _native_measureNativeTextEdit_sizeCache;
    }

    // ============================================================
    public static void native_editBegin(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.requestFocus();
    }

    public static void native_editEnd(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit) nativeTextEdit;
        nativeTextEditTmp.clearFocus();
    }

    // ============================================================
    public static native boolean native_notifyCheckTextShouldUpdate(long zfjniPointerOwnerZFUITextEdit, Object newString);

    public static native void native_notifyTextUpdate(long zfjniPointerOwnerZFUITextEdit, Object newString);

    public static native void native_notifyTextSelectRangeOnUpdate(long zfjniPointerOwnerZFUITextEdit);

    public static native void native_notifyTextReturnClicked(long zfjniPointerOwnerZFUITextEdit);

    public static native void native_notifyTextEditBegin(long zfjniPointerOwnerZFUITextEdit);

    public static native void native_notifyTextEditEnd(long zfjniPointerOwnerZFUITextEdit);

    // ============================================================
    private int _ZFP_selectedRangeUpdateFlag = 0;

    @Override
    public void setSelection(int start, int stop) {
        ++_ZFP_selectedRangeUpdateFlag;
        super.setSelection(start, stop);
        --_ZFP_selectedRangeUpdateFlag;
        if (_ZFP_selectedRangeUpdateFlag == 0) {
            ZFUITextEdit.native_notifyTextSelectRangeOnUpdate(this.zfjniPointerOwnerZFUITextEdit);
        }
    }

    @Override
    public void setSelection(int index) {
        ++_ZFP_selectedRangeUpdateFlag;
        super.setSelection(index);
        --_ZFP_selectedRangeUpdateFlag;
        if (_ZFP_selectedRangeUpdateFlag == 0) {
            ZFUITextEdit.native_notifyTextSelectRangeOnUpdate(this.zfjniPointerOwnerZFUITextEdit);
        }
    }

    @Override
    protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        if (gainFocus) {
            ZFUITextEdit.native_notifyTextEditBegin(this.zfjniPointerOwnerZFUITextEdit);
        } else {
            InputMethodManager imm = (InputMethodManager) this.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(this.getWindowToken(), 0);
            ZFUITextEdit.native_notifyTextEditEnd(this.zfjniPointerOwnerZFUITextEdit);
        }
    }

    private static class _TextWatcher implements TextWatcher {
        private WeakReference<ZFUITextEdit> owner = null;

        public _TextWatcher(ZFUITextEdit owner) {
            this.owner = new WeakReference<ZFUITextEdit>(owner);
        }

        private String savedString = null;
        private boolean overrided = false;

        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            this.savedString = s.toString();
        }

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {
        }

        @Override
        public void afterTextChanged(Editable s) {
            if (this.owner.get() == null || this.overrided) {
                return;
            }

            String newString = s.toString();
            if (newString.compareTo(this.savedString) == 0) {
                return;
            }
            if (newString.contains("\n")) {
                this.overrided = true;
                this.owner.get().setText(this.savedString);
                this.overrided = false;
                ZFUITextEdit.native_notifyTextReturnClicked(this.owner.get().zfjniPointerOwnerZFUITextEdit);
                return;
            }
            if (!ZFUITextEdit.native_notifyCheckTextShouldUpdate(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString)) {
                this.overrided = true;
                this.owner.get().setText(this.savedString);
                this.overrided = false;
            } else {
                ZFUITextEdit.native_notifyTextUpdate(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString);
            }
        }
    }

    private long zfjniPointerOwnerZFUITextEdit = 0;
    private TextWatcher _textWatcher = null;

    // ============================================================
    @SuppressWarnings("deprecation")
    public ZFUITextEdit(Context context) {
        super(context);

        this.setTextColor(Color.BLACK);
        this.setBackgroundDrawable(null);
        this.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        this.setPadding(0, 0, 0, 0);
        this.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        this.setImeOptions(EditorInfo.IME_ACTION_NONE);
        this.setLines(1);

        _textWatcher = new _TextWatcher(this);
        this.addTextChangedListener(_textWatcher);

        this.setOnKeyListener(new OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_ENTER) {
                    if (event.getAction() == KeyEvent.ACTION_UP) {
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                    }
                    return true;
                }
                return false;
            }
        });
        this.setOnEditorActionListener(new OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                switch (actionId) {
                    case EditorInfo.IME_ACTION_NONE:
                    case EditorInfo.IME_ACTION_NEXT:
                    case EditorInfo.IME_ACTION_SEARCH:
                    case EditorInfo.IME_ACTION_DONE:
                    case EditorInfo.IME_ACTION_GO:
                    case EditorInfo.IME_ACTION_SEND:
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                        return true;
                    default:
                        break;
                }
                if (event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                    if (event.getAction() == KeyEvent.ACTION_UP) {
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                    }
                    return true;
                }
                return false;
            }
        });
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEvent(event);
        }
        if (event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }

    @Override
    public boolean dispatchKeyEventPreIme(KeyEvent event) {
        if (event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEventPreIme(event);
        }
        if (event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }
}

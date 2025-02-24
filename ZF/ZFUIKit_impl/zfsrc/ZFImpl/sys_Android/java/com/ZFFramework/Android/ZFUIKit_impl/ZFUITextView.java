package com.ZFFramework.Android.ZFUIKit_impl;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.text.TextUtils.TruncateAt;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.TextView;

import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

@SuppressLint("RtlHardcoded")
public final class ZFUITextView extends TextView {
    public static Object native_nativeTextViewCreate() {
        return new ZFUITextView(ZFMainEntry.appContext());
    }

    public static void native_nativeTextViewDestroy(Object nativeTextView) {
        ((ZFUITextView) nativeTextView).setText(null);
    }

    public static void native_text(Object nativeTextView,
                                   Object nativeText) {
        ((ZFUITextView) nativeTextView).setText((String) nativeText);
    }

    public static void native_textAppearance(Object nativeTextView,
                                             int textAppearance) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        if (textAppearance == ZFUITextAppearance.e_Normal) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        } else if (textAppearance == ZFUITextAppearance.e_Bold) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        } else if (textAppearance == ZFUITextAppearance.e_Italic) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.ITALIC));
        } else if (textAppearance == ZFUITextAppearance.e_BoldItalic) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD_ITALIC));
        } else {
            ZFAndroidLog.shouldNotGoHere();
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
    }

    public static void native_textAlign(Object nativeTextView,
                                        int textAlign) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        if ((textAlign & ZFUIAlign.e_Left) == ZFUIAlign.e_Left) {
            nativeTextViewTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        } else if ((textAlign & ZFUIAlign.e_Right) == ZFUIAlign.e_Right) {
            nativeTextViewTmp.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);
        } else if (textAlign == ZFUIAlign.e_Center) {
            nativeTextViewTmp.setGravity(Gravity.CENTER);
        } else {
            nativeTextViewTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }

    public static void native_textColor(Object nativeTextView,
                                        int textColor) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        nativeTextViewTmp.setTextColor(textColor);
    }

    public static void native_singleLine(Object nativeTextView,
                                             boolean singleLine) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        nativeTextViewTmp.setSingleLine(singleLine);
    }

    public static void native_textTruncateMode(Object nativeTextView,
                                               int textTruncateMode) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        if (textTruncateMode == ZFUITextTruncateMode.e_Disable) {
            nativeTextViewTmp.setEllipsize(TruncateAt.END);
        } else if (textTruncateMode == ZFUITextTruncateMode.e_Tail) {
            nativeTextViewTmp.setEllipsize(TruncateAt.END);
        } else if (textTruncateMode == ZFUITextTruncateMode.e_Middle) {
            nativeTextViewTmp.setEllipsize(TruncateAt.MIDDLE);
        } else if (textTruncateMode == ZFUITextTruncateMode.e_Head) {
            nativeTextViewTmp.setEllipsize(TruncateAt.START);
        } else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }

    private static int[] _native_measureNativeTextView_sizeCache = new int[2]; // width, height

    public static int[] native_measureNativeTextView(Object nativeTextView,
                                                     int maxWidthOrNegative,
                                                     int maxHeightOrNegative,
                                                     int textSize) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        int widthOld = nativeTextViewTmp.getMeasuredWidth();
        int heightOld = nativeTextViewTmp.getMeasuredHeight();
        int widthSpec = (maxWidthOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        int heightSpec = (maxHeightOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);

        if (textSize == nativeTextViewTmp.getTextSize()) {
            nativeTextViewTmp.measure(widthSpec, heightSpec);
        } else {
            float savedTextSize = nativeTextViewTmp.getTextSize();
            nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
            nativeTextViewTmp.measure(widthSpec, heightSpec);
            nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, savedTextSize);
        }

        _native_measureNativeTextView_sizeCache[0] = nativeTextViewTmp.getMeasuredWidth();
        _native_measureNativeTextView_sizeCache[1] = nativeTextViewTmp.getMeasuredHeight();

        // measured size must be restored, otherwise text's draw step would cause strange error
        nativeTextViewTmp.setMeasuredDimension(widthOld, heightOld);
        return _native_measureNativeTextView_sizeCache;
    }

    public static int native_textSizeCurrent(Object nativeTextView) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        return (int) (nativeTextViewTmp.getTextSize());
    }

    public static void native_textSizeAuto_update(Object nativeTextView,
                                                  int textSizeCurrent) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView) nativeTextView;
        nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSizeCurrent);
    }

    // ============================================================
    @SuppressWarnings("deprecation")
    public ZFUITextView(Context context) {
        super(context);

        this.setTextColor(Color.BLACK);
        this.setBackgroundDrawable(null);
        this.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        this.setSingleLine(true);
        this.setPadding(0, 0, 0, 0);
    }
}

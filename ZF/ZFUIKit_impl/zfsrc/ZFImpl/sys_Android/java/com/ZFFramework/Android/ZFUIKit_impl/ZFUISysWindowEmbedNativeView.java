package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;
import android.view.ViewGroup;

public final class ZFUISysWindowEmbedNativeView {
    public static void native_nativeViewAdd(Object parent,
                                            Object child) {
        View childTmp = (View)child;
        ((ViewGroup)parent).addView(childTmp, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    }
    public static void native_nativeViewRemove(Object parent,
                                               Object child) {
        View childTmp = (View)child;
        ((ViewGroup)parent).removeView(childTmp);
    }
}

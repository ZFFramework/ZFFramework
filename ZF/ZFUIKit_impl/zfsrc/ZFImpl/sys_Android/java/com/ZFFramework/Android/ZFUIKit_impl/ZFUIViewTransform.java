package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewTransform {
    public static void native_viewTransform(Object nativeView,
                                            float translateX,
                                            float translateY,
                                            float scaleX,
                                            float scaleY,
                                            float rotateZ) {
        View nativeViewTmp = (View) nativeView;
        nativeViewTmp.setTranslationX(translateX);
        nativeViewTmp.setTranslationY(translateY);
        nativeViewTmp.setScaleX(scaleX);
        nativeViewTmp.setScaleY(scaleY);
        nativeViewTmp.setRotation(rotateZ);
    }
}

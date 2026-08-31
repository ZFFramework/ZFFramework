package com.ZFFramework.ZFUIKit_impl;

import android.view.View;

public class ZFUIViewPositionMap {
    private static int[] _viewPositionMapCache = new int[2];

    public static int[] native_viewPositionMap(Object nativeView, int localPosX, int localPosY) {
        View nativeViewTmp = (View) nativeView;
        nativeViewTmp.getLocationOnScreen(_viewPositionMapCache);
        _viewPositionMapCache[0] += localPosX;
        _viewPositionMapCache[1] += localPosY;
        return _viewPositionMapCache;
    }
}

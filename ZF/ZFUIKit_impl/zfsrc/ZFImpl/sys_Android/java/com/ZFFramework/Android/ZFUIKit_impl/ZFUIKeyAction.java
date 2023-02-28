
package com.ZFFramework.Android.ZFUIKit_impl;

import android.view.KeyEvent;

import com.ZFFramework.Android.ZF_impl.ZFEnum;

public class ZFUIKeyAction {
    public static final int e_KeyDown = ZFEnum.eAssert("ZFUIKeyAction", "KeyDown");
    public static final int e_KeyRepeat = ZFEnum.eAssert("ZFUIKeyAction", "KeyRepeat");
    public static final int e_KeyUp = ZFEnum.eAssert("ZFUIKeyAction", "KeyUp");
    public static final int e_KeyCancel = ZFEnum.eAssert("ZFUIKeyAction", "KeyCancel");

    public static int keyActionFromKeyActionRaw(int keyActionRaw) {
        switch (keyActionRaw) {
            case KeyEvent.ACTION_DOWN:
                return ZFUIKeyAction.e_KeyDown;
            case KeyEvent.ACTION_MULTIPLE:
                return ZFUIKeyAction.e_KeyRepeat;
            case KeyEvent.ACTION_UP:
                return ZFUIKeyAction.e_KeyUp;
            default:
                return ZFUIKeyAction.e_KeyCancel;
        }
    }
}

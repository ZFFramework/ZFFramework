
package com.ZFFramework.ZFUIKit_impl;

import android.view.KeyEvent;

import com.ZFFramework.ZF_impl.ZFEnum;

public class ZFUIKeyAction {
    public static final int e_Down = ZFEnum.eAssert("ZFUIKeyAction", "Down");
    public static final int e_Repeat = ZFEnum.eAssert("ZFUIKeyAction", "Repeat");
    public static final int e_Up = ZFEnum.eAssert("ZFUIKeyAction", "Up");
    public static final int e_Cancel = ZFEnum.eAssert("ZFUIKeyAction", "Cancel");

    public static int keyActionFromKeyActionRaw(int keyActionRaw) {
        switch (keyActionRaw) {
            case KeyEvent.ACTION_DOWN:
                return ZFUIKeyAction.e_Down;
            case KeyEvent.ACTION_MULTIPLE:
                return ZFUIKeyAction.e_Repeat;
            case KeyEvent.ACTION_UP:
                return ZFUIKeyAction.e_Up;
            default:
                return ZFUIKeyAction.e_Cancel;
        }
    }
}

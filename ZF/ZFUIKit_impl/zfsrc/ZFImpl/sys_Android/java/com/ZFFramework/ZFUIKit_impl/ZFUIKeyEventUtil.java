package com.ZFFramework.ZFUIKit_impl;

import android.view.KeyEvent;

import java.util.ArrayList;
import java.util.List;

public class ZFUIKeyEventUtil {

    public static interface Impl {
        public boolean onKey(int keyId, int keyAction, int keyCode, int keyCodeRaw);
    }

    private Impl _impl = null;

    public ZFUIKeyEventUtil(Impl impl) {
        _impl = impl;
    }

    public void onKeyCancel() {
        _pressedKeyCancel();
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        _pressedKeyAdd((int) event.getDownTime(), event.getKeyCode());
        return _impl.onKey(
                (int) event.getDownTime(),
                ZFUIKeyAction.keyActionFromKeyActionRaw(event.getAction()),
                ZFUIKeyCode.keyCodeFromKeyCodeRaw(keyCode),
                keyCode
        );
    }

    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
        return _impl.onKey(
                (int) event.getDownTime(),
                ZFUIKeyAction.keyActionFromKeyActionRaw(event.getAction()),
                ZFUIKeyCode.keyCodeFromKeyCodeRaw(keyCode),
                keyCode
        );
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
        _pressedKeyRemove((int) event.getDownTime(), event.getKeyCode());
        return _impl.onKey(
                (int) event.getDownTime(),
                ZFUIKeyAction.keyActionFromKeyActionRaw(event.getAction()),
                ZFUIKeyCode.keyCodeFromKeyCodeRaw(keyCode),
                keyCode
        );
    }

    private final List<Integer> _pressedKeyId = new ArrayList<Integer>();
    private final List<Integer> _pressedKeyCode = new ArrayList<Integer>();

    private void _pressedKeyAdd(int keyId, int keyCode) {
        _pressedKeyId.add(keyId);
        _pressedKeyCode.add(keyCode);
    }

    private void _pressedKeyRemove(int keyId, int keyCode) {
        int index = _pressedKeyId.indexOf(keyId);
        if (index >= 0 && _pressedKeyCode.get(index) == keyCode) {
            _pressedKeyId.remove(index);
            _pressedKeyCode.remove(index);
        }
    }

    private void _pressedKeyCancel() {
        for (int i = 0; i < _pressedKeyId.size(); ++i) {
            _impl.onKey(
                    _pressedKeyId.get(i),
                    ZFUIKeyAction.e_Cancel,
                    ZFUIKeyCode.keyCodeFromKeyCodeRaw(_pressedKeyCode.get(i)),
                    _pressedKeyCode.get(i)
            );
        }
    }

}

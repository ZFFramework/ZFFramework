package com.ZFFramework.ZFUIKit_impl;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.ZFFramework.NativeUtil.ZFAndroidLog;
import com.ZFFramework.NativeUtil.ZFObject;
import com.ZFFramework.ZF_impl.ZFMainEntry;

/*
 * @brief window as a Activity in Android
 *
 * must be started by ZFMainEntry or another ZFUISysWindow\n
 * before use, you must declare this in your AndroidManifest.xml:
 * @code
 *   <activity
 *       android:name="com.ZFFramework.ZFUIKit_impl.ZFUISysWindow"
 *       android:configChanges="orientation|keyboardHidden|screenSize"
 *       android:label="ZFFramework"
 *       android:windowSoftInputMode="adjustResize" >
 *   </activity>
 * @endcode
 */
public final class ZFUISysWindow extends Activity {

    /**
     * @brief util for #ZFUISysWindow::nativeWindowEmbedNativeView and #ZFUISysWindow::mainWindowRegister
     */
    public static void mainWindowRegisterForNativeView(ViewGroup nativeParent) {
        if (nativeParent != null) {
            native_mainWindowRegisterForNativeView(nativeParent);
        }
    }

    /**
     * @brief util for #ZFUISysWindow:nativeWindowEmbedNativeView
     */
    public static void nativeWindowEmbedNativeView(ViewGroup nativeParent, String sysWindowName) {
        if (nativeParent != null && sysWindowName != null && sysWindowName.length() > 0) {
            native_nativeWindowEmbedNativeView(nativeParent, sysWindowName);
        }
    }

    public static boolean nativeWindowNotifyKeyEvent(String sysWindowName, int keyCode, KeyEvent event) {
        long zfjniPointerSysWindow = ZFObject.invoke(sysWindowName != null ? sysWindowName : "ZFUISysWindow.mainWindow");
        if (zfjniPointerSysWindow != 0) {
            return native_notifyKeyEvent(zfjniPointerSysWindow
                    , (int) event.getDownTime()
                    , ZFUIKeyAction.keyActionFromKeyActionRaw(event.getAction())
                    , ZFUIKeyCode.keyCodeFromKeyCodeRaw(keyCode)
                    , keyCode
            );
        } else {
            return false;
        }
    }

    // ============================================================
    private static final String _key_isMainWindow = "isMainWindow";
    private static final String _key_zfjniPointerOwnerZFUISysWindow = "zfjniPointerOwnerZFUISysWindow";
    private boolean _isMainWindow = false;
    private long _zfjniPointerOwnerZFUISysWindow = 0;
    private MainLayout _containerView = null;
    private int _sysWindowOrientation = ZFUIOrientation.e_Top;

    // ============================================================
    public static void native_nativeMainWindowCreate(long zfjniPointerOwnerZFUISysWindow) {
        Intent intent = new Intent(ZFMainEntry.mainEntryActivity(), ZFUISysWindow.class);
        intent.putExtra(_key_isMainWindow, true);
        intent.putExtra(_key_zfjniPointerOwnerZFUISysWindow, zfjniPointerOwnerZFUISysWindow);
        ZFMainEntry.mainEntryActivity().startActivity(intent);
    }

    public static Object native_nativeWindowRootViewOnAdd(Object nativeWindow,
                                                          Object nativeWindowRootView) {
        ZFUISysWindow nativeWindowTmp = (ZFUISysWindow) nativeWindow;
        nativeWindowTmp._containerView.addView((View) nativeWindowRootView);
        return nativeWindowTmp._containerView;
    }

    public static void native_nativeWindowRootViewOnRemove(Object nativeWindow,
                                                           Object nativeWindowRootView) {
        ((ZFUISysWindow) nativeWindow)._containerView.removeView((View) nativeWindowRootView);
    }

    public static void native_modalWindowShow(Object nativeWindow,
                                              long zfjniPointerOwnerZFUISysWindow) {
        Intent intent = new Intent((ZFUISysWindow) nativeWindow, ZFUISysWindow.class);
        intent.putExtra(_key_zfjniPointerOwnerZFUISysWindow, zfjniPointerOwnerZFUISysWindow);
        ((ZFUISysWindow) nativeWindow).startActivity(intent);
    }

    public static void native_modalWindowFinish(Object nativeWindow) {
        ((ZFUISysWindow) nativeWindow).finish();
    }

    public static void native_sysWindowLayoutParamOnUpdate(Object nativeWindow) {
        ZFUISysWindow nativeWindowTmp = (ZFUISysWindow) nativeWindow;
        nativeWindowTmp._containerView.requestLayout();
    }

    public static int native_sysWindowOrientation(Object nativeWindow) {
        if (nativeWindow == null) {
            int nativeOrientation = ZFMainEntry.appContext().getResources().getConfiguration().orientation;
            if (nativeOrientation == Configuration.ORIENTATION_LANDSCAPE) {
                return ZFUIOrientation.e_Left;
            } else {
                return ZFUIOrientation.e_Top;
            }
        }
        return ((ZFUISysWindow) nativeWindow)._sysWindowOrientation;
    }

    public static void native_sysWindowOrientationFlags(Object nativeWindow,
                                                        int sysWindowOrientationFlags) {
        ZFUISysWindow nativeWindowTmp = (ZFUISysWindow) nativeWindow;

        boolean left = ((sysWindowOrientationFlags & ZFUIOrientation.e_Left) != 0);
        boolean top = ((sysWindowOrientationFlags & ZFUIOrientation.e_Top) != 0);
        boolean right = ((sysWindowOrientationFlags & ZFUIOrientation.e_Right) != 0);
        boolean bottom = ((sysWindowOrientationFlags & ZFUIOrientation.e_Bottom) != 0);

        int count = 0;
        if (left) {
            ++count;
        }
        if (top) {
            ++count;
        }
        if (right) {
            ++count;
        }
        if (bottom) {
            ++count;
        }
        if (count > 1) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
        } else if (top) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else if (left) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
        } else if (right) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (bottom) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
        }
    }

    // ============================================================
    private static native void native_mainWindowRegisterForNativeView(Object nativeParent);

    private static native void native_nativeWindowEmbedNativeView(Object nativeParent,
                                                                  Object sysWindowName);

    private static native void native_notifyMeasureWindow(long zfjniPointerOwnerZFUISysWindow,
                                                          int refWidth,
                                                          int refHeight,
                                                          int[] resultRect);

    private static native void native_notifyOnCreate(long zfjniPointerOwnerZFUISysWindow,
                                                     Object nativeWindow);

    private static native void native_notifyOnDestroy(long zfjniPointerOwnerZFUISysWindow);

    private static native void native_notifyOnResume(long zfjniPointerOwnerZFUISysWindow);

    private static native void native_notifyOnPause(long zfjniPointerOwnerZFUISysWindow);

    private static native void native_notifyOnRotate(long zfjniPointerOwnerZFUISysWindow);

    private static native boolean native_notifyKeyEvent(long zfjniPointerOwnerZFUISysWindow,
                                                        int keyId,
                                                        int keyAction,
                                                        int keyCode,
                                                        int keyCodeRaw);

    // ============================================================
    private static class MainLayout extends FrameLayout {
        public ZFUISysWindow _owner = null;
        public int _left = 0;
        public int _top = 0;
        public int _right = 0;
        public int _bottom = 0;

        public MainLayout(ZFUISysWindow owner) {
            super(owner);
            _owner = owner;
            this.setBackgroundColor(Color.WHITE);
        }

        private static int[] _rectCache = new int[4];

        @Override
        protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
            ZFUIOnScreenKeyboardState.keyboardFrameUpdate(((Activity) getContext()).getWindow(), _rectCache);
            int keyboardHeight = _rectCache[3];

            int width = MeasureSpec.getSize(widthMeasureSpec);
            int height = MeasureSpec.getSize(heightMeasureSpec);
            this.setMeasuredDimension(width, height);
            height += keyboardHeight;

            if (_owner != null && _owner._zfjniPointerOwnerZFUISysWindow != 0) {
                ZFUISysWindow.native_notifyMeasureWindow(_owner._zfjniPointerOwnerZFUISysWindow, width, height, _rectCache);
                _left = _rectCache[0];
                _top = _rectCache[1];
                _right = _rectCache[0] + _rectCache[2];
                _bottom = _rectCache[1] + _rectCache[3];
            }

            int childWidthSpec = MeasureSpec.makeMeasureSpec(_right - _left, MeasureSpec.EXACTLY);
            int childHeightSpec = MeasureSpec.makeMeasureSpec(_bottom - _top, MeasureSpec.EXACTLY);
            for (int i = 0; i < this.getChildCount(); ++i) {
                this.getChildAt(i).measure(childWidthSpec, childHeightSpec);
            }
        }

        @Override
        protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
            for (int i = 0; i < this.getChildCount(); ++i) {
                this.getChildAt(i).layout(this._left, this._top, this._right, this._bottom);
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        Bundle params = this.getIntent().getExtras();
        if (params == null) {
            ZFAndroidLog.shouldNotGoHere();
        }
        _isMainWindow = params.getBoolean(_key_isMainWindow, false);
        _zfjniPointerOwnerZFUISysWindow = params.getLong(_key_zfjniPointerOwnerZFUISysWindow);
        if (_zfjniPointerOwnerZFUISysWindow == 0) {
            ZFAndroidLog.shouldNotGoHere();
        }

        // replace main entry
        if (_isMainWindow && (ZFMainEntry.mainEntryActivity() instanceof ZFMainEntry)) {
            ZFMainEntry.mainEntryActivity().finish();
            ZFMainEntry.mainEntryActivity(this);
        }

        _containerView = new MainLayout(this);
        ZFUISysWindow.native_notifyOnCreate(_zfjniPointerOwnerZFUISysWindow, this);
        this.setContentView(_containerView);
    }

    @Override
    protected void onDestroy() {
        boolean needDestroyMainEntry = this._isMainWindow;

        ZFUISysWindow.native_notifyOnDestroy(_zfjniPointerOwnerZFUISysWindow);

        _containerView._owner = null;
        _containerView = null;

        if (needDestroyMainEntry && ZFMainEntry.mainEntryActivity() == this) {
            ZFMainEntry.ZFFrameworkCleanup();
            ZFMainEntry.mainEntryActivity(null);
        }

        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ZFUISysWindow.native_notifyOnResume(_zfjniPointerOwnerZFUISysWindow);
    }

    @Override
    protected void onPause() {
        _keyEventImpl.onKeyCancel();
        ZFUISysWindow.native_notifyOnPause(_zfjniPointerOwnerZFUISysWindow);
        super.onPause();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        int sysWindowOrientationOld = _sysWindowOrientation;
        switch (((WindowManager) getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getOrientation()) {
            case Surface.ROTATION_0:
                _sysWindowOrientation = ZFUIOrientation.e_Top;
                break;
            case Surface.ROTATION_90:
                _sysWindowOrientation = ZFUIOrientation.e_Right;
                break;
            case Surface.ROTATION_180:
                _sysWindowOrientation = ZFUIOrientation.e_Bottom;
                break;
            case Surface.ROTATION_270:
                _sysWindowOrientation = ZFUIOrientation.e_Left;
                break;
            default:
                _sysWindowOrientation = ZFUIOrientation.e_Top;
                break;
        }

        if (sysWindowOrientationOld != _sysWindowOrientation) {
            ZFUISysWindow.native_notifyOnRotate(_zfjniPointerOwnerZFUISysWindow);
        }
    }

    private final ZFUIKeyEventUtil _keyEventImpl = new ZFUIKeyEventUtil(new ZFUIKeyEventUtil.Impl() {
        @Override
        public boolean onKey(int keyId, int keyAction, int keyCode, int keyCodeRaw) {
            if (_zfjniPointerOwnerZFUISysWindow != 0) {
                return native_notifyKeyEvent(_zfjniPointerOwnerZFUISysWindow, keyId, keyAction, keyCode, keyCodeRaw);
            }
            return false;
        }
    });

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyDown(keyCode, event)
                || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
        return _keyEventImpl.onKeyMultiple(keyCode, repeatCount, event)
                || super.onKeyMultiple(keyCode, repeatCount, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyUp(keyCode, event)
                || super.onKeyUp(keyCode, event);
    }

}

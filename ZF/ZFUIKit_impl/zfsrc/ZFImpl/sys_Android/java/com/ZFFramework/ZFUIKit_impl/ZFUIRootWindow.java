package com.ZFFramework.ZFUIKit_impl;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.DisplayCutout;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.ZFFramework.NativeUtil.ZFAndroidLog;
import com.ZFFramework.NativeUtil.ZFObject;
import com.ZFFramework.ZF_impl.ZFMainEntry;

/*
 * @brief window as a Activity in Android
 *
 * must be started by ZFMainEntry or another ZFUIRootWindow\n
 * before use, you must declare this in your AndroidManifest.xml:
 * @code
 *   <activity
 *       android:name="com.ZFFramework.ZFUIKit_impl.ZFUIRootWindow"
 *       android:configChanges="orientation|keyboardHidden|screenSize"
 *       android:label="ZFFramework"
 *       android:windowSoftInputMode="adjustResize" >
 *   </activity>
 * @endcode
 */
public final class ZFUIRootWindow extends Activity {

    /**
     * @brief util for #ZFUIRootWindow::nativeWindowEmbedNativeView and #ZFUIRootWindow::mainWindowRegister
     */
    public static void mainWindowRegisterForNativeView(ViewGroup nativeParent) {
        if (nativeParent != null) {
            native_mainWindowRegisterForNativeView(nativeParent);
        }
    }

    /**
     * @brief util for #ZFUIRootWindow:nativeWindowEmbedNativeView
     */
    public static void nativeWindowEmbedNativeView(ViewGroup nativeParent, String rootWindowName) {
        if (nativeParent != null && rootWindowName != null && rootWindowName.length() > 0) {
            native_nativeWindowEmbedNativeView(nativeParent, rootWindowName);
        }
    }

    public static boolean nativeWindowNotifyKeyEvent(String rootWindowName, int keyCode, KeyEvent event) {
        long zfjniPointerRootWindow = ZFObject.invoke(rootWindowName != null ? rootWindowName : "ZFUIRootWindow.mainWindow");
        if (zfjniPointerRootWindow != 0) {
            return native_notifyKeyEvent(zfjniPointerRootWindow, (int) event.getDownTime(), ZFUIKeyAction.keyActionFromKeyActionRaw(event.getAction()), ZFUIKeyCode.keyCodeFromKeyCodeRaw(keyCode), keyCode);
        } else {
            return false;
        }
    }

    // ============================================================
    public FrameLayout rootContainer() {
        return _rootContainer;
    }

    public FrameLayout mainContainer() {
        return _mainContainer;
    }

    // ============================================================
    private static final String _key_isMainWindow = "isMainWindow";
    private static final String _key_zfjniPointerOwnerZFUIRootWindow = "zfjniPointerOwnerZFUIRootWindow";
    private boolean _isMainWindow = false;
    private long _zfjniPointerOwnerZFUIRootWindow = 0;
    private FrameLayout _rootContainer = null;
    private MainLayout _mainContainer = null;
    private int _windowOrientation = ZFUIOrientation.e_Top;
    private boolean _preferFullscreen = false;

    // ============================================================
    public static void native_nativeMainWindowCreate(long zfjniPointerOwnerZFUIRootWindow) {
        Intent intent = new Intent(ZFMainEntry.mainEntryActivity(), ZFUIRootWindow.class);
        intent.putExtra(_key_isMainWindow, true);
        intent.putExtra(_key_zfjniPointerOwnerZFUIRootWindow, zfjniPointerOwnerZFUIRootWindow);
        ZFMainEntry.mainEntryActivity().startActivity(intent);
    }

    public static Object native_nativeWindowRootViewOnAdd(Object nativeWindow, Object nativeWindowRootView) {
        ZFUIRootWindow nativeWindowTmp = (ZFUIRootWindow) nativeWindow;
        nativeWindowTmp._mainContainer.addView((View) nativeWindowRootView);
        return nativeWindowTmp._mainContainer;
    }

    public static void native_nativeWindowRootViewOnRemove(Object nativeWindow, Object nativeWindowRootView) {
        ((ZFUIRootWindow) nativeWindow)._mainContainer.removeView((View) nativeWindowRootView);
    }

    public static void native_modalWindowShow(Object nativeWindow, long zfjniPointerOwnerZFUIRootWindow) {
        Intent intent = new Intent((ZFUIRootWindow) nativeWindow, ZFUIRootWindow.class);
        intent.putExtra(_key_zfjniPointerOwnerZFUIRootWindow, zfjniPointerOwnerZFUIRootWindow);
        ((ZFUIRootWindow) nativeWindow).startActivity(intent);
    }

    public static void native_modalWindowHide(Object nativeWindow) {
        ((ZFUIRootWindow) nativeWindow).finish();
    }

    public static void native_layoutParamOnUpdate(Object nativeWindow, boolean preferFullscreen) {
        ZFUIRootWindow nativeWindowTmp = (ZFUIRootWindow) nativeWindow;
        nativeWindowTmp._preferFullscreen = preferFullscreen;
        nativeWindowTmp._mainContainer.requestLayout();

        nativeWindowTmp.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, preferFullscreen ? WindowManager.LayoutParams.FLAG_FULLSCREEN : 0);
        {
            int uiFlags = nativeWindowTmp.getWindow().getDecorView().getSystemUiVisibility();
            if (preferFullscreen) {
//                uiFlags |= View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
//                uiFlags |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
                uiFlags |= View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
                uiFlags |= View.SYSTEM_UI_FLAG_FULLSCREEN;
                uiFlags |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            } else {
//                uiFlags &= ~View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
//                uiFlags &= ~View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
                uiFlags &= ~View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
                uiFlags &= ~View.SYSTEM_UI_FLAG_FULLSCREEN;
                uiFlags &= ~View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            }
            nativeWindowTmp.getWindow().getDecorView().setSystemUiVisibility(uiFlags);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowManager.LayoutParams lp = nativeWindowTmp.getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = preferFullscreen
                    ? WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS
                    : WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_DEFAULT
            ;
            nativeWindowTmp.getWindow().setAttributes(lp);
        }
    }

    public static int native_windowOrientation(Object nativeWindow) {
        if (nativeWindow == null) {
            int nativeOrientation = ZFMainEntry.appContext().getResources().getConfiguration().orientation;
            if (nativeOrientation == Configuration.ORIENTATION_LANDSCAPE) {
                return ZFUIOrientation.e_Left;
            } else {
                return ZFUIOrientation.e_Top;
            }
        }
        return ((ZFUIRootWindow) nativeWindow)._windowOrientation;
    }

    public static void native_windowOrientationFlags(Object nativeWindow, int windowOrientationFlags) {
        ZFUIRootWindow nativeWindowTmp = (ZFUIRootWindow) nativeWindow;

        boolean left = ((windowOrientationFlags & ZFUIOrientation.e_Left) != 0);
        boolean top = ((windowOrientationFlags & ZFUIOrientation.e_Top) != 0);
        boolean right = ((windowOrientationFlags & ZFUIOrientation.e_Right) != 0);
        boolean bottom = ((windowOrientationFlags & ZFUIOrientation.e_Bottom) != 0);

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

    public static void native_windowColor(Object nativeWindow, int windowColor) {
        ZFUIRootWindow nativeWindowTmp = (ZFUIRootWindow) nativeWindow;
        if (Color.alpha(windowColor) == 0) {
            windowColor = Color.BLACK;
        }
        nativeWindowTmp.rootContainer().setBackgroundColor(windowColor);
        boolean isDarkBg = (Color.red(windowColor) < 32 && Color.green(windowColor) < 32 && Color.blue(windowColor) < 32);

        {
            nativeWindowTmp.getWindow().setStatusBarColor(windowColor);
            int uiFlags = nativeWindowTmp.getWindow().getDecorView().getSystemUiVisibility();
            if (!isDarkBg) {
                uiFlags |= View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
            } else {
                uiFlags &= ~View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
            }
            nativeWindowTmp.getWindow().getDecorView().setSystemUiVisibility(uiFlags);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            nativeWindowTmp.getWindow().setNavigationBarColor(windowColor);
            int uiFlags = nativeWindowTmp.getWindow().getDecorView().getSystemUiVisibility();
            if (!isDarkBg) {
                uiFlags |= View.SYSTEM_UI_FLAG_LIGHT_NAVIGATION_BAR;
            } else {
                uiFlags &= ~View.SYSTEM_UI_FLAG_LIGHT_NAVIGATION_BAR;
            }
            nativeWindowTmp.getWindow().getDecorView().setSystemUiVisibility(uiFlags);
        }
    }

    // ============================================================
    public static native void native_mainWindowRegisterForNativeView(Object nativeParent);

    public static native void native_nativeWindowEmbedNativeView(Object nativeParent, Object rootWindowName);

    public static native void native_notifyMeasureWindow(long zfjniPointerOwnerZFUIRootWindow, int refWidth, int refHeight, int marginLeft, int marginTop, int marginRight, int marginBottom, int[] resultRect);

    public static native void native_notifyOnCreate(long zfjniPointerOwnerZFUIRootWindow, Object nativeWindow);

    public static native void native_notifyOnDestroy(long zfjniPointerOwnerZFUIRootWindow);

    public static native void native_notifyOnResume(long zfjniPointerOwnerZFUIRootWindow);

    public static native void native_notifyOnPause(long zfjniPointerOwnerZFUIRootWindow);

    public static native void native_notifyOnRotate(long zfjniPointerOwnerZFUIRootWindow);

    public static native boolean native_notifyKeyEvent(long zfjniPointerOwnerZFUIRootWindow, int keyId, int keyAction, int keyCode, int keyCodeRaw);

    // ============================================================
    private static class MainLayout extends FrameLayout {
        public ZFUIRootWindow _owner = null;
        public int _left = 0;
        public int _top = 0;
        public int _right = 0;
        public int _bottom = 0;

        public MainLayout(ZFUIRootWindow owner) {
            super(owner);
            _owner = owner;
        }

        private static final int[] _rectCache = new int[4];

        @Override
        protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
            ZFUIOnScreenKeyboardState.keyboardFrameUpdate(((Activity) getContext()).getWindow(), _rectCache);
            int keyboardHeight = _rectCache[3];

            int width = MeasureSpec.getSize(widthMeasureSpec);
            int height = MeasureSpec.getSize(heightMeasureSpec);
            this.setMeasuredDimension(width, height);
            height += keyboardHeight;

            if (_owner != null && _owner._zfjniPointerOwnerZFUIRootWindow != 0) {
                int safeAreaLeft = 0;
                int safeAreaTop = 0;
                int safeAreaRight = 0;
                int safeAreaBottom = 0;
                if (_owner._preferFullscreen) {
                    WindowInsets insets = _owner.getWindow().getDecorView().getRootWindowInsets();
                    if (insets != null) {
                        DisplayCutout cutout = null;
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                            cutout = insets.getDisplayCutout();
                            if (cutout != null) {
                                safeAreaLeft = cutout.getSafeInsetLeft();
                                safeAreaTop = cutout.getSafeInsetTop();
                                safeAreaRight = cutout.getSafeInsetRight();
                                safeAreaBottom = cutout.getSafeInsetBottom();
                            }
                        }
                    }
                }
                ZFUIRootWindow.native_notifyMeasureWindow(_owner._zfjniPointerOwnerZFUIRootWindow, width, height, safeAreaLeft, safeAreaTop, safeAreaRight, safeAreaBottom, _rectCache);
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
        _zfjniPointerOwnerZFUIRootWindow = params.getLong(_key_zfjniPointerOwnerZFUIRootWindow);
        if (_zfjniPointerOwnerZFUIRootWindow == 0) {
            ZFAndroidLog.shouldNotGoHere();
        }

        // replace main entry
        if (_isMainWindow && (ZFMainEntry.mainEntryActivity() instanceof ZFMainEntry)) {
            ZFMainEntry.mainEntryActivity().finish();
            ZFMainEntry.mainEntryActivity(this);
        }

        _rootContainer = new FrameLayout(this);
        this.setContentView(_rootContainer);

        _mainContainer = new MainLayout(this);
        _rootContainer.addView(_mainContainer, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        ZFUIRootWindow.native_notifyOnCreate(_zfjniPointerOwnerZFUIRootWindow, this);
    }

    @Override
    protected void onDestroy() {
        boolean needDestroyMainEntry = this._isMainWindow;

        ZFUIRootWindow.native_notifyOnDestroy(_zfjniPointerOwnerZFUIRootWindow);

        _rootContainer = null;
        _mainContainer._owner = null;
        _mainContainer = null;

        if (needDestroyMainEntry && ZFMainEntry.mainEntryActivity() == this) {
            ZFMainEntry.ZFFrameworkCleanup();
            ZFMainEntry.mainEntryActivity(null);
        }

        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ZFUIRootWindow.native_notifyOnResume(_zfjniPointerOwnerZFUIRootWindow);
    }

    @Override
    protected void onPause() {
        _keyEventImpl.onKeyCancel();
        ZFUIRootWindow.native_notifyOnPause(_zfjniPointerOwnerZFUIRootWindow);
        super.onPause();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        int windowOrientationOld = _windowOrientation;
        switch (((WindowManager) getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getOrientation()) {
            case Surface.ROTATION_0:
                _windowOrientation = ZFUIOrientation.e_Top;
                break;
            case Surface.ROTATION_90:
                _windowOrientation = ZFUIOrientation.e_Right;
                break;
            case Surface.ROTATION_180:
                _windowOrientation = ZFUIOrientation.e_Bottom;
                break;
            case Surface.ROTATION_270:
                _windowOrientation = ZFUIOrientation.e_Left;
                break;
            default:
                _windowOrientation = ZFUIOrientation.e_Top;
                break;
        }

        if (windowOrientationOld != _windowOrientation) {
            ZFUIRootWindow.native_notifyOnRotate(_zfjniPointerOwnerZFUIRootWindow);
        }
    }

    private final ZFUIKeyEventUtil _keyEventImpl = new ZFUIKeyEventUtil(new ZFUIKeyEventUtil.Impl() {
        @Override
        public boolean onKey(int keyId, int keyAction, int keyCode, int keyCodeRaw) {
            if (_zfjniPointerOwnerZFUIRootWindow != 0) {
                return native_notifyKeyEvent(_zfjniPointerOwnerZFUIRootWindow, keyId, keyAction, keyCode, keyCodeRaw);
            }
            return false;
        }
    });

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyDown(keyCode, event) || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
        return _keyEventImpl.onKeyMultiple(keyCode, repeatCount, event) || super.onKeyMultiple(keyCode, repeatCount, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyUp(keyCode, event) || super.onKeyUp(keyCode, event);
    }

}

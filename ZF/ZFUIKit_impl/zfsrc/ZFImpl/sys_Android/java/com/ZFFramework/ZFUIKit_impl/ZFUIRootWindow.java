package com.ZFFramework.ZFUIKit_impl;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Insets;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.DisplayCutout;
import android.view.KeyEvent;
import android.view.PixelCopy;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.ZFFramework.NativeUtil.ZFAndroidLog;
import com.ZFFramework.NativeUtil.ZFAndroidPost;
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
    private _RootContainer _rootContainer = null;
    private MainContainer _mainContainer = null;
    private int _windowOrientation = ZFUIOrientation.e_Top;
    private int _windowColorTopDetected = 0;
    private int _windowColorBottomDetected = 0;
    private int _safeAreaLeft = 0;
    private int _safeAreaTop = 0;
    private int _safeAreaRight = 0;
    private int _safeAreaBottom = 0;

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
        nativeWindowTmp._mainContainer.requestLayout();

        nativeWindowTmp.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, preferFullscreen ? WindowManager.LayoutParams.FLAG_FULLSCREEN : 0);
        nativeWindowTmp.getWindow().addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        nativeWindowTmp.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
        {
            int uiFlags = nativeWindowTmp.getWindow().getDecorView().getSystemUiVisibility();
            uiFlags |= View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
            uiFlags |= View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
            uiFlags |= View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
            if (preferFullscreen) {
                uiFlags |= View.SYSTEM_UI_FLAG_FULLSCREEN;
                uiFlags |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
                uiFlags |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            } else {
                uiFlags &= ~View.SYSTEM_UI_FLAG_FULLSCREEN;
                uiFlags &= ~View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
                uiFlags &= ~View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            }
            nativeWindowTmp.getWindow().getDecorView().setSystemUiVisibility(uiFlags);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowManager.LayoutParams lp = nativeWindowTmp.getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS;
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
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
        } else if (top) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else if (left) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
        } else if (right) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (bottom) {
            nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
        } else {
            DisplayMetrics metrics = ZFMainEntry.appContext().getResources().getDisplayMetrics();
            float widthInches = metrics.widthPixels / metrics.xdpi;
            float heightInches = metrics.heightPixels / metrics.ydpi;
            float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);
            if (diagonal >= 7.0) {
                nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
            } else {
                nativeWindowTmp.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            }
        }
    }

    public static void native_windowColor(Object nativeWindow, int windowColor) {
        ZFUIRootWindow nativeWindowTmp = (ZFUIRootWindow) nativeWindow;
        nativeWindowTmp._rootContainer.setBackgroundColor(Color.alpha(windowColor) != 0 ? windowColor : Color.BLACK);
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
    private static class MainContainer extends FrameLayout {
        public ZFUIRootWindow _owner = null;
        public int _left = 0;
        public int _top = 0;
        public int _right = 0;
        public int _bottom = 0;

        public MainContainer(ZFUIRootWindow owner) {
            super(owner);
            _owner = owner;
            setOnApplyWindowInsetsListener(new OnApplyWindowInsetsListener() {
                @Override
                public WindowInsets onApplyWindowInsets(View v, WindowInsets insets) {
                    if (_owner == null) {
                        return insets;
                    }
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                        Insets tmp = insets.getInsets(WindowInsets.Type.systemBars());
                        _owner._safeAreaLeft = tmp.left;
                        _owner._safeAreaTop = tmp.top;
                        _owner._safeAreaRight = tmp.right;
                        _owner._safeAreaBottom = tmp.bottom;
                    } else {
                        _owner._safeAreaLeft = insets.getSystemWindowInsetLeft();
                        _owner._safeAreaTop = insets.getSystemWindowInsetTop();
                        _owner._safeAreaRight = insets.getSystemWindowInsetRight();
                        _owner._safeAreaBottom = insets.getSystemWindowInsetBottom();
                    }
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                        WindowInsets tmp = _owner.getWindow().getDecorView().getRootWindowInsets();
                        if (tmp != null) {
                            DisplayCutout cutout = tmp.getDisplayCutout();
                            if (cutout != null) {
                                _owner._safeAreaLeft = Math.max(_owner._safeAreaLeft, cutout.getSafeInsetLeft());
                                _owner._safeAreaTop = Math.max(_owner._safeAreaTop, cutout.getSafeInsetTop());
                                _owner._safeAreaRight = Math.max(_owner._safeAreaRight, cutout.getSafeInsetRight());
                                _owner._safeAreaBottom = Math.max(_owner._safeAreaBottom, cutout.getSafeInsetBottom());
                            }
                        }
                    }
                    requestLayout();
                    return insets;
                }
            });
            setFitsSystemWindows(true);
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
                ZFUIRootWindow.native_notifyMeasureWindow(
                        _owner._zfjniPointerOwnerZFUIRootWindow
                        , width, height
                        , _owner._safeAreaLeft, _owner._safeAreaTop, _owner._safeAreaRight, _owner._safeAreaBottom
                        , _rectCache
                );
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

        _rootContainer = new _RootContainer(this);
        this.setContentView(_rootContainer);

        _mainContainer = new MainContainer(this);
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

    private int _backToExitTaskId = -1;
    private final ZFUIKeyEventUtil _keyEventImpl = new ZFUIKeyEventUtil(new ZFUIKeyEventUtil.Impl() {
        @Override
        public boolean onKey(int keyId, int keyAction, int keyCode, int keyCodeRaw) {
            if (_zfjniPointerOwnerZFUIRootWindow != 0) {
                if (native_notifyKeyEvent(_zfjniPointerOwnerZFUIRootWindow, keyId, keyAction, keyCode, keyCodeRaw)) {
                    return true;
                }
            }

            if (keyCode == ZFUIKeyCode.e_kPhoneBack) {
                if (_backToExitTaskId != -1) {
                    if (keyAction == ZFUIKeyAction.e_Up) {
                        ZFAndroidPost.cancel(_backToExitTaskId);
                        _backToExitTaskId = -1;
                        finish();
                    }
                } else {
                    if (keyAction == ZFUIKeyAction.e_Up) {
                        _backToExitTaskId = ZFAndroidPost.run(new Runnable() {
                            @Override
                            public void run() {
                                _backToExitTaskId = -1;
                            }
                        }, 2000);
                    }
                }
                return true;
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

    // ============================================================
    private class _RootContainer extends FrameLayout {
        public _RootContainer(Context context) {
            super(context);
            this.setBackgroundColor(Color.BLACK);
        }

        private final Paint _paintCacheTop = new Paint();
        private final Paint _paintCacheBottom = new Paint();

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            if (_drawOverride == 0) {
                _paintCacheTop.setColor(_windowColorTopDetected);
                canvas.drawRect(0, 0, getWidth(), _safeAreaTop, _paintCacheTop);

                _paintCacheBottom.setColor(_windowColorBottomDetected);
                canvas.drawRect(0, getHeight() - _safeAreaBottom, getWidth(), getHeight(), _paintCacheBottom);
            }
        }

        private final ViewTreeObserver.OnDrawListener _onDrawListener = new ViewTreeObserver.OnDrawListener() {
            @Override
            public void onDraw() {
                if (_drawOverride == 0 && _windowColorUpdateTaskId == -1) {
                    _windowColorUpdateTaskId = ZFAndroidPost.run(new Runnable() {
                        @Override
                        public void run() {
                            _windowColorUpdateTaskId = -1;
                            _windowColorUpdateAction();
                            _windowColorTopChecker.check(getWindow());
                            _windowColorBottomChecker.check(getWindow());
                        }
                    });
                }
            }
        };

        @Override
        protected void onAttachedToWindow() {
            super.onAttachedToWindow();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                getViewTreeObserver().addOnDrawListener(_onDrawListener);
            }
        }

        @Override
        protected void onDetachedFromWindow() {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                getViewTreeObserver().removeOnDrawListener(_onDrawListener);
            }
            super.onDetachedFromWindow();
        }

        private int _windowColorUpdateTaskId = -1;
        private final Bitmap _bmp = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
        private final Canvas _canvas = new Canvas(_bmp);
        private int _drawOverride = 0;

        private void _windowColorUpdateAction() {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q) {
                return;
            }
            boolean changed = false;

            _bmp.eraseColor(Color.TRANSPARENT);
            _canvas.save();
            ++_drawOverride;
            _canvas.translate(-getWidth() / 2, -_safeAreaTop);
            this.draw(_canvas);
            --_drawOverride;
            _canvas.restore();
            int windowColor = _bmp.getColor(0, 0).toArgb();
            if (_windowColorTopDetected != windowColor) {
                _windowColorTopDetected = windowColor;
                changed = true;
            }

            _bmp.eraseColor(Color.TRANSPARENT);
            _canvas.save();
            _canvas.translate(0, -(getHeight() - _bmp.getHeight() - _safeAreaBottom));
            ++_drawOverride;
            this.draw(_canvas);
            --_drawOverride;
            _canvas.restore();
            windowColor = _bmp.getColor(0, 0).toArgb();
            if (_windowColorBottomDetected != windowColor) {
                _windowColorBottomDetected = windowColor;
                changed = true;
            }

            if (changed) {
                invalidate();
            }
        }

        private final _WindowColorChecker _windowColorTopChecker = new _WindowColorChecker(new _WindowColorChecker.Callback() {
            @Override
            public void onResult(int color) {
                boolean isDarkBg = _isDarkBg(color);
                getWindow().setStatusBarColor(Color.TRANSPARENT);
                int uiFlags = getWindow().getDecorView().getSystemUiVisibility();
                if (!isDarkBg) {
                    uiFlags |= View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
                } else {
                    uiFlags &= ~View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
                }
                getWindow().getDecorView().setSystemUiVisibility(uiFlags);
            }
        }, true);
        private final _WindowColorChecker _windowColorBottomChecker = new _WindowColorChecker(new _WindowColorChecker.Callback() {
            @Override
            public void onResult(int color) {
                if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
                    return;
                }
                boolean isDarkBg = _isDarkBg(color);
                getWindow().setNavigationBarColor(Color.argb(1, 0, 0, 0));
                int uiFlags = getWindow().getDecorView().getSystemUiVisibility();
                if (!isDarkBg) {
                    uiFlags |= View.SYSTEM_UI_FLAG_LIGHT_NAVIGATION_BAR;
                } else {
                    uiFlags &= ~View.SYSTEM_UI_FLAG_LIGHT_NAVIGATION_BAR;
                }
                getWindow().getDecorView().setSystemUiVisibility(uiFlags);
            }
        }, false);

        private boolean _isDarkBg(int color) {
//            return ColorUtils.calculateLuminance(color) <= 0.5;
            return ((int) (Color.red(color) * 0.299) + (int) (Color.green(color) * 0.587f) + (int) (Color.blue(color) * 0.114f)) < 186;
        }
    }

    private static class _WindowColorChecker {
        public static interface Callback {
            public void onResult(int color);
        }

        public _WindowColorChecker(Callback callback, boolean isTop) {
            _callback = callback;
            _isTop = isTop;
        }

        private final Callback _callback;
        private final boolean _isTop;
        private boolean _running = false;
        private final Bitmap _bmp = Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888);
        private final Rect _rectCache = new Rect();
        private static final Handler _handler = new Handler(Looper.getMainLooper());

        public void check(Window window) {
            if (_running || Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
                return;
            }
            int w = window.getDecorView().getWidth();
            int h = window.getDecorView().getHeight();
            if (w <= 0 || h <= 0 || !window.getDecorView().isAttachedToWindow()) {
                return;
            }
            _running = true;
            if (_isTop) {
                _rectCache.set(w / 2, 0, w / 2 + 1, 1);
            } else {
                _rectCache.set(w / 2, h - 1, w / 2 + 1, h);
            }
            try {
                PixelCopy.request(window, _rectCache, _bmp, new PixelCopy.OnPixelCopyFinishedListener() {
                    @Override
                    public void onPixelCopyFinished(int copyResult) {
                        _running = false;
                        if (copyResult == PixelCopy.SUCCESS) {
                            int color = _bmp.getPixel(0, 0);
                            _callback.onResult(color);
                        }
                    }
                }, _handler);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

}

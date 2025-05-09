package com.ZFFramework.ZFUIKit_impl;

import android.app.Activity;
import android.app.Application;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;

import com.ZFFramework.NativeUtil.ZFAndroidSize;
import com.ZFFramework.NativeUtil.ZFAndroidUI;
import com.ZFFramework.ZF_impl.ZFMainEntry;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class ZFUIOnScreenKeyboardState {
    private static Application.ActivityLifecycleCallbacks _activityLifecycleCallback = new Application.ActivityLifecycleCallbacks() {
        @Override
        public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
            ZFUIOnScreenKeyboardState.keyboardStateRegister(activity);
        }

        @Override
        public void onActivityDestroyed(Activity activity) {
            ZFUIOnScreenKeyboardState.keyboardStateUnregister(activity);
        }

        @Override
        public void onActivityStarted(Activity activity) {
        }

        @Override
        public void onActivityResumed(Activity activity) {
        }

        @Override
        public void onActivityPaused(Activity activity) {
        }

        @Override
        public void onActivityStopped(Activity activity) {
        }

        @Override
        public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
        }
    };

    protected static void native_keyboardStaticInit() {
        ZFMainEntry.app().registerActivityLifecycleCallbacks(_activityLifecycleCallback);
        if (ZFMainEntry.mainEntryActivity() != null) {
            ZFUIOnScreenKeyboardState.keyboardStateRegister(ZFMainEntry.mainEntryActivity());
        }
    }

    protected static void native_keyboardStaticCleanup() {
        ZFMainEntry.app().unregisterActivityLifecycleCallbacks(_activityLifecycleCallback);

        Iterator<Map.Entry<Activity, _RegisterFlagData>> entries = _keyboardStateRegisterFlag.entrySet().iterator();
        while (entries.hasNext()) {
            Map.Entry<Activity, _RegisterFlagData> entry = entries.next();
            ZFUIOnScreenKeyboardState.keyboardStateUnregister(entry.getKey());
        }
    }

    private static class _WindowData {
        public boolean keyboardStateDelaying = true;
        public final int[] keyboardFrame = new int[4];
    }

    private static Map<Window, _WindowData> _keyboardState = new HashMap<Window, _WindowData>();

    private static Handler _keyboardStateUpdater = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Window window = (Window) msg.obj;
            _WindowData windowData = _keyboardState.get(window);
            if (windowData != null) {
                windowData.keyboardStateDelaying = false;
                ZFUIOnScreenKeyboardState.notifyKeyboardStateOnUpdate(window);
            }
        }
    };

    private static class _RegisterFlagData {
        public View activityRootView = null;
        public _OnGlobalLayoutListener globalLayoutListener = null;

        public _RegisterFlagData(View activityRootView, _OnGlobalLayoutListener globalLayoutListener) {
            this.activityRootView = activityRootView;
            this.globalLayoutListener = globalLayoutListener;
        }
    }

    private static Map<Activity, _RegisterFlagData> _keyboardStateRegisterFlag = new HashMap<Activity, _RegisterFlagData>();

    private static class _OnGlobalLayoutListener implements OnGlobalLayoutListener {
        private WeakReference<Activity> _activity = null;
        private WeakReference<View> _activityRootView = null;

        public _OnGlobalLayoutListener(Activity activity, View activityRootView) {
            _activity = new WeakReference<Activity>(activity);
            _activityRootView = new WeakReference<View>(activityRootView);
        }

        private int _heightDiffOld = 0;

        @Override
        public void onGlobalLayout() {
            Window window = _activity.get().getWindow();
            _WindowData windowData = _keyboardState.get(window);
            if (windowData == null) {
                windowData = new _WindowData();
                _keyboardState.put(window, windowData);
            } else {
                windowData.keyboardStateDelaying = true;
            }

            Rect r = new Rect();
            _activityRootView.get().getWindowVisibleDisplayFrame(r);

            int heightDiff = _activityRootView.get().getRootView().getHeight() - (r.bottom - r.top);
            ZFAndroidSize screenSize = ZFAndroidUI.screenSize(ZFMainEntry.appContext());
            if ((heightDiff >= screenSize.height / 5) != (_heightDiffOld >= screenSize.height / 5)) {
                _heightDiffOld = heightDiff;
                _keyboardStateUpdater.sendMessage(Message.obtain(_keyboardStateUpdater, 0, window));
            }
        }
    }

    ;

    public static void keyboardStateRegister(Activity activity) {
        keyboardStateUnregister(activity);
        View activityRootView = ZFAndroidUI.activityRootView(activity);
        _OnGlobalLayoutListener listener = new _OnGlobalLayoutListener(activity, activityRootView);
        _keyboardStateRegisterFlag.put(activity, new _RegisterFlagData(activityRootView, listener));
        activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(listener);
        _keyboardState.put(activity.getWindow(), new _WindowData());
    }

    public static void keyboardStateUnregister(Activity activity) {
        View activityRootView = ZFAndroidUI.activityRootView(activity);
        _RegisterFlagData flagData = _keyboardStateRegisterFlag.remove(activity);
        if (flagData != null) {
            activityRootView.getViewTreeObserver().removeOnGlobalLayoutListener(flagData.globalLayoutListener);
            _keyboardState.remove(activity.getWindow());
        }
    }

    public static boolean native_keyboardShowing() {
        return (_keyboardFrame[3] > 0);
    }

    private static int[] _keyboardFrame = new int[4]; // left, top, width, height

    public static int[] native_keyboardFrame() {
        return _keyboardFrame;
    }

    /**
     * @brief for special conditions, such as ZFFramework's view tree was embeded to native view,
     * you should manually notify keyboard changing state
     */
    public static void notifyKeyboardStateOnUpdate(Window window) {
        if (window == null) {
            _keyboardFrame[0] = 0;
            _keyboardFrame[1] = 0;
            _keyboardFrame[2] = 0;
            _keyboardFrame[3] = 0;
            return;
        }
        _WindowData windowData = _keyboardState.get(window);
        if (windowData == null) {
            int old = _keyboardFrame[3];
            keyboardFrameUpdate(window, _keyboardFrame);
            if (_keyboardFrame[3] != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnUpdate();
            }
        } else if (windowData.keyboardStateDelaying) {
            System.arraycopy(windowData.keyboardFrame, 0, _keyboardFrame, 0, 4);
        } else {
            int old = windowData.keyboardFrame[3];
            keyboardFrameUpdate(window, windowData.keyboardFrame);
            System.arraycopy(windowData.keyboardFrame, 0, _keyboardFrame, 0, 4);
            if (_keyboardFrame[3] != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnUpdate();
            }
        }
    }

    public static void keyboardFrameUpdate(Window window, int[] outKeyboardFrame) {
        Rect rect = new Rect();
        window.getDecorView().getWindowVisibleDisplayFrame(rect);
        ZFAndroidSize screenSize = ZFAndroidUI.screenSize(ZFMainEntry.appContext());
        outKeyboardFrame[0] = 0;
        outKeyboardFrame[1] = rect.bottom;
        outKeyboardFrame[2] = screenSize.width;
        outKeyboardFrame[3] = screenSize.height - rect.bottom;
    }

    private static native void native_notifyKeyboardStateOnUpdate();
}

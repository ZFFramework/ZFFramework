package com.ZFFramework.ZF_impl;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.Window;

import java.lang.ref.WeakReference;

/*
 * @brief main entry
 *
 * typical start steps:
 * -  start your launcher activity
 * -  load any *.so files of ZFFramework and your app's cpp code
 * -  use one of these ways to start:
 *   -  ZFMainEntry.mainEntryFromLoader, which would create new Activity and finish Loader Activity,
 *     you must add this to your AndroidManifest.xml
 *       @code
 *         <activity
 *             android:name="com.ZFFramework.ZF_impl.ZFMainEntry"
 *             android:label="ZFFramework"
 *             android:launchMode="singleTask" >
 *         </activity>
 *       @endcode
 *
 *   -  attach to existing UI, without create new Activity
 *     -# ZFMainEntry.mainEntryEmbedAttach
 *     -# ZFUISysWindow.mainWindowRegisterForNativeView
 *     -# ZFMainEntry.mainEntryEmbedExecute
 *     -# ZFMainEntry.mainEntryEmbedDetach
 */
public final class ZFMainEntry extends Activity {

    /*
     * create from loader activity
     */
    public static void mainEntryFromLoader(Activity loaderActivity) {
        if (loaderActivity == null || (_app != null && _app.get() != null)) {
            return;
        }
        _app = new WeakReference<>(loaderActivity.getApplication());
        _appContext = new WeakReference<>(loaderActivity.getApplicationContext());
        Intent intent = new Intent(loaderActivity, ZFMainEntry.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        loaderActivity.startActivity(intent);
        loaderActivity.finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        _appContext = new WeakReference<Context>(this.getApplicationContext());
        mainEntryActivity(this);
        ZFFrameworkInit();
        ZFMainExecute(null);
    }

    @Override
    protected void onDestroy() {
        if (ZFMainEntry.mainEntryActivity() == this) {
            ZFFrameworkCleanup();
        }
        super.onDestroy();
    }

    // ============================================================
    public static void mainEntryEmbedAttach(Application app, Activity ownerActivity) {
        if (_app != null && _app.get() != null) {
            return;
        }
        if (app == null) {
            return;
        }
        _app = new WeakReference<>(app);
        _appContext = new WeakReference<>(app.getApplicationContext());
        mainEntryActivity(ownerActivity);
        ZFFrameworkInit();
    }

    public static int mainEntryEmbedExecute(String[] params) {
        return ZFMainExecute(params);
    }

    public static void mainEntryEmbedDetach() {
        if (_app == null || _app.get() == null) {
            return;
        }
        ZFFrameworkCleanup();
        mainEntryActivity(null);
    }

    // ============================================================
    // debug mode
    private static boolean _debugMode = false;

    public static void debugMode(boolean value) {
        _debugMode = value;
        ZFMainEntry.native_debugMode(value);
    }

    public static boolean debugMode() {
        return _debugMode;
    }

    private native static void native_debugMode(boolean value);

    // ============================================================
    // global state
    private static WeakReference<Application> _app = null;
    private static WeakReference<Context> _appContext = null;

    public static Application app() {
        return _app != null ? _app.get() : null;
    }

    public static Context appContext() {
        return _appContext != null ? _appContext.get() : null;
    }

    public static AssetManager assetManager() {
        return _appContext != null && _appContext.get() != null ? _appContext.get().getAssets() : null;
    }

    private static WeakReference<Activity> _mainEntryActivity = null;

    public static Activity mainEntryActivity() {
        return _mainEntryActivity != null ? _mainEntryActivity.get() : null;
    }

    public static void mainEntryActivity(Activity activity) {
        if (activity == null) {
            _mainEntryActivity = null;
        } else {
            _mainEntryActivity = new WeakReference<Activity>(activity);
        }
    }

    // ============================================================
    // native
    private static boolean _mainEntryIsStarted = false;

    public static void ZFFrameworkInit() {
        if (!_mainEntryIsStarted) {
            _mainEntryIsStarted = true;
            native_ZFFrameworkInit();
        }
    }

    public static void ZFFrameworkCleanup() {
        if (_mainEntryIsStarted) {
            _mainEntryIsStarted = false;
            native_ZFFrameworkCleanup();
            _app = null;
            System.exit(0);
        }
    }

    public static int ZFMainExecute(String[] params) {
        return native_ZFMainExecute(params);
    }

    // ============================================================
    // native communication
    protected static Object native_app() {
        return ZFMainEntry.app();
    }

    protected static Object native_appContext() {
        return ZFMainEntry.appContext();
    }

    protected static Object native_assetManager() {
        return ZFMainEntry.assetManager();
    }

    protected static Object native_mainEntryActivity() {
        return ZFMainEntry.mainEntryActivity();
    }

    private native static void native_ZFFrameworkInit();

    private native static void native_ZFFrameworkCleanup();

    private native static int native_ZFMainExecute(String[] params);

}

package com.ZFFramework.Android.ZF_impl;

import android.Manifest;
import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.view.Window;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

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
 *             android:name="com.ZFFramework.Android.ZF_impl.ZFMainEntry"
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
        _context = null;
        Intent intent = new Intent(loaderActivity, ZFMainEntry.class);
        loaderActivity.startActivity(intent);
        loaderActivity.finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
    }

    private boolean _mainEntryFirstTime = true;

    @Override
    protected void onResume() {
        super.onResume();
        if (_mainEntryFirstTime) {
            _mainEntryFirstTime = false;
            if (this._requestPermission()) {
                _mainEntry();
            }
        }
    }

    @Override
    protected void onDestroy() {
        if (_mainEntryCalled) {
            ZFFrameworkCleanup();
        }
        super.onDestroy();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        _mainEntry();
    }

    private boolean _requestPermission() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return true;
        }
        List<String> permissions = new ArrayList<String>();
        if (ZFMainEntry.sdcardRW()) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                permissions.add(Manifest.permission.READ_EXTERNAL_STORAGE);
                permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            }
        }
        if (permissions.isEmpty()) {
            return true;
        }
        String[] tmp = new String[permissions.size()];
        permissions.toArray(tmp);
        this.requestPermissions(tmp, 1);
        return false;
    }

    private boolean _mainEntryCalled = false;

    private void _mainEntry() {
        if (_mainEntryCalled) {
            return;
        }
        _mainEntryCalled = true;
        _context = new WeakReference<Context>(this);
        mainEntryActivity(this);
        ZFFrameworkInit();
        ZFMainExecute(null);
    }

    // ============================================================
    public static void mainEntryEmbedAttach(Application app, Context context, Activity ownerActivity) {
        if (_app != null && _app.get() != null) {
            return;
        }
        if (app == null || context == null || ownerActivity == null) {
            return;
        }
        _app = new WeakReference<>(app);
        _appContext = new WeakReference<>(context.getApplicationContext());
        _context = new WeakReference<>(context);
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
        _app = null;
        _appContext = null;
        _context = null;
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
    // whether we need sdcard permission
    private static boolean _sdcardRW = true;

    public static void sdcardRW(boolean value) {
        _sdcardRW = value;
    }

    public static boolean sdcardRW() {
        return _sdcardRW;
    }

    // ============================================================
    // global state
    private static WeakReference<Application> _app = null;
    private static WeakReference<Context> _appContext = null;
    private static WeakReference<Context> _context = null;

    public static Application app() {
        return _app != null ? _app.get() : null;
    }

    public static Context appContext() {
        return _appContext != null ? _appContext.get() : null;
    }

    public static Context context() {
        return _context != null ? _context.get() : null;
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

    private static void ZFFrameworkInit() {
        if (!_mainEntryIsStarted) {
            _mainEntryIsStarted = true;
            native_ZFFrameworkInit();
        }
    }

    private static void ZFFrameworkCleanup() {
        if (_mainEntryIsStarted) {
            _mainEntryIsStarted = false;
            native_ZFFrameworkCleanup();
        }
    }

    private static int ZFMainExecute(String[] params) {
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

    protected static Object native_context() {
        return ZFMainEntry.context();
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

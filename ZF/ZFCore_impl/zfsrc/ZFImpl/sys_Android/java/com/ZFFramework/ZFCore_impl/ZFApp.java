package com.ZFFramework.ZFCore_impl;

import android.os.Handler;
import android.os.Looper;

public final class ZFApp {

    private static final Handler _ZFP_appRestartHandler = new Handler(Looper.getMainLooper());

    public static void appRestart() {
        _ZFP_appRestartHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                native_appRestart();
            }
        }, 100);
    }

    public native static void native_appRestart();

}


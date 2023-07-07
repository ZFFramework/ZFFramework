package com.ZFFramework.Android.ZFCore_impl;

import android.os.Handler;

public final class ZFApp {

    private static final Handler _ZFP_appRestartHandler = new Handler();

    public static void appRestart() {
        _ZFP_appRestartHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                native_appRestart();
            }
        }, 100);
    }

    private native static void native_appRestart();

}


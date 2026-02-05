package com.ZFFramework.ZFCore_impl;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;

import com.ZFFramework.ZF_impl.ZFMainEntry;

public final class ZFApp {

    private static final Handler _ZFP_appRestartHandler = new Handler(Looper.getMainLooper());

    public static void appRestart() {
        _ZFP_appRestartHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                Context context = ZFMainEntry.appContext();
                Intent intent = context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
                if (intent != null) {
                    intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
                    context.startActivity(intent);
                }
                android.os.Process.killProcess(android.os.Process.myPid());
                System.exit(0);
            }
        }, 100);
    }

}


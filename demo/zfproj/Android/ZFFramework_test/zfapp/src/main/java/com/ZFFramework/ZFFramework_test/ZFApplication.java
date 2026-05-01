package com.ZFFramework.ZFFramework_test;

import android.app.Application;

public class ZFApplication extends Application {
    static {
        System.loadLibrary("ZFFramework_test");
    }
}

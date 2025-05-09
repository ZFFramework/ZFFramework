package com.ZFFramework.ZFFramework_test;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

import com.ZFFramework.ZF_impl.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("ZFFramework_test");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        if (BuildConfig.DEBUG) {
            ZFMainEntry.debugMode(true);
        }
        ZFMainEntry.mainEntryFromLoader(this);
    }
}


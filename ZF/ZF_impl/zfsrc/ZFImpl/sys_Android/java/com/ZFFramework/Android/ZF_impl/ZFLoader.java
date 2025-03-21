package com.ZFFramework.Android.ZF_impl;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

import com.ZFFramework.Android.ZFFramework_test.BuildConfig;

public abstract class ZFLoader extends Activity {

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

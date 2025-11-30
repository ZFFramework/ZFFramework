package com.ZFFramework.{ZFTT_R_proj_name};

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

import com.ZFFramework.ZF_impl.ZFMainEntry;
import {ZFTT_R_app_package}.BuildConfig;

public class Loader extends Activity {
    static {
        System.loadLibrary("{ZFTT_R_proj_name}");
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


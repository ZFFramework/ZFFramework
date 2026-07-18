package com.ZFFramework.{ZFTT_R_proj_name};

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

import com.ZFFramework.ZF_impl.ZFMainEntry;

public class ZFLoader extends Activity {
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


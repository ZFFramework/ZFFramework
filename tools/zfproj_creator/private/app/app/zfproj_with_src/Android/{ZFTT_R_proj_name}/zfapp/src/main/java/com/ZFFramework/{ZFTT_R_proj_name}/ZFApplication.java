package com.ZFFramework.{ZFTT_R_proj_name};

import android.app.Application;

public class ZFApplication extends Application {
    static {
        System.loadLibrary("{ZFTT_R_proj_name}");
    }
}

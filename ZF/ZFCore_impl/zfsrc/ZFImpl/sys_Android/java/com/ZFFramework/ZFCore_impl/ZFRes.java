package com.ZFFramework.ZFCore_impl;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import com.ZFFramework.ZF_impl.ZFMainEntry;
import android.content.res.AssetManager;

public final class ZFRes {
    private static final String _resPostfix = ".mp2";

    public static String[] native_listAssets(String resPath) {
        try {
            return ZFMainEntry.assetManager().list(resPath);
        }
        catch(IOException e) {
            return null;
        }
    }

    public static boolean native_resIsExist(String resPath) {
        try {
            String[] files = ZFMainEntry.assetManager().list(resPath);
            if(files.length > 0) {
                return true;
            }
        }
        catch(Exception e) {
        }

        try {
            InputStream is = ZFMainEntry.assetManager().open(resPath);
            is.close();
            return true;
        }
        catch(Exception e) {
        }

        try {
            InputStream is = ZFMainEntry.assetManager().open(resPath + _resPostfix);
            is.close();
            return true;
        }
        catch(Exception e) {
        }

        return false;
    }

    public static boolean native_resIsDir(String resPath) {
        try {
            String[] files = ZFMainEntry.assetManager().list(resPath);
            if(files.length > 0) {
                return true;
            }
        }
        catch(Exception e) {
        }

        return false;
    }
}

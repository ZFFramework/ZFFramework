package com.ZFFramework.Android.NativeUtil;

import java.io.IOException;
import java.io.InputStream;

/*
 * wrap Android's InputStream to ZFInput
 */
public class ZFAndroidInput {

    public InputStream input = null;

    public ZFAndroidInput(InputStream input) {
        this.input = input;
    }

    // ============================================================
    private static int native_nativeInputRead(Object nativeInput, byte[] buf, int count) {
        if (buf == null) {
            // not supported
            return -1;
        }
        ZFAndroidInput nativeInputTmp = (ZFAndroidInput) nativeInput;
        if (nativeInputTmp.input == null) {
            return 0;
        }
        try {
            int read = nativeInputTmp.input.read(buf, 0, count);
            if (read <= 0) {
                return 0;
            } else {
                return read;
            }
        } catch (IOException ignored) {
        }
        return 0;
    }

    private static void native_nativeInputClose(Object nativeInput) {
        ZFAndroidInput nativeInputTmp = (ZFAndroidInput) nativeInput;
        if (nativeInputTmp.input == null) {
            return;
        }
        try {
            nativeInputTmp.input.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        nativeInputTmp.input = null;
    }

}

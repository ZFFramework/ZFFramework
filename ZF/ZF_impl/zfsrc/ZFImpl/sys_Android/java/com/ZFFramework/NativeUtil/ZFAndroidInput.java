package com.ZFFramework.NativeUtil;

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
    public static int native_nativeInputRead(Object nativeInput, byte[] buf, int count) {
        if (buf == null) {
            // not supported
            return -1;
        }
        ZFAndroidInput nativeInputTmp = (ZFAndroidInput) nativeInput;
        if (nativeInputTmp.input == null) {
            return -1;
        }
        int offset = 0;
        do {
            try {
                int read = nativeInputTmp.input.read(buf, offset, count - offset);
                if (read <= 0) {
                    break;
                }
                offset += read;
            } catch (IOException ignored) {
            }
        } while (offset < count);
        return offset;
    }

    public static void native_nativeInputClose(Object nativeInput) {
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

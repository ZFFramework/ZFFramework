package com.ZFFramework.Android.NativeUtil;

import java.io.IOException;
import java.io.OutputStream;

public class ZFAndroidOutput {

    public OutputStream output = null;

    public ZFAndroidOutput(OutputStream output) {
        this.output = output;
    }

    // ============================================================
    public static int native_nativeOutputWrite(Object nativeOutput, byte[] buf, int size) {
        ZFAndroidOutput nativeOutputTmp = (ZFAndroidOutput) nativeOutput;
        if (nativeOutputTmp.output == null) {
            return 0;
        }
        try {
            nativeOutputTmp.output.write(buf, 0, size);
            return size;
        } catch (IOException ignored) {
        }
        return 0;
    }

}

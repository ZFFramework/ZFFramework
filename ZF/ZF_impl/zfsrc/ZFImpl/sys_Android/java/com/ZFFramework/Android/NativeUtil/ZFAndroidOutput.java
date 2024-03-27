package com.ZFFramework.Android.NativeUtil;

import java.io.IOException;
import java.io.OutputStream;

/*
 * wrap Android's OutputStream to ZFOutput
 */
public class ZFAndroidOutput {

    public OutputStream output = null;

    public ZFAndroidOutput(OutputStream output) {
        this.output = output;
    }

    // ============================================================
    private static int native_nativeOutputWrite(Object nativeOutput, byte[] buf, int size) {
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

    private static void native_nativeOutputClose(Object nativeOutput) {
        ZFAndroidOutput nativeOutputTmp = (ZFAndroidOutput) nativeOutput;
        if (nativeOutputTmp.output == null) {
            return;
        }
        try {
            nativeOutputTmp.output.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        nativeOutputTmp.output = null;
    }

}

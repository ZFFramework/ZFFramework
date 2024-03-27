package com.ZFFramework.Android.NativeUtil;

public class ZFObject {

    public static long invoke(String methodName) {
        return native_invoke(0, methodName, null);
    }

    public static long invoke(String methodName, long... params) {
        return native_invoke(0, methodName, _paramsConvert(params));
    }

    public static long invoke(String methodName, String... params) {
        return native_invoke_generic(0, methodName, _paramsConvert(params));
    }

    public static long invoke(long zfjniPointer, String methodName) {
        return native_invoke(zfjniPointer, methodName, null);
    }

    public static long invoke(long zfjniPointer, String methodName, long... params) {
        return native_invoke(zfjniPointer, methodName, _paramsConvert(params));
    }

    public static long invoke(long zfjniPointer, String methodName, String... params) {
        return native_invoke_generic(zfjniPointer, methodName, _paramsConvert(params));
    }

    // ============================================================
    public static long zfRetain(long zfjniPointer) {
        native_zfRetain(zfjniPointer);
        return zfjniPointer;
    }

    public static void zfRelease(long zfjniPointer) {
        native_zfRelease(zfjniPointer);
    }

    public static long zfautoRelease(long zfjniPointer) {
        native_zfautoRelease(zfjniPointer);
        return zfjniPointer;
    }

    // ============================================================
    private static long[] _paramsConvert(long... params) {
        long[] paramsTmp = null;
        if (params != null && params.length > 0) {
            paramsTmp = new long[params.length];
            System.arraycopy(params, 0, paramsTmp, 0, params.length);
        }
        return paramsTmp;
    }

    private static Object[] _paramsConvert(String... params) {
        Object[] paramsTmp = null;
        if (params != null && params.length > 0) {
            paramsTmp = new Object[params.length];
            System.arraycopy(params, 0, paramsTmp, 0, params.length);
        }
        return paramsTmp;
    }

    private static native long native_invoke(long zfjniPointer, String methodName, long[] params);

    private static native long native_invoke_generic(long zfjniPointer, String methodName, Object[] params);

    // ============================================================
    private static native void native_zfRetain(long zfjniPointer);

    private static native void native_zfRelease(long zfjniPointer);

    private static native void native_zfautoRelease(long zfjniPointer);

}

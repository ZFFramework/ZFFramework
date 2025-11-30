package com.ZFFramework.NativeUtil;

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
    public static native long zfRetain(long zfjniPointer);

    public static native void zfRelease(long zfjniPointer);

    public static native long zfautoRelease(long zfjniPointer);

    // ============================================================
    public static native String toString(long zfjniPointer);

    public static native double toNumber(long zfjniPointer);

    // ============================================================
    public static long toListener(ZFListener listener) {
        return native_toListener(listener);
    }

    public static native long native_toListener(Object listener);

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

    public static native long native_invoke(long zfjniPointer, String methodName, long[] params);

    public static native long native_invoke_generic(long zfjniPointer, String methodName, Object[] params);

}

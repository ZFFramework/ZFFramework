package com.ZFFramework.Android.NativeUtil;

import java.io.InputStream;

public class ZFLua {

    public static long ZFLuaExecute(String buf, long... params) {
        return native_ZFLuaExecute_buf(buf, _paramsConvert(params));
    }

    public static long ZFLuaExecute(InputStream input, long... params) {
        return native_ZFLuaExecute_input(new ZFAndroidInput(input), _paramsConvert(params));
    }

    public static long ZFLuaExecute(ZFAndroidInput input, long... params) {
        return native_ZFLuaExecute_input(input, _paramsConvert(params));
    }

    public static long ZFLuaExecute(long zfjniPointerZFInput, long... params) {
        return native_ZFLuaExecute(zfjniPointerZFInput, _paramsConvert(params));
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

    private static native long native_ZFLuaExecute_buf(String buf, long[] params);

    private static native long native_ZFLuaExecute_input(Object input, long[] params);

    private static native long native_ZFLuaExecute(long zfjniPointerZFInput, long[] params);

}

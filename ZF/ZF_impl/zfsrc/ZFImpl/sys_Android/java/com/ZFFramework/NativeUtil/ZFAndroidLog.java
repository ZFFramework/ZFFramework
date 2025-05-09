package com.ZFFramework.NativeUtil;

import android.annotation.SuppressLint;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ZFAndroidLog {

    public static String defaultLogTag = "JNILog";

    public static void p(String fmt) {
        android.util.Log.e(ZFAndroidLog.defaultLogTag, fmt);
    }

    public static void p(String fmt, Object... args) {
        android.util.Log.e(ZFAndroidLog.defaultLogTag, String.format(fmt, args));
    }

    public static void assertMsg(String fmt) {
        ZFAndroidLog._printAssertMsg(fmt);
    }

    public static void assertMsg(String fmt, Object... args) {
        ZFAndroidLog._printAssertMsg(String.format(fmt, args));
    }

    public static void shouldNotGoHere() {
        ZFAndroidLog._printAssertMsg("should not go here");
    }

    private static void _printAssertMsg(String msg) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        ZFAndroidLog.p("====================== assert message ======================");
        ZFAndroidLog.p("| called by %s [%s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber());
        ZFAndroidLog.p("| " + msg);
        ZFAndroidLog.p("---------------------- assert message ----------------------");
    }

    // caller info
    public static String callerInfo() {
        return ZFAndroidLog._callerInfo(1);
    }

    public static String callerInfo(int ignoreLevel) {
        return ZFAndroidLog._callerInfo(ignoreLevel);
    }

    @SuppressLint("DefaultLocale")
    private static String _callerInfo(int ignoreLevel) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4 + ignoreLevel];
        return String.format("[%s %s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber());
    }

    // stack trace
    public static String stackTrace() {
        return ZFAndroidLog._stackTrace(0, Integer.MAX_VALUE);
    }

    public static String stackTrace(int levelIgnore) {
        return ZFAndroidLog._stackTrace(levelIgnore, Integer.MAX_VALUE);
    }

    public static String stackTrace(int levelIgnore, int maxNum) {
        return ZFAndroidLog._stackTrace(levelIgnore, maxNum);
    }

    private static String _stackTrace(int levelIgnore, int maxNum) {
        StringBuilder sb = new StringBuilder();
        StackTraceElement[] ste = Thread.currentThread().getStackTrace();
        sb.append("======================== stack trace =======================");
        sb.append('\n');
        for (int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            sb.append(String.format("|  %s [%s (%d)]",
                    ste[i].getMethodName(),
                    ste[i].getFileName(),
                    ste[i].getLineNumber()));
            sb.append('\n');
        }
        sb.append("------------------------ stack trace -----------------------");
        sb.append('\n');
        return sb.toString();
    }

    public static void stackTracePrint() {
        ZFAndroidLog._stackTracePrint(0, Integer.MAX_VALUE);
    }

    public static void stackTracePrint(int levelIgnore) {
        ZFAndroidLog._stackTracePrint(levelIgnore, Integer.MAX_VALUE);
    }

    public static void stackTracePrint(int levelIgnore, int maxNum) {
        ZFAndroidLog._stackTracePrint(levelIgnore, maxNum);
    }

    private static void _stackTracePrint(int levelIgnore, int maxNum) {
        StackTraceElement[] ste = Thread.currentThread().getStackTrace();
        ZFAndroidLog.p("======================== stack trace =======================");
        for (int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            ZFAndroidLog.p(String.format("|  %s [%s (%d)]",
                    ste[i].getMethodName(),
                    ste[i].getFileName(),
                    ste[i].getLineNumber()));
        }
        ZFAndroidLog.p("------------------------ stack trace -----------------------");
    }

    public static boolean isEmpty(String s) {
        return s == null || s.isEmpty();
    }

    public static String className(Class<?> cls) {
        if (cls == null) {
            return "null";
        } else if (!isEmpty(cls.getSimpleName())) {
            return cls.getSimpleName();
        } else if (cls.getSuperclass() != null && !isEmpty(cls.getSuperclass().getSimpleName())) {
            return cls.getSuperclass().getSimpleName();
        }
        // com.xxx.Xxx$1/123456
        String name = cls.getName();
        int index = name.lastIndexOf(".");
        if (index >= 0) {
            return name.substring(index + 1);
        } else {
            return cls.getName();
        }
    }

    public static String className(String name) {
        if (name == null) {
            return null;
        } else {
            int pos = name.lastIndexOf(".");
            if (pos >= 0 && pos != name.length() - 1) {
                return name.substring(pos + 1);
            } else {
                return name;
            }
        }
    }

    /**
     * @brief get info of native ZFObject, param must a (ZFObject *) pointer
     * converted by #JNIConvertZFObjectFromJNIType
     */
    public native static String native_objectInfo(long zfjniPointerZFObject);

    // log utils
    @SuppressLint("SimpleDateFormat")
    private static final SimpleDateFormat _curTimeToStringFmtCache = new SimpleDateFormat("HH:mm:ss.SSS");

    public static String curTimeToString() {
        return _curTimeToStringFmtCache.format(new Date());
    }
}

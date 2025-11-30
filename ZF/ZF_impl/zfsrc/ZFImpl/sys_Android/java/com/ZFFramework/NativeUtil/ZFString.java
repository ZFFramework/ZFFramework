package com.ZFFramework.NativeUtil;

public class ZFString {

    public static boolean isEmpty(String s) {
        return s == null || s.isEmpty();
    }

    public static boolean isEqual(String s0, String s1) {
        if (s0 == null || s0.isEmpty()) {
            return s1 == null || s1.isEmpty();
        } else {
            return s0.equals(s1);
        }
    }

}

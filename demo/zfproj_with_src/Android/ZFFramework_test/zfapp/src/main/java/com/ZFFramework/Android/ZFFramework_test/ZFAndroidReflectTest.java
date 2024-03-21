package com.ZFFramework.Android.ZFFramework_test;

import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;

import java.util.ArrayList;
import java.util.List;

public class ZFAndroidReflectTest {

    public static int[] TestIntArr() {
        int[] ret = new int[3];
        for (int i = 0; i < ret.length; ++i) {
            ret[i] = i;
        }
        return ret;
    }

    public static String[] TestStringArr() {
        String[] ret = new String[3];
        for (int i = 0; i < ret.length; ++i) {
            ret[i] = String.format("s_%s", i);
        }
        return ret;
    }

    public static List<String> TestUnknownObject() {
        List<String> ret = new ArrayList<>();
        for (int i = 0; i < 3; ++i) {
            ret.add(String.format("S_%s", i));
        }
        return ret;
    }

    // ============================================================
    public ZFAndroidReflectTest() {
        ZFAndroidLog.p("ZFAndroidReflectTest()");
    }

    public ZFAndroidReflectTest(int p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(int %s)", p));
    }

    public ZFAndroidReflectTest(String p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(String %s)", p));
    }

    public ZFAndroidReflectTest(ZFAndroidReflectTest p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(ZFAndroidReflectTest %s)", p));
    }

    public ZFAndroidReflectTest(int[] p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(int[] %s)", p));
    }

    public ZFAndroidReflectTest(String[] p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(String[] %s)", (Object[]) p));
    }

    public ZFAndroidReflectTest(List<String> p) {
        ZFAndroidLog.p(String.format("ZFAndroidReflectTest(List<String> %s)", p));
    }

    // ============================================================
    public void test() {
        ZFAndroidLog.p("ZFAndroidReflectTest::test()");
    }

    public int testInt() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testInt()");
        return 1;
    }

    public Integer testInteger() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testInteger()");
        return 2;
    }

    public String testString() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testString()");
        return "3";
    }

    public ZFAndroidReflectTest testSelf() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testSelf()");
        return this;
    }

    public int[] testIntArr() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testIntArr()");
        return TestIntArr();
    }

    public String[] testStringArr() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testStringArr()");
        return TestStringArr();
    }

    public List<String> testUnknownObject() {
        ZFAndroidLog.p("ZFAndroidReflectTest::testUnknownObject()");
        return TestUnknownObject();
    }

}


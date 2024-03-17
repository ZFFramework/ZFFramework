package com.ZFFramework.Android.NativeUtil;

import android.os.Build;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Type;

/*
 * util to register native class to ZF
 *
 * limitations:
 * -  public fileds are registered as setter and getter method with the filed name
 * -  no array type supported (`char[]`, `String[]`, etc)
 * -  when a method referencing other class (as return type or param type),
 *   the referenced class must be registered before register the method
 */
public class ZFAndroidReflect {

    public static void register(Class<?> cls) {
        register(cls, null);
    }

    public static void register(Class<?> cls, String clsNameInZF) {
        long zfjniPointerCls = registerClass(cls, clsNameInZF);
        if (zfjniPointerCls == 0) {
            return;
        }
        for (Constructor<?> m : cls.getDeclaredConstructors()) {
            registerConstructor(zfjniPointerCls, m);
        }
        for (Method m : cls.getDeclaredMethods()) {
            registerMethod(zfjniPointerCls, m);
        }
    }

    public static void unregister(Class<?> cls) {
        unregisterClass(cls);
    }

    // ============================================================
    public static long registerClass(Class<?> cls) {
        return registerClass(cls, null);
    }

    public static long registerClass(Class<?> cls, String clsNameInZF) {
        if (clsNameInZF == null || clsNameInZF.isEmpty()) {
            clsNameInZF = cls.getSimpleName();
        }
        return native_registerClass(cls, cls.getName(), clsNameInZF);
    }

    public static void unregisterClass(Class<?> cls) {
        native_unregisterClass(cls.getName());
    }

    public static void registerConstructor(long zfjniPointerCls, Constructor<?> m) {
        if (Modifier.isPublic(m.getModifiers())) {
            native_registerConstructor(
                    zfjniPointerCls,
                    _paramTypeNames(m.getGenericParameterTypes())
            );
        }
    }

    public static void registerMethod(long zfjniPointerCls, Method m) {
        if (Modifier.isPublic(m.getModifiers())) {
            native_registerMethod(
                    zfjniPointerCls,
                    m.getName(),
                    Modifier.isStatic(m.getModifiers()),
                    _paramTypeName(m.getGenericReturnType()),
                    _paramTypeNames(m.getGenericParameterTypes())
            );
        }
    }

    // ============================================================
    private static native long native_registerClass(Class<?> cls, String clsNameInJava, String clsNameInZF);

    private static native void native_unregisterClass(String clsNameInJava);

    private static String _paramTypeName(Type type) {
        /*
         * Java Type => getTypeName() => toString()
         * ============================================================
         * void => void => void
         * int => int => int
         * int[] => int[] => class [I
         * int[][] => int[][] => class [[I
         * Integer => java.lang.Integer => class java.lang.Integer
         * Integer[] => java.lang.Integer[] => class [Ljava.lang.Integer;
         * Integer[][] => java.lang.Integer[][] => class [[Ljava.lang.Integer;
         * Class<?> => java.lang.Class<?> => java.lang.Class<?>
         * Class => java.lang.Class => class java.lang.Class
         * Class[] => java.lang.Class[] => class [Ljava.lang.Class;
         * List<?> => java.util.List<?> => java.util.List<?>
         * List<String>[] => java.util.List<java.lang.String>[] => java.util.List<java.lang.String>[]
         */
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            return type.getTypeName();
        } else {
            // class Xxx
            String ret = type.toString();
            if (ret.startsWith("class ")) {
                ret = ret.substring("class ".length());
            }
            int arrayCount = 0;
            while (ret.startsWith("[")) {
                if (ret.startsWith("[L")) {
                    // [Lxxx;
                    ret = ret.substring(2, ret.length() - 1);
                } else {
                    // [I
                    ret = ret.substring(1);
                }
                ++arrayCount;
            }
            switch (ret) {
                case "Z":
                    ret = "boolean";
                    break;
                case "B":
                    ret = "byte";
                    break;
                case "C":
                    ret = "char";
                    break;
                case "S":
                    ret = "short";
                    break;
                case "I":
                    ret = "int";
                    break;
                case "J":
                    ret = "long";
                    break;
                case "F":
                    ret = "float";
                    break;
                case "D":
                    ret = "double";
                    break;
                case "V":
                    ret = "void";
                    break;
                default:
                    break;
            }
            while (arrayCount > 0) {
                ret += "[]";
                --arrayCount;
            }
            return ret;
        }
    }

    private static String[] _paramTypeNames(Type[] types) {
        if (types == null || types.length == 0) {
            return null;
        }
        String[] ret = new String[types.length];
        for (int i = 0; i < types.length; i++) {
            ret[i] = _paramTypeName(types[i]);
        }
        return ret;
    }

    private static native void native_registerConstructor(long zfjniPointerCls,
                                                          String[] paramTypeNames);

    private static native void native_registerMethod(long zfjniPointerCls,
                                                     String methodName,
                                                     boolean isStatic,
                                                     String returnTypeName,
                                                     String[] paramTypeNames);

}

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
 * -  when a method referencing other class (as return type or param type),
 *   the referenced class must be registered before register the method,
 *   so you must register all necessary class (by #registerClass)
 *   before registering class contents (constructors/methods/fileds, by #registerClassContents),
 *   otherwise, all the class that has not be registered would be treated as plain Object,
 *   which may cause invalid method dispatching
 * -  methods with generic type as param, can not be dispatched if only generic type differ, e.g.
 *   @code
 *   public void func(List<String> p);
 *   public void func(List<Integer> p);
 *   @endcode
 *
 * -  public fileds are registered as setter and getter method with the filed name
 */
public class ZFAndroidReflect {

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

    public static void registerClassContents(Class<?> cls, long zfjniPointerCls) {
        for (Constructor<?> m : cls.getDeclaredConstructors()) {
            if (Modifier.isPublic(m.getModifiers())
                    && m.getGenericParameterTypes().length > 0
            ) {
                native_registerConstructor(
                        zfjniPointerCls,
                        _paramTypeNames(m.getGenericParameterTypes())
                );
            }
        }
        for (Method m : cls.getDeclaredMethods()) {
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
    }

    // ============================================================
    private static native long native_registerClass(Class<?> cls, String clsNameInJava, String clsNameInZF);

    private static native void native_unregisterClass(String clsNameInJava);

    // java.util.List<java.lang.String> => java.util.List
    private static String _typeNameFormat(String typeName) {
        return typeName.replaceAll("<.*>", "");
    }

    private static String _typeName(Type type) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            return type.getTypeName();
        }

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

    private static String _typeName(Class<?> cls) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            return cls.getTypeName();
        } else {
            if (cls.isArray()) {
                try {
                    Class<?> cl = cls;
                    int dimensions = 0;
                    while (cl.isArray()) {
                        dimensions++;
                        cl = cl.getComponentType();
                    }
                    StringBuilder sb = new StringBuilder();
                    sb.append(cl.getName());
                    for (int i = 0; i < dimensions; i++) {
                        sb.append("[]");
                    }
                    return sb.toString();
                } catch (Throwable e) { /*FALLTHRU*/ }
            }
            return cls.getName();
        }
    }

    private static String _paramTypeName(Type type) {
        return _typeNameFormat(_typeName(type));
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

    // ============================================================
    private static long native_registerClass(String clsNameInJava,
                                             String clsNameInZF) {
        Class<?> cls = null;
        try {
            cls = Class.forName(clsNameInJava);
        } catch (ClassNotFoundException ignored) {
        }
        if (cls != null) {
            return registerClass(cls, clsNameInZF);
        } else {
            return 0;
        }
    }

    private static void native_unregisterClass(String clsNameInJava,
                                               long zfjniPointerCls) {
        Class<?> cls = null;
        try {
            cls = Class.forName(clsNameInJava);
        } catch (ClassNotFoundException ignored) {
        }
        if (cls != null) {
            unregisterClass(cls);
        }
    }

    private static void native_registerClassContents(String clsNameInJava,
                                                     long zfjniPointerCls) {
        Class<?> cls = null;
        try {
            cls = Class.forName(clsNameInJava);
        } catch (ClassNotFoundException ignored) {
        }
        if (cls != null) {
            registerClassContents(cls, zfjniPointerCls);
        }
    }

    private static boolean native_typeCheck(String typeName, Object obj) {
        if (obj == null) {
            return true;
        }
        String typeNameObj = _typeName(obj.getClass());
        if (typeNameObj.compareTo(typeName) == 0) {
            return true;
        }
        if (typeName.contains("[]") || typeNameObj.contains("[]")) {
            return false;
        }
        Class<?> desired = null;
        try {
            desired = Class.forName(typeName);
        } catch (Exception ignored) {
        }
        return desired != null && desired.isAssignableFrom(obj.getClass());
    }

}

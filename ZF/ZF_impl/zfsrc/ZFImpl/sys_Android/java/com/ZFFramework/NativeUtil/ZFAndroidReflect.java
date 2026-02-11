package com.ZFFramework.NativeUtil;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.List;

public class ZFAndroidReflect {

    public static class InvokeResult {
        public Object result = null;
        public boolean success = false;
        public String errorHint = null;
    }

    public static void invokeDetail(InvokeResult invokeResult, Object obj, String name, Object... params) {
        native_invoke(invokeResult, obj, name, params);
    }

    public static Object invoke(Object obj, String name, Object... params) {
        InvokeResult invokeResult = (InvokeResult) native_obtainInvokeResult();
        native_invoke(invokeResult, obj, name, params);
        Object ret = invokeResult.result;
        native_releaseInvokeResult(invokeResult);
        return ret;
    }

    public static int asInt(Object v, int def) {
        if (v instanceof Number) {
            return ((Number) v).intValue();
        } else {
            return def;
        }
    }

    public static long asLong(Object v, long def) {
        if (v instanceof Number) {
            return ((Number) v).longValue();
        } else {
            return def;
        }
    }

    public static float asFloat(Object v, float def) {
        if (v instanceof Number) {
            return ((Number) v).floatValue();
        } else {
            return def;
        }
    }

    public static double asDouble(Object v, double def) {
        if (v instanceof Number) {
            return ((Number) v).doubleValue();
        } else {
            return def;
        }
    }

    public static String toString(Object v) {
        if (v != null) {
            return v.toString();
        } else {
            return "null";
        }
    }

    // ============================================================
    public static Class<?> primitiveWrap(Class<?> cls) {
        if (cls == null || !cls.isPrimitive()) {
            return cls;
        }
        if (cls == Integer.TYPE) return Integer.class;
        if (cls == Long.TYPE) return Long.class;
        if (cls == Boolean.TYPE) return Boolean.class;
        if (cls == Float.TYPE) return Float.class;
        if (cls == Character.TYPE) return Character.class;
        if (cls == Byte.TYPE) return Byte.class;
        if (cls == Double.TYPE) return Double.class;
        if (cls == Short.TYPE) return Short.class;
        if (cls == Void.TYPE) return Void.class;
        return cls;
    }

    public static Class<?> primitiveUnwrap(Class<?> cls) {
        if (cls == Integer.class) return int.class;
        if (cls == Long.class) return long.class;
        if (cls == Boolean.class) return boolean.class;
        if (cls == Float.class) return float.class;
        if (cls == Character.class) return char.class;
        if (cls == Byte.class) return byte.class;
        if (cls == Double.class) return double.class;
        if (cls == Short.class) return short.class;
        if (cls == Void.class) return void.class;
        return cls;
    }

    // ============================================================
    private static final List<InvokeResult> _cache = new ArrayList<>();

    public static Object native_obtainInvokeResult() {
        synchronized (_cache) {
            if (_cache.isEmpty()) {
                return new InvokeResult();
            } else {
                return _cache.remove(_cache.size() - 1);
            }
        }
    }

    public static void native_releaseInvokeResult(Object invokeResult) {
        InvokeResult tmp = (InvokeResult) invokeResult;
        tmp.result = null;
        tmp.success = false;
        tmp.errorHint = null;
        synchronized (_cache) {
            if (_cache.size() < 8) {
                _cache.add((InvokeResult) invokeResult);
            }
        }
    }

    /*
     * * invoke(ret, null, "SomeClass", ...) : new instance of `SomeClass`
     * * invoke(ret, null, "SomeClass.someMethod", ...) : call static method `SomeClass.someMethod(...)`
     * * invoke(ret, obj, "someMethod", ...) : call instance method `obj.someMethod(...)`
     *
     * note:
     * * for inner class: `OuterClass$InnerClass.someMethod`
     */
    public static void native_invoke(Object invokeResultTmp, Object obj, String name, Object[] params) {
        InvokeResult invokeResult = (InvokeResult) invokeResultTmp;
        int paramCount = params != null ? params.length : 0;
        try {
            if (obj != null) {
                if (_invoke_field(invokeResult, obj, params, paramCount, obj.getClass(), name)) {
                    return;
                }
                if (_invoke_method(invokeResult, obj, name, params, paramCount)) {
                    return;
                }
            } else {
                Class<?> cls = null;
                try {
                    cls = Class.forName(name);
                } catch (Exception ignored) {
                }
                if (cls != null) {
                    if (_invoke_ctor(invokeResult, params, paramCount, cls)) {
                        return;
                    }
                } else {
                    int pos = name.lastIndexOf(".");
                    if (pos < 0) {
                        invokeResult.errorHint = "unknown scope: " + name;
                        return;
                    }
                    try {
                        cls = Class.forName(name.substring(0, pos));
                    } catch (Exception ignored) {
                    }
                    if (cls == null) {
                        invokeResult.errorHint = "unknown class: " + name.substring(0, pos);
                        return;
                    }
                    String methodName = name.substring(pos + 1);
                    if (_invoke_field(invokeResult, null, params, paramCount, cls, methodName)) {
                        return;
                    }
                    if (_invoke_staticMethod(invokeResult, params, paramCount, cls, methodName)) {
                        return;
                    }
                }
            }
        } catch (Exception e) {
            invokeResult.errorHint = e.toString();
            return;
        }
        invokeResult.errorHint = String.format("no matching rule to call: %s, obj: %s, params: [%s]", name, obj, _invoke_paramInfo(params));
    }

    private static boolean _invoke_typeCheck(Class<?> desired, Object actual) {
        if (actual == null) {
            if (desired.isPrimitive() || desired.isEnum()) {
                return false;
            }
        } else {
            desired = primitiveWrap(desired);
            if (!desired.isInstance(actual)) {
                return false;
            }
        }
        return true;
    }

    private static boolean _invoke_paramCheck(Class<?>[] paramTypes, Object[] params) {
        if (paramTypes.length != (params == null ? 0 : params.length)) {
            return false;
        }
        for (int i = 0; i < paramTypes.length; ++i) {
            if (!_invoke_typeCheck(paramTypes[i], params[i])) {
                return false;
            }
        }
        return true;
    }

    private static boolean _invoke_ctor(InvokeResult invokeResult, Object[] params, int paramCount, Class<?> cls) throws Exception {
        if (paramCount == 0) {
            invokeResult.result = cls.newInstance();
            invokeResult.success = true;
            return true;
        }
        for (Constructor<?> m : cls.getConstructors()) {
            if (_invoke_paramCheck(m.getParameterTypes(), params)) {
                invokeResult.result = m.newInstance(params);
                invokeResult.success = true;
                return true;
            }
        }
        return false;
    }

    private static boolean _invoke_method(InvokeResult invokeResult, Object obj, String name, Object[] params, int paramCount) throws Exception {
        for (Method m : obj.getClass().getMethods()) {
            if (m.getParameterCount() != paramCount
                    || Modifier.isStatic(m.getModifiers())
                    || m.getName().compareTo(name) != 0
                    || !_invoke_paramCheck(m.getParameterTypes(), params)
            ) {
                continue;
            }
            invokeResult.result = m.invoke(obj, params);
            invokeResult.success = true;
            return true;
        }
        return false;
    }

    private static boolean _invoke_field(InvokeResult invokeResult, Object obj, Object[] params, int paramCount, Class<?> cls, String methodName) throws Exception {
        Field f = null;
        try {
            f = cls.getField(methodName);
        } catch (Exception ignored) {
            return false;
        }
        if (obj == null) {
            if (!Modifier.isStatic(f.getModifiers())) {
                throw new Exception(String.format("%s.%s is not static field", ZFAndroidLog.className(cls), methodName));
            }
        } else {
            if (Modifier.isStatic(f.getModifiers())) {
                throw new Exception(String.format("%s.%s is static field", ZFAndroidLog.className(cls), methodName));
            }
        }
        if (paramCount > 1) {
            throw new Exception(String.format("%s.%s field got invalid params: [%s]", _invoke_paramInfo(params)));
        }
        if (paramCount == 0) {
            invokeResult.result = f.get(obj);
        } else {
            f.set(obj, params[0]);
        }
        invokeResult.success = true;
        return true;
    }

    private static boolean _invoke_staticMethod(InvokeResult invokeResult, Object[] params, int paramCount, Class<?> cls, String methodName) throws Exception {
        for (Method m : cls.getMethods()) {
            if (m.getParameterCount() != paramCount
                    || !Modifier.isStatic(m.getModifiers())
                    || m.getName().compareTo(methodName) != 0
                    || !_invoke_paramCheck(m.getParameterTypes(), params)
            ) {
                continue;
            }
            invokeResult.result = m.invoke(null, params);
            invokeResult.success = true;
            return true;
        }
        return false;
    }

    private static String _invoke_paramInfo(Object[] params) {
        if (params == null || params.length == 0) {
            return "";
        } else {
            StringBuilder sb = new StringBuilder();
            for (Object p : params) {
                if (sb.length() > 0) {
                    sb.append(", ");
                }
                sb.append(p != null ? p.toString() : "null");
            }
            return sb.toString();
        }
    }

    // ============================================================
    public static Object native_implInterface(String interfaceClass, long zfjniPointerImpl) {
        Class<?> cls = null;
        try {
            cls = Class.forName(interfaceClass);
        } catch (Exception ignored) {
            ZFAndroidLog.p("[ZFAndroidReflect] no interface class for name: %s", interfaceClass);
            ZFObject.zfobjRelease(zfjniPointerImpl);
            return null;
        }
        ZFAndroidValue<Long> zfjniPointerImplHolder = new ZFAndroidValue<>(zfjniPointerImpl);
        try {
            return Proxy.newProxyInstance(cls.getClassLoader(), new Class[]{cls}, new InvocationHandler() {
                @Override
                public Object invoke(Object proxy, Method method, Object[] args) {
                    Object ret = native_implInterfaceCallback(zfjniPointerImplHolder.value, proxy, method.getName(), method, args);
                    if (ret == null) {
                        if (method.getName().compareTo("toString") == 0) {
                            return String.format("<Proxy[%s] %s>", ZFAndroidLog.className(interfaceClass), this.hashCode());
                        } else if (method.getName().compareTo("hashCode") == 0) {
                            return this.hashCode();
                        } else if (method.getName().compareTo("equals") == 0) {
                            return args != null && args.length == 1 && proxy == args[0];
                        }
                    }
                    return ret;
                }

                @Override
                protected void finalize() throws Throwable {
                    if (zfjniPointerImplHolder.value != 0) {
                        long tmp = zfjniPointerImplHolder.value;
                        zfjniPointerImplHolder.value = 0L;
                        ZFObject.zfobjRelease(tmp);
                    }
                    super.finalize();
                }
            });
        } catch (Exception e) {
            ZFAndroidLog.p("[ZFAndroidReflect] create proxy failed for interface class: %s, reason: %s", interfaceClass, e);
        }
        if (zfjniPointerImplHolder.value != 0) {
            long tmp = zfjniPointerImplHolder.value;
            zfjniPointerImplHolder.value = 0L;
            ZFObject.zfobjRelease(tmp);
        }
        return null;
    }

    public static native Object native_implInterfaceCallback(long zfjniPointerImpl, Object proxy, String methodName, Object method, Object[] args);

}

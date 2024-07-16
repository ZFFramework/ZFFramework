package com.ZFFramework.Android.NativeUtil;

import java.util.Iterator;
import java.util.Map;

public class ZFAndroidMapIter {

    public static Object native_iterFind(Object nativeMap, Object key) {
        if (nativeMap != null) {
            _Iter it = new _Iter();
            it.m = (Map) nativeMap;
            it.it = it.m.entrySet().iterator();
            while (it.it.hasNext()) {
                Map.Entry entry = (Map.Entry) it.it.next();
                if (entry.equals(key)) {
                    it.entry = entry;
                    break;
                }
            }
            if (it.entry != null) {
                return it;
            }
        }
        return null;
    }

    public static Object native_iter(Object nativeMap) {
        if (nativeMap != null) {
            _Iter it = new _Iter();
            it.m = (Map) nativeMap;
            it.it = it.m.entrySet().iterator();
            if (it.it.hasNext()) {
                it.entry = (Map.Entry) it.it.next();
            }
            return it;
        }
        return null;
    }

    public static boolean native_iterValid(Object iter) {
        _Iter it = (_Iter) iter;
        return it != null && it.it != null && (it.it.hasNext() || it.entry != null);
    }

    public static void native_iterNext(Object iter) {
        _Iter it = (_Iter) iter;
        if (it != null) {
            if (it.it != null && it.it.hasNext()) {
                it.entry = (Map.Entry) it.it.next();
            } else {
                it.entry = null;
            }
        }
    }

    public static Object native_iterKey(Object iter) {
        _Iter it = (_Iter) iter;
        if (it != null && it.entry != null) {
            return it.entry.getKey();
        } else {
            return null;
        }
    }

    public static Object native_iterValue(Object iter) {
        _Iter it = (_Iter) iter;
        if (it != null && it.entry != null) {
            return it.entry.getValue();
        } else {
            return null;
        }
    }

    public static void native_iterValue(Object iter, Object value) {
        _Iter it = (_Iter) iter;
        if (it != null && it.entry != null) {
            it.m.put(it.entry.getKey(), value);
        }
    }

    public static Object native_iterRemove(Object iter) {
        _Iter it = (_Iter) iter;
        if (it != null && it.entry != null) {
            Object ret = it.entry.getValue();
            it.it.remove();
            if (it.it.hasNext()) {
                it.entry = (Map.Entry) it.it.next();
            }
            return ret;
        } else {
            return null;
        }
    }

    public static Object native_iterCopy(Object iter) {
        _Iter it = (_Iter) iter;
        if (it != null && it.entry != null) {
            _Iter ret = new _Iter();
            ret.m = it.m;
            ret.it = it.it;
            ret.entry = it.entry;
            return ret;
        } else {
            return null;
        }
    }

    public static boolean native_iterIsEqual(Object iter0, Object iter1) {
        _Iter it0 = (_Iter) iter0;
        _Iter it1 = (_Iter) iter1;
        if (it0 == null || it0.entry == null) {
            return it1 == null || it1.entry == null;
        } else {
            if (it1 == null || it1.entry == null) {
                return false;
            } else {
                return it0.m == it1.m && it0.entry == it1.entry;
            }
        }
    }

    private static class _Iter {
        public Map m = null;
        public Iterator it = null;
        public Map.Entry entry = null;
    }

}

package com.ZFFramework.Android.ZFNet_impl;

import com.ZFFramework.Android.NativeUtil.ZFInputWrapper;
import com.ZFFramework.Android.NativeUtil.ZFAndroidInput;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public final class ZFHttpRequest {

    public long zfjniPointerOwnerZFHttpRequest = 0;
    public long zfjniPointerOwnerZFHttpResponse = 0;
    public HttpURLConnection connection = null;
    public Map<String, String> sendHeader = new HashMap<>();
    public List<String> recvHeader = null;
    public boolean running = true;

    private void _recvHeaderUpdate() {
        if (this.recvHeader != null) {
            return;
        }
        this.recvHeader = new ArrayList<>();
        if (this.connection == null) {
            return;
        }

        int count = this.connection.getHeaderFields().size();
        for (int i = 0; i < count; ++i) {
            String key = this.connection.getHeaderFieldKey(i);
            if (key != null) {
                this.recvHeader.add(key);
            }
        }
    }

    // ============================================================
    public static native void native_ZFHttpRequest_notifyResponse(long zfjniPointerOwnerZFHttpRequest,
                                                                  long zfjniPointerOwnerZFHttpResponse,
                                                                  int code,
                                                                  String errorHint,
                                                                  Object nativeBodyInput);

    // ============================================================
    // for request
    public static Object native_nativeTaskCreate(long zfjniPointerOwnerZFHttpRequest,
                                                 long zfjniPointerOwnerZFHttpResponse) {
        ZFHttpRequest task = new ZFHttpRequest();
        task.zfjniPointerOwnerZFHttpRequest = zfjniPointerOwnerZFHttpRequest;
        task.zfjniPointerOwnerZFHttpResponse = zfjniPointerOwnerZFHttpResponse;
        return task;
    }

    public static void native_nativeTaskDestroy(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            try {
                task.connection.disconnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
            task.connection = null;
        }
    }

    public static void native_url(Object nativeTask, String url, int timeout) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        try {
            task.connection = (HttpURLConnection) new URL(url).openConnection();
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (task.connection == null) {
            return;
        }
        task.connection.setConnectTimeout(timeout);
        task.connection.setReadTimeout(timeout);
    }

    public static void native_httpMethod(Object nativeTask, String httpMethod) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            try {
                task.connection.setRequestMethod(httpMethod);
            } catch (ProtocolException e) {
                e.printStackTrace();
            }
        }
    }

    public static void native_header(Object nativeTask, String key, String value) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            task.connection.addRequestProperty(key, value);
            task.sendHeader.put(key, value);
        }
    }

    public static String native_header(Object nativeTask, String key) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            return task.sendHeader.get(key);
        }
        return null;
    }

    public static int native_headerCount(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            return task.sendHeader.size();
        }
        return 0;
    }

    private static class NativeIterator {
        public Iterator<String> it = null;
        public String key = null;
    }

    public static Object native_headerIter(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            NativeIterator it = new NativeIterator();
            it.it = task.sendHeader.keySet().iterator();
            if (it.it.hasNext()) {
                it.key = it.it.next();
            }
            return it;
        }
        return null;
    }

    public static boolean native_headerIterValid(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null && it.it != null && (it.it.hasNext() || it.key != null);
    }

    public static void native_headerIterNext(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        if (it != null) {
            if (it.it != null && it.it.hasNext()) {
                it.key = it.it.next();
            } else {
                it.key = null;
            }
        }
    }

    public static String native_headerIterKey(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null ? it.key : null;
    }

    public static String native_headerIterValue(Object nativeTask, Object nativeIt) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            NativeIterator it = (NativeIterator) nativeIt;
            if (it != null && it.key != null) {
                return task.sendHeader.get(it.key);
            }
        }
        return null;
    }

    public static void native_headerIterValue(Object nativeTask, Object nativeIt, String value) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            NativeIterator it = (NativeIterator) nativeIt;
            if (it != null && it.key != null) {
                task.connection.setRequestProperty(it.key, value);
                task.sendHeader.put(it.key, value);
            }
        }
    }

    public static void native_headerIterRemove(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        if (it != null && it.it != null) {
            it.it.remove();
        }
    }

    public static void native_request(Object nativeTask, Object nativeInput) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        ZFInputWrapper nativeInputTmp = (ZFInputWrapper) nativeInput;

        int code = -1;
        String errorHint = null;
        ZFAndroidInput nativeBodyInput = null;

        if (!task.running) {
            return;
        }

        do {
            if (task.connection == null) {
                errorHint = "invalid connection";
                break;
            }
            if (nativeInputTmp != null) {
                task.connection.setDoOutput(true);
            }

            try {
                task.connection.connect();
            } catch (IOException e) {
                try {
                    code = task.connection.getResponseCode();
                } catch (IOException ignored) {
                }
                errorHint = "connection failed: " + e.getMessage();
                break;
            }
            if (!task.running) {
                return;
            }
            if (nativeInputTmp != null) {
                errorHint = native_request_write(task, nativeInputTmp);
                try {
                    nativeInputTmp.close();
                } catch (IOException ignored) {
                }
                if (errorHint != null) {
                    break;
                }
            }
            if (!task.running) {
                return;
            }

            try {
                code = task.connection.getResponseCode();
            } catch (IOException ignored) {
            }
            InputStream input = null;
            try {
                input = task.connection.getInputStream();
            } catch (IOException e) {
                errorHint = "read failed: " + e.getMessage();
            }
            if (input == null) {
                if (errorHint == null) {
                    errorHint = "read failed";
                }
                break;
            }
            if (!task.running) {
                return;
            }

            nativeBodyInput = new ZFAndroidInput(input);
        } while (false);

        if (task.running) {
            native_ZFHttpRequest_notifyResponse(
                    task.zfjniPointerOwnerZFHttpRequest,
                    task.zfjniPointerOwnerZFHttpResponse,
                    code,
                    errorHint,
                    nativeBodyInput
            );
        }
    }

    private static String native_request_write(ZFHttpRequest task, ZFInputWrapper input) {
        OutputStream output = null;
        if (task.connection != null) {
            try {
                output = task.connection.getOutputStream();
            } catch (IOException ignored) {
            }
        }
        if (output == null) {
            return "write fail";
        }
        final int ioBufSize = 256;
        byte[] ioBuf = new byte[ioBufSize];
        while (task.running) {
            int read = 0;
            try {
                read = input.read(ioBuf);
            } catch (IOException ignored) {
            }
            if (read > 0) {
                try {
                    output.write(ioBuf, 0, read);
                } catch (IOException ignored) {
                    return "write timeout";
                }
            }
            if (read < ioBufSize) {
                break;
            }
        }
        return null;
    }

    public static void native_requestCancel(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task.running = false;
    }

    // ============================================================
    // for response
    public static String native_responseHeader(Object nativeTask, String key) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            return task.connection.getHeaderField(key);
        }
        return null;
    }

    public static int native_responseHeaderCount(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._recvHeaderUpdate();
        return task.recvHeader.size();
    }

    public static Object native_responseHeaderIter(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._recvHeaderUpdate();
        NativeIterator it = new NativeIterator();
        it.it = task.recvHeader.iterator();
        if (it.it.hasNext()) {
            it.key = it.it.next();
        }
        return it;
    }

    public static boolean native_responseHeaderIterValid(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null && it.it != null && (it.it.hasNext() || it.key != null);
    }

    public static void native_responseHeaderIterNext(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        if (it != null) {
            if (it.it != null && it.it.hasNext()) {
                it.key = it.it.next();
            } else {
                it.key = null;
            }
        }
    }

    public static String native_responseHeaderIterKey(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null ? it.key : null;
    }

    public static String native_responseHeaderIterValue(Object nativeTask, Object nativeIt) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            NativeIterator it = (NativeIterator) nativeIt;
            if (it != null && it.key != null) {
                return task.connection.getHeaderField(it.key);
            }
        }
        return null;
    }

}

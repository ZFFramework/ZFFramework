package com.ZFFramework.Android.ZFNet_impl;

import com.ZFFramework.Android.NativeUtil.ZFAndroidBuffer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public final class ZFHttpRequest {

    public long zfjniPointerOwnerZFHttpRequest = 0;
    public long zfjniPointerOwnerZFHttpResponse = 0;
    public HttpURLConnection connection = null;
    public Map<String, String> sendHeader = new HashMap<>();
    public boolean running = true;

    // ============================================================
    public static native void native_ZFHttpRequest_notifyResponse(long zfjniPointerOwnerZFHttpRequest,
                                                                  long zfjniPointerOwnerZFHttpResponse,
                                                                  int code,
                                                                  String errorHint,
                                                                  Object body);

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
            return it;
        }
        return null;
    }

    public static boolean native_headerIterValid(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null && it.it != null && it.it.hasNext();
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

    public static void native_request(Object nativeTask, Object nativeBuf) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        int code = -1;
        String errorHint = null;
        ZFAndroidBuffer bodyBuf = null;

        if (!task.running) {
            return;
        }

        do {
            if (task.connection == null) {
                errorHint = "invalid connection";
                break;
            }
            if (nativeBuf != null) {
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
            if (nativeBuf != null) {
                errorHint = native_request_write(task, (ZFAndroidBuffer) nativeBuf);
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
                if(errorHint == null)
                {
                    errorHint = "read failed";
                }
                break;
            }
            if (!task.running) {
                return;
            }

            bodyBuf = new ZFAndroidBuffer();
            OutputStream output = bodyBuf.toOutputStream();
            int ioBufSize = 256;
            byte[] ioBuf = new byte[ioBufSize];
            while (task.running) {
                int read = 0;
                try {
                    read = input.read(ioBuf);
                } catch (IOException e) {
                    errorHint = "read timeout: " + e.getMessage();
                    bodyBuf = null;
                    break;
                }
                if (read > 0) {
                    try {
                        output.write(ioBuf, 0, read);
                    } catch (IOException ignored) {
                    }
                }
                if (read < ioBufSize) {
                    break;
                }
            }
        } while (false);

        if (task.running) {
            native_ZFHttpRequest_notifyResponse(
                    task.zfjniPointerOwnerZFHttpRequest,
                    task.zfjniPointerOwnerZFHttpResponse,
                    code,
                    errorHint,
                    bodyBuf
            );
        }
    }

    private static String native_request_write(ZFHttpRequest task, ZFAndroidBuffer buf) {
        InputStream input = null;
        OutputStream output = null;
        if (task.connection != null) {
            if (buf != null && buf.bufferSize() > 0) {
                input = buf.toInputStream();
                try {
                    output = task.connection.getOutputStream();
                } catch (IOException ignored) {
                }
            }
        }
        if (input == null || output == null) {
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
        if (task.connection != null) {
            return task.connection.getHeaderFields().size();
        }
        return 0;
    }

    public static Object native_responseHeaderIter(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            NativeIterator it = new NativeIterator();
            it.it = task.connection.getHeaderFields().keySet().iterator();
            return it;
        }
        return null;
    }

    public static boolean native_responseHeaderIterValid(Object nativeTask, Object nativeIt) {
        NativeIterator it = (NativeIterator) nativeIt;
        return it != null && it.it != null && it.it.hasNext();
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

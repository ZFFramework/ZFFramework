package com.ZFFramework.ZFNet_impl;

import com.ZFFramework.NativeUtil.ZFAndroidInput;
import com.ZFFramework.NativeUtil.ZFInputWrapper;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public final class ZFHttpRequest {

    public long zfjniPointerOwnerZFHttpRequest = 0;
    public long zfjniPointerOwnerZFHttpResponse = 0;
    public HttpURLConnection connection = null;
    public Map<String, String> sendHeaderCache = null;
    public Map<String, String> recvHeaderCache = null;
    public boolean running = true;

    private String _join(List<String> l) {
        StringBuilder sb = new StringBuilder();
        for (String v : l) {
            if (sb.length() > 0) {
                sb.append(",");
            }
            sb.append(v);
        }
        return sb.toString();
    }

    private void _sendHeaderUpdate() {
        if (this.sendHeaderCache != null || this.connection == null) {
            return;
        }
        this.sendHeaderCache = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (Map.Entry<String, List<String>> entry : this.connection.getRequestProperties().entrySet()) {
            this.sendHeaderCache.put(entry.getKey(), _join(entry.getValue()));
        }
    }

    private void _recvHeaderUpdate() {
        if (this.recvHeaderCache != null || this.connection == null) {
            return;
        }
        this.recvHeaderCache = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (Map.Entry<String, List<String>> entry : this.connection.getHeaderFields().entrySet()) {
            if (entry.getKey() != null) {
                this.recvHeaderCache.put(entry.getKey(), _join(entry.getValue()));
            }
        }
    }

    // ============================================================
    public static native void native_notifyResponse(long zfjniPointerOwnerZFHttpRequest,
                                                    long zfjniPointerOwnerZFHttpResponse,
                                                    int code,
                                                    String errorHint,
                                                    Object nativeBodyInput);

    // ============================================================
    // for request
    public static Object native_nativeTaskCreate(long zfjniPointerOwnerZFHttpRequest) {
        ZFHttpRequest task = new ZFHttpRequest();
        task.zfjniPointerOwnerZFHttpRequest = zfjniPointerOwnerZFHttpRequest;
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
            task.sendHeaderCache = null;
        }
    }

    public static void native_headerRemove(Object nativeTask, String key) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            if (task.connection.getRequestProperty(key) != null) {
                task.connection.setRequestProperty(key, "");
                task.sendHeaderCache = null;
            }
        }
    }

    public static String native_header(Object nativeTask, String key) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._sendHeaderUpdate();
        return task.sendHeaderCache.get(key);
    }

    public static int native_headerCount(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        if (task.connection != null) {
            task._sendHeaderUpdate();
            return task.sendHeaderCache.size();
        }
        return 0;
    }

    public static Object native_headerMap(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._sendHeaderUpdate();
        return task.sendHeaderCache;
    }

    public static void native_request(Object nativeTask, Object nativeInput, long zfjniPointerOwnerZFHttpResponse) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task.zfjniPointerOwnerZFHttpResponse = zfjniPointerOwnerZFHttpResponse;
        ZFInputWrapper nativeInputTmp = (ZFInputWrapper) nativeInput;

        int code = -1;
        String errorHint = null;
        ZFAndroidInput nativeBodyInput = null;

        if (!task.running) {
            return;
        }
        task._sendHeaderUpdate();

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
            native_notifyResponse(
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
        task._recvHeaderUpdate();
        return task.recvHeaderCache.get(key);
    }

    public static int native_responseHeaderCount(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._recvHeaderUpdate();
        return task.recvHeaderCache.size();
    }

    public static Object native_responseHeaderMap(Object nativeTask) {
        ZFHttpRequest task = (ZFHttpRequest) nativeTask;
        task._recvHeaderUpdate();
        return task.recvHeaderCache;
    }

}

package com.ZFFramework.ZFNet_impl;

import com.ZFFramework.ZF_impl.ZFResultType;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.TimeUnit;

import okhttp3.ConnectionPool;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import okio.ByteString;

public final class ZFWebSocket {

    public long zfjniPointerOwnerZFWebSocket = 0;
    public WebSocket ws = null;
    public int wsTaskId = 0;

    public static Object native_open(long zfjniPointerOwnerZFWebSocket, String url) {
        ZFWebSocket ws = new ZFWebSocket();
        ws.zfjniPointerOwnerZFWebSocket = zfjniPointerOwnerZFWebSocket;
        ++ws.wsTaskId;
        ws.ws = new OkHttpClient.Builder()
                .addInterceptor(new GzipRequestInterceptor())
                .connectionPool(new ConnectionPool(5, 5, TimeUnit.MINUTES))
                .build()
                .newWebSocket(new Request.Builder().url(url).build(), new WebSocketListener() {
                    private final int _wsTaskIdSaved = ws.wsTaskId;

                    @Override
                    public void onOpen(@NotNull WebSocket webSocket, @NotNull Response response) {
                        super.onOpen(webSocket, response);
                        if (_wsTaskIdSaved == ws.wsTaskId) {
                            native_notifyOnOpen(ws.zfjniPointerOwnerZFWebSocket);
                        }
                        try {
                            response.close();
                        } catch (Exception ignored) {
                        }
                    }

                    @Override
                    public void onClosed(@NotNull WebSocket webSocket, int code, @NotNull String reason) {
                        super.onClosed(webSocket, code, reason);
                        if (_wsTaskIdSaved == ws.wsTaskId) {
                            ++ws.wsTaskId;
                            native_notifyOnClose(ws.zfjniPointerOwnerZFWebSocket, ZFResultType.e_Success, reason);
                        }
                    }

                    @Override
                    public void onClosing(@NotNull WebSocket webSocket, int code, @NotNull String reason) {
                        super.onClosing(webSocket, code, reason);
                    }

                    @Override
                    public void onFailure(@NotNull WebSocket webSocket, @NotNull Throwable t, @Nullable Response response) {
                        super.onFailure(webSocket, t, response);
                        if (_wsTaskIdSaved == ws.wsTaskId) {
                            ++ws.wsTaskId;
                            native_notifyOnClose(ws.zfjniPointerOwnerZFWebSocket, ZFResultType.e_Fail, t.toString());
                        }
                        try {
                            response.close();
                        } catch (Exception ignored) {
                        }
                    }

                    @Override
                    public void onMessage(@NotNull WebSocket webSocket, @NotNull String text) {
                        super.onMessage(webSocket, text);
                        if (_wsTaskIdSaved == ws.wsTaskId) {
                            native_notifyOnRecv(ws.zfjniPointerOwnerZFWebSocket, text.getBytes());
                        }
                    }

                    @Override
                    public void onMessage(@NotNull WebSocket webSocket, @NotNull ByteString bytes) {
                        super.onMessage(webSocket, bytes);
                        if (_wsTaskIdSaved == ws.wsTaskId) {
                            native_notifyOnRecvBin(ws.zfjniPointerOwnerZFWebSocket, bytes.toByteArray());
                        }
                    }
                });
        return ws;
    }

    public static void native_close(Object nativeWebSocket) {
        ZFWebSocket ws = (ZFWebSocket) nativeWebSocket;
        ++ws.wsTaskId;
        ws.ws.close(1000, null);
        ws.zfjniPointerOwnerZFWebSocket = 0;
        ws.ws = null;
    }

    public static void native_send(Object nativeWebSocket, Object data) {
        ZFWebSocket ws = (ZFWebSocket) nativeWebSocket;
        ws.ws.send(new String((byte[]) data, StandardCharsets.UTF_8));
    }

    public static void native_sendBin(Object nativeWebSocket, Object data) {
        ZFWebSocket ws = (ZFWebSocket) nativeWebSocket;
        byte[] tmp = (byte[]) data;
        ws.ws.send(ByteString.of(tmp, 0, tmp.length));
    }

    public static native void native_notifyOnOpen(long zfjniPointerOwnerZFWebSocket);

    public static native void native_notifyOnClose(long zfjniPointerOwnerZFWebSocket, int result, String reasonHint);

    public static native void native_notifyOnRecv(long zfjniPointerOwnerZFWebSocket, Object data);

    public static native void native_notifyOnRecvBin(long zfjniPointerOwnerZFWebSocket, Object data);

}

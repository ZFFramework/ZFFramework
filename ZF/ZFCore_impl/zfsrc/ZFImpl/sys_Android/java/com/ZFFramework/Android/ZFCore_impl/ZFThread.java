package com.ZFFramework.Android.ZFCore_impl;

import android.os.Handler;
import android.os.Message;

public final class ZFThread {
    // ============================================================
    // common
    private static long currentThreadId() {
        return Thread.currentThread().getId();
    }

    // ============================================================
    // executeInMainThread
    private static Handler _mainThreadHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            native_doExecuteInMainThread(msg.what, ZFThread.currentThreadId());
        }
    };
    protected static void native_executeInMainThread(int executeDataId) {
        _mainThreadHandler.sendEmptyMessage(executeDataId);
    }
    private native static void native_doExecuteInMainThread(int executeDataId, long sysThread);

    // ============================================================
    // executeInNewThread
    private static class _NewThread extends Thread {
        public int executeDataId = 0;

        public _NewThread(int executeDataId) {
            this.executeDataId = executeDataId;
        }

        @Override
        public void run() {
            super.run();
            native_doExecuteInNewThread(this.executeDataId, ZFThread.currentThreadId());
        }
    }
    protected static void native_executeInNewThread(int executeDataId) {
        _NewThread thread = new _NewThread(executeDataId);
        thread.start();
    }
    private native static void native_doExecuteInNewThread(int executeDataId, long sysThread);

    // ============================================================
    // other native communication
    protected static long native_currentThread() {
        return ZFThread.currentThreadId();
    }
    protected static void native_sleep(long miliSecs) {
        try {
            Thread.sleep(miliSecs);
        }
        catch(InterruptedException e) {
            e.printStackTrace();
        }
    }
}

package com.ZFFramework.ZFCore_impl;

import android.os.Handler;
import android.os.Looper;

public final class ZFThread {
    // ============================================================
    // common
    private static long currentThreadId() {
        return Thread.currentThread().getId();
    }

    // ============================================================
    // executeInMainThread
    private static class _MainThreadRunnable implements Runnable {
        public boolean running = true;
        public int executeDataId;

        public _MainThreadRunnable(int executeDataId) {
            this.executeDataId = executeDataId;
        }

        @Override
        public void run() {
            if (this.running) {
                native_doExecuteInMainThread(this.executeDataId, ZFThread.currentThreadId());
            }
        }
    }

    private static Handler _mainThreadHandler = new Handler(Looper.getMainLooper());

    protected static Object native_executeInMainThread(int executeDataId) {
        _MainThreadRunnable taskData = new _MainThreadRunnable(executeDataId);
        _mainThreadHandler.post(taskData);
        return taskData;
    }

    protected static void native_executeInMainThreadCleanup(Object nativeToken) {
        _MainThreadRunnable taskData = (_MainThreadRunnable) nativeToken;
        taskData.running = false;
    }

    private native static void native_doExecuteInMainThread(int executeDataId, long sysThread);

    // ============================================================
    // executeInNewThread
    private static class _NewThread extends Thread {
        public boolean running = true;
        public int executeDataId;

        public _NewThread(int executeDataId) {
            this.executeDataId = executeDataId;
        }

        @Override
        public void run() {
            super.run();
            if (this.running) {
                native_doExecuteInNewThread(this.executeDataId, ZFThread.currentThreadId());
            }
        }
    }

    protected static Object native_executeInNewThread(int executeDataId) {
        _NewThread thread = new _NewThread(executeDataId);
        thread.start();
        return thread;
    }

    protected static void native_executeInNewThreadCleanup(Object nativeToken) {
        _NewThread thread = (_NewThread) nativeToken;
        thread.running = false;
    }

    private native static void native_doExecuteInNewThread(int executeDataId, long sysThread);

    // ============================================================
    // other native communication
    protected static long native_currentThread() {
        return ZFThread.currentThreadId();
    }
}

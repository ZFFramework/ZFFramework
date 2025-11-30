package com.ZFFramework.ZFCore_impl;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.util.Timer;
import java.util.TimerTask;

public final class ZFTimer {
    public Object zfnativeImpl() {
        return this._timer;
    }

    public static Object native_nativeTimerCreate() {
        return new ZFTimer();
    }

    // ============================================================
    private static class _MainThreadHandler extends Handler {
        public _MainThreadHandler() {
            super(Looper.getMainLooper());
        }

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            _TimerTask timerTask = (_TimerTask) msg.obj;
            if (timerTask._running) {
                ZFTimer.native_notifyTimerActivate(timerTask._zfjniPointerToken, timerTask._timerImplId);
            }
        }
    }

    private static class _TimerTask extends TimerTask {
        protected boolean _running = true;
        protected long _zfjniPointerToken = 0;
        protected long _timerImplId = 0;

        public _TimerTask(long zfjniPointerToken) {
            _zfjniPointerToken = zfjniPointerToken;
        }

        @Override
        public void run() {
            if (_running) {
                _mainThreadCallback.sendMessage(Message.obtain(_mainThreadCallback, 0, this));
            }
        }

        @Override
        public boolean cancel() {
            _running = false;
            _zfjniPointerToken = 0;
            return super.cancel();
        }
    }

    private Timer _timer = null;
    private _TimerTask _timerTask = null;
    private static final Handler _mainThreadCallback = new _MainThreadHandler();

    // ============================================================
    public static void native_start(Object nativeTimer, long zfjniPointerToken, long interval, long timerImplId) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder._timer = new Timer();
        holder._timerTask = new _TimerTask(zfjniPointerToken);
        holder._timerTask._timerImplId = timerImplId;
        holder._timer.schedule(holder._timerTask, interval, interval > 0 ? interval : 1);
    }

    public static void native_stop(Object nativeTimer) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder._timerTask.cancel();
        holder._timer.cancel();
        holder._timer.purge();
        holder._timer = null;
        holder._timerTask = null;
    }

    public native static void native_notifyTimerActivate(long zfjniPointerToken, long timerImplId);

}

package com.ZFFramework.ZFCore_impl;

import android.os.Handler;
import android.os.Message;

import java.util.Timer;
import java.util.TimerTask;

public final class ZFTimer {
    public Timer timer = null;
    public ZFTimerTask timerTask = null;

    public Object zfnativeImpl() {
        return this.timer;
    }

    public static Object native_nativeTimerCreate() {
        return new ZFTimer();
    }

    private static class _MainThreadHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFTimerTask timerTask = (ZFTimerTask) msg.obj;
            if (timerTask._running) {
                ZFTimer.native_notifyTimerActivate(timerTask._zfjniPointerToken, timerTask._timerImplId);
            }
        }
    }

    private static Handler _mainThreadCallback = new _MainThreadHandler();

    protected static class ZFTimerTask extends TimerTask {
        protected boolean _running = true;
        protected long _zfjniPointerToken = 0;
        protected long _timerImplId = 0;

        public ZFTimerTask(long zfjniPointerToken) {
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

    public static void native_start(Object nativeTimer, long zfjniPointerToken, long interval, long timerImplId) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder.timer = new Timer();
        holder.timerTask = new ZFTimerTask(zfjniPointerToken);
        holder.timerTask._timerImplId = timerImplId;
        holder.timer.schedule(holder.timerTask, interval, interval > 0 ? interval : 1);
    }

    public static void native_stop(Object nativeTimer) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder.timerTask.cancel();
        holder.timer.cancel();
        holder.timer.purge();
        holder.timer = null;
        holder.timerTask = null;
    }

    public native static void native_notifyTimerActivate(long zfjniPointerToken, long timerImplId);

}

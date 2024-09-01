package com.ZFFramework.Android.ZFCore_impl;

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

    protected static Object native_nativeTimerCreate() {
        return new ZFTimer();
    }

    private static class _MainThreadHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFTimerTask timerTask = (ZFTimerTask) msg.obj;
            if (timerTask._running) {
                ZFTimer.native_notifyTimerActivate(timerTask._zfjniPointerToken);
            }
        }
    }

    private static Handler _mainThreadCallback = new _MainThreadHandler();

    protected static class ZFTimerTask extends TimerTask {
        protected boolean _running = true;
        protected long _zfjniPointerToken = 0;

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

    protected static void native_timerStart(Object nativeTimer, long zfjniPointerToken, long timerInterval) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder.timer = new Timer();
        holder.timerTask = new ZFTimerTask(zfjniPointerToken);
        holder.timer.schedule(holder.timerTask, timerInterval, timerInterval);
    }

    protected static void native_timerStop(Object nativeTimer) {
        ZFTimer holder = (ZFTimer) nativeTimer;
        holder.timerTask.cancel();
        holder.timer.cancel();
        holder.timer.purge();
        holder.timer = null;
        holder.timerTask = null;
    }

    protected native static void native_notifyTimerActivate(long zfjniPointerToken);

}

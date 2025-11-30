package com.ZFFramework.NativeUtil;

import android.os.Handler;
import android.os.Looper;

import java.util.HashMap;
import java.util.Map;

public class ZFAndroidPost {

    public static synchronized int run(Runnable callback) {
        return run(callback, 0);
    }

    public static synchronized int run(Runnable callback, long delay) {
        do {
            ++_taskIdCur;
            if (_taskIdCur < 0) {
                _taskIdCur = 0;
            }
        } while (_taskMap.containsKey(_taskIdCur));
        int taskId = _taskIdCur;
        _Runnable impl = new _Runnable();
        impl.callback = callback;
        impl.taskId = taskId;
        _taskMap.put(taskId, impl);
        _handler.postDelayed(impl, delay);
        return taskId;
    }

    public static synchronized void cancel(int taskId) {
        _Runnable impl = _taskMap.remove(taskId);
        if (impl != null) {
            _handler.removeCallbacks(impl);
            impl.callback = null;
        }
    }

    // ============================================================
    private static int _taskIdCur = 0;
    private static final Map<Integer, _Runnable> _taskMap = new HashMap<>();
    private static final Handler _handler = new Handler(Looper.getMainLooper());

    private static class _Runnable implements Runnable {
        public Runnable callback = null;
        public int taskId = ZFTaskId.INVALID;

        @Override
        public void run() {
            _taskMap.remove(taskId);
            callback.run();
            callback = null;
        }
    }

}

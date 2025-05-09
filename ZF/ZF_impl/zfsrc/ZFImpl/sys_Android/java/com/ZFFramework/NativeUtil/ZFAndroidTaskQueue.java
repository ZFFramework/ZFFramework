package com.ZFFramework.NativeUtil;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

/*
 * run task in background
 */
public class ZFAndroidTaskQueue {

    private static ZFAndroidTaskQueue _instance = null;

    public static ZFAndroidTaskQueue instance() {
        if (_instance == null) {
            _instance = new ZFAndroidTaskQueue();
        }
        return _instance;
    }

    public void run(Runnable runnable) {
        if (runnable == null) {
            return;
        }
        if (_thread == null) {
            synchronized (_queue) {
                if (_thread == null) {
                    _thread = new Thread("ZFAndroidTaskQueue") {
                        @Override
                        public void run() {
                            super.run();
                            while (_running) {
                                Runnable runnable = null;
                                try {
                                    runnable = _queue.take();
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                                if (runnable == null) {
                                    continue;
                                }
                                runnable.run();
                            }
                        }
                    };
                    _thread.start();
                }
            }
        }
        _queue.add(runnable);
    }

    public void cancel(Runnable runnable) {
        if (runnable != null) {
            _queue.remove(runnable);
        }
    }

    public void close() {
        _running = false;
    }

    private final BlockingQueue<Runnable> _queue = new LinkedBlockingDeque<>();
    private Thread _thread = null;
    private boolean _running = true;

}

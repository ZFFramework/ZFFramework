package com.ZFFramework.NativeUtil;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class ZFAndroidAsync {

    public static void run(Runnable runnable) {
        _pool.execute(runnable);
    }

    private static final int _CPU_COUNT = Runtime.getRuntime().availableProcessors();
    private static final ThreadFactory _threadFactory = new ThreadFactory() {
        private final AtomicInteger _count = new AtomicInteger(1);

        @Override
        public Thread newThread(Runnable r) {
            return new Thread(r, String.format("ZFAndroidAsync #%s", _count));
        }
    };
    private static final ThreadPoolExecutor _pool = new ThreadPoolExecutor(
            Math.max(2, Math.min(_CPU_COUNT, 4))
            , _CPU_COUNT * 2
            , 30
            , TimeUnit.SECONDS
            , new LinkedBlockingQueue<>(1024)
            , _threadFactory
    ) {{
        allowCoreThreadTimeOut(true);
    }};

}

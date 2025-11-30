package com.ZFFramework.NativeUtil;

public class ZFRunnable {

    public static interface P0 {
        public void run();
    }

    public static interface P1<P0> {
        public void run(P0 p0);
    }

    public static interface P2<P0, P1> {
        public void run(P0 p0, P1 p1);
    }

    public static interface P3<P0, P1, P2> {
        public void run(P0 p0, P1 p1, P2 p2);
    }

    public static interface P4<P0, P1, P2, P3> {
        public void run(P0 p0, P1 p1, P2 p2, P3 p3);
    }

    public static interface P5<P0, P1, P2, P3, P4> {
        public void run(P0 p0, P1 p1, P2 p2, P3 p3, P4 p4);
    }

    public static interface P6<P0, P1, P2, P3, P4, P5> {
        public void run(P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);
    }

    public static interface P7<P0, P1, P2, P3, P4, P5, P6> {
        public void run(P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);
    }

    public static interface P8<P0, P1, P2, P3, P4, P5, P6, P7> {
        public void run(P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);
    }

    // ============================================================
    public static void RUN(Runnable c) {
        if (c != null) {
            c.run();
        }
    }

    public static void RUN(P0 c) {
        if (c != null) {
            c.run();
        }
    }

    public static <P0> void RUN(P1<P0> c, P0 p0) {
        if (c != null) {
            c.run(p0);
        }
    }

    public static <P0, P1> void RUN(P2<P0, P1> c, P0 p0, P1 p1) {
        if (c != null) {
            c.run(p0, p1);
        }
    }

    public static <P0, P1, P2> void RUN(P3<P0, P1, P2> c, P0 p0, P1 p1, P2 p2) {
        if (c != null) {
            c.run(p0, p1, p2);
        }
    }

    public static <P0, P1, P2, P3> void RUN(P4<P0, P1, P2, P3> c, P0 p0, P1 p1, P2 p2, P3 p3) {
        if (c != null) {
            c.run(p0, p1, p2, p3);
        }
    }

    public static <P0, P1, P2, P3, P4> void RUN(P5<P0, P1, P2, P3, P4> c, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4) {
        if (c != null) {
            c.run(p0, p1, p2, p3, p4);
        }
    }

    public static <P0, P1, P2, P3, P4, P5> void RUN(P6<P0, P1, P2, P3, P4, P5> c, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
        if (c != null) {
            c.run(p0, p1, p2, p3, p4, p5);
        }
    }

    public static <P0, P1, P2, P3, P4, P5, P6> void RUN(P7<P0, P1, P2, P3, P4, P5, P6> c, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
        if (c != null) {
            c.run(p0, p1, p2, p3, p4, p5, p6);
        }
    }

    public static <P0, P1, P2, P3, P4, P5, P6, P7> void RUN(P8<P0, P1, P2, P3, P4, P5, P6, P7> c, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
        if (c != null) {
            c.run(p0, p1, p2, p3, p4, p5, p6, p7);
        }
    }

}

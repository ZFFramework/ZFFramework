package com.ZFFramework.NativeUtil;

import com.ZFFramework.ZF_impl.ZFSeekPos;

import java.io.IOException;
import java.io.InputStream;

/*
 * wrap ZFInput to Android's InputStream
 * the source ZFInput would be retained until ZFInputWrapper's close(),
 * it's recommended call close() immediately when done
 */
public class ZFInputWrapper extends InputStream {

    public boolean ioSeek(long size, int pos) {
        if (this.zfjniPointerOwnerZFInput == 0) {
            return false;
        } else {
            return native_nativeInputSeek(this.zfjniPointerOwnerZFInput, size, pos);
        }
    }

    public long ioTell() {
        if (this.zfjniPointerOwnerZFInput == 0) {
            return -1;
        } else {
            return native_nativeInputTell(this.zfjniPointerOwnerZFInput);
        }
    }

    public long ioSize() {
        if (this.zfjniPointerOwnerZFInput == 0) {
            return -1;
        } else {
            return native_nativeInputSize(this.zfjniPointerOwnerZFInput);
        }
    }

    // ============================================================
    public long zfjniPointerOwnerZFInput = 0;
    public boolean seekable = false;

    public static Object native_nativeInputCreate(long zfjniPointerOwnerZFInput, boolean seekable) {
        ZFInputWrapper input = new ZFInputWrapper();
        input.zfjniPointerOwnerZFInput = zfjniPointerOwnerZFInput;
        input.seekable = seekable;
        return input;
    }

    public static native long native_nativeInputRead(long zfjniPointerOwnerZFInput, byte[] buf, long offset, long size);

    public static native void native_nativeInputClose(long zfjniPointerOwnerZFInput);

    // pos: ZFSeekPos
    public static native boolean native_nativeInputSeek(long zfjniPointerOwnerZFInput, long size, int pos);

    public static native long native_nativeInputTell(long zfjniPointerOwnerZFInput);

    public static native long native_nativeInputSize(long zfjniPointerOwnerZFInput);

    // ============================================================
    private final byte[] _readBuf = new byte[1];

    @Override
    public int read() throws IOException {
        if (this.zfjniPointerOwnerZFInput == 0) {
            throw new IOException("invalid input");
        }
        if (native_nativeInputRead(this.zfjniPointerOwnerZFInput, _readBuf, 0, 1) == 1) {
            return _readBuf[0];
        } else {
            return -1;
        }
    }

    @Override
    public int read(byte[] b) throws IOException {
        return read(b, 0, b.length);
    }

    @Override
    public int read(byte[] b, int off, int len) throws IOException {
        if (b == null) {
            throw new NullPointerException();
        } else if (off < 0 || len < 0 || len > b.length - off) {
            throw new IndexOutOfBoundsException();
        } else if (len == 0) {
            return 0;
        } else if (this.zfjniPointerOwnerZFInput == 0) {
            throw new IOException("invalid input");
        }

        long read = native_nativeInputRead(this.zfjniPointerOwnerZFInput, b, off, len);
        return read == 0 ? -1 : (int) read;
    }

    private byte[] _skipBuf = null;
    private long _skipBufLen = 0;

    @Override
    public long skip(long n) throws IOException {
        if (this.zfjniPointerOwnerZFInput == 0) {
            throw new IOException("invalid input");
        }
        if (n <= 0) {
            return 0;
        }
        if (n > _skipBufLen) {
            _skipBufLen = (long) ((n / 64 + 1) * 64);
            _skipBuf = new byte[(int) _skipBufLen];
        }
        return native_nativeInputRead(this.zfjniPointerOwnerZFInput, _skipBuf, 0, n);
    }

    @Override
    public int available() throws IOException {
        if (this.zfjniPointerOwnerZFInput == 0) {
            throw new IOException("invalid input");
        }
        return 0;
    }

    @Override
    public void close() throws IOException {
        if (this.zfjniPointerOwnerZFInput != 0) {
            native_nativeInputClose(this.zfjniPointerOwnerZFInput);
            this.zfjniPointerOwnerZFInput = 0;
        }
    }

    private long _markPos = -1;

    @Override
    public synchronized void mark(int readlimit) {
        if (!seekable) {
            _markPos = -1;
            return;
        }
        _markPos = native_nativeInputTell(this.zfjniPointerOwnerZFInput);
    }

    @Override
    public synchronized void reset() throws IOException {
        if (!seekable) {
            throw new IOException("mark/reset not supported");
        }
        if (_markPos != -1) {
            native_nativeInputSeek(this.zfjniPointerOwnerZFInput, _markPos, ZFSeekPos.e_Begin);
            _markPos = -1;
        }
    }

    @Override
    public boolean markSupported() {
        return seekable;
    }

}

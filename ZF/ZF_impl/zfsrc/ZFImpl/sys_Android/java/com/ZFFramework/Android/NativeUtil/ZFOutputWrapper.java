package com.ZFFramework.Android.NativeUtil;

import java.io.IOException;
import java.io.OutputStream;

/*
 * wrap ZFOutput to Android's OutputStream
 * the source ZFOutput would be retained until ZFOutputWrapper's close(),
 * it's recommended call close() immediately when done
 */
public class ZFOutputWrapper extends OutputStream {

    public boolean ioSeek(long size, int pos) {
        if (this.zfjniPointerOwnerZFOutput == 0) {
            return false;
        } else {
            return native_nativeOutputSeek(this.zfjniPointerOwnerZFOutput, size, pos);
        }
    }

    public long ioTell() {
        if (this.zfjniPointerOwnerZFOutput == 0) {
            return -1;
        } else {
            return native_nativeOutputTell(this.zfjniPointerOwnerZFOutput);
        }
    }

    public long ioSize() {
        if (this.zfjniPointerOwnerZFOutput == 0) {
            return -1;
        } else {
            return native_nativeOutputSize(this.zfjniPointerOwnerZFOutput);
        }
    }

    // ============================================================
    public long zfjniPointerOwnerZFOutput = 0;

    public static Object native_nativeOutputCreate(long zfjniPointerOwnerZFOutput) {
        ZFOutputWrapper output = new ZFOutputWrapper();
        output.zfjniPointerOwnerZFOutput = zfjniPointerOwnerZFOutput;
        return output;
    }

    public static native long native_nativeOutputWrite(long zfjniPointerOwnerZFOutput, byte[] buf, long offset, long size);

    public static native void native_nativeOutputClose(long zfjniPointerOwnerZFOutput);

    // pos: ZFSeekPos
    public static native boolean native_nativeOutputSeek(long zfjniPointerOwnerZFOutput, long size, int pos);

    public static native long native_nativeOutputTell(long zfjniPointerOwnerZFOutput);

    public static native long native_nativeOutputSize(long zfjniPointerOwnerZFOutput);

    // ============================================================

    /**
     * Writes the specified byte to this output stream. The general
     * contract for <code>write</code> is that one byte is written
     * to the output stream. The byte to be written is the eight
     * low-order bits of the argument <code>b</code>. The 24
     * high-order bits of <code>b</code> are ignored.
     * <p>
     * Subclasses of <code>OutputStream</code> must provide an
     * implementation for this method.
     *
     * @param b the <code>byte</code>.
     * @throws IOException if an I/O error occurs. In particular,
     * an <code>IOException</code> may be thrown if the
     * output stream has been closed.
     */
    private final byte[] _writeBuf = new byte[1];

    @Override
    public void write(int b) throws IOException {
        if (this.zfjniPointerOwnerZFOutput == 0) {
            throw new IOException("invalid output");
        }
        this._writeBuf[0] = (byte) (b & 0xFF);
        if (native_nativeOutputWrite(this.zfjniPointerOwnerZFOutput, this._writeBuf, 0, 1) != 1) {
            throw new IOException("unable to write 1 byte to output");
        }
    }

    @Override
    public void write(byte[] b) throws IOException {
        write(b, 0, b.length);
    }

    @Override
    public void write(byte[] b, int off, int len) throws IOException {
        if (b == null) {
            throw new NullPointerException();
        } else if ((off < 0) || (off > b.length) || (len < 0) ||
                ((off + len) > b.length) || ((off + len) < 0)) {
            throw new IndexOutOfBoundsException();
        } else if (len == 0) {
            return;
        } else if (this.zfjniPointerOwnerZFOutput == 0) {
            throw new IOException("invalid output");
        }
        if (native_nativeOutputWrite(this.zfjniPointerOwnerZFOutput, b, off, len) != len) {
            throw new IOException(String.format("unable to write %s bytes to output", len));
        }
    }

    @Override
    public void flush() throws IOException {
    }

    @Override
    public void close() throws IOException {
        if (this.zfjniPointerOwnerZFOutput != 0) {
            native_nativeOutputClose(this.zfjniPointerOwnerZFOutput);
            this.zfjniPointerOwnerZFOutput = 0;
        }
    }

}

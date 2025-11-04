package com.ZFFramework.ZFUIKit_impl;

import android.media.MediaDataSource;
import android.media.MediaPlayer;
import android.os.Build;

import com.ZFFramework.NativeUtil.ZFAndroidTaskQueue;
import com.ZFFramework.NativeUtil.ZFInputWrapper;
import com.ZFFramework.ZF_impl.ZFSeekPos;

import java.io.IOException;

public final class ZFAudio {

    public long zfjniPointerOwnerZFAudio = 0;
    public MediaPlayer mp = null;

    // ============================================================
    public static Object native_nativeAudioCreate(long zfjniPointerOwnerZFAudio) {
        ZFAudio nativeAudio = new ZFAudio();
        nativeAudio.zfjniPointerOwnerZFAudio = zfjniPointerOwnerZFAudio;
        return nativeAudio;
    }

    public static void native_nativeAudioDestroy(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        nativeAudioTmp.zfjniPointerOwnerZFAudio = 0;
        nativeAudioTmp._mpDetach();
    }

    public static void native_nativeAudioLoad(Object nativeAudio, Object nativeInput) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            native_notifyAudioOnLoad(
                    nativeAudioTmp.zfjniPointerOwnerZFAudio,
                    false,
                    "load from input not supported for current system version");
            return;
        }
        nativeAudioTmp._mpAttach();
        int mpTaskId = nativeAudioTmp._mpTaskId;
        ZFInputWrapper input = (ZFInputWrapper) nativeInput;
        ZFAndroidTaskQueue.instance().run(new Runnable() {
            @Override
            public void run() {
                if (mpTaskId != nativeAudioTmp._mpTaskId) {
                    return;
                }
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                    MediaDataSource src = new MediaDataSource() {
                        @Override
                        public int readAt(long position, byte[] buffer, int offset, int size) throws IOException {
                            input.ioSeek(position, ZFSeekPos.e_Begin);
                            return input.read(buffer, offset, size);
                        }

                        @Override
                        public long getSize() throws IOException {
                            return input.ioSize();
                        }

                        @Override
                        public void close() throws IOException {
                            input.close();
                        }
                    };
                    if (mpTaskId == nativeAudioTmp._mpTaskId) {
                        try {
                            nativeAudioTmp.mp.setDataSource(src);
                        } catch (Exception e) {
                            if (mpTaskId == nativeAudioTmp._mpTaskId) {
                                native_notifyAudioOnLoad(nativeAudioTmp.zfjniPointerOwnerZFAudio, false, e.toString());
                            }
                            return;
                        }
                    }
                    if (mpTaskId == nativeAudioTmp._mpTaskId) {
                        try {
                            nativeAudioTmp.mp.prepare();
                        } catch (Exception e) {
                            if (mpTaskId == nativeAudioTmp._mpTaskId) {
                                native_notifyAudioOnLoad(nativeAudioTmp.zfjniPointerOwnerZFAudio, false, e.toString());
                            }
                            return;
                        }
                    }
                    if (mpTaskId == nativeAudioTmp._mpTaskId) {
                        native_notifyAudioOnLoad(nativeAudioTmp.zfjniPointerOwnerZFAudio, true, null);
                    }
                }
            }
        });
    }

    public static void native_nativeAudioLoadCancel(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        nativeAudioTmp._mpDetach();
    }

    public static void native_nativeAudioStart(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        try {
            nativeAudioTmp.mp.start();
        } catch (Exception e) {
            native_notifyAudioOnStop(nativeAudioTmp.zfjniPointerOwnerZFAudio, false, e.toString());
            return;
        }
        native_notifyAudioOnResume(nativeAudioTmp.zfjniPointerOwnerZFAudio);
    }

    public static void native_nativeAudioStop(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        if (nativeAudioTmp.mp != null) {
            try {
                nativeAudioTmp.mp.pause();
                nativeAudioTmp.mp.seekTo(0);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static void native_nativeAudioResume(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        try {
            nativeAudioTmp.mp.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
        native_notifyAudioOnResume(nativeAudioTmp.zfjniPointerOwnerZFAudio);
    }

    public static void native_nativeAudioPause(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        try {
            nativeAudioTmp.mp.pause();
        } catch (Exception e) {
            e.printStackTrace();
        }
        native_notifyAudioOnPause(nativeAudioTmp.zfjniPointerOwnerZFAudio);
    }

    public static long native_nativeAudioDuration(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        if (nativeAudioTmp.mp != null) {
            try {
                return nativeAudioTmp.mp.getDuration();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 0;
    }

    public static long native_nativeAudioPosition(Object nativeAudio) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        if (nativeAudioTmp.mp != null) {
            try {
                return nativeAudioTmp.mp.getCurrentPosition();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 0;
    }

    public static void native_nativeAudioPosition(Object nativeAudio, long position) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        try {
            nativeAudioTmp.mp.seekTo((int) position);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void native_nativeAudioVolume(Object nativeAudio, float volume) {
        ZFAudio nativeAudioTmp = (ZFAudio) nativeAudio;
        try {
            nativeAudioTmp.mp.setVolume(volume, volume);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static native void native_notifyAudioOnLoad(long zfjniPointerOwnerZFAudio,
                                                       boolean success,
                                                       String errorHint);

    public static native void native_notifyAudioOnStop(long zfjniPointerOwnerZFAudio,
                                                       boolean success,
                                                       String errorHint);

    public static native void native_notifyAudioOnResume(long zfjniPointerOwnerZFAudio);

    public static native void native_notifyAudioOnPause(long zfjniPointerOwnerZFAudio);

    // ============================================================
    private int _mpTaskId = 0;

    private void _mpAttach() {
        _mpDetach();
        mp = new MediaPlayer();
        mp.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                native_notifyAudioOnStop(zfjniPointerOwnerZFAudio, true, null);
            }
        });
        mp.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                native_notifyAudioOnStop(zfjniPointerOwnerZFAudio, false, String.format("play failed (%s %s)", what, extra));
                return true;
            }
        });
    }

    private void _mpDetach() {
        ++_mpTaskId;
        if (mp != null) {
            mp.setOnCompletionListener(null);
            mp.setOnErrorListener(null);
            ZFAndroidTaskQueue.instance().run(new Runnable() {
                @Override
                public void run() {
                    try {
                        mp.stop();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    try {
                        mp.reset();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    try {
                        mp.release();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        }
        mp = null;
    }

}

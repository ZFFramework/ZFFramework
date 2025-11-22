
package com.ZFFramework.ZFUIKit_impl;

import android.graphics.Camera;
import android.graphics.Matrix;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.view.animation.Interpolator;
import android.view.animation.Transformation;

import com.ZFFramework.NativeUtil.ZFAndroidLog;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class ZFAniForNative {
    protected static class NativeAnimation extends Animation {
        public NativeAnimation(long zfjniPointerOwnerZFAniForNative) {
            this.setFillEnabled(true);
            this.setFillBefore(true);
            this.setFillAfter(true);
            this.zfjniPointerOwnerZFAniForNative = zfjniPointerOwnerZFAniForNative;
        }

        public long zfjniPointerOwnerZFAniForNative = 0;

        private NativeAnimationListener _animationListener = null;

        public void nativeAniListener(NativeAnimationListener listener) {
            if (this._animationListener != null) {
                this._animationListener.nativeAniDetach();
            }
            this._animationListener = listener;
            super.setAnimationListener(listener);
        }

        public NativeAnimationListener nativeAniListener() {
            return _animationListener;
        }

        @Deprecated
        @Override
        public void setAnimationListener(AnimationListener listener) {
            super.setAnimationListener(listener);
        }

        // alpha
        public float alphaFrom = 1;
        public float alphaTo = 1;
        // scale
        public float scaleXFrom = 1;
        public float scaleXTo = 1;
        public float scaleYFrom = 1;
        public float scaleYTo = 1;
        public float scaleZFrom = 1;
        public float scaleZTo = 1;
        // translate
        public int translateXFrom = 0;
        public int translateXTo = 0;
        public int translateYFrom = 0;
        public int translateYTo = 0;
        public int translateZFrom = 0;
        public int translateZTo = 0;
        // rotate
        public float rotateXFrom = 0;
        public float rotateXTo = 0;
        public float rotateYFrom = 0;
        public float rotateYTo = 0;
        public float rotateZFrom = 0;
        public float rotateZTo = 0;

        public boolean hasRotate() {
            return (this.rotateXFrom != 0 || this.rotateXTo != 0
                    || this.rotateYFrom != 0 || this.rotateYTo != 0
                    || this.rotateZFrom != 0 || this.rotateZTo != 0);
        }

        public void nativeAniReset() {
            // alpha
            this.alphaFrom = 1;
            this.alphaTo = 1;
            // scale
            this.scaleXFrom = 1;
            this.scaleXTo = 1;
            this.scaleYFrom = 1;
            this.scaleYTo = 1;
            // translate
            this.translateXFrom = 0;
            this.translateXTo = 0;
            this.translateYFrom = 0;
            this.translateYTo = 0;
            // rotate
            this.rotateXFrom = 0;
            this.rotateXTo = 0;
            this.rotateYFrom = 0;
            this.rotateYTo = 0;
            this.rotateZFrom = 0;
            this.rotateZTo = 0;
        }

        @Override
        public void initialize(int width,
                               int height,
                               int parentWidth,
                               int parentHeight) {
            super.initialize(width, height, parentWidth, parentHeight);
            m_width = width;
            m_height = height;
            m_centerX = width / 2;
            m_centerY = height / 2;

            _hasScale = (this.scaleXFrom != 1 || this.scaleXTo != 1
                    || this.scaleYFrom != 1 || this.scaleYTo != 1
                    || this.scaleZFrom != 1 || this.scaleZTo != 1);
            _hasTranslate = (this.translateXFrom != 0 || this.translateXTo != 0
                    || this.translateYFrom != 0 || this.translateYTo != 0
                    || this.translateZFrom != 0 || this.translateZTo != 0);
            _hasRotate = (this.rotateXFrom != 0 || this.rotateXTo != 0
                    || this.rotateYFrom != 0 || this.rotateYTo != 0
                    || this.rotateZFrom != 0 || this.rotateZTo != 0);
        }

        @Override
        protected void applyTransformation(float interpolatedTime, Transformation t) {
            if (this._animationListener == null) {
                return;
            }

            Matrix matrix = t.getMatrix();

            // fix coordinate axis
            matrix.postTranslate(-m_centerX, -m_centerY);

            // scale
            if (_hasScale) {
                matrix.postScale(
                        this.scaleXFrom + (this.scaleXTo - this.scaleXFrom) * interpolatedTime,
                        this.scaleYFrom + (this.scaleYTo - this.scaleYFrom) * interpolatedTime);
            }

            // rotate
            if (_hasRotate) {
                m_camera.save();
                if (this.rotateXFrom != 0 || this.rotateXTo != 0) {
                    m_camera.rotateX(this.rotateXFrom + (this.rotateXTo - this.rotateXFrom) * interpolatedTime);
                }
                if (this.rotateYFrom != 0 || this.rotateYTo != 0) {
                    m_camera.rotateY(-(this.rotateYFrom + (this.rotateYTo - this.rotateYFrom) * interpolatedTime));
                }
                if (this.rotateZFrom != 0 || this.rotateZTo != 0) {
                    m_camera.rotateZ(-(this.rotateZFrom + (this.rotateZTo - this.rotateZFrom) * interpolatedTime));
                }
                m_camera.getMatrix(m_matrix);
                m_camera.restore();
                matrix.postConcat(m_matrix);
            }

            // translate
            if (_hasTranslate) {
                matrix.postTranslate(
                        (this.translateXFrom + (this.translateXTo - this.translateXFrom) * interpolatedTime),
                        (this.translateYFrom + (this.translateYTo - this.translateYFrom) * interpolatedTime));
            }

            // alpha
            if (this.alphaFrom != this.alphaTo) {
                t.setAlpha(t.getAlpha() * (this.alphaFrom + (this.alphaTo - this.alphaFrom) * interpolatedTime));
            }

            // restore coordinate axis
            matrix.postTranslate(m_centerX, m_centerY);
        }

        private int m_width = 0;
        private int m_height = 0;
        private int m_centerX = 0;
        private int m_centerY = 0;
        private boolean _hasScale = false;
        private boolean _hasTranslate = false;
        private boolean _hasRotate = false;
        private static Matrix m_matrix = new Matrix();
        private static Camera m_camera = new Camera();
    }

    protected static class NativeAnimationListener implements AnimationListener, View.OnAttachStateChangeListener {
        private NativeAnimation _owner = null;
        private View _ownerView = null;

        public NativeAnimationListener(NativeAnimation owner, View ownerView) {
            this._owner = owner;
            this._ownerView = ownerView;
            this._ownerView.addOnAttachStateChangeListener(this);
        }

        public void nativeAniDetach() {
            if (this._owner != null) {
                View ownerViewTmp = this._ownerView;
                this._owner = null;
                this._ownerView = null;
                ownerViewTmp.clearAnimation();
                ownerViewTmp.removeOnAttachStateChangeListener(this);
            }
        }

        @Override
        public void onAnimationStart(Animation animation) {
        }

        private static Handler _stopDelay = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                ((NativeAnimationListener) msg.obj).doStop();
            }
        };

        @Override
        public void onAnimationEnd(Animation animation) {
            _stopDelay.sendMessageDelayed(Message.obtain(_stopDelay, 0, this), 0);
        }

        @Override
        public void onAnimationRepeat(Animation animation) {
        }

        @Override
        public void onViewAttachedToWindow(View v) {
        }

        @Override
        public void onViewDetachedFromWindow(View v) {
            this.doStop();
        }

        private void doStop() {
            if (this._owner != null) {
                long ownerTmp = this._owner.zfjniPointerOwnerZFAniForNative;
                this.nativeAniDetach();
                ZFAniForNative.native_notifyAniStop(ownerTmp);
            }
        }
    }

    private static Map<View, List<NativeAnimation>> _anis = new HashMap<View, List<NativeAnimation>>();

    public static Object native_nativeAniCreate(long zfjniPointerOwnerZFAniForNative) {
        return new NativeAnimation(zfjniPointerOwnerZFAniForNative);
    }

    public static void native_nativeAniDestroy(Object nativeAni) {
    }

    public static void native_nativeAniStart(Object nativeAni,
                                             Object nativeView) {
        NativeAnimation nativeAniTmp = (NativeAnimation) nativeAni;
        View nativeViewTmp = (View) nativeView;
        nativeAniTmp.nativeAniListener(new NativeAnimationListener(nativeAniTmp, nativeViewTmp));

        AnimationSet as = null;
        boolean asNeedStart = false;
        if (nativeViewTmp.getAnimation() != null && nativeViewTmp.getAnimation() instanceof AnimationSet) {
            as = (AnimationSet) nativeViewTmp.getAnimation();
        } else {
            as = new AnimationSet(false);
            asNeedStart = true;
        }

        List<NativeAnimation> attached = _anis.get(nativeViewTmp);
        if (attached == null) {
            attached = new ArrayList<NativeAnimation>();
            _anis.put(nativeViewTmp, attached);
        }
        attached.add(nativeAniTmp);
        as.addAnimation(nativeAniTmp);

        if (asNeedStart) {
            nativeViewTmp.startAnimation(as);
        }

        // rotate must be placed at head
        // otherwise, rotation after scale/translate would have wrong center axis
        ArrayList<Animation> al = (ArrayList<Animation>) as.getAnimations();
        if (al.size() > 1) {
            for (int i = 0; i < al.size(); ++i) {
                if (al.get(i) instanceof NativeAnimation
                        && ((NativeAnimation) al.get(i)).hasRotate()) {
                    _anisCache.add(al.remove(i));
                    --i;
                }
            }
            al.addAll(0, _anisCache);
            _anisCache.clear();
        }
    }

    private static ArrayList<Animation> _anisCache = new ArrayList<Animation>();

    public static void native_nativeAniStop(Object nativeAni,
                                            Object nativeView) {
        NativeAnimation nativeAniTmp = (NativeAnimation) nativeAni;
        nativeAniTmp.nativeAniListener(null);
        View nativeViewTmp = (View) nativeView;

        List<NativeAnimation> attached = _anis.get(nativeViewTmp);
        if (attached == null) {
            nativeViewTmp.setAnimation(null);
            return;
        }
        attached.remove(nativeAniTmp);
        if (attached.isEmpty()) {
            _anis.remove(nativeViewTmp);
            nativeViewTmp.setAnimation(null);
        }
    }

    private static native void native_notifyAniStop(long zfjniPointerOwnerZFAniForNative);

    private static _CurveLinear _curveLinear = new _CurveLinear();
    private static _CurveEaseIn _curveEaseIn = new _CurveEaseIn();
    private static _CurveEaseOut _curveEaseOut = new _CurveEaseOut();
    private static _CurveEaseInOut _curveEaseInOut = new _CurveEaseInOut();

    public static void native_setup(
            Object nativeAni
            , int nativeCurve
            , int duration
    ) {
        NativeAnimation nativeAniTmp = (NativeAnimation) nativeAni;
        nativeAniTmp.nativeAniReset();

        if (nativeCurve == ZFAniForNativeCurve.e_Linear) {
            nativeAniTmp.setInterpolator(_curveLinear);
        } else if (nativeCurve == ZFAniForNativeCurve.e_EaseInOut) {
            nativeAniTmp.setInterpolator(_curveEaseInOut);
        } else if (nativeCurve == ZFAniForNativeCurve.e_EaseIn) {
            nativeAniTmp.setInterpolator(_curveEaseIn);
        } else if (nativeCurve == ZFAniForNativeCurve.e_EaseOut) {
            nativeAniTmp.setInterpolator(_curveEaseOut);
        } else {
            ZFAndroidLog.shouldNotGoHere();
        }

        nativeAniTmp.setDuration(duration);
    }

    // ============================================================
    private static float _pow(float x, int n) {
        /*
            float ret = x;
            for(int i = 1; i < n; ++i) {
                ret *= x;
            }
            return ret;
        */
        return (float) Math.pow(x, n);
    }

    private static float _curve(float x,
                                float y0,
                                float y1,
                                float y2,
                                float y3,
                                float y4,
                                float y5,
                                float y6) {
        return (float) (0
                + y0 * _pow(1 - x, 6)
                + 6 * y1 * x * _pow(1 - x, 5)
                + 15 * y2 * _pow(x, 2) * _pow(1 - x, 4)
                + 20 * y3 * _pow(x, 3) * _pow(1 - x, 3)
                + 15 * y4 * _pow(x, 4) * _pow(1 - x, 2)
                + 6 * y5 * _pow(x, 5) * (1 - x)
                + y6 * _pow(x, 6)
        );
    }

    private static float _curve(float x,
                                float y0,
                                float y1,
                                float y2,
                                float y3,
                                float y4,
                                float y5,
                                float y6,
                                float y7,
                                float y8,
                                float y9,
                                float y10,
                                float y11,
                                float y12,
                                float y13,
                                float y14,
                                float y15) {
        return (float) (0
                + y0 * _pow(1 - x, 15)
                + 15 * y1 * x * _pow(1 - x, 14)
                + 105 * y2 * _pow(x, 2) * _pow(1 - x, 13)
                + 455 * y3 * _pow(x, 3) * _pow(1 - x, 12)
                + 1365 * y4 * _pow(x, 4) * _pow(1 - x, 11)
                + 3003 * y5 * _pow(x, 5) * _pow(1 - x, 10)
                + 5005 * y6 * _pow(x, 6) * _pow(1 - x, 9)
                + 6435 * y7 * _pow(x, 7) * _pow(1 - x, 8)
                + 6435 * y8 * _pow(x, 8) * _pow(1 - x, 7)
                + 5005 * y9 * _pow(x, 9) * _pow(1 - x, 6)
                + 3003 * y10 * _pow(x, 10) * _pow(1 - x, 5)
                + 1365 * y11 * _pow(x, 11) * _pow(1 - x, 4)
                + 455 * y12 * _pow(x, 12) * _pow(1 - x, 3)
                + 105 * y13 * _pow(x, 13) * _pow(1 - x, 2)
                + 15 * y14 * _pow(x, 14) * (1 - x)
                + y15 * _pow(x, 15)
        );
    }

    // ============================================================
    private static class _CurveLinear implements Interpolator {
        @Override
        public float getInterpolation(float time) {
            return time;
        }
    }

    // ============================================================
    private static class _CurveEaseIn implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.005f;
        private static final float y2 = 0.010f;
        private static final float y3 = 0.015f;
        private static final float y4 = 0.020f;
        private static final float y5 = 0.025f;
        private static final float y6 = 1.000f;

        @Override
        public float getInterpolation(float time) {
            return (float) _curve((float) time, y0, y1, y2, y3, y4, y5, y6);
        }
    }

    // ============================================================
    private static class _CurveEaseOut implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.975f;
        private static final float y2 = 0.980f;
        private static final float y3 = 0.985f;
        private static final float y4 = 0.990f;
        private static final float y5 = 0.995f;
        private static final float y6 = 1.000f;

        @Override
        public float getInterpolation(float time) {
            return (float) _curve((float) time, y0, y1, y2, y3, y4, y5, y6);
        }
    }

    // ============================================================
    private static class _CurveEaseInOut implements Interpolator {
        private static final float y0 = 0.000f;
        private static final float y1 = 0.005f;
        private static final float y2 = 0.010f;
        private static final float y3 = 0.015f;
        private static final float y4 = 0.020f;
        private static final float y5 = 0.025f;
        private static final float y6 = 0.030f;
        private static final float y7 = 0.035f;
        private static final float y8 = 0.965f;
        private static final float y9 = 0.970f;
        private static final float y10 = 0.975f;
        private static final float y11 = 0.980f;
        private static final float y12 = 0.985f;
        private static final float y13 = 0.990f;
        private static final float y14 = 0.995f;
        private static final float y15 = 1.000f;

        @Override
        public float getInterpolation(float time) {
            return (float) _curve((float) time, y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11, y12, y13, y14, y15);
        }
    }
}

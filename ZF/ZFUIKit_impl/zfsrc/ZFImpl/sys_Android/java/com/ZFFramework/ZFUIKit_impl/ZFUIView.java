package com.ZFFramework.ZFUIKit_impl;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.ZFFramework.NativeUtil.ZFAndroidSize;
import com.ZFFramework.NativeUtil.ZFAndroidUI;
import com.ZFFramework.ZF_impl.ZFMainEntry;

public class ZFUIView extends ViewGroup {

    // ============================================================
    public long zfjniPointerOwnerZFUIView = 0;
    public View nativeImplView = null;
    public boolean viewUIEnable = true;
    public boolean viewUIEnableTree = true;
    public int viewFrame_x = 0;
    public int viewFrame_y = 0;
    public int viewFrame_width = 0;
    public int viewFrame_height = 0;
    public int nativeImplViewFrame_x = 0;
    public int nativeImplViewFrame_y = 0;
    public int nativeImplViewFrame_width = 0;
    public int nativeImplViewFrame_height = 0;

    // ============================================================
    private Rect _rectNativeCache = new Rect();

    // ============================================================
    public static void native_nativeViewCacheOnSave(Object nativeView) {
        ((ZFUIView) nativeView).zfjniPointerOwnerZFUIView = 0;
    }

    public static void native_nativeViewCacheOnRestore(Object nativeView, long zfjniPointerOwnerZFUIView) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        nativeViewTmp.zfjniPointerOwnerZFUIView = zfjniPointerOwnerZFUIView;

        nativeViewTmp.setVisibility(View.VISIBLE);
        nativeViewTmp.setAlpha(1);
        nativeViewTmp.viewUIEnable = true;
        nativeViewTmp.viewUIEnableTree = true;
        nativeViewTmp.setClickable(true);
        nativeViewTmp.setBackgroundColor(Color.TRANSPARENT);

        ZFUIViewFocus.native_focusable(nativeView, false);
    }

    public static Object native_nativeViewCreate(long zfjniPointerOwnerZFUIView) {
        ZFUIView ret = new ZFUIView(ZFMainEntry.appContext());
        ret.zfjniPointerOwnerZFUIView = zfjniPointerOwnerZFUIView;
        return ret;
    }

    public static void native_nativeImplView(Object nativeView, Object nativeImplView, int virtualIndex) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        View nativeImplViewOld = nativeViewTmp.nativeImplView;
        View nativeImplViewNew = (View) nativeImplView;
        if (nativeViewTmp.nativeImplView != null) {
            nativeViewTmp.removeView(nativeViewTmp.nativeImplView);
        }
        nativeViewTmp.nativeImplView = nativeImplViewNew;
        if (nativeViewTmp.nativeImplView != null) {
            nativeViewTmp.addView(nativeViewTmp.nativeImplView, virtualIndex);
        }

        ZFUIViewFocus.ZFUIViewImplUpdate(nativeViewTmp, nativeImplViewOld, nativeImplViewNew);
    }

    public static void native_nativeImplViewFrame(
            Object nativeView
            , int nativeImplViewFrame_x
            , int nativeImplViewFrame_y
            , int nativeImplViewFrame_width
            , int nativeImplViewFrame_height
    ) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        nativeViewTmp.nativeImplViewFrame_x = nativeImplViewFrame_x;
        nativeViewTmp.nativeImplViewFrame_y = nativeImplViewFrame_y;
        nativeViewTmp.nativeImplViewFrame_width = nativeImplViewFrame_width;
        nativeViewTmp.nativeImplViewFrame_height = nativeImplViewFrame_height;
        nativeViewTmp.requestLayout();
    }

    public static float native_UIScaleForImpl(Object nativeView) {
        return ZFUIView.native_UIScaleForPixel(nativeView);
    }

    public static float native_UIScaleForPixel(Object nativeView) {
        return ZFAndroidUI.screenDensity(((View) nativeView).getContext());
    }

    public static void native_visible(Object nativeView, boolean visible) {
        ((ZFUIView) nativeView).setVisibility(visible ? View.VISIBLE : View.GONE);
    }

    public static void native_alpha(Object nativeView, float alpha) {
        ((ZFUIView) nativeView).setAlpha(alpha);
    }

    public static void native_viewUIEnable(Object nativeView, boolean viewUIEnable) {
        ((ZFUIView) nativeView).viewUIEnable = viewUIEnable;
    }

    public static void native_viewUIEnableTree(Object nativeView, boolean viewUIEnableTree) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        nativeViewTmp.viewUIEnableTree = viewUIEnableTree;
        nativeViewTmp.setClickable(viewUIEnableTree);
    }

    public static void native_bgColor(Object nativeView, int bgColor) {
        ((ZFUIView) nativeView).setBackgroundColor(bgColor);
    }

    public static void native_child(
            Object nativeView
            , Object nativeChild
            , int virtualIndex
            , int childLayer
            , int childLayerIndex
    ) {
        ((ZFUIView) nativeView).addView((View) nativeChild, virtualIndex);
    }

    public static void native_childRemove(
            Object nativeView
            , int virtualIndex
            , int childLayer
            , int childLayerIndex
    ) {
        ((ZFUIView) nativeView).removeViewAt(virtualIndex);
    }

    public static void native_childRemoveAllForDealloc(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        if (!(nativeViewTmp.getChildCount() == 0
                || (nativeViewTmp.getChildCount() == 1 && nativeViewTmp.nativeImplView != null))) {
            nativeViewTmp.removeAllViews();
            if (nativeViewTmp.nativeImplView != null) {
                nativeViewTmp.addView(nativeViewTmp.nativeImplView);
            }
        }
    }

    public static void native_viewFrame(
            Object nativeView
            , int viewFrame_x
            , int viewFrame_y
            , int viewFrame_width
            , int viewFrame_height
    ) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        nativeViewTmp.viewFrame_x = viewFrame_x;
        nativeViewTmp.viewFrame_y = viewFrame_y;
        nativeViewTmp.viewFrame_width = viewFrame_width;
        nativeViewTmp.viewFrame_height = viewFrame_height;
        nativeViewTmp.requestLayout();
    }

    public static void native_layoutRequest(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView) nativeView;
        nativeViewTmp.requestLayout();
    }

    public static Object native_measureNativeView(
            Object nativeView
            , int maxWidthOrNegative
            , int maxHeightOrNegative
    ) {
        View nativeViewTmp = (View) nativeView;
        nativeViewTmp.measure(
                (maxWidthOrNegative >= 0)
                        ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                        : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
                (maxHeightOrNegative >= 0)
                        ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                        : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));
        return new ZFAndroidSize(nativeViewTmp.getMeasuredWidth(), nativeViewTmp.getMeasuredHeight());
    }

    // ============================================================
    public static native void native_notifyLayoutView(
            long zfjniPointerOwnerZFUIView
            , int rect_x
            , int rect_y
            , int rect_width
            , int rect_height
    );

    public static native void native_notifyUIEvent_mouse(
            long zfjniPointerOwnerZFUIView
            , int mouseId
            , int mouseAction
            , int mousePointX
            , int mousePointY
    );

    public static native boolean native_notifyUIEvent_key(
            long zfjniPointerOwnerZFUIView
            , int keyId
            , int keyAction
            , int keyCode
            , int keyCodeRaw
    );

    // ============================================================
    protected ZFUIView(Context context) {
        super(context);
        this.init(context);
    }

    protected ZFUIView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.init(context);
    }

    protected ZFUIView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        this.init(context);
    }

    private void init(Context context) {
        this.setClipToPadding(false);
        this.setFocusable(false);
        this.setFocusableInTouchMode(false);
    }

    private ZFUIKeyEventUtil _keyEventImpl = new ZFUIKeyEventUtil(new ZFUIKeyEventUtil.Impl() {
        @Override
        public boolean onKey(int keyId, int keyAction, int keyCode, int keyCodeRaw) {
            if (zfjniPointerOwnerZFUIView != 0
                    && ZFUIView.native_notifyUIEvent_key(
                    zfjniPointerOwnerZFUIView,
                    keyId,
                    keyAction,
                    keyCode,
                    keyCodeRaw)
            ) {
                return true;
            }
            return false;
        }
    });

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyDown(keyCode, event)
                || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
        return _keyEventImpl.onKeyMultiple(keyCode, repeatCount, event)
                || super.onKeyMultiple(keyCode, repeatCount, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return _keyEventImpl.onKeyUp(keyCode, event)
                || super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        _keyEventImpl.onKeyCancel();
    }

    @Override
    protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        _keyEventImpl.onKeyCancel();
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        if (!this.viewUIEnableTree) {
            return false;
        }
        if (this.viewUIEnable
                || (ev.getAction() != MotionEvent.ACTION_DOWN && ev.getAction() != MotionEvent.ACTION_POINTER_DOWN)
        ) {
            return super.dispatchTouchEvent(ev);
        }

        int x = (int) ev.getX() + this.getScrollX();
        int y = (int) ev.getY() + this.getScrollY();
        for (int i = 0; i < this.getChildCount(); ++i) {
            View child = this.getChildAt(i);
            if (child != this.nativeImplView
                    && (child.getVisibility() == View.VISIBLE || child.getAnimation() != null)) {
                child.getHitRect(this._rectNativeCache);
                if (this._rectNativeCache.contains(x, y)) {
                    return super.dispatchTouchEvent(ev);
                }
            }
        }
        return false;
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = ZFUIMouseAction.e_Down;
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                action = ZFUIMouseAction.e_Down;
                break;
            case MotionEvent.ACTION_MOVE:
                action = ZFUIMouseAction.e_Move;
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                action = ZFUIMouseAction.e_Up;
                break;
            case MotionEvent.ACTION_CANCEL:
                action = ZFUIMouseAction.e_Cancel;
                break;
            default:
                return true;
        }
        for (int i = 0; i < event.getPointerCount(); ++i) {
            if (this.zfjniPointerOwnerZFUIView != 0) {
                ZFUIView.native_notifyUIEvent_mouse(
                        this.zfjniPointerOwnerZFUIView,
                        event.getPointerId(i),
                        action,
                        (int) event.getX(i),
                        (int) event.getY(i));
            }
        }
        return true;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (this.zfjniPointerOwnerZFUIView != 0
                && this.getParent() != null && (!(this.getParent() instanceof ZFUIView))
        ) {
            int width = MeasureSpec.getSize(widthMeasureSpec);
            int height = MeasureSpec.getSize(heightMeasureSpec);
            ZFUIView.native_notifyLayoutView(
                    this.zfjniPointerOwnerZFUIView,
                    0,
                    0,
                    width,
                    height);
        }
        this.setMeasuredDimension(this.viewFrame_width, this.viewFrame_height);
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if (this.zfjniPointerOwnerZFUIView != 0
                && this.getParent() != null && (!(this.getParent() instanceof ZFUIView))
        ) {
            int width = r - l;
            int height = b - t;
            ZFUIView.native_notifyLayoutView(
                    this.zfjniPointerOwnerZFUIView,
                    l,
                    t,
                    width,
                    height);
        }
        for (int i = 0; i < this.getChildCount(); ++i) {
            View child = this.getChildAt(i);
            if (child == this.nativeImplView) {
                child.measure(
                        MeasureSpec.makeMeasureSpec(this.nativeImplViewFrame_width, MeasureSpec.EXACTLY),
                        MeasureSpec.makeMeasureSpec(this.nativeImplViewFrame_height, MeasureSpec.EXACTLY));
                child.layout(
                        this.nativeImplViewFrame_x,
                        this.nativeImplViewFrame_y,
                        this.nativeImplViewFrame_x + this.nativeImplViewFrame_width,
                        this.nativeImplViewFrame_y + this.nativeImplViewFrame_height);
            } else if (child instanceof ZFUIView) {
                ZFUIView childTmp = (ZFUIView) child;
                child.measure(
                        MeasureSpec.makeMeasureSpec(childTmp.viewFrame_width, MeasureSpec.EXACTLY),
                        MeasureSpec.makeMeasureSpec(childTmp.viewFrame_height, MeasureSpec.EXACTLY));
                childTmp.layout(
                        childTmp.viewFrame_x,
                        childTmp.viewFrame_y,
                        childTmp.viewFrame_x + childTmp.viewFrame_width,
                        childTmp.viewFrame_y + childTmp.viewFrame_height);
            } else {
                // usually should not go here
                child.measure(
                        MeasureSpec.makeMeasureSpec(r - l, MeasureSpec.EXACTLY),
                        MeasureSpec.makeMeasureSpec(b - t, MeasureSpec.EXACTLY));
                child.layout(0, 0, r - l, b - t);
            }
        }
    }
}

package com.ZFFramework.ZFUIKit_impl;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

import com.ZFFramework.NativeUtil.ZFAndroidNinePatch;
import com.ZFFramework.ZF_impl.ZFMainEntry;

public final class ZFUIImageView extends ImageView {
    public static Object native_nativeImageViewCreate() {
        return new ZFUIImageView(ZFMainEntry.appContext());
    }

    public static void native_nativeImageViewDestroy(Object nativeImageView) {
        ((ZFUIImageView) nativeImageView).setImageDrawable(null);
    }

    public static void native_image(Object nativeImageView,
                                    Object nativeImage,
                                    float imageScale,
                                    int ninePatch_left,
                                    int ninePatch_top,
                                    int ninePatch_right,
                                    int ninePatch_bottom) {
        ZFUIImageView nativeImageViewTmp = (ZFUIImageView) nativeImageView;
        nativeImageViewTmp.setImageDrawable((Drawable) nativeImage);
        nativeImageViewTmp._imageScale = imageScale;
        nativeImageViewTmp._ninePatchLeft = ninePatch_left;
        nativeImageViewTmp._ninePatchTop = ninePatch_top;
        nativeImageViewTmp._ninePatchRight = ninePatch_right;
        nativeImageViewTmp._ninePatchBottom = ninePatch_bottom;
        nativeImageViewTmp.invalidate();
    }

    // ============================================================
    private Drawable _image = null;
    private float _imageScale = 1;
    private int _ninePatchLeft = 0;
    private int _ninePatchTop = 0;
    private int _ninePatchRight = 0;
    private int _ninePatchBottom = 0;

    public ZFUIImageView(Context context) {
        super(context);

        this.setScaleType(ScaleType.FIT_XY);
    }

    @Override
    public void setImageDrawable(Drawable drawable) {
        if (drawable != null) {
            drawable = drawable.getConstantState().newDrawable();
        }

        _image = drawable;
        super.setImageDrawable(drawable);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (_image == null) {
            // nothing to do
        } else if (_ninePatchLeft == 0 && _ninePatchTop == 0 && _ninePatchRight == 0 && _ninePatchBottom == 0) {
            super.onDraw(canvas);
        } else {
            ZFAndroidNinePatch.drawNinePatch(
                    ((BitmapDrawable) _image).getBitmap(),
                    _imageScale,
                    _ninePatchLeft, _ninePatchTop, _ninePatchRight, _ninePatchBottom,
                    canvas,
                    this.getWidth(), this.getHeight());
        }
    }
}

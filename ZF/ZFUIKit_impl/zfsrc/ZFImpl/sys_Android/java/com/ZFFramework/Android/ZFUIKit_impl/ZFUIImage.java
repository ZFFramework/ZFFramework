package com.ZFFramework.Android.ZFUIKit_impl;

import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;

import com.ZFFramework.Android.NativeUtil.ZFInputWrapper;
import com.ZFFramework.Android.NativeUtil.ZFOutputWrapper;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;


public final class ZFUIImage {
    public static Object native_nativeImageFromInput(Object nativeInput) {
        ZFInputWrapper input = (ZFInputWrapper) nativeInput;
        Bitmap bitmap = BitmapFactory.decodeStream(input);
        if (bitmap == null) {
            return null;
        } else {
            return new BitmapDrawable(ZFMainEntry.appContext().getResources(), bitmap);
        }
    }

    public static boolean native_nativeImageToOutput(Object nativeImage, Object nativeOutput) {
        BitmapDrawable src = (BitmapDrawable) nativeImage;
        ZFOutputWrapper output = (ZFOutputWrapper) nativeOutput;
        return src.getBitmap().compress(CompressFormat.PNG, 1, output);
    }

    public static Object native_nativeImageCopy(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable) nativeImage;
        Bitmap dst = src.getBitmap().copy(Bitmap.Config.ARGB_8888, true);
        return new BitmapDrawable(ZFMainEntry.appContext().getResources(), dst);
    }

    public static int[] native_nativeImageSize(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable) nativeImage;
        return new int[]{src.getBitmap().getWidth(), src.getBitmap().getHeight()};
    }
}

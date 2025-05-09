package com.ZFFramework.ZFUIWebKit_impl;

import android.content.Context;
import android.graphics.Bitmap;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.ZFFramework.ZF_impl.ZFMainEntry;

import java.lang.ref.WeakReference;

public final class ZFUIWebView extends WebView {
    public static class ZFUIWebViewWebClient extends WebViewClient {
        private WeakReference<ZFUIWebView> _owner = null;

        public ZFUIWebViewWebClient(ZFUIWebView owner) {
            _owner = new WeakReference<ZFUIWebView>(owner);
        }

        @Override
        public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
            if (_owner.get()._ZFP_loading) {
                _owner.get()._ZFP_webRedirect = true;
            }
            _owner.get()._ZFP_loading = true;
            return super.shouldOverrideUrlLoading(view, request);
        }

        @Override
        public void onPageStarted(WebView view, String url, Bitmap facIcon) {
            _owner.get()._ZFP_loading = true;
            if (_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView != 0) {
                ZFUIWebView.native_notifyWebLoadStateOnUpdate(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView);
            }
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            if (!_owner.get()._ZFP_webRedirect) {
                _owner.get()._ZFP_loading = false;
            }

            if (!_owner.get()._ZFP_loading && !_owner.get()._ZFP_webRedirect) {
                if (_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView != 0) {
                    ZFUIWebView.native_notifyWebLoadStateOnUpdate(_owner.get()._ZFP_zfjniPointerOwnerZFUIWebView);
                }
            } else {
                _owner.get()._ZFP_webRedirect = false;
            }
        }
    }

    public static Object native_nativeWebViewCreate(long zfjniPointerOwnerZFUIWebView) {
        ZFUIWebView nativeWebView = new ZFUIWebView(ZFMainEntry.appContext());
        nativeWebView._ZFP_zfjniPointerOwnerZFUIWebView = zfjniPointerOwnerZFUIWebView;
        return nativeWebView;
    }

    public static void native_nativeWebViewDestroy(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.setWebViewClient(null);
        nativeWebViewTmp._ZFP_zfjniPointerOwnerZFUIWebView = 0;
    }

    public static void native_loadUrl(Object nativeWebView, Object url) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.loadUrl((String) url);
    }

    public static void native_loadHtml(Object nativeWebView, Object html, Object baseUrl) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.loadDataWithBaseURL((String) baseUrl, (String) html, null, "utf-8", null);
    }

    public static void native_reload(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.reload();
    }

    public static void native_loadStop(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.stopLoading();
    }

    public static void native_goBack(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.goBack();
    }

    public static void native_goForward(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        nativeWebViewTmp.goForward();
    }

    public static boolean native_loading(Object nativeWebView) {
        ZFUIWebView nativeWebViewTmp = (ZFUIWebView) nativeWebView;
        return nativeWebViewTmp._ZFP_loading;
    }

    // ============================================================
    public static native void native_notifyWebLoadStateOnUpdate(long zfjniPointerOwnerZFUIWebView);

    // ============================================================
    public long _ZFP_zfjniPointerOwnerZFUIWebView = 0;
    public boolean _ZFP_loading = false;
    public boolean _ZFP_webRedirect = false;

    public ZFUIWebView(Context context) {
        super(context);
        this.setWebViewClient(new ZFUIWebViewWebClient(this));
    }
}

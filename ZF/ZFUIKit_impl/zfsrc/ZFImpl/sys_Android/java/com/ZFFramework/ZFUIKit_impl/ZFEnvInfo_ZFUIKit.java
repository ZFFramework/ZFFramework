package com.ZFFramework.ZFUIKit_impl;

import com.ZFFramework.ZF_impl.ZFMainEntry;

import java.util.Locale;

public class ZFEnvInfo_ZFUIKit {

    public static String native_localeId() {
        return _locale().getCountry();
    }

    public static String native_localeLangId() {
        return _locale().getLanguage();
    }

    private static Locale _locale() {
        return ZFMainEntry.appContext().getResources().getConfiguration().getLocales().get(0);
    }

}

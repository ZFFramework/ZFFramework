#include "ZFUIPageAni.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfbool _ZFP_ZFUIPageAniCheck(ZF_IN ZFUIPage *page, ZFEnum *resumeOrPauseReason, ZFUIPage *siblingPage) {
    if(zffalse
            || (page == zfnull && siblingPage == zfnull)
            || resumeOrPauseReason == zfnull
            ) {
        return zffalse;
    }
    if(resumeOrPauseReason->classData()->classIsTypeOf(v_ZFUIPageResumeReason::ClassData())) {
        return resumeOrPauseReason->enumValue() != v_ZFUIPageResumeReason::e_ByManagerResume;
    }
    else if(resumeOrPauseReason->classData()->classIsTypeOf(v_ZFUIPagePauseReason::ClassData())) {
        return resumeOrPauseReason->enumValue() != v_ZFUIPagePauseReason::e_ByManagerPause;
    }
    else {
        return zffalse;
    }
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForAlpha
        , ZFMP_IN(ZFUIPage *, page)
        , ZFMP_IN(ZFEnum *, resumeOrPauseReason)
        , ZFMP_IN(ZFUIPage *, siblingPage)
        ) {
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    ZFUIPage *alphaInPage = zfnull;
    ZFUIPage *alphaOutPage = zfnull;
    if(resumeOrPauseReason->classData()->classIsTypeOf(v_ZFUIPageResumeReason::ClassData())) {
        alphaInPage = page;
        alphaOutPage = siblingPage;
    }
    else {
        alphaInPage = siblingPage;
        alphaOutPage = page;
    }

    if(alphaInPage != zfnull) {
        alphaInPage->pageAni(ZFAni("alpha", zfobj<v_zffloat>(0), zfobj<v_zffloat>(1)));
    }
    if(alphaOutPage != zfnull) {
        alphaOutPage->pageAni(ZFAni("alpha", zfobj<v_zffloat>(1), zfobj<v_zffloat>(0)));
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForSlide
        , ZFMP_IN(ZFUIPage *, page)
        , ZFMP_IN(ZFEnum *, resumeOrPauseReason)
        , ZFMP_IN(ZFUIPage *, siblingPage)
        ) {
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    if(resumeOrPauseReason->classData()->classIsTypeOf(v_ZFUIPageResumeReason::ClassData())) {
        if(resumeOrPauseReason->enumValue() == v_ZFUIPageResumeReason::e_ByRequest) {
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(target->width() * (1 - progress));
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(-target->width() * progress);
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
        }
        else { // v_ZFUIPageResumeReason::e_FromBackground
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(-target->width() * (1 - progress));
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(target->width() * progress);
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
        }
    }
    else {
        if(resumeOrPauseReason->enumValue() == v_ZFUIPagePauseReason::e_ToBackground) {
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(target->width() * (1 - progress));
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(-target->width() * progress);
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
        }
        else { // v_ZFUIPagePauseReason::e_BeforeDestroy
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(-target->width() * (1 - progress));
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateX(target->width() * progress);
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
        }
    }
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFUIPageAniPrepareForPopup
        , ZFMP_IN(ZFUIPage *, page)
        , ZFMP_IN(ZFEnum *, resumeOrPauseReason)
        , ZFMP_IN(ZFUIPage *, siblingPage)
        ) {
    if(!_ZFP_ZFUIPageAniCheck(page, resumeOrPauseReason, siblingPage)) {return zffalse;}

    if(resumeOrPauseReason->classData()->classIsTypeOf(v_ZFUIPageResumeReason::ClassData())) {
        if(resumeOrPauseReason->enumValue() == v_ZFUIPageResumeReason::e_ByRequest) {
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateY(-target->height() * (1 - progress));
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(siblingPage != zfnull) {
                zfobj<ZFAnimation> requestOutAni;
                siblingPage->pageAni(requestOutAni);
            }
        }
        else { // v_ZFUIPageResumeReason::e_FromBackground
            if(page != zfnull) {
                zfobj<ZFAnimation> resumeInAni;
                page->pageAni(resumeInAni);
            }
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateY(target->height() * progress);
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseIn>()));
            }
        }
    }
    else {
        if(resumeOrPauseReason->enumValue() == v_ZFUIPagePauseReason::e_ToBackground) {
            if(siblingPage != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateY(-target->height() * (1 - progress));
                } ZFLISTENER_END()
                siblingPage->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseOut>()));
            }
            if(page != zfnull) {
                zfobj<ZFAnimation> requestOutAni;
                page->pageAni(requestOutAni);
            }
        }
        else { // v_ZFUIPagePauseReason::e_BeforeDestroy
            if(siblingPage != zfnull) {
                zfobj<ZFAnimation> resumeInAni;
                siblingPage->pageAni(resumeInAni);
            }
            if(page != zfnull) {
                ZFLISTENER(impl) {
                    zffloat progress = zfargs.param0().to<v_zffloat *>()->zfv;
                    ZFAnimation *ani = zfargs.sender();
                    ZFUIView *target = ani->target();
                    target->translateY(target->height() * progress);
                } ZFLISTENER_END()
                page->pageAni(ZFAni(impl)->c_curve(zfobj<ZFCurveEaseIn>()));
            }
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


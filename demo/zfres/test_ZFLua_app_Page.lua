
return function(zfargs)
    local pm = zfargs:param0();

    pm:observerAdd(ZFUIPage.EventPageOnResume(), function(zfargs)
            local page = zfargs:sender();
            local pm = page:pageManager();
            pm:objectTag("leftButton"):viewVisible(pm:pageCount() > 1);
            pm:objectTag("centerButton"):label():text(page:objectInfoOfInstance());
        end);

    _ZFP_ZFLua_app_test_setupPage = function(page)
            page:observerAdd(ZFUIPage.EventPageOnCreate(), function(zfargs)
                    zfLog("page onCreate");
                    local page = zfargs:sender();

                    local pageView = ZFUIButtonBasic();
                    page:pageView():childAdd(pageView):sizeFill();

                    pageView:label():text(page:objectInfoOfInstance());
                    pageView:viewBackgroundColor(ZFUIColorRandom());

                    local pageHolder = page:objectHolder();
                    pageView:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)
                            local page = pageHolder:objectHolded();
                            page:pageManager():pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
                        end);
                end);
            page:observerAdd(ZFUIPage.EventPageAniOnPrepare(), function(zfargs)
                    zfLog("page aniOnPrepare");
                    ZFUIPageAniPrepareForSlide(zfargs:sender(), zfargs:param0(), zfargs:param1());
                end);
            return page;
        end

    pm:pageAniOverride(zfnull, zfnull);
    pm:pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
end


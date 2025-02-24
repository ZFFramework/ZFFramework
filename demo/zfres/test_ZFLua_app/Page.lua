
return function(zfargs)
    ---@type ZFUIPageManager
    local pm = zfargs:param0();

    pm:observerAdd(ZFUIPage.E_PageOnResume(), function(zfargs)
            local page = zfargs:sender();
            local pm = page:pageManager();
            pm:objectTag("leftButton"):visible(pm:pageCount() > 1);
            pm:objectTag("centerButton"):label():text(page:objectInfoOfInstance());
        end);

    _ZFP_ZFLua_app_test_setupPage = function(page)
            page:observerAdd(ZFUIPage.E_PageOnCreate(), function(zfargs)
                    ZFLog("page onCreate");
                    local page = zfargs:sender();

                    local pageView = ZFUIButtonBasic();
                    page:pageView():child(pageView):sizeFill();

                    pageView:label():text(page:objectInfoOfInstance());
                    pageView:bgColor(ZFUIColorRandom());

                    local pageHolder = zfweak(page);
                    pageView:observerAdd(ZFUIButton.E_ButtonOnClick(), function(zfargs)
                            local page = pageHolder:get();
                            page:pageManager():pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
                        end);
                end);
            page:observerAdd(ZFUIPage.E_PageAniOnPrepare(), function(zfargs)
                    ZFLog("page aniOnPrepare");
                    ZFUIPageAniPrepareForSlide(zfargs:sender(), zfargs:param0(), zfargs:param1());
                end);
            return page;
        end

    pm:pageAniOverride(zfnull, zfnull);
    pm:pageCreate(_ZFP_ZFLua_app_test_setupPage(ZFUIPage()));
end


local pm = ZFUIPageManager();

pm:observerAdd(ZFUIPageManager.EventManagerOnCreate(), function(zfargs)
        ZFLog("manager onCreate");
        local pm = zfargs:sender();
        local pmHolder = pm:objectHolder();

        local managerContainer = pm:managerContainer();

        local topView = ZFUILinearLayout();
        pm:objectTag("topView", topView);
        managerContainer:childAdd(topView):widthFill();
        topView:layoutOrientation(ZFUIOrientation.e_Left());

        local leftButton = ZFUIButtonBasic();
        pm:objectTag("leftButton", leftButton);
        topView:childAdd(leftButton):widthFill():alignCenter():layoutWeight(1):reserveSpace(zftrue);
        leftButton:label():text("Back");
        leftButton:viewBackgroundColor(ZFUIColorRandom());
        leftButton:viewVisible(zffalse);

        leftButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)
                local pm = pmHolder:objectHolded();
                pm:pageAt(pm:pageCount() - 1):pageDestroy();
            end);

        local centerButton = ZFUIButtonBasic();
        pm:objectTag("centerButton", centerButton);
        topView:childAdd(centerButton):widthFill():alignCenter():layoutWeight(2);
        centerButton:viewBackgroundColor(ZFUIColorRandom());

        local rightButton = ZFUIButtonBasic();
        pm:objectTag("rightButton", rightButton);
        topView:childAdd(rightButton):widthFill():alignCenter():layoutWeight(1);
        rightButton:label():text("ExitTest");
        rightButton:viewBackgroundColor(ZFUIColorRandom());
        rightButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)
                local pm = pmHolder:objectHolded();
                pm:managerDestroy();
            end);
    end);

return pm;


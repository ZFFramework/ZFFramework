local pm = ZFUIPageManager();

pm:observerAdd(ZFUIPageManager.EventManagerOnCreate(), function(zfargs)
        ZFLog("manager onCreate");
        local pm = zfargs:sender();
        local pmHolder = pm:objectHolder();

        local managerContainer = pm:managerContainer();

        local topView = ZFUILinearLayout();
        pm:objectTag("topView", topView);
        managerContainer:child(topView):widthFill();
        topView:orientation(ZFUIOrientation.e_Left());

        local leftButton = ZFUIButtonBasic();
        pm:objectTag("leftButton", leftButton);
        topView:child(leftButton):widthFill():alignCenter():weight(1):reserveSpace(zftrue);
        leftButton:label():text("Back");
        leftButton:bgColor(ZFUIColorRandom());
        leftButton:visible(zffalse);

        leftButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)
                local pm = pmHolder:objectHolded();
                pm:pageAt(pm:pageCount() - 1):pageDestroy();
            end);

        local centerButton = ZFUIButtonBasic();
        pm:objectTag("centerButton", centerButton);
        topView:child(centerButton):widthFill():alignCenter():weight(2);
        centerButton:bgColor(ZFUIColorRandom());

        local rightButton = ZFUIButtonBasic();
        pm:objectTag("rightButton", rightButton);
        topView:child(rightButton):widthFill():alignCenter():weight(1);
        rightButton:label():text("ExitTest");
        rightButton:bgColor(ZFUIColorRandom());
        rightButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(zfargs)
                local pm = pmHolder:objectHolded();
                pm:managerDestroy();
            end);
    end);

return pm;


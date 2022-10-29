local pm = ZFUIPageManager();

pm:observerAdd(ZFUIPageManager.EventManagerOnCreate(), function(listenerData, userData)
        zfLog("manager onCreate");
        local pm = listenerData:sender();

        local managerContainer = pm:managerContainer();

        local topView = ZFUILinearLayout();
        pm:objectTag("topView", topView);
        managerContainer:childAdd(topView):widthFill();
        topView:layoutOrientation(ZFUIOrientation.e_Left());

        local leftButton = ZFUIButtonBasic();
        pm:objectTag("leftButton", leftButton);
        topView:childAdd(leftButton):widthFill():alignCenter():layoutWeight(1):reserveSpace(zftrue);
        leftButton:buttonLabelText("Back");
        leftButton:viewBackgroundColor(ZFUIColorRandom());
        leftButton:viewVisible(zffalse);
        leftButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:pageAt(pm:pageCount() - 1):pageDestroy();
            end, pm:objectHolder());

        local centerButton = ZFUIButtonBasic();
        pm:objectTag("centerButton", centerButton);
        topView:childAdd(centerButton):widthFill():alignCenter():layoutWeight(2);
        centerButton:viewBackgroundColor(ZFUIColorRandom());

        local rightButton = ZFUIButtonBasic();
        pm:objectTag("rightButton", rightButton);
        topView:childAdd(rightButton):widthFill():alignCenter():layoutWeight(1);
        rightButton:buttonLabelText("ExitTest");
        rightButton:viewBackgroundColor(ZFUIColorRandom());
        rightButton:observerAdd(ZFUIButton.EventButtonOnClick(), function(listenerData, userData)
                local pm = userData:objectHolded();
                pm:managerDestroy();
            end, pm:objectHolder());
    end);

return pm;


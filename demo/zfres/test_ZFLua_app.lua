local pm = zfimport("test_ZFLua_app_PageManager.lua");

pm:managerCreateForWindow();
pm:managerContainer():layoutParam():layoutMargin(ZFUIMarginMake(20));

local pageSetup = zfimport("test_ZFLua_app_Page.lua");
pageSetup:execute(ZFArgs():param0(pm));

return pm;


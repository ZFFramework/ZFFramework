local pm = zfimport("PageManager.lua");

pm:managerCreateForWindow();
pm:managerContainer():layoutParam():margin(ZFUIMarginCreate(20));

local pageSetup = zfimport("Page.lua");
pageSetup:execute(ZFArgs():param0(pm));

return pm;


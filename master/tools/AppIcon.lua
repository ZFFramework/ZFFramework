
local ctx = ZFUIDraw.beginForImage('(128,128)')
ZFUIDraw.drawColor(ctx, '#ff8000')
ZFUIDraw.drawText(ctx, 'ZF', ZFUITextConfig()
    :textColor('#ffffff')
    :textSize(100)
    :textAlign('Center')
    :textAppearance('Bold')
    )
local img = ZFUIDraw.endForImage(ctx)
ZFUIImageToOutput(ZFOutputForPathInfo(ZFLocalPathInfo('../../demo/zfscript/AppIcon.png')), img)


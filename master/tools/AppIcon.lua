
local ctx = ZFUIDraw.beginForImage('(512,512)')
ZFUIDraw.drawColor(ctx, '#ff8000')
ZFUIDraw.drawText(ctx, 'ZF', ZFUITextConfig()
    :textColor('#ffffff')
    :textSize(400)
    :textAlign('Center')
    :textAppearance('Bold')
    )
local img = ZFUIDraw.endForImage(ctx)
ZFUIImageToOutput(ZFOutputForPathInfo(ZFLocalPathInfo('AppIcon.png')), img)


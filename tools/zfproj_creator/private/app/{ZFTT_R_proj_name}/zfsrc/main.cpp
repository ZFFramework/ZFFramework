#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"

static void myTestCode(void);

ZFMAIN_ENTRY()
{
    // TODO: add your code here
    myTestCode();
}

static void myTestCode(void)
{
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    zfblockedAlloc(ZFUITextView, textView);
    window->childAdd(textView)->c_alignCenter();
    textView->text("hello world\n\nhello ZFFramework");
    textView->textSingleLine(zffalse);

    zfLogTrim() << "welcome to ZFFramework";
}


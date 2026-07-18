#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"

static void myTestCode(void);

ZFMAIN_ENTRY() {
    // TODO: add your code here
    myTestCode();
}

static void myTestCode(void) {
    zfobj<ZFUIWindow> window;
    window->show();

    zfobj<ZFUITextView> textView;
    window->child(textView)->c_alignCenter();
    textView->text("hello world\n\nhello ZFFramework");
    textView->singleLine(zffalse);

    ZFLogTrim() << "welcome to ZFFramework";
}


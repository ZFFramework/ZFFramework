
# Introduction

welcome to ZFFramework, a cross-platform and powerful application framework in C++

everything here starts with "ZF", which stands for "Zero Framework"

* it's not a traditional framework, can be loaded like a dynamic library, plug and play
* designed to be able to run at any platform that supplies C++03 compatible implementation


Homepage:

* Online docs: http://ZFFramework.com
* Github repo: https://github.com/ZFFramework/ZFFramework

    * [![Doxygen](https://github.com/ZFFramework/ZFFramework/actions/workflows/doxygen.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/doxygen.yml)
    * [![Android](https://github.com/ZFFramework/ZFFramework/actions/workflows/android.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/android.yml)
    * [![iOS](https://github.com/ZFFramework/ZFFramework/actions/workflows/ios.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/ios.yml)
    * [![Qt Linux](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_linux.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_linux.yml)
    * [![Qt MacOS](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_macos.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_macos.yml)
    * [![Qt Windows](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_windows.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_windows.yml)
    * NOTE: this repo would keep clean (remove unnecessary history) and update frequently,
    if you want stable or history version, please refer to [ZFFrameworkDist](https://github.com/ZFFrameworkDist/ZFFramework)


# Quick overview

## cpp hello world

this piece of code shows how to show a hello world on UI and log output

```cpp
#include "ZFUIWidget.h" // for common UI module
ZFMAIN_ENTRY() // app starts from here
{
    // show a hello world to log output
    zfLogT() << "hello wolrd";

    // show a window (full screen by default)
    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    // show a hello world as a text view
    zfblockedAlloc(ZFUITextView, textView);
    window->childAdd(textView)->c_alignTop()->c_margin(40);
    textView->text("hello world");

    // button and click (as observer)
    zfblockedAlloc(ZFUIButtonBasic, button);
    window->childAdd(button)->c_alignBottom()->c_margin(40);
    button->label()->text("click me");
    ZFLISTENER(onClick) {
        ZFUIButtonBasic *button = userData->objectHolded();
        zfLogTrimT() << "button clicked:" << button;
        zfLogTrimT() << "sender:" << listenerData.sender();
    } ZFLISTENER_END(onClick)
    button->onClick(onClick, button->objectHolder());
}
```


## lua hello world

this piece of code shows equivalent lua code to use ZFFramework,
<b>all the lua bindings are automatically done by reflection!</b>

```lua
zfLog('hello world')

local window = ZFUIWindow()
window:windowShow()

local textView = zfAlloc('ZFUITextView')
window:childAdd(textView):alignTop():margin(40)
textView:text('hello wolrd')

local button = ZFUIButtonBasic.ClassData():newInstance()
window:childAdd(button):alignBottom():margin(40)
button:label():text('click me')
button:onClick(
    function (listenerData, userData)
        zfLog('button clicked: %s', userData:objectHolded())
        zfLog('sender: %s', listenerData:sender())
    end,
    button:objectHolder())
```


## powerful dynamic register

both lua and cpp can dynamic register class and method

```cpp
#include "ZFLua.h"
ZFMAIN_ENTRY()
{
    ZFDynamic()
        .classBegin("MyBaseView", "ZFUIView")
            .method("void", "testFunc", ZFMP()
                .mp("zfstring", "testParam0")
                , ZFListenerForLambda({
                    ZFMethodInvokeData *m = listenerData.param0T();
                    zfLogTrimT() << m->invokerMethod << "called, param0:" << m->param0;
                }))
        .classEnd();

    ZFLuaExecute(
        "ZFDynamic()\n"
        "    :classBegin('MyChildView', 'MyBaseView')\n"
        "        :method('void', 'testFunc', ZFMP()\n"
        "            :mp('zfstring', 'testParam0')\n"
        "            , function(listenerData, userData)\n"
        "                local m = listenerData:param0()\n"
        "                m:callSuper()\n"
        "                zfLogTrim('%s called, param0: %s', m:invokerMethod(), m:param0())\n"
        "            end)\n"
        "    :classEnd()\n"
        "\n"
        "local myView = MyChildView()\n"
        "myView:testFunc('luaParam0')\n"
    );

    zfautoObject obj = ZFClass::classForName("MyChildView")->newInstance();
    obj->invoke("testFunc", zflineAlloc(v_zfstring, "cppParam0"));

    ZFMethodAlias(ZFMethodForName("MyChildView", "testFunc"), "testAliased");
    ZFLuaExecute(
        "local myView = MyChildView()\n"
        "myView:testAliased('luaParam0')\n"
    );
}
```


## powerful abstract IO

chain http file and zip file, and R/W contents in the zip file just like normal local file

```cpp
#include "ZFCore.h"
ZFMAIN_ENTRY()
{
    ZFFileResExtPathAdd("ZFCompress:http:http://192.168.xxx.xxx/xxx.zip|");
    ZFInputReadToOutput(zfLogTrimT(), ZFInputForResFile("path/in/zip/file.txt"));
    ZFFilePathInfoTreePrint(ZFPathInfo("res:"));
}
```


# Getting started

* [Download](https://zfframework.github.io/doc/_doc_tag__download.html) necessary files
* [Setup](https://zfframework.github.io/doc/_doc_tag__setup.html) set up necessary environment for ZFFramework
* [Tutorial](https://zfframework.github.io/doc/_doc_tag__tutorial.html) quick tutorial to code with ZFFramework
* [FAQ](https://zfframework.github.io/doc/_doc_tag__f_a_q.html)


# Detailed

## Requirement

* for the core modlue:

    * C++03 compatible compiler (require templates, no boost/RTTI/exceptions required)
    * STL containers (require: map/unordered_map/vector/deque/list), or supply custom wrapper

* for the implementation module:

    * depends on the actual platform implementation


## Main features

* minimum requirement
* powerful reflection, serialzation, styleable logic

    * for how powerful ZFFramework is, you may refer to [Feature page](https://zfframework.github.io/doc/_doc_tag__feature.html)
    * automatic lua binding, no extra bind code or config are necessary
    * automatic serialization
    * enhanced global event observer and event filter

* fully modularization, "core + protocol + dynamic implementation" design

    support any platform if you are able to supply a native C++ implementation,
    most of implementation can be replaced easily, and implementation is required only if its owner module being used

* easy to communicate with native code

    even to embed UI elements and native UI elements with each other

* UI module to write cross-platform UI easily
* built-in auto scale logic to support multiple screen size

    you have no need to write size-dependent code in both app and implementation


## Current status

* finished
    * core module (memory management, reflection, serialization)
    * basic UI module (view, window, label, image view, button, layout, scroll view, list view)
    * basic algorithm (xml, json, regexp, md5, base64, crc32, encryption, compression)
    * common platform implementations (iOS, Android, Qt)
    * auto lua binding by reflection
* working
    * more useful UI modules
    * basic network module
    * basic database module
* future
    * standalone visual UI editor
    * more IDE / compile env integrations
    * more platform implementations


## What we do

* aiming to be portable and can be ported easily,
    aiming to be lightweighted and able to be embeded easily,
    you may simply drag and drop all src files to your build system
* fully dynamic, extensible


## License

ZFFramework is under MIT license ([see here](https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE)),
feel free to copy or modify or use it

* project home page: [http://ZFFramework.com](http://ZFFramework.com)
* blog: [http://zsaber.com](http://zsaber.com)
* repo: [https://github.com/ZFFramework/ZFFramework](https://github.com/ZFFramework/ZFFramework)

    [Issues](https://github.com/ZFFramework/ZFFramework/issues) or [Pull Request](https://github.com/ZFFramework/ZFFramework/pulls) are welcome


if you like my work, [buy me a coffee](https://github.com/ZSaberLv0/ZSaberLv0)?


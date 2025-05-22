
<!-- vim-markdown-toc GFM -->

* [Introduction](#introduction)
* [Quick overview](#quick-overview)
    * [cpp hello world](#cpp-hello-world)
    * [lua hello world](#lua-hello-world)
    * [dynamic register](#dynamic-register)
    * [automatic serialzation](#automatic-serialzation)
    * [abstract IO](#abstract-io)
* [Getting started](#getting-started)
* [Detailed](#detailed)
    * [Requirement](#requirement)
    * [Main features](#main-features)
    * [License](#license)

<!-- vim-markdown-toc -->

# Introduction

welcome to ZFFramework, a cross-platform and powerful application framework in C++

everything here starts with "ZF", which stands for "Zero Framework"

* it's not a traditional framework, can be loaded like a dynamic library, plug and play
* designed to be able to run at any platform that supplies C++03 compatible implementation


Homepage:

* Online docs: http://ZFFramework.com
* Github repo: https://github.com/ZFFramework/ZFFramework

    [![Doxygen](https://github.com/ZFFramework/ZFFramework/actions/workflows/doxygen.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/doxygen.yml)
    [![Android](https://github.com/ZFFramework/ZFFramework/actions/workflows/android.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/android.yml)
    [![iOS](https://github.com/ZFFramework/ZFFramework/actions/workflows/ios.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/ios.yml)
    [![cmake Linux](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_linux.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_linux.yml)
    [![cmake MacOS](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_macos.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_macos.yml)
    [![cmake Windows](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_windows.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/cmake_windows.yml)
    [![Qt Linux](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_linux.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_linux.yml)
    [![Qt MacOS](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_macos.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_macos.yml)
    [![Qt Windows](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_windows.yml/badge.svg)](https://github.com/ZFFramework/ZFFramework/actions/workflows/qt_windows.yml)

* history version: [ZFFrameworkDist](https://github.com/ZFFrameworkDist/ZFFramework)


# Quick overview

## cpp hello world

this piece of code shows how to show a hello world on UI and log output

```cpp
#include "ZFUIWidget.h" // for common UI module
ZFMAIN_ENTRY() { // app starts from here
    // show a hello world to log output
    ZFLog("hello %s", "world");

    // show a window
    zfobj<ZFUIWindow> window;
    window->show();

    // build view tree
    // (the c_xxx is a util for chained call)
    window
        ->child(zfobj<ZFUITextView>()
            ->c_text(zfstr("hello %s", "world"))
        )->c_alignTop()->c_margin(40)

        ->child(zfobj<ZFUIButtonBasic>()
            ->c_labelProp("text", zfobj<v_zfstring>("click me"))
            ->c_onClick([](const ZFArgs &zfargs) {
                ZFUIButtonBasic *button = zfargs.sender();
                ZFUIHintShow(zfstr("button clicked: %s", button));
            })
        )->c_alignBottom()->c_margin(40)
        ;
}
```


## lua hello world

this piece of code shows equivalent lua code to use ZFFramework,
<b>all the lua bindings are automatically done by reflection!</b>

```lua
ZFLog('hello %s', 'world')

local window = ZFUIWindow()
window:show()

window
    :child(ZFUITextView()
        :text(zfstr('hello %s', 'world'))
    ):alignTop():margin(40)

    :child(ZFUIButtonBasic()
        :labelProp('text', 'click me')
        :onClick(function(a)
                ---@type ZFUIButtonBasic
                local button = a:sender()
                ZFUIHintShow(zfstr('button clicked: %s', button))
            end)
    ):alignBottom():margin(40)
```


## dynamic register

both lua and cpp can dynamic register class and method, even for existing class

```cpp
#include "ZFLua.h"
ZFMAIN_ENTRY() {
    ZFDynamic()
        .classBegin("MyBaseView", "ZFUIView")
            .method("void", "testFunc", ZFMP()
                .mp("zfstring", "testParam0")
                , [](const ZFArgs &zfargs) {
                    ZFLogTrim() << zfargs.ownerMethod() << " called, param0: " << zfargs.param0();
                })
        .classEnd();

    ZFLuaExecute(
        "ZFDynamic()\n"
        "    :classBegin('MyChildView', 'MyBaseView')\n"
        "        :method('void', 'testFunc', ZFMP()\n"
        "            :mp('zfstring', 'testParam0')\n"
        "            , function(zfargs)\n"
        "                zfargs:callSuper()\n"
        "                ZFLogTrim('%s called, param0: %s', zfargs:ownerMethod(), zfargs:param0())\n"
        "            end)\n"
        "    :classEnd()\n"
        "\n"
        "local myView = MyChildView()\n"
        "myView:testFunc('luaParam0')\n"
    );

    zfauto obj = ZFClass::classForName("MyChildView")->newInstance();
    obj->invoke("testFunc", zfobj<v_zfstring>("cppParam0"));

    ZFMethodAlias(ZFMethodForName("MyChildView", "testFunc"), "testAliased");
    ZFLuaExecute(
        "local myView = MyChildView()\n"
        "myView:testAliased('luaParam0')\n"
    );
}
```


## automatic serialzation

```lua
ZFDynamic()
    :classBegin('MyObj', 'ZFStyle')
    :property('zfstring', 'myProp')
    :classEnd()

    :classBegin('MyContainer', 'ZFStyle')
    :property('MyObj', 'myObj', MyObj())
    :property('ZFArray', 'myObjArr', ZFArray())
    :property('ZFMap', 'myObjMap', ZFMap())
    :classEnd()

local obj = MyContainer()
obj:myObj():myProp('123')
obj:myObjArr()
    :add(MyObj():myProp('456'))
    :add(MyObj():myProp('789'))
obj:myObjMap()
    :set('456', MyObj():myProp('456'))
    :set('789', MyObj():myProp('789'))
ZFObjectToXml(ZFOutputForConsole(), obj)
ZFObjectToJson(ZFOutputForConsole(), obj)
```


## abstract IO

example: chain http and zip file, and R/W contents in the zip file just like normal local file

```cpp
#include "ZFCore.h"
ZFMAIN_ENTRY() {
    // before
    ZFPathInfoTreePrint(ZFPathInfo("res:"));

    ZFResExtPathAdd("compress:http:http://192.168.xxx.xxx/xxx.zip|");

    // after
    ZFPathInfoTreePrint(ZFPathInfo("res:"));
    ZFInputRead(ZFLogTrim(), ZFInputForRes("path/in/zip/file.txt"));
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
    most of implementation can be replaced easily and dynamically,
    all implementation is required only if its owner module being used

* easy to communicate with native code

    even to embed UI elements and native UI elements with each other


## License

ZFFramework is under [MIT license](https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE),
feel free to copy or modify or use it

* project home page: [http://ZFFramework.com](http://ZFFramework.com)
* repo: [https://github.com/ZFFramework/ZFFramework](https://github.com/ZFFramework/ZFFramework)

    [Issues](https://github.com/ZFFramework/ZFFramework/issues) or [Pull Request](https://github.com/ZFFramework/ZFFramework/pulls) are welcome


if you like my work, [buy me a coffee](https://github.com/ZSaberLv0/ZSaberLv0)?


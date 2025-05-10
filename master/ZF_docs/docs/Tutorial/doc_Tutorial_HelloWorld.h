/**
 * @page DocTag_Tutorial_HelloWorld Hello world
 *
 * to start a quick hello world:
 * @code
 *   // include ZFCore.h for core function of ZFFramework
 *   // you may need to setup your project's include path first
 *   #include "ZFCore.h"
 *
 *   // declare app's main entry
 *   ZFMAIN_ENTRY() {
 *       // well, hello world to ZFFramework
 *       ZFLog() << "hello world";
 *
 *       // use ZFLog or zfstr and `%s` to print "anything"
 *       ZFLog("hello %s %s", "world", 2);
 *       ZFLog(zfstr("hello %s %.2s", "world", 3.1));
 *   }
 * @endcode
 *
 * ZFFramework would be started from #ZFMAIN_ENTRY,
 * you may write any C++ compatible code here\n
 * in this example,
 * we just output "hello world" to ZFLog,
 * which would output to a proper console by default
 */


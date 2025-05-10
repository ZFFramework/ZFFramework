/**
 * @page DocTag_Tutorial_HelloWorldWithLua Hello world with lua
 *
 * it's recommend to use ZFFramework with lua,
 * nearly most of cpp apis can be called from lua with very similar syntax
 *
 * to use lua:
 * @code
 *   #include "ZFLua.h"
 *   ZFMAIN_ENTRY() {
 *       // invoke lua string
 *       ZFLuaExecute(
 *           "ZFLog('hello %s', 'world')"
 *       );
 *
 *       // invoke lua file
 *       ZFLuaExecute(ZFInputForFile("path_to_lua.lua"));
 *   }
 * @endcode
 *
 * or, you may use ZFLoader for quick test:
 * -# prepare the ZFLoader app by any of these methods
 *   -  download prebuilt binaries : https://github.com/ZFFramework/AppArchive
 *   -  build from source : https://github.com/ZFFramework/ZFLoader
 * -# open the ZFLoader app directly,
 *   you may got a hint of path to put a `zf.lua`
 * -# write your test code to the `zf.lua`,
 *   and reopen the ZFLoader app,
 *   your test code would be run automatically
 */


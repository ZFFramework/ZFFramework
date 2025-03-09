/**
 * @file ZFLuaExecute.h
 * @brief execute lua script
 */

#ifndef _ZFI_ZFLuaExecute_h_
#define _ZFI_ZFLuaExecute_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief main entry for lua module, execute lua code
 *
 * thanks to powerful reflection mechanism in ZFFramework,
 * most of components are registered to lua by reflection automatically\n
 * \n
 * all types of ZFFramework are wrapped as #zfauto in lua world,
 * including: #ZFObject types (wrapped directly)
 * and non-ZFObject normal C++ types (wrapped by #ZFTypeIdWrapper)\n
 * then, all reflectable #ZFMethod supply #ZFMethodGenericInvoker
 * to invoke the method without knowing all actual types\n
 * \n
 * here is a list of functions available in lua to communicate with ZFFramework:
 * -  "zfAlloc("ClassName" [params...])"
 *   or "ClassName([params...])"\n
 *   alloc a ZFObject type\n
 *   "ClassName" can be #v_ZFClass, #v_zfstring, or native lua string\n
 *   if extra init param passed,
 *   your class must supplys reflectable #ZFObject::objectOnInit
 * -  invoker
 *   -  "ret = zfl_call(obj, "functionName", param0, param1, ...)"
 *     or "obj:functionName(param0, param1, ...)"\n
 *     call object's instance method, params are optional\n
 *     for "functionName", see #ZFTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, #v_zfstring, or native lua string,
 *     while other types must exactly match the original types
 *   -  "ret = zfl_call(zfnull, "ClassOrNamespace.methodName", param0, param1, ...)"
 *     or "ret = ClassOrNamespace.methodName(param0, param1, ...)"\n
 *     call global function or static class member method, params are optional\n
 *     for "functionName", see #ZFTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, #v_zfstring, #v_ZFClass, or native lua string,
 *     while other types must exactly match the original types\n
 *     these namespace are considered as the same:
 *     -  #ZF_NAMESPACE_GLOBAL_NAME
 *     -  #ZF_NAMESPACE_GLOBAL_ABBR_NAME
 *     -  empty string
 *   -  while calling functions with lua raw string, such as "func('abc')",
 *     we will try to convert to proper param type by string converter,
 *     if overload functions exists,
 *     conversion may take more than once,
 *     until it matches first function
 * -  ZFEnum
 *   -  "YourEnum.e_YourEnumValue()"\n
 *     access the enum value
 *   -  "YourEnumFlags('YourEnumValue1|YourEnumValue2')"
 *     or "YourEnumFlags(YourEnum.e_YourEnumValue1(), YourEnum.e_YourEnumValue2, ...)"\n
 *     create enum flags
 * -  value holder
 *   -  "value = YourTypeName("yourTypeData")"
 *     or "value = zfAlloc("YourTypeName")"
 *     or "value = YourTypeName()"\n
 *     create a non-ZFObject type registered by #ZFTYPEID_DECLARE,
 *     return the associated "YourTypeName" that holds the value\n
 *     "YourTypeName" represents the type name in #ZFTYPEID_DECLARE\n
 *     "yourTypeData" store string datas that would be decoded by YourTypeNameFromString\n
 *     "yourTypeData" can be #v_zfstring, or native lua string\n
 *     if your value holder supplys reflectable #ZFObject::objectOnInit
 *     (#ZFOBJECT_ON_INIT_DECLARE_2 series),
 *     the value holder can also be constructed by function like call:
 *     "YourTypeName(param0, param1)"
 *   -  "value:yourFunc()"
 *     or "YourTypeName.YourFunc()"\n
 *     for non-ZFObject types that wrapped by #ZFTYPEID_DECLARE,
 *     you may use #ZFMETHOD_USER_REGISTER_0 series to register methods
 *     to its wrapper type "YourTypeName",
 *     then the methods can be invoked directly to your value type
 * -  value converter
 *   -  "zfl_value(v)"\n
 *     convert a value to lua's raw value,
 *     the result lua value can be:
 *     -  lua string
 *     -  lua integer
 *     -  lua number
 *     -  lua boolean
 *   -  these types are automatically converted when return from cpp to lua:
 *     -  v_zfbool => lua boolean
 * -  value comparer
 *   -  "zfl_cmp(v0, v1)"\n
 *     compare two values, including ZFObject types and lua types, return:
 *     -  -2 : error or uncomparable
 *     -  -1 : v0 < v1
 *     -  0 : v0 == v1
 *     -  1 : v0 > v1
 *   -  "zfl_eq(v0, v1)"\n
 *     similar to zfl_cmp, compare two values
 * -  callback
 *   -  "ZFCallbackForLua(luaFunc)"\n
 *     create a #ZFListener from lua function\n
 *     the lua function's proto type must be:
 *     @code
 *       function myLuaCallback(zfargs)
 *       end
 *     @endcode
 *     \n
 *     about thread safe:
 *     -  when the callback called in the same #ZFThread where the callback created,
 *       the callback should work as expected,
 *       all variable capture should work
 *     -  when the callback called in different #ZFThread:
 *       -  the callback and all captured upvalues would be copied to a new lua state
 *         managed by the #ZFThread
 *       -  only these value types can be captured:
 *         -  lua primitive types: boolean, integer, number, string
 *         -  ZFObject types
 *
 *     \n
 *     further more, lua function can be converted to #ZFListener implicitly
 *     @code
 *       button:observerAdd(ZFUIButton.E_ButtonOnClick(), function(zfargs)
 *           end)
 *     @endcode
 *     useful in dynamic method register:
 *     @code
 *       ZFDynamic()
 *           :staticMethod('MyRetType', 'myMethod', ZFMP()
 *               :mp('MyParamType', 'p0')
 *           , function(zfargs)
 *               ---@type MyRetType
 *               local p0 = zfargs:param0()
 *
 *               -- return by either of ways:
 *               zfargs:result(MyRetType())
 *               -- or simply return
 *               -- return MyRetType()
 *           end)
 *     @endcode
 *   -  "output:output(text[, size, result])"\n
 *     write to output callback
 *   -  "input:input(buf [, size, result])"\n
 *     read from input callback
 * -  array
 *   -  "ZFCoreArrayCreate([a, b, c, ...])"\n
 *     create a array, params support these types:
 *     -  zfauto
 *     -  native lua number (stored as #v_zfdouble)
 *     -  native lua string (stored as #v_zfstring)
 * -  param and return value
 *   -  simply use lua standard logic to process params and return values,
 *     here's some example:
 *     @code
 *       -- get all params
 *       local args = {...};
 *       local argsCount = #args;
 *       -- return value
 *       return args[1];
 *     @endcode
 *     note that, all params passed from #ZFLuaExecute are all #zfauto type
 * -  util
 *   -  "zfl_iter(a)"\n
 *     util to loop #ZFCoreArray, #ZFContainer, #ZFKeyValueContainer:
 *     @code
 *       -- for array type
 *       for i,e in zfl_iter(a) do
 *       end
 *       -- for map type
 *       for i,k,v in zfl_iter(m) do
 *       end
 *     @endcode
 *   -  "zfl_range(n, ...)"\n
 *     util to perform range loop:
 *     @code
 *       for i in zfl_range(xxx) do
 *       end
 *     @endcode
 *     -  "zfl_range(n)" : loop in range `[0, n)`
 *     -  "zfl_range(n0, n1)" : loop in range `[n0, n1)`
 *     -  "zfl_range(n0, n1, offset)" : loop in range `[n0, n1)`, each step with specified offset
 * -  local path info spec
 *   -  "zfl_L()"\n
 *     lua_State of current chunk, stored as #v_zfptr
 *   -  "ZFLocalPathInfo()"\n
 *     return path info of current context, null if not available
 *   -  "zfimport(localFilePath)"\n
 *     util method for #ZFLuaExecute + #ZFInputForLocal,
 *     useful to load local resource or class definition
 * -  debug helper
 *   -  "ZFLog(fmt, ...)"
 *     or ZFLogTrim(fmt, ...)\n
 *     use zfstringAppend then output to ZFLog
 *   -  "zfl_tableInfo(v)"
 *     or "zfl_tableInfoPrint(v)"\n
 *     return string that represents the table
 *
 *
 * note:
 * -  all types would be registered automatically,
 * -  for dynamically loaded library, all types would also be registered normally,
 *   however, won't be automatically unregistered when unloaded
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, zfauto, ZFLuaExecute
        , ZFMP_IN(const ZFInput &, input)
        )
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, zfauto, ZFLuaExecute
        , ZFMP_IN(const zfchar *, buf)
        )
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_8(ZFLIB_ZFLua, zfauto, ZFLuaExecute
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_8(ZFLIB_ZFLua, zfauto, ZFLuaExecute
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN(ZFObject *, param0)
        , ZFMP_IN_OPT(ZFObject *, param1, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param2, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param3, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param4, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param5, ZFMP_DEF())
        , ZFMP_IN_OPT(ZFObject *, param6, ZFMP_DEF())
        /* ZFMETHOD_MAX_PARAM */
        // , ZFMP_IN_OPT(ZFObject *, param7, ZFMP_DEF())
        )
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFLua, zfauto, ZFLuaExecuteDetail
        , ZFMP_IN(const ZFInput &, input)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, luaParams)
        , ZFMP_IN_OPT(void *, L, zfnull)
        )
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFLua, zfauto, ZFLuaExecuteDetail
        , ZFMP_IN(const zfchar *, buf)
        , ZFMP_IN(const ZFCoreArray<zfauto> &, luaParams)
        , ZFMP_IN_OPT(void *, L, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaExecute_h_


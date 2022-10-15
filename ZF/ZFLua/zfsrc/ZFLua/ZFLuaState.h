/**
 * @file ZFLuaState.h
 * @brief native lua state impl
 */

#ifndef _ZFI_ZFLuaState_h_
#define _ZFI_ZFLuaState_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global native lua state currently used by #ZFLuaExecute,
 *   ensured not null
 *
 * ZFFramework can be used in any native lua state (by using #ZFLuaExecute),
 * but only if you have attached them\n
 * by default #ZFLuaState would have a builtin state that already setup properly,
 * while you can attach your own native lua state to ZFFramework,
 * by #ZFLuaStateAttach/#ZFLuaStateDetach\n
 * the builtin lua state can also be changed by #ZFLuaStateChange
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void *, ZFLuaState)

/**
 * @brief change builtin lua state
 *
 * this method can not undo, old one would be closed
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateChange,
                        ZFMP_IN(void *, L))

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateListT,
                        ZFMP_IN_OUT(ZFCoreArray<void *> &, ret))
/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, ZFCoreArrayPOD<void *>, ZFLuaStateList)

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void *, ZFLuaStateOpen)
/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateClose,
                        ZFMP_IN(void *, L))

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateAttach,
                        ZFMP_IN(void *, L))
/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateDetach,
                        ZFMP_IN(void *, L))

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLuaStateAttach,
 * param0 is a #v_ZFPtr to lua state
 */
ZFOBSERVER_EVENT_GLOBAL(ZFLIB_ZFLua, LuaStateOnAttach)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLuaStateDetach,
 * param0 is a #v_ZFPtr to lua state
 */
ZFOBSERVER_EVENT_GLOBAL(ZFLIB_ZFLua, LuaStateOnDetach)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaState_h_


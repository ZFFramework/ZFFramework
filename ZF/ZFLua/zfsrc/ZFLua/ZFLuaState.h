/**
 * @file ZFLuaState.h
 * @brief native lua state impl
 */

#ifndef _ZFI_ZFLuaState_h_
#define _ZFI_ZFLuaState_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief return lua state for current thread,
 *   auto create and attach if no state attached to current thread
 *
 * ZFFramework can be used in any native lua state (by using #ZFLuaExecute),
 * but only if you have attached them by #ZFLuaStateAttach\n
 * note that, lua is not thread safe,
 * ZFFramework maintains lua states for each native #ZFThread,
 * these lua state is completely separate from each other,
 * no global data or data sharing available,
 * you must supply global methods in cpp to share data between threads\n
 * \n
 * by default, #ZFLuaState would automatically create a thread local lua state for current thread
 * (create new if not exist, you may change it by #ZFLuaStateChange),
 * you may still attach your own lua state for same thread by #ZFLuaStateAttach,
 * and obtain by #ZFLuaStateList for current thread,
 * or #ZFLuaStateListForAllThread for all thread
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void *, ZFLuaState)
/**
 * @brief try get lua state for current thread, or return null if none
 * @see ZFLuaState
 */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void *, ZFLuaStateCheck)

/** @brief change builtin lua state for current thread, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateChange
        , ZFMP_IN(void *, L)
        )

/** @brief get lua state list for current thread, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, ZFCoreArray<void *>, ZFLuaStateList)

/** @brief get lua state list for all thread, use with caution, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFLua, void, ZFLuaStateListForAllThread
        , ZFMP_OUT(ZFCoreArray<void *> &, luaStateList)
        , ZFMP_OUT(ZFCoreArray<ZFThread *> &, threadList)
        )

/** @brief create new lua state, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFLua, void *, ZFLuaStateOpen)
/** @brief close lua state, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateClose
        , ZFMP_IN(void *, L)
        )

/** @brief attach existing lua state to current thread, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateAttach
        , ZFMP_IN(void *, L)
        )
/** @brief detach lua state from current thread, see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFLua, void, ZFLuaStateDetach
        , ZFMP_IN(void *, L)
        )

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLuaStateAttach,
 * called in the same thread of #ZFLuaStateAttach called,
 * param0 is a #v_zfptr to lua state
 */
ZFEVENT_GLOBAL(ZFLIB_ZFLua, LuaStateOnAttach)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLuaStateDetach,
 * called in the same thread of #ZFLuaStateDetach called,
 * param0 is a #v_zfptr to lua state
 */
ZFEVENT_GLOBAL(ZFLIB_ZFLua, LuaStateOnDetach)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaState_h_


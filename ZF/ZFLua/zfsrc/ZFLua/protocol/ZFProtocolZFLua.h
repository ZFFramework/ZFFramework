/**
 * @file ZFProtocolZFLua.h
 * @brief protocol for ZFLua
 */

#ifndef _ZFI_ZFProtocolZFLua_h_
#define _ZFI_ZFProtocolZFLua_h_

#include "ZFCore/ZFProtocol.h"
#include "../ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFLua
 *
 * impl must supply all protocol described by #ZFLuaExecute
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFLIB_ZFLua, ZFLua)
public:
    /** @brief see #ZFLuaStateOpen, ensured called in proper thread with proper mutex */
    virtual void *luaStateOpen(void) zfpurevirtual;
    /** @brief see #ZFLuaStateClose, ensured called in proper thread with proper mutex */
    virtual void luaStateClose(ZF_IN void *L) zfpurevirtual;

    /** @brief see #ZFLuaStateAttach, ensured called in proper thread with proper mutex */
    virtual void luaStateAttach(ZF_IN void *L) zfpurevirtual;
    /** @brief see #ZFLuaStateDetach, ensured called in proper thread with proper mutex */
    virtual void luaStateDetach(ZF_IN void *L) zfpurevirtual;

    /** @brief called when class or method changed, ensured called in proper thread with proper mutex */
    virtual void classDataUpdate(
            ZF_IN void *L
            , ZF_IN const ZFClassDataUpdateData &data
            ) zfpurevirtual;

    /**
     * @brief see #ZFLuaExecute, ensured called in proper thread with proper mutex,
     *   impl must supply all methods mentioned in #ZFLuaExecute
     */
    virtual zfbool luaExecute(
            ZF_IN void *L
            , ZF_IN const ZFPathInfo &pathInfoOrNull
            , ZF_IN const ZFInput &input
            , ZF_OUT_OPT zfauto *luaResult = zfnull
            , ZF_IN_OPT const ZFCoreArray<zfauto> *luaParams = zfnull
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) zfpurevirtual;

    /** @brief see #ZFLuaGC, ensured called in proper thread with proper mutex */
    virtual void luaGC(ZF_IN void *L) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFLua)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFLua_h_


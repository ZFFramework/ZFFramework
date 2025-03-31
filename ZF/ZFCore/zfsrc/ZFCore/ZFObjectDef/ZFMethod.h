/**
 * @file ZFMethod.h
 * @brief reflectable method definination for ZFFramework
 */

#ifndef _ZFI_ZFMethod_h_
#define _ZFI_ZFMethod_h_

#include "ZFObjectClassTypeFwd.h"
#include "ZFMethodGenericInvoker.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief access type for ZFMethod
 */
typedef enum {
    ZFMethodAccessTypePublic, /**< @brief public */
    ZFMethodAccessTypeProtected, /**< @brief protected */
    ZFMethodAccessTypePrivate, /**< @brief private */
} ZFMethodAccessType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodAccessTypePublic "public"
/** @brief string tokens */
#define ZFTOKEN_ZFMethodAccessTypeProtected "protected"
/** @brief string tokens */
#define ZFTOKEN_ZFMethodAccessTypePrivate "private"

// ============================================================
#define _ZFP_ZFMethodType_ZFMethodTypeNormal()
#define _ZFP_ZFMethodType_ZFMethodTypeStatic() static
#define _ZFP_ZFMethodType_ZFMethodTypeVirtual() virtual
#define _ZFP_ZFMethodType(ZFMethodType_) _ZFP_ZFMethodType_##ZFMethodType_()

#define _ZFP_ZFMethodTypeChain_ZFMethodTypeNormal(...) __VA_ARGS__
#define _ZFP_ZFMethodTypeChain_ZFMethodTypeStatic(...)
#define _ZFP_ZFMethodTypeChain_ZFMethodTypeVirtual(...) __VA_ARGS__
#define _ZFP_ZFMethodTypeChain(ZFMethodType_, ...) _ZFP_ZFMethodTypeChain_##ZFMethodType_(__VA_ARGS__)

/**
 * @brief the method type
 */
typedef enum {
    ZFMethodTypeNormal, /**< @brief normal method */
    ZFMethodTypeStatic, /**< @brief static method */
    ZFMethodTypeVirtual, /**< @brief virtual method */
} ZFMethodType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeNormal ""
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeStatic "static"
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeVirtual "virtual"

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFClass;
#define _ZFP_ZFMETHOD_INVOKER(N) \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    inline T_ReturnType executeExact(ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) const { \
        if(this->methodInvoker()) { \
            return reinterpret_cast< \
                    T_ReturnType (*)(const ZFMethod *, zfany const & ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) \
                    >(this->methodInvoker()) \
                (this, obj ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA)); \
        } \
        else { \
            return _ZFP_MtdGII<T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TYPE, ZFM_COMMA, ZFM_COMMA)>(\
                    this, obj ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA) \
                ); \
        } \
    } \
    /* this would save much binary size by removing support of dynamic method forwarding, for internal impl use only, use with caution */ \
    template<typename T_ReturnType ZFM_REPEAT(N, ZFM_REPEAT_TEMPLATE, ZFM_COMMA, ZFM_COMMA)> \
    inline T_ReturnType _ZFP_execute(ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) const { \
        return reinterpret_cast< \
                T_ReturnType (*)(const ZFMethod *, zfany const & ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) \
                >(this->methodInvoker()) \
            (this, obj ZFM_REPEAT(N, ZFM_REPEAT_NAME, ZFM_COMMA, ZFM_COMMA)); \
    }

// ============================================================
#define _ZFP_ZFMethodNoDefaultParam ZFM_EMPTY
#define _ZFP_ZFMethodDefaultParam(DefaultValue) \
    = (DefaultValue) ZFM_EMPTY

// tricks to solve recursive macro
#define _ZFP_MtdM_EMPTY(...)
#define _ZFP_MtdM_EXPAND(...) __VA_ARGS__
#define _ZFP_MtdP_EXPAND(...) __VA_ARGS__
#define _ZFP_MtdD_EXPAND(...) __VA_ARGS__
/**
 * @brief macro to wrap param types for #ZFMETHOD_INLINE_0 series
 *
 * similar to #ZF_IN for normal methods,
 * here's a list of these macros used for #ZFMETHOD_INLINE_0 series:
 * -  #ZFMP_IN
 * -  #ZFMP_IN_OPT
 * -  #ZFMP_OUT
 * -  #ZFMP_OUT_OPT
 * -  #ZFMP_IN_OUT
 * -  #ZFMP_IN_OUT_OPT
 */
#define ZFMP_IN(ParamType, paramName) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EMPTY, _ZFP_ZFMethodNoDefaultParam
/** @brief see #ZFMP_IN */
#define ZFMP_IN_OPT(ParamType, paramName, DefaultValue) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EXPAND, _ZFP_ZFMethodDefaultParam(DefaultValue)
/** @brief see #ZFMP_IN */
#define ZFMP_OUT(ParamType, paramName) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EMPTY, _ZFP_ZFMethodNoDefaultParam
/** @brief see #ZFMP_IN */
#define ZFMP_OUT_OPT(ParamType, paramName, DefaultValue) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EXPAND, _ZFP_ZFMethodDefaultParam(DefaultValue)
/** @brief see #ZFMP_IN */
#define ZFMP_IN_OUT(ParamType, paramName) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EMPTY, _ZFP_ZFMethodNoDefaultParam
/** @brief see #ZFMP_IN */
#define ZFMP_IN_OUT_OPT(ParamType, paramName, DefaultValue) \
    _ZFP_MtdM_EXPAND, ParamType, paramName, _ZFP_MtdM_EXPAND, _ZFP_ZFMethodDefaultParam(DefaultValue)

#define _ZFP_ZFMP_DUMMY() \
    _ZFP_MtdM_EMPTY, ParamType, paramName, _ZFP_MtdM_EMPTY, _ZFP_ZFMethodNoDefaultParam

// ============================================================
zfclassFwd ZFListener;
extern ZFLIB_ZFCore const ZFListener &_ZFP_ZFMethod_paramDefaultValueCallbackDummy(void);
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFMethodMP {
public:
    zfuint paramCount;
    ZFSigName paramTypeId[ZFMETHOD_MAX_PARAM];
    ZFSigName paramName[ZFMETHOD_MAX_PARAM];
    ZFListener *paramDefaultValueCallback[ZFMETHOD_MAX_PARAM];
public:
    _ZFP_ZFMethodMP &add(
            ZF_IN const zfstring &paramTypeId
            , ZF_IN const zfstring &paramName
            , ZF_IN const ZFListener &paramDefaultValueCallback
            );
public:
    _ZFP_ZFMethodMP(void) : paramCount(0) {
    }
    ~_ZFP_ZFMethodMP(void);
private:
    _ZFP_ZFMethodMP(ZF_IN const _ZFP_ZFMethodMP &ref);
};

// ============================================================
zfclassFwd _ZFP_ZFMethodPrivateExt;
/**
 * @brief reflectable method for ZFObject
 *
 * (for function type of ZFMethod, please refer to #ZFMETHOD_FUNC_DECLARE_0)\n
 * \n
 * to use ZFMethod, you must first declare it in your class:
 * @code
 *   zfclass YourClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *      / **
 *        * you can add doxygen docs here
 *        * /
 *       ZFMETHOD_INLINE_0(void, yourMethod) {
 *           // method code here
 *       }
 *   };
 * @endcode
 * or you may want to declare in header and define in cpp file:
 * @code
 *   // YourClass.h
 *   zfclass YourClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *       ZFMETHOD_DECLARE_0(void, yourMethod) // declare only
 *   };
 *
 *   // YourClass.cpp
 *   ZFMETHOD_DEFINE_0(YourClass, void, yourMethod) {
 *       // method code here
 *   }
 * @endcode
 * param and return type must registered by #ZFTYPEID_DECLARE series,
 * and the param's num is limited to #ZFMETHOD_MAX_PARAM\n
 * \n
 * the ZFMETHOD_XXX macro takes many params,
 * to walk through it, let's take a example:
 * @code
 *   ZFMETHOD_DECLARE_DETAIL_2(
 *       PublicOrProtectedOrPrivate, ZFMethodType_,
 *       ReturnType, MethodName
 *       , ZFMP_IN(ParamType0, param0)
 *       , ZFMP_IN_OPT(ParamType1, param1, DefaultValue1)
 *       )
 * @endcode
 * -  PublicOrProtectedOrPrivate is the access type for the method,
 *   could be: public, protected, private
 * -  ZFMethodType_ indicates the method's type,
 *   could be: ZFMethodTypeStatic, ZFMethodTypeVirtual, ZFMethodTypeNormal
 * -  RegSig used to differ the method from overloaded method,
 *   you may use #ZF_CALLER_LINE for most case
 * -  ReturnType is the method's return value's type,
 *   could be any types that are registerd by #ZFTYPEID_DECLARE
 * -  MethodName is the method's name
 * -  ParamTypeN is the type of param at index N,
 *   could be any types that are registerd by #ZFTYPEID_DECLARE
 * -  paramN is the name of param at index N
 * -  DefaultValueN is the default value for the param
 *
 * to override a method declared in parent,
 * simply override like normal virtual functions\n
 * \n
 * once declared, the method would be automatically registered to
 * it's owner class when it's owner class registered, see also #ZFOBJECT_REGISTER\n
 * \n
 * to access the method, use ZFMethodAccess/ZFMethodAccessDetail_0 is recommended:
 * @code
 *   const ZFMethod *method = ZFMethodAccess(YourClassType, yourMethodName);
 * @endcode
 * also, you may use ZFClass's member function to reflect from string,
 * such as #ZFClass::methodForName\n
 * \n
 * once you have successfully get the ZFMethod data,
 * you can execute it without knowing the owner's class type:
 * @code
 *   const ZFClass *cls = ...; // we may not know the actual class type
 *   const ZFMethod *method = cls->methodForName("someMethod");
 *   zfauto obj = cls->newInstance();
 *
 *   // execute
 *   YourReturnType result = method->executeExact<YourReturnType, ParamType...>(obj, someParam...);
 *
 *   // or, you may use generic version:
 *   zfauto result2 = method->methodInvoke(obj, someParam...);
 * @endcode
 * @warning when using executeExact, you take the full responsibility to make sure
 *   the ReturnType and ParamType exactly match the method,
 *   if not, no compile error would occurred,
 *   but app may (or may not) crash at runtime
 *
 * \n
 * if you want to reflect overloaded method, use both methodName and param's type id
 * @code
 *   cls->methodForName("methodName", ZFTypeId<Param0>::TypeId());
 * @endcode
 *
 * @warning in subclass, you may declare a method with the same name of parent's one,
 *   but the behavior may be unexpected in some situations,
 *   so it's highly deprecated to do so
 * @note it's thread-safe to execute ZFMethod,
 *   but whether the actual run code (the code in the method body)
 *   is thread-safe, is depends on you
 */
zffinal zfclassNotPOD ZFLIB_ZFCore ZFMethod {
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFMethod)

public:
    void _ZFP_ZFMethod_init(
            ZF_IN zfbool isUserRegister
            , ZF_IN zfbool isDynamicRegister
            , ZF_IN ZFObject *dynamicRegisterUserData
            , ZF_IN ZFFuncAddrType invoker
            , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
            , ZF_IN ZFMethodType methodType
            , ZF_IN const zfstring &methodName
            , ZF_IN const zfstring &returnTypeId
            , ZF_IN const _ZFP_ZFMethodMP &mp
            );
    void _ZFP_ZFMethod_initClassMemberType(
            ZF_IN const ZFClass *ownerClass
            , ZF_IN ZFMethodAccessType privilegeType
            );
    void _ZFP_ZFMethod_initFuncType(ZF_IN const zfstring &methodNamespace);
    /** @cond ZFPrivateDoc */
    ZFMethod(void);
    /** @endcond */
    ~ZFMethod(void);

public:
    _ZFP_ZFMETHOD_INVOKER(0)
    _ZFP_ZFMETHOD_INVOKER(1)
    _ZFP_ZFMETHOD_INVOKER(2)
    _ZFP_ZFMETHOD_INVOKER(3)
    _ZFP_ZFMETHOD_INVOKER(4)
    _ZFP_ZFMETHOD_INVOKER(5)
    _ZFP_ZFMETHOD_INVOKER(6)
    _ZFP_ZFMETHOD_INVOKER(7)
    _ZFP_ZFMETHOD_INVOKER(8)
    /* ZFMETHOD_MAX_PARAM */

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    // ============================================================
    // general
public:
    /** @brief see #ZFMethodAlias */
    const ZFMethod *aliasFrom(void) const;
    /** @brief see #ZFMethodAlias */
    const ZFCoreArray<const ZFMethod *> &aliasTo(void) const;

    /**
     * @brief internal method id, for debug use only
     */
    inline zfidentity methodId(void) const {
        return this->_ZFP_ZFMethod_methodId.sigId();
    }
    /**
     * @brief whether the method is registered by #ZFMethodUserRegister_0
     */
    inline zfbool isUserRegister(void) const {
        return this->_ZFP_ZFMethod_isUserRegister;
    }
    /**
     * @brief store impl state for #ZFMethodUserRegister_0
     */
    inline zfany userRegisterUserData(void) const {
        return this->_ZFP_ZFMethod_isUserRegister ? this->_ZFP_ZFMethod_methodUserData : zfnull;
    }
    /**
     * @brief store impl state for #ZFMethodUserRegister_0
     */
    void userRegisterUserData(ZF_IN ZFObject *userData) const;
    /**
     * @brief whether the method is registered by #ZFMethodDynamicRegister
     */
    inline zfbool isDynamicRegister(void) const {
        return this->_ZFP_ZFMethod_isDynamicRegister;
    }
    /**
     * @brief see #ZFMethodDynamicRegister
     */
    inline zfany dynamicRegisterUserData(void) const {
        return this->_ZFP_ZFMethod_isDynamicRegister ? this->_ZFP_ZFMethod_methodUserData : zfnull;
    }
    /**
     * @brief get the method's name
     */
    inline const zfstring &methodName(void) const {
        return this->_ZFP_ZFMethod_methodName;
    }

    /**
     * @brief get the method's return value's type id
     */
    inline const zfstring &returnTypeId(void) const {
        return this->_ZFP_ZFMethod_returnTypeId;
    }

    /**
     * @brief get the method's param num
     */
    inline zfindex paramCount(void) const {
        return (zfindex)this->_ZFP_ZFMethod_paramCount;
    }
    /**
     * @brief get the method's minimum param num,
     *   same as #paramCount if no default param
     */
    inline zfindex paramCountMin(void) const {
        return (zfindex)this->_ZFP_ZFMethod_paramCountMin;
    }
    /**
     * @brief check whether method param type id matches
     */
    zfbool paramTypeIdIsMatch(
            ZF_IN_OPT const zfchar *paramTypeId0 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
            ) const;
    /**
     * @brief see #paramTypeIdIsMatch
     */
    zfbool paramTypeIdIsMatch(ZF_IN const ZFMethod *method) const;
    /**
     * @brief get the method's param type id at index
     */
    inline const zfstring &paramTypeIdAt(ZF_IN zfindex index) const {
        ZFCoreAssert(index < this->paramCount());
        return this->_ZFP_ZFMethod_paramTypeIdList()[index];
    }
    /**
     * @brief get the method's param name at index, usually for debug use
     */
    inline const zfstring &paramNameAt(ZF_IN zfindex index) const {
        ZFCoreAssert(index < this->paramCount());
        return this->_ZFP_ZFMethod_paramNameList()[index];
    }
    /**
     * @brief get the method param's default value access callback
     */
    const ZFListener &paramDefaultValueCallbackAt(ZF_IN zfindex index) const;
    /**
     * @brief get the method's param default value at index, null if no default param
     */
    zfauto paramDefaultValueAt(ZF_IN zfindex index) const;
    /**
     * @brief return the first default param's index, or #zfindexMax if no default param
     */
    inline zfindex paramDefaultBeginIndex(void) const {
        return (this->_ZFP_ZFMethod_paramDefaultBeginIndex == (zfuint)-1 ? zfindexMax() : (zfindex)this->_ZFP_ZFMethod_paramDefaultBeginIndex);
    }

    /**
     * @brief util to return param info
     *
     * sample output: `P0 p0, P1 p1 = def`,
     * or output nothing if no param
     */
    void paramInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #paramInfoT */
    zfstring paramInfo(void) const {
        zfstring ret;
        this->paramInfoT(ret);
        return ret;
    }

    /**
     * @brief invoker function address for both class member type and function type
     *
     * proto type:
     * @code
     *   ReturnType methodInvoker(
     *           ZF_IN const ZFMethod *invokerMethod
     *           , ZF_IN zfany const &invokerObject
     *           , ZF_IN ParamType0 param0
     *           , ZF_IN ParamType1 param1
     *           , / * ... * /);
     * @endcode
     *
     * @note the methodInvoker is ensured not null for normal method
     *   and user registered method,
     *   and may be null for dynamic registered method (#ZFMethodDynamicRegister),
     *   you may change it by #methodInvoker
     */
    inline ZFFuncAddrType methodInvoker(void) const {
        return this->_ZFP_ZFMethod_invoker;
    }
    /**
     * @brief override original #methodInvoker
     *
     * set null to remove the overrided value,
     * the original invoker can be accessed by #methodInvokerOrig
     * @note no safe check for the method's proto type,
     *   you must ensure it's valid and safe to be called
     * @note assume the original method's proto type is:
     *   @code
     *     ReturnType method(ParamType0, ParamType1, ...);
     *   @endcode
     *   then the invoker's proto type must be:
     *   @code
     *     ReturnType method(
     *         const ZFMethod *invokerMethod
     *         , zfany const &invokerObject
     *         , ParamType0 param0
     *         , ParamType1 param1
     *         , ...
     *         );
     *   @endcode
     * @note overriding the invoker only affect methods when they are reflected,
     *   calling it directly won't be affected
     *   @code
     *     obj->myMethod(); // won't be affected
     *     method->executeExact<void>(obj); // would be affected
     *   @endcode
     */
    void methodInvoker(ZF_IN ZFFuncAddrType methodInvoker) const;
    /**
     * @brief see #methodInvoker
     */
    ZFFuncAddrType methodInvokerOrig(void) const;

    /**
     * @brief generic invoker for advanced reflection
     *
     * as a static language, C++'s function invoking require explicit type declaration,
     * which is a pain when binding with script languages\n
     * since ZFFramework supply reflection (though limited),
     * we are trying to solve the dynamic script binding, how it works:
     * -  #ZFTypeId::Value\n
     *   to supply type conversion to ZFObject types without knowing actual type
     * -  #ZFMethodGenericInvoker\n
     *   to invoke the reflectable method without static type binding
     *   (all params and result are #ZFObject types)
     *
     * \n
     * typical steps for users:
     * -# have all methods you need to bind been declared by #ZFMETHOD_INLINE_0 series
     * -# supply wrapper class to hold the type
     * -# ensure all params can be converted to ZFObject types,
     *   by declaring them by #ZFTYPEID_DECLARE
     * -# all done, all binding works should be done by impl
     *
     * \n
     * typical steps for impl:
     * -# supply type convert methods to bind #ZFTypeId::Value types to script languages
     * -# using reflection of #ZFClass and #ZFMethod,
     *   to bind all class and methods to script languages
     *
     * \n
     * the final script that calling ZFFFramework, should look like:
     * @code
     *   local obj = MyZFObjectType();
     *   local objParam = SomeType();
     *   local result = obj:myFunc(zfint(1), zfstring("2"), objParam)
     * @endcode
     */
    inline ZFMethodGenericInvoker methodGenericInvoker(void) const {
        return this->_ZFP_ZFMethod_methodGenericInvoker;
    }
    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief util method to invoke #ZFMethodGenericInvoker
     *
     * note all params must be exactly same type,
     * use #ZFDI_invoke if you want auto param conversion
     */
    zfauto methodInvoke(
            ZF_IN_OPT ZFObject *ownerObjOrNull = zfnull
            ) const;
    /** @brief see #methodInvoke */
    zfauto methodInvoke(
            ZF_IN ZFObject *ownerObjOrNull
            , ZF_IN ZFObject *param0
            , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
            ) const;
    /** @brief see #methodInvoke */
    zfbool methodInvokeT(
            ZF_OUT zfauto &ret
            , ZF_OUT_OPT zfstring *errorHint
            , ZF_IN ZFObject *ownerObjOrNull
            , ZF_IN_OPT ZFObject *param0 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
            ) const;
    /** @brief see #methodInvoke */
    zfauto methodInvokeDetail(
            ZF_IN ZFObject *ownerObjOrNull
            , ZF_IN const ZFCoreArray<zfauto> &params
            , ZF_OUT_OPT zfbool *success = zfnull
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) const;

    /**
     * @brief see #methodGenericInvoker
     */
    ZFMethodGenericInvoker methodGenericInvokerOrig(void) const;

    /**
     * @brief change default impl for #methodGenericInvoker
     *
     * the original invoker can be accessed by #methodGenericInvokerOrig
     */
    void methodGenericInvoker(ZF_IN ZFMethodGenericInvoker methodGenericInvoker) const;

    // ============================================================
    // class member type
public:
    /**
     * @brief method's owner class, null for function type method
     */
    inline const ZFClass *ownerClass(void) const {
        return this->_ZFP_ZFMethod_ownerClass;
    }
    /**
     * @brief method's owner proeprty, null if none
     */
    inline const ZFProperty *ownerProperty(void) const {
        return this->_ZFP_ZFMethod_ownerProperty;
    }

    /**
     * @brief get the method's access type
     */
    inline ZFMethodAccessType methodAccessType(void) const {
        return (ZFMethodAccessType)this->_ZFP_ZFMethod_privilegeType;
    }
    /** @brief util to check whether #methodAccessType is #ZFMethodAccessTypePublic */
    inline zfbool isPublic(void) const {return (ZFMethodAccessType)this->_ZFP_ZFMethod_privilegeType == ZFMethodAccessTypePublic;}
    /** @brief util to check whether #methodAccessType is #ZFMethodAccessTypeProtected */
    inline zfbool isProtected(void) const {return (ZFMethodAccessType)this->_ZFP_ZFMethod_privilegeType == ZFMethodAccessTypeProtected;}
    /** @brief util to check whether #methodAccessType is #ZFMethodAccessTypePrivate */
    inline zfbool isPrivate(void) const {return (ZFMethodAccessType)this->_ZFP_ZFMethod_privilegeType == ZFMethodAccessTypePrivate;}

    /**
     * @brief method type
     */
    inline ZFMethodType methodType(void) const {
        return (ZFMethodType)this->_ZFP_ZFMethod_methodType;
    }
    /** @brief util to check whether #methodType is #ZFMethodTypeNormal */
    inline zfbool isNormal(void) const {return (ZFMethodType)this->_ZFP_ZFMethod_methodType == ZFMethodTypeNormal;}
    /** @brief util to check whether #methodType is #ZFMethodTypeStatic */
    inline zfbool isStatic(void) const {return (ZFMethodType)this->_ZFP_ZFMethod_methodType == ZFMethodTypeStatic;}
    /** @brief util to check whether #methodType is #ZFMethodTypeVirtual */
    inline zfbool isVirtual(void) const {return (ZFMethodType)this->_ZFP_ZFMethod_methodType == ZFMethodTypeVirtual;}

    // ============================================================
    // func type
public:
    /**
     * @brief true if this method is function type (declared by ZFMETHOD_FUNC_DECLARE_XXX series)
     */
    inline zfbool isFuncType(void) const {
        return (this->_ZFP_ZFMethod_ownerClass == zfnull);
    }
    /**
     * @brief get the method namespace, for func type only,
     *   null or empty string for global scope (#ZF_NAMESPACE_GLOBAL)
     */
    inline const zfstring &methodNamespace(void) const {
        return this->_ZFP_ZFMethod_methodNamespace;
    }

    // ============================================================
    // other
public:
    /**
     * @brief whether the method is internal method
     *
     * methods would be treated as internal if:
     * -  method name starts with '_'
     *
     * see also #isInternalPrivate
     */
    inline zfbool isInternal(void) const {
        return this->_ZFP_ZFMethod_isInternal;
    }
    /**
     * @brief whether the method is internal private method
     *
     * methods would be treated as internal private if:
     * -  method name starts with '_ZFP_'
     *
     * internal methods would be ignored from reflection,
     * see also #isInternal
     */
    inline zfbool isInternalPrivate(void) const {
        return this->_ZFP_ZFMethod_isInternalPrivate;
    }

public:
    ZFMethod *_ZFP_ZFMethod_removeConst(void) const {
        return const_cast<ZFMethod *>(this);
    }

public:
    inline ZFSigName *_ZFP_ZFMethod_paramTypeIdList(void) const {
        return _ZFP_ZFMethod_paramBuf;
    }
    inline ZFSigName *_ZFP_ZFMethod_paramNameList(void) const {
        return _ZFP_ZFMethod_paramBuf + ZFMETHOD_MAX_PARAM;
    }

public:
    // general
    zfuint _ZFP_ZFMethod_refCount;

    zfbool _ZFP_ZFMethod_isInternal;
    zfbool _ZFP_ZFMethod_isInternalPrivate;
    zfbool _ZFP_ZFMethod_isUserRegister;
    zfbool _ZFP_ZFMethod_isDynamicRegister;
    zfany _ZFP_ZFMethod_methodUserData;

    ZFSigName _ZFP_ZFMethod_methodId;
    _ZFP_ZFMethodPrivateExt *_ZFP_ZFMethod_ext;

    ZFFuncAddrType _ZFP_ZFMethod_invoker;
    ZFMethodGenericInvoker _ZFP_ZFMethod_methodGenericInvoker;
    ZFSigName _ZFP_ZFMethod_methodName;
    ZFSigName _ZFP_ZFMethod_returnTypeId;
    zfuint _ZFP_ZFMethod_paramCount;
    zfuint _ZFP_ZFMethod_paramCountMin;
    ZFSigName *_ZFP_ZFMethod_paramBuf;
    ZFListener *_ZFP_ZFMethod_paramDefaultValueCallbackList;
    zfuint _ZFP_ZFMethod_paramDefaultBeginIndex;

    // for class member type
    unsigned short /* ZFMethodAccessType */ _ZFP_ZFMethod_privilegeType;
    unsigned short /* ZFMethodType */ _ZFP_ZFMethod_methodType;
    const ZFClass *_ZFP_ZFMethod_ownerClass;
    const ZFProperty *_ZFP_ZFMethod_ownerProperty;

    // for func type
    ZFSigName _ZFP_ZFMethod_methodNamespace;
};

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFMethodDataHolderInit(void);
extern ZFLIB_ZFCore ZFMethod *_ZFP_ZFMethodRegister(
        ZF_IN zfbool isUserRegister
        , ZF_IN zfbool isDynamicRegister
        , ZF_IN ZFObject *dynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN ZFMethodAccessType methodAccessType
        , ZF_IN const zfstring &methodNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN const zfstring &returnTypeId
        , ZF_IN const _ZFP_ZFMethodMP &mp
        );
extern ZFLIB_ZFCore void _ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method);

zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFMethodRegisterHolder {
public:
    _ZFP_ZFMethodRegisterHolder(
            ZF_IN zfbool isUserRegister
            , ZF_IN zfbool isDynamicRegister
            , ZF_IN ZFObject *dynamicRegisterUserData
            , ZF_IN ZFFuncAddrType methodInvoker
            , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
            , ZF_IN ZFMethodType methodType
            , ZF_IN const ZFClass *ownerClass
            , ZF_IN ZFMethodAccessType methodAccessType
            , ZF_IN const zfstring &methodNamespace
            , ZF_IN const zfstring &methodName
            , ZF_IN const zfstring &returnTypeId
            , ZF_IN const _ZFP_ZFMethodMP &mp
            );
    ~_ZFP_ZFMethodRegisterHolder(void);
public:
    ZFMethod *method;
};

// ============================================================
/** @brief see #ZFMethodGetAll */
extern ZFLIB_ZFCore void ZFMethodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret);
/**
 * @brief get all method currently registered
 *
 * note, this method may cause performance issue
 * if you have many method registered,
 * use with caution\n
 * use #ZFClass::methodForName or #ZFMethodFuncForName if necessary
 */
inline ZFCoreArray<const ZFMethod *> ZFMethodGetAll(void) {
    ZFCoreArray<const ZFMethod *> ret;
    ZFMethodGetAllT(ret);
    return ret;
}

// ============================================================
/* ZFMETHOD_MAX_PARAM */
/**
 * @brief util method to find method
 *
 * method can be class member or function type method,
 * return first registered one if more than one method found,
 * use #ZFMethodForNameGetAll to check if you have overloaded method,
 * use #ZFMethodFuncForName to explicitly find function type method
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodForName(
        ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
        );
/** @brief see #ZFMethodForName */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodForName(
        ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
        , ZF_IN_OPT const zfchar *paramTypeId0
        , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
        , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
        );

/**
 * @brief util method to find method, see ZFMethodForName
 */
extern ZFLIB_ZFCore void ZFMethodForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
        );
/** @brief see #ZFMethodForNameGetAllT */
inline ZFCoreArray<const ZFMethod *> ZFMethodForNameGetAll(
        ZF_IN const zfstring &classNameOrNamespace
        , ZF_IN const zfstring &methodName
        ) {
    ZFCoreArray<const ZFMethod *> ret;
    ZFMethodForNameGetAllT(ret, classNameOrNamespace, methodName);
    return ret;
}

// ============================================================
/**
 * @brief create new #ZFMethod and alias to existing method
 *
 * @code
 *   zfclass MyClass : zfextend ZFObject {
 *       ZFOBJECT_DECLARE(MyClass, ZFObject)
 *       ZFMETHOD_DECLARE_0(void, foo)
 *   };
 *   ZFOBJECT_REGISTER(MyClass)
 *   ZFMETHOD_DEFINE_0(MyClass, void, foo) {
 *       ZFLogTrim() << "foo()";
 *   }
 *
 *   ZFMethodAlias(ZFMethodAccess(MyClass, foo), "bar");
 *   ZFLogTrim() << MyClass::ClassData()->methodForName("foo");
 *   ZFLogTrim() << MyClass::ClassData()->methodForName("bar");
 *   zfobj<MyClass> obj;
 *   obj->invoke("foo");
 *   obj->invoke("bar");
 * @endcode
 *
 * @see ZFClassAlias
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodAlias(
        ZF_IN const ZFMethod *method
        , ZF_IN const zfstring &aliasName
        );
/**
 * @brief see #ZFMethodAlias
 */
extern ZFLIB_ZFCore void ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethod_h_


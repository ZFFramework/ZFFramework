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
    ZFMethodPrivilegeTypePublic, /**< @brief public */
    ZFMethodPrivilegeTypeProtected, /**< @brief protected */
    ZFMethodPrivilegeTypePrivate, /**< @brief private */
} ZFMethodPrivilegeType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePublic "public"
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypeProtected "protected"
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePrivate "private"

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
    inline T_ReturnType execute(ZFObject *obj ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) const { \
        if(this->_ZFP_ZFMethod_invoker) { \
            return reinterpret_cast< \
                    T_ReturnType (*)(const ZFMethod *, zfany const & ZFM_REPEAT(N, ZFM_REPEAT_PARAM, ZFM_COMMA, ZFM_COMMA)) \
                    >(this->_ZFP_ZFMethod_invoker) \
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
                >(this->_ZFP_ZFMethod_invoker) \
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

/**
 * @brief callback to access method param's default value,
 *   for method generic invoker
 */
typedef zfauto (*ZFMethodParamDefaultValueCallback)(
        ZF_IN const ZFMethod *invokerMethod
        , ZF_IN zfindex paramIndex
        );

// ============================================================
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
 *       PublicOrProtectedOrPrivate, ZFMethodType_, RegSig,
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
 *   YourReturnType result = method->execute<YourReturnType, ParamType...>(obj, someParam...);
 *
 *   // or, you may use generic version:
 *   zfauto result2 = method->methodInvoke(obj, someParam...);
 * @endcode
 * @warning you take the full responsibility to make sure
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
    zfbool _ZFP_ZFMethodNeedInit;
    void _ZFP_ZFMethod_init(
            ZF_IN zfbool methodIsUserRegister
            , ZF_IN zfbool methodIsDynamicRegister
            , ZF_IN ZFObject *methodDynamicRegisterUserData
            , ZF_IN ZFFuncAddrType invoker
            , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
            , ZF_IN ZFMethodType methodType
            , ZF_IN const zfchar *methodName
            , ZF_IN const zfchar *returnTypeId
            , ZF_IN const zfchar *returnTypeName
            , ... /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
            );
    void _ZFP_ZFMethod_initClassMemberType(
            ZF_IN const ZFClass *methodOwnerClass
            , ZF_IN ZFMethodPrivilegeType privilegeType
            );
    void _ZFP_ZFMethod_initFuncType(ZF_IN const zfchar *methodNamespace);
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
    inline const ZFMethod *methodAliasFrom(void) const {
        return this->_ZFP_ZFMethod_methodAliasFrom;
    }
    /** @brief see #ZFMethodAlias */
    inline const ZFCoreArray<const ZFMethod *> &methodAliasTo(void) const {
        return this->_ZFP_ZFMethod_methodAliasTo;
    }

    /**
     * @brief internal method id, for debug use only
     */
    inline const zfchar *methodInternalId(void) const {
        return this->_ZFP_ZFMethod_methodInternalId;
    }
    /**
     * @brief whether the method is registered by #ZFMethodUserRegister_0
     */
    inline zfbool methodIsUserRegister(void) const {
        return this->_ZFP_ZFMethod_methodIsUserRegister;
    }
    /**
     * @brief store impl state for #ZFMethodUserRegister_0
     */
    inline zfany methodUserRegisterUserData(void) const {
        return this->_ZFP_ZFMethod_methodUserRegisterUserData;
    }
    /**
     * @brief store impl state for #ZFMethodUserRegister_0
     */
    void methodUserRegisterUserData(ZF_IN ZFObject *userData) const;
    /**
     * @brief whether the method is registered by #ZFMethodDynamicRegister
     */
    inline zfbool methodIsDynamicRegister(void) const {
        return this->_ZFP_ZFMethod_methodIsDynamicRegister;
    }
    /**
     * @brief see #ZFMethodDynamicRegister
     */
    inline zfany methodDynamicRegisterUserData(void) const {
        return this->_ZFP_ZFMethod_methodDynamicRegisterUserData;
    }
    /**
     * @brief get the method's name
     */
    inline const zfchar *methodName(void) const {
        return this->_ZFP_ZFMethod_methodName;
    }

    /**
     * @brief get the method's return value's type id
     */
    inline const zfchar *methodReturnTypeId(void) const {
        return this->_ZFP_ZFMethod_returnTypeId;
    }
    /**
     * @brief get the method's return value's type name
     */
    inline const zfchar *methodReturnTypeName(void) const {
        return this->_ZFP_ZFMethod_returnTypeName;
    }

    /**
     * @brief get the method's param num
     */
    inline zfindex methodParamCount(void) const {
        return (zfindex)this->_ZFP_ZFMethod_paramCount;
    }
    /**
     * @brief get the method's minimum param num,
     *   same as #methodParamCount if no default param
     */
    inline zfindex methodParamCountMin(void) const {
        return (zfindex)this->_ZFP_ZFMethod_paramCountMin;
    }
    /**
     * @brief check whether method param type id matches
     *
     * null type id means not specified (any type matches),
     * use #ZFTypeId_void to explicitly limit param count
     */
    zfbool methodParamTypeIdIsMatch(
            ZF_IN_OPT const zfchar *methodParamTypeId0 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
            , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
            ) const;
    /**
     * @brief see #methodParamTypeIdIsMatch
     */
    zfbool methodParamTypeIdIsMatch(ZF_IN const ZFMethod *method) const;
    /**
     * @brief get the method's param type id at index
     */
    inline const zfchar *methodParamTypeIdAt(ZF_IN zfindex index) const {
        zfCoreAssert(index < this->methodParamCount());
        return this->_ZFP_ZFMethod_paramTypeIdList[index];
    }
    /**
     * @brief get the method's param type name at index, usually for debug use
     */
    inline const zfchar *methodParamTypeNameAt(ZF_IN zfindex index) const {
        zfCoreAssert(index < this->methodParamCount());
        return this->_ZFP_ZFMethod_paramTypeNameList[index];
    }
    /**
     * @brief get the method's param name at index, usually for debug use
     */
    inline const zfchar *methodParamNameAt(ZF_IN zfindex index) const {
        zfCoreAssert(index < this->methodParamCount());
        return this->_ZFP_ZFMethod_paramNameList[index];
    }
    /**
     * @brief get the method param's default value access callback
     */
    inline ZFMethodParamDefaultValueCallback methodParamDefaultValueCallbackAt(ZF_IN zfindex index) const {
        zfCoreAssert(index < this->methodParamCount());
        return this->_ZFP_ZFMethod_paramDefaultValueCallbackList[index];
    }
    /**
     * @brief get the method's param default value at index, null if no default param
     */
    inline zfauto methodParamDefaultValueAt(ZF_IN zfindex index) const {
        zfCoreAssert(index < this->methodParamCount());
        if(index < this->methodParamDefaultBeginIndex()) {
            return zfnull;
        }
        else {
            return this->_ZFP_ZFMethod_paramDefaultValueCallbackList[index](this, index);
        }
    }
    /**
     * @brief return the first default param's index, or #zfindexMax if no default param
     */
    inline zfindex methodParamDefaultBeginIndex(void) const {
        return (this->_ZFP_ZFMethod_paramDefaultBeginIndex == (zfuint)-1 ? zfindexMax() : (zfindex)this->_ZFP_ZFMethod_paramDefaultBeginIndex);
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
     *     method->execute<void>(obj); // would be affected
     *   @endcode
     */
    void methodInvoker(ZF_IN ZFFuncAddrType methodInvoker) const;
    /**
     * @brief see #methodInvoker
     */
    inline ZFFuncAddrType methodInvokerOrig(void) const {
        return this->_ZFP_ZFMethod_invokerOrig;
    }

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
     *   var obj = MyZFObjectType::newInstance();
     *   var objParam = SomeType::newInstance();
     *   var result = obj.myFunc(v_zfnumber(1), v_zfstring("2"), objParam)
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
            , ZF_IN_OPT ZFObject *param1 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param2 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param3 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param4 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param5 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param6 = ZFMethodGenericInvokerDefaultParam()
            , ZF_IN_OPT ZFObject *param7 = ZFMethodGenericInvokerDefaultParam()
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
    inline ZFMethodGenericInvoker methodGenericInvokerOrig(void) const {
        return this->_ZFP_ZFMethod_methodGenericInvokerOrig;
    }

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
    inline const ZFClass *methodOwnerClass(void) const {
        return this->_ZFP_ZFMethod_methodOwnerClass;
    }
    /**
     * @brief method's owner proeprty, null if none
     */
    inline const ZFProperty *methodOwnerProperty(void) const {
        return this->_ZFP_ZFMethod_methodOwnerProperty;
    }

    /**
     * @brief get the method's access type
     */
    inline ZFMethodPrivilegeType methodPrivilegeType(void) const {
        return this->_ZFP_ZFMethod_privilegeType;
    }
    /** @brief util to check whether #methodPrivilegeType is #ZFMethodPrivilegeTypePublic */
    inline zfbool methodIsPublic(void) const {return this->_ZFP_ZFMethod_privilegeType == ZFMethodPrivilegeTypePublic;}
    /** @brief util to check whether #methodPrivilegeType is #ZFMethodPrivilegeTypeProtected */
    inline zfbool methodIsProtected(void) const {return this->_ZFP_ZFMethod_privilegeType == ZFMethodPrivilegeTypeProtected;}
    /** @brief util to check whether #methodPrivilegeType is #ZFMethodPrivilegeTypePrivate */
    inline zfbool methodIsPrivate(void) const {return this->_ZFP_ZFMethod_privilegeType == ZFMethodPrivilegeTypePrivate;}

    /**
     * @brief method type
     */
    inline ZFMethodType methodType(void) const {
        return this->_ZFP_ZFMethod_methodType;
    }
    /** @brief util to check whether #methodType is #ZFMethodTypeNormal */
    inline zfbool methodIsNormal(void) const {return this->_ZFP_ZFMethod_methodType == ZFMethodTypeNormal;}
    /** @brief util to check whether #methodType is #ZFMethodTypeStatic */
    inline zfbool methodIsStatic(void) const {return this->_ZFP_ZFMethod_methodType == ZFMethodTypeStatic;}
    /** @brief util to check whether #methodType is #ZFMethodTypeVirtual */
    inline zfbool methodIsVirtual(void) const {return this->_ZFP_ZFMethod_methodType == ZFMethodTypeVirtual;}

    // ============================================================
    // func type
public:
    /**
     * @brief true if this method is function type (declared by ZFMETHOD_FUNC_DECLARE_XXX series)
     */
    inline zfbool methodIsFunctionType(void) const {
        return (this->_ZFP_ZFMethod_methodOwnerClass == zfnull);
    }
    /**
     * @brief get the method namespace, for func type only,
     *   null or empty string for global scope (#ZF_NAMESPACE_GLOBAL)
     */
    inline const zfchar *methodNamespace(void) const {
        return this->_ZFP_ZFMethod_methodNamespace;
    }

    // ============================================================
    // other
public:
    /**
     * @brief whether the method is internal method
     *
     * all method that have "_ZFP_" as prefix would be treated as internal method\n
     * internal method should not be used publicly,
     * however, you may still apply reflection,
     * see #methodIsInternalPrivate
     */
    inline zfbool methodIsInternal(void) const {
        return this->_ZFP_ZFMethod_methodIsInternal;
    }
    /**
     * @brief whether the method is internal private method
     *
     * all method that have "_ZFP_I_" as prefix would be treated as internal private method\n
     * internal private method should not be used publicly,
     * also, it would be ignored from reflection,
     * see #methodIsInternal
     */
    inline zfbool methodIsInternalPrivate(void) const {
        return this->_ZFP_ZFMethod_methodIsInternalPrivate;
    }

public:
    ZFMethod *_ZFP_ZFMethod_removeConst(void) const {
        return const_cast<ZFMethod *>(this);
    }

public:
    // general
    zfuint _ZFP_ZFMethod_refCount;
    const ZFMethod *_ZFP_ZFMethod_methodAliasFrom;
    ZFCoreArray<const ZFMethod *> _ZFP_ZFMethod_methodAliasTo;
    zfchar *_ZFP_ZFMethod_methodInternalId;
    zfbool _ZFP_ZFMethod_methodIsUserRegister;
    ZFObject *_ZFP_ZFMethod_methodUserRegisterUserData;
    zfbool _ZFP_ZFMethod_methodIsDynamicRegister;
    ZFObject *_ZFP_ZFMethod_methodDynamicRegisterUserData;
    ZFFuncAddrType _ZFP_ZFMethod_invoker;
    ZFFuncAddrType _ZFP_ZFMethod_invokerOrig;
    ZFMethodGenericInvoker _ZFP_ZFMethod_methodGenericInvoker;
    ZFMethodGenericInvoker _ZFP_ZFMethod_methodGenericInvokerOrig;
    ZFSigName _ZFP_ZFMethod_methodName;
    ZFSigName _ZFP_ZFMethod_returnTypeId;
    ZFSigName _ZFP_ZFMethod_returnTypeName;
    zfuint _ZFP_ZFMethod_paramCount;
    zfuint _ZFP_ZFMethod_paramCountMin;
    ZFSigName *_ZFP_ZFMethod_paramBuf;
    ZFSigName *_ZFP_ZFMethod_paramTypeIdList;
    ZFSigName *_ZFP_ZFMethod_paramTypeNameList;
    ZFSigName *_ZFP_ZFMethod_paramNameList;
    ZFMethodParamDefaultValueCallback *_ZFP_ZFMethod_paramDefaultValueCallbackList;
    ZFCoreArray<zfauto> _ZFP_ZFMethod_paramDefaultValueList;
    zfuint _ZFP_ZFMethod_paramDefaultBeginIndex;

    // for class member type
    const ZFClass *_ZFP_ZFMethod_methodOwnerClass;
    const ZFProperty *_ZFP_ZFMethod_methodOwnerProperty;
    ZFMethodPrivilegeType _ZFP_ZFMethod_privilegeType;
    ZFMethodType _ZFP_ZFMethod_methodType;

    // for func type
    ZFSigName _ZFP_ZFMethod_methodNamespace;

    // other
    zfbool _ZFP_ZFMethod_methodIsInternal;
    zfbool _ZFP_ZFMethod_methodIsInternalPrivate;
};

// ============================================================
extern ZFLIB_ZFCore void _ZFP_ZFMethodDataHolderInit(void);
extern ZFLIB_ZFCore ZFMethod *_ZFP_ZFMethodRegister(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ...
        );
extern ZFLIB_ZFCore ZFMethod *_ZFP_ZFMethodRegisterV(
        ZF_IN zfbool methodIsUserRegister
        , ZF_IN zfbool methodIsDynamicRegister
        , ZF_IN ZFObject *methodDynamicRegisterUserData
        , ZF_IN ZFFuncAddrType methodInvoker
        , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
        , ZF_IN ZFMethodType methodType
        , ZF_IN const ZFClass *methodOwnerClass
        , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
        , ZF_IN const zfchar *methodNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN const zfchar *returnTypeId
        , ZF_IN const zfchar *returnTypeName
        /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
        , ZF_IN va_list vaList
        );
extern ZFLIB_ZFCore void _ZFP_ZFMethodUnregister(ZF_IN const ZFMethod *method);

zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFMethodRegisterHolder {
public:
    _ZFP_ZFMethodRegisterHolder(
            ZF_IN zfbool methodIsUserRegister
            , ZF_IN zfbool methodIsDynamicRegister
            , ZF_IN ZFObject *methodDynamicRegisterUserData
            , ZF_IN ZFFuncAddrType methodInvoker
            , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
            , ZF_IN ZFMethodType methodType
            , ZF_IN const ZFClass *methodOwnerClass
            , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
            , ZF_IN const zfchar *methodNamespace
            , ZF_IN const zfchar *methodName
            , ZF_IN const zfchar *returnTypeId
            , ZF_IN const zfchar *returnTypeName
            /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
            , ...
            );
    _ZFP_ZFMethodRegisterHolder(
            ZF_IN zfbool dummy
            , ZF_IN zfbool methodIsUserRegister
            , ZF_IN zfbool methodIsDynamicRegister
            , ZF_IN ZFObject *methodDynamicRegisterUserData
            , ZF_IN ZFFuncAddrType methodInvoker
            , ZF_IN ZFMethodGenericInvoker methodGenericInvoker
            , ZF_IN ZFMethodType methodType
            , ZF_IN const ZFClass *methodOwnerClass
            , ZF_IN ZFMethodPrivilegeType methodPrivilegeType
            , ZF_IN const zfchar *methodNamespace
            , ZF_IN const zfchar *methodName
            , ZF_IN const zfchar *returnTypeId
            , ZF_IN const zfchar *returnTypeName
            /* ParamTypeIdString, ParamTypeName, ParamName, DefaultValueAccessCallback, end with zfnull */
            , ZF_IN va_list vaList
            );
    ~_ZFP_ZFMethodRegisterHolder(void);
public:
    ZFMethod *method;
};

// ============================================================
zfclassFwd ZFFilterForZFMethod;
/** @brief see #ZFMethodGetAll */
extern ZFLIB_ZFCore void ZFMethodGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN_OPT const ZFFilterForZFMethod *methodFilter = zfnull
        );
/**
 * @brief get all method currently registered
 *
 * note, this method may cause performance issue
 * if you have many method registered,
 * use with caution\n
 * use #ZFClass::methodForName or #ZFMethodFuncForName if necessary
 */
inline ZFCoreArray<const ZFMethod *> ZFMethodGetAll(ZF_IN_OPT const ZFFilterForZFMethod *methodFilter = zfnull) {
    ZFCoreArray<const ZFMethod *> ret;
    ZFMethodGetAllT(ret, methodFilter);
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
        ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        );
/** @brief see #ZFMethodForName */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodForName(
        ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        , ZF_IN_OPT const zfchar *methodParamTypeId0
        , ZF_IN_OPT const zfchar *methodParamTypeId1 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId2 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId3 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId4 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId5 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId6 = zfnull
        , ZF_IN_OPT const zfchar *methodParamTypeId7 = zfnull
        );

/**
 * @brief util method to find method, see ZFMethodForName
 */
extern ZFLIB_ZFCore void ZFMethodForNameGetAllT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
        );
/** @brief see #ZFMethodForNameGetAllT */
inline ZFCoreArray<const ZFMethod *> ZFMethodForNameGetAll(
        ZF_IN const zfchar *classNameOrNamespace
        , ZF_IN const zfchar *methodName
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
 *       zfLogTrim() << "foo()";
 *   }
 *
 *   ZFMethodAlias(ZFMethodAccess(MyClass, foo), "bar");
 *   zfLogTrim() << MyClass::ClassData()->methodForName("foo");
 *   zfLogTrim() << MyClass::ClassData()->methodForName("bar");
 *   zfobj<MyClass> obj;
 *   obj->invoke("foo");
 *   obj->invoke("bar");
 * @endcode
 *
 * @see ZFClassAlias
 */
extern ZFLIB_ZFCore const ZFMethod *ZFMethodAlias(
        ZF_IN const ZFMethod *method
        , ZF_IN const zfchar *aliasName
        );
/**
 * @brief see #ZFMethodAlias
 */
extern ZFLIB_ZFCore void ZFMethodAliasRemove(ZF_IN const ZFMethod *aliasMethod);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethod_h_


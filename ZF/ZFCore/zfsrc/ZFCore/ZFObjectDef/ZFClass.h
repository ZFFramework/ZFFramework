/**
 * @file ZFClass.h
 * @brief meta data for ZFObject
 */

#ifndef _ZFI_ZFClass_h_
#define _ZFI_ZFClass_h_

#include "zfautoFwd.h"
#include "ZFObjectObserver.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #zfconv */
typedef zfbool (*zfconvImpl)(ZF_OUT zfauto &ret, ZF_IN ZFObject *obj);

// ============================================================
typedef ZFObject *(*_ZFP_ZFObjectConstructor)(void);
typedef void (*_ZFP_ZFObjectDestructor)(ZF_IN ZFObject *obj);
typedef void (*_ZFP_ZFObjectCheckInitImplementationListCallback)(ZF_IN_OUT ZFClass *cls);
typedef ZFInterface * (*_ZFP_ZFObjectToInterfaceCastCallback)(ZF_IN ZFObject *obj);

typedef ZFObject *(*_ZFP_zfobjAllocCacheCallback)(void);
typedef void (*_ZFP_zfobjAllocCacheReleaseCallback)(ZF_IN ZFObject *obj);

// ============================================================
/** @brief see #ZFClass::instanceObserverAdd */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFClassInstanceObserverAddParam {
    ZFCORE_PARAM_DECLARE_SELF(ZFClassInstanceObserverAddParam)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM(ZFListener, observer)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM(zfany, owner, zfnull)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM(ZFLevel, observerLevel, ZFLevelAppNormal)

    /** @brief see #ZFClass::instanceObserverAdd */
    ZFCORE_PARAM(zfbool, observeAllChildType, zftrue)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN ZFClassInstanceObserverAddParam const &ref) const {
        return (zftrue
                && this->observer() == ref.observer()
                && this->owner() == ref.owner()
                && this->observerLevel() == ref.observerLevel()
                && this->observeAllChildType() == ref.observeAllChildType()
            );
    }
    zfbool operator != (ZF_IN ZFClassInstanceObserverAddParam const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
};

// ============================================================
zfclassFwd _ZFP_ZFClassPrivate;
zfclassFwd _ZFP_ZFObjectPrivate;
/**
 * @brief ZFObject's class info
 * @see ZFObject
 */
zffinal zfclassNotPOD ZFLIB_ZFCore ZFClass {
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFClass)

    // ============================================================
    // static methods
public:
    /**
     * @brief get class info by name
     * @return class info or zfnull if not found
     *
     * typical usage:
     * @code
     *   const ZFClass *cls = ZFClass::classForName("MyObject");
     *   zfauto objTmp = ((cls != zfnull) ? cls->newInstance() : zfauto());
     *   MyObject *obj = objTmp;
     * @endcode
     * @note the class to find must:
     *   -  inherit from ZFObject or its subclass
     *   -  have #ZFOBJECT_DECLARE declared in the class
     *   -  have #ZFOBJECT_REGISTER defined,
     *      or match the situation described in #ZFOBJECT_REGISTER
     *
     * @see ZFOBJECT_DECLARE, ZFOBJECT_REGISTER
     *
     * @note the className may be simple format ("MyObject")
     *   or full format ("Scope0.Scope1.MyObject"),
     *   while in simple format,
     *   the class must be placed in global scope (#ZF_NAMESPACE_GLOBAL)
     * @note for #ZFTypeIdWrapper, the class name would be "YourType" instead of "v_YourType"
     */
    static const ZFClass *classForName(ZF_IN const zfstring &classNameOrFullName);
    /**
     * @brief find class with explicit namespace, see #classForName
     */
    static const ZFClass *classForName(
            ZF_IN const zfstring &className
            , ZF_IN const zfstring &classNamespace
            );

    // ============================================================
    // instance observer
public:
    /**
     * @brief add an observer which would be called if any of this class's instance created
     */
    void instanceObserverAdd(
            ZF_IN const ZFListener &observer
            , ZF_IN_OPT ZFObject *owner = zfnull
            , ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal
            , ZF_IN_OPT zfbool observeAllChildType = zftrue
            ) const;
    /** @brief see #instanceObserverAdd */
    inline void instanceObserverAdd(ZF_IN const ZFClassInstanceObserverAddParam &param) const {
        this->instanceObserverAdd(param.observer(), param.owner(), param.observerLevel(), param.observeAllChildType());
    }
    /** @brief see #instanceObserverAdd */
    void instanceObserverRemove(ZF_IN const ZFListener &observer) const;
    /** @brief see #instanceObserverAdd */
    void instanceObserverRemoveByOwner(ZF_IN ZFObject *owner) const;
    /**
     * @brief see #instanceObserverAdd
     *
     * @note it's unsafe to remove all observer manually,
     *   which may break unrelated modules' observer logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    void instanceObserverRemoveAll(void) const;
    void _ZFP_ZFClass_instanceObserverNotify(ZF_IN ZFObject *obj) const;

    // ============================================================
    // class info
public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    /** @brief see #objectInfoOfInheritTree */
    void objectInfoOfInheritTreeT(ZF_IN_OUT zfstring &ret) const;
    /**
     * @brief return a string describe the class inherit tree, debug use only
     */
    zfstring objectInfoOfInheritTree(void) const {
        zfstring ret;
        this->objectInfoOfInheritTreeT(ret);
        return ret;
    }

public:
    /**
     * @brief true if class is subclass of cls, or implement cls
     */
    zfbool classIsTypeOf(ZF_IN const ZFClass *cls) const;

public:
    /** @brief see #ZFClassAlias */
    inline const ZFCoreArray<zfstring> &classAliasTo(void) const {
        return this->_ZFP_ZFClass_classAliasTo;
    }

    /**
     * @brief class namespace, null or empty string for global scope class
     */
    inline const zfstring &classNamespace(void) const {
        return this->_ZFP_ZFClass_classNamespace;
    }
    /**
     * @brief class name, e.g. "ZFObject"
     */
    inline const zfstring &className(void) const {
        return this->_ZFP_ZFClass_className;
    }
    /**
     * @brief class full name, e.g. "NS0.NS1.YourObject"
     */
    inline const zfstring &classNameFull(void) const {
        return this->_ZFP_ZFClass_classNameFull;
    }

    /**
     * @brief class's parent, zfnull if none
     */
    inline const ZFClass *classParent(void) const {
        return this->_ZFP_ZFClass_classParent;
    }

    /**
     * @brief true if class is registered by #ZFClassDynamicRegister
     */
    zfbool classIsDynamicRegister(void) const;
    /**
     * @brief see #ZFClassDynamicRegister
     */
    zfany classDynamicRegisterUserData(void) const;
    /**
     * @brief true if the class is abstract class
     *
     * #newInstance would return zfnull if the class is abstract
     */
    inline zfbool classIsAbstract(void) const {
        return this->_ZFP_ZFClass_classIsAbstract;
    }
    /**
     * @brief true if the class is an interface
     */
    inline zfbool classIsInterface(void) const {
        return this->_ZFP_ZFClass_classIsInterface;
    }
    /**
     * @brief whether the class is internal class
     *
     * all class that have "_ZFP_" as prefix (or parent scope prefix) would be treated as internal class\n
     * internal class should not be used publicly,
     * however, you may still apply alloc observer or reflection,
     * see #classIsInternalPrivate
     */
    inline zfbool classIsInternal(void) const {
        return this->_ZFP_ZFClass_classIsInternal;
    }
    /**
     * @brief whether the class is internal private class
     *
     * all class that have "_ZFP_I_" as prefix (or parent scope prefix) would be treated as internal private class\n
     * internal private class should not be used publicly,
     * also, it would be ignored from alloc observer and reflection,
     * see #classIsInternal
     */
    inline zfbool classIsInternalPrivate(void) const {
        return this->_ZFP_ZFClass_classIsInternalPrivate;
    }

    /**
     * @brief whether the class can alloc directly,
     *   see #ZFOBJECT_PRIVATE_ALLOC
     *
     * when marked by #ZFOBJECT_PRIVATE_ALLOC,
     * the class can not be created normally (#zfobjAlloc or #ZFDI_alloc),
     * typically it's protected by impl to prevent internal class from being allocated accidently,
     * but you can still create by reflection,
     * i.e. #newInstance series
     */
    inline zfbool classCanAllocPublic(void) const {
        return this->_ZFP_ZFClass_classCanAllocPublic;
    }

    /**
     * @brief make a new instance of ZFObject dynamically, which is described by ZFClass
     *
     * typical usage:
     * @code
     *   ZFObject *myObj = zfobjAlloc(MyObject);
     *   ...
     *   // OK, newObj is a new instance of MyObject
     *   zfauto newObjTmp = myObj->classData()->newInstance();
     *   ZFObject *newObj = newObjTmp;
     * @endcode
     * @note object's no param version of objectOnInit would be called,
     *   if you want to construct with params,
     *   use #newInstanceGenericBegin instead
     */
    zfauto newInstance(void) const;

    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief for advanced reflection use only
     *
     * create object by specified params,
     * the object must have proper #ZFObject::objectOnInit
     * declared as #ZFOBJECT_ON_INIT_DECLARE_1 series,
     * and params are passed according to #ZFMethodGenericInvoker
     * @note this method do much reflection steps to find proper objectOnInit,
     *   which may cause performance issue,
     *   use with caution,
     *   or use #newInstanceGenericBegin if available:
     *   @code
     *     zfauto result;
     *     ZFCoreArray<const ZFMethod *> objectOnInitMethodList;
     *     cls->methodForNameGetAllT(objectOnInitMethodList, "objectOnInit");
     *     // you may do your extra method filters before actual alloc the object
     *     void *token = cls->newInstanceGenericBegin();
     *     if(token != zfnull) {
     *         for(zfindex i = 0; i < objectOnInitMethodList.count(); ++i) {
     *             if(cls->newInstanceGenericCheck(token, objectOnInitMethodList[i], zfargs)) {
     *                 result = cls->newInstanceGenericEnd(token, zftrue);
     *                 break;
     *             }
     *         }
     *         if(result == zfnull) {
     *             cls->newInstanceGenericEnd(token, zffalse);
     *         }
     *     }
     *   @endcode
     * @note if the objectOnInit is declared by #ZFOBJECT_ON_INIT_DECLARE_1 series,
     *   it's called as is,
     *   if it's user registered (#ZFMETHOD_USER_REGISTER_1 series),
     *   #ZFObject::objectOnInit would be called before invoke the method
     *   to prevent logic error
     * @note if all params are #ZFMP_DEF, this method would call
     *   original #ZFObject::objectOnInit instead (same as #newInstance)
     *
     * see also #ZFObjectTagKeyword_newInstanceGenericFailed
     */
    zfauto newInstance(
            ZF_IN ZFObject *param0
            , ZF_IN_OPT ZFObject *param1 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param2 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param3 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param4 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param5 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param6 = ZFMP_DEF()
            , ZF_IN_OPT ZFObject *param7 = ZFMP_DEF()
            ) const;
    /** @brief see #newInstance */
    zfauto newInstanceDetail(
            ZF_IN const ZFCoreArray<zfauto> &params
            , ZF_OUT_OPT zfbool *success = zfnull
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            ) const;

    /** @brief see #newInstance */
    void *newInstanceGenericBegin(void) const;
    /** @brief see #newInstance */
    zfbool newInstanceGenericCheck(
            ZF_IN void *&token
            , ZF_IN const ZFMethod *objectOnInitMethod
            , ZF_IN_OUT const ZFArgs &zfargs
            ) const;
    /** @brief see #newInstance */
    zfauto newInstanceGenericEnd(
            ZF_IN void *&token
            , ZF_IN zfbool objectOnInitMethodInvokeSuccess
            ) const;

    zffinal zfauto _ZFP_ZFClass_newInstance(ZF_IN _ZFP_ZFObjectPrivate *dObj) const;

    /**
     * @brief get implemented interface count, see #dynamicInterfaceAt
     */
    zfindex implementedInterfaceCount(void) const;
    /**
     * @brief get implemented interface, see #dynamicInterfaceAt
     */
    const ZFClass *implementedInterfaceAt(ZF_IN zfindex index) const;
    /**
     * @brief get dynamic interface registered by #ZFImplementDynamicRegister, see #implementedInterfaceAt
     */
    zfindex dynamicInterfaceCount(void) const;
    /**
     * @brief get dynamic interface registered by #ZFImplementDynamicRegister, see #implementedInterfaceAt
     */
    const ZFClass *dynamicInterfaceAt(ZF_IN zfindex index) const;
    /**
     * @brief true if this class dynamic implement specified parent by #ZFImplementDynamicRegister
     */
    zfbool dynamicImplementOf(ZF_IN const ZFClass *parent) const;

    /**
     * @brief get all child excluding self, for debug use only
     */
    ZFCoreArray<const ZFClass *> childGetAll(void) const;
    /**
     * @brief get all parent (parent/interface/dynamicInterface) excluding self, for debug use only
     */
    ZFCoreArray<const ZFClass *> parentGetAll(void) const;

    // ============================================================
    // ZFMethod
public:
    /**
     * @brief get the count of method registered to this ZFClass
     *
     * note that parent class's method won't be included\n
     * the order is ensured to be the same as ZFMethod declared
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    zfindex methodCount(void) const;
    /** @brief see #zfiter */
    zfiter methodIter(void) const;
    /** @brief see #zfiter */
    const ZFMethod *methodIterValue(ZF_IN const zfiter &it) const;

    /** @brief see #methodGetAllIgnoreParent */
    void methodGetAllIgnoreParentT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const;
    /**
     * @brief get all method, excluding methods inherited from parent
     */
    ZFCoreArray<const ZFMethod *> methodGetAllIgnoreParent(void) const {
        ZFCoreArray<const ZFMethod *> ret;
        this->methodGetAllIgnoreParentT(ret);
        return ret;
    }

    /** @brief see #methodGetAll */
    void methodGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret) const;
    /**
     * @brief get all method, including methods inherited from parent
     */
    ZFCoreArray<const ZFMethod *> methodGetAll(void) const {
        ZFCoreArray<const ZFMethod *> ret;
        this->methodGetAllT(ret);
        return ret;
    }

    /* ZFMETHOD_MAX_PARAM */
    /**
     * @brief get the method by name within this class only, or zfnull if not exists
     *
     * the following optional params are, type id of each param type
     * described by #ZFTypeId::TypeId,
     * if more than one method with same name (and or param type id) were found,
     * the first one would be returned
     *
     * @see methodForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFMethod *methodForNameIgnoreParent(
            ZF_IN const zfstring &methodName
            , ZF_IN const zfchar *paramTypeId0
            , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
            ) const;
    /** @brief see #methodForNameIgnoreParent */
    const ZFMethod *methodForNameIgnoreParent(ZF_IN const zfstring &methodName) const;
    /** @brief see #methodForNameIgnoreParent */
    void methodForNameGetAllIgnoreParentT(
            ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
            , ZF_IN const zfstring &methodName
            ) const;
    /** @brief see #methodForNameIgnoreParent */
    ZFCoreArray<const ZFMethod *> methodForNameGetAllIgnoreParent(ZF_IN const zfstring &methodName) const {
        ZFCoreArray<const ZFMethod *> ret;
        this->methodForNameGetAllIgnoreParentT(ret, methodName);
        return ret;
    }
    /**
     * @brief get the method by name hierarchically from class inherit tree, or zfnull if not exists
     *
     * searching from subclass to parent class, return the first matched
     * @see methodForNameIgnoreParent
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     * @note ensured breadth-first
     */
    const ZFMethod *methodForName(
            ZF_IN const zfstring &methodName
            , ZF_IN const zfchar *paramTypeId0
            , ZF_IN_OPT const zfchar *paramTypeId1 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId2 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId3 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId4 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId5 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId6 = zfnull
            , ZF_IN_OPT const zfchar *paramTypeId7 = zfnull
            ) const;
    /** @brief see #methodForName */
    const ZFMethod *methodForName(ZF_IN const zfstring &methodName) const;
    /** @brief see #methodForNameGetAll */
    void methodForNameGetAllT(
            ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
            , ZF_IN const zfstring &methodName
            ) const;
    /**
     * @brief get all method with name, ignoring method id
     */
    ZFCoreArray<const ZFMethod *> methodForNameGetAll(ZF_IN const zfstring &methodName) const {
        ZFCoreArray<const ZFMethod *> ret;
        this->methodForNameGetAllT(ret, methodName);
        return ret;
    }

    // ============================================================
    // ZFProperty
public:
    /**
     * @brief get ZFProperty count registered to this class
     *
     * note that parent class's property won't be included\n
     * the order is ensured to be the same as property declared
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    zfindex propertyCount(void) const;
    /** @brief see #zfiter */
    zfiter propertyIter(void) const;
    /** @brief see #zfiter */
    const ZFProperty *propertyIterValue(ZF_IN const zfiter &it) const;

    /** @brief see #propertyGetAllIgnoreParent */
    void propertyGetAllIgnoreParentT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const;
    /**
     * @brief get all property, including properties inherited from parent
     */
    ZFCoreArray<const ZFProperty *> propertyGetAllIgnoreParent(void) const {
        ZFCoreArray<const ZFProperty *> ret;
        this->propertyGetAllIgnoreParentT(ret);
        return ret;
    }

    /** @brief see #propertyGetAll */
    void propertyGetAllT(ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret) const;
    /**
     * @brief get all property, including properties inherited from parent
     */
    ZFCoreArray<const ZFProperty *> propertyGetAll(void) const {
        ZFCoreArray<const ZFProperty *> ret;
        this->propertyGetAllT(ret);
        return ret;
    }

    /**
     * @brief get the property by name within this class only, or zfnull if not exists
     *
     * propertyName should be "Property" without "set" or "get"\n
     * note that parent class's property won't be included
     * @see propertyForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     */
    const ZFProperty *propertyForNameIgnoreParent(ZF_IN const zfstring &propertyName) const;
    /**
     * @brief get the property by name hierarchically from class inherit tree, or zfnull if not exists
     *
     * propertyName should be "Property" without "set" or "get"\n
     * searching from subclass to parent class, return the first matched
     * @see propertyForName
     * @note can be found only if it matches the condition described in #ZFOBJECT_REGISTER
     * @note ensured breadth-first
     */
    const ZFProperty *propertyForName(ZF_IN const zfstring &propertyName) const;

    /**
     * @brief try to get property setter method within this class only
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertySetterForNameIgnoreParent(ZF_IN const zfstring &propertyName) const;
    /**
     * @brief try to get property setter method
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertySetterForName(ZF_IN const zfstring &propertyName) const;
    /**
     * @brief try to get property getter method within this class only
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertyGetterForNameIgnoreParent(ZF_IN const zfstring &propertyName) const;
    /**
     * @brief try to get property getter method
     *
     * note, the property has no need to exist,
     * we only search method with proper name and param and return type
     */
    const ZFMethod *propertyGetterForName(ZF_IN const zfstring &propertyName) const;

    /**
     * @brief whether the property has #ZFPROPERTY_ON_INIT_DECLARE
     */
    zfbool propertyInitStepExist(void) const;
    /**
     * @brief whether the property has #ZFPROPERTY_ON_INIT_DECLARE
     */
    zfbool propertyInitStepExist(ZF_IN const ZFProperty *property) const;

    /**
     * @brief whether two class has same property init step
     */
    zfbool propertyInitStepIsEqual(
            ZF_IN const ZFProperty *property
            , ZF_IN const ZFClass *refClass
            ) const;

public:
    /** @brief see #zfconv */
    void zfconvRegister(ZF_IN const ZFClass *dstCls, ZF_IN zfconvImpl impl) const;
    /** @brief see #zfconv */
    void zfconvUnregister(ZF_IN const ZFClass *dstCls) const;
    /** @brief see #zfconv */
    zfconvImpl zfconvCheck(ZF_IN const zfstring &dstCls) const;
    /** @brief see #zfconv */
    zfconvImpl zfconvCheck(ZF_IN const ZFClass *dstCls) const;

    // ============================================================
    // class instance methods
public:
    /**
     * @brief set a classTag for the class, internal use only
     *
     * @note all classTags would be removed during #ZFFrameworkCleanup
     *   as #ZFLevelZFFrameworkHigh and #ZFLevelZFFrameworkEssential,
     *   you must ensure the classTag is safe to be deleted at this time
     */
    void classTag(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *tag
            ) const;
    /** @brief see #classTag */
    zfany classTag(ZF_IN const zfstring &key) const;
    /** @brief see #classTag */
    zffinal void classTagGetAllKeyValue(
            ZF_IN_OUT ZFCoreArray<zfstring> &allKey
            , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
            ) const;
    /** @brief see #classTag */
    inline void classTagRemove(ZF_IN const zfstring &key) const {
        this->classTag(key, zfnull);
    }
    /** @brief see #classTag */
    zffinal zfauto classTagRemoveAndGet(ZF_IN const zfstring &key) const;
    /**
     * @brief see #classTag
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void classTagRemoveAll(void) const;

    // ============================================================
    /**
     * @brief store class meta data cache, internal use only
     *
     * @note all dataCaches would be removed during #ZFFrameworkCleanup
     *   as #ZFLevelZFFrameworkHigh and #ZFLevelZFFrameworkEssential,
     *   you must ensure the dataCache is safe to be deleted at this time
     * @note usually used to store meta-data for performance use only
     * @note all dataCaches would be cleared during #ZFGlobalEvent::E_ClassDataUpdate
     */
    void dataCache(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *tag
            ) const;
    /** @brief see #dataCache */
    zfany dataCache(ZF_IN const zfstring &key) const;
    /** @brief see #dataCache */
    zffinal void dataCacheGetAllKeyValue(
            ZF_IN_OUT ZFCoreArray<zfstring> &allKey
            , ZF_IN_OUT ZFCoreArray<zfauto> &allValue
            ) const;
    /** @brief see #dataCache */
    inline void dataCacheRemove(ZF_IN const zfstring &key) const {
        this->dataCache(key, zfnull);
    }
    /** @brief see #dataCache */
    zffinal zfauto dataCacheRemoveAndGet(ZF_IN const zfstring &key) const;
    /** @brief see #dataCache */
    zffinal void dataCacheRemoveAll(void) const;

    // ============================================================
    /**
     * @brief a unique id that represents the class
     * @note the id may be reused if owner class unloaded
     */
    zffinal zfidentity classId(void) const {
        return _ZFP_ZFClass_classNameFull.sigId();
    }

    // ============================================================
    // private
public:
    static ZFClass *_ZFP_ZFClassRegister(
            ZF_IN const zfstring &classNamespace
            , ZF_IN const zfstring &className
            , ZF_IN const ZFClass *parent
            , ZF_IN const ZFClass *outer
            , ZF_IN zfbool classCanAllocPublic
            , ZF_IN _ZFP_zfobjAllocCacheCallback objectAllocWithCacheCallback
            , ZF_IN _ZFP_ZFObjectConstructor constructor
            , ZF_IN _ZFP_ZFObjectDestructor destructor
            , ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback
            , ZF_IN zfbool classIsInterface
            , ZF_IN zfbool classIsDynamicRegister
            , ZF_IN ZFObject *classDynamicRegisterUserData
            );
    static void _ZFP_ZFClassUnregister(ZF_IN const ZFClass *cls);
    /** @cond ZFPrivateDoc */
    ZFClass(void);
    ~ZFClass(void);
    /** @endcond */
    void _ZFP_ZFClass_autoRegister(void) const;
    zfbool _ZFP_ZFClass_interfaceNeedRegister(void);
    void _ZFP_ZFClass_interfaceRegister(
            ZF_IN zfint dummy
            , ZF_IN const ZFClass *cls
            , ZF_IN _ZFP_ZFObjectToInterfaceCastCallback callback
            , ...
            );
    ZFInterface *_ZFP_ZFClass_interfaceCast(
            ZF_IN ZFObject *obj
            , ZF_IN const ZFClass *interfaceClass
            ) const;
    ZFObject *_ZFP_ZFClass_objectCast(
            ZF_IN ZFObject *obj
            , ZF_IN const ZFClass *objectClass
            ) const;

    zfbool _ZFP_ZFClass_ZFImplementDynamicRegister(ZF_IN const ZFClass *clsToImplement) const;
    void _ZFP_ZFClass_ZFImplementDynamicUnregister(ZF_IN const ZFClass *clsToImplement) const;

    void _ZFP_ZFClass_objectDesctuct(ZF_IN ZFObject *obj) const;
    ZFClass *_ZFP_ZFClass_removeConst(void) const {
        return const_cast<ZFClass *>(this);
    }

    void _ZFP_ZFClass_methodRegister(ZF_IN const ZFMethod *method) const;
    void _ZFP_ZFClass_methodUnregister(ZF_IN const ZFMethod *method) const;
    void _ZFP_ZFClass_propertyRegister(ZF_IN const ZFProperty *zfproperty) const;
    void _ZFP_ZFClass_propertyUnregister(ZF_IN const ZFProperty *zfproperty) const;
    void _ZFP_ZFClass_propertyAutoInitRegister(ZF_IN const ZFProperty *property) const;
    void _ZFP_ZFClass_propertyAutoInitAction(ZF_IN ZFObject *owner) const;
    void _ZFP_ZFClass_propertyInitStepRegister(ZF_IN const ZFProperty *property) const;

    _ZFP_zfobjAllocCacheCallback _ZFP_objectAllocWithCacheCallback(void) const;
    _ZFP_ZFObjectConstructor _ZFP_objectConstructor(void) const;
    _ZFP_ZFObjectDestructor _ZFP_objectDestructor(void) const;
    void _ZFP_classDynamicRegisterObjectInstanceDetach(ZF_IN ZFObject *obj) const;

private:
    _ZFP_ZFClassPrivate *d;
    friend zfclassFwd _ZFP_ZFClassPrivate;
    const ZFClass *_ZFP_ZFClass_classParent;
    ZFSigName _ZFP_ZFClass_classNamespace;
    ZFSigName _ZFP_ZFClass_className;
    ZFSigName _ZFP_ZFClass_classNameFull;
    zfbool _ZFP_ZFClass_classIsAbstract;
    zfbool _ZFP_ZFClass_classIsInterface;
    zfbool _ZFP_ZFClass_classIsInternal;
    zfbool _ZFP_ZFClass_classIsInternalPrivate;
    zfbool _ZFP_ZFClass_methodAndPropertyCacheNeedUpdate;
public:
    zfbool _ZFP_ZFClass_implListNeedInit;
    zfbool _ZFP_ZFClass_classCanAllocPublic;
    ZFCoreArray<zfstring> _ZFP_ZFClass_classAliasTo;
};

/**
 * @brief used to mark object allocation failed for #ZFClass::newInstanceGenericCheck
 *
 * when object created by #ZFClass::newInstanceGenericBegin,
 * it's possible to have mismatch param for generic invoke of `objectOnInit`,
 * you may set an error message to the allocated object by #ZFObject::objectTag
 * with this as key and the error message (#v_zfstring) as value,
 * to tell #ZFClass::newInstanceGenericCheck that the allocation failed
 */
#define ZFObjectTagKeyword_newInstanceGenericFailed "_ZFP_NIGFail"

// ============================================================
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFClassRegisterHolder {
public:
    _ZFP_ZFClassRegisterHolder(
            ZF_IN const zfstring &classNamespace
            , ZF_IN const zfstring &className
            , ZF_IN const ZFClass *parent
            , ZF_IN const ZFClass *outer
            , ZF_IN zfbool classCanAllocPublic
            , ZF_IN _ZFP_zfobjAllocCacheCallback objectAllocWithCacheCallback
            , ZF_IN _ZFP_ZFObjectConstructor constructor
            , ZF_IN _ZFP_ZFObjectDestructor destructor
            , ZF_IN _ZFP_ZFObjectCheckInitImplementationListCallback checkInitImplListCallback
            , ZF_IN_OPT zfbool classIsInterface = zffalse
            , ZF_IN_OPT zfbool classIsDynamicRegister = zffalse
            , ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull
            );
    ~_ZFP_ZFClassRegisterHolder(void);
public:
    ZFClass *cls;
};

// ============================================================
/** @brief see #ZFClassGetAll */
extern ZFLIB_ZFCore void ZFClassGetAllT(ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret);
/**
 * @brief get all class currently registered, for debug use only
 */
inline ZFCoreArray<const ZFClass *> ZFClassGetAll(void) {
    ZFCoreArray<const ZFClass *> ret;
    ZFClassGetAllT(ret);
    return ret;
}

// ============================================================
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when any class data changed, notified to #ZFClassDataUpdateObserver,
 *   see #ZFObject::observerNotify
 *
 * param0 is a #v_ZFClassDataUpdateData\n
 * called when:
 * -  class registered or unregistered
 * -  class's property registered or unregistered (including user registered property)
 * -  class's method or function method registered or unregistered (including user registered method)
 *
 *
 * \n
 * for each event call, #ZFClassDataUpdateData::changedClass,
 * #ZFClassDataUpdateData::changedProperty
 * and #ZFClassDataUpdateData::changedMethod
 * are ensured only one not null\n
 * \n
 * \n
 * note this method would be notified only if #ZFFrameworkStateCheck with #ZFLevelZFFrameworkLow
 * returned #ZFFrameworkStateAvailable
 *
 * @warning app code can add observer to this event,
 *   but must not manually notify this event\n
 *   also, take care of performance
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ClassDataUpdate)
ZF_NAMESPACE_END(ZFGlobalEvent)

extern ZFLIB_ZFCore ZFObserver &_ZFP_ZFClassDataUpdateObserverRef(void);
/**
 * @brief see #ZFGlobalEvent::E_ClassDataUpdate
 *
 * @warning app code can add observer to this event,
 *   but must not manually notify this event\n
 *   also, take care of performance
 */
#define ZFClassDataUpdateObserver() (_ZFP_ZFClassDataUpdateObserverRef())

// ============================================================
zfclassNotPOD ZFLIB_ZFCore _ZFP_ZFCoreCriticalClassNotTypeOf {
public:
    static zfstring classInfo(ZF_IN const ZFClass *cls) {
        if(cls) {
            return cls->classNameFull();
        }
        else {
            return ZFTOKEN_zfnull;
        }
    }
};
/**
 * @brief log that likes "[file function (line)] class xxx not type of xxx"
 */
#define ZFCoreCriticalClassNotTypeOf(cls, desired) \
    ZFCoreCriticalMessage("class %s not type of %s", \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(cls), \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(desired))
/**
 * @brief see #ZFCoreCriticalClassNotTypeOf
 */
#define ZFCoreCriticalClassNotTypeOfDetail(callerInfo, cls, desired) \
    ZFCoreCriticalMessageDetail(callerInfo, "class %s not type of %s", \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(cls), \
        _ZFP_ZFCoreCriticalClassNotTypeOf::classInfo(desired))

// ============================================================
/**
 * @brief alias class to a new name
 *
 * class alias only affects reflection, typically used for script binding\n
 * you can alias more than one name to same class,
 * all of the aliased class points to same original #ZFClass
 * @code
 *   ZFClassAlias(ZFObject::ClassData(), "MyObject");
 *   ZFClassAlias(ZFObject::ClassData(), "MyClass");
 *
 *   // the aliased class is exactly same of the original,
 *   // so the reflected class name still prints "ZFObject"
 *   ZFLogTrim() << ZFClass::classForName("MyObject")->className();
 * @endcode
 * @see ZFMethodAlias
 */
extern ZFLIB_ZFCore void ZFClassAlias(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfstring &aliasNameFull
        );
/**
 * @brief see #ZFClassAlias
 */
extern ZFLIB_ZFCore void ZFClassAliasRemove(
        ZF_IN const ZFClass *cls
        , ZF_IN const zfstring &aliasNameFull
        );

// ============================================================
/**
 * @brief object conversion
 *
 * some objects can be cast implicitly,
 * `zfstring` to `const void *` for example,
 * this method was designed for this, usage:
 * @code
 *   // register once
 *   ZFCONV_REG(v_zfptr, v_zfstring) {
 *       ret = zfobj<v_zfptr>(obj->zfv.cString());
 *
 *       // usually, the result object should retain the obj,
 *       // to prevent unsafe pointer reference
 *       ret->objectTag("zfconv", obj);
 *
 *       return zftrue;
 *   }
 *
 *   // use, return converted object, or null if not available or convert failed
 *   zfauto converted = zfconv(ZFTypeId_zfptr(), zfobj<v_zfstring>());
 *
 *   // or use with explicit check
 *   zfauto ret;
 *   if(zfconvT(ret, ZFTypeId_zfptr(), zfobj<v_zfstring>())) {
 *       // conversion was success
 *   }
 * @endcode
 *
 * possible returns:
 * -  original object, if can be cast by #zfcast
 * -  empty object, if cls is #ZFTypeIdWrapper and obj is null
 * -  null, if obj is null
 * -  new object converted by #ZFCONV_REG
 *
 * extra possible returns if implicitConv is true:
 * -  new object by invoking constructor of desired class with source object as param
 * -  new object by #ZFDI_objectFromString
 */
extern ZFLIB_ZFCore zfauto zfconv(
        ZF_IN const zfstring &cls
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool implicitConv = zftrue
        );
/** @brief see #zfconv */
extern ZFLIB_ZFCore zfauto zfconv(
        ZF_IN const ZFClass *cls
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool implicitConv = zftrue
        );

/** @brief see #zfconv */
extern ZFLIB_ZFCore zfbool zfconvT(
        ZF_OUT zfauto &ret
        , ZF_IN const zfstring &cls
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool implicitConv = zftrue
        );
/** @brief see #zfconv */
extern ZFLIB_ZFCore zfbool zfconvT(
        ZF_OUT zfauto &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfbool implicitConv = zftrue
        );

/** @brief see #zfconv */
#define ZFCONV_REG(dstCls, srcCls) \
    static zfbool _ZFP_zfconv_##dstCls##_##srcCls(ZF_OUT zfauto &ret, ZF_IN ZFObject *obj); \
    ZF_STATIC_REGISTER_INIT(zfconv_##dstCls##_##srcCls) { \
        srcCls::ClassData()->zfconvRegister(dstCls::ClassData(), _ZFP_zfconv_##dstCls##_##srcCls); \
    } \
    ZF_STATIC_REGISTER_DESTROY(zfconv_##dstCls##_##srcCls) { \
        srcCls::ClassData()->zfconvUnregister(dstCls::ClassData()); \
    } \
    ZF_STATIC_REGISTER_END(zfconv_##dstCls##_##srcCls) \
    static zfbool _ZFP_zfconv_##dstCls##_##srcCls(ZF_OUT zfauto &ret, ZF_IN ZFObject *obj)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClass_h_


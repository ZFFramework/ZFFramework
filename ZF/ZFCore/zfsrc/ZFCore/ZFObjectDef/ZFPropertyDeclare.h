/**
 * @file ZFPropertyDeclare.h
 * @brief macros to declare ZFProperty
 */

#ifndef _ZFI_ZFPropertyDeclare_h_
#define _ZFI_ZFPropertyDeclare_h_

#include "ZFMethodDeclare.h"
#include "ZFObjectUtil.h"
#include "zfsynchronize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
// ============================================================
/**
 * @brief access the property directly, compile error if not declared
 */
#define ZFPropertyAccess(OwnerClass, Name) ((const ZFProperty *)OwnerClass::_ZFP_Prop_##Name())

/**
 * @brief get property by name, or null if not registered
 */
extern ZFLIB_ZFCore const ZFProperty *ZFPropertyForName(
        ZF_IN const zfstring &classNameOrFullName
        , ZF_IN const zfstring &propertyName
        );

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPropertyNoInitValue
#define _ZFP_PropInitV(...) __VA_ARGS__

// ============================================================
/**
 * @brief declare a retain property
 *
 * -  ZFPROPERTY_XXX is similar to \@property in Object-C,
 *   which would automatically generate setter and getter for you\n
 *   here's a list of what they'll generate
 *   (assume property's type is Type and property's name is yourPropName):
 *   -  ZFPROPERTY_RETAIN:
 *     -  public:\n
 *       virtual void yourPropName(Type const &propertyValue); // (reflectable)\n
 *       virtual Type const &yourPropName(void); // (reflectable)
 *   -  ZFPROPERTY_ASSIGN:
 *     -  public:\n
 *       virtual void yourPropName(Type const &propertyValue); // (reflectable)\n
 *       virtual Type const &yourPropName(void); // (reflectable)
 *   -  for a retain property, Type must be ZFObject or ZFObject's subclass,
 *     and value is set by #zfRetainChange,
 *     and would automatically be released when owner object deallocated
 *   -  for a assign property, Type could be any type except for const type
 *   -  when declare assign property as ZFObject type,
 *     it is automatically treated as weak property
 *     (similar to weak in Object-C)
 * -  ZFProperty support those type only:
 *   -  ZFObject *
 *   -  all types that registered by #ZFTYPEID_DECLARE
 * -  ZFProperty is also reflectable,
 *   you may reflect the ZFProperty itself by #ZFClass::propertyForName,
 *   or reflect the setter and getter as ZFMethod,
 *   for how to reflect, see #ZFMethod, #ZFOBJECT_REGISTER\n
 * -  you may customize the accessibility for getter and setter,
 *   or add default value for setter,
 *   by using ZFPROPERTY_XXX_DETAIL:\n
 *   ZFPROPERTY_RETAIN_DETAIL(
 *       Type, Name, InitValueOrEmpty,
 *       SetterAccessType, GetterAccessType)\n
 *   the InitValueOrEmpty is the init value for the property,
 *     could be your_value or ZFPropertyNoInitValue,
 *     if not set, the built-in value would be set,
 *     e.g. zffalse for zfbool, empty string for zfstring
 *     (for retain property, the init value must be auto released object,
 *     use #zfauto or #zfobj is recommended)\n
 *   the SetterAccessType/GetterAccessType could be
 *     public/protected/private\n
 *   for example:
 *   @code
 *     zfclass OwnerClass : zfextend Base {
 *         ZFOBJECT_DECLARE(OwnerClass, Base)
 *
 *         ZFPROPERTY_ASSIGN_DETAIL(
 *             zfstring, StringProperty, ZFPropertyNoInitValue,
 *             public, public)
 *     };
 *   @endcode
 * -  to override a property in subclass, you may use ZFPROPERTY_ON_XXX
 *   @warning you must not declare two property with same name in child and base class
 *   @note overrided property won't be included in it's ZFClass,
 *     it's only a method override, no new ZFMethod or ZFProperty would be declared in child class
 * -  to add Doxygen docs, you should:
 *   @code
 *    / **
 *      * @brief without fn defines, doc for getters only (ie. Name)
 *      *
 *      * setters' docs would be auto generated,
 *      * which is referencing to getters
 *      * /
 *     ZFPROPERTY_RETAIN(Type, Name)
 *   @endcode
 */
#define ZFPROPERTY_RETAIN(Type, Name, ...) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, _ZFP_PropInitV(__VA_ARGS__), \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_READONLY(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        protected, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_DETAIL( \
    Type, Name, InitValueOrEmpty, \
    SetterAccessType, GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER_RETAIN(GetterAccessType, Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_RETAIN(SetterAccessType, Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, zftTraits<Type>::TrType::ClassData()->classNameFull(), Name, \
                                       InitValueOrEmpty) \
    public:

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN(Type, Name, ...) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, _ZFP_PropInitV(__VA_ARGS__), \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_READONLY(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        protected, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_DETAIL( \
    Type, Name, InitValueOrEmpty, \
    SetterAccessType, GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER_ASSIGN(GetterAccessType, Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_ASSIGN(SetterAccessType, Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFTypeId<zftTraits<Type>::TrNoRef>::TypeId(), Name, \
                                       InitValueOrEmpty) \
    public:

// ============================================================
/**
 * @brief override life cycle callback for the property
 *
 * proto type:
 * @code
 *   // for retain property
 *   void propertyOnInit(
 *           ZF_IN_OUT zfauto [const] &propertyValue
 *           , ZF_IN zfauto const &propertyValueOld
 *           );
 *
 *   // for assign property
 *   void propertyOnInit(
 *           ZF_IN_OUT YourPropertyType [const] &propertyValue
 *           , ZF_IN YourPropertyType const &propertyValueOld
 *           );
 * @endcode
 *
 * we have these property life cycle events:
 * -  propertyOnInit: called when property first accessed\n
 *   you may change the propertyValue during this step
 *   to modify the property's init value
 * -  propertyOnUpdate: called when setter called\n
 *   you may verify the property value, and modify propertyValue to correct one
 * -  propertyOnAttach: called when property first accessed or setter called\n
 *   you may do additional attach steps here,
 *   or apply your impl according to the property's new value
 * -  propertyOnDetach: called when property setter called or owner object deallocated\n
 *   you may do additional detach steps here
 *
 *
 * after override, for detach events (detach/dealloc),
 * subclass's one would be called before parent's one,
 * for others (init/update/attach),
 * parent's one would be called first\n
 * \n
 * \n
 * here's a simple hint for the property's life cycle:
 * -  property first time accessed:
 *   -# store raw init value
 *   -# propertyOnInit
 *   -# propertyOnAttach
 * -  property's setter called:
 *   -# propertyOnDetach
 *   -# store raw value
 *   -# propertyOnUpdate
 *   -# propertyOnAttach
 *   -# #ZFObject::E_ObjectPropertyValueOnUpdate
 * -  property's owner object deallocated:
 *   -# propertyOnDetach
 *
 * @note if you override OnInit event from subclass of the property's owner class,
 *   the property's getter would be called during owner object's allocation
 *   to suit most logic case,
 *   which may cause unnecessary property allocation,
 *   to skip the step, use #ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT instead
 */
#define ZFPROPERTY_ON_INIT_DECLARE(Type, Name) \
    public: \
        static zfbool _ZFP_propLExt_##Name(void) {return zftrue;} \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnInit, ZFM_EMPTY)
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_INIT_DECLARE_NO_AUTO_INIT(Type, Name) \
    public: \
        static zfbool _ZFP_propLExt_##Name(void) {return zffalse;} \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnInit, ZFM_EMPTY)
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_INIT_DEFINE(OwnerClass, Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnInit, ZFM_EMPTY, ZFM_EXPAND( \
            OwnerClass::ClassData()->_ZFP_ZFClass_propertyAutoInitRegister(OwnerClass::_ZFP_Prop_##Name()); \
            if(OwnerClass::_ZFP_propLExt_##Name()) { \
                OwnerClass::ClassData()->_ZFP_ZFClass_propertyInitStepRegister(OwnerClass::_ZFP_Prop_##Name()); \
            } \
        ))
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_INIT_INLINE(Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, OnInit, ZFM_EMPTY, ZFM_EXPAND( \
            zfself::ClassData()->_ZFP_ZFClass_propertyAutoInitRegister(zfself::_ZFP_Prop_##Name()); \
            zfself::ClassData()->_ZFP_ZFClass_propertyInitStepRegister(zfself::_ZFP_Prop_##Name()); \
        ))
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_INIT_INLINE_NO_AUTO_INIT(Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, OnInit, ZFM_EMPTY, ZFM_EXPAND( \
            zfself::ClassData()->_ZFP_ZFClass_propertyInitStepRegister(zfself::_ZFP_Prop_##Name()); \
        ))

/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_UPDATE_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnUpdate, ZFM_EMPTY)
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_UPDATE_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnUpdate, ZFM_EMPTY, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_UPDATE_INLINE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, OnUpdate, ZFM_EMPTY, ZFM_EMPTY())

/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_ATTACH_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnAttach, ZFM_EXPAND)
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_ATTACH_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnAttach, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_ATTACH_INLINE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, OnAttach, ZFM_EXPAND, ZFM_EMPTY())

/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_DETACH_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnDetach, ZFM_EXPAND)
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_DETACH_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnDetach, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_ON_INIT_DECLARE */
#define ZFPROPERTY_ON_DETACH_INLINE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, OnDetach, ZFM_EXPAND, ZFM_EMPTY())

// ============================================================
template<typename T_ZFObject>
zfclassNotPOD _ZFP_PropRVH { // RetainValueHolder
public:
    void value(ZF_IN ZFObject *obj) {
        T_ZFObject old = this->_value;
        this->_value = zfcast(T_ZFObject, zfunsafe_zfRetain(obj));
        zfunsafe_zfRelease(old);
    }
    T_ZFObject &value(void) {
        return this->_value;
    }
public:
    _ZFP_PropRVH(void)
    : _value(zfnull)
    {
    }
    template<typename T_ZFObject2>
    _ZFP_PropRVH(ZF_IN T_ZFObject2 obj)
    : _value(zfunsafe_zfRetain(zfcast(T_ZFObject, obj)))
    {
    }
    ~_ZFP_PropRVH(void) {
        zfunsafe_zfRelease(this->_value);
    }
private:
    T_ZFObject _value;
};
template<typename T_Type>
zfclassNotPOD _ZFP_PropRVSC_r { // RawValueStoreCallback_retain
public:
    static void f(
            ZF_IN void *rawValueStoreToken
            , ZF_IN ZFObject *value
            ) {
        ((_ZFP_PropRVH<T_Type> *)rawValueStoreToken)->value(value);
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_PropRVSC_a { // RawValueStoreCallback_assign
public:
    static void *f(
            ZF_IN void *rawValueStoreToken
            , ZF_IN const void *value
            ) {
        *(T_Type *)rawValueStoreToken = *(T_Type *)value;
        return rawValueStoreToken;
    }
};

template<typename T_Type, int isZFObject = zftIsZFObject(typename zftTraits<T_Type>::TrType)>
zfclassNotPOD _ZFP_PropWeak {
public:
    static inline ZFObject *v(ZF_IN T_Type const &p) {
        return zfnull;
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_PropWeak<T_Type, 1> {
public:
    static inline ZFObject *v(ZF_IN T_Type const &p) {
        return zfcast(ZFObject *, p);
    }
};
template<>
zfclassNotPOD _ZFP_PropWeak<zfany, 0> {
public:
    static inline ZFObject *v(ZF_IN zfany const &p) {
        return zfcast(ZFObject *, p);
    }
};
template<typename T_Type>
zfclassNotPOD _ZFP_PropWeak<zfanyT<T_Type>, 0> {
public:
    static inline ZFObject *v(ZF_IN zfanyT<T_Type> const &p) {
        return zfcast(ZFObject *, p);
    }
};

#define _ZFP_ZFPROPERTY_DECLARE_REGISTER_RETAIN(Type, ZFTypeId_noneOrType, Name, \
                                                propertyClassOfRetainProperty) \
    public: \
        static ZFProperty *_ZFP_Prop_##Name(void) { \
            static _ZFP_ZFPropertyRegisterHolder _propertyInfoHolder(zffalse \
                    , zffalse \
                    , zfnull \
                    , zfself::ClassData() \
                    , zftext(#Name) \
                    , zftext(#Type) \
                    , ZFTypeId_noneOrType \
                    , ZFMethodAccessDetail_1(zfself, Name \
                        , ZFMP_IN(Type const &, propertyValue) \
                        ) \
                    , ZFMethodAccessDetail_0(zfself, Name) \
                    , zfnull \
                    , zfnull \
                    , propertyClassOfRetainProperty \
                    , zfself::_ZFP_propCbAccessed_##Name \
                    , zfself::_ZFP_propCbIsInit_##Name \
                    , zfself::_ZFP_propCbReset_##Name \
                    , zfnull \
                    , zfnull \
                    , zfself::_ZFP_propCbEnsureInit_##Name \
                    , zfself::_ZFP_propCbDel_##Name \
                ); \
            return _propertyInfoHolder.propertyInfo; \
        }
#define _ZFP_ZFPROPERTY_DECLARE_REGISTER_ASSIGN(Type, ZFTypeId_noneOrType, Name, \
                                                propertyClassOfRetainProperty) \
    public: \
        static ZFProperty *_ZFP_Prop_##Name(void) { \
            static _ZFP_ZFPropertyRegisterHolder _propertyInfoHolder(zffalse \
                    , zffalse \
                    , zfnull \
                    , zfself::ClassData() \
                    , zftext(#Name) \
                    , zftext(#Type) \
                    , ZFTypeId_noneOrType \
                    , ZFMethodAccessDetail_1(zfself, Name \
                        , ZFMP_IN(Type const &, propertyValue) \
                        ) \
                    , ZFMethodAccessDetail_0(zfself, Name) \
                    , zfnull \
                    , zfnull \
                    , propertyClassOfRetainProperty \
                    , zfself::_ZFP_propCbAccessed_##Name \
                    , zfself::_ZFP_propCbIsInit_##Name \
                    , zfself::_ZFP_propCbReset_##Name \
                    , zfnull \
                    , zfnull \
                    , zfself::_ZFP_propCbEnsureInit_##Name \
                    , zfself::_ZFP_propCbDel_##Name \
                ); \
            return _propertyInfoHolder.propertyInfo; \
        }

#define _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFTypeId_noneOrType, Name, \
                                             InitValueOrEmpty) \
    public: \
        /** @cond ZFPrivateDoc */ \
        typedef Type _ZFP_PropVT_##Name; \
        typedef zfauto _ZFP_PropHT_##Name; \
        /** @endcond */ \
    private: \
        zffinal zfclassNotPOD _ZFP_PropV_##Name { \
        public: \
            _ZFP_PropV_##Name(void) \
            : _ZFP_v(zfnull) \
            { \
                zfself::_ZFP_Prop_##Name(); \
            } \
            ~_ZFP_PropV_##Name(void) { \
                zfpoolDelete(this->_ZFP_v); \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name &_ZFP_init( \
                    ZF_IN ZFObject *owner \
                    , ZF_IN zfbool notifyOwnerAttach \
                    ) { \
                if(!(this->_ZFP_v)) { \
                    this->_ZFP_v = zfpoolNew(_ZFP_PropRVH<zfself::_ZFP_PropVT_##Name>, InitValueOrEmpty); \
                    _ZFP_ZFPropertyLifeCycleCall_init_retain( \
                        zfself::_ZFP_Prop_##Name(), \
                        owner, \
                        zfcast(ZFObject *, this->_ZFP_v->value()), \
                        notifyOwnerAttach, \
                        _ZFP_PropRVSC_r<zfself::_ZFP_PropVT_##Name>::f, \
                        this->_ZFP_v); \
                } \
                return this->_ZFP_v->value(); \
            } \
            inline zfbool _ZFP_accessed(void) const { \
                return (this->_ZFP_v != zfnull); \
            } \
            void _ZFP_dealloc( \
                    ZF_IN ZFObject *owner \
                    , ZF_IN zfbool notifyOwnerDetach \
                    ) { \
                _ZFP_ZFPropertyLifeCycleCall_dealloc_retain( \
                    zfself::_ZFP_Prop_##Name(), \
                    owner, \
                    zfcast(ZFObject *, this->_ZFP_v->value()), \
                    notifyOwnerDetach); \
                zfpoolDelete(this->_ZFP_v); \
                this->_ZFP_v = zfnull; \
            } \
        public: \
            _ZFP_PropRVH<zfself::_ZFP_PropVT_##Name> *_ZFP_v; \
        }; \
        zfself::_ZFP_PropV_##Name Name##_PropV; \
    private: \
        static zfbool _ZFP_propCbIsInit_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &ownerObj \
                , ZF_OUT_OPT zfauto *outInitValue \
                ) { \
            zfself *t = zfcast(zfself *, ownerObj); \
            if(t->Name##_PropV._ZFP_accessed()) { \
                zfself::_ZFP_PropV_##Name _holder; \
                if(outInitValue != zfnull) { \
                    *outInitValue = _holder._ZFP_init(ownerObj, zffalse); \
                } \
                zfbool ret = (ZFObjectCompareValue( \
                        t->Name(), _holder._ZFP_init(ownerObj, zffalse)) \
                    == ZFCompareEqual); \
                _holder._ZFP_dealloc(ownerObj, zffalse); \
                return ret; \
            } \
            else { \
                return zftrue; \
            } \
        } \
    public:
#define _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFTypeId_noneOrType, Name, \
                                             InitValueOrEmpty) \
    public: \
        /** @cond ZFPrivateDoc */ \
        typedef Type _ZFP_PropVT_##Name; \
        typedef Type _ZFP_PropHT_##Name; \
        /** @endcond */ \
    private: \
        zffinal zfclassNotPOD _ZFP_PropV_##Name { \
        public: \
            _ZFP_PropV_##Name(void) \
            : _ZFP_v(zfnull) \
            { \
                zfself::_ZFP_Prop_##Name(); \
            } \
            ~_ZFP_PropV_##Name(void) { \
                zfpoolDelete(this->_ZFP_v); \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name &_ZFP_init( \
                    ZF_IN ZFObject *owner \
                    , ZF_IN zfbool notifyOwnerAttach \
                    ) { \
                if(!(this->_ZFP_v)) { \
                    this->_ZFP_v = zfpoolNew(zfself::_ZFP_PropVT_##Name, InitValueOrEmpty); \
                    _ZFP_ZFPropertyLifeCycleCall_init_assign( \
                        zfself::_ZFP_Prop_##Name(), \
                        owner, \
                        this->_ZFP_v, \
                        notifyOwnerAttach, \
                        _ZFP_PropWeak<zfself::_ZFP_PropVT_##Name>::v(*(this->_ZFP_v))); \
                } \
                return (*(this->_ZFP_v)); \
            } \
            inline zfbool _ZFP_accessed(void) const { \
                return (this->_ZFP_v != zfnull); \
            } \
            void _ZFP_dealloc( \
                    ZF_IN ZFObject *owner \
                    , ZF_IN zfbool notifyOwnerDetach \
                    ) { \
                _ZFP_ZFPropertyLifeCycleCall_dealloc_assign( \
                    zfself::_ZFP_Prop_##Name(), \
                    owner, \
                    this->_ZFP_v, \
                    notifyOwnerDetach, \
                    _ZFP_PropWeak<zfself::_ZFP_PropVT_##Name>::v(*(this->_ZFP_v))); \
                zfpoolDelete(this->_ZFP_v); \
                this->_ZFP_v = zfnull; \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name *_ZFP_v; \
        }; \
        zfself::_ZFP_PropV_##Name Name##_PropV; \
    private: \
        static zfbool _ZFP_propCbIsInit_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &ownerObj \
                , ZF_OUT_OPT zfauto *outInitValue \
                ) { \
            zfself *t = zfcast(zfself *, ownerObj); \
            if(t->Name##_PropV._ZFP_accessed()) { \
                zfself::_ZFP_PropV_##Name _holder; \
                if(outInitValue != zfnull) { \
                    ZFTypeId<zfself::_ZFP_PropVT_##Name>::ValueStore(*outInitValue, _holder._ZFP_init(ownerObj, zffalse)); \
                } \
                zfbool ret = (ZFComparerDefault( \
                        t->Name(), _holder._ZFP_init(ownerObj, zffalse)) \
                    == ZFCompareEqual); \
                _holder._ZFP_dealloc(ownerObj, zffalse); \
                return ret; \
            } \
            else { \
                return zftrue; \
            } \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name) \
    private: \
        static zfbool _ZFP_propCbAccessed_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &ownerObj \
                ) { \
            return zfcast(zfself *, ownerObj)->Name##_PropV._ZFP_accessed(); \
        } \
        static void _ZFP_propCbReset_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &owner \
                ) { \
            if(_ZFP_propCbAccessed_##Name(property, owner)) { \
                owner->_ZFP_ZFObject_objectPropertyValueOnReset(zfself::_ZFP_Prop_##Name()); \
            } \
            zfcast(zfself *, owner)->Name##_PropV._ZFP_dealloc(owner, zftrue); \
        } \
        static void _ZFP_propCbEnsureInit_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &owner \
                ) { \
            zfcast(zfself *, owner)->Name##_PropV._ZFP_init(owner, zftrue); \
        } \
        static void _ZFP_propCbDel_##Name( \
                ZF_IN const ZFProperty *property \
                , ZF_IN zfany const &owner \
                ) { \
            zfcast(zfself *, owner)->Name##_PropV._ZFP_dealloc(owner, zffalse); \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, ZFTypeId_noneOrType, Name, \
                                       InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_REGISTER_RETAIN(Type, ZFTypeId_noneOrType, Name, \
                                            zftTraits<Type>::TrType::ClassData()) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFTypeId_noneOrType, Name, \
                                         InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name)
#define _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFTypeId_noneOrType, Name, \
                                       InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_REGISTER_ASSIGN(Type, ZFTypeId_noneOrType, Name, \
                                            zfnull) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFTypeId_noneOrType, Name, \
                                         InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name)

// ============================================================
#define _ZFP_ZFPROPERTY_SETTER_RETAIN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
                notAutoRegister \
                , AccessType, ZFMethodTypeVirtual, S \
                , void, Name \
                , _ZFP_MtdP_EXPAND(ZFMP_IN(Type const &, propertyValue)) \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                ) { \
            zfsynchronizeLock(this); \
            zfbool accessed = Name##_PropV._ZFP_accessed(); \
            ZFObject *valueOld = zfcast(ZFObject *, Name##_PropV._ZFP_init(this->toObject(), zffalse)); \
            _ZFP_ZFPropertyLifeCycleCall_setter_retain( \
                zfself::_ZFP_Prop_##Name(), \
                this->toObject(), \
                accessed, \
                valueOld, \
                zfcast(ZFObject *, propertyValue), \
                _ZFP_PropRVSC_r<zfself::_ZFP_PropVT_##Name>::f, \
                Name##_PropV._ZFP_v); \
            zfsynchronizeUnlock(this); \
        } \
    public:
#define _ZFP_ZFPROPERTY_SETTER_ASSIGN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
                notAutoRegister \
                , AccessType, ZFMethodTypeVirtual, S \
                , void, Name \
                , _ZFP_MtdP_EXPAND(ZFMP_IN(Type const &, propertyValue)) \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                ) { \
            zfsynchronizeLock(this); \
            zfbool accessed = Name##_PropV._ZFP_accessed(); \
            Type valueOld = Name##_PropV._ZFP_init(this->toObject(), zffalse); \
            _ZFP_ZFPropertyLifeCycleCall_setter_assign( \
                zfself::_ZFP_Prop_##Name(), \
                this->toObject(), \
                accessed, \
                &valueOld, \
                &propertyValue, \
                _ZFP_PropRVSC_a<zfself::_ZFP_PropVT_##Name>::f, \
                Name##_PropV._ZFP_v, \
                _ZFP_PropWeak<zfself::_ZFP_PropVT_##Name>::v(valueOld), \
                _ZFP_PropWeak<zfself::_ZFP_PropVT_##Name>::v(propertyValue)); \
            zfsynchronizeUnlock(this); \
        } \
    public:
#define _ZFP_ZFPROPERTY_GETTER_RETAIN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
                notAutoRegister \
                , AccessType, ZFMethodTypeVirtual, G \
                , Type const &, Name \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                ) { \
            zfsynchronize(this); \
            return Name##_PropV._ZFP_init(this->toObject(), zftrue); \
        } \
    public:
#define _ZFP_ZFPROPERTY_GETTER_ASSIGN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
                notAutoRegister \
                , AccessType, ZFMethodTypeVirtual, G \
                /* ZFTAG_TRICKS: return non const ref, but use const ref in reflection */ \
                , Type &, Name \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                , _ZFP_ZFMP_DUMMY() \
                ) { \
            zfsynchronize(this); \
            return Name##_PropV._ZFP_init(this->toObject(), zftrue); \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_INLINE(Type, Name, lifeCycleName, constFix, extraRegStep) \
    private: \
        zfclassNotPOD _ZFP_propLReg_##lifeCycleName##_##Name { \
        public: \
            _ZFP_propLReg_##lifeCycleName##_##Name(void) { \
                static zfbool needInit = zftrue; \
                if(needInit) { \
                    needInit = zffalse; \
                    _ZFP_ZFPropertyLifeCycleRegister( \
                        zftext(#lifeCycleName), \
                        zfself::_ZFP_Prop_##Name(), \
                        zfself::ClassData(), \
                        zfself::_ZFP_propLI_##lifeCycleName##_##Name); \
                    extraRegStep \
                } \
            } \
        }; \
        _ZFP_propLReg_##lifeCycleName##_##Name _ZFP_propLReg_##lifeCycleName##_##Name; \
        static void _ZFP_propLI_##lifeCycleName##_##Name( \
                ZF_IN ZFObject *propertyOwnerObject \
                , ZF_IN const ZFProperty *property \
                , ZF_IN void *propertyValue \
                , ZF_IN const void *propertyValueOld \
                , ZF_IN ZFObject *propertyLifeCycleUserData \
                ) { \
            zfcast(zfself *, propertyOwnerObject)->zfself::_ZFP_propL_##lifeCycleName##_##Name( \
                *(constFix(const) zfself::_ZFP_PropHT_##Name *)propertyValue, \
                *(zfself::_ZFP_PropHT_##Name *)propertyValueOld); \
        } \
    public: \
        zffinal void _ZFP_propL_##lifeCycleName##_##Name( \
            ZF_IN zfself::_ZFP_PropHT_##Name constFix(const) &propertyValue, \
            ZF_IN zfself::_ZFP_PropHT_##Name const &propertyValueOld)
#define _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, lifeCycleName, constFix) \
    public: \
        zffinal void _ZFP_propL_##lifeCycleName##_##Name( \
            ZF_IN zfself::_ZFP_PropHT_##Name constFix(const) &propertyValue, \
            ZF_IN zfself::_ZFP_PropHT_##Name const &propertyValueOld);
#define _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, lifeCycleName, constFix, extraRegStep) \
    ZF_STATIC_REGISTER_INIT(propL_##OwnerClass##_##lifeCycleName##_##Name) { \
        _ZFP_ZFPropertyLifeCycleRegister( \
            zftext(#lifeCycleName), \
            OwnerClass::_ZFP_Prop_##Name(), \
            OwnerClass::ClassData(), \
            a); \
        extraRegStep \
    } \
    public: \
        static void a( \
                ZF_IN ZFObject *propertyOwnerObject \
                , ZF_IN const ZFProperty *property \
                , ZF_IN void *propertyValue \
                , ZF_IN const void *propertyValueOld \
                , ZF_IN ZFObject *propertyLifeCycleUserData \
                ) { \
            zfcast(OwnerClass *, propertyOwnerObject)->OwnerClass::_ZFP_propL_##lifeCycleName##_##Name( \
                *(constFix(const) OwnerClass::_ZFP_PropHT_##Name *)propertyValue, \
                *(OwnerClass::_ZFP_PropHT_##Name *)propertyValueOld); \
        } \
    ZF_STATIC_REGISTER_END(propL_##OwnerClass##_##lifeCycleName##_##Name) \
    void OwnerClass::_ZFP_propL_##lifeCycleName##_##Name( \
        ZF_IN zfself::_ZFP_PropHT_##Name constFix(const) &propertyValue, \
        ZF_IN zfself::_ZFP_PropHT_##Name const &propertyValueOld)

extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleRegister(
        ZF_IN const zfchar *lifeCycleName
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClass
        , ZF_IN _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleUnregister(
        ZF_IN const zfchar *lifeCycleName
        , ZF_IN const ZFProperty *property
        , ZF_IN const ZFClass *ownerClass
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_init_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN ZFObject *value
        , ZF_IN zfbool notifyOwnerAttach
        , ZF_IN void (*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN ZFObject *value
            )
        , ZF_IN void *rawValueStoreToken
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_init_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN void *value
        , ZF_IN zfbool notifyOwnerAttach
        , ZF_IN ZFObject *weakProp
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_dealloc_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN ZFObject *value
        , ZF_IN zfbool notifyOwnerDetach
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_dealloc_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN void *value
        , ZF_IN zfbool notifyOwnerDetach
        , ZF_IN ZFObject *weakProp
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_setter_retain(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN ZFObject *propertyValueOld
        , ZF_IN ZFObject *propertyValueNew
        , ZF_IN void (*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN ZFObject *value
            )
        , ZF_IN void *rawValueStoreToken
        );
extern ZFLIB_ZFCore void _ZFP_ZFPropertyLifeCycleCall_setter_assign(
        ZF_IN const ZFProperty *property
        , ZF_IN ZFObject *propertyOwnerObject
        , ZF_IN zfbool accessed
        , ZF_IN void *propertyValueOld
        , ZF_IN const void *propertyValueNew
        , ZF_IN void *(*rawValueStoreCallback)(
            ZF_IN void *rawValueStoreToken
            , ZF_IN const void *value
            )
        , ZF_IN void *rawValueStoreToken
        , ZF_IN ZFObject *weakPropOld
        , ZF_IN ZFObject *weakPropNew
        );

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDeclare_h_


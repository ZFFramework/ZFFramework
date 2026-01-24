/**
 * @file ZFObjectInterface.h
 * @brief interface declare
 */

#ifndef _ZFI_ZFObjectInterface_h_
#define _ZFI_ZFObjectInterface_h_

#include "zfany.h"
#include "ZFClass.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFClass;

/**
 * @brief shows the type is an interface, see #ZFInterface
 */
#define zfinterface class
/**
 * @brief shows class implement from interface, see #ZFInterface
 */
#define zfimplement public

// ============================================================
zfclassNotPOD ZFLIB_ZFCore _ZFP_ObjI_Base {
protected:
    virtual ~_ZFP_ObjI_Base(void) {}

public:
    static const ZFClass *ClassData(void) {return zfnull;}
public:
    static void _ZFP_ObjI_reg(ZFClass *cls) {}
public:
    static void _ZFP_ObjI_ICk(void) {}
};
#define _ZFP_ZFINTERFACE_DECLARE(InterfaceName, ParentInterface, OuterClass, ...) \
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(InterfaceName) \
    public: \
        static void _ZFP_zftIsZFObject(void) {} \
        static void _ZFP_zftIsZFInterface(void) {} \
    protected: \
        /** @brief typedef for super (always #ZFInterface for an interface type) */ \
        typedef ParentInterface zfsuper; \
        /** @brief typedef for self */ \
        typedef InterfaceName zfself; \
    private: \
        static void _ZFP_ObjI_regCk(ZF_IN ZFClass *cls) { \
            if(cls->_ZFP_ZFClass_implListNeedInit) { \
                cls->_ZFP_ZFClass_implListNeedInit = zffalse; \
                if(zfself::_ZFP_ObjI_reg != zfsuper::_ZFP_ObjI_reg) { \
                    zfself::_ZFP_ObjI_reg(cls); \
                } \
            } \
        } \
    public: \
        /** @brief get class info */ \
        static const ZFClass *ClassData(void) { \
            static _ZFP_ZFClassRegisterHolder h( \
                    ZF_NAMESPACE_CURRENT(), \
                    zftext(#InterfaceName), \
                    ParentInterface::ClassData(), \
                    OuterClass::ClassData(), \
                    zftrue, \
                    zfnull, \
                    zfnull, \
                    zfnull, \
                    &zfself::_ZFP_ObjI_regCk, \
                    zftrue \
                ); \
            return h.cls; \
        } \
    protected: \
        virtual void _ZFP_ObjI_ICk(void) { \
            /* used to ensure inherit from ZFInterface */ \
            _ZFP_ObjI_Base::_ZFP_ObjI_ICk(); \
        } \
    public: \
        /** @cond ZFPrivateDoc */ \
        template<typename T_ZFObject> \
        inline T_ZFObject to(void) { \
            return zfcast(T_ZFObject, this); \
        } \
        /** @endcond */ \
    public:
/**
 * @brief see #ZFINTERFACE_DECLARE
 *
 * you may have custom constructor with this macro,
 * but must be declared as protected with no params,
 * and must also declare an virtual protected destructor
 */
#define ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(InterfaceName, ParentInterface, ...) \
        ZFM_VA_APPEND(_ZFP_ZFINTERFACE_DECLARE, _ZFP_ObjI_Base, InterfaceName, ParentInterface, ##__VA_ARGS__) \
    public:
/**
 * @brief see #ZFInterface
 */
#define ZFINTERFACE_DECLARE(InterfaceName, ParentInterface, ...) \
        ZFM_VA_APPEND(_ZFP_ZFINTERFACE_DECLARE, _ZFP_ObjI_Base, InterfaceName, ParentInterface, ##__VA_ARGS__) \
    protected: \
        /** @cond ZFPrivateDoc */ \
        InterfaceName(void) {} \
        virtual ~InterfaceName(void) {} \
        /** @endcond */ \
    public:

template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface, int isFromInterface>
zfclassNotPOD _ZFP_ObjICW { // interface cast wrapper
};
template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface>
zfclassNotPOD _ZFP_ObjICW<T_FromZFObjectOrZFInterface, T_ToZFInterface, 0> {
public:
    static ZFInterface *_ZFP_cast(ZF_IN ZFObject *obj) {
        return (T_ToZFInterface *)zfcast(T_FromZFObjectOrZFInterface *, obj);
    }
};
template<typename T_FromZFObjectOrZFInterface, typename T_ToZFInterface>
zfclassNotPOD _ZFP_ObjICW<T_FromZFObjectOrZFInterface, T_ToZFInterface, 1> {
public:
    template<typename T_ZFObject>
    static inline ZFInterface *_ZFP_cast(ZF_IN T_ZFObject *obj) {
        return zfcast(T_ToZFInterface *, obj->toObject());
    }
};
template<typename T_FromZFObjectOrZFInterface>
zfclassNotPOD _ZFP_ObjICW<T_FromZFObjectOrZFInterface, ZFInterface, 0> {
public:
    template<typename T_ZFObject>
    static inline ZFInterface *_ZFP_cast(ZF_IN T_ZFObject *obj) {
        return zfnull;
    }
};
template<typename T_FromZFObjectOrZFInterface>
zfclassNotPOD _ZFP_ObjICW<T_FromZFObjectOrZFInterface, ZFInterface, 1> {
public:
    template<typename T_ZFObject>
    static inline ZFInterface *_ZFP_cast(ZF_IN T_ZFObject *obj) {
        return zfnull;
    }
};
#define _ZFP_ZFIMPLEMENT_DECLARE_EXPAND_PARAM(Interface) \
    , Interface::ClassData(), &zfself::_ZFP_ObjICCb_##Interface
#define _ZFP_ZFIMPLEMENT_DECLARE_EXPAND_CAST_CALLBACK(Interface) \
    static ZFInterface *_ZFP_ObjICCb_##Interface(ZF_IN ZFObject *obj) { \
        return _ZFP_ObjICW<zfself, Interface, (zftIsTypeOf<zfself, ZFObject>::Value ? 0 : 1)>::_ZFP_cast(obj); \
    }
#define _ZFP_ZFIMPLEMENT_DECLARE_EXPAND_INTERFACE_ON_INIT(Interface) \
    Interface::_ZFP_ObjI_onInit();
#define _ZFP_ZFIMPLEMENT_DECLARE_EXPAND_INTERFACE_ON_DEALLOC(Interface) \
    Interface::_ZFP_ObjI_onDealloc();
#define _ZFP_ZFIMPLEMENT_DECLARE(ImplementedInterfaces, ...) \
    public: \
        static void _ZFP_ObjI_reg(ZFClass *cls) { \
            if(cls->_ZFP_ZFClass_interfaceNeedRegister()) { \
                cls->_ZFP_ZFClass_interfaceRegister(0 \
                    ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENT_DECLARE_EXPAND_PARAM, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
                    , (const ZFClass *)zfnull \
                    ); \
            } \
        } \
    public: \
        ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENT_DECLARE_EXPAND_CAST_CALLBACK, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
    public:
/**
 * @brief see #ZFINTERFACE_DECLARE
 */
#define ZFIMPLEMENT_DECLARE(ImplementedInterfaces, ...) \
        _ZFP_ZFIMPLEMENT_DECLARE(ImplementedInterfaces, ##__VA_ARGS__) \
    public: \
        virtual inline void _ZFP_ObjI_ctor(void) { \
            zfsuper::_ZFP_ObjI_ctor(); \
            ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENT_DECLARE_EXPAND_INTERFACE_ON_INIT, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
        } \
        virtual inline void _ZFP_ObjI_dtor(void) { \
            ZFM_FIX_PARAM(_ZFP_ZFIMPLEMENT_DECLARE_EXPAND_INTERFACE_ON_DEALLOC, ZFM_EMPTY, ImplementedInterfaces, ##__VA_ARGS__) \
            zfsuper::_ZFP_ObjI_dtor(); \
        } \
    public:

/**
 * @brief dummy base of interface types
 *
 * usage:
 * @code
 *   // declare an interface
 *   zfinterface YourInterface : zfextend ZFInterface {
 *       ZFINTERFACE_DECLARE(YourInterface, ZFInterface)
 *   };
 *
 *   // implement an interface
 *   zfclass YourType : zfextend ZFObject
 *                    , zfimplement YourInterface
 *                    , zfimplement ParentInterface1
 *                    , zfimplement ParentInterface2
 *                    ...
 *   {
 *       ZFOBJECT_DECLARE(YourType, ZFObject)
 *       ZFIMPLEMENT_DECLARE(YourInterface, ParentInterface1, ParentInterface2, ...)
 *   };
 * @endcode
 *
 * (ZFTAG_LIMITATION) due to some limitations of C++ (basically the limitations of diamond inheritance),
 * you should remember these while using interface:
 * -  an interface must inherit from #ZFInterface or other interface (at most one),
 *   it can only be "implemented" by a "ZFObject type"
 * -  you must have #ZFINTERFACE_DECLARE declared in your interface,
 *   and must have ZFIMPLEMENT_DECLARE declared in your child type
 *   that implement the interface
 *
 * \n
 * ADVANCED:\n
 * to simulate a interface that inherit from multiple interfaces,
 * you should do it like this:\n
 * Java code:
 * @code
 *   public interface IParent0 {}
 *   public interface IParent1 {}
 *   public interface IChild extends IParent0, IParent1 {}
 *   public class ObjParent implements IParent0 {}
 *   public class ObjChild extends ObjParent implements IChild {}
 * @endcode
 * equivalent code for ZFInterface:
 * @code
 *   zfinterface IParent0 : zfextend ZFInterface {
 *       ZFINTERFACE_DECLARE(IParent0, ZFInterface)
 *   };
 *   zfinterface IParent1 : zfextend ZFInterface {
 *       ZFINTERFACE_DECLARE(IParent1, ZFInterface)
 *   };
 *   zfinterface IChild : zfextend IParent0 { // note: can only inherit at most one parent interface
 *       ZFINTERFACE_DECLARE(IChild, IParent0)
 *   };
 *   zfclass ObjParent : zfextend ZFObject, zfimplement IParent0 {
 *       ZFOBJECT_DECLARE(ObjParent, ZFObject)
 *       ZFIMPLEMENT_DECLARE(IParent0)
 *   };
 *   zfclass ObjChild : zfextend ObjParent, zfimplement IChild
 *                    , zfimplement IParent1 // note: here is required
 *   {
 *       ZFOBJECT_DECLARE(ObjChild, ObjParent)
 *       ZFIMPLEMENT_DECLARE(IChild
 *               , IParent1 // note: here is required
 *               )
 *   };
 * @endcode
 * it's strongly recommend not to make interface inherit multiple interfaces
 */
zfinterface ZFLIB_ZFCore ZFInterface {
    _ZFP_ZFINTERFACE_DECLARE(ZFInterface, _ZFP_ObjI_Base, _ZFP_ObjI_Base)

protected:
    /** @cond ZFPrivateDoc */
    ZFInterface(void) {}
    virtual ~ZFInterface(void) {}
    /** @endcond */

public:
    static void _ZFP_ObjI_reg(ZFClass *cls) {}
    zffinal inline void _ZFP_ObjI_onInit(void) {}
    zffinal inline void _ZFP_ObjI_onDealloc(void) {}

public:
    /** @brief get instance's class info */
    virtual const ZFClass *classData(void) zfpurevirtual;

    /**
     * @brief convert to ZFObject type
     */
    virtual ZFObject *toObject(void) zfpurevirtual;
};

/**
 * @brief declare custom init step for the interface
 *
 * although #ZFInterface is designed as interface type,
 * it is normal class in C++ in fact,
 * you can supply your own custom init step\n
 * once declared, the init step would be called
 * after #ZFObject::objectOnInit and before #ZFObject::objectOnInitFinish,
 * ordered from parent interface to child interface,
 * and the paired #ZFINTERFACE_ON_DEALLOC_DECLARE would be called
 * after #ZFObject::objectOnDeallocPrepare and before #ZFObject::objectOnDealloc\n
 * \n
 * note: use only if necessary, and at your own risk\n
 * overrided init step would be called for each instance of this class and all of subclasses,
 * take care of performance
 */
#define ZFINTERFACE_ON_INIT_DECLARE() \
    public: \
        zffinal void _ZFP_ObjI_onInit(void)
/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_INIT_DEFINE(YourInterface) \
    void YourInterface::_ZFP_ObjI_onInit(void)

/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_DEALLOC_DECLARE() \
    public: \
        zffinal void _ZFP_ObjI_onDealloc(void)
/** @brief see #ZFINTERFACE_ON_INIT_DECLARE */
#define ZFINTERFACE_ON_DEALLOC_DEFINE(YourInterface) \
    void YourInterface::_ZFP_ObjI_onDealloc(void)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFObjectInterface_h_


/**
 * @file ZFIdMap.h
 * @brief id to string map util, useful to define extendable string enum
 */

#ifndef _ZFI_ZFIdMap_h_
#define _ZFI_ZFIdMap_h_

#include "../ZFCoreDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;

// ============================================================
extern ZFLIB_ZFCore const zfidentity *_ZFP_ZFIdMapRegister(
        ZF_IN const zfstring &idName
        , ZF_IN_OPT zfbool isDynamicRegister = zffalse
        );
extern ZFLIB_ZFCore void _ZFP_ZFIdMapUnregister(
        ZF_IN zfidentity idValue
        , ZF_IN_OPT zfbool isDynamicRegister = zffalse
        );
/**
 * @brief get id name from id value, or null if no such id, see #ZFIdMapIdForName
 */
extern ZFLIB_ZFCore zfstring ZFIdMapNameForId(ZF_IN zfidentity idValue);
/**
 * @brief get id value from id name, or #zfidentityInvalid if no such id name, see #ZFIDMAP
 *
 * the id name should looks like `YourClass.YourIdName` or `YourNamespace.YourIdName`
 *
 * note: can be found only if:
 * -  declared with #ZFIDMAP_INLINE
 * -  registered by #ZFIDMAP_REGISTER series
 */
extern ZFLIB_ZFCore zfidentity ZFIdMapIdForName(ZF_IN const zfstring &idName);

/**
 * @brief see #ZFIDMAP
 *
 * get all registered id data, for debug use only
 * @note can be found only if registered by #ZFIDMAP_REGISTER
 */
extern ZFLIB_ZFCore void ZFIdMapGetAll(
        ZF_IN_OUT ZFCoreArray<zfidentity> &idValues
        , ZF_IN_OUT ZFCoreArray<zfstring> &idNames
        );
/** @brief see #ZFIdMapGetAll */
extern ZFLIB_ZFCore void ZFIdMapGetAllValueT(ZF_IN_OUT ZFCoreArray<zfidentity> &idValues);
/** @brief see #ZFIdMapGetAll */
inline ZFCoreArray<zfidentity> ZFIdMapGetAllValue(void) {
    ZFCoreArray<zfidentity> ret;
    ZFIdMapGetAllValueT(ret);
    return ret;
}
/** @brief see #ZFIdMapGetAll */
extern ZFLIB_ZFCore void ZFIdMapGetAllNameT(ZF_IN_OUT ZFCoreArray<zfstring> &idNames);
/** @brief see #ZFIdMapGetAll */
inline ZFCoreArray<zfstring> ZFIdMapGetAllName(void) {
    ZFCoreArray<zfstring> ret;
    ZFIdMapGetAllNameT(ret);
    return ret;
}

/**
 * @brief dynamically register your own id
 *
 * assert fail if already registered
 */
extern ZFLIB_ZFCore zfidentity ZFIdMapDynamicRegister(ZF_IN const zfstring &idName);
/**
 * @brief unregister id that was registered by #ZFIdMapDynamicRegister
 *
 * do nothing if no such id,
 * assert fail if the id is not dynamically registered
 */
extern ZFLIB_ZFCore void ZFIdMapDynamicUnregister(ZF_IN zfidentity idValue);

zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFIdMapHolder {
public:
    _ZFP_ZFIdMapHolder(
            ZF_IN const zfstring &idName
            , ZF_IN const ZFClass *ownerClass
            , ZF_IN const zfstring &ownerNamespace
            , ZF_IN const zfstring &methodName
            );
    ~_ZFP_ZFIdMapHolder(void);

public:
    const zfidentity *idValue;
};
/**
 * @brief declare an id within ZFObject class scope
 *
 * usage:
 * @code
 *   zfclass YourClass ... {
 *       ZFIDMAP(YourSth)
 *   };
 *
 *   ZFIDMAP_REGISTER(YourClass, YourSth)
 * @endcode
 * declared id can be accessed by:
 * @code
 *   zfidentity idValue = YourClass::IdYourSth();
 *   zfstring idName = ZFIdMapNameForId(idValue);
 * @endcode
 * note that subclass may declare an id same as parent,
 * while the final id name is different:\n
 *   ParentClass::IdYourSth() => "ParentClass.IdYourSth"\n
 *   ChildClass::IdYourSth() => "ChildClass.IdYourSth"\n
 * \n
 * @note we declare the id as int types for performance,
 *   it's ensured each id has different value,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the id value is not ensured the same,
 *   you should use the name of the id to store or pass between apps,
 *   and you can use #ZFIdMapIdForName or #ZFIdMapNameForId
 *   to convert them easily
 */
#define ZFIDMAP(YourIdName) \
    ZFIDMAP_DETAIL(Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_DETAIL(prefix, YourIdName) \
    public: \
        /** \n */ \
        static zfidentity prefix##YourIdName(void);

/**
 * @brief auto register version of #ZFIDMAP
 *
 * note: this would create a dummy class to perform register,
 * which would increase your owner object's size
 */
#define ZFIDMAP_INLINE(YourIdName) \
    ZFIDMAP_DETAIL_INLINE(Id, YourIdName)
/** @brief see #ZFIDMAP_INLINE */
#define ZFIDMAP_DETAIL_INLINE(prefix, YourIdName) \
    private: \
        zfclassNotPOD _ZFP_IdMapReg_##prefix##YourIdName { \
        public: \
            _ZFP_IdMapReg_##prefix##YourIdName(void) { \
                zfself::prefix##YourIdName(); \
            } \
        } _ZFP_IdMap_##prefix##YourIdName; \
    public: \
        static zfidentity prefix##YourIdName(void) { \
            static _ZFP_ZFIdMapHolder d( \
                    zfstr("%s.%s", zfself::ClassData()->classNameFull(), ZFM_TOSTRING(prefix##YourIdName)) \
                    , zfself::ClassData() \
                    , zfnull \
                    , zftext(ZFM_TOSTRING(prefix##YourIdName)) \
                ); \
            return *(d.idValue); \
        }

/**
 * @brief declare an id in global scope, see #ZFIDMAP
 *
 * usage:
 * @code
 *   // in header files
 *   ZF_NAMESPACE_BEGIN(YourNamespace)
 *   / ** @brief you can add doxygen docs here * /
 *   ZFIDMAP_GLOBAL(YourSth)
 *   ZF_NAMESPACE_END(YourNamespace)
 *
 *   ZFIDMAP_GLOBAL_REGISTER(YourSth)
 *
 *   // use the id
 *   zfidentity idValue = YourNamespace::IdYourSth();
 * @endcode
 * unlike #ZFIDMAP, this macro would declare id outside of class scope
 */
#define ZFIDMAP_GLOBAL(ZFLIB_, YourIdName) \
    ZFIDMAP_GLOBAL_DETAIL(ZFLIB_, Id, YourIdName)
/** @brief see #ZFIDMAP_GLOBAL */
#define ZFIDMAP_GLOBAL_DETAIL(ZFLIB_, prefix, YourIdName) \
    /** \n */ \
    extern ZFLIB_ zfidentity prefix##YourIdName(void);

/** @brief see #ZFIDMAP */
#define ZFIDMAP_REGISTER(Scope, YourIdName) \
    ZFIDMAP_REGISTER_DETAIL(Scope, Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_REGISTER_DETAIL(Scope, prefix, YourIdName) \
    zfidentity Scope::prefix##YourIdName(void) { \
        static _ZFP_ZFIdMapHolder d( \
                zfstr("%s.%s", zfself::ClassData()->classNameFull(), ZFM_TOSTRING(prefix##YourIdName)) \
                , zfself::ClassData() \
                , zfnull \
                , zftext(ZFM_TOSTRING(prefix##YourIdName)) \
            ); \
        return *(d.idValue); \
    } \
    ZF_STATIC_REGISTER_INIT(ZFIdMap_##Scope##_##YourIdName) { \
        (void)Scope::prefix##YourIdName(); \
    } \
    ZF_STATIC_REGISTER_END(ZFIdMap_##Scope##_##YourIdName)

/** @brief see #ZFIDMAP */
#define ZFIDMAP_GLOBAL_REGISTER(YourIdName) \
    ZFIDMAP_GLOBAL_REGISTER_DETAIL(Id, YourIdName)
/** @brief see #ZFIDMAP */
#define ZFIDMAP_GLOBAL_REGISTER_DETAIL(prefix, YourIdName) \
    zfidentity prefix##YourIdName(void) { \
        static _ZFP_ZFIdMapHolder d( \
                zfstr("%s.%s", ZF_NAMESPACE_CURRENT() ? ZF_NAMESPACE_CURRENT() : "", ZFM_TOSTRING(prefix##YourIdName)) \
                , zfnull \
                , ZF_NAMESPACE_CURRENT() \
                , zftext(ZFM_TOSTRING(prefix##YourIdName)) \
            ); \
        return *(d.idValue); \
    } \
    ZF_STATIC_REGISTER_INIT(ZFIdMap_##YourIdName) { \
        (void)prefix##YourIdName(); \
    } \
    ZF_STATIC_REGISTER_END(ZFIdMap_##YourIdName) \

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIdMap_h_


/**
 * @file ZFStyleable.h
 * @brief styleable
 */

#ifndef _ZFI_ZFStyleable_h_
#define _ZFI_ZFStyleable_h_

#include "ZFSerializable.h"
#include "ZFCopyable.h"
#include "ZFObjectObserver.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFStyleable
zfclassFwd _ZFP_ZFStyleKeyHolder;
/**
 * @brief styleable element that can apply style from another object
 *
 * a styleable typically is a serializable,
 * while serializable process data from and to #ZFSerializableData,
 * a styleable process data from and to other styleable object\n
 * \n
 * to use styleable, simply implement from #ZFStyleable,
 * if all of your properties are declared as #ZFProperty,
 * then every thing's done,
 * and style can be copied by #ZFStyleable::styleableCopyFrom\n
 * for how styleable would be copied, see #styleableOnCopyPropertyFrom
 */
zfinterface ZFLIB_ZFCore ZFStyleable : zfextend ZFInterface {
    ZFINTERFACE_DECLARE(ZFStyleable, ZFInterface)

public:
    /**
     * @brief return default style of this instance
     *
     * note this method use reflection to find the default style
     * (see #ZFSTYLE_DEFAULT_DECLARE),
     * cache it first if necessary
     */
    virtual zfanyT<ZFStyleable> defaultStyle(void);

public:
    /**
     * @brief copy style from another styleable, see #ZFStyleable
     *
     * usually you should not override this method,
     * override #styleableOnCopyFrom instead
     */
    zffinal void styleableCopyFrom(ZF_IN ZFObject *anotherStyleable);

public:
    /**
     * @brief true if this object is #defaultStyle
     */
    virtual zfbool styleableIsDefaultStyle(void) {
        return zffalse;
    }

    /**
     * @brief return a list of styleable property, for debug use only
     */
    zffinal void styleablePropertyGetAllT(
            ZF_OUT ZFCoreArray<const ZFProperty *> &ret
            );
    /**
     * @brief return a list of styleable property, for debug use only
     */
    zffinal ZFCoreArray<const ZFProperty *> styleablePropertyGetAll(void);

protected:
    /**
     * @brief copy property with styleable logic
     */
    virtual void styleableOnCopyPropertyFrom(
            ZF_IN ZFObject *anotherStyleable
            , ZF_IN const ZFProperty *property
            );
    /**
     * @brief for subclass to achieve custom style copy step,
     *   called by #styleableCopyFrom, see #ZFStyleable
     */
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);

public:
    /** @brief see #ZFStyleSet */
    zffinal void styleKey(ZF_IN const zfstring &styleKey);
    /** @brief see #ZFStyleSet */
    zffinal const zfstring &styleKey(void);
    /** @brief see #ZFStyleSet */
    zffinal void propStyle(
            ZF_IN const zfstring &propertyName
            , ZF_IN const zfstring &styleKey
            );
    /** @brief see #ZFStyleSet */
    zffinal const zfstring &propStyle(ZF_IN const zfstring &propertyName);
private:
    friend zfclassFwd _ZFP_ZFStyleKeyHolder;
};

// ============================================================
// ZFStyle
/**
 * @brief common styleable object
 *
 * implement #ZFStyleable, #ZFSerializable, #ZFCopyable\n
 * every style, serialize and copy logic has been done by reflect,
 * if all of your properties are declared as #ZFProperty
 */
zfclass ZFLIB_ZFCore ZFStyle : zfextend ZFObject, zfimplement ZFStyleable, zfimplement ZFSerializable, zfimplement ZFCopyable {
    ZFOBJECT_DECLARE(ZFStyle, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj) {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->styleableCopyFrom(zfcast(zfself *, anotherObj));
    }
};

// ============================================================
// DefaultStyle
#define _ZFP_ZFStyleableDefault_level ZFLevelZFFrameworkHigh
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFStyleableDefaultPointerHolder {
public:
    void *d;
    _ZFP_ZFStyleableDefaultPointerHolder(void) : d(zfnull) {}
};

extern ZFLIB_ZFCore _ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfstring &name);
typedef void (*_ZFP_ZFStyleableDefaultDeleteCallback)(ZF_IN void *instance);
zfclassLikePOD ZFLIB_ZFCore _ZFP_ZFStyleableDefaultDeleteCallbackHolder {
public:
    _ZFP_ZFStyleableDefaultDeleteCallbackHolder(
            ZF_IN _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback
            , ZF_IN void *instance
            )
    : deleteCallback(deleteCallback)
    , instance(instance)
    {
    }
    ~_ZFP_ZFStyleableDefaultDeleteCallbackHolder(void) {
        if(this->deleteCallback && this->instance) {
            this->deleteCallback(this->instance);
        }
    }
private:
    _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback;
    void *instance;
};

#define _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    public: \
        zfclass _ZFP_ZFStyleableDefault_##YourStyle; \
    public: \
        /** \n default style for @ref YourStyle */ \
        static zfanyT<YourStyle> DefaultStyle(void); \
    private: \
        static void _ZFP_ZFStyleablEnumDefaultStyle(ZF_IN YourStyle *newInstance); \
        static const ZFCorePointer *&_ZFP_ZFStyleableDefaultCleaner(void); \
        static void _ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance); \
    public:
#define _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    zfclass YourStyle::_ZFP_ZFStyleableDefault_##YourStyle : zfextend YourStyle { \
        ZFOBJECT_DECLARE(_ZFP_ZFStyleableDefault_##YourStyle, YourStyle) \
    public: \
        zfoverride \
        virtual zfbool styleableIsDefaultStyle(void) { \
            return zftrue; \
        } \
    }; \
    zfanyT<YourStyle> YourStyle::DefaultStyle(void) { \
        static _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(#YourStyle); \
        if(holder->d == zfnull) { \
            ZFCoreMutexLocker(); \
            if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateNotAvailable) { \
                return zfnull; \
            } \
            zfauto obj = _ZFP_ZFStyleableDefault_##YourStyle::ClassData()->newInstance(); \
            if(obj != zfnull) { \
                zfself::_ZFP_ZFStyleablEnumDefaultStyle(obj.to<YourStyle *>()); \
            } \
        } \
        return (YourStyle *)holder->d; \
    } \
    ZFMETHOD_USER_REGISTER_DETAIL_0(YourStyle, \
        public, ZFMethodTypeStatic, s, \
        zfanyT<YourStyle>, DefaultStyle) { \
        return YourStyle::DefaultStyle(); \
    } \
    void YourStyle::_ZFP_ZFStyleablEnumDefaultStyle(ZF_IN YourStyle *newInstance) { \
        if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateNotAvailable) { \
            return; \
        } \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(#YourStyle); \
        if(holder->d == newInstance) { \
            return; \
        } \
        const ZFCorePointer *&cleanerRef = _ZFP_ZFStyleableDefaultCleaner(); \
        const ZFCorePointer *cleanerOld = cleanerRef; \
        const ZFCorePointer *cleanerNew = zfnull; \
        cleanerRef = zfnull; \
        if(newInstance != zfnull) { \
            holder->d = zfobjRetain(newInstance); \
            cleanerNew = ZFObjectGlobalInstanceAdd(ZFCorePointerForObject<_ZFP_ZFStyleableDefaultDeleteCallbackHolder *>( \
                zfnew(_ZFP_ZFStyleableDefaultDeleteCallbackHolder, YourStyle::_ZFP_ZFStyleableDefaultOnDelete, holder->d)), \
                _ZFP_ZFStyleableDefault_level); \
            cleanerRef = cleanerNew; \
        } \
        if(cleanerOld != zfnull) { \
            ZFObjectGlobalInstanceRemove(cleanerOld, _ZFP_ZFStyleableDefault_level); \
            holder->d = newInstance; \
            cleanerRef = cleanerNew; \
        } \
    } \
    const ZFCorePointer *&YourStyle::_ZFP_ZFStyleableDefaultCleaner(void) { \
        static const ZFCorePointer *_cleaner = zfnull; \
        return _cleaner; \
    } \
    void YourStyle::_ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance) { \
        YourStyle::_ZFP_ZFStyleableDefaultCleaner() = zfnull; \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(#YourStyle); \
        holder->d = zfnull; \
        zfobjRelease((YourStyle *)instance); \
    }

/**
 * @brief used to declare a default style, see #ZFStyleable
 *
 * usage:
 * @code
 *   // in h file
 *   zfclass YourStyleableObject : zfextend ParentStyleable {
 *       ZFOBJECT_DECLARE(YourStyleableObject, ParentStyleable)
 *       ZFSTYLE_DEFAULT_DECLARE(YourStyleableObject)
 *   };
 *
 *   // in cpp file
 *   ZFSTYLE_DEFAULT_DEFINE(YourStyleableObject)
 * @endcode
 * the macros above declare these types for you:
 * -  default style access method:
 *   static zfanyT<YourObjectStyle> DefaultStyle(void);
 *   virtual zfanyT<ZFStyleable> defaultStyle(void);
 *
 * \n
 * @note a style holder object would be created automatically when access the default style,
 *   and would be deleted automatically during #ZFFrameworkCleanup with level #ZFLevelZFFrameworkHigh,
 *   so it would result null if you access DefaultStyle before initialized or after it's deallocated
 * @note the default style holder object should only be used to hold the styles,
 *   typically you should not use it directly
 * @note it is useful to combine styleable logic and serializable logic,
 *   see #ZFStyleSet for more info
 * @note for performance, the default style won't be copied to its instance by default,
 *   you may use #ZFClass::instanceObserverAdd to observe styleable object's
 *   instance alloc event, and apply your default styles\n
 *   or use #ZFStyleDefaultApplyAutoCopy or #ZFSTYLE_DEFAULT_AUTO_COPY to make the auto copy
 */
#define ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle)

/**
 * @brief util method to setup automatically copy style from
 *   #ZFStyleable::defaultStyle if its property value changed
 *
 * for performance, the default style won't be copied to each instance by default,
 * by calling this method, when #ZFStyleable::defaultStyle's property value changed,
 * it would be copied to the registered instance
 */
extern ZFLIB_ZFCore void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style);

/**
 * @brief util macro to setup automatically copy style from
 *   #ZFStyleable::defaultStyle
 *
 * use #ZFINTERFACE_ON_INIT_DECLARE to copy styles during init,
 * and use #ZFStyleDefaultApplyAutoCopy to ensure default style value changes would be applied
 */
#define ZFSTYLE_DEFAULT_AUTO_COPY() \
    ZFINTERFACE_ON_INIT_DECLARE() { \
        if(!this->styleableIsDefaultStyle()) { \
            zfanyT<ZFStyleable> defaultStyle = this->defaultStyle(); \
            if(defaultStyle != zfnull) { \
                this->styleableCopyFrom(defaultStyle); \
                ZFStyleDefaultApplyAutoCopy(this); \
            } \
        } \
    }

// ============================================================
// style holder
/**
 * @brief used to store style holder
 *
 * typical style logic are implemented by:
 * 1. use #ZFStyleable::styleKey to attach object to observe style change
 * 1. use #ZFStyleSet or #ZFStyleLoad to modify styles
 * 1. during style change event,
 *   all the styles would be copied automatically
 *
 * example:
 * @code
 *   // register
 *   MyStyleObject *obj = xxx;
 *   obj->styleKey("MyStyle/MyStyleObject");
 *
 *   // change style
 *   ZFStyleUpdateBegin();
 *       ZFStyleSet("MyStyle/MyStyleObject", xxx);
 *       ZFStyleUpdateBegin(); // can be embeded, but must be paired
 *           ZFStyleSet("xxx", xxx);
 *       ZFStyleUpdateEnd();
 *   ZFStyleUpdateEnd();
 *
 *   // use ZFStyleLoad is recommended
 *   ZFStyleLoad(xxx);
 * @endcode
 * \n
 * \n
 * the style logic can also be used during serialization,
 * when serializable contains key #ZFSerializableKeyword_styleKey,
 * the style would be copied during serialization,
 * and restored when serialized back to data,
 * a typical workflow:
 * @code
 *   // original src data
 *   <MyStyleObject styleKey="MyStyle/MyStyleObject" overridedProp="overridedValue" />
 *
 *   // serialize from data
 *   // step 1: load style and copy style to target object
 *   <MyStyleObject someProp="styleValue" overridedProp="styleValue" />
 *   // step 2: serialize as normal
 *   <MyStyleObject someProp="styleValue" overridedProp="overridedValue" />
 *
 *   // serialize back to data
 *   // step 1: serialize as normal
 *   <MyStyleObject someProp="styleValue" overridedProp="overridedValue" />
 *   // step 2: load referenced style object if exists
 *   <ReferencedStyle someProp="styleValue" overridedProp="styleValue" />
 *   // step 3: compare the target and referenced style object,
 *   //         remove duplicate attributes
 *   <MyStyleObject overridedProp="overridedValue" />
 *   // step 4: finally restore the styleKey
 *   <MyStyleObject styleKey="MyStyle/MyStyleObject" overridedProp="overridedValue" />
 *
 *   // note the final data is identical with original src data,
 *   // and impl only needs to take care of refOwner
 *   // during #ZFSerializable::serializeToData
 * @endcode
 * \n
 * \n
 * by default, all #ZFStyleable supports style logic,
 * for non-ZFStyleable types, #zfint for example,
 * can also benifit from style logic if:
 * -  registered by #ZFTYPEID_DECLARE
 * -  it's property declared by #ZFPROPERTY_ASSIGN series
 *
 * to apply style logic for these non-ZFStyleable types:
 * @code
 *   ownerZFStyleable->propStyle("yourPropertyName", "yourStyleKey");
 * @endcode
 * when style of yourStyleKey changed,
 * yourPropertyName's setter method would be called to apply the style\n
 * \n
 * \n
 * note, by default, all styles would be cleared during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkNormal
 * \n
 * ADVANCED:\n
 * you may register your own style decoder by #ZFSTYLE_DECODER_DEFINE
 * to supply additional style logic\n
 * by default, we bundled a special load logic by #ZFStyleDecoder_ZFObjectIO,
 * which would load style from specified path info, for example:
 * @code
 *   <MyStyleObject styleKey="@res!!my_cfg.xml" />
 * @endcode
 */
extern ZFLIB_ZFCore void ZFStyleSet(
        ZF_IN const zfstring &styleKey
        , ZF_IN ZFStyleable *styleValue
        );
/**
 * @brief see #ZFStyleSet
 *
 * @note the returned object is the original object set by #ZFStyleSet,
 *   use #ZFStyleable::styleableCopyFrom or #ZFCopyable::copy
 *   to create new one if necessary,
 *   you should not modify the original object
 */
extern ZFLIB_ZFCore zfauto ZFStyleGet(ZF_IN const zfstring &styleKey);
/**
 * @brief get all styles, for debug use only, see #ZFStyleSet
 */
extern ZFLIB_ZFCore void ZFStyleGetAll(
        ZF_IN_OUT ZFCoreArray<zfstring> &styleKey
        , ZF_IN_OUT ZFCoreArray<zfauto> &styleValue
        );
/** @brief see #ZFStyleGetAll */
extern ZFLIB_ZFCore void ZFStyleGetAllKeyT(
        ZF_IN_OUT ZFCoreArray<zfstring> &styleKey
        );
/** @brief see #ZFStyleGetAll */
inline ZFCoreArray<zfstring> ZFStyleGetAllKey(void) {
    ZFCoreArray<zfstring> ret;
    ZFStyleGetAllKeyT(ret);
    return ret;
}
/** @brief see #ZFStyleGetAll */
extern ZFLIB_ZFCore void ZFStyleGetAllValueT(
        ZF_IN_OUT ZFCoreArray<zfauto> &styleValue
        );
/** @brief see #ZFStyleGetAll */
inline ZFCoreArray<zfauto> ZFStyleGetAllValue(void) {
    ZFCoreArray<zfauto> ret;
    ZFStyleGetAllValueT(ret);
    return ret;
}

/**
 * @brief remove all styles, see #ZFStyleSet
 */
extern ZFLIB_ZFCore void ZFStyleRemoveAll(void);

/**
 * @brief see #ZFStyleSet
 */
extern ZFLIB_ZFCore void ZFStyleUpdateBegin();
/**
 * @brief see #ZFStyleSet
 */
extern ZFLIB_ZFCore void ZFStyleUpdateEnd();

/**
 * @brief util macro to call #ZFStyleUpdateBegin/#ZFStyleUpdateEnd
 */
#define ZFStyleUpdateBlock() _ZFP_ZFStyleUpdateBlock ZFUniqueName(_ZFP_ZFStyleUpdateBlock)
zfclassLikePOD _ZFP_ZFStyleUpdateBlock {
public:
    _ZFP_ZFStyleUpdateBlock(void) {
        ZFStyleUpdateBegin();
    }
    ~_ZFP_ZFStyleUpdateBlock(void) {
        ZFStyleUpdateEnd();
    }
};

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFStyleUpdateBegin
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFStyleOnUpdateBegin)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFStyleUpdateEnd
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFStyleOnUpdate)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when setting an invalid style value\n
 * sender is the styleable object that changing the styleKey,
 * param0 is a #v_zfstring if chaning property's name
 * or holds empty string if changing the styleable object itself,
 * param1 is a #v_zfstring that holds the new styleKey
 */
ZFEVENT_GLOBAL(ZFLIB_ZFCore, ZFStyleOnInvalid)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
/**
 * @brief register a custom decoder for #ZFStyleSet
 *
 * usage:
 * @code
 *   // in header file
 *   / **
 *    * add your doc here
 *    * /
 *   #define ZFStyleDecoder_xxx xxx
 *
 *   ZFSTYLE_DECODER_DEFINE(ZFStyleDecoder_xxx, {
 *       // perform your decode action, proto type:
 *       //   zfbool decode(
 *       //           ZF_OUT zfauto &ret
 *       //           , ZF_IN const zfstring &styleKey
 *       //           );
 *   })
 * @endcode
 * all of the decoder would be executed once (order is not ensured) until success,
 * if none returned true, builtin style map would be searched instead
 */
#define ZFSTYLE_DECODER_DEFINE(registerSig, decodeAction, ...) \
    _ZFP_ZFSTYLE_DECODER_DEFINE(registerSig, decodeAction, ##__VA_ARGS__)
#define _ZFP_ZFSTYLE_DECODER_DEFINE(registerSig, decodeAction, ...) \
    ZF_STATIC_REGISTER_INIT(ZFStyleDecoder_##registerSig) { \
        _ZFP_ZFStyleDecoderRegister(zftext(#registerSig), zfself::_ZFP_decode); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFStyleDecoder_##registerSig) { \
        _ZFP_ZFStyleDecoderUnregister(zftext(#registerSig)); \
    } \
    static zfbool _ZFP_decode( \
            ZF_OUT zfauto &ret \
            , ZF_IN const zfstring &styleKey \
            ) { \
        decodeAction __VA_ARGS__ \
    } \
    ZF_STATIC_REGISTER_END(ZFStyleDecoder_##registerSig)
typedef zfbool (*_ZFP_ZFStyleDecoder)(
        ZF_OUT zfauto &ret
        , ZF_IN const zfstring &styleKey
        );
extern ZFLIB_ZFCore void _ZFP_ZFStyleDecoderRegister(
        ZF_IN const zfstring &registerSig
        , ZF_IN _ZFP_ZFStyleDecoder decoder
        );
extern ZFLIB_ZFCore void _ZFP_ZFStyleDecoderUnregister(ZF_IN const zfstring &registerSig);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleable_h_


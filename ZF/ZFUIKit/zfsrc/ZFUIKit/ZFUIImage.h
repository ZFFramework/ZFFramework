/**
 * @file ZFUIImage.h
 * @brief image
 */

#ifndef _ZFI_ZFUIImage_h_
#define _ZFI_ZFUIImage_h_

#include "ZFUIGlobalStyle.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImage
zfclassFwd _ZFP_ZFUIImagePrivate;
/**
 * @brief image storage
 * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
 *
 * \n
 * serializable data:
 * @code
 *   <ZFUIImage imageType="type name for ZFUIIMAGE_SERIALIZE_TYPE_DEFINE"
 *       customAttr="customValue" // extra attributes for impl, see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *       imageBin="image binary data, base64 encoded" // if nothing supplied, we will serialize by binary data
 *   >
 *       <anything ... /> // extra attributes for impl, see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 *   </ZFUIImage>
 * @endcode
 */
zffinal zfclass ZFLIB_ZFUIKit ZFUIImage : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFUIImage, ZFStyle)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #imageScaleFixed changed
     */
    ZFEVENT(ImageScaleOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #imageState changed
     */
    ZFEVENT(ImageStateOnUpdate)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);

    // ============================================================
    // property
public:
    /**
     * @brief image's scale, 1 by default
     *
     * we have these image scale settings:
     * -  #ZFUIImage::imageScale, scale setting, serializable
     * -  #ZFUIGlobalStyle::imageScale, global scale for all images
     * -  #ZFUIImage::imageScaleFixed, util method to access actual scale
     *
     * to access pixel size, use #imageSizeFixed instead
     */
    ZFPROPERTY_ASSIGN(zffloat, imageScale, 1)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, imageScale)
    /**
     * @brief nine patch described by a margin value, disabled if zero margin or margin exceeds image size
     * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageNinePatch)

    /**
     * @brief util method to access actual scale
     *
     * ensured to be (#imageScale * #ZFUIGlobalStyle::imageScale)
     */
    ZFMETHOD_DECLARE_0(zffloat const &, imageScaleFixed)

    /**
     * @brief get size of the image
     * @note #ZFUIImage always use custom scale value, see #ZFUIImage::imageScale
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, imageSize)
    /**
     * @brief get raw size of the image without scale (in pixel unit)
     *
     * this is the pixel size of the image,
     * not affected by #imageScale
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, imageSizeFixed)

    // ============================================================
    // image state
public:
    /**
     * @brief return current image state
     *
     * used to implement stateful image, for example:
     * -  animated image
     * -  dynamic loading image
     *
     * how it works:
     * -# as for normal image, the #imageState would return the ZFUIImage itself
     * -# impl may create a dummy placeholder ZFUIImage object,
     *   supplying #imageStateImpl as actual impl
     *
     * note: valid only when any observer attached by #imageStateAttach,
     * to explicitly obtain latest state, use #imageStateUpdate
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUIImage>, imageState)
    /** @brief see #imageState */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUIImage>, imageStateUpdate)

    /**
     * @brief for impl to attach image state update observer
     *
     * callback would be called when first time attached,
     * and each time if #imageState updated,
     * sender would be the original owner ZFUIImage\n
     * \n
     * note: when no longer used, impl must call #imageStateDetach to cleanup
     */
    ZFMETHOD_DECLARE_1(void, imageStateAttach
            , ZFMP_IN(const ZFListener &, callback)
            )
    /** @brief see #imageStateAttach */
    ZFMETHOD_DECLARE_1(void, imageStateDetach
            , ZFMP_IN(const ZFListener &, callback)
            )
    /**
     * @brief for impl to implement stateful image logic
     *
     * there are different ways to call the impl:
     * -  when first observer attached and each time #imageStateUpdate called:
     *   -  sender would be the owner ZFUIImage object
     *   -  param0 would be a #v_ZFUIImageStateImplAction::e_Attach
     *   -  impl must notify owner ZFUIImage's imageStateImplNotifyUpdate,
     *     immediately and each time needs to update
     * -  when all observer detached:
     *   -  sender would be the owner ZFUIImage object
     *   -  param0 would be a #v_ZFUIImageStateImplAction::e_Detach
     * -  when owner image copy to another image:
     *   -  sender would be the owner ZFUIImage object
     *   -  param0 would be a #v_ZFUIImageStateImplAction::e_Copy
     *   -  param1 would be the target ZFUIImage object
     */
    ZFMETHOD_DECLARE_1(void, imageStateImpl
            , ZFMP_IN(const ZFListener &, impl)
            )
    /** @brief see #imageStateImpl */
    ZFMETHOD_DECLARE_0(const ZFListener &, imageStateImpl)
    /** @brief see #imageStateImpl */
    ZFMETHOD_DECLARE_1(void, imageStateImplNotifyUpdate
            , ZFMP_IN(ZFUIImage *, imageState)
            )

    // ============================================================
    // other
    ZFOBJECT_PRIVATE_ALLOC("can only be created by ZFUIImageLoadXxx series")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);
    zfoverride
    virtual void objectPropertyValueOnUpdate(
            ZF_IN const ZFProperty *property
            , ZF_IN const void *oldValue
            );

public:
    /**
     * @brief get native image object
     */
    ZFMETHOD_DECLARE_0(void *, nativeImage)

public:
    zffinal void _ZFP_ZFUIImage_imageScaleOnUpdate(void);
protected:
    /** @brief see #E_ImageScaleOnUpdate */
    virtual void imageScaleOnUpdate(void) {
        this->observerNotify(ZFUIImage::E_ImageScaleOnUpdate());
    }

    /** @brief see #E_ImageStateOnUpdate */
    virtual void imageStateOnUpdate(void) {
        this->observerNotify(ZFUIImage::E_ImageStateOnUpdate());
    }

    // ============================================================
    // internal methods
public:
    /**
     * @brief internal use only, set the native image
     *
     * the image would be retained, actual retain logic is depending on the implementation
     */
    ZFMETHOD_DECLARE_2(void, nativeImage
            , ZFMP_IN(void *, nativeImage)
            , ZFMP_IN_OPT(zfbool, retainNativeImage, zftrue)
            )

public:
    /** @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE */
    ZFMETHOD_DECLARE_1(void, imageSerializeType
            , ZFMP_IN(const zfstring &, typeName)
            )
    /** @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE */
    ZFMETHOD_DECLARE_0(const zfstring &, imageSerializeType)
    /** @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE */
    ZFMETHOD_DECLARE_1(void, imageSerializeData
            , ZFMP_IN(const ZFSerializableData &, serializableData)
            )
    /** @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE */
    ZFMETHOD_DECLARE_0(const ZFSerializableData &, imageSerializeData)
    /**
     * @brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
     *
     * impl should store the result ZFSerializableData to #ZFArgs::result,
     * or store a #v_zfstring containing error hint
     */
    ZFMETHOD_DECLARE_1(void, imageSerializeDataGetter
            , ZFMP_IN(const ZFListener &, impl)
            )
    /**@brief see #ZFUIIMAGE_SERIALIZE_TYPE_DEFINE */
    ZFMETHOD_DECLARE_0(const ZFListener &, imageSerializeDataGetter)

    /**@brief see #ZFSerializeDisable */
    ZFMETHOD_DECLARE_1(void, imageSerializeDisable
            , ZFMP_IN(zfbool, disable)
            )
    /**@brief see #ZFSerializeDisable */
    ZFMETHOD_DECLARE_0(zfbool, imageSerializeDisable)

private:
    _ZFP_ZFUIImagePrivate *d;
    friend zfclassFwd _ZFP_ZFUIImagePrivate;
};

// ============================================================
typedef zfbool (*_ZFP_ZFUIImageSerializeFromCallback)(
        ZF_IN_OUT ZFUIImage *image
        , ZF_IN const ZFSerializableData &imageData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        );

extern ZFLIB_ZFUIKit void _ZFP_ZFUIImageSerializeTypeRegister(
        ZF_IN const zfstring &name
        , ZF_IN _ZFP_ZFUIImageSerializeFromCallback fromCallback
        );
extern ZFLIB_ZFUIKit void _ZFP_ZFUIImageSerializeTypeUnregister(ZF_IN const zfstring &name);
/**
 * @brief register a serializable logic for ZFUIImage
 *
 * ZFUIImage can be loaded from many sources,
 * so we supply a dynamic serializable logic depending on image's load logic,
 * use this macro to register your own serialize logic,
 * such as load from file or load from color\n
 * \n
 * example
 * @code
 *   // in cpp file
 *   ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(registerSig, yourTypeName) {
 *       // serialize callback from data
 *       // proto type:
 *       //   zfbool action(
 *       //           ZF_IN_OUT ZFUIImage *ret
 *       //           , ZF_IN const ZFSerializableData &serializableData
 *       //           , ZF_OUT_OPT zfstring *outErrorHint
 *       //           , ZF_OUT_OPT ZFSerializableData *outErrorPos
 *       //           );
 *       return zftrue;
 *   }
 * @endcode
 *
 * when image loaded or serialized successfully,
 * impl may store these serializable datas to the image:
 * -  #ZFUIImage::imageSerializeType : the type registered by ZFUIIMAGE_SERIALIZE_TYPE_DEFINE
 * -  #ZFUIImage::imageSerializeData or #ZFUIImage::imageSerializeDataGetter :
 *   actual serializable data,
 *   or a callback to obtain (store result to #ZFArgs::result) the serializable data
 */
#define ZFUIIMAGE_SERIALIZE_TYPE_DEFINE(registerSig, typeName) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##registerSig(ZF_IN_OUT ZFUIImage *ret, \
                                                                    ZF_IN const ZFSerializableData &serializableData, \
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageSerializeReg_##registerSig, ZFLevelZFFrameworkHigh) { \
        _ZFP_ZFUIImageSerializeTypeRegister(typeName, _ZFP_ZFUIImageSerializeFromCallback_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIImageSerializeReg_##registerSig) { \
        _ZFP_ZFUIImageSerializeTypeUnregister(typeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFUIImageSerializeReg_##registerSig) \
    static zfbool _ZFP_ZFUIImageSerializeFromCallback_##registerSig(ZF_IN_OUT ZFUIImage *ret, \
                                                                    ZF_IN const ZFSerializableData &serializableData, \
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)

/** @brief see #ZFUIImageSerializeTypeGetAll */
extern ZFLIB_ZFUIKit void ZFUIImageSerializeTypeGetAllT(ZF_IN_OUT ZFCoreArray<zfstring> &ret);
/**
 * @brief usually for debug use only
 */
inline ZFCoreArray<zfstring> ZFUIImageSerializeTypeGetAll(void) {
    ZFCoreArray<zfstring> ret;
    ZFUIImageSerializeTypeGetAllT(ret);
    return ret;
}

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_imageType "imageType"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIImage_imageBin "imageBin"

// ============================================================
/** @brief see #ZFUIImage::imageStateImpl */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIImageStateImplAction)
    ZFENUM_VALUE(Attach) /**< @brief see #ZFUIImage::imageStateImpl */
    ZFENUM_VALUE(Detach) /**< @brief see #ZFUIImage::imageStateImpl */
    ZFENUM_VALUE(Copy) /**< @brief see #ZFUIImage::imageStateImpl */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Attach)
    ZFENUM_VALUE_REGISTER(Detach)
    ZFENUM_VALUE_REGISTER(Copy)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIImageStateImplAction)
ZFENUM_REG(ZFLIB_ZFUIKit, ZFUIImageStateImplAction)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImage_h_


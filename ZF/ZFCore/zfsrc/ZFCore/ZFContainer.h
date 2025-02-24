/**
 * @file ZFContainer.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFContainer_h_
#define _ZFI_ZFContainer_h_

#include "ZFObject.h"
#include "ZFIterable.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief container of ZFObject
 *
 * automatically retain the object when add,
 * and release it after remove\n
 * \n
 * a container can not contain null,
 * you may want to use #ZFNull if necessary\n
 * \n
 * a container is serializable only if
 * all of it's content are serializable,
 * for performance, you should not check the serializable state too frequently\n
 * clear contents before serialize from data\n
 * whether contents of the container is sorted,
 * is depends on subclass\n
 * \n
 * serializable data:
 * @code
 *   <ContainerClass> // container class
 *       <ElementClass0 /> // each elements
 *       <ElementClass1 />
 *       ...
 *   </ContainerClass>
 * @endcode
 *
 * @note a container would have special serializing step with ref or style logic,
 *   see #serializableOnSerializeToData for more info
 */
zfabstract ZFLIB_ZFCore ZFContainer : zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable, zfimplement ZFIterable {
    ZFOBJECT_DECLARE_ABSTRACT(ZFContainer, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable, ZFIterable)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element added or removed or order changed
     */
    ZFEVENT(ContentOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element added\n
     * param0 is the element added to this container
     */
    ZFEVENT(ContentOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element removed\n
     * param0 is the element removed from this container
     */
    ZFEVENT(ContentOnRemove)

public:
    /**
     * @brief add data from another container
     */
    virtual void addFrom(ZF_IN ZFContainer *another) zfpurevirtual;

public:
    /**
     * @brief return a short string describe the content
     */
    ZFMETHOD_DECLARE_3(void, objectInfoOfContentT
            , ZFMP_IN_OUT(zfstring &, ret)
            , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
            , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
            )
    /** @brief see #objectInfoOfContentT */
    ZFMETHOD_DECLARE_2(zfstring, objectInfoOfContent
            , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
            , ZFMP_IN_OPT(const ZFTokenForContainer &, token, ZFTokenForContainerDefault())
            )

protected:
    /** @brief see #E_ContentOnUpdate */
    virtual inline void contentOnUpdate(void) {
        this->observerNotify(ZFContainer::E_ContentOnUpdate());
    }
    /** @brief see #E_ContentOnAdd */
    virtual inline void contentOnAdd(ZF_IN ZFObject *element) {
        this->observerNotify(ZFContainer::E_ContentOnAdd(), element);
    }
    /** @brief see #E_ContentOnRemove */
    virtual inline void contentOnRemove(ZF_IN ZFObject *element) {
        this->observerNotify(ZFContainer::E_ContentOnRemove(), element);
    }

protected:
    zfoverride
    virtual zfbool serializableOnCheck(void);
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    /**
     * @brief see #ZFSerializable::serializableOnSerializeToData
     *
     * while serializing a container to data with ref or style logic,
     * it's hard to auto serialize all contents,
     * to workaround this problem,
     * we would serialize all contents normally if no ref logic found,
     * and would call #serializableOnSerializeToDataWithRef otherwise,
     * you should override #serializableOnSerializeToDataWithRef
     * if you have different ref logic
     */
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );
protected:
    /**
     * @brief see #serializableOnSerializeToData
     */
    virtual zfbool serializableOnSerializeToDataWithRef(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            );

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj);

public:
    /** @brief see #ZFObject::objectOnDeallocPrepare, remove all contents before dealloc */
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        this->objectInfoOfContentT(ret, 10);
    }

    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFContainer_h_


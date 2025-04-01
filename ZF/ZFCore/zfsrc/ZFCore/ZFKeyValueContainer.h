/**
 * @file ZFKeyValueContainer.h
 * @brief key value container of ZFObject
 */

#ifndef _ZFI_ZFKeyValueContainer_h_
#define _ZFI_ZFKeyValueContainer_h_

#include "ZFIterable.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFKeyValueContainerPrivate;
/**
 * @brief key value container of ZFObject
 *
 * automatically retain Key and Value when add,
 * and release it after remove
 * @note all Keys must be comparable and sortable
 *   (#ZFObject::objectCompare must not return ZFCompareUncomparable)
 * @see ZFObject::objectCompare
 *
 * to go through all the contents, you should:
 * @code
 *   for(zfiter it = container->iter(); it; ++it) {
 *       if(condition) {
 *           // safe to remove since keys and values is a copy of the contents
 *           // however, you must not access the removed elements after remove
 *           container->iterRemove(it);
 *       }
 *   }
 * @endcode
 * \n
 * a key value container is serializable only if
 * all of it's key and value are serializable\n
 * for performance, you should not check the serializable state too frequently\n
 * clear contents before serialize from data\n
 * \n
 * serializable data:
 * @code
 *   <ContainerClass>
 *       <node>
 *           <ElementKeyClass0 />
 *           <ElementValueClass0 />
 *       </node>
 *       <node>
 *           <ElementKeyClass1 />
 *           <ElementValueClass1 />
 *       </node>
 *       ...
 *   </ContainerClass>
 * @endcode
 */
zfabstract ZFLIB_ZFCore ZFKeyValueContainer: zfextend ZFObject, zfimplement ZFSerializable, zfimplement ZFCopyable, zfimplement ZFIterable, zfimplement ZFIterableKeyValue {
    ZFOBJECT_DECLARE_ABSTRACT(ZFKeyValueContainer, ZFObject)
    ZFIMPLEMENT_DECLARE(ZFSerializable, ZFCopyable, ZFIterable, ZFIterableKeyValue)

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
     * param0 is the element key,
     * param1 is the element value
     */
    ZFEVENT(ContentOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when element removed\n
     * param0 is the element key,
     * param1 is the element value
     */
    ZFEVENT(ContentOnRemove)

public:
    /**
     * @brief add data from another container
     */
    virtual void addFrom(ZF_IN ZFKeyValueContainer *another) zfpurevirtual;

    /** @cond ZFPrivateDoc */
    virtual void iterAdd(ZF_IN ZFObject *value) {ZFCoreCriticalNotSupported();}
    virtual void iterAdd(
            ZF_IN ZFObject *value
            , ZF_IN_OUT zfiter &it
            ) {
        ZFCoreCriticalNotSupported();
    }
    virtual void iterAdd(
            ZF_IN ZFObject *key
            , ZF_IN ZFObject *value
            ) zfpurevirtual;
    /** @endcond */

public:
    /**
     * @brief return a short string describe the content
     */
    ZFMETHOD_DECLARE_3(void, objectInfoOfContentT
            , ZFMP_IN_OUT(zfstring &, ret)
            , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
            , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
            )
    /** @brief see #objectInfoOfContentT */
    ZFMETHOD_DECLARE_2(zfstring, objectInfoOfContent
            , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
            , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
            )

protected:
    /** @brief see #E_ContentOnUpdate */
    virtual inline void contentOnUpdate(void) {
        this->observerNotify(ZFKeyValueContainer::E_ContentOnUpdate());
    }
    /** @brief see #E_ContentOnAdd */
    virtual inline void contentOnAdd(
            ZF_IN ZFObject *key
            , ZF_IN ZFObject *value
            ) {
        this->observerNotify(ZFKeyValueContainer::E_ContentOnAdd(), key, value);
    }
    /** @brief see #E_ContentOnRemove */
    virtual inline void contentOnRemove(
            ZF_IN ZFObject *key
            , ZF_IN ZFObject *value
            ) {
        this->observerNotify(ZFKeyValueContainer::E_ContentOnRemove(), key, value);
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
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );
protected:
    /**
     * @brief see #ZFKeyValueContainer::serializableOnSerializeToData
     */
    virtual zfbool serializableOnSerializeToDataWithRef(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    zfoverride
    virtual void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        this->objectInfoOfContentT(ret, 5);
    }

    zfoverride
    virtual ZFCompareResult objectCompareValueImpl(ZF_IN ZFObject *anotherObj);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFKeyValueContainer_h_


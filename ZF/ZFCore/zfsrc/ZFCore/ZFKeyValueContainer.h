/**
 * @file ZFKeyValueContainer.h
 * @brief key value container of ZFObject
 */

#ifndef _ZFI_ZFKeyValueContainer_h_
#define _ZFI_ZFKeyValueContainer_h_

#include "ZFContainer.h"
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
 *
 * @note by default, container would serialize using #ZFSerializableData to holds
 *   necessary type info, which would make the serialize result quite verbose,
 *   to make it able to serialize from and to string, you may:
 *   -  declare your own dynamic subclass of container (typically by #ZFDynamic::classBegin)
 *   -  declare two dynamic method named `keyClass` and `valueClass`, and return the element's class
 *   -  serializing your dynamic subclass by #ZFObjectFromString/#ZFObjectToString
 */
zfabstract ZFLIB_ZFCore ZFKeyValueContainer: zfextend ZFContainer {
    ZFOBJECT_DECLARE_ABSTRACT(ZFKeyValueContainer, ZFContainer)

public:
    /**
     * @brief get key value pair with iter, see #zfiter
     */
    ZFMETHOD_INTERFACE_1(zfany, iterKey
            , ZFMP_IN(const zfiter &, it)
            )

public:
    /**
     * @brief add key value to tail, see #zfiter
     *
     * for most key value container,
     * this is same as set key with value
     */
    ZFMETHOD_INTERFACE_2(zfiter, iterAdd
            , ZFMP_IN(ZFObject *, key)
            , ZFMP_IN(ZFObject *, value)
            )
    /**
     * @brief return true if already exist, or false and insert <key, defValue> if not exist
     */
    ZFMETHOD_INTERFACE_3(zfbool, iterAccess
            , ZFMP_OUT(zfiter &, it)
            , ZFMP_IN(ZFObject *, key)
            , ZFMP_IN_OPT(ZFObject *, defValue, ZFNull())
            )

public:
    /**
     * @brief add data from another container
     */
    ZFMETHOD_INTERFACE_1(void, addFrom
            , ZFMP_IN(ZFKeyValueContainer *, another)
            )

private:
    /** @cond ZFPrivateDoc */
    ZFMETHOD_OVERRIDE_INLINE_1(zfiter, iterAdd
            , ZFMP_IN(ZFObject *, value)
            ) {
        ZFCoreCriticalNotSupported();
        return zfnull;
    }
    ZFMETHOD_OVERRIDE_INLINE_2(zfiter, iterAdd
            , ZFMP_IN(ZFObject *, value)
            , ZFMP_IN_OUT(zfiter &, it)
            ) {
        ZFCoreCriticalNotSupported();
        return zfnull;
    }
    ZFMETHOD_OVERRIDE_INLINE_1(void, addFrom
            , ZFMP_IN(ZFContainer *, another)
            ) {
        if(another != zfnull) {
            zfself *tmp = zfcast(zfself *, another);
            if(tmp == zfnull) {
                ZFCoreCriticalNotSupported();
                return;
            }
            this->addFrom(tmp);
        }
    }
    /** @endcond */

public:
    zfoverride
    virtual void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            );
    zfoverride
    virtual zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            );
public:
    /**
     * @brief return a short string describe the content
     */
    ZFMETHOD_DECLARE_3(void, objectInfoOfContentT
            , ZFMP_IN_OUT(zfstring &, ret)
            , ZFMP_IN(zfindex, maxCount)
            , ZFMP_IN(const ZFTokenForKeyValueContainer &, token)
            )
    /** @brief see #objectInfoOfContentT */
    ZFMETHOD_DECLARE_2(zfstring, objectInfoOfContent
            , ZFMP_IN(zfindex, maxCount)
            , ZFMP_IN(const ZFTokenForKeyValueContainer &, token)
            )

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
    virtual zfbool serializableOnSerializeFromString(
            ZF_IN const zfchar *src
            , ZF_IN_OPT zfindex srcLen = zfindexMax()
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToString(
            ZF_IN_OUT zfstring &ret
            , ZF_OUT_OPT zfstring *errorHint = zfnull
            );

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


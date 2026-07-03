/**
 * @file ZFContainer.h
 * @brief container of ZFObject
 */

#ifndef _ZFI_ZFContainer_h_
#define _ZFI_ZFContainer_h_

#include "ZFObject.h"

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
 * @note by default, container would serialize using #ZFSerializableData to holds
 *   necessary type info, which would make the serialize result quite verbose,
 *   to make it able to serialize from and to string, you may:
 *   -  declare your own dynamic subclass of container (typically by #ZFDynamic::classBegin)
 *   -  declare a dynamic method named `elementClass`, and return the element's class
 *   -  serializing your dynamic subclass by #ZFObjectFromString/#ZFObjectToString
 */
zfabstract ZFLIB_ZFCore ZFContainer : zfextend ZFStyle {
    ZFOBJECT_DECLARE_ABSTRACT(ZFContainer, ZFStyle)

public:
    /**
     * @brief change capacity
     */
    virtual void capacity(ZF_IN zfindex capacity) zfpurevirtual;

    /**
     * @brief return total count of this iterable
     */
    virtual zfindex count(void) zfpurevirtual;
    /**
     * @brief return a iter, see #zfiter
     */
    virtual zfiter iter(void) zfpurevirtual;

    /**
     * @brief find element
     */
    virtual zfiter iterFind(ZF_IN ZFObject *element) zfpurevirtual;

    /**
     * @brief get value by iter, see #zfiter
     *
     * iter must be valid
     */
    virtual zfany iterValue(ZF_IN const zfiter &it) zfpurevirtual;

public:
    /**
     * @brief set value at iter, see #zfiter
     */
    virtual void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN ZFObject *value
            ) zfpurevirtual;
    /**
     * @brief remove value at iter, see #zfiter
     */
    virtual void iterRemove(ZF_IN_OUT zfiter &it) zfpurevirtual;
    /**
     * @brief remove all contents of this iterable
     */
    virtual void removeAll(void) zfpurevirtual;

    /**
     * @brief add value to tail
     */
    virtual zfiter iterAdd(ZF_IN ZFObject *value) zfpurevirtual;
    /**
     * @brief add value before iter,
     *   see #zfiter
     */
    virtual zfiter iterAdd(
            ZF_IN ZFObject *value
            , ZF_IN_OUT zfiter &it
            ) zfpurevirtual;

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
     * @brief see #serializableOnSerializeToData
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
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);

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


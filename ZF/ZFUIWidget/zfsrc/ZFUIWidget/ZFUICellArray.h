/**
 * @file ZFUICellArray.h
 * @brief basic cell adapter which hold all cells directly (with no recycle logic)
 */

#ifndef _ZFI_ZFUICellArray_h_
#define _ZFI_ZFUICellArray_h_

#include "ZFUICellAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUICellArray_cell "ZFSerializableKeyword_ZFUICellArray_cell"

/**
 * @brief basic cell adapter which hold all cells directly (with no recycle logic)
 *
 * serializable data:
 * @code
 *   <ZFUICellArray>
 *       <SomeView category="cell" />
 *       ... // each cell
 *   </ZFUICellArray>
 * @endcode
 */
zfclass ZFLIB_ZFUIWidget ZFUICellArray : zfextend ZFStyle, zfimplement ZFUICellAdapter {
    ZFOBJECT_DECLARE(ZFUICellArray, ZFStyle)
    ZFIMPLEMENT_DECLARE(ZFUICellAdapter)

protected:
    zfoverride
    virtual void objectOnInit(void) {
        zfsuper::objectOnInit();
        d = zfobjAlloc(ZFArray);
    }
    zfoverride
    virtual void objectOnDealloc(void) {
        zfobjRelease(d);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->cellRemoveAll();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief directly access internal cell array,
     *   you may modify it directly, with caution
     */
    ZFMETHOD_INLINE_0(ZFArray *, cellArray) {
        return d;
    }
    /**
     * @brief add cell
     */
    ZFMETHOD_INLINE_1(void, cell
            , ZFMP_IN(ZFUICell *, cell)
            ) {
        d->add(cell);
    }
    /**
     * @brief add cell at index
     */
    ZFMETHOD_INLINE_2(void, cell
            , ZFMP_IN(ZFUICell *, cell)
            , ZFMP_IN(zfindex, index)
            ) {
        d->add(cell, index);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_2(void, cellRemove
            , ZFMP_IN(zfindex, index)
            , ZFMP_IN_OPT(zfindex, count, 1)
            ) {
        d->remove(index, count);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_1(void, cellRemoveElement
            , ZFMP_IN(ZFUICell *, cell)
            ) {
        d->removeElement(cell);
    }
    /**
     * @brief remove all cell
     */
    ZFMETHOD_INLINE_0(void, cellRemoveAll) {
        d->removeAll();
    }

public:
    zfoverride
    virtual zfindex cellCount(void) {
        return d->count();
    }
    zfoverride
    virtual zfautoT<ZFUICell> cellAt(ZF_IN zfindex index) {
        return d->get(index);
    }

protected:
    zfoverride
    virtual zfauto cellCacheOnAccess(ZF_IN zfindex index) {
        return d->get(index);
    }

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

private:
    ZFArray *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUICellArray_h_


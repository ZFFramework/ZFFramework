#include "ZFUICellArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUICellArray)

zfbool ZFUICellArray::serializableOnSerializeFromData(
        ZF_IN const ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    this->cellRemoveAll();

    for(zfindex i = 0; i < serializableData.childCount(); ++i) {
        const ZFSerializableData &categoryData = serializableData.childAt(i);
        if(categoryData.resolved()) {continue;}
        zfstring category = ZFSerializableUtil::checkCategory(categoryData);
        if(!zfstringIsEqual(category, ZFSerializableKeyword_ZFUICellArray_cell)) {continue;}

        zfauto element;
        if(!ZFObjectFromDataT(element, categoryData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(element == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "null cell");
            return zffalse;
        }
        if(!element->classData()->classIsTypeOf(ZFUICell::ClassData())) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                element->objectInfoOfInstance(), ZFUICell::ClassData()->classNameFull());
            return zffalse;
        }
        this->cell(element);

        categoryData.resolveMark();
    }
    return zftrue;
}
zfbool ZFUICellArray::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        , ZF_IN_OPT ZFSerializable *refOwner /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, outErrorHint, refOwner)) {return zffalse;}
    zfself *ref = zfcast(zfself *, refOwner);

    if(ref == zfnull || ref->cellCount() == 0) {
        for(zfindex i = 0, count = this->cellCount(); i < count; ++i) {
            ZFSerializableData cellData;
            if(!ZFObjectToDataT(cellData, this->cellAt(i), outErrorHint)) {
                return zffalse;
            }
            cellData.category(ZFSerializableKeyword_ZFUICellArray_cell);
            serializableData.child(cellData);
        }
    }
    else {
        zfbool cellAllSame = zftrue;
        if(this->cellCount() != ref->cellCount()) {
            cellAllSame = zffalse;
        }
        else {
            for(zfindex i = 0; i < this->cellCount(); ++i) {
                if(!ZFObjectCompare(this->cellAt(i), ref->cellAt(i))) {
                    cellAllSame = zffalse;
                }
            }
        }
        if(!cellAllSame) {
            ZFSerializableUtilErrorOccurred(outErrorHint,
                "cell list mismatch, this: %s, ref: %s",
                this->objectInfoOfInstance(), ref->objectInfoOfInstance());
            return zffalse;
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


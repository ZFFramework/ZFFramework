#include "ZFUIListAdapterArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListAdapterArray)

zfbool ZFUIListAdapterArray::serializableOnSerializeFromData(
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
        if(!zfstringIsEqual(category, ZFSerializableKeyword_ZFUIListAdapterArray_cell)) {continue;}

        zfauto element;
        if(!ZFObjectFromDataT(element, categoryData, outErrorHint, outErrorPos)) {
            return zffalse;
        }
        if(element == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "null cell");
            return zffalse;
        }
        if(!element->classData()->classIsTypeOf(ZFUIListCell::ClassData())) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                element->objectInfoOfInstance(), ZFUIListCell::ClassData()->classNameFull());
            return zffalse;
        }
        this->cellAdd(element);

        categoryData.resolveMark();
    }
    return zftrue;
}
zfbool ZFUIListAdapterArray::serializableOnSerializeToData(
        ZF_IN_OUT ZFSerializableData &serializableData
        , ZF_IN ZFSerializable *referencedOwnerOrNull
        , ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */
        ) {
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = zfcast(zfself *, referencedOwnerOrNull);

    if(ref == zfnull || ref->cellCount() == 0) {
        for(zfindex i = 0, count = this->cellCount(); i < count; ++i) {
            ZFSerializableData cellData;
            if(!ZFObjectToDataT(cellData, this->cellAt(i), outErrorHint)) {
                return zffalse;
            }
            cellData.category(ZFSerializableKeyword_ZFUIListAdapterArray_cell);
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


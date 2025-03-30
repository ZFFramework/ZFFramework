#include "ZFStruct.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFStruct)

ZFOBJECT_ON_INIT_DEFINE_2(ZFStruct
        , ZFMP_IN(const zfchar *, src)
        , ZFMP_IN_OPT(zfindex, srcLen, zfindexMax())
        ) {
    zfstring errorHint;
    if(!this->serializeFromString(src, srcLen, &errorHint)) {
        this->objectTag(ZFObjectTagKeyword_newInstanceGenericFailed, zfobj<v_zfstring>(errorHint));
    }
}

zfidentity ZFStruct::objectHashImpl(void) {
    ZFCoreArray<const ZFProperty *> allProperty = this->classData()->propertyGetAll();
    zfidentity ret = 0;
    for(zfindex i = 0; i < allProperty.count(); ++i) {
        zfauto v = allProperty[i]->getterMethod()->methodInvoke(this);
        if(v) {
            ret = zfidentityHash(ret, v->objectHash());
        }
    }
    return ret;
}
ZFCompareResult ZFStruct::objectCompareImpl(ZF_IN ZFObject *anotherObj) {
    return ZFPropertyAllEqual(this, anotherObj)
        ? ZFCompareEqual
        : ZFCompareUncomparable
        ;
}
ZFCompareResult ZFStruct::objectCompareValueImpl(ZF_IN ZFObject *anotherObj) {
    return ZFPropertyAllEqual(this, anotherObj)
        ? ZFCompareEqual
        : ZFCompareUncomparable
        ;
}
void ZFStruct::objectInfoImpl(ZF_IN_OUT zfstring &ret) {
    ZFObjectVerboseInfoT(ret, this);
}

zfbool ZFStruct::serializableOnSerializeFromString(
        ZF_IN const zfchar *src
        , ZF_IN_OPT zfindex srcLen /* = zfindexMax() */
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ZFCoreArray<ZFIndexRange> posList;
    if(!ZFCoreDataPairSplitString(
                posList
                , zfindexMax()
                , src
                , srcLen
                , ","
                , "("
                , ")"
                , zftrue
                )) {
        zfstringAppend(errorHint, "invalid value: %s", zfstring(src, srcLen));
        return zffalse;
    }
    ZFCoreArray<const ZFProperty *> allProperty = this->classData()->propertyGetAll();
    for(zfindex i = 0; i < posList.count() && i < allProperty.count(); ++i) {
        const ZFProperty *prop = allProperty[i];
        const ZFIndexRange &pos = posList[i];
        if(pos.count == 0) {
            ZFPropertyValueReset(prop, this);
            continue;
        }
        zfauto v;
        if(!ZFDI_objectFromString(v, prop->propertyTypeId(), src + pos.start, pos.count, errorHint)) {
            return zffalse;
        }
        prop->setterMethod()->methodInvoke(this, v);
    }
    return zftrue;
}
zfbool ZFStruct::serializableOnSerializeToString(
        ZF_IN_OUT zfstring &ret
        , ZF_OUT_OPT zfstring *errorHint /* = zfnull */
        ) {
    ret += "(";
    ZFCoreArray<const ZFProperty *> allProperty = this->classData()->propertyGetAll();
    for(zfindex i = 0; i < allProperty.count(); ++i) {
        if(i != 0) {
            ret += ",";
        }
        const ZFProperty *prop = allProperty[i];
        if(!ZFPropertyIsInitValue(prop, this)) {
            zfauto v = prop->getterMethod()->methodInvoke(this);
            if(!ZFObjectToStringT(ret, v, errorHint)) {
                return zffalse;
            }
        }
    }
    ret += ")";
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


#include "ZFPropertySerializable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"

#include "ZFPropertyDeclare.h"
#include "ZFPropertyUserRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_DEFINE(ZFProperty, const ZFProperty *, {
        if(zfstringIsEqual(serializableData.itemClass(), ZFSerializableKeyword_null)) {
            v = zfnull;
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFProperty(), outErrorHint, outErrorPos) == zfnull) {
            return zffalse;
        }

        zfstring tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_owner, outErrorHint, outErrorPos);
        if(tmpValue == zfnull) {
            return zffalse;
        }
        const ZFClass *ownerClass = ZFClass::classForName(tmpValue);
        if(ownerClass == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "no such class \"%s\"", tmpValue);
            return zffalse;
        }

        tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_property, outErrorHint, outErrorPos);
        if(tmpValue == zfnull) {
            return zffalse;
        }
        v = ownerClass->propertyForName(tmpValue);
        if(v == zfnull) {
            ZFSerializableUtilErrorOccurredAt(outErrorHint, outErrorPos, serializableData,
                "no such property \"%s\" in class \"%s\"", tmpValue, ownerClass->classNameFull());
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v == zfnull) {
            serializableData.itemClass(ZFSerializableKeyword_null);
            return zftrue;
        }

        serializableData.itemClass(ZFTypeId_ZFProperty());

        serializableData.attr(ZFSerializableKeyword_ZFProperty_owner, v->propertyOwnerClass()->classNameFull());
        serializableData.attr(ZFSerializableKeyword_ZFProperty_property, v->propertyName());

        return zftrue;
    }, {
        ZFCoreArray<ZFIndexRange> pos;
        if(!zfCoreDataPairSplitString(pos, 3, src, srcLen, ":", zfnull, zfnull, zftrue)) {return zffalse;}
        const ZFClass *cls = ZFClass::classForName(zfstring(src + pos[0].start, pos[0].count));
        if(cls == zfnull) {
            return zffalse;
        }
        v = cls->propertyForName(zfstring(src + pos[2].start, pos[2].count));
        return (v != zfnull);
    }, {
        if(v) {
            s += v->propertyOwnerClass()->classNameFull();
            s += "::";
            s += v->propertyName();
        }
        return zftrue;
    })
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const zfstring &, propertyInternalId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, zfbool, propertyIsUserRegister)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, zfbool, propertyIsDynamicRegister)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, zfany, propertyDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const ZFClass *, propertyOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const zfstring &, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const zfstring &, propertyTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const zfstring &, propertyTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const ZFMethod *, setterMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const ZFMethod *, getterMethod)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, zfbool, propertyIsRetainProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFProperty, const ZFClass *, propertyClassOfRetainProperty)

ZF_NAMESPACE_GLOBAL_END


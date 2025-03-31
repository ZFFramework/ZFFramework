#include "ZFPropertyUtil.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFPropertyCopyAll(
        ZF_IN ZFObject *dstObj
        , ZF_IN ZFObject *srcObj
        ) {
    if(dstObj == zfnull || srcObj == zfnull) {
        return;
    }

    ZFCoreArray<const ZFProperty *> allProperty = dstObj->classData()->propertyGetAll();
    const ZFClass *srcClass = srcObj->classData();
    for(zfindex i = 0; i < allProperty.count(); ++i) {
        const ZFProperty *property = allProperty.get(i);
        if(!srcClass->classIsTypeOf(property->ownerClass())
                || property->isInternal()
                || !property->setterMethod()->isPublic()
                || !property->getterMethod()->isPublic()
                ) {
            continue;
        }
        ZFPropertyCopy(property, dstObj, srcObj);
    }
}

// ============================================================
zfbool ZFPropertyAllEqual(
        ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        ) {
    if(obj0 == obj1) {
        return zftrue;
    }
    if(obj0 == zfnull || obj1 == zfnull) {
        return zffalse;
    }
    const ZFClass *cls0 = obj0->classData();
    const ZFClass *cls1 = obj1->classData();

    ZFCoreArray<const ZFProperty *> allProperty = cls0->propertyGetAll();
    for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i) {
        const ZFProperty *prop = allProperty[i];
        if(!prop->isInternal()) {
            if((!cls1->classIsTypeOf(prop->ownerClass()) && !prop->ownerClass()->classIsTypeOf(cls1))
                    || ZFPropertyCompareValue(prop, obj0, obj1) != ZFCompareEqual
                    ) {
                return zffalse;
            }
        }
    }

    return zftrue;
}

void ZFObjectPropertyInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */
        ) {
    if(obj == zfnull) {
        return;
    }

    ZFCoreArray<const ZFProperty *> allProperty = obj->classData()->propertyGetAll();
    zfindex count = 0;
    zfindex index = 0;
    for( ; index < allProperty.count() && count < maxCount; ++index) {
        const ZFProperty *prop = allProperty[index];
        if(
                prop->isInternal()
                || ZFPropertyIsInitValue(prop, obj)
                ) {
            continue;
        }

        if(count == 0) {
            ret += token.tokenLeft;
        }
        else {
            ret += token.tokenSeparator;
        }
        ++count;

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            ret += prop->propertyName();
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            {
                zfstring s;
                ZFObjectInfoT(s, prop->getterMethod()->methodInvoke(obj));

                zfindex iL = 0;
                for(zfindex i = 0; i < s.length(); i += zfcharGetSize(s + i)) {
                    if(s[i] == '\n') {
                        ret.append(s, iL, i + 1 - iL);
                        iL = i + 1;
                        ret += "    ";
                    }
                }
                if(iL < s.length()) {
                    ret.append(s, iL, s.length() - iL);
                }
            }
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
    }
    if(index < allProperty.count()) {
        if(count > 0) {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    if(count > 0) {
        ret += token.tokenRight;
    }
}

void ZFObjectVerboseInfoT(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */
        ) {
    if(obj == zfnull) {
        ret += ZFTOKEN_zfnull;
        return;
    }

    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += obj->classData()->classNameFull();
    ret += "(";
    zfsFromPointerT(ret, obj);
    ret += ")";

    ZFObjectPropertyInfoT(ret, obj, maxCount, token);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsValueAccessed
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(zfany const &, ownerObj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyIsInitValue
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(zfany const &, ownerObj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCompareResult, ZFPropertyCompare
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, obj0)
        , ZFMP_IN(ZFObject *, obj1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCompareResult, ZFPropertyCompareValue
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, obj0)
        , ZFMP_IN(ZFObject *, obj1)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, ZFPropertyCopy
        , ZFMP_IN(const ZFProperty *, propertyInfo)
        , ZFMP_IN(ZFObject *, dstObj)
        , ZFMP_IN(ZFObject *, srcObj)
        )

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfbool, ZFPropertyAllEqual
        , ZFMP_IN(ZFObject *, dstObj)
        , ZFMP_IN(ZFObject *, srcObj)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFObjectPropertyInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfstring, ZFObjectPropertyInfo
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFObjectVerboseInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfstring, ZFObjectVerboseInfo
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )

ZF_NAMESPACE_GLOBAL_END
#endif


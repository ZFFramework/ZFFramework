#include "ZFClassUtil.h"
#include "ZFObjectImpl.h"
#include "ZFFilterForZFClass.h"
#include "ZFFilterForZFMethod.h"
#include "ZFFilterForZFProperty.h"
#include "ZFPropertyUtil.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFClassUtil)

void allClassParentT(
        ZF_IN_OUT ZFCoreArray<const ZFClass *> &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT const ZFFilterForZFClass *filter /* = zfnull */
        ) {
    if(filter == zfnull) {
        while(cls != zfnull) {
            ret.add(cls);
            for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
                ret.add(cls->dynamicInterfaceAt(i));
            }
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
                ret.add(cls->implementedInterfaceAt(i));
            }
            cls = cls->classParent();
        }
    }
    else {
        while(cls != zfnull) {
            if(filter->filterPassed(cls)) {
                ret.add(cls);
            }
            for(zfindex i = 0; i < cls->dynamicInterfaceCount(); ++i) {
                if(filter->filterPassed(cls->dynamicInterfaceAt(i))) {
                    ret.add(cls->dynamicInterfaceAt(i));
                }
            }
            for(zfindex i = 0; i < cls->implementedInterfaceCount(); ++i) {
                if(filter->filterPassed(cls->implementedInterfaceAt(i))) {
                    ret.add(cls->implementedInterfaceAt(i));
                }
            }
            cls = cls->classParent();
        }
    }
}

void allMethodT(
        ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT const ZFFilterForZFMethod *filter /* = zfnull */
        ) {
    ZFCoreArray<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i) {
        const ZFClass *cls = allClassParent.get(i);
        for(zfiter it = cls->methodIter(); it; ++it) {
            const ZFMethod *method = cls->methodIterValue(it);
            if(filter == zfnull || filter->filterPassed(method)) {
                ret.add(method);
            }
        }
    }
}

void allPropertyT(
        ZF_IN_OUT ZFCoreArray<const ZFProperty *> &ret
        , ZF_IN const ZFClass *cls
        , ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */
        ) {
    ZFCoreArray<const ZFClass *> allClassParent = ZFClassUtil::allClassParent(cls, (filter == zfnull) ? zfnull : &(filter->classFilter));
    for(zfindex i = 0; i < allClassParent.count(); ++i) {
        const ZFClass *cls = allClassParent.get(i);
        for(zfiter it = cls->propertyIter(); it; ++it) {
            const ZFProperty *property = cls->propertyIterValue(it);
            if(filter == zfnull || filter->filterPassed(property)) {
                ret.add(property);
            }
        }
    }
}

zfbool allPropertyIsEqual(
        ZF_IN ZFObject *obj0
        , ZF_IN ZFObject *obj1
        , ZF_IN_OPT const ZFFilterForZFProperty *filter /* = zfnull */
        ) {
    if(obj0 == obj1) {
        return zftrue;
    }
    if(obj0 == zfnull || obj1 == zfnull) {
        return zffalse;
    }
    const ZFClass *cls0 = obj0->classData();
    const ZFClass *cls1 = obj1->classData();

    ZFCoreArray<const ZFProperty *> allProperty = ZFClassUtil::allProperty(cls0, filter);
    for(zfindex i = allProperty.count() - 1; i != zfindexMax(); --i) {
        if(cls1->classIsTypeOf(allProperty[i]->ownerClass())
                && ZFPropertyCompareValue(allProperty[i], obj0, obj1) != ZFCompareEqual
                ) {
            return zffalse;
        }
    }

    return zftrue;
}

void objectPropertyInfo(
        ZF_IN_OUT zfstring &ret
        , ZF_IN ZFObject *obj
        , ZF_IN_OPT zfindex maxCount /* = zfindexMax() */
        , ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault() */
        ) {
    if(obj == zfnull) {
        return;
    }

    ZFCoreArray<const ZFProperty *> allProperty = ZFClassUtil::allProperty(obj->classData());
    zfindex count = 0;
    zfindex index = 0;
    for( ; index < allProperty.count() && count < maxCount; ++index) {
        if(ZFPropertyIsInitValue(allProperty[index], obj)) {
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
            ret += allProperty[index]->propertyName();
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            {
                zfstring s;
                ZFPropertyGetInfo(s, allProperty[index], obj);

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

void objectInfoT(
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

    ZFClassUtil::objectPropertyInfo(ret, obj, maxCount, token);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

ZF_NAMESPACE_END(ZFClassUtil)

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFClassUtil)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allClassParentT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFClass *> &, ret)
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFClass *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<const ZFClass *>, allClassParent
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFClass *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allMethodT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret)
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<const ZFMethod *>, allMethod
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(void, allPropertyT
        , ZFMP_IN_OUT(ZFCoreArray<const ZFProperty *> &, ret)
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(ZFCoreArray<const ZFProperty *>, allProperty
        , ZFMP_IN(const ZFClass *, cls)
        , ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, allPropertyIsEqual
        , ZFMP_IN(ZFObject *, obj0)
        , ZFMP_IN(ZFObject *, obj1)
        , ZFMP_IN_OPT(const ZFFilterForZFProperty *, filter, zfnull)
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, objectPropertyInfo
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfstring, objectPropertyInfo
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, objectInfoT
        , ZFMP_IN_OUT(zfstring &, ret)
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfstring, objectInfo
        , ZFMP_IN(ZFObject *, obj)
        , ZFMP_IN_OPT(zfindex, maxCount, zfindexMax())
        , ZFMP_IN_OPT(const ZFTokenForKeyValueContainer &, token, ZFTokenForKeyValueContainerDefault())
        )

ZF_NAMESPACE_END(ZFClassUtil)
ZF_NAMESPACE_GLOBAL_END
#endif


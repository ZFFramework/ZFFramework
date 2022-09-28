#include "ZFFilterForZFProperty.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFFilterForZFProperty::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    zfindex flagsCount = 0;
    if(this->ignorePublicSetter())
    {
        ++flagsCount;
        ret += "ignorePublicSetter, ";
    }
    if(this->ignoreProtectedSetter())
    {
        ++flagsCount;
        ret += "ignoreProtectedSetter, ";
    }
    if(this->ignorePrivateSetter())
    {
        ++flagsCount;
        ret += "ignorePrivateSetter, ";
    }
    if(this->ignorePublicGetter())
    {
        ++flagsCount;
        ret += "ignorePublicGetter, ";
    }
    if(this->ignoreProtectedGetter())
    {
        ++flagsCount;
        ret += "ignoreProtectedGetter, ";
    }
    if(this->ignorePrivateGetter())
    {
        ++flagsCount;
        ret += "ignorePrivateGetter, ";
    }

    if(flagsCount == 0)
    {
        ret.remove(ret.length() - 1);
    }
    else
    {
        ret.remove(ret.length() - 2);
        ret += ')';
    }
    zfsuper::objectInfoT(ret);
}

zfbool ZFFilterForZFProperty::filterOnCheckActive(ZF_IN const ZFProperty * const &e) const
{
    if(!zfsuper::filterOnCheckActive(e)
       || !this->classFilter.filterCheckActive(e->propertyOwnerClass()))
    {
        return zffalse;
    }

    if((e->setterMethod()->methodIsPrivate() && this->ignorePrivateSetter())
       || (e->setterMethod()->methodIsProtected() && this->ignoreProtectedSetter())
       || (e->setterMethod()->methodIsPublic() && this->ignorePublicSetter())
       || (e->getterMethod()->methodIsPrivate() && this->ignorePrivateGetter())
       || (e->getterMethod()->methodIsProtected() && this->ignoreProtectedGetter())
       || (e->getterMethod()->methodIsPublic() && this->ignorePublicGetter()))
    {
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


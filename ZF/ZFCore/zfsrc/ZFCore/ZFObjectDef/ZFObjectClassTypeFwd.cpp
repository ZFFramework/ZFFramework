#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFClassDataUpdateData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFClassDataUpdateData changeType:%s changedClass:%s changedProperty:%s changedMethod:%s name:%s>"
            , this->changeType
            , this->changedClass
            , this->changedProperty
            , this->changedMethod
            , this->name
            );
}

ZF_NAMESPACE_GLOBAL_END


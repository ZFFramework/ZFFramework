#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFClassDataChangeData::objectInfoT(ZF_IN_OUT zfstring &ret) const {
    zfstringAppend(ret, "<ZFClassDataChangeData changeType:%s changedClass:%s changedProperty:%s changedMethod:%s name:%s>"
            , this->changeType
            , this->changedClass
            , this->changedProperty
            , this->changedMethod
            , this->name
            );
}

ZF_NAMESPACE_GLOBAL_END


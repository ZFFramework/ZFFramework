#include "ZFUICell.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUICell)

// ============================================================
ZFOBJECT_REGISTER(ZFUICell)
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUICell, zfanyT<ZFUIView>, cellView) {
    if(this->cellView() != zfnull) {
        this->internalFgViewAdd(this->cellView(), this->cellViewLayoutParam());
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUICell, zfanyT<ZFUIView>, cellView) {
    if(this->cellView() != zfnull) {
        this->internalFgViewRemove(this->cellView());
    }
}

ZFPROPERTY_ON_INIT_DEFINE(ZFUICell, zfanyT<ZFUILayoutParam>, cellViewLayoutParam) {
    propertyValue = this->layoutParamCreate();
    ZFUILayoutParam *value = propertyValue;
    value->sizeParam(ZFUISizeParamFillFill());
    value->align(ZFUIAlign::e_Center);
}

void ZFUICell::layoutOnMeasure(
        ZF_OUT ZFUISize &ret
        , ZF_IN const ZFUISize &sizeHint
        , ZF_IN const ZFUISizeParam &sizeParam
        ) {
    if(this->cellView() == zfnull) {
        zfsuper::layoutOnMeasure(ret, sizeHint, sizeParam);
        return;
    }

    this->cellView()->layoutMeasure(sizeHint, ZFUISizeParamWrapWrap());
    ret = this->cellView()->layoutMeasuredSize();
}

ZF_NAMESPACE_GLOBAL_END


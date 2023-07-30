/**
 * @file ZFUIButtonGroupBasic.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroupBasic_h_
#define _ZFI_ZFUIButtonGroupBasic_h_

#include "ZFUIButtonGroup.h"
#include "ZFUILinearLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupBasic
/**
 * @brief basic button group which layout buttons as #ZFUILinearLayout
 */
zfclass ZFLIB_ZFUIWidget ZFUIButtonGroupBasic : zfextends ZFUILinearLayout, zfimplements ZFUIButtonGroup {
    ZFOBJECT_DECLARE(ZFUIButtonGroupBasic, ZFUILinearLayout)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonGroup)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonGroupBasic)

protected:
    zfoverride
    virtual inline void buttonOnAdd(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->childAdd(button, buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnAdd(button, buttonIndex);
    }
    zfoverride
    virtual inline void buttonOnRemove(
            ZF_IN ZFUIButton *button
            , ZF_IN zfindex buttonIndex
            ) {
        this->childRemoveAt(buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnRemove(button, buttonIndex);
    }

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            ) {
        return zfsuper::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)
            && zfsuperI(ZFUIButtonGroup)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos);
    }
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_IN ZFSerializable *referencedOwnerOrNull
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            ) {
        return zfsuper::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)
            && zfsuperI(ZFUIButtonGroup)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint);
    }
    zfoverride
    virtual inline zfbool serializableOnCheckNeedSerializeChildren(void) {
        return zfsuperI(ZFUIButtonGroup)::serializableOnCheckNeedSerializeChildren();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroupBasic_h_


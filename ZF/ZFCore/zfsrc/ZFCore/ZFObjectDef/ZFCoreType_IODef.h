/**
 * @file ZFCoreType_IODef.h
 * @brief type io with io callbacks
 */

#ifndef _ZFI_ZFCoreType_IODef_h_
#define _ZFI_ZFCoreType_IODef_h_

#include "ZFIOCallback.h"
#include "ZFSerializable.h"
#include "ZFFilterForZFClass.h"
#include "ZFFilterForZFMethod.h"
#include "ZFFilterForZFProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// util
#define _ZFP_ZFCoreType_IODef_outputObjectPointer() { \
        if(v) { \
            v->objectInfoT(s); \
        } \
        else { \
            s += ZFTOKEN_zfnull; \
        } \
    }

// ============================================================
// common output for core types
ZFOUTPUT_TYPE_TEMPLATE(
        ZFM_EXPAND(typename T_Public, typename T_Internal)
        , ZFM_EXPAND(ZFFilterBase<T_Public, T_Internal>)
        , {
            v.objectInfoT(s);
        })

ZFOUTPUT_TYPE_TEMPLATE(
        typename T_Element
        , ZFCoreArray<T_Element>
        , {
            v.objectInfoT(s);
        })

ZFOUTPUT_TYPE_TEMPLATE(
        typename T_Element
        , ZFCoreQueuePOD<T_Element>
        , {
            v.objectInfoT(s);
        })

ZFOUTPUT_TYPE(ZFCoreMap, {v.objectInfoT(s);})

ZFOUTPUT_TYPE(ZFCorePointerBase, {v.objectInfoT(s);})

// ============================================================
// common output for ZFObject types
ZFOUTPUT_TYPE(ZFFilterForZFClass, {v.objectInfoT(s);})

ZFOUTPUT_TYPE(ZFObject *, _ZFP_ZFCoreType_IODef_outputObjectPointer())

ZFOUTPUT_TYPE(const ZFClass *, _ZFP_ZFCoreType_IODef_outputObjectPointer())

ZFOUTPUT_TYPE(const ZFMethod *, _ZFP_ZFCoreType_IODef_outputObjectPointer())

ZFOUTPUT_TYPE(const ZFProperty *, _ZFP_ZFCoreType_IODef_outputObjectPointer())

ZFOUTPUT_TYPE(ZFObserver, {v.objectInfoT(s);})

ZFOUTPUT_TYPE(ZFArgs, {v.objectInfoT(s);})

ZFOUTPUT_TYPE(ZFAny, {
    if(v.toObject()) {
        v.toObject()->objectInfoT(s);
    }
    else {
        s += ZFTOKEN_zfnull;
    }
})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCoreType_IODef_h_


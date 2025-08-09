/**
 * @file ZFTextTemplateRun.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateRun_h_
#define _ZFI_ZFTextTemplateRun_h_

#include "ZFTextTemplate.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global param for #ZFTextTemplateRun, affects all tasks of #ZFTextTemplateRun
 *
 * while running the #ZFTextTemplateRun,
 * these things can be applied with #ZFTextTemplateApply:
 * -  dir name
 * -  dir content (process all child files and dirs)
 * -  file name
 * -  file content
 *
 * if dir name or file name not filtered,
 * they would be renamed by #ZFTextTemplateApply\n
 * if dir content not filtered,
 * all the child files and dirs would be applied\n
 * if file content not filtered,
 * the content would be applied with #ZFTextTemplateApply
 */
zfclass ZFLIB_ZFUtility ZFTextTemplateRunFilter : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFTextTemplateRunFilter, ZFStyle)
    ZFOBJECT_SINGLETON_DECLARE(ZFTextTemplateRunFilter, instance)
public:
    /** @brief see #ZFTextTemplateRunFilter, array of #ZFRegExp */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, dirNameFilter, zfobj<ZFArray>())
    /** @brief see #ZFTextTemplateRunFilter, array of #ZFRegExp */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, dirContentFilter, zfobj<ZFArray>())
    /** @brief see #ZFTextTemplateRunFilter, array of #ZFRegExp */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, fileNameFilter, zfobj<ZFArray>())
    /** @brief see #ZFTextTemplateRunFilter, array of #ZFRegExp */
    ZFPROPERTY_RETAIN_READONLY(zfanyT<ZFArray>, fileContentFilter, zfobj<ZFArray>())
protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable) {
        zfself *ref = zfcast(zfself *, anotherStyleable);
        if(ref != zfnull && ref != this) {
            this->dirNameFilter()->copyFrom(ref->dirNameFilter());
            this->dirContentFilter()->copyFrom(ref->dirContentFilter());
            this->fileNameFilter()->copyFrom(ref->fileNameFilter());
            this->fileContentFilter()->copyFrom(ref->fileContentFilter());
        }
    }
};

/**
 * @brief util method to apply #ZFTextTemplateApply to a given path recursively
 */
ZFMETHOD_FUNC_DECLARE_3(ZFLIB_ZFUtility, zfbool, ZFTextTemplateRun
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const ZFTextTemplateParam &, textTemplateParam)
        , ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull)
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateRun_h_


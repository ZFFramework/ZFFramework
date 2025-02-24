/**
 * @file ZFSigName.h
 * @brief readonly string container for internal use only
 */

#ifndef _ZFI_ZFSigName_h_
#define _ZFI_ZFSigName_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFSigNamePrivate;
/**
 * @brief readonly string container for internal use only
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFSigName {
public:
    /** @cond ZFPrivateDoc */
    ZFSigName(void);
    ZFSigName(ZF_IN const ZFSigName &ref);
    ZFSigName(ZF_IN const zfstring &s);
    ZFSigName(ZF_IN const zfchar *s);
    ZFSigName(ZF_IN const zfnullT &dummy);
    ~ZFSigName(void);
    /** @endcond */

public:
    /**
     * @brief internal id for the string content
     *
     * the id may change if all the reference to one string content is released
     */
    zfidentity sigId(void) const;

public:
    /** @brief whether empty */
    zfbool isEmpty(void) const;

    /** @brief access string value, return empty string if empty */
    const zfchar *cString(void) const;

    /** @brief length of string content */
    zfindex length(void) const;

    /** @brief string comparation */
    zfint compare(ZF_IN const ZFSigName &ref) const;

    /** @brief string comparation */
    zfint compare(ZF_IN const zfstring &s) const {
        return ((const zfstring &)(*this)).compare(s);
    }

    /** @brief string comparation */
    zfint compare(ZF_IN const zfchar *s) const {
        return zfscmp(this->cString(), s ? s : "");
    }
    /** @cond ZFPrivateDoc */
public:
    operator const zfchar * (void) const {return this->cString();}
    operator const zfstring & (void) const;
public:
    ZFSigName &operator = (ZF_IN const ZFSigName &ref);
    ZFSigName &operator = (ZF_IN const zfstring &s);
    ZFSigName &operator = (ZF_IN const zfchar *s);
    ZFSigName &operator = (ZF_IN const zfnullT &dummy);
public:
    zfbool operator == (ZF_IN const ZFSigName &ref) const {return this->compare(ref) == 0;}
    zfbool operator != (ZF_IN const ZFSigName &ref) const {return this->compare(ref) != 0;}
    zfbool operator == (ZF_IN const zfstring &s) const {return this->compare(s) == 0;}
    zfbool operator != (ZF_IN const zfstring &s) const {return this->compare(s) != 0;}
    zfbool operator == (ZF_IN const zfchar *s) const {return this->compare(s) == 0;}
    zfbool operator != (ZF_IN const zfchar *s) const {return this->compare(s) != 0;}
    zfbool operator == (ZF_IN const zfnullT &dummy) const {return this->isEmpty();}
    zfbool operator != (ZF_IN const zfnullT &dummy) const {return !this->isEmpty();}
public:
    /* ZFTAG_TRICKS: tricks to make zfstlmap<ZFSigName, xxx> works */
    inline zfbool operator < (ZF_IN const ZFSigName &ref) const {return this->compare(ref) < 0;}
    inline zfbool operator <= (ZF_IN const ZFSigName &ref) const {return this->compare(ref) <= 0;}
    inline zfbool operator > (ZF_IN const ZFSigName &ref) const {return this->compare(ref) > 0;}
    inline zfbool operator >= (ZF_IN const ZFSigName &ref) const {return this->compare(ref) >= 0;}
    /** @endcond */

private:
    _ZFP_ZFSigNamePrivate *d;
};
ZFOUTPUT_TYPE(ZFSigName, {s += v.cString();})

/**
 * @brief print all state, for debug use only
 */
extern ZFLIB_ZFCore void ZFSigNameInfo(ZF_OUT zfstring &ret);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFSigName_h_


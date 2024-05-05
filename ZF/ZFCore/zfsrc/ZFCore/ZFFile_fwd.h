/**
 * @file ZFFile_fwd.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_fwd_h_
#define _ZFI_ZFFile_fwd_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileSeparator
extern ZFLIB_ZFCore const zfchar _ZFP_ZFFileSeparator;
extern ZFLIB_ZFCore const zfchar *_ZFP_ZFFileSeparatorString;

/**
 * @brief file separator (ensured to be '/' for now)
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, zfchar, ZFFileSeparator, _ZFP_ZFFileSeparator)

/**
 * @brief string version of #ZFFileSeparator
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFLIB_ZFCore, const zfchar *, ZFFileSeparatorString, _ZFP_ZFFileSeparatorString)

// ============================================================
// ZFFileOpenOption
/**
 * @brief open falgs, similar to fopen's flags,
 *   while we have binary mode only
 *
 * usage:
 * -  ZFFileOpenOption::e_Create\n
 *   create a new file for write only
 * -  ZFFileOpenOption::e_Create | ZFFileOpenOption::e_Read\n
 *   create a new file for read/write
 * -  ZFFileOpenOption::e_Read\n
 *   open a existing file for read only, fail if file not exist
 * -  ZFFileOpenOption::e_Read | ZFFileOpenOption::e_Write\n
 *   open a existing file for read/write, fail if file not exist
 * -  ZFFileOpenOption::e_Write\n
 *   same as ZFFileOpenOption::e_Create, create a new file for write only
 * -  ZFFileOpenOption::e_Append\n
 *   open a file for append or create new if not exist, for write only
 * -  ZFFileOpenOption::e_Append | ZFFileOpenOption::e_Read\n
 *   open a file for append or create new if not exist, for read/write
 */
ZFENUM_BEGIN(ZFLIB_ZFCore, ZFFileOpenOption)
    ZFENUM_VALUE_WITH_INIT(Create, 1) /**< @brief create for read write */
    ZFENUM_VALUE_WITH_INIT(Read, 2) /**< @brief readonly */
    ZFENUM_VALUE_WITH_INIT(Write, 4) /**< @brief write only */
    ZFENUM_VALUE_WITH_INIT(Append, 8) /**< @brief append only */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Create)
    ZFENUM_VALUE_REGISTER(Read)
    ZFENUM_VALUE_REGISTER(Write)
    ZFENUM_VALUE_REGISTER(Append)
ZFENUM_END_FLAGS(ZFLIB_ZFCore, ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
zfclassFwd _ZFP_ZFFileFindDataPrivate;
/**
 * @brief data used by ZFFile when finding files
 * @see ZFFileFindFirst, ZFFileFindNext, ZFFileFindClose
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFFileFindData {
public:
    /** @cond ZFPrivateDoc */
    ZFFileFindData(void);
    ZFFileFindData(ZF_IN ZFFileFindData const &ref);
    ZFFileFindData &operator = (ZF_IN ZFFileFindData const &ref);
    zfbool operator == (ZF_IN ZFFileFindData const &ref) const;
    zfbool operator != (ZF_IN ZFFileFindData const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
    ~ZFFileFindData(void);

public:
    /**
     * @brief return file name of file
     */
    const zfchar *fileName(void) const {
        return this->impl().fileName;
    }
    /**
     * @brief return true if is a directory
     */
    zfbool fileIsDir(void) const {
        return this->impl().fileIsDir;
    }

public:
    /** @brief see #objectInfo */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return object info */
    inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief for impl to achieve custom find logic */
    zffinal zfclassNotPOD Impl {
    public:
        zfstring fileName; /**< @brief file path */
        zfbool fileIsDir; /**< @brief whether directory */
        void *nativeFd; /**< @brief for impl to store native find data */
    public:
        /** @cond ZFPrivateDoc */
        Impl(void)
        : fileName()
        , fileIsDir(zffalse)
        , nativeFd(zfnull)
        {
        }
        /** @endcond */
    };
public:
    /** @brief the impl */
    Impl &impl(void) const;
    /** @brief name for the impl */
    const zfchar *implName(void) const;
    /** @brief user data passed from #implAttach */
    void *implUserData(void) const;
    /** @brief begin first find */
    void implAttach(
            ZF_IN const zfchar *implName
            , ZF_IN_OPT void *implUserData = zfnull
            );
    /** @brief close find */
    void implDetach(void);
    /** @brief check whether impl matches, assert fail if not match, return the #implUserData */
    void *implCheck(ZF_IN const zfchar *implName) const;

    /** @brief util for impl to store extra data */
    void implTag(
            ZF_IN const zfchar *key
            , ZF_IN ZFObject *value
            ) const;
    /** @brief util for impl to store extra data */
    zfany implTag(ZF_IN const zfchar *key) const;
private:
    _ZFP_ZFFileFindDataPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFFileFindData, ZFFileFindData)
ZFOUTPUT_TYPE(ZFFileFindData, {v.objectInfoT(s);})

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_pathInfo "pathInfo"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_flags "flags"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_localPath "localPath"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_fwd_h_


/**
 * @file ZFFile_fwd.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_fwd_h_
#define _ZFI_ZFFile_fwd_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileOpenOption
/**
 * @brief open falgs, similar to fopen's flags,
 *   while we have binary mode only
 *
 * usage:
 * -  v_ZFFileOpenOption::e_Create\n
 *   create a new file for write only
 * -  v_ZFFileOpenOption::e_Create | v_ZFFileOpenOption::e_Read\n
 *   create a new file for read/write
 * -  v_ZFFileOpenOption::e_Read\n
 *   open a existing file for read only, fail if file not exist
 * -  v_ZFFileOpenOption::e_Read | v_ZFFileOpenOption::e_Write\n
 *   open a existing file for read/write, fail if file not exist
 * -  v_ZFFileOpenOption::e_Write\n
 *   same as v_ZFFileOpenOption::e_Create, create a new file for write only
 * -  v_ZFFileOpenOption::e_Append\n
 *   open a file for append or create new if not exist, for write only
 * -  v_ZFFileOpenOption::e_Append | v_ZFFileOpenOption::e_Read\n
 *   open a file for append or create new if not exist, for read/write
 */
ZFENUM_BEGIN(ZFLIB_ZFCore, ZFFileOpenOption)
    ZFENUM_VALUE_WITH_INIT(Create, 1 << 0) /**< @brief create for read write */
    ZFENUM_VALUE_WITH_INIT(Read, 1 << 1) /**< @brief readonly */
    ZFENUM_VALUE_WITH_INIT(Write, 1 << 2) /**< @brief write only */
    ZFENUM_VALUE_WITH_INIT(Append, 1 << 3) /**< @brief append only */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Create)
    ZFENUM_VALUE_REGISTER(Read)
    ZFENUM_VALUE_REGISTER(Write)
    ZFENUM_VALUE_REGISTER(Append)
ZFENUM_END_FLAGS(ZFLIB_ZFCore, ZFFileOpenOption, ZFFileOpenOptionFlags)
ZFENUM_REG_FLAGS(ZFLIB_ZFCore, ZFFileOpenOption, ZFFileOpenOptionFlags)

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
    const zfstring &name(void) const {
        return this->impl().name;
    }
    /**
     * @brief return true if is a directory
     */
    zfbool isDir(void) const {
        return this->impl().isDir;
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
        zfstring name; /**< @brief file path */
        zfbool isDir; /**< @brief whether directory */
        void *nativeFd; /**< @brief for impl to store native find data */
    public:
        /** @cond ZFPrivateDoc */
        Impl(void)
        : name()
        , isDir(zffalse)
        , nativeFd(zfnull)
        {
        }
        /** @endcond */
    private:
        Impl(ZF_IN const Impl &);
        Impl &operator = (ZF_IN const Impl &);
    };
public:
    /** @brief the impl */
    Impl &impl(void) const;
    /** @brief copy the impl data */
    void implCopy(ZF_IN const ZFFileFindData &ref) const;
    /** @brief name for the impl */
    const zfstring &implName(void) const;
    /** @brief user data passed from #implAttach */
    void *implUserData(void) const;
    /** @brief begin first find */
    void implAttach(
            ZF_IN const zfstring &implName
            , ZF_IN_OPT void *implUserData = zfnull
            );
    /** @brief close find */
    void implDetach(void);
    /** @brief check whether impl matches, assert fail if not match, return the #implUserData */
    void *implCheck(ZF_IN const zfstring &implName) const;

    /** @brief util for impl to store extra data */
    void implTag(
            ZF_IN const zfstring &key
            , ZF_IN ZFObject *value
            ) const;
    /** @brief util for impl to store extra data */
    zfany implTag(ZF_IN const zfstring &key) const;
private:
    _ZFP_ZFFileFindDataPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFFileFindData, ZFFileFindData)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFFileFindData, ZFFileFindData)
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


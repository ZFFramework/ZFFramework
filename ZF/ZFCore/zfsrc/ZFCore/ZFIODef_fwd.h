/**
 * @file ZFIODef_fwd.h
 * @brief abstract IO
 */

#ifndef _ZFI_ZFIODef_fwd_h_
#define _ZFI_ZFIODef_fwd_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFIO_pathInfo "pathInfo"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFIO_flags "flags"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFIO_localPath "localPath"

// ============================================================
// ZFIOOpenOption
/**
 * @brief file open option
 *
 * usage:
 * -  v_ZFIOOpenOption::e_Read\n
 *   open for read only, fail if not exist
 * -  v_ZFIOOpenOption::e_Write\n
 *   open for write only, create file if not exist, clear contents if exists
 * -  v_ZFIOOpenOption::e_Read | v_ZFIOOpenOption::e_Write\n
 *   open for read and write, create file if not exist, clear contents if exists
 * -  v_ZFIOOpenOption::e_Modify\n
 *   open for read and write, create file if not exist, keep contents if exists,
 *   position would be set to 0,
 *   seek to end to achieve "append" logic
 *
 *
 * difference to fopen's flag:
 * -  binary mode only
 * -  no `a` (append flag), recommended to use Modify + seek to simulate
 */
ZFENUM_BEGIN(ZFLIB_ZFCore, ZFIOOpenOption)
    ZFENUM_VALUE_WITH_INIT(Read, 1 << 0) /**< @brief read only */
    ZFENUM_VALUE_WITH_INIT(Write, 1 << 1) /**< @brief write only */
    ZFENUM_VALUE_WITH_INIT(Modify, 1 << 2) /**< @brief modify */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Read)
    ZFENUM_VALUE_REGISTER(Write)
    ZFENUM_VALUE_REGISTER(Modify)
ZFENUM_END_FLAGS(ZFLIB_ZFCore, ZFIOOpenOption, ZFIOOpenOptionFlags)
ZFENUM_REG_FLAGS(ZFLIB_ZFCore, ZFIOOpenOption, ZFIOOpenOptionFlags)

// ============================================================
zfclassFwd _ZFP_ZFIOFindDataPrivate;
/**
 * @brief data used by ZFIO when finding files
 *
 * typical usage:
 * @code
 *   ZFIOImpl *io = ZFIOImplForPathType(pathType);
 *   ZFIOFindData fd;
 *   if(io->ioFindFirst(fd, path)) {
 *     do {
 *       // do something
 *     } while(io->ioFindNext(fd));
 *     io->ioFindClose(fd);
 *   }
 * @endcode
 * @return false if error or nothing found
 * @note path must be well formed, use #ZFPathFormat if necessary
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFIOFindData {
public:
    /** @cond ZFPrivateDoc */
    ZFIOFindData(void);
    ZFIOFindData(ZF_IN ZFIOFindData const &ref);
    ZFIOFindData &operator = (ZF_IN ZFIOFindData const &ref);
    zfbool operator == (ZF_IN ZFIOFindData const &ref) const;
    zfbool operator != (ZF_IN ZFIOFindData const &ref) const {
        return !this->operator == (ref);
    }
    /** @endcond */
    ~ZFIOFindData(void);

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
    void implCopy(ZF_IN const ZFIOFindData &ref) const;
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
    _ZFP_ZFIOFindDataPrivate *d;
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFLIB_ZFCore, ZFIOFindData, ZFIOFindData)
ZFTYPEID_ACCESS_ONLY_REG(ZFLIB_ZFCore, ZFIOFindData, ZFIOFindData)
ZFOUTPUT_TYPE(ZFIOFindData, {v.objectInfoT(s);})

// ============================================================
zfclassFwd ZFIOImpl;
/** @brief see #ZFIOImplForPathType */
zfabstract ZFLIB_ZFCore ZFIOToken : zfextend ZFObject {
    ZFOBJECT_DECLARE_ABSTRACT(ZFIOToken, ZFObject)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called before #ioClose
     */
    ZFEVENT(IOCloseOnPrepare)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called after #ioClose
     */
    ZFEVENT(IOCloseOnFinish)

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void) {
        this->ioClose();
        zfsuper::objectOnDeallocPrepare();
    }
protected:
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret += this->pathType();
        ret += ":";
        ret += this->pathData();
    }
public:
    /** @brief path info for this token */
    virtual ZFPathInfo pathInfo(void) {
        return ZFPathInfo(this->pathType(), this->pathData());
    }
    /** @brief path info for this token */
    virtual zfstring pathType(void) zfpurevirtual;
    /** @brief path info for this token */
    virtual zfstring pathData(void) zfpurevirtual;
    /** @brief open flags for this token */
    virtual ZFIOOpenOptionFlags ioFlags(void) zfpurevirtual;

public:
    /**
     * @brief see #ZFIOImplForPathType
     *
     * note:
     * -  when done, ioClose can be called exactly once to release io resource
     * -  ioClose would be called automatically when token deallocated
     * -  you must not access the token after ioClose was called
     *
     * impl must:
     * -  do nothing for invalid token or already closed, and return true
     * -  notify #E_IOCloseOnPrepare and #E_IOCloseOnFinish
     */
    virtual zfbool ioClose(void) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * return size read, or 0 if error occurred, typical usage:
     * @code
     *   zfindex sizeRead = 0;
     *   while((sizeRead = impl->ioRead(token, buf)) > 0) {
     *       // do something with buf
     *   }
     * @endcode
     *
     * note that the tail '\0' won't be written to buf
     * @warning always read in binary mode,
     *   for text files,
     *   you must make sure the file is UTF8 encoded without BOM
     */
    virtual zfindex ioRead(
            ZF_OUT void *buf
            , ZF_IN zfindex maxByteSize
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * return size written, or 0 if error occurred, typical usage:
     * @code
     *   zfindex sizeWritten = impl->ioWrite(token, src, size);
     *   if(sizeWritten != size) { // you can also check by sizeWritten
     *       // write fail
     *   }
     * @endcode
     * \n
     * maxByteSize could be zfindexMax(),
     * which means the write should be stopped when reached 0x00 in src,
     * usually to output a UTF8 string
     */
    virtual zfindex ioWrite(
            ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * seek to specified position, return false if error or not available
     */
    virtual zfbool ioSeek(
            ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * return current position or zfindexMax if error or not available
     */
    virtual zfindex ioTell(void) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * return total size or zfindexMax if error or not available
     */
    virtual zfindex ioSize(void) zfpurevirtual;
};

// ============================================================
/** @brief see #ZFIOImplForPathType */
zfabstract ZFLIB_ZFCore ZFIOImpl : zfextend ZFObject {
    ZFOBJECT_DECLARE_ABSTRACT(ZFIOImpl, ZFObject)
protected:
    zfoverride
    virtual inline void objectInfoImpl(ZF_IN_OUT zfstring &ret) {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        ret += "ZFIOImpl:";
        ret += this->pathType();
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
public:
    /**
     * @brief see #ZFIOImplForPathType
     *
     * return path type for this impl
     */
    virtual zfstring pathType(void) zfpurevirtual;

    /**
     * @brief see #ZFIOImplForPathType
     *
     * whether the specified pathData is exist
     */
    virtual zfbool ioIsExist(ZF_IN const zfstring &pathData) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * whether the specified pathData is exist
     */
    virtual zfbool ioIsDir(ZF_IN const zfstring &pathData) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * get file name from specified pathData
     */
    virtual zfbool ioToFileName(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * convert to child pathData from specified pathData
     */
    virtual zfbool ioToChild(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * convert to child pathData from specified pathData
     */
    virtual zfbool ioToParent(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * create new path specified by pathData
     */
    virtual zfbool ioPathCreate(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * remove item at specified pathData
     */
    virtual zfbool ioRemove(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * convert to child pathData from specified pathData
     */
    virtual zfbool ioMove(
            ZF_IN const zfstring &pathDataTo
            , ZF_IN const zfstring &pathDataFrom
            , ZF_IN_OPT zfbool isForce = zftrue
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * see #ZFIOFindData
     */
    virtual zfbool ioFindFirst(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            ) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * see #ZFIOFindData
     */
    virtual zfbool ioFindNext(ZF_IN_OUT ZFIOFindData &fd) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * see #ZFIOFindData
     */
    virtual void ioFindClose(ZF_IN_OUT ZFIOFindData &fd) zfpurevirtual;
    /**
     * @brief see #ZFIOImplForPathType
     *
     * open with specified pathData
     */
    virtual zfautoT<ZFIOToken> ioOpen(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            ) zfpurevirtual;

    // ============================================================
    // default/dummy impl
public:
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioIsExistDefault(ZF_IN const zfstring &pathData);
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioIsDirDefault(ZF_IN const zfstring &pathData);
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToFileNameDefault(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToChildDefault(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToParentDefault(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioPathCreateDefault(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioRemoveDefault(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioMoveDefault(
            ZF_IN const zfstring &pathDataTo
            , ZF_IN const zfstring &pathDataFrom
            , ZF_IN_OPT zfbool isForce = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioFindFirstDefault(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioFindNextDefault(ZF_IN_OUT ZFIOFindData &fd);
    /** @brief see #ZFIOImplForPathType */
    static void ioFindCloseDefault(ZF_IN_OUT ZFIOFindData &fd);
    /** @brief see #ZFIOImplForPathType */
    static zfautoT<ZFIOToken> ioOpenDefault(
            ZF_IN const zfstring &pathData
            , ZF_IN ZFIOOpenOptionFlags flags
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioCloseDefault(ZF_IN void *token);
    /** @brief see #ZFIOImplForPathType */
    static zfindex ioReadDefault(
            ZF_IN void *token
            , ZF_IN void *buf
            , ZF_IN zfindex maxByteSize
            );
    /** @brief see #ZFIOImplForPathType */
    static zfindex ioWriteDefault(
            ZF_IN void *token
            , ZF_IN const void *src
            , ZF_IN_OPT zfindex maxByteSize = zfindexMax()
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioSeekDefault(
            ZF_IN void *token
            , ZF_IN zfindex byteSize
            , ZF_IN_OPT ZFSeekPos seekPos = ZFSeekPosBegin
            );
    /** @brief see #ZFIOImplForPathType */
    static zfindex ioTellDefault(ZF_IN void *token);
    /** @brief see #ZFIOImplForPathType */
    static zfindex ioSizeDefault(ZF_IN void *token);

    // ============================================================
    // default impl for chained path info
public:
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioIsExistForChained(ZF_IN const zfstring &pathData);
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioIsDirForChained(ZF_IN const zfstring &pathData);
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToFileNameForChained(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToChildForChained(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            , ZF_IN const zfstring &childName
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioToParentForChained(
            ZF_IN_OUT zfstring &ret
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioPathCreateForChained(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool autoCreateParent = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioRemoveForChained(
            ZF_IN const zfstring &pathData
            , ZF_IN_OPT zfbool isRecursive = zftrue
            , ZF_IN_OPT zfbool isForce = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioMoveForChained(
            ZF_IN const zfstring &pathDataTo
            , ZF_IN const zfstring &pathDataFrom
            , ZF_IN_OPT zfbool isForce = zftrue
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioFindFirstForChained(
            ZF_IN_OUT ZFIOFindData &fd
            , ZF_IN const zfstring &pathData
            );
    /** @brief see #ZFIOImplForPathType */
    static zfbool ioFindNextForChained(ZF_IN_OUT ZFIOFindData &fd);
    /** @brief see #ZFIOImplForPathType */
    static void ioFindCloseForChained(ZF_IN_OUT ZFIOFindData &fd);
};

// ============================================================
/**
 * @brief main entry for abstract IO for ZFFramework
 *
 * typical usage:
 * -# use #ZFIOImplForPathType to access io impl
 * -# use #ZFIOImpl::ioOpen to open specified pathData
 * -# use #ZFIOToken::ioRead or #ZFIOToken::ioWrite to process contents
 *
 * ZFIOImpl is a low level abstract IO to process various IO,
 * which including: file, resource, network\n
 * for convenient, it's recommend to use #ZFInput and #ZFOutput
 * for high level IO processing
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfautoT<ZFIOImpl>, ZFIOImplForPathType
        , ZFMP_IN(const zfstring &, pathType)
        )
/** @brief return a list of #ZFIOImplRegister, for debug use only */
ZFMETHOD_FUNC_DECLARE_0(ZFLIB_ZFCore, ZFCoreArray<zfautoT<ZFIOImpl> >, ZFIOImplList)

/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFIOImplRegister
        , ZFMP_IN(const zfstring &, pathType)
        , ZFMP_IN(ZFIOImpl *, ioImpl)
        )
/** @brief see #ZFIOImplForPathType */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, void, ZFIOImplUnregister
        , ZFMP_IN(const zfstring &, pathType)
        )

/** @brief see #ZFIOImplForPathType */
#define ZFIO_DECLARE(ZFLIB_, pathType) \
    ZFMETHOD_FUNC_DECLARE_0(ZFLIB_, const zfstring &, ZFPathType_##pathType)

/** @brief see #ZFIOImplForPathType */
#define ZFIO_DEFINE(pathType, ZFIOImpl_) \
    ZFMETHOD_FUNC_DEFINE_0(const zfstring &, ZFPathType_##pathType) { \
        static zfstring ret = zftext(ZFM_TOSTRING(pathType)); \
        return ret; \
    } \
    ZF_STATIC_REGISTER_INIT(ZFIOReg_##pathType) { \
        ZFIOImplRegister(ZFPathType_##pathType(), zfobj<ZFIOImpl_>()); \
    } \
    ZF_STATIC_REGISTER_DESTROY(ZFIOReg_##pathType) { \
        ZFIOImplUnregister(ZFPathType_##pathType()); \
    } \
    ZF_STATIC_REGISTER_END(ZFIOReg_##pathType)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIODef_fwd_h_


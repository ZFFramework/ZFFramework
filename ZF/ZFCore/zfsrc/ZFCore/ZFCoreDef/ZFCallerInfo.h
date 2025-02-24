/**
 * @file ZFCallerInfo.h
 * @brief caller info utils
 */
#ifndef _ZFI_ZFCallerInfo_h_
#define _ZFI_ZFCallerInfo_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZFLIB_ZFCore void _ZFP_ZF_CALLER_FILE_TO_NAME(
        ZF_IN_OUT zfstring &ret
        , ZF_IN const zfchar *filePath
        );
extern ZFLIB_ZFCore zfstring _ZFP_ZF_CALLER_FILE_TO_NAME(ZF_IN const zfchar *filePath);

/**
 * @brief convert file path to file name
 *
 * used by ZF_CALLER_FILE_NAME, you may use this function directly if you want\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a std::string if necessary
 */
#define ZF_CALLER_FILE_TO_NAME(path) (_ZFP_ZF_CALLER_FILE_TO_NAME(path).cString())
/**
 * @brief see #ZF_CALLER_FILE_TO_NAME
 */
#define ZF_CALLER_FILE_TO_NAME_REF(ret, path) _ZFP_ZF_CALLER_FILE_TO_NAME(ret, path)
/**
 * @brief similar to __FILE__ in C++ world, except that don't include full path
 *
 * note that this macro contains string convertion, take care of performance if necessary\n
 * the return value would be deleted after line end,
 * you should not save the value for future use,
 * store it in a zfstring if necessary
 */
#define ZF_CALLER_FILE_NAME ZF_CALLER_FILE_TO_NAME(__FILE__)

/**
 * @brief same as __FILE__ in C++ world
 *
 * ensured in zfchar format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FILE __FILE__
/**
 * @brief same as __FUNCTION__ in C++ world
 *
 * ensured in zfchar format, you should convert to zfchar type if necessary
 */
#define ZF_CALLER_FUNCTION __FUNCTION__
/**
 * @brief same as __LINE__ in C++ world
 */
#define ZF_CALLER_LINE __LINE__

// ============================================================
// ZFCallerInfo
/**
 * @brief util class to hold caller info
 */
zffinal zfclassLikePOD ZFLIB_ZFCore ZFCallerInfo {
public:
    /**
     * @brief main constructor
     */
    ZFCallerInfo(void);
    /**
     * @brief main constructor
     */
    ZFCallerInfo(
            ZF_IN const zfchar *callerFile
            , ZF_IN const zfchar *callerFunc
            , ZF_IN zfuint callerLine
            );
    /** @cond ZFPrivateDoc */
    ZFCallerInfo(ZF_IN const ZFCallerInfo &ref);
    ~ZFCallerInfo(void);
    ZFCallerInfo &operator = (ZF_IN const ZFCallerInfo &ref);
    zfbool operator == (ZF_IN const ZFCallerInfo &ref) const;
    inline zfbool operator != (ZF_IN const ZFCallerInfo &ref) const {return !this->operator == (ref);}
    /** @endcond */

public:
    /** @brief see #ZF_CALLER_FILE */
    inline const zfchar *callerFile(void) const {return _callerFile;}
    /** @brief see #ZF_CALLER_FUNCTION */
    inline const zfchar *callerFunc(void) const {return _callerFunc;}
    /** @brief see #ZF_CALLER_LINE */
    inline zfuint callerLine(void) const {return _callerLine;}

public:
    /**
     * @brief set the caller info
     */
    void callerInfo(
            ZF_IN const zfchar *callerFile
            , ZF_IN const zfchar *callerFunc
            , ZF_IN zfuint callerLine
            );
    /** @brief return caller info looks like "[File function (line)]" */
    zfbool callerInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief return caller info looks like "[File function (line)]" */
    inline zfstring callerInfo(void) const {
        zfstring ret;
        this->callerInfoT(ret);
        return ret;
    }
public:
    /** @brief see #objectInfo */
    inline void objectInfoT(ZF_IN_OUT zfstring &ret) const {
        this->callerInfoT(ret);
    }
    /** @brief return object info */
    inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

private:
    zfchar *_callerFileH;
    zfchar *_callerFuncH;
    const zfchar *_callerFile;
    const zfchar *_callerFunc;
    zfuint _callerLine;
public:
    ZFCallerInfo &_ZFP_callerInfo(
            ZF_IN const zfchar *callerFile
            , ZF_IN const zfchar *callerFunc
            , ZF_IN zfuint callerLine
            ) {
        this->_callerFile = callerFile;
        this->_callerFunc = callerFunc;
        this->_callerLine = callerLine;
        return *this;
    }
};
ZFOUTPUT_TYPE(ZFCallerInfo, v.objectInfoT(s);)

/**
 * @brief util macro to make #ZFCallerInfo
 */
#define ZFCallerInfoCreate() \
    ZFCallerInfo()._ZFP_callerInfo(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE)

extern ZFLIB_ZFCore const ZFCallerInfo &_ZFP_ZFCallerInfoEmpty(void);
/**
 * @brief empty caller info
 */
#define ZFCallerInfoEmpty() _ZFP_ZFCallerInfoEmpty()

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallerInfo_h_


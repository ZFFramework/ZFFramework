/**
 * @file ZFToken.h
 * @brief string token util
 */

#ifndef _ZFI_ZFToken_h_
#define _ZFI_ZFToken_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZFLIB_ZFCore ZFTokenForContainer {
public:
    zfstring tokenLeft; /**< @brief "[" by default */
    zfstring tokenRight; /**< @brief "]" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForContainer(void)
    : tokenLeft("[")
    , tokenRight("]")
    , tokenSeparator(", ")
    , tokenValueLeft("")
    , tokenValueRight("")
    , tokenEtc("...")
    {
    }
    ZFTokenForContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    zfbool operator == (ZF_IN const ZFTokenForContainer &ref) const {
        return (this->tokenLeft == ref.tokenLeft
                && this->tokenRight == ref.tokenRight
                && this->tokenSeparator == ref.tokenSeparator
                && this->tokenValueLeft == ref.tokenValueLeft
                && this->tokenValueRight == ref.tokenValueRight
                && this->tokenEtc == ref.tokenEtc
            );
    }
    inline zfbool operator != (ZF_IN const ZFTokenForContainer &ref) const {return !(this->operator == (ref));}
    /** @endcond */
};
extern ZFLIB_ZFCore const ZFTokenForContainer _ZFP_ZFTokenForContainerTrim;
extern ZFLIB_ZFCore const ZFTokenForContainer _ZFP_ZFTokenForContainerDetail;
extern ZFLIB_ZFCore const ZFTokenForContainer _ZFP_ZFTokenForContainerPlainList;
extern ZFLIB_ZFCore ZFTokenForContainer _ZFP_ZFTokenForContainerDefault;
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   [e0, e1, e2, ...]
 * @endcode
 */
#define ZFTokenForContainerTrim() _ZFP_ZFTokenForContainerTrim
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   [
 *       e0,
 *       e1,
 *       e2,
 *       ...
 *   ]
 * @endcode
 */
#define ZFTokenForContainerDetail() _ZFP_ZFTokenForContainerDetail
/**
 * @brief see #ZFTokenForContainer
 *
 * output format:
 * @code
 *   e0
 *   e1
 *   e2
 *   ...
 * @endcode
 */
#define ZFTokenForContainerPlainList() _ZFP_ZFTokenForContainerPlainList
/**
 * @brief see #ZFTokenForContainer, modifyable, #ZFTokenForContainerTrim by default
 */
#define ZFTokenForContainerDefault() _ZFP_ZFTokenForContainerDefault

// ============================================================
/**
 * @brief util class to hold string tokens
 */
zfclassLikePOD ZFLIB_ZFCore ZFTokenForKeyValueContainer {
public:
    zfstring tokenLeft; /**< @brief "{" by default */
    zfstring tokenRight; /**< @brief "}" by default */
    zfstring tokenSeparator; /**< @brief ", " by default */
    zfstring tokenPairLeft; /**< @brief "" by default */
    zfstring tokenPairRight; /**< @brief "" by default */
    zfstring tokenPairSeparator; /**< @brief " = " by default */
    zfstring tokenKeyLeft; /**< @brief "" by default */
    zfstring tokenKeyRight; /**< @brief "" by default */
    zfstring tokenValueLeft; /**< @brief "" by default */
    zfstring tokenValueRight; /**< @brief "" by default */
    zfstring tokenEtc; /**< @brief "..." by default */

public:
    /** @cond ZFPrivateDoc */
    ZFTokenForKeyValueContainer(void)
    : tokenLeft("{")
    , tokenRight("}")
    , tokenSeparator(", ")
    , tokenPairLeft("")
    , tokenPairRight("")
    , tokenPairSeparator(" = ")
    , tokenKeyLeft("")
    , tokenKeyRight("")
    , tokenValueLeft("")
    , tokenValueRight("")
    , tokenEtc("...")
    {
    }
    ZFTokenForKeyValueContainer(
          ZF_IN zfstring const &tokenLeft
        , ZF_IN zfstring const &tokenRight
        , ZF_IN zfstring const &tokenSeparator
        , ZF_IN zfstring const &tokenPairLeft
        , ZF_IN zfstring const &tokenPairRight
        , ZF_IN zfstring const &tokenPairSeparator
        , ZF_IN zfstring const &tokenKeyLeft
        , ZF_IN zfstring const &tokenKeyRight
        , ZF_IN zfstring const &tokenValueLeft
        , ZF_IN zfstring const &tokenValueRight
        , ZF_IN zfstring const &tokenEtc
        )
    : tokenLeft(tokenLeft)
    , tokenRight(tokenRight)
    , tokenSeparator(tokenSeparator)
    , tokenPairLeft(tokenPairLeft)
    , tokenPairRight(tokenPairRight)
    , tokenPairSeparator(tokenPairSeparator)
    , tokenKeyLeft(tokenKeyLeft)
    , tokenKeyRight(tokenKeyRight)
    , tokenValueLeft(tokenValueLeft)
    , tokenValueRight(tokenValueRight)
    , tokenEtc(tokenEtc)
    {
    }
    zfbool operator == (ZF_IN const ZFTokenForKeyValueContainer &ref) const {
        return (this->tokenLeft == ref.tokenLeft
                && this->tokenRight == ref.tokenRight
                && this->tokenSeparator == ref.tokenSeparator
                && this->tokenPairLeft == ref.tokenPairLeft
                && this->tokenPairRight == ref.tokenPairRight
                && this->tokenPairSeparator == ref.tokenPairSeparator
                && this->tokenKeyLeft == ref.tokenKeyLeft
                && this->tokenKeyRight == ref.tokenKeyRight
                && this->tokenValueLeft == ref.tokenValueLeft
                && this->tokenValueRight == ref.tokenValueRight
                && this->tokenEtc == ref.tokenEtc
            );
    }
    inline zfbool operator != (ZF_IN const ZFTokenForKeyValueContainer &ref) const {return !(this->operator == (ref));}
    /** @endcond */
};
extern ZFLIB_ZFCore const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerTrim;
extern ZFLIB_ZFCore const ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDetail;
extern ZFLIB_ZFCore ZFTokenForKeyValueContainer _ZFP_ZFTokenForKeyValueContainerDefault;
/**
 * @brief see #ZFTokenForKeyValueContainer
 *
 * output format:
 * @code
 *   {k0=v0, k1=v1, ...}
 * @endcode
 */
#define ZFTokenForKeyValueContainerTrim() _ZFP_ZFTokenForKeyValueContainerTrim
/**
 * @brief see #ZFTokenForKeyValueContainer
 *
 * output format:
 * @code
 *   {
 *       k0 = v0,
 *       k1 = v1,
 *       ...
 *   }
 * @endcode
 */
#define ZFTokenForKeyValueContainerDetail() _ZFP_ZFTokenForKeyValueContainerDetail
/**
 * @brief see #ZFTokenForKeyValueContainer, modifyable, #ZFTokenForKeyValueContainerTrim by default
 */
#define ZFTokenForKeyValueContainerDefault() _ZFP_ZFTokenForKeyValueContainerDefault

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFToken_h_


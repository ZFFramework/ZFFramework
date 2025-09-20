/**
 * @file ZFStyleLoad.h
 * @brief style load utility
 */

#ifndef _ZFI_ZFStyleLoad_h_
#define _ZFI_ZFStyleLoad_h_

#include "ZFObjectIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFStyleListPrivate;
/**
 * @brief see #ZFStyleLoad
 */
zfclass ZFLIB_ZFCore ZFStyleList : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFStyleList, ZFStyle)

public:
    /** @brief style count */
    ZFMETHOD_DECLARE_0(zfindex, itemCount)
    /** @brief style key */
    ZFMETHOD_DECLARE_1(const zfstring &, itemKeyAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief style value */
    ZFMETHOD_DECLARE_1(ZFStyleable *, itemValueAt
            , ZFMP_IN(zfindex, index)
            )
    /** @brief style value */
    ZFMETHOD_DECLARE_2(void, itemForKey
            , ZFMP_IN(const zfstring &, key)
            , ZFMP_IN(ZFStyleable *, value)
            )

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );

private:
    _ZFP_ZFStyleListPrivate *d;
};

extern ZFLIB_ZFCore void _ZFP_ZFStyleLoadErrorCallbackForConsole(ZF_IN const ZFArgs &zfargs);
/** @brief error callback for #ZFStyleLoad */
#define ZFStyleLoadErrorCallbackForConsole() ZFCallbackForFunc(_ZFP_ZFStyleLoadErrorCallbackForConsole)
/** @brief default error callback for #ZFStyleLoad */
ZFEXPORT_VAR_DECLARE(ZFLIB_ZFCore, ZFListener, ZFStyleLoadErrorCallbackDefault)
/** @brief util for impl to output error hint */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, void, ZFStyleLoadErrorHint
        , ZFMP_IN_OUT(const ZFOutput &, output)
        , ZFMP_IN(const ZFArgs &, zfargs)
        )

// ============================================================
/**
 * @brief check whether the item should be load as style
 *
 * by default, these files or dirs would be ignored:
 * -  files or dirs whose name start with '.' or '_' (e.g. '.ignored.xml')
 *   -  `.ignored.xml`
 *   -  `_ignored.xml`
 * -  files whose name end with '_'
 *   -  `ignored_.xml`
 *   -  `ignored_`
 */
ZFMETHOD_FUNC_DECLARE_1(ZFLIB_ZFCore, zfbool, ZFStyleLoadCheck
        , ZFMP_IN(const zfstring &, fileName)
        )

/**
 * @brief util to load multiple styles from directory
 *
 * pathInfo must points to a file or directory that contains styles,
 * example:
 * @code
 *   ~/
 *       MyStyle/
 *           button.xml
 *           label.json
 *           image.png
 *           image.xml
 * @endcode
 * \n
 * all styles are loaded by #ZFObjectIOLoad,
 * and #ZFPathOfWithoutAllExt would be used to transform relative path to styleKey for #ZFStyleSet,
 * for example: `~/path1/path2/mybutton.123.xml` would result `path1/path2/mybutton` as styleKey,
 * you may also use #ZFStyleLoadFile for loading single one file in deep file tree\n
 * \n
 * if two item with same name exists,
 * the later one would override the first one
 * (but which one is later, is not ensured, try to prevent that)\n
 * \n
 * by default, files or dirs would be filtered by #ZFStyleLoadCheck
 * \n
 *
 * specially, if the loaded object is type of #ZFStyleList,
 * all of its contents would be appended to current style,
 * instead of setting #ZFStyleList itself as a style value\n
 * \n
 * the errorCallback would be called when anything load failed,
 * param0 would be the error location
 * (ZFPathInfo or ZFSerializableData or zfstring, depends on load method),
 * param1 would be a zfstring contains error hint
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFStyleLoad
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
        )

/**
 * @brief explicitly load specified file, see #ZFStyleLoad
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFStyleLoadFile
        , ZFMP_IN(const ZFPathInfo &, pathInfo)
        , ZFMP_IN(const zfstring &, childPath)
        )

/**
 * @brief load multiple style from serializableData
 *
 * data should contains:
 * @code
 *   <ZFStyleList>
 *       <Style1 prop="styleKey1" ... />
 *       <Style2 prop="styleKey2" ... />
 *   </ZFStyleList>
 * @endcode
 *
 * the #ZFStyleList is a dummy holder to make the serializable data itself serializable
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFStyleLoad
        , ZFMP_IN(const ZFSerializableData &, serializableData)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
        )

/**
 * @brief see #ZFStyleLoad
 */
ZFMETHOD_FUNC_DECLARE_2(ZFLIB_ZFCore, zfbool, ZFStyleLoad
        , ZFMP_IN(ZFStyleList *, styleList)
        , ZFMP_IN_OPT(const ZFListener &, errorCallback, ZFStyleLoadErrorCallbackDefault())
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleLoad_h_


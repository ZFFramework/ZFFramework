/**
 * @file ZFCoreUtilAutoPtr.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilAutoPtr_h_
#define _ZFI_ZFCoreUtilAutoPtr_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zflineFree
template<typename T_POD>
zfclassLikePOD _ZFP_zflineFreeContainer {
public:
    explicit _ZFP_zflineFreeContainer(ZF_IN T_POD *p) : p(p) {}
    ~_ZFP_zflineFreeContainer(void) {
        zffree(p);
    }
public:
    T_POD *p;
};
template<typename T_POD>
_ZFP_zflineFreeContainer<T_POD> _ZFP_zflineFreeWrapper(ZF_IN T_POD *p) {
    return _ZFP_zflineFreeContainer<T_POD>(p);
}
/**
 * @brief util macro to free a POD after line end
 */
#define zflineFree(obj) (_ZFP_zflineFreeWrapper(obj).p)

// ============================================================
// zfblockedFree
zfclassLikePOD ZFLIB_ZFCore _ZFP_zfblockedFreeContainer {
public:
    _ZFP_zfblockedFreeContainer(ZF_IN void *p) : p(p) {}
    ~_ZFP_zfblockedFreeContainer(void) {
        zffree(this->p);
    }
private:
    void *p;
};
/**
 * @brief util macro to make a POD object automatically deleted
 *   after code block, using #zffree
 *
 * usage:
 * @code
 *   { // code block
 *       zfbyte *buf = (zfbyte *)zfmalloc(size);
 *       zfblockedFree(buf); // must be placed in single line
 *   } // buf would be released by #zffree after code block
 * @endcode
 */
#define zfblockedFree(obj) \
    _ZFP_zfblockedFreeContainer ZFUniqueName(zfblockedFree_)(obj)

// ============================================================
// zflineDelete
template<typename T_Object>
zfclassLikePOD _ZFP_zflineDeleteContainer {
public:
    _ZFP_zflineDeleteContainer(ZF_IN const _ZFP_zflineDeleteContainer &ref) : p(ref.p) {}
    _ZFP_zflineDeleteContainer(ZF_IN T_Object *p) : p(p) {}
    ~_ZFP_zflineDeleteContainer(void) {
        zfdelete(p);
    }
public:
    T_Object *p;
};
template<typename T_Object>
_ZFP_zflineDeleteContainer<T_Object> _ZFP_zflineDeleteWrapper(ZF_IN T_Object *p) {
    return _ZFP_zflineDeleteContainer<T_Object>(p);
}
/**
 * @brief util macro to delete a object after line end
 */
#define zflineDelete(obj) (_ZFP_zflineDeleteWrapper(obj).p)

// ============================================================
// zfblockedDelete
typedef void (*_ZFP_zfblockedDeleteCallback)(ZF_IN void *p);
template<typename T_Object>
void _ZFP_zfblockedDeleteOnDelete(ZF_IN void *p) {
    zfdelete((T_Object *)p);
}
zfclassLikePOD ZFLIB_ZFCore _ZFP_zfblockedDeleteContainer {
public:
    _ZFP_zfblockedDeleteContainer(void) : p(zfnull), deleteCallback(zfnull) {}
    ~_ZFP_zfblockedDeleteContainer(void) {
        if(this->p != zfnull) {
            this->deleteCallback(this->p);
        }
    }
public:
    void *p;
    _ZFP_zfblockedDeleteCallback deleteCallback;
};
template<typename T_Object>
void _ZFP_zfblockedDeleteSetup(
        ZF_IN _ZFP_zfblockedDeleteContainer *container
        , ZF_IN T_Object * const &p
        ) {
    container->p = (void *)p;
    container->deleteCallback = _ZFP_zfblockedDeleteOnDelete<T_Object>;
}
/**
 * @brief util macro to make a non-ZFObject object automatically deleted
 *   after code block, using #zfdelete
 *
 * usage:
 * @code
 *   { // code block
 *       NonePODObject *obj = zfnew(NonePODObject);
 *       zfblockedDelete(obj); // must be placed in single line
 *   } // buf would be released by #zfdelete after code block
 * @endcode
 */
#define zfblockedDelete(obj) \
    _ZFP_zfblockedDeleteContainer ZFUniqueName(zfblockedDelete_); \
    _ZFP_zfblockedDeleteSetup(&ZFUniqueName(zfblockedDelete_), obj)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilAutoPtr_h_


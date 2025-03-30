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

// ============================================================
// zfblockedCleanup
/**
 * @brief util to perform custom cleanup action after code block
 */
#define zfblockedCleanup_1(action \
        , P0, p0 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0) \
        : p0(p0) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
    } ZFUniqueName(zfblockedCleanup_)(p0)
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_2(action \
        , P0, p0 \
        , P1, p1 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                ) \
        : p0(p0) \
        , p1(p1) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_3(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_4(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_5(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                , P4 &p4 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        , p4(p4) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_6(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                , P4 &p4 \
                , P5 &p5 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        , p4(p4) \
        , p5(p5) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
        P5 &p5; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_7(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                , P4 &p4 \
                , P5 &p5 \
                , P6 &p6 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        , p4(p4) \
        , p5(p5) \
        , p6(p6) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
        P5 &p5; \
        P6 &p6; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            , p6 \
            )
/** @brief see #zfblockedCleanup_1 */
#define zfblockedCleanup_8(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        , P7, p7 \
        ) \
    zfclassLikePOD ZFUniqueName(zfblockedCleanup_T) { \
    public: \
        ZFUniqueName(zfblockedCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                , P4 &p4 \
                , P5 &p5 \
                , P6 &p6 \
                , P7 &p7 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        , p4(p4) \
        , p5(p5) \
        , p6(p6) \
        , p7(p7) \
        {} \
        ~ZFUniqueName(zfblockedCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
        P5 &p5; \
        P6 &p6; \
        P7 &p7; \
    } ZFUniqueName(zfblockedCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            , p6 \
            , p7 \
            )

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreUtilAutoPtr_h_


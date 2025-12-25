/**
 * @file ZFCoreUtilAutoPtr.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilAutoPtr_h_
#define _ZFI_ZFCoreUtilAutoPtr_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfscopeFree
zfclassLikePOD ZFLIB_ZFCore _ZFP_zfscopeFreeContainer {
public:
    _ZFP_zfscopeFreeContainer(ZF_IN void *p) : p(p) {}
    ~_ZFP_zfscopeFreeContainer(void) {
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
 *       zfscopeFree(buf); // must be placed in single line
 *   } // buf would be released by #zffree after code block
 * @endcode
 */
#define zfscopeFree(obj) \
    _ZFP_zfscopeFreeContainer ZFUniqueName(zfscopeFree_)(obj)

// ============================================================
// zfscopeCleanup
/**
 * @brief util to perform custom cleanup action after code block
 */
#define zfscopeCleanup_1(action \
        , P0, p0 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0) \
        : p0(p0) \
        {} \
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
    } ZFUniqueName(zfscopeCleanup_)(p0)
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_2(action \
        , P0, p0 \
        , P1, p1 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
                , P1 &p1 \
                ) \
        : p0(p0) \
        , p1(p1) \
        {} \
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_3(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        {} \
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            , p2 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_4(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        {} \
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_5(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
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
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_6(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
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
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
        P5 &p5; \
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_7(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
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
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
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
    } ZFUniqueName(zfscopeCleanup_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            , p6 \
            )
/** @brief see #zfscopeCleanup_1 */
#define zfscopeCleanup_8(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        , P7, p7 \
        ) \
    zfclassLikePOD ZFUniqueName(zfscopeCleanup_T) { \
    public: \
        ZFUniqueName(zfscopeCleanup_T)(P0 &p0 \
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
        ~ZFUniqueName(zfscopeCleanup_T)(void) { \
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
    } ZFUniqueName(zfscopeCleanup_)(p0 \
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


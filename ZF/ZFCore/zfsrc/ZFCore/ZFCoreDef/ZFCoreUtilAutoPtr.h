/**
 * @file ZFCoreUtilAutoPtr.h
 * @brief utilities for ZFFramework
 */

#ifndef _ZFI_ZFCoreUtilAutoPtr_h_
#define _ZFI_ZFCoreUtilAutoPtr_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zffreeInScope
zfclassLikePOD ZFLIB_ZFCore _ZFP_zffreeInScopeContainer {
public:
    _ZFP_zffreeInScopeContainer(ZF_IN void *p) : p(p) {}
    ~_ZFP_zffreeInScopeContainer(void) {
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
 *       zffreeInScope(buf); // must be placed in single line
 *   } // buf would be released by #zffree after code block
 * @endcode
 */
#define zffreeInScope(obj) \
    _ZFP_zffreeInScopeContainer ZFUniqueName(zffreeInScope_)(obj)

// ============================================================
// zfcleanupInScope
/**
 * @brief util to perform custom cleanup action after code block
 */
#define zfcleanupInScope_1(action \
        , P0, p0 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0) \
        : p0(p0) \
        {} \
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
    } ZFUniqueName(zfcleanupInScope_)(p0)
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_2(action \
        , P0, p0 \
        , P1, p1 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
                , P1 &p1 \
                ) \
        : p0(p0) \
        , p1(p1) \
        {} \
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_3(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        {} \
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            , p2 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_4(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
                , P1 &p1 \
                , P2 &p2 \
                , P3 &p3 \
                ) \
        : p0(p0) \
        , p1(p1) \
        , p2(p2) \
        , p3(p3) \
        {} \
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_5(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
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
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_6(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
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
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
            action \
        } \
    private: \
        P0 &p0; \
        P1 &p1; \
        P2 &p2; \
        P3 &p3; \
        P4 &p4; \
        P5 &p5; \
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_7(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
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
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
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
    } ZFUniqueName(zfcleanupInScope_)(p0 \
            , p1 \
            , p2 \
            , p3 \
            , p4 \
            , p5 \
            , p6 \
            )
/** @brief see #zfcleanupInScope_1 */
#define zfcleanupInScope_8(action \
        , P0, p0 \
        , P1, p1 \
        , P2, p2 \
        , P3, p3 \
        , P4, p4 \
        , P5, p5 \
        , P6, p6 \
        , P7, p7 \
        ) \
    zfclassLikePOD ZFUniqueName(zfcleanupInScope_T) { \
    public: \
        ZFUniqueName(zfcleanupInScope_T)(P0 &p0 \
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
        ~ZFUniqueName(zfcleanupInScope_T)(void) { \
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
    } ZFUniqueName(zfcleanupInScope_)(p0 \
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


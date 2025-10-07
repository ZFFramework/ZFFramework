/**
 * @file ZFCoreTypeDef_ClassType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_ClassType_h_
#define _ZFI_ZFCoreTypeDef_ClassType_h_

#include "ZFCoreTypeDef_CoreType.h"
#include <cstdlib>
#include <memory>
#include <cstring>

// for internal debug use only
// #define _ZFP_ZFMEM_LOG_DISABLE_MEMPOOL 1
// #define _ZFP_ZFMEM_LOG 1
// #define _ZFP_ZFMEM_LOG_VERBOSE 1
// #define _ZFP_ZFMEM_LOG_LARGE_OBJECT 1

#if _ZFP_ZFMEM_LOG_LARGE_OBJECT
    inline void _ZFP_ZFMEM_LOG_LARGE_OBJECT_action(zfindex size) {
    }
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief forward declaration of a class type
 */
#define zfclassFwd class
/**
 * @brief shows the class is a POD type
 */
#define zfclassPOD class
/**
 * @brief shows the class is not a POD type,
 *   but you may use it like a POD
 *   except memset it to 0
 */
#define zfclassLikePOD class
/**
 * @brief shows the class is not a POD type,
 *   you should not memset it
 *   or declare it in stack
 *   or copy value by copy constructor or operator =
 */
#define zfclassNotPOD class

/**
 * @brief dummy macro shows class inherit from another
 */
#define zfextend public

/**
 * @brief dummy macro shows that method override parent's method
 */
#define zfoverride

/**
 * @brief dummy macro shows that a method or class is designed must not to be overrided
 */
#define zffinal

/**
 * @brief dummy macro shows that a method is pure virtual method
 */
#define zfpurevirtual = 0

// ============================================================
/**
 * @brief a macro to show that a class doesn't allow copy constructor
 *
 * usage:
 * @code
 *   class YourClass {
 *       ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(YourClass)
 *   };
 * @endcode
 */
#define ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ClassName) \
    private: \
        ClassName(const ClassName &); \
        ClassName &operator = (const ClassName &); \
    public:

// ============================================================
extern ZFLIB_ZFCore void *_ZFP_zfmalloc(zfindex size);
/** @brief same as malloc defined for future use */
#define zfmalloc(size) _ZFP_zfmalloc(size)
extern ZFLIB_ZFCore void *_ZFP_zfmallocZero(zfindex size);
/** @brief util method to #zfmalloc and #zfmemset to zero */
#define zfmallocZero(size) _ZFP_zfmallocZero(size)

extern ZFLIB_ZFCore void *_ZFP_zfrealloc(void *oldPtr, zfindex newSize);
/** @brief same as realloc defined for future use */
#define zfrealloc(oldPtr, newSize) _ZFP_zfrealloc((oldPtr), (newSize))
extern ZFLIB_ZFCore void *_ZFP_zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize);
/** @brief util method to #zfrealloc and #zfmemset the increased memory to zero */
#define zfreallocZero(oldPtr, newSize, oldSize) _ZFP_zfreallocZero((oldPtr), (newSize), (oldSize))

extern ZFLIB_ZFCore void _ZFP_zffree(void *ptr);
/** @brief same as free defined for future use, do nothing if ptr is NULL */
#define zffree(ptr) _ZFP_zffree(ptr)

/** @brief same as new defined for future use */
#define zfnew(Type, ...) (new (_ZFP_zfmalloc(sizeof(Type))) Type(__VA_ARGS__))
template<typename Type> inline void _ZFP_zfdelete(Type *obj) {obj->~Type(); _ZFP_zffree((void *)obj);}
/** @brief same as delete defined for future use */
#define zfdelete(instance) _ZFP_zfdelete(instance)

/** @brief placement new defined for future use */
#define zfnewPlacement(buf, Type, ...) (new (buf) Type(__VA_ARGS__))
template<typename Type> inline void _ZFP_zfdeletePlacement(Type *obj) {obj->~Type();}
/** @brief placement delete (instance->~Type()) defined for future use */
#define zfdeletePlacement(instance) _ZFP_zfdeletePlacement(instance)

#if _ZFP_ZFMEM_LOG_LARGE_OBJECT
    inline void *_ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(void *p, zfindex size) {
        _ZFP_ZFMEM_LOG_LARGE_OBJECT_action(size);
        return p;
    }
    template<typename T>
    T *_ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfnew(T *p) {
        _ZFP_ZFMEM_LOG_LARGE_OBJECT_action(sizeof(T));
        return p;
    }
    #undef zfmalloc
    #define zfmalloc(size) _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(_ZFP_zfmalloc(size), (size))
    #undef zfmallocZero
    #define zfmallocZero(size) _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(_ZFP_zfmallocZero(size), (size))
    #undef zfrealloc
    #define zfrealloc(oldPtr, newSize) _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(_ZFP_zfrealloc((oldPtr), (newSize)), (newSize))
    #undef zfreallocZero
    #define zfreallocZero(oldPtr, newSize, oldSize) _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(_ZFP_zfreallocZero((oldPtr), (newSize), (oldSize)), (newSize))
    #undef zfnew
    #define zfnew(Type, ...) _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfnew(new (_ZFP_zfmalloc(sizeof(Type))) Type(__VA_ARGS__))
#endif

// ============================================================
/** @brief wrapper to memset */
inline void *zfmemset(void *p, zfint ch, zfindex size) {return memset(p, ch, (size_t)size);}
/** @brief wrapper to memcpy */
inline void *zfmemcpy(void *dst, const void *src, zfindex size) {return memcpy(dst, src, (size_t)size);}
/** @brief wrapper to memmove */
inline void *zfmemmove(void *dst, const void *src, zfindex size) {return memmove(dst, src, (size_t)size);}
/** @brief wrapper to memcmp */
inline zfint zfmemcmp(const void *p1, const void *p2, zfindex size) {return (zfint)memcmp(p1, p2, (size_t)size);}

/**
 * @brief memmove for common object type, object must support operator =
 *
 * it's all right if dst and src share same memory
 */
template<typename T_Element>
T_Element *zfmemmoveObject(T_Element *dst, const T_Element *src, zfindex count) {
    if(dst && src && count != 0 && count != zfindexMax()) {
        if(dst > src && src + count > dst) {
            const T_Element *srcEnd = src;
            src += count;
            dst += count;
            while(src > srcEnd) {
                --dst;
                --src;
                *dst = *src;
            }
        }
        else {
            const T_Element *srcEnd = src + count;
            while(src < srcEnd) {
                *dst = *src;
                ++dst;
                ++src;
            }
        }
    }
    return dst;
}

// ============================================================
/**
 * @brief dummy macro that shows the param used as required input
 */
#define ZF_IN
/**
 * @brief dummy macro that shows the param used as optional input
 */
#define ZF_IN_OPT
/**
 * @brief dummy macro that shows the param used as required output
 */
#define ZF_OUT
/**
 * @brief dummy macro that shows the param used as optional output
 */
#define ZF_OUT_OPT
/**
 * @brief dummy macro that shows the param used as required input and output
 */
#define ZF_IN_OUT
/**
 * @brief dummy macro that shows the param used as optional input and output
 */
#define ZF_IN_OUT_OPT

// ============================================================
#if _ZFP_ZFMEM_LOG
    extern ZFLIB_ZFCore void _ZFP_ZFMEM_printStatus(int threshold = 10);

    inline void *_ZFP_ZFMEM_zfmalloc(zfindex size, zfbool mallocZero, const char *file, const char *func, int line) {
        void *ret = _ZFP_zfmalloc(size);
        if(mallocZero && ret) {
            zfmemset(ret, 0, size);
        }

        #if _ZFP_ZFMEM_LOG_LARGE_OBJECT
            _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(ret, size);
        #endif
        if(mallocZero) {
            _ZFP_ZFMEM_logNew(ret, "zfmallocZero     ", (size_t)size, file, func, line);
        }
        else {
            _ZFP_ZFMEM_logNew(ret, "zfmalloc         ", (size_t)size, file, func, line);
        }
        return ret;
    }
    #undef zfmalloc
    #define zfmalloc(size) _ZFP_ZFMEM_zfmalloc((size), zffalse, __FILE__, __FUNCTION__, __LINE__)
    #undef zfmallocZero
    #define zfmallocZero(size) _ZFP_ZFMEM_zfmalloc((size), zftrue, __FILE__, __FUNCTION__, __LINE__)

    inline void *_ZFP_ZFMEM_zfrealloc(void *oldPtr, zfindex newSize, const char *file, const char *func, int line) {
        _ZFP_ZFMEM_logDelete(oldPtr, "zfreallocD       ", file, func, line);
        void *ret = _ZFP_zfrealloc(oldPtr, newSize);
        #if _ZFP_ZFMEM_LOG_LARGE_OBJECT
            _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(ret, newSize);
        #endif
        _ZFP_ZFMEM_logNew(ret, "zfrealloc        ", (size_t)newSize, file, func, line);
        return ret;
    }
    #undef zfrealloc
    #define zfrealloc(oldPtr, newSize) _ZFP_ZFMEM_zfrealloc((oldPtr), (newSize), __FILE__, __FUNCTION__, __LINE__)
    inline void *_ZFP_ZFMEM_zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize, const char *file, const char *func, int line) {
        _ZFP_ZFMEM_logDelete(oldPtr, "zfreallocZeroD   ", file, func, line);
        void *ret = _ZFP_zfreallocZero(oldPtr, newSize, oldSize);
        #if _ZFP_ZFMEM_LOG_LARGE_OBJECT
            _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc(ret, newSize);
        #endif
        _ZFP_ZFMEM_logNew(ret, "zfreallocZero    ", (size_t)newSize, file, func, line);
        return ret;
    }
    #undef zfreallocZero
    #define zfreallocZero(oldPtr, newSize, oldSize) _ZFP_ZFMEM_zfreallocZero((oldPtr), (newSize), (oldSize), __FILE__, __FUNCTION__, __LINE__)

    #undef zffree
    #define zffree(ptr) _ZFP_zffree(_ZFP_ZFMEM_delete((ptr), "zffree           "))

    extern ZFLIB_ZFCore void _ZFP_ZFMEM_logNew(void *p, const char *action, size_t size, const char *file, const char *func, int line);
    extern ZFLIB_ZFCore void _ZFP_ZFMEM_logDelete(void *p, const char *action, const char *file, const char *func, int line);
    template<typename T>
    T *_ZFP_ZFMEM_new_action(T *p, const char *action, const char *file, const char *func, int line) {
        #if _ZFP_ZFMEM_LOG_LARGE_OBJECT
            _ZFP_ZFMEM_LOG_LARGE_OBJECT_action_zfmalloc((void *)p, sizeof(T));
        #endif
        _ZFP_ZFMEM_logNew((void *)p, action, sizeof(T), file, func, line);
        return p;
    }
    template<typename T>
    T _ZFP_ZFMEM_delete_action(T p, const char *action, const char *file, const char *func, int line) {
        _ZFP_ZFMEM_logDelete((void *)p, action, file, func, line);
        return p;
    }
    #define _ZFP_ZFMEM_new(p, action) _ZFP_ZFMEM_new_action((p), action, __FILE__, __FUNCTION__, __LINE__)
    #define _ZFP_ZFMEM_delete(p, action) _ZFP_ZFMEM_delete_action((p), action, __FILE__, __FUNCTION__, __LINE__)

    #undef zfnew
    #define zfnew(Type, ...) _ZFP_ZFMEM_new((new (_ZFP_zfmalloc(sizeof(Type))) Type(__VA_ARGS__)), "zfnew            ")
    #undef zfdelete
    #define zfdelete(instance) _ZFP_zfdelete(_ZFP_ZFMEM_delete((instance), "zfdelete         "))

    #undef zfnewPlacement
    #define zfnewPlacement(buf, Type, ...) _ZFP_ZFMEM_new((new (buf) Type(__VA_ARGS__)), "zfnewPlacement   ")
    #undef zfdeletePlacement
    #define zfdeletePlacement(instance) _ZFP_zfdeletePlacement(_ZFP_ZFMEM_delete((instance), "zfdeletePlacement"))
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_ClassType_h_


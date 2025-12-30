/**
 * @file zfstring.h
 * @brief low level string container
 */

#ifndef _ZFI_zfstring_h_
#define _ZFI_zfstring_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"
#include "ZFCoreMutex.h"
#include "ZFMemPool.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
template<typename T_Char>
zfindex _ZFP_zfstring_len(ZF_IN const T_Char *s) {
    const T_Char *p = s;
    while(*p) {++p;}
    return p - s;
}
template<typename T_Char>
zfint _ZFP_zfstring_cmp(
        ZF_IN const T_Char *s1
        , ZF_IN const T_Char *s2
        ) {
    while(*s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
    return *s1 - *s2;
}
template<typename T_Char>
zfint _ZFP_zfstring_ncmp(
        ZF_IN const T_Char *s1
        , ZF_IN const T_Char *s2
        , ZF_IN zfindex len
        ) {
    while(--len && *s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
    return *s1 - *s2;
}

// ============================================================
inline zfindex _ZFP_zfstring_len(ZF_IN const zfchar *s) {
    return zfslen(s);
}
inline zfint _ZFP_zfstring_cmp(
        ZF_IN const zfchar *s1
        , ZF_IN const zfchar *s2
        ) {
    return zfscmp(s1, s2);
}
inline zfint _ZFP_zfstring_ncmp(
        ZF_IN const zfchar *s1
        , ZF_IN const zfchar *s2
        , ZF_IN zfindex len
        ) {
    return zfsncmp(s1, s2, len);
}

template<typename T_Char>
zfclassNotPOD _ZFP_zfstringD {
public:
    zfuint refCount;
    zfuint capacity; // capacity including tail '\0'
    zfuint length;
    union {
        T_Char *buf; // capacity != 0
        const T_Char *ptr; // capacity = 0
    } d;
public:
    _ZFP_zfstringD(void) : refCount(1), capacity(0), length(0), d() {
        static T_Char buf[1] = {0};
        d.ptr = buf;
    }
};
template<typename T_Char>
zfclassLikePOD _ZFP_zfstringH {
public:
    _ZFP_zfstringH(void) : d(zfpoolNew(_ZFP_zfstringD<T_Char>)) {}
    ~_ZFP_zfstringH(void) {zfpoolDelete(d);}
public:
    _ZFP_zfstringD<T_Char> *d;
};

// ============================================================
/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD zft_zfstring {
public:
    /** @brief construct an empty string */
    zft_zfstring(void)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s)
    {
        ZFCoreMutexLocker();
        d = s.d;
        ++(d->refCount);
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, ZF_IN zfindex len)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
        this->assign(s.cString(), len);
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, ZF_IN zfindex pos, ZF_IN zfindex len)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
        if(pos < s.length()) {
            if(len > s.length() - pos) {
                len = s.length() - pos;
            }
            this->assign(s.cString() + pos, len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
        if(s) {
            this->assign(s);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
        if(s) {
            this->assign(s, len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s, ZF_IN zfindex pos, ZF_IN zfindex len)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
        if(s) {
            this->assign(s + pos, len);
        }
    }
    /** @brief construct empty string */
    zft_zfstring(ZF_IN const zfnullT &dummy)
    {
        ZFCoreMutexLocker();
        d = _ZFP_Empty();
        ++(d->refCount);
    }
    ~zft_zfstring(void) {
        ZFCoreMutexLocker();
        if(d->refCount == 1) {
            if(d->capacity) {
                zffree(d->d.buf);
            }
            zfpoolDelete(d);
        }
        else {
            --(d->refCount);
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char * (void) const {
        return this->isEmpty() ? zfnull : this->cString();
    }
public:
    inline zft_zfstring<T_Char> &operator = (ZF_IN const zft_zfstring<T_Char> &s) {
        ZFCoreMutexLocker();
        _ZFP_zfstringD<T_Char> *dTmp = d;
        d = s.d;
        ++(d->refCount);
        if(dTmp->refCount == 1) {
            if(dTmp->capacity) {
                zffree(dTmp->d.buf);
            }
            zfpoolDelete(dTmp);
        }
        else {
            --(dTmp->refCount);
        }
        return *this;
    }
    inline zft_zfstring<T_Char> &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    inline zft_zfstring<T_Char> &operator = (ZF_IN zfnullT const &dummy) {return this->assign(dummy);}
    zfbool operator == (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) != 0);}
    zfbool operator == (ZF_IN const T_Char *ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const T_Char *ref) const {return (this->compare(ref) != 0);}
    zfbool operator == (ZF_IN zfnullT const &dummy) const {return this->isEmpty();}
    zfbool operator != (ZF_IN zfnullT const &dummy) const {return !this->isEmpty();}
public:
    /* ZFTAG_TRICKS: tricks to make ZFMap<zfstring, xxx> works */
    inline zfbool operator < (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) < 0;}
    inline zfbool operator <= (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) <= 0;}
    inline zfbool operator > (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) > 0;}
    inline zfbool operator >= (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) >= 0;}
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline zft_zfstring<T_Char> &operator += (ZF_IN T_Char c) {return this->append(c);}
    inline zft_zfstring<T_Char> &operator += (ZF_IN const zft_zfstring<T_Char> &s) {return this->append(s);}
    inline zft_zfstring<T_Char> &operator += (ZF_IN const T_Char *s) {return this->append(s);}

    template<typename T_Int>
    inline const T_Char *operator + (ZF_IN T_Int const &offset) const {
        return this->cString() + offset;
    }
    /** @endcond */

public:
    /**
     * @brief change char at index
     */
    void set(
            ZF_IN zfindex pos
            , ZF_IN T_Char c
            ) {
        _prepareWrite(this->length());
        d->d.buf[pos] = c;
    }
    /**
     * @brief get char at index
     */
    inline T_Char get(ZF_IN zfindex pos) const {
        return d->d.ptr[pos];
    }

    /** @cond ZFPrivateDoc */
    template<typename T_Int>
    inline T_Char operator [] (ZF_IN T_Int pos) const {
        return this->get(pos);
    }
    /** @endcond */

public:
    /**
     * @brief swap internal data without deep copy,
     *   designed for performance
     */
    void swap(ZF_IN_OUT zft_zfstring<T_Char> &ref) {
        ZFCoreMutexLocker();
        _ZFP_zfstringD<T_Char> *dTmp = d;
        d = ref.d;
        ref.d = dTmp;
    }

public:
    /** @brief append string */
    inline zft_zfstring<T_Char> &append(ZF_IN T_Char c) {
        _prepareWrite(d->length + 1);
        d->d.buf[d->length] = c;
        d->d.buf[++(d->length)] = '\0';
        return *this;
    }
    /** @brief append string */
    inline zft_zfstring<T_Char> &append(ZF_IN const zft_zfstring<T_Char> &s) {
        if(s) {
            _appendImpl(s.cString(), s.length());
        }
        return *this;
    }
    /** @brief append string */
    inline zft_zfstring<T_Char> &append(
            ZF_IN const zft_zfstring<T_Char> &s
            , ZF_IN zfindex len
            ) {
        len = (len <= s.length() ? len : s.length());
        if(len > 0) {
            _appendImpl(s.cString(), len);
        }
        return *this;
    }
    /** @brief append string */
    inline zft_zfstring<T_Char> &append(
            ZF_IN const zft_zfstring<T_Char> &s
            , ZF_IN zfindex offset
            , ZF_IN zfindex len
            ) {
        if(offset > s.length()) {
            offset = s.length();
        }
        if(len > s.length() - offset) {
            len = s.length() - offset;
        }
        if(len > 0) {
            _appendImpl(s.cString() + offset, len);
        }
        return *this;
    }
    /** @brief append string */
    zft_zfstring<T_Char> &append(
            ZF_IN const void *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(s) {
            if(len == zfindexMax()) {
                len = _ZFP_zfstring_len((const T_Char *)s);
            }
            if(len > 0) {
                _appendImpl(s, len);
            }
        }
        return *this;
    }
private:
    void _appendImpl(
            ZF_IN const void *s
            , ZF_IN zfindex len
            ) {
        _prepareWrite(d->length + len);
        zfmemcpy(d->d.buf + d->length, s, len * sizeof(T_Char));
        d->d.buf[d->length += (zfuint)len] = '\0';
    }

public:
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(ZF_IN const zft_zfstring<T_Char> &s) {
        this->operator = (s);
        return *this;
    }
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(
            ZF_IN const zft_zfstring<T_Char> &s
            , ZF_IN zfindex len
            ) {
        return this->assign(s.cString(), len <= s.length() ? len : s.length());
    }
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(
            ZF_IN const zft_zfstring<T_Char> &s
            , ZF_IN zfindex offset
            , ZF_IN zfindex len
            ) {
        if(offset > s.length()) {
            offset = s.length();
        }
        if(len > s.length() - offset) {
            len = s.length() - offset;
        }
        return this->assign(s.cString() + offset, len);
    }
    /** @brief replace all content of the string */
    zft_zfstring<T_Char> &assign(
            ZF_IN const void *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(len == zfindexMax()) {
            if(s) {
                len = _ZFP_zfstring_len((const T_Char *)s);
            }
            else {
                len = 0;
            }
        }
        if(len > 0) {
            if(len >= d->capacity || d->refCount > 1
                    || (d->capacity >= 128 && len < d->capacity / 3)
                    ) {
                _capacityChange(len, zffalse);
            }
            d->length = (zfuint)len;
            zfmemcpy(d->d.buf, s, len * sizeof(T_Char));
            d->d.buf[len] = '\0';
        }
        else {
            this->removeAll();
        }
        return *this;
    }
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(ZF_IN const zfnullT &dummy) {
        this->removeAll();
        return *this;
    }

public:
    /** @brief insert string */
    inline zft_zfstring<T_Char> &insert(
            ZF_IN zfindex insertAt
            , ZF_IN const zft_zfstring<T_Char> &s
            ) {
        return this->insert(insertAt, s.cString(), s.length());
    }
    /** @brief insert string */
    zft_zfstring<T_Char> &insert(
            ZF_IN zfindex insertAt
            , ZF_IN const void *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(insertAt >= this->length()) {
            this->append(s, len);
        }
        else if(s) {
            if(len == zfindexMax()) {
                len = _ZFP_zfstring_len((const T_Char *)s);
            }
            if(len > 0) {
                zfindex lenTmp = this->length();
                _prepareWrite(lenTmp + len);
                d->length = (zfuint)(lenTmp + len);
                zfmemmove(d->d.buf + insertAt + len, d->d.buf + insertAt, (lenTmp - insertAt) * sizeof(T_Char));
                zfmemcpy(d->d.buf + insertAt, s, len * sizeof(T_Char));
                d->d.buf[d->length] = '\0';
            }
        }
        return *this;
    }

public:
    /** @brief replace string in range */
    inline zft_zfstring<T_Char> &replace(
            ZF_IN zfindex replacePos
            , ZF_IN zfindex replaceLen
            , ZF_IN const zft_zfstring<T_Char> &s
            ) {
        return this->replace(replacePos, replaceLen, s.cString(), s.length());
    }
    /** @brief replace string in range */
    zft_zfstring<T_Char> &replace(
            ZF_IN zfindex replacePos
            , ZF_IN zfindex replaceLen
            , ZF_IN const void *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(replacePos >= this->length()) {
            this->append(s, len);
        }
        else if(s) {
            zfindex lenTmp = this->length();
            if(replaceLen > lenTmp - replacePos) {
                replaceLen = lenTmp - replacePos;
            }
            if(len == zfindexMax()) {
                len = _ZFP_zfstring_len((const T_Char *)s);
            }
            if(len > replaceLen) {
                _prepareWrite(lenTmp + len - replaceLen);
            }
            else {
                _prepareWrite(lenTmp);
            }
            zfmemmove(d->d.buf + replacePos + len, d->d.buf + replacePos + replaceLen, (lenTmp - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(d->d.buf + replacePos, s, len * sizeof(T_Char));
            d->length = (zfuint)(lenTmp + len - replaceLen);
            d->d.buf[d->length] = '\0';
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const {
        return d->d.ptr;
    }
    /** @brief length of the string */
    inline zfindex length(void) const {
        return (zfindex)d->length;
    }
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {
        return d->length == 0;
    }

public:
    /** @brief ensure the string's capacity (including tail '\0'), note the result capacity is not ensured same as requested one */
    inline void capacity(ZF_IN zfindex capacity) {
        if(capacity >= d->capacity) {
            _capacityChange(capacity - 1, zftrue);
        }
    }
    /** @brief get current capacity (including tail '\0') */
    inline zfindex capacity(void) const {
        return (zfindex)d->capacity;
    }
    /** @brief trim to a proper capacity to save memory */
    inline void capacityTrim(void) {
        if(d->capacity >= 128 && d->length < d->capacity / 3) {
            _capacityChange(this->length(), zftrue);
        }
    }
    /** @brief remove part of the string */
    void remove(
            ZF_IN zfindex pos
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        zfindex lenTmp = this->length();
        if(pos < lenTmp) {
            if(len > lenTmp - pos) {
                len = lenTmp - pos;
            }
            if(len > 0) {
                _prepareWrite(lenTmp);
                zfmemmove(d->d.buf + pos, d->d.buf + pos + len, (lenTmp - pos - len) * sizeof(T_Char));
                d->length -= (zfuint)len;
                d->d.buf[d->length] = '\0';
            }
        }
    }
    /** @brief remove all content of the string */
    void removeAll(void) {
        if(!this->isEmpty()) {
            ZFCoreMutexLocker();
            if(d->refCount == 1) {
                if(d->capacity >= 128) {
                    zffree(d->d.buf);
                    zfpoolDelete(d);
                    d = _ZFP_Empty();
                    ++(d->refCount);
                }
                else if(d->capacity > 0) {
                    d->d.buf[0] = '\0';
                    d->length = 0;
                }
                else {
                    d->d.ptr = "";
                    d->length = 0;
                }
            }
            else {
                --(d->refCount);
                d = _ZFP_Empty();
                ++(d->refCount);
            }
        }
    }

public:
    /** @brief compare with another string */
    inline zfint compare(ZF_IN const zft_zfstring<T_Char> &s) const {
        return d == s.d ? 0 : _ZFP_zfstring_cmp(this->cString(), s.cString());
    }
    /** @brief compare with another string */
    zfint compare(
            ZF_IN const T_Char *s
            , ZF_IN zfindex len = zfindexMax()
            ) const {
        if(len == zfindexMax()) {
            len = s ? _ZFP_zfstring_len(s) : 0;
        }
        if(this->length() < len) {
            return -1;
        }
        else if(this->length() > len) {
            return 1;
        }
        else {
            return _ZFP_zfstring_ncmp(this->cString(), s, len);
        }
    }

public:
    /**
     * @brief return internal buffer
     */
    const void *buffer(void) const {
        return d->d.ptr;
    }

    /**
     * @brief give up the buffer's ownership and return the buffer,
     *   you must free it manually by #zfunsafe_bufferFree
     */
    void zfunsafe_bufferGiveUp(ZF_OUT void *&ret, ZF_OUT zfindex &length) {
        ZFCoreMutexLocker();
        _prepareWrite(this->length());
        ret = (void *)d->d.buf;
        length = d->length;
        zfpoolDelete(d);
        d = _ZFP_Empty();
        ++(d->refCount);
    }
    /**
     * @brief free buffer returned by #zfunsafe_bufferGiveUp
     */
    static void zfunsafe_bufferFree(ZF_IN void *buf) {
        zffree(buf);
    }

    /**
     * @brief directly access internal writable buffer
     */
    T_Char *zfunsafe_buffer(void) {
        _prepareWrite(this->length());
        return d->d.buf;
    }
    /**
     * @brief directly modify the string's length
     */
    void zfunsafe_length(ZF_IN zfindex length) {
        _prepareWrite(this->length());
        d->length = (zfuint)length;
    }

private:
    _ZFP_zfstringD<T_Char> *d;
public:
    /** @brief global null string ref for impl */
    static inline const zft_zfstring<T_Char> &Empty(void) {
        static const zft_zfstring<T_Char> d;
        return d;
    }
public:
    /**
     * @brief explicitly create from literal string,
     *   you must ensure the literal's life exceeds the returned string
     */
    static zft_zfstring<T_Char> shared(
            ZF_IN const void *sLiteral
            , ZF_IN_OPT zfindex length = zfindexMax()
            ) {
        _ZFP_zfstringD<T_Char> *d = zfpoolNew(_ZFP_zfstringD<T_Char>);
        d->d.ptr = (const T_Char *)sLiteral;
        d->length = (zfuint)(length == zfindexMax() ? _ZFP_zfstring_len((const T_Char *)sLiteral) : length);
        return zft_zfstring<T_Char>(d);
    }
private:
    explicit zft_zfstring(ZF_IN _ZFP_zfstringD<T_Char> *d)
    : d(d)
    {
    }
private:
    static _ZFP_zfstringD<T_Char> *_ZFP_Empty(void) {
        static _ZFP_zfstringH<T_Char> d;
        return d.d;
    }
    static inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity < 32) {
            capacity = ((capacity / 16) + 1) * 16;
        }
        else if(capacity < 64) {
            capacity = ((capacity / 32) + 1) * 32;
        }
        else if(capacity < 1024) {
            capacity = ((capacity / 128) + 1) * 128;
        }
        else {
            capacity = ((capacity / 1024) + 1) * 1024;
        }
    }
    // capacity: excluding tail '\0'
    void _capacityChange(ZF_IN zfindex capacity, zfbool keepContents) {
        _capacityOptimize(capacity);
        ZFCoreMutexLocker();
        if(d->refCount == 1) {
            if(d->capacity > 0) {
                d->d.buf = (T_Char *)zfrealloc(d->d.buf, capacity * sizeof(T_Char));
            }
            else {
                const T_Char *ptr = d->d.ptr;
                d->d.buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
                if(ptr && keepContents) {
                    zfmemcpy(d->d.buf, ptr, capacity * sizeof(T_Char));
                }
                else {
                    d->d.buf[0] = '\0';
                }
            }
        }
        else {
            _ZFP_zfstringD<T_Char> *dTmp = d;
            d = zfpoolNew(_ZFP_zfstringD<T_Char>);
            d->length = dTmp->length;
            d->d.buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
            if(dTmp->length > 0 && keepContents) {
                zfmemcpy(d->d.buf, dTmp->d.ptr, dTmp->length * sizeof(T_Char));
                d->d.buf[dTmp->length] = '\0';
            }
            else {
                d->d.buf[0] = '\0';
            }
        }
        d->capacity = (zfuint)capacity;
        if(!keepContents) {
            d->length = 0;
        }
    }
    inline void _prepareWrite(ZF_IN zfindex len) {
        if(len >= d->capacity || d->refCount > 1) {
            _capacityChange(len, zftrue);
        }
    }
};

/**
 * @brief util macro for zfstring::shared
 */
#ifndef zftext
    #define zftext(s) zfstring::shared(s)
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfstring_h_


/**
 * @file zfstring.h
 * @brief string impl to reduce dependency of std::string
 */

#ifndef _ZFI_zfstring_h_
#define _ZFI_zfstring_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfstringType_Normal ((zft_zfuint16)0)
#define _ZFP_zfstringType_RawPtr ((zft_zfuint16)1)

// ============================================================
template<typename T_Char>
zffinal zfclassPOD _ZFP_zfstring {
public:
    union {
        T_Char *buf; // normal string
        const T_Char *ptr; // when type == _ZFP_zfstringType_RawPtr
    } s;
    zfuint length;
    zft_zfuint16 refCount;
    zft_zfuint16 type;

public:
    _ZFP_zfstring(void)
    : s()
    , length(0)
    , refCount(1)
    , type(_ZFP_zfstringType_Normal)
    {
    }
    ~_ZFP_zfstring(void) {
        if(this->type == _ZFP_zfstringType_Normal) {
            zffree(s.buf);
        }
    }
};

/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD zft_zfstring {
public:
    /** @brief construct an empty string */
    zft_zfstring(void)
    : d(zfnull)
    {
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s)
    : d(s.d)
    {
        if(d) {
            ++d->refCount;
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos)
    : d(zfnull)
    {
        if(pos < s.length()) {
            this->assign(s.cString() + pos);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos, zfindex len)
    : d(zfnull)
    {
        if(pos < s.length()) {
            if(len > s.length() - pos) {
                len = s.length() - pos;
            }
            this->assign(s.cString() + pos, len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s)
    : d(zfnull)
    {
        if(s) {
            this->assign(s);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s, zfindex len)
    : d(zfnull)
    {
        if(s) {
            this->assign(s, len);
        }
    }
    ~zft_zfstring(void) {
        if(d) {
            if(d->refCount == 1) {
                zfdelete(d);
            }
            else {
                --(d->refCount);
            }
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char *(void) const {return this->cString();}
public:
    inline zft_zfstring<T_Char> &operator = (ZF_IN const zft_zfstring<T_Char> &s) {
        _ZFP_zfstring<T_Char> *dTmp = d;
        d = s.d;
        if(d) {
            ++(d->refCount);
        }
        if(dTmp) {
            if(dTmp->refCount == 1) {
                zfdelete(dTmp);
            }
            else {
                --(dTmp->refCount);
            }
        }
        return *this;
    }
    inline zft_zfstring<T_Char> &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    zfbool operator == (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) != 0);}
    zfbool operator == (ZF_IN const T_Char *ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const T_Char *ref) const {return (this->compare(ref) != 0);}
public:
    /* ZFTAG_TRICKS: tricks to make zfstlmap<zfstring, xxx> works */
    inline zfbool operator < (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) < 0;}
    inline zfbool operator <= (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) <= 0;}
    inline zfbool operator > (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) > 0;}
    inline zfbool operator >= (ZF_IN const zft_zfstring<T_Char> &ref) const {return this->compare(ref) >= 0;}
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline zft_zfstring<T_Char> &operator += (ZF_IN const zft_zfstring<T_Char> &s) {return this->append(s);}
    inline zft_zfstring<T_Char> &operator += (ZF_IN const T_Char *s) {return this->append(s);}
    zft_zfstring<T_Char> &operator += (ZF_IN T_Char c) {
        _capacityRequire(this->length() + 1);
        d->s.buf[d->length] = c;
        ++(d->length);
        d->s.buf[d->length] = '\0';
        return *this;
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
        if(d == zfnull || d->type == _ZFP_zfstringType_RawPtr || d->refCount != 1) {
            _capacityRequire(this->length());
        }
        d->s.buf[pos] = c;
    }
    /**
     * @brief get char at index
     */
    inline T_Char get(ZF_IN zfindex pos) const {
        if(d) {
            return d->s.ptr[pos];
        }
        else {
            return '\0';
        }
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
        _ZFP_zfstring<T_Char> *dTmp = d;
        d = ref.d;
        ref.d = dTmp;
    }

public:
    /** @brief append string */
    inline zft_zfstring<T_Char> &append(ZF_IN const zft_zfstring<T_Char> &s) {this->append(s.cString(), s.length()); return *this;}
    /** @brief append string */
    zft_zfstring<T_Char> &append(
            ZF_IN const T_Char *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(s) {
            if(len == zfindexMax()) {
                len = _len(s);
            }
            zfindex lenTmp = this->length();
            _capacityRequire(lenTmp + len);
            zfmemcpy(d->s.buf + lenTmp, s, len * sizeof(T_Char));
            d->length = (zfuint)(lenTmp + len);
            d->s.buf[d->length] = '\0';
        }
        return *this;
    }

public:
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(ZF_IN const zft_zfstring<T_Char> &s) {
        this->operator = (s);
        return *this;
    }
    /** @brief replace all content of the string */
    zft_zfstring<T_Char> &assign(
            ZF_IN const T_Char *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(len == zfindexMax()) {
            if(s) {
                len = _len(s);
            }
            else {
                len = 0;
            }
        }
        _capacityRequire(len);;
        d->length = (zfuint)len;
        zfmemcpy(d->s.buf, s, len * sizeof(T_Char));
        d->s.buf[len] = '\0';
        return *this;
    }

    /**
     * @brief transfer ownership and assign,
     *   the source memory would be free-ed by this string object
     */
    void zfunsafe_assign(
            ZF_IN T_Char *s
            , ZF_IN zfindex length
            ) {
        if(d) {
            if(d->refCount > 1) {
                --(d->refCount);
                d = zfnew(_ZFP_zfstring<T_Char>);
            }
            else {
                if(d->type == _ZFP_zfstringType_Normal && d->s.buf) {
                    zffree(d->s.buf);
                }
            }
        }
        else {
            d = zfnew(_ZFP_zfstring<T_Char>);
        }
        d->s.buf = s;
        d->length = length;
    }
    /**
     * @brief directly access internal writable buffer
     */
    T_Char *zfunsafe_buffer(void) {
        return d->s.buf;
    }
    /**
     * @brief directly modify the string's length
     */
    void zfunsafe_length(ZF_IN zfindex length) {
        d->length = (zfuint)length;
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
            , ZF_IN const T_Char *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(insertAt >= this->length()) {
            this->append(s, len);
        }
        else if(s) {
            if(len == zfindexMax()) {
                len = _len(s);
            }
            zfindex lenTmp = this->length();
            _capacityRequire(lenTmp + len);
            d->length = (zfuint)(lenTmp + len);
            zfmemmove(d->s.buf + insertAt + len, d->s.buf + insertAt, (lenTmp - insertAt) * sizeof(T_Char));
            zfmemcpy(d->s.buf + insertAt, s, len * sizeof(T_Char));
            d->s.buf[d->length] = '\0';
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
            , ZF_IN const T_Char *s
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
                len = _len(s);
            }
            if(len > replaceLen) {
                _capacityRequire(lenTmp + len - replaceLen);
            }
            else {
                _capacityRequire(lenTmp);
            }
            zfmemmove(d->s.buf + replacePos + len, d->s.buf + replacePos + replaceLen, (lenTmp - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(d->s.buf + replacePos, s, len * sizeof(T_Char));
            d->length = (zfuint)(lenTmp + len - replaceLen);
            d->s.buf[d->length] = '\0';
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const {
        return d ? d->s.ptr : "";
    }
    /** @brief length of the string */
    inline zfindex length(void) const {
        return d ? d->length : 0;
    }
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {
        return d ? d->length == 0 : zftrue;
    }

public:
    /** @brief ensure the string's capacity, note the result capacity is not ensured same as requested one */
    inline void capacity(ZF_IN zfindex capacity) {
        if(capacity > this->length()) {
            _capacityRequire(capacity);
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
                _capacityRequire(lenTmp);
                zfmemmove(d->s.buf + pos, d->s.buf + pos + len, (lenTmp - pos - len) * sizeof(T_Char));
                d->length -= len;
                d->s.buf[d->length] = '\0';
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void) {
        if(d) {
            if(d->refCount == 1) {
                if(d->type == _ZFP_zfstringType_Normal) {
                    if(d->s.buf) {
                        d->s.buf[0] = '\0';
                    }
                    d->length = 0;
                }
                else {
                    d->s.ptr = "";
                    d->length = 0;
                }
            }
            else {
                --(d->refCount);
                d = zfnull;
            }
        }
    }

public:
    /** @brief compare with another string */
    inline zfint compare(ZF_IN const zft_zfstring<T_Char> &s) const {return _cmp(this->cString(), s.cString());}
    /** @brief compare with another string */
    zfint compare(
            ZF_IN const T_Char *s
            , ZF_IN zfindex len = zfindexMax()
            ) const {
        const T_Char *buf = this->cString();
        if(s) {
            if(len == zfindexMax()) {
                len = _len(s);
            }
            return _ncmp(buf, s, (len >= this->length() ? this->length() : len) + 1);
        }
        else {
            return (zfint)buf[0];
        }
    }

private:
    _ZFP_zfstring<T_Char> *d;
private:
    static inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        ++capacity; // length + '\0'
        if(capacity < 64) {
            capacity = ((capacity / 16) + 1) * 16;
        }
        else if(capacity < 256) {
            capacity = ((capacity / 64) + 1) * 64;
        }
        else if(capacity < 1024) {
            capacity = ((capacity / 256) + 1) * 256;
        }
        else {
            capacity = ((capacity / 1024) + 1) * 1024;
        }
    }
    void _capacityRequire(ZF_IN zfindex capacity) {
        _capacityOptimize(capacity);
        if(d == zfnull) {
            d = zfnew(_ZFP_zfstring<T_Char>);
        }
        if(d->refCount == 1) {
            if(d->length == _ZFP_zfstringType_RawPtr) {
                const T_Char *ptr = d->s.ptr;
                d->length = (zfuint)_len(ptr);
                d->s.buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
                zfmemcpy(d->s.buf, ptr, (d->length + 1) * sizeof(T_Char));
            }
            else {
                d->s.buf = (T_Char *)zfrealloc(d->s.buf, capacity * sizeof(T_Char));
                d->s.buf[d->length] = '\0';
            }
        }
        else {
            _ZFP_zfstring<T_Char> *dTmp = d;
            d = zfnew(_ZFP_zfstring<T_Char>);
            if(dTmp->length == _ZFP_zfstringType_RawPtr) {
                d->length = (zfuint)_len(dTmp->s.ptr);
            }
            else {
                d->length = dTmp->length;
            }
            zfindex capacity = d->length;
            _capacityOptimize(capacity);
            d->s.buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
            zfmemcpy(d->s.buf, dTmp->s.ptr, (d->length + 1) * sizeof(T_Char));
            --(dTmp->refCount);
        }
    }
    static zfindex _len(ZF_IN const T_Char *s) {
        const T_Char *p = s;
        while(*p) {++p;}
        return p - s;
    }
    static zfint _cmp(
            ZF_IN const T_Char *s1
            , ZF_IN const T_Char *s2
            ) {
        while(*s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
        return *s1 - *s2;
    }
    static zfint _ncmp(
            ZF_IN const T_Char *s1
            , ZF_IN const T_Char *s2
            , ZF_IN zfindex len
            ) {
        while(--len && *s1 && *s2 && *s1 == *s2) {++s1, ++s2;}
        return *s1 - *s2;
    }
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfstring_h_


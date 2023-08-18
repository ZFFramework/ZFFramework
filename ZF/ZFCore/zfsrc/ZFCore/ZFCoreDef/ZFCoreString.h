/**
 * @file ZFCoreString.h
 * @brief string impl to reduce dependency of std::string
 */

#ifndef _ZFI_ZFCoreString_h_
#define _ZFI_ZFCoreString_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

enum {
    _ZFP_zfstring_bufSize = (sizeof(void *) + sizeof(zfuint) * 2),
};
#define _ZFP_zfstringType_DynamicBuf ((zfuint)-1)

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Char>
zffinal zfclassPOD _ZFP_zfstring {
public:
    union {
        // _ZFP_zfstringType_DynamicBuf
        struct {
            T_Char *s;
            zfuint capacity; // capacity exclude '\0'
            zfuint length;
        } s;

        // builtin
        T_Char buf[_ZFP_zfstring_bufSize]; // capacity = _ZFP_zfstring_bufSize - 1
    } d;
    /*
     * when !=_ZFP_zfstringType_DynamicBuf, using builtin buf, and length is string size
     * when ==_ZFP_zfstringType_DynamicBuf, using dynamic buf
     */
    zfuint length;

public:
    _ZFP_zfstring(void)
    : d()
    , length(0)
    {
    }

public:
    inline T_Char *buf(void) {
        switch(this->length) {
            case _ZFP_zfstringType_DynamicBuf:
                return d.s.s;
            default:
                return d.buf;
        }
    }
    inline const T_Char *buf(void) const {
        switch(this->length) {
            case _ZFP_zfstringType_DynamicBuf:
                return d.s.s;
            default:
                return d.buf;
        }
    }
};
/** @endcond */

/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD zft_zfstring {
public:
    /** @brief construct an empty string */
    zft_zfstring(void)
    : d()
    {
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s)
    : d()
    {
        T_Char *buf = _capacityRequire(s.length());
        zfmemcpy(buf, s.cString(), (s.length() + 1) * sizeof(T_Char));
        _updateLength(s.length());
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos)
    : d()
    {
        if(pos < s.length()) {
            zfindex len = s.length() - pos;
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s.cString() + pos, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos, zfindex len)
    : d()
    {
        if(pos < s.length()) {
            if(len > s.length() - pos) {
                len = s.length() - pos;
            }
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s.cString() + pos, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s)
    : d()
    {
        if(s) {
            zfindex len = _len(s);
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s, (len + 1) * sizeof(T_Char));
            _updateLength(len);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s, zfindex len)
    : d()
    {
        if(s) {
            if(len == zfindexMax()) {
                len = _len(s);
            }
            T_Char *buf = _capacityRequire(len);
            zfmemcpy(buf, s, len * sizeof(T_Char));
            buf[len] = '\0';
            _updateLength(len);
        }
    }
    ~zft_zfstring(void) {
        switch(d.length) {
            case _ZFP_zfstringType_DynamicBuf:
                zffree(d.d.s.s);
                break;
            default:
                break;
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char *(void) const {return this->cString();}
public:
    inline zft_zfstring<T_Char> &operator = (ZF_IN const zft_zfstring<T_Char> &s) {
        if(&s == this) {
            return *this;
        }
        else {
            return this->assign(s);
        }
    }
    inline zft_zfstring<T_Char> &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    zfbool operator == (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const zft_zfstring<T_Char> &ref) const {return (this->compare(ref) != 0);}
    zfbool operator == (ZF_IN const T_Char *ref) const {return (this->compare(ref) == 0);}
    zfbool operator != (ZF_IN const T_Char *ref) const {return (this->compare(ref) != 0);}
public:
    // tricks to make zfstlmap<zfstring, xxx> works
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
        zfindex len = this->length();
        T_Char *buf = _capacityRequire(len + 1);
        buf[len] = c;
        ++len;
        buf[len] = '\0';
        _updateLength(len);
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
        d.buf()[pos] = c;
    }
    /**
     * @brief get char at index
     */
    inline T_Char const &get(ZF_IN zfindex pos) const {
        return d.buf()[pos];
    }

    /** @cond ZFPrivateDoc */
    template<typename T_Int>
    inline T_Char &operator [] (ZF_IN T_Int pos) {
        return d.buf()[pos];
    }
    template<typename T_Int>
    inline const T_Char &operator [] (ZF_IN T_Int pos) const {
        return d.buf()[pos];
    }
    /** @endcond */

public:
    /**
     * @brief swap internal data without deep copy,
     *   designed for performance
     */
    void swap(ZF_IN_OUT zft_zfstring<T_Char> &ref) {
        zfbyte buf[sizeof(d)];
        zfmemcpy(buf, &d, sizeof(d));
        zfmemcpy(&d, &ref.d, sizeof(d));
        zfmemcpy(&ref.d, buf, sizeof(d));
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
            T_Char *buf = _capacityRequire(lenTmp + len);
            zfmemcpy(buf + lenTmp, s, len * sizeof(T_Char));
            lenTmp += len;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
        }
        return *this;
    }

public:
    /** @brief replace all content of the string */
    inline zft_zfstring<T_Char> &assign(ZF_IN const zft_zfstring<T_Char> &s) {return this->assign(s.cString(), s.length());}
    /** @brief replace all content of the string */
    zft_zfstring<T_Char> &assign(
            ZF_IN const T_Char *s
            , ZF_IN_OPT zfindex len = zfindexMax()
            ) {
        if(s) {
            if(len == zfindexMax()) {
                len = _len(s);
            }
            T_Char *buf = _capacityRequire(len);;
            zfmemcpy(buf, s, len * sizeof(T_Char));
            buf[len] = '\0';
            _updateLength(len);
        }
        else {
            d.buf()[0] = '\0';
            _updateLength(0);
        }
        return *this;
    }

    /**
     * @brief transfer ownership and assign,
     *   the source memory would be free-ed by this string object
     *
     * note the capacity should not contain the tail '\0'
     */
    void zfunsafe_assign(
            ZF_IN T_Char *s
            , ZF_IN zfindex capacity
            , ZF_IN zfindex length
            ) {
        if(d.length == _ZFP_zfstringType_DynamicBuf) {
            if(d.d.s.s != s) {
                zffree(d.d.s.s);
            }
        }
        d.length = _ZFP_zfstringType_DynamicBuf;
        d.d.s.s = s;
        d.d.s.capacity = (zfuint)capacity;
        d.d.s.length = (zfuint)length;
    }
    /**
     * @brief directly access internal writable buffer
     */
    T_Char *zfunsafe_buffer(void) {
        return d.buf();
    }
    /**
     * @brief directly modify the string's length
     */
    void zfunsafe_length(ZF_IN zfindex length) {
        if(length <= this->capacity()) {
            if(d.length == _ZFP_zfstringType_DynamicBuf) {
                d.d.s.length = (zfuint)length;
            }
            else {
                d.length = (zfuint)length;
            }
        }
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
            T_Char *buf = _capacityRequire(lenTmp + len);
            zfmemmove(buf + insertAt + len, buf + insertAt, (lenTmp - insertAt) * sizeof(T_Char));
            zfmemcpy(buf + insertAt, s, len * sizeof(T_Char));
            lenTmp += len;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
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
            T_Char *buf = _capacityRequire(lenTmp + len - replaceLen);
            zfmemmove(buf + replacePos + len, buf + replacePos + replaceLen, (lenTmp - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(buf + replacePos, s, len * sizeof(T_Char));
            lenTmp = lenTmp + len - replaceLen;
            buf[lenTmp] = '\0';
            _updateLength(lenTmp);
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const {
        return d.buf();
    }
    /** @brief length of the string */
    inline zfindex length(void) const {
        switch(d.length) {
            case _ZFP_zfstringType_DynamicBuf:
                return (zfindex)d.d.s.length;
            default:
                return (zfindex)d.length;
        }
    }
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {
        switch(d.length) {
            case _ZFP_zfstringType_DynamicBuf:
                return d.d.s.length == 0;
            default:
                return d.length == 0;
        }
    }

public:
    /** @brief ensure the string's capacity, note the result capacity is not ensured same as requested one */
    inline void capacity(ZF_IN zfindex capacity) {_capacityRequire(capacity);}
    /** @brief capacity of the string */
    zfindex capacity(void) {
        switch(d.length) {
            case _ZFP_zfstringType_DynamicBuf:
                return (zfindex)d.d.s.capacity;
            default:
                return (zfindex)(_ZFP_zfstring_bufSize - 1);
        }
    }
    /**
     * @brief trim current capacity
     *
     * do nothing if not necessary to trim
     */
    void capacityTrim(void) {
        zfindex capacity = this->count();
        _capacityOptimize(capacity);
        if(capacity != this->capacity()) {
            _capacityDoChange(capacity);
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
                T_Char *buf = d.buf();
                zfmemmove(buf + pos, buf + pos + len, (lenTmp - pos - len) * sizeof(T_Char));
                lenTmp -= len;
                buf[lenTmp] = '\0';
                _updateLength(lenTmp);
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void) {
        d.buf()[0] = '\0';
        _updateLength(0);
    }

public:
    /** @brief compare with another string */
    inline zfint compare(ZF_IN const zft_zfstring<T_Char> &s) const {return _cmp(this->cString(), s.cString());}
    /** @brief compare with another string */
    zfint compare(
            ZF_IN const T_Char *s
            , ZF_IN zfindex len = zfindexMax()
            ) const {
        const T_Char *buf = d.buf();
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
    _ZFP_zfstring<T_Char> d;
private:
    static inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity < _ZFP_zfstring_bufSize) {
            capacity = _ZFP_zfstring_bufSize - 1;
        }
        else if(capacity < 256) {
            capacity = ((capacity / 64) + 1) * 64 - 1;
        }
        else if(capacity < 1024) {
            capacity = ((capacity / 256) + 1) * 256 - 1;
        }
        else {
            capacity = ((capacity / 1024) + 1) * 1024 - 1;
        }
    }
    T_Char *_capacityRequire(ZF_IN zfindex capacity) {
        _capacityOptimize(capacity);
        if(capacity > this->capacity()) {
            return _capacityDoChange(capacity);
        }
        else {
            return d.buf();
        }
    }
    T_Char *_capacityDoChange(ZF_IN zfindex capacity) {
        if(capacity <= _ZFP_zfstring_bufSize - 1) {
            if(d.length == _ZFP_zfstringType_DynamicBuf) {
                T_Char *bufOld = d.d.s.s;
                d.length = d.d.s.length;
                zfmemcpy(d.d.buf, bufOld, (d.length + 1) * sizeof(T_Char));
                zffree(bufOld);
            }
            return d.d.buf;
        }
        else {
            if(d.length != _ZFP_zfstringType_DynamicBuf) {
                T_Char *buf = (T_Char *)zfmalloc((capacity + 1) * sizeof(T_Char));
                zfmemcpy(buf, d.d.buf, (d.length + 1) * sizeof(T_Char));
                d.d.s.s = buf;
                d.d.s.capacity = (zfuint)capacity;
                d.d.s.length = d.length;
                d.length = _ZFP_zfstringType_DynamicBuf;
                return d.d.s.s;
            }
            else {
                d.d.s.s = (T_Char *)zfrealloc(d.d.s.s, (capacity + 1) * sizeof(T_Char));
                d.d.s.capacity = (zfuint)capacity;
                return d.d.s.s;
            }
        }
    }
    inline void _updateLength(ZF_IN zfindex len) {
        switch(d.length) {
            case _ZFP_zfstringType_DynamicBuf:
                d.d.s.length = (zfuint)len;
                break;
            default:
                d.length = (zfuint)len;
                break;
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

#endif // #ifndef _ZFI_ZFCoreString_h_


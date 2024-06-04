/**
 * @file zfstring.h
 * @brief string impl to reduce dependency of std::string
 */

#ifndef _ZFI_zfstring_h_
#define _ZFI_zfstring_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"
#include "ZFCoreMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassPOD ZFLIB_ZFCore _ZFP_zfstringHead {
public:
    zfuint refCount;
    zfuint capacity;
    zfuint length;
};

/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD zft_zfstring {
public:
    /** @brief construct an empty string */
    zft_zfstring(void)
    : _buf(zfnull)
    {
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s)
    : _buf(s._buf)
    {
        if(_buf) {
            zfCoreMutexLocker();
            ++_head(_buf).refCount;
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos)
    : _buf(zfnull)
    {
        if(pos < s.length()) {
            this->assign(s.cString() + pos);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const zft_zfstring<T_Char> &s, zfindex pos, zfindex len)
    : _buf(zfnull)
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
    : _buf(zfnull)
    {
        if(s) {
            this->assign(s);
        }
    }
    /** @brief copy content from another string */
    zft_zfstring(ZF_IN const T_Char *s, zfindex len)
    : _buf(zfnull)
    {
        if(s) {
            this->assign(s, len);
        }
    }
    ~zft_zfstring(void) {
        if(_buf) {
            zfCoreMutexLocker();
            if(_head(_buf).refCount == 1) {
                zffree(&_head(_buf));
            }
            else {
                --(_head(_buf).refCount);
            }
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char *(void) const {return this->cString();}
public:
    inline zft_zfstring<T_Char> &operator = (ZF_IN const zft_zfstring<T_Char> &s) {
        zfCoreMutexLocker();
        T_Char *bufTmp = _buf;
        _buf = s._buf;
        if(_buf) {
            ++(_head(_buf).refCount);
        }
        if(bufTmp) {
            if(_head(bufTmp).refCount == 1) {
                zffree(&_head(bufTmp));
            }
            else {
                --(_head(bufTmp).refCount);
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
        _prepareWrite(this->length() + 1);
        _buf[_head(_buf).length] = c;
        _buf[++(_head(_buf).length)] = '\0';
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
        _prepareWrite(this->length());
        _buf[pos] = c;
    }
    /**
     * @brief get char at index
     */
    inline T_Char get(ZF_IN zfindex pos) const {
        if(_buf) {
            return _buf[pos];
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
        T_Char *bufTmp = _buf;
        _buf = ref._buf;
        ref._buf = bufTmp;
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
            _prepareWrite(lenTmp + len);
            zfmemcpy(_buf + lenTmp, s, len * sizeof(T_Char));
            _buf[_head(_buf).length = (zfuint)(lenTmp + len)] = '\0';
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
        _prepareWrite(len);;
        _head(_buf).length = (zfuint)len;
        zfmemcpy(_buf, s, len * sizeof(T_Char));
        _buf[len] = '\0';
        return *this;
    }

    /**
     * @brief directly access internal writable buffer
     */
    T_Char *zfunsafe_buffer(void) {
        return _buf;
    }
    /**
     * @brief directly modify the string's length
     */
    void zfunsafe_length(ZF_IN zfindex length) {
        _head(_buf).length = (zfuint)length;
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
            _prepareWrite(lenTmp + len);
            _head(_buf).length = (zfuint)(lenTmp + len);
            zfmemmove(_buf + insertAt + len, _buf + insertAt, (lenTmp - insertAt) * sizeof(T_Char));
            zfmemcpy(_buf + insertAt, s, len * sizeof(T_Char));
            _buf[_head(_buf).length] = '\0';
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
                _prepareWrite(lenTmp + len - replaceLen);
            }
            else {
                _prepareWrite(lenTmp);
            }
            zfmemmove(_buf + replacePos + len, _buf + replacePos + replaceLen, (lenTmp - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(_buf + replacePos, s, len * sizeof(T_Char));
            _head(_buf).length = (zfuint)(lenTmp + len - replaceLen);
            _buf[_head(_buf).length] = '\0';
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const {
        return _buf ? _buf : (const T_Char *)&_buf;
    }
    /** @brief length of the string */
    inline zfindex length(void) const {
        return _buf ? _head(_buf).length : 0;
    }
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {
        return !_buf || _head(_buf).length == 0;
    }

public:
    /** @brief ensure the string's capacity (excluding tail '\0'), note the result capacity is not ensured same as requested one */
    inline void capacity(ZF_IN zfindex capacity) {
        _prepareWrite(capacity);
    }
    /** @brief get current capacity (excluding tail '\0') */
    inline zfindex capacity(void) const {
        return _buf ? (zfindex)(_head(_buf).capacity - 1) : 0;
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
                zfmemmove(_buf + pos, _buf + pos + len, (lenTmp - pos - len) * sizeof(T_Char));
                _head(_buf).length -= len;
                _buf[_head(_buf).length] = '\0';
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void) {
        if(_buf) {
            zfCoreMutexLocker();
            if(_head(_buf).refCount == 1) {
                _buf[0] = '\0';
                _head(_buf).length = 0;
            }
            else {
                --(_head(_buf).refCount);
                _buf = zfnull;
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
    T_Char *_buf;
    static inline _ZFP_zfstringHead &_head(ZF_IN T_Char *buf) {
        return *((_ZFP_zfstringHead *)buf - 1);
    }
private:
    static inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
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
    void _prepareWrite(ZF_IN zfindex capacity) {
        _capacityOptimize(capacity);
        if(_buf && capacity <= _head(_buf).capacity && _head(_buf).refCount == 1) {
            return;
        }
        zfCoreMutexLocker();
        if(_buf && _head(_buf).refCount == 1) {
            _ZFP_zfstringHead *head = (_ZFP_zfstringHead *)_buf - 1;
            head = (_ZFP_zfstringHead *)zfrealloc(head, sizeof(_ZFP_zfstringHead) + capacity * sizeof(T_Char));
            head->capacity = capacity;
            _buf = (T_Char *)(head + 1);
        }
        else {
            T_Char *bufTmp = _buf;
            _ZFP_zfstringHead *head = (_ZFP_zfstringHead *)zfmalloc(sizeof(_ZFP_zfstringHead) + capacity * sizeof(T_Char));
            _buf = (T_Char *)(head + 1);
            head->refCount = 1;
            head->capacity = capacity;
            if(bufTmp) {
                head->length = _head(bufTmp).length;
                zfmemcpy(_buf, bufTmp, (_head(bufTmp).length + 1) * sizeof(T_Char));
                --(_head(bufTmp).refCount);
            }
            else {
                head->length = 0;
                _buf[0] = '\0';
            }
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


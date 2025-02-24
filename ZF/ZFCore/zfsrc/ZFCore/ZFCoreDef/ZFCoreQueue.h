/**
 * @file ZFCoreQueue.h
 * @brief core queue type for private use only
 */

#ifndef _ZFI_ZFCoreQueue_h_
#define _ZFI_ZFCoreQueue_h_

#include "ZFCoreArray.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief core queue type for performance and for private use only
 *
 * @warning can hold POD types only
 */
template<typename T_POD>
zffinal zfclassLikePOD ZFCoreQueuePOD {
private:
    enum {
        _bufSize = 16, // capacity = _bufSize - 1
    };
public:
    /**
     * @brief main constructor
     */
    ZFCoreQueuePOD(void)
    : _bufBuiltin()
    , _bufHead(_bufBuiltin)
    , _bufTail(_bufBuiltin + _bufSize)
    , _pHead(_bufHead)
    , _pTail(_pHead)
    {
    }
    virtual ~ZFCoreQueuePOD(void) {
        if(_bufHead != _bufBuiltin) {
            zffree(_bufHead);
        }
    }
private:
    // we don't allow copy
    ZFCoreQueuePOD(ZF_IN const ZFCoreQueuePOD<T_POD> &ref);
    ZFCoreQueuePOD<T_POD> &operator = (ZF_IN const ZFCoreQueuePOD<T_POD> &ref);

public:
    /** @brief see #objectInfo */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const {
        this->objectInfoOfContentT(ret, 10);
    }
    /** @brief return object info */
    zffinal inline zfstring objectInfo(void) const {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief see #objectInfoOfContent */
    zffinal void objectInfoOfContentT(
            ZF_IN_OUT zfstring &ret
            , ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            , ZF_IN_OPT typename ZFCoreInfoGetter<T_POD>::InfoGetter infoGetter = zfnull
            ) const {
        zfindex count = 0;
        ret += token.tokenLeft;
        for(T_POD *p = _pHead; p != _pTail && count < maxCount; ++count, _loopNext(p)) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }
            ret += token.tokenValueLeft;
            if(infoGetter != zfnull) {
                infoGetter(ret, *p);
            }
            else {
                zftToStringT(ret, *p);
            }
            ret += token.tokenValueRight;
        }
        if(count < this->count()) {
            if(count > 0) {
                ret += token.tokenSeparator;
            }
            ret += token.tokenEtc;
        }
        ret += token.tokenRight;
    }
    /**
     * @brief return contents info
     */
    zffinal zfstring objectInfoOfContent(
            ZF_IN_OPT zfindex maxCount = zfindexMax()
            , ZF_IN_OPT const ZFTokenForContainer &token = ZFTokenForContainerDefault()
            , ZF_IN_OPT typename ZFCoreInfoGetter<T_POD>::InfoGetter infoGetter = zfnull
            ) const {
        zfstring ret;
        this->objectInfoOfContentT(ret, maxCount, token, infoGetter);
        return ret;
    }

private:
    static inline void _capacityOptimize(ZF_IN_OUT zfindex &capacity) {
        if(capacity == 0) {
        }
        else if(capacity < 64) {
            capacity = ((capacity / 16) + 1) * 16 - 1;
        }
        else if(capacity < 256) {
            capacity = ((capacity / 64) + 1) * 64 - 1;
        }
        else {
            capacity = ((capacity / 256) + 1) * 256 - 1;
        }
    }
public:
    /**
     * @brief get current capacity
     *
     * capacity would be increased automatically during adding elements
     */
    inline zfindex capacity(void) const {
        return (_bufTail - _bufHead - 1);
    }
    /**
     * @brief change the capacity
     */
    void capacity(ZF_IN zfindex capacity) {
        if(capacity > this->capacity()) {
            _capacityOptimize(capacity);
            T_POD *bufHeadNew = (T_POD *)zfmalloc((capacity + 1) * sizeof(T_POD));
            _bufChange(bufHeadNew, bufHeadNew + capacity + 1);
        }
    }
    /**
     * @brief trim the buffer, call only if necessary
     */
    zffinal void capacityTrim(void) {
        zfindex capacity = this->count();
        if(capacity < _bufSize) {
            if(_bufHead != _bufBuiltin) {
                _bufChange(_bufBuiltin, _bufBuiltin + _bufSize);
            }
        }
        else {
            _capacityOptimize(capacity);
            if(capacity != this->capacity()) {
                T_POD *bufHeadNew = (T_POD *)zfmalloc((capacity + 1) * sizeof(T_POD));
                _bufChange(bufHeadNew, bufHeadNew + capacity + 1);
            }
        }
    }

public:
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    inline T_POD &add(void) {
        this->capacity(this->count() + 1);
        T_POD *ret = _pTail;
        _loopNext(_pTail);
        return *ret;
    }
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    inline void add(ZF_IN T_POD const &e) {
        this->capacity(this->count() + 1);
        *_pTail = e;
        _loopNext(_pTail);
    }
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    template<typename T_Type>
    void addFrom(ZF_IN const ZFCoreArray<T_Type> &arr) {
        for(zfindex i = 0, iEnd = arr.count(); i < iEnd; ++i) {
            this->add(arr[i]);
        }
    }
    /**
     * @brief push element at tail of the queue,
     *   auto increase capacity if necessary
     */
    void addFrom(
            ZF_IN const T_POD *buf
            , ZF_IN zfindex count
            ) {
        if(buf == zfnull || count == 0) {
            return;
        }
        this->capacity(this->count() + count);
        if(_pHead <= _pTail) {
            if(_pTail + count < _bufTail) {
                zfmemcpy(_pTail, buf, count * sizeof(T_POD));
                _pTail += count;
            }
            else {
                zfindex tmp = _bufTail - _pTail;
                zfmemcpy(_pTail, buf, tmp * sizeof(T_POD));
                zfmemcpy(_bufHead, buf + tmp, (count - tmp) * sizeof(T_POD));
                _pTail = _bufHead + (count - tmp);
            }
        }
        else {
            zfmemcpy(_pTail, buf, count * sizeof(T_POD));
            _pTail += count;
        }
    }
    /**
     * @brief take element at head of the queue, assert fail if empty
     */
    inline T_POD &take(void) {
        ZFCoreAssertWithMessage(_pHead != _pTail, "take from an empty queue");
        T_POD *ret = _pHead;
        _loopNext(_pHead);
        return *ret;
    }

    /**
     * @brief take element at tail of the queue, assert fail if empty
     */
    inline T_POD &takeLast(void) {
        ZFCoreAssertWithMessage(_pHead != _pTail, "take from an empty queue");
        if(_pTail == _bufHead) {
            _pTail = _bufTail - 1;
        }
        else {
            --_pTail;
        }
        return *_pTail;
    }

public:
    /**
     * @brief element count of this array
     */
    inline zfindex count(void) const {
        return ((_pTail >= _pHead)
            ? (_pTail - _pHead)
            : ((_bufTail - _pHead) + (_pTail - _bufHead)));
    }
    /**
     * @brief true if empty
     */
    inline zfbool isEmpty(void) const {
        return (_pHead == _pTail);
    }

public:
    /**
     * @brief remove all contents
     */
    void removeAll(void) {
        _pHead = _bufHead;
        _pTail = _bufHead;
    }

public:
    /**
     * @brief copy contents to array
     */
    template<typename T_Type>
    void toArrayT(ZF_IN_OUT ZFCoreArray<T_Type> &array) const {
        array.capacity(array.capacity() + this->count());
        T_POD *p = _pHead;
        while(p != _pTail) {
            array.add(*p);
            _loopNext(p);
        }
    }
    /**
     * @brief copy contents to array
     */
    ZFCoreArray<T_POD> toArray(void) const {
        ZFCoreArray<T_POD> ret;
        this->toArrayT(ret);
        return ret;
    }

private:
    T_POD _bufBuiltin[_bufSize];
    T_POD *_bufHead;
    T_POD *_bufTail;
    T_POD *_pHead;
    T_POD *_pTail;
private:
    inline void _loopNext(ZF_IN_OUT T_POD *&p) const {
        ++p;
        if(p >= _bufTail) {
            p = _bufHead;
        }
    }
    void _bufChange(
            ZF_IN T_POD *bufHeadNew
            , ZF_IN T_POD *bufTailNew
            ) {
        T_POD *pTailNew = bufHeadNew + this->count();

        if(_pTail >= _pHead) {
            zfmemcpy(bufHeadNew, _pHead, (_pTail - _pHead) * sizeof(T_POD));
        }
        else {
            zfmemcpy(bufHeadNew, _pHead, (_bufTail - _pHead) * sizeof(T_POD));
            zfmemcpy(bufHeadNew + (_bufTail - _pHead), _bufHead, (_pTail - _bufHead) * sizeof(T_POD));
        }

        if(_bufHead != _bufBuiltin) {
            zffree(_bufHead);
        }
        _bufHead = bufHeadNew;
        _bufTail = bufTailNew;
        _pHead = _bufHead;
        _pTail = pTailNew;
    }
};
ZFOUTPUT_TYPE_TEMPLATE(typename T_POD, ZFCoreQueuePOD<T_POD>, {v.objectInfoT(s);})

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreQueue_h_


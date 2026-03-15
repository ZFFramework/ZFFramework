/**
 * @file zfstlordermap.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstlordermap_h_
#define _ZFI_zfstlordermap_h_

#include "zfstlhashmap.h"
#include "zfstllist.h"

/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Hash = zfstlhash<T_Key>, typename T_Equal = zfstlequalto<T_Key> >
zfclassNotPOD zfstlordermap {
private:
    zfclassNotPOD T_HashWrap : zfextend T_Hash {
    public:
        zfstlsize operator () (ZF_IN const T_Key *const &key) const {
            return T_Hash::operator () (*key);
        }
    };
    zfclassNotPOD T_EqualWrap : zfextend T_Equal {
    public:
        zfbool operator () (ZF_IN const T_Key *const &key0, ZF_IN const T_Key *const &key1) const {
            return T_Equal::operator () (*key0, *key1);
        }
    };
    zfclassFwd Item;
    typedef zfstlhashmap<const T_Key *, Item *, T_HashWrap, T_EqualWrap> MapType;
    typedef zfstllist<Item *> ArrayType;
    zfclassNotPOD Item {
    public:
        zfstlpair<T_Key, T_Value> entry;
        typename MapType::iterator mapIt;
        typename ArrayType::iterator arrIt;
    public:
        Item(void) : entry(), mapIt(), arrIt() {}
        Item(ZF_IN const zfstlpair<T_Key, T_Value> &entry) : entry(entry), mapIt(), arrIt() {}
        Item(ZF_IN const T_Key &key, ZF_IN const T_Value &value) : entry(zfstlpair<T_Key, T_Value>(value)), mapIt(), arrIt() {}
    };

public:
    zfclassLikePOD iterator {
    public:
        zfstlpair<T_Key, T_Value> &operator *(void) {
            return (*implIt)->entry;
        }
        zfstlpair<T_Key, T_Value> *operator ->(void) {
            return &((*implIt)->entry);
        }
        const zfstlpair<T_Key, T_Value> &operator *(void) const {
            return (*implIt)->entry;
        }
        const zfstlpair<T_Key, T_Value> *operator ->(void) const {
            return &((*implIt)->entry);
        }
        iterator &operator++(void) {
            ++implIt;
            return *this;
        }
        iterator operator++(int) {
            iterator t(implIt);
            ++implIt;
            return t;
        }
        iterator &operator--(void) {
            --implIt;
            return *this;
        }
        iterator operator--(int) {
            iterator t(implIt);
            --implIt;
            return t;
        }
        zfbool operator == (ZF_IN const iterator &it) const {
            return implIt == it.implIt;
        }
        zfbool operator != (ZF_IN const iterator &it) const {
            return implIt != it.implIt;
        }
    public:
        iterator(void) : implIt() {}
        iterator(ZF_IN typename ArrayType::iterator const &implIt) : implIt(implIt) {}
        iterator(ZF_IN const iterator &it) : implIt(it.implIt) {}
    private:
        typename ArrayType::iterator implIt;
        friend zfclassFwd zfstlordermap<T_Key, T_Value, T_Hash, T_Equal>;
    };
    zfclassLikePOD const_iterator {
    public:
        const zfstlpair<T_Key, T_Value> &operator *(void) const {
            return (*implIt)->entry;
        }
        const zfstlpair<T_Key, T_Value> *operator ->(void) const {
            return &((*implIt)->entry);
        }
        const_iterator &operator++(void) {
            ++implIt;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator t(implIt);
            ++implIt;
            return t;
        }
        const_iterator &operator--(void) {
            --implIt;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator t(implIt);
            --implIt;
            return t;
        }
        zfbool operator == (ZF_IN const const_iterator &it) const {
            return implIt == it.implIt;
        }
        zfbool operator != (ZF_IN const const_iterator &it) const {
            return implIt != it.implIt;
        }
    public:
        const_iterator(void) : implIt() {}
        const_iterator(ZF_IN typename ArrayType::const_iterator const &implIt) : implIt(implIt) {}
        const_iterator(ZF_IN const const_iterator &it) : implIt(it.implIt) {}
        const_iterator(ZF_IN const iterator &it) : implIt(it.implIt) {}
    private:
    private:
        typename ArrayType::const_iterator implIt;
        friend zfclassFwd zfstlordermap<T_Key, T_Value, T_Hash, T_Equal>;
    };

public:
    void swap(ZF_IN_OUT zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) {
        map.swap(ref.map);
        arr.swap(ref.arr);
    }
    zfstlsize size(void) const {return map.size();}
    zfbool empty(void) const {return map.empty();}
    iterator find(ZF_IN const T_Key &key) {
        typename MapType::iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            return iterator(mapIt->second->arrIt);
        }
        else {
            return iterator(arr.end());
        }
    }
    const_iterator find(ZF_IN const T_Key &key) const {
        typename MapType::const_iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            return const_iterator(mapIt->second->arrIt);
        }
        else {
            return const_iterator(arr.end());
        }
    }
    iterator begin(void) {
        if(!map.empty()) {
            return iterator((*(arr.begin()))->arrIt);
        }
        else {
            return iterator(arr.end());
        }
    }
    const_iterator begin(void) const {
        if(!map.empty()) {
            return const_iterator((*(arr.begin()))->arrIt);
        }
        else {
            return const_iterator(arr.end());
        }
    }
    iterator end(void) {
        return iterator(arr.end());
    }
    const_iterator end(void) const {
        return const_iterator(arr.end());
    }
    zfstlpair<iterator, zfbool> insert(ZF_IN const zfstlpair<T_Key, T_Value> &entry) {
        typename MapType::iterator mapIt = map.find(&(entry.first));
        if(mapIt == map.end()) {
            Item *item = zfpoolNew(Item, entry);
            item->mapIt = map.insert(zfstlpair<const T_Key *, Item *>(&(item->entry.first), item)).first;
            arr.push_back(item);
            --(item->arrIt = arr.end());
            return zfstlpair<iterator, zfbool>(iterator(item->arrIt), zftrue);
        }
        else {
            Item *item = mapIt->second;
            item->entry.second = entry.second;
            arr.erase(item->arrIt);
            arr.push_back(item);
            --(item->arrIt = arr.end());
            return zfstlpair<iterator, zfbool>(iterator(item->arrIt), zffalse);
        }
    }
    iterator erase(ZF_IN iterator it) {
        if(it.implIt != arr.end()) {
            Item *item = *(it.implIt);
            map.erase(item->mapIt);
            iterator ret(arr.erase(it.implIt));
            zfpoolDelete(item);
            return ret;
        }
        else {
            return iterator(arr.end());
        }
    }
    zfstlsize erase(ZF_IN const T_Key &key) {
        typename MapType::iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            Item *item = mapIt->second;
            map.erase(mapIt);
            arr.erase(item->arrIt);
            zfpoolDelete(item);
            return 1;
        }
        else {
            return 0;
        }
    }
    void clear(void) {
        ArrayType tmp;
        arr.swap(tmp);
        map.clear();
        for(typename ArrayType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            zfpoolDelete(*it);
        }
    }
    void reserve(ZF_IN zfstlsize n) {
        map.reserve(n);
    }

    // ============================================================
    // order map spec
public:
    iterator update(ZF_IN iterator it) {
        if(it.implIt != arr.end()) {
            Item *item = *(it.implIt);
            arr.erase(item->arrIt);
            arr.push_back(item);
            --(item->arrIt = arr.end());
            return iterator(item->arrIt);
        }
        else {
            return iterator(arr.end());
        }
    }
    iterator update(ZF_IN const T_Key &key) {
        typename MapType::iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            Item *item = mapIt->second;
            arr.erase(item->arrIt);
            arr.push_back(item);
            --(item->arrIt = arr.end());
            return iterator(item->arrIt);
        }
        else {
            return iterator(arr.end());
        }
    }

public:
    zfstlordermap(void) : map(), arr() {}
private:
    zfstlordermap(ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &);
    zfstlordermap &operator = (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &);
    zfbool operator == (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &) const;
    zfbool operator != (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &) const;

private:
    MapType map;
    ArrayType arr;
};
/** @endcond */

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Key, typename T_Value, typename T_Hash = zfstlhash<T_Key>, typename T_Equal = zfstlequalto<T_Key> >
class zfimplordermap : public zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> {
private:
    zfclassNotPOD _Iter : zfextend zfiter::Impl {
    public:
        typename zfimplordermap<T_Key, T_Value, T_Hash, T_Equal>::iterator it;
        typename zfimplordermap<T_Key, T_Value, T_Hash, T_Equal>::iterator end;
    public:
        zfoverride
        virtual zfbool valid(void) {
            return it != end;
        }
        zfoverride
        virtual void next(void) {
            ++it;
        }
        zfoverride
        virtual Impl *copy(void) {
            _Iter *ret = zfpoolNew(_Iter);
            ret->it = it;
            ret->end = end;
            return ret;
        }
        zfoverride
        virtual void destroy(void) {
            zfpoolDelete(this);
        }
        zfoverride
        virtual zfbool isEqual(ZF_IN Impl *d) {
            _Iter *t = (_Iter *)d;
            return it == t->it;
        }
    };
public:
    zffinal inline zfiter iter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->begin();
        impl->end = this->end();
        return zfiter(impl);
    }

    zffinal inline zfiter iterFind(ZF_IN T_Key const &key) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->find(key);
        impl->end = this->end();
        return zfiter(impl);
    }

    zffinal inline T_Key const &iterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    zffinal inline T_Value &iterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }

    zffinal inline void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN T_Value const &newValue
            ) {
        it.impl<_Iter *>()->it->second = newValue;
    }
    zffinal inline void iterRemove(ZF_IN_OUT zfiter &it) {
        this->erase((it.impl<_Iter *>()->it)++);
    }

    zffinal inline void iterAdd(
            ZF_IN T_Key const &key
            , ZF_IN T_Value const &value
            ) {
        this->insert(zfstlpair<T_Key, T_Value>(key, value));
    }

    // ============================================================
    // order map spec
public:
    zfiter iterUpdate(ZF_IN_OUT zfiter &it) {
        if(it) {
            _Iter *impl = it.impl<_Iter *>();
            impl->it = this->update(impl->it);
        }
        return it;
    }
    zfiter iterUpdate(ZF_IN const T_Key &key) {
        typename zfimplordermap<T_Key, T_Value, T_Hash, T_Equal>::iterator it = this->find(key);
        if(it != this->end()) {
            it = this->update(it);
            _Iter *impl = zfpoolNew(_Iter);
            impl->it = it;
            impl->end = this->end();
            return zfiter(impl);
        }
        else {
            return zfnull;
        }
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlordermap_h_


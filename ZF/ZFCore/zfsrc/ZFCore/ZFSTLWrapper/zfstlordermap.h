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
    typedef zfstllist<Item *> ListType;
    zfclassNotPOD Item {
    public:
        zfstlpair<T_Key, T_Value> entry;
        typename MapType::iterator mapIt;
        typename ListType::iterator listIt;
    public:
        Item(void) : entry(), mapIt(), listIt() {}
        Item(ZF_IN const zfstlpair<T_Key, T_Value> &entry) : entry(entry), mapIt(), listIt() {}
        Item(ZF_IN const T_Key &key, ZF_IN const T_Value &value) : entry(zfstlpair<T_Key, T_Value>(key, value)), mapIt(), listIt() {}
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
        iterator(ZF_IN typename ListType::iterator const &implIt) : implIt(implIt) {}
        iterator(ZF_IN const iterator &it) : implIt(it.implIt) {}
    private:
        typename ListType::iterator implIt;
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
        const_iterator(ZF_IN typename ListType::const_iterator const &implIt) : implIt(implIt) {}
        const_iterator(ZF_IN const const_iterator &it) : implIt(it.implIt) {}
        const_iterator(ZF_IN const iterator &it) : implIt(it.implIt) {}
    private:
    private:
        typename ListType::const_iterator implIt;
        friend zfclassFwd zfstlordermap<T_Key, T_Value, T_Hash, T_Equal>;
    };

public:
    void swap(ZF_IN_OUT zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) {
        map.swap(ref.map);
        list.swap(ref.list);
    }
    zfstlsize size(void) const {return map.size();}
    zfbool empty(void) const {return map.empty();}
    iterator find(ZF_IN const T_Key &key) {
        typename MapType::iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            return iterator(mapIt->second->listIt);
        }
        else {
            return iterator(list.end());
        }
    }
    const_iterator find(ZF_IN const T_Key &key) const {
        typename MapType::const_iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            return const_iterator(mapIt->second->listIt);
        }
        else {
            return const_iterator(list.end());
        }
    }
    iterator begin(void) {
        return iterator(list.begin());
    }
    const_iterator begin(void) const {
        return const_iterator(list.begin());
    }
    iterator end(void) {
        return iterator(list.end());
    }
    const_iterator end(void) const {
        return const_iterator(list.end());
    }
    zfstlpair<iterator, zfbool> insert(ZF_IN const zfstlpair<T_Key, T_Value> &entry) {
        typename MapType::iterator mapIt = map.find(&(entry.first));
        if(mapIt == map.end()) {
            Item *item = zfpoolNew(Item, entry);
            item->mapIt = map.insert(zfstlpair<const T_Key *, Item *>(&(item->entry.first), item)).first;
            list.push_back(item);
            --(item->listIt = list.end());
            return zfstlpair<iterator, zfbool>(iterator(item->listIt), zftrue);
        }
        else {
            Item *item = mapIt->second;
            item->entry.second = entry.second;
            list.erase(item->listIt);
            list.push_back(item);
            --(item->listIt = list.end());
            return zfstlpair<iterator, zfbool>(iterator(item->listIt), zffalse);
        }
    }
    T_Value &operator [] (ZF_IN T_Key const &key) {
        iterator it = this->find(key);
        if(it != this->end()) {
            return it->second;
        }
        else {
            return this->insert(zfstlpair<T_Key, T_Value>(key, T_Value())).first->second;
        }
    }
    iterator erase(ZF_IN iterator it) {
        if(it.implIt != list.end()) {
            Item *item = *(it.implIt);
            map.erase(item->mapIt);
            iterator ret(list.erase(it.implIt));
            zfpoolDelete(item);
            return ret;
        }
        else {
            return iterator(list.end());
        }
    }
    zfstlsize erase(ZF_IN const T_Key &key) {
        typename MapType::iterator mapIt = map.find(&key);
        if(mapIt != map.end()) {
            Item *item = mapIt->second;
            map.erase(mapIt);
            list.erase(item->listIt);
            zfpoolDelete(item);
            return 1;
        }
        else {
            return 0;
        }
    }
    void clear(void) {
        ListType tmp;
        list.swap(tmp);
        map.clear();
        for(typename ListType::iterator it = tmp.begin(); it != tmp.end(); ++it) {
            zfpoolDelete(*it);
        }
    }
    void reserve(ZF_IN zfstlsize n) {
        map.reserve(n);
    }

    // ============================================================
    // order map spec
public:
    iterator move(
            ZF_IN iterator from
            , ZF_IN iterator to
            ) {
        if(from.implIt != list.end() && from != to) {
            Item *item = *(from.implIt);
            list.erase(item->listIt);
            item->listIt = list.insert(to.implIt, item);
            return item->listIt;
        }
        else {
            return from;
        }
    }

public:
    zfstlordermap(void) : map(), list() {}
    zfstlordermap(ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) {
        this->operator = (ref);
    }
    zfstlordermap &operator = (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) {
        if(this == &ref) {
            return *this;
        }
        if(!ref.empty()) {
            zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> tmp;
            for(const_iterator it = ref.begin(); it != ref.end(); ++it) {
                tmp.insert(*it);
            }
            swap(tmp);
        }
        return *this;
    }
    zfbool operator == (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) const {
        if(this == &ref) {return zftrue;}
        if(this->size() != ref.size()) {return zffalse;}
        for(const_iterator it = ref.begin(); it != ref.end(); ++it) {
            const_iterator itSelf = this->find(it->first);
            if(itSelf == this->end() || it->second != itSelf->second) {
                return zffalse;
            }
        }
        return zftrue;
    }
    zfbool operator != (ZF_IN const zfstlordermap<T_Key, T_Value, T_Hash, T_Equal> &ref) const {
        return !(this->operator == (ref));
    }

private:
    MapType map;
    ListType list;
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
    inline zfiter iter(void) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->begin();
        impl->end = this->end();
        return zfiter(impl);
    }

    inline zfiter iterFind(ZF_IN T_Key const &key) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->find(key);
        impl->end = this->end();
        return zfiter(impl);
    }

    inline T_Key const &iterKey(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->first;
    }
    inline T_Value &iterValue(ZF_IN const zfiter &it) {
        return it.impl<_Iter *>()->it->second;
    }

    inline void iterValue(
            ZF_IN_OUT zfiter &it
            , ZF_IN T_Value const &newValue
            ) {
        it.impl<_Iter *>()->it->second = newValue;
    }
    inline void iterRemove(ZF_IN_OUT zfiter &it) {
        this->erase((it.impl<_Iter *>()->it)++);
    }

    inline zfiter iterAdd(
            ZF_IN T_Key const &key
            , ZF_IN T_Value const &value
            ) {
        _Iter *impl = zfpoolNew(_Iter);
        impl->it = this->insert(zfstlpair<T_Key, T_Value>(key, value)).first;
        impl->end = this->end();
        return zfiter(impl);
    }
public:
    // ============================================================
    // order map spec
    void iterMove(ZF_IN_OUT zfiter &from, ZF_IN const zfiter &to) {
        if(from) {
            _Iter *impl = from.impl<_Iter *>();
            impl->it = this->move(impl->it, to ? to.impl<_Iter *>()->it : this->end());
        }
    }
};
/** @endcond */

#endif // #ifndef _ZFI_zfstlordermap_h_


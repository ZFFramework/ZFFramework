#include "ZFImpl_sys_iOS_ZF_impl.h"

#if ZF_ENV_sys_iOS

#include <objc/runtime.h>
#include <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFImpl_sys_iOS_zfstringFromNSString(
        ZF_IN_OUT zfstring &result
        , ZF_IN NSString *nativeString
        ) {
    if(nativeString != nil) {
        const char *tmp = nativeString.UTF8String;
        if(tmp != zfnull) {
            result += tmp;
        }
    }
}
NSString *ZFImpl_sys_iOS_zfstringToNSString(ZF_IN const zfchar *s) {
    if(s == zfnull) {
        return [NSString string];
    }
    else {
        return [NSString stringWithCString:s encoding:NSUTF8StringEncoding];
    }
}

static zfbool _ZFP_ZFImpl_sys_iOS_classIsSysClass(ZF_IN Class cls) {
    return (zffalse
        || cls == [NSObject class]
        || cls == [UIResponder class]
        || cls == [UIView class]
        || cls == [UIViewController class]
        );
}
NSString *ZFImpl_sys_iOS_objectInfoNative(
        ZF_IN id nativeObject
        , ZF_IN_OPT NSArray *ignorePropertyNames /* = nil */
        ) {
    if(nativeObject == nil) {
        return @"<null>";
    }

    NSString *ret = [NSString stringWithFormat:@"<%@ %08X", [nativeObject class], (unsigned int)[nativeObject hash]];

    for(Class cls = [nativeObject class];
            cls != nil && !_ZFP_ZFImpl_sys_iOS_classIsSysClass(cls);
            cls = [cls superclass]) {
        unsigned int propertyCount = 0;
        objc_property_t *allProperty = class_copyPropertyList(cls, &propertyCount);
        for(unsigned int i = 0; i < propertyCount; ++i) {
            char *propertyAttrName = property_copyAttributeValue(allProperty[i], "V");

            BOOL skipped = (propertyAttrName == nil);
            if(propertyAttrName != nil) {free(propertyAttrName);}
            if(skipped) {
                continue;
            }

            NSString *propertyName = ZFImpl_sys_iOS_zfstringToNSString(property_getName(allProperty[i]));;
            if([ignorePropertyNames count] > 0 && [ignorePropertyNames indexOfObject:propertyName] != NSNotFound) {
                continue;
            }
            ret = [ret stringByAppendingFormat:@" \"%@\"=\"%@\"", propertyName, [nativeObject valueForKey:propertyName]];
        }
        free(allProperty);
    }

    ret = [ret stringByAppendingString:@">"];

    return ret;
}
zfstring ZFImpl_sys_iOS_objectInfo(
        ZF_IN id nativeObject
        , ZF_IN_OPT ZFCoreArray<zfstring> *ignorePropertyNames /* = zfnull */
        ) {
    NSArray *ignorePropertyNamesNative = nil;
    if(ignorePropertyNames != zfnull) {
        NSMutableArray *ignorePropertyNamesNativeTmp = [NSMutableArray new];
        for(zfindex i = 0; i < ignorePropertyNames->count(); ++i) {
            [ignorePropertyNamesNativeTmp addObject:ZFImpl_sys_iOS_zfstringToNSString(ignorePropertyNames->get(i))];
        }
        ignorePropertyNamesNative = ignorePropertyNamesNativeTmp;
    }
    NSString *tmp = ZFImpl_sys_iOS_objectInfoNative(nativeObject, ignorePropertyNamesNative);
    zfstring ret;
    ZFImpl_sys_iOS_zfstringFromNSString(ret, tmp);
    return ret;
}

// ============================================================
zfclassNotPOD _ZFP_ZFImpl_sys_iOS_NSDictionaryIter : zfextend zfiter::Impl {
public:
    NSDictionary *m;
    NSUInteger index;
public:
    zfoverride
    virtual zfbool valid(void) {
        return m && index < m.allKeys.count;
    }
    zfoverride
    virtual void next(void) {
        ++index;
        if(index >= m.allKeys.count) {
            index = NSUIntegerMax;
        }
    }
    zfoverride
    virtual Impl *copy(void) {
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *ret = zfpoolNew(_ZFP_ZFImpl_sys_iOS_NSDictionaryIter);
        ret->m = m;
        ret->index = index;
        return ret;
    }
    zfoverride
    virtual void destroy(void) {
        zfpoolDelete(this);
    }
    zfoverride
    virtual zfbool isEqual(ZF_IN Impl *d) {
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *t = (_ZFP_ZFImpl_sys_iOS_NSDictionaryIter *)d;
        return (m == t->m && (
                    index == t->index
                    || (index >= m.allKeys.count && t->index >= t->m.allKeys.count)
                    ));
    }
};

zfiter ZFImpl_sys_iOS_NSDictionaryIter(ZF_IN NSDictionary *dict) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = zfpoolNew(_ZFP_ZFImpl_sys_iOS_NSDictionaryIter);
    impl->m = dict;
    impl->index = 0;
    return zfiter(impl);
}
zfiter ZFImpl_sys_iOS_NSDictionaryIterFind(
        ZF_IN NSDictionary *dict
        , ZF_IN id key
        ) {
    NSUInteger index = [dict.allKeys indexOfObject:key];
    if(index < dict.allKeys.count) {
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = zfpoolNew(_ZFP_ZFImpl_sys_iOS_NSDictionaryIter);
        impl->m = dict;
        impl->index = index;
        return zfiter(impl);
    }
    else {
        return zfnull;
    }
}
id ZFImpl_sys_iOS_NSDictionaryIterKey(ZF_IN const zfiter &it) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = it.impl<_ZFP_ZFImpl_sys_iOS_NSDictionaryIter *>();
    return [impl->m.allKeys objectAtIndex:impl->index];
}
id ZFImpl_sys_iOS_NSDictionaryIterValue(ZF_IN const zfiter &it) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = it.impl<_ZFP_ZFImpl_sys_iOS_NSDictionaryIter *>();
    id key = [impl->m.allKeys objectAtIndex:impl->index];
    return [impl->m objectForKey:key];
}
void ZFImpl_sys_iOS_NSDictionaryIterValue(
        ZF_IN_OUT zfiter &it
        , ZF_IN id value
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = it.impl<_ZFP_ZFImpl_sys_iOS_NSDictionaryIter *>();
    if(![impl->m isKindOfClass:[NSMutableDictionary class]]) {
        ZFCoreCriticalShouldNotGoHere();
        return;
    }
    id key = [impl->m.allKeys objectAtIndex:impl->index];
    if(key != nil && value != nil) {
        [((NSMutableDictionary *)(impl->m)) setObject:value forKey:key];
    }
}
void ZFImpl_sys_iOS_NSDictionaryIterRemove(ZF_IN_OUT zfiter &it) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIter *impl = it.impl<_ZFP_ZFImpl_sys_iOS_NSDictionaryIter *>();
    if(![impl->m isKindOfClass:[NSMutableDictionary class]]) {
        ZFCoreCriticalShouldNotGoHere();
        return;
    }
    id key = [impl->m.allKeys objectAtIndex:impl->index];
    if(key != nil) {
        [((NSMutableDictionary *)impl->m) removeObjectForKey:key];
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS


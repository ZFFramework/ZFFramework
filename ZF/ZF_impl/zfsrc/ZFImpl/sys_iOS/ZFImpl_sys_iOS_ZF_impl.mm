#include "ZFImpl_sys_iOS_ZF_impl.h"

#include "ZFCore/zfstringW.h"

#if ZF_ENV_sys_iOS

#include <objc/runtime.h>
#include <UIKit/UIKit.h>

@interface _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData : NSObject
@property(nonatomic, strong) NSArray *allKeys;
@property(nonatomic, assign) NSUInteger index;
@end
@implementation _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData
@end

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

            NSString *propertyName = [NSString stringWithCString:property_getName(allProperty[i]) encoding:NSUTF8StringEncoding];
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
static void _ZFP_ZFImpl_sys_iOS_NSDictionaryIter_delete(ZF_IN void *data) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge_transfer _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)data;
    impl = nil;
}
static void *_ZFP_ZFImpl_sys_iOS_NSDictionaryIter_copy(ZF_IN void *data) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *implSrc = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)data;
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = [_ZFP_ZFImpl_sys_iOS_NSDictionaryIterData new];
    impl.allKeys = implSrc.allKeys;
    impl.index = implSrc.index;
    return (__bridge_retained void *)impl;
}

zfiterator ZFImpl_sys_iOS_NSDictionaryIter(ZF_IN NSDictionary *dict) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = [_ZFP_ZFImpl_sys_iOS_NSDictionaryIterData new];
    impl.allKeys = dict.allKeys;
    return zfiterator(
        (__bridge_retained void *)impl,
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter_delete,
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter_copy);
}
zfiterator ZFImpl_sys_iOS_NSDictionaryIterFind(
        ZF_IN NSDictionary *dict
        , ZF_IN id key
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = [_ZFP_ZFImpl_sys_iOS_NSDictionaryIterData new];
    impl.allKeys = dict.allKeys;
    impl.index = [impl.allKeys indexOfObject:key];
    return zfiterator(
        (__bridge_retained void *)impl,
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter_delete,
        _ZFP_ZFImpl_sys_iOS_NSDictionaryIter_copy);
}
zfbool ZFImpl_sys_iOS_NSDictionaryIterValid(
        ZF_IN NSDictionary *dict
        , ZF_IN const zfiterator &it
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    return impl.index < impl.allKeys.count;
}
void ZFImpl_sys_iOS_NSDictionaryIterNext(
        ZF_IN NSDictionary *dict
        , ZF_IN_OUT zfiterator &it
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    impl.index += 1;
}
id ZFImpl_sys_iOS_NSDictionaryIterKey(
        ZF_IN NSDictionary *dict
        , ZF_IN const zfiterator &it
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    return [impl.allKeys objectAtIndex:impl.index];
}
id ZFImpl_sys_iOS_NSDictionaryIterValue(
        ZF_IN NSDictionary *dict
        , ZF_IN const zfiterator &it
        ) {
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    id key = [impl.allKeys objectAtIndex:impl.index];
    return [dict objectForKey:key];
}
void ZFImpl_sys_iOS_NSDictionaryIterValue(
        ZF_IN NSDictionary *dict
        , ZF_IN_OUT zfiterator &it
        , ZF_IN id value
        ) {
    if(![dict isKindOfClass:[NSMutableDictionary class]]) {
        return;
    }
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    id key = [impl.allKeys objectAtIndex:impl.index];
    if(key != nil && value != nil) {
        [((NSMutableDictionary *)dict) setObject:value forKey:key];
    }
}
void ZFImpl_sys_iOS_NSDictionaryIterRemove(
        ZF_IN NSDictionary *dict
        , ZF_IN_OUT zfiterator &it
        ) {
    if(![dict isKindOfClass:[NSMutableDictionary class]]) {
        return;
    }
    _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *impl = (__bridge _ZFP_ZFImpl_sys_iOS_NSDictionaryIterData *)it.data();
    id key = [impl.allKeys objectAtIndex:impl.index];
    if(key != nil) {
        [((NSMutableDictionary *)dict) removeObjectForKey:key];
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS


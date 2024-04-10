#include "ZFPathType_http.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool _ZFP_ZFPathType_http_IsDir(ZF_IN const zfchar *pathData) {
    zfindex len = zfslen(pathData);
    zfblockedAlloc(ZFHttpRequest, send
            , len > 0 && pathData[len - 1] == '/' ? pathData : zfstr("%s/", pathData).cString()
            , ZFHttpMethod::e_GET
            );
    zfautoT<ZFHttpResponse *> recv = send->requestSync();
    return recv->success() && zfstringIsEqual(recv->header("Content-Type"), "text/html");
}

// ============================================================
zfclassNotPOD _ZFP_ZFPathType_http_FindData {
public:
    ZFCoreArray<zfstring> dirs;
    ZFCoreArray<zfstring> files;
    zfindex index;
};

static void _ZFP_ZFPathType_http_htmlFix(ZF_OUT zfstring &ret, ZF_IN const zfchar *src);

zfbool _ZFP_ZFPathType_http_FindNext(ZF_IN_OUT ZFFileFindData &fd);
zfbool _ZFP_ZFPathType_http_FindFirst(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        ) {
    zfindex len = zfslen(pathData);
    zfblockedAlloc(ZFHttpRequest, send
            , len > 0 && pathData[len - 1] == '/' ? pathData : zfstr("%s/", pathData).cString()
            , ZFHttpMethod::e_GET
            );
    zfautoT<ZFHttpResponse *> recv = send->requestSync();
    if(!recv->success() || !zfstringIsEqual(recv->header("Content-Type"), "text/html")) {
        return zffalse;
    }
    zfstring body;
    _ZFP_ZFPathType_http_htmlFix(body, recv->bodyText());
    ZFXml root = ZFXmlFromString(body);
    if(!root) {
        return zffalse;
    }

    zfblockedAlloc(ZFRegExp, pattern
            , "^/|^[a-z]+://|[\\?&=~]|\\./|^\\.+$"
            );

    _ZFP_ZFPathType_http_FindData *impl = zfnew(_ZFP_ZFPathType_http_FindData);
    ZFCoreArray<ZFXml> toCheck;
    toCheck.add(root);
    do {
        ZFXml item = toCheck.removeLastAndGet();
        ZFXml child = item.childLastElement();
        while(child) {
            toCheck.add(child);
            child = child.siblingPrevElement();
        }
        if(zfstringIsEqual(item.xmlName(), "a")) {
            const zfchar *url = item.attrValue("href");
            if(zfstringIsEmpty(url)) {
                continue;
            }
            if(url[0] == '.' && url[1] == '/') {
                url += 2;
                if(*url == '\0') {
                    continue;
                }
            }
            ZFRegExpResult match;
            pattern->regExpMatch(match, url);
            if(match.matched) {
                continue;
            }
            zfindex len = zfslen(url);
            if(url[len - 1] == '/') {
                if(zfstringFind(url, len - 1, '/') == zfindexMax()) {
                    impl->dirs.add(zfstring(url, len - 1));
                }
            }
            else {
                if(zfstringFind(url, '/') == zfindexMax()) {
                    impl->files.add(url);
                }
            }
        }
    } while(!toCheck.isEmpty());

    if(impl->dirs.isEmpty() && impl->files.isEmpty()) {
        zfdelete(impl);
        return zffalse;
    }
    fd.implAttach("ZFHttpFindFirst", impl);
    impl->index = 0;
    if(_ZFP_ZFPathType_http_FindNext(fd)) {
        return zftrue;
    }
    else {
        fd.implDetach();
        return zffalse;
    }
}
zfbool _ZFP_ZFPathType_http_FindNext(ZF_IN_OUT ZFFileFindData &fd) {
    _ZFP_ZFPathType_http_FindData *impl = (_ZFP_ZFPathType_http_FindData *)fd.implUserData();
    if(impl->index < impl->dirs.count()) {
        fd.impl().fileIsDir = zftrue;
        fd.impl().fileName = impl->dirs[impl->index];
        ++(impl->index);
        return zftrue;
    }
    else if(impl->index < impl->dirs.count() + impl->files.count()) {
        fd.impl().fileIsDir = zffalse;
        fd.impl().fileName = impl->files[impl->index - impl->dirs.count()];
        ++(impl->index);
        return zftrue;
    }
    else {
        return zffalse;
    }
}
void _ZFP_ZFPathType_http_FindClose(ZF_IN_OUT ZFFileFindData &fd) {
    fd.implDetach();
    _ZFP_ZFPathType_http_FindData *impl = (_ZFP_ZFPathType_http_FindData *)fd.implUserData();
    zfdelete(impl);
}

static void _ZFP_ZFPathType_http_htmlFix(ZF_OUT zfstring &ret, ZF_IN const zfchar *src) {
    // remove void elements: https://developer.mozilla.org/en-US/docs/Glossary/Void_element
    zfblockedAlloc(ZFRegExp, reg
            , "<(area|base|br|col|embed|hr|img|input|link|meta|source|track|wbr)\\b[^>]*>"
            );
    do {
        ZFRegExpResult match;
        reg->regExpMatch(match, src);
        if(!match.matched) {
            break;
        }
        ret.append(src, match.matchedRange.start);
        src += match.matchedRange.start + match.matchedRange.count;
    } while(zftrue);
    if(src != zfnull) {
        ret += src;
    }

    // convert DOCTYPE
    zfindex index = zfstringFind(ret, 32, "<!doctype");
    if(index != zfindexMax()) {
        ret.replace(index, 9 /* zfslen("<!doctype") */, "<!DOCTYPE");
    }
}

ZF_NAMESPACE_GLOBAL_END


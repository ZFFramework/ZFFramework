#include "ZFPathType_http.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFPathType_http_FindData {
public:
    ZFCoreArray<zfstring> dirs;
    ZFCoreArray<zfstring> files;
    zfindex index;
};

zfbool _ZFP_ZFPathType_http_FindNext(ZF_IN_OUT ZFFileFindData &fd);
zfbool _ZFP_ZFPathType_http_FindFirst(
        ZF_IN_OUT ZFFileFindData &fd
        , ZF_IN const zfchar *pathData
        ) {
    zfindex len = zfslen(pathData);
    zfobj<ZFHttpRequest> send(
            len > 0 && pathData[len - 1] == '/' ? pathData : zfstr("%s/", pathData).cString()
            , v_ZFHttpMethod::e_GET
            );
    zfautoT<ZFHttpResponse> recv = send->requestSync();
    if(!recv->success() || !zfstringIsEqual(recv->header("Content-Type"), "text/html")) {
        return zffalse;
    }
    const zfchar *body = recv->body();

    zfobj<ZFRegExp> pattern("(?<=<a .*href=\")([^\"]+)(?=\")");
    zfobj<ZFRegExp> ignorePattern("^/|^[a-z]+://|[\\?&=~]|\\./|^\\.+$");

    _ZFP_ZFPathType_http_FindData *impl = zfnew(_ZFP_ZFPathType_http_FindData);
    while(*body) {
        ZFRegExpResult match;
        pattern->find(match, body);
        if(!match.matched) {
            break;
        }
        const zfchar *url = body + match.matchedRange.start;
        const zfchar *urlEnd = url + match.matchedRange.count;
        body += match.matchedRange.start + match.matchedRange.count;

        if(url[0] == '.' && url[1] == '/') {
            url += 2;
            if(url >= urlEnd) {
                continue;
            }
        }
        ZFRegExpResult ignoreResult;
        ignorePattern->find(ignoreResult, url, urlEnd - url);
        if(ignoreResult.matched) {
            continue;
        }
        if(*(urlEnd - 1) == '/') {
            if(zfstringFind(url, urlEnd - url - 1, '/') == zfindexMax()) {
                zfstring result;
                ZFCoreDataDecode(result, url, urlEnd - url - 1);
                impl->dirs.add(result);
            }
        }
        else {
            if(zfstringFind(url, urlEnd - url, '/') == zfindexMax()) {
                zfstring result;
                ZFCoreDataDecode(result, url, urlEnd - url);
                impl->files.add(result);
            }
        }
    }

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
        fd.impl().isDir = zftrue;
        fd.impl().name = impl->dirs[impl->index];
        ++(impl->index);
        return zftrue;
    }
    else if(impl->index < impl->dirs.count() + impl->files.count()) {
        fd.impl().isDir = zffalse;
        fd.impl().name = impl->files[impl->index - impl->dirs.count()];
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

ZF_NAMESPACE_GLOBAL_END


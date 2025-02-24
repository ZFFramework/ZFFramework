#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void *_ZFP_zfmallocZero(zfindex size) {
    void *ret = zfmalloc(size);
    if(ret) {
        zfmemset(ret, 0, size);
    }
    return ret;
}
void *_ZFP_zfreallocZero(void *oldPtr, zfindex newSize, zfindex oldSize) {
    void *ret = zfrealloc(oldPtr, newSize);
    if(ret && newSize > oldSize) {
        zfmemset(((zfbyte *)ret) + oldSize, 0, newSize - oldSize);
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

// ============================================================
#if _ZFP_ZFMEM_LOG
#include "zfimplLog.h"
#define _ZFP_ZFMemLog(fmt, ...) zfimplLog("[ZFMem] " fmt, ##__VA_ARGS__)
#include "../ZFSTLWrapper/zfstlmap.h"
#include "../ZFSTLWrapper/zfstlstring.h"
#include "../ZFSTLWrapper/zfstldeque.h"
ZF_NAMESPACE_GLOBAL_BEGIN

static const char *_ZFP_ZFMEM_file(const char *file) {
    const char *fileEnd = file;
    while(*fileEnd) {++fileEnd;}
    const char *fileStart = fileEnd;
    while(fileStart > file && (*fileStart != '/' && *fileStart != '\\')) {--fileStart;}
    if(*fileStart == '/' || *fileStart == '\\') {++fileStart;}
    return fileStart;
}
static void _ZFP_ZFMEM_hint(zfstlstring &hint, const char *action, size_t size, const char *file, const char *func, int line) {
    hint += action;
    char buf[32];
    snprintf(buf, sizeof(buf), "%6u", (unsigned int)size);
    hint += buf;
    hint += " [";
    hint += _ZFP_ZFMEM_file(file);
    hint += ' ';
    hint += func;
    hint += ' ';
    int e = 10;
    int t = line;
    while(t / 10 != 0) {t /= 10; e *= 10;}
    t = line;
    while(e != 1) {
        e /= 10;
        hint += '0' + (t / e);
        t = t % e;
    }
    hint += "]";
}

static zfstlmap<void *, zfstlstring> &_ZFP_ZFMEM_d(void) {
    static zfstlmap<void *, zfstlstring> d;
    return d;
}

void _ZFP_ZFMEM_logNew(void *p, const char *action, size_t size, const char *file, const char *func, int line) {
    zfstlstring hint;
    _ZFP_ZFMEM_hint(hint, action, size, file, func, line);
    #if _ZFP_ZFMEM_LOG_VERBOSE
    _ZFP_ZFMemLog("%p %s", p, hint.c_str());
    #endif

    zfstlmap<void *, zfstlstring> &d = _ZFP_ZFMEM_d();
    d[p] = hint;
}
void _ZFP_ZFMEM_logDelete(void *p, const char *action, const char *file, const char *func, int line) {
    if(!p) {
        return;
    }

    #if _ZFP_ZFMEM_LOG_VERBOSE
    zfstlstring hint;
    _ZFP_ZFMEM_hint(hint, action, 0, file, func, line);
    _ZFP_ZFMemLog("%p %s", p, hint.c_str());
    #endif

    zfstlmap<void *, zfstlstring> &d = _ZFP_ZFMEM_d();
    d.erase(p);
}

void _ZFP_ZFMEM_printStatus(int threshold /* = 10 */) {
    zfstlmap<zfstlstring, zfint> m;
    zfstlmap<void *, zfstlstring> &d = _ZFP_ZFMEM_d();
    for(zfstlmap<void *, zfstlstring>::iterator it = d.begin(); it != d.end(); ++it) {
        ++(m[it->second]);
    }
    _ZFP_ZFMemLog("============================================================");
    zfstldeque<int> countList;
    zfstldeque<const char *> hintList;
    for(zfstlmap<zfstlstring, zfint>::iterator it = m.begin(); it != m.end(); ++it) {
        if(it->second > threshold) {
            int count = it->second;
            const char *hint = it->first.c_str();
            size_t index = 0;
            while(index < countList.size() && count < countList[index]) {++index;}
            countList.insert(countList.begin() + index, count);
            hintList.insert(hintList.begin() + index, hint);
        }
    }
    for(zfstlsize i = 0; i < countList.size(); ++i) {
        _ZFP_ZFMemLog("%8d %s", countList[i], hintList[i]);
    }
}

ZF_NAMESPACE_GLOBAL_END

#endif


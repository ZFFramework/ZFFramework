#include "ZFMemPool.h"
#include "../ZFSTLWrapper/zfstlhashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_MP_DH {
public:
    _ZFP_MP_D &A(ZF_IN zfuint size) {
        return m[size];
    }
    ~_ZFP_MP_DH(void) {
        zfstlhashmap<zfuint, _ZFP_MP_D> mTmp;
        mTmp.swap(m);
        for(zfstlhashmap<zfuint, _ZFP_MP_D>::iterator it = mTmp.begin(); it != mTmp.end(); ++it) {
            _ZFP_MP_D *d = &(it->second);
            while(d->available != zfnull) {
                void *t = d->available;
                d->available = *(void **)(d->available);
                zffree(t);
            }
        }
    }
private:
    zfstlhashmap<zfuint, _ZFP_MP_D> m;
};

_ZFP_MP_D &_ZFP_MP_A(ZF_IN zfuint size) {
    static _ZFP_MP_DH d;
    return d.A(size);
}

ZF_NAMESPACE_GLOBAL_END


#include "ZFTextTemplateRun.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTextTemplateRunFilter)
ZFOBJECT_SINGLETON_DEFINE_WITH_LEVEL(ZFTextTemplateRunFilter, instance, ZFLevelZFFrameworkNormal)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTextTemplateRunFilterDefault, ZFLevelZFFrameworkLow) {
    ZFTextTemplateRunFilter *p = ZFTextTemplateRunFilter::instance();

    p->dirContentFilter()->add(zfobj<ZFRegExp>(
                "([^/]\\.git$)"
                "|([^/]\\.svn$)"
                "|([^/]\\.hg$)"
                ));
    p->fileContentFilter()->add(zfobj<ZFRegExp>(
                "(\\.ico$)"
                "|(\\.icns$)"

                "|(\\.png$)"
                "|(\\.jpg$)"
                "|(\\.jpeg$)"
                "|(\\.gif$)"

                "|(\\.mp2$)"
                "|(\\.mp3$)"
                "|(\\.mp4$)"
                "|(\\.wav$)"
                "|(\\.ogg$)"

                "|(\\.db$)"
                "|(\\.rar$)"
                "|(\\.zip$)"
                "|(\\.7z$)"

                "|(\\.a(\\..*)*$)"
                "|(\\.o(\\..*)*$)"
                "|(\\.so(\\..*)*$)"
                "|(\\.dll(\\..*)*$)"
                "|(\\.dylib(\\..*)*$)"
                ));
}
ZF_GLOBAL_INITIALIZER_END(ZFTextTemplateRunFilterDefault)

ZF_NAMESPACE_GLOBAL_END


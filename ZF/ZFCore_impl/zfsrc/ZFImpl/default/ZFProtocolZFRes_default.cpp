#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFRes.h"

#include "ZFImpl/ZFImpl_env.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// res files must be placed at "zfres/" in the same dir of your executable

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFResImpl_default, ZFRes, v_ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_resRootPath = ZFPathForModule();
        this->_resRootPath += '/';
        #if ZF_ENV_sys_MacOS
            if(ZFFileIsDir(zfstr("%s../Resources/zfres"
                            , this->_resRootPath
                            ))) {
                this->_resRootPath += "../Resources/";
            }
        #endif
        this->_resRootPath += "zfres/";
    }

public:
    zfoverride
    virtual const zfstring &resRootPath(void) {
        return this->_resRootPath;
    }
private:
    zfstring _resRootPath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFResImpl_default)

ZF_NAMESPACE_GLOBAL_END


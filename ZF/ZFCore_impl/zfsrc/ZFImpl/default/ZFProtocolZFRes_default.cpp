#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFRes.h"

#include "ZFImpl/ZFImpl_env.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// res files must be placed at "zfres/" in the same dir of your executable

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFResImpl_default, ZFRes, ZFProtocolLevel::e_Default)
public:
    zfoverride
    virtual void protocolOnInit(void) {
        zfsuper::protocolOnInit();
        this->_resRootPath = ZFPathForModule();
        this->_resRootPath += ZFFileSeparator();
        #if ZF_ENV_sys_MacOS
            if(ZFFileIsDir(zfstr("%s..%sResources%szfres"
                            , this->_resRootPath
                            , ZFFileSeparator()
                            , ZFFileSeparator()
                            ))) {
                this->_resRootPath += "..";
                this->_resRootPath += ZFFileSeparator();
                this->_resRootPath += "Resources";
                this->_resRootPath += ZFFileSeparator();
            }
        #endif
        this->_resRootPath += "zfres";
        this->_resRootPath += ZFFileSeparator();
    }

public:
    zfoverride
    virtual const zfstring &resRootPath(void) {
        return this->_resRootPath;
    }
private:
    zfstring _resRootPath;
ZFPROTOCOL_IMPLEMENTATION_END(ZFResImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFResImpl_default)

ZF_NAMESPACE_GLOBAL_END



#undef _JNI_EXPORT
#ifdef _WIN32
    #define _JNI_EXPORT __declspec(dllexport)
#else
    #define _JNI_EXPORT __attribute__((visibility("default")))
#endif

#include "JNIUtil.h"
#include <assert.h>
#include <string>
#include <vector>

#if NEED_JNIUTIL
namespace JNIUtil {

// ============================================================
// wrapper
JNIString::JNIString(void)
: _d(new std::string())
{
}
JNIString::JNIString(JNIString const &ref)
: _d(new std::string(*(const std::string *)ref._d))
{
}
JNIString::JNIString(const char *s)
: _d(new std::string(s))
{
}
JNIString::~JNIString(void) {
    delete (std::string *)_d;
}
JNIString &JNIString::operator = (JNIString const &ref) {
    *(std::string *)_d = *(const std::string *)ref._d;
    return *this;
}
JNIString &JNIString::operator = (const char *s) {
    *(std::string *)_d = s;
    return *this;
}
const char *JNIString::c_str(void) const {
    return ((std::string *)_d)->c_str();
}
JNIString::operator const char * (void) const {
    return this->c_str();
}
JNIString &JNIString::operator += (char c) {
    *(std::string *)_d += c;
    return *this;
}
JNIString &JNIString::operator += (const char *s) {
    *(std::string *)_d += s;
    return *this;
}
void JNIString::clear(void) {
    ((std::string *)_d)->clear();
}

JNIParamTypeContainer::JNIParamTypeContainer(void)
: _d(new std::vector<JNIType>())
{
}
JNIParamTypeContainer::~JNIParamTypeContainer(void) {
    delete (std::vector<JNIType> *)_d;
}
JNIParamTypeContainer &JNIParamTypeContainer::add(const JNIType &paramType) {
    ((std::vector<JNIType> *)_d)->push_back(paramType);
    return *this;
}

// ============================================================
static JavaVM *gs_javaVM = NULL;
static jint gs_jniVersionDesired = -1;

static JNIGetJNIEnvCallback gs_JNIGetJNIEnvCallback = NULL;

// ============================================================
bool JNIInit(JavaVM *javaVM, jint version) {
    JNIEnv* jniEnv = NULL;
    if(javaVM->GetEnv((void **)&jniEnv, version) != JNI_OK
            || jniEnv == NULL
            ) {
        return false;
    }
    gs_javaVM = javaVM;
    gs_jniVersionDesired = version;
    return true;
}

bool JNIInit(JNIGetJNIEnvCallback callback) {
    if(callback == NULL) {
        return false;
    }
    gs_JNIGetJNIEnvCallback = callback;
    return true;
}

JavaVM *JNIGetJavaVM(void) {
    return gs_javaVM;
}

jint JNIGetDesiredVersion(void) {
    return gs_jniVersionDesired;
}

JNIEnv *JNIGetJNIEnv(void) {
    if(gs_JNIGetJNIEnvCallback != NULL) {
        return gs_JNIGetJNIEnvCallback();
    }
    else if(gs_javaVM != NULL) {
        JNIEnv* jniEnv;
        gs_javaVM->GetEnv((void**)&jniEnv, gs_jniVersionDesired);
        return jniEnv;
    }
    else {
        return NULL;
    }
}

void JNIConvertClassNameToClassSig(JNIString &ret, const char *className) {
    if(className == NULL || *className == '\0') {
        assert(false);
    }

    ret += 'L';
    while(*className != '\0') {
        if(*className == '.') {
            ret += '/';
        }
        else {
            ret += *className;
        }
        ++className;
    }
    ret += ';';
}

void JNIConvertClassNameFromClassSig(JNIString &ret, const char *classSig) {
    if(classSig == NULL || *classSig == '\0') {
        assert(false);
    }

    ++classSig;
    while(*classSig != '\0' && *classSig != ';') {
        if(*classSig == '/') {
            ret += '.';
        }
        else {
            ret += *classSig;
        }
        ++classSig;
    }
}

void JNIConvertClassNameForFindClass(JNIString &ret, const char *className) {
    if(className == NULL || *className == '\0') {
        assert(false);
    }

    while(*className != '\0') {
        if(*className == '.') {
            ret += '/';
        }
        else {
            ret += *className;
        }
        ++className;
    }
}

// ============================================================
#define _JNIUtil_DEFINE_STATIC_TYPE(Type) \
    const JNIType &JNIType::S_##Type(void) { \
        static JNIType v(JNIType::T_##Type); \
        return v; \
    }
_JNIUtil_DEFINE_STATIC_TYPE(boolean)
_JNIUtil_DEFINE_STATIC_TYPE(byte)
_JNIUtil_DEFINE_STATIC_TYPE(char)
_JNIUtil_DEFINE_STATIC_TYPE(short)
_JNIUtil_DEFINE_STATIC_TYPE(int)
_JNIUtil_DEFINE_STATIC_TYPE(long)
_JNIUtil_DEFINE_STATIC_TYPE(float)
_JNIUtil_DEFINE_STATIC_TYPE(double)
_JNIUtil_DEFINE_STATIC_TYPE(void)

const JNIType &JNIType::S_object_Object(void) {
    static JNIType v(JNIType::T_object, "java.lang.Object");
    return v;
}
const JNIType &JNIType::S_object_String(void) {
    static JNIType v(JNIType::T_object, "java.lang.String");
    return v;
}

class _JNITypePrivate {
public:
    JNIType::Type type;
    JNIString classNameOrArrayElementTypeId;
    JNIString typeId;
    bool needUpdateTypeId;

public:
    void updateId(void) {
        this->typeId.clear();
        switch(this->type) {
            case JNIType::T_boolean:
                this->typeId += "Z";
                break;
            case JNIType::T_byte:
                this->typeId += "B";
                break;
            case JNIType::T_char:
                this->typeId += "C";
                break;
            case JNIType::T_short:
                this->typeId += "S";
                break;
            case JNIType::T_int:
                this->typeId += "I";
                break;
            case JNIType::T_long:
                this->typeId += "J";
                break;
            case JNIType::T_float:
                this->typeId += "F";
                break;
            case JNIType::T_double:
                this->typeId += "D";
                break;
            case JNIType::T_void:
                this->typeId += "V";
                break;
            case JNIType::T_object:
                JNIConvertClassNameToClassSig(this->typeId, this->classNameOrArrayElementTypeId.c_str());
                break;
            case JNIType::T_array:
                this->typeId += '[';
                this->typeId += this->classNameOrArrayElementTypeId;
                break;
            default:
                break;
        }
    }
};

JNIType::JNIType(void) : _type(JNIType::T_boolean), _id(NULL) {
}
JNIType::JNIType(
        JNIType::Type type
        , const char *classNameOrArrayElementTypeId /* = NULL */
        ) : _type(JNIType::T_boolean), _id(NULL) {
    this->setType(type, classNameOrArrayElementTypeId);
}
JNIType::JNIType(const JNIType &ref) : _type(ref._type), _id(NULL) {
    this->operator=(ref);
}
JNIType &JNIType::operator = (const JNIType &ref) {
    if(this != &ref) {
        if(_id != NULL) {
            delete _id;
            _id = NULL;
        }
        _type = ref._type;
        if(ref._id != NULL) {
            _id = new JNIString(*ref._id);
        }
    }
    return *this;
}

void JNIType::setType(JNIType::Type type,
                      const char *classNameOrArrayElementTypeId /* = NULL */) {
    _type = type;
    switch(type) {
        case T_object:
            if(_id == NULL) {
                _id = new JNIString();
            }
            else {
                _id->clear();
            }
            JNIConvertClassNameToClassSig(*_id, classNameOrArrayElementTypeId);
            break;
        case T_array:
            if(_id == NULL) {
                _id = new JNIString();
            }
            else {
                _id->clear();
            }
            *_id += '[';
            *_id += classNameOrArrayElementTypeId;
            break;
        default:
            if(_id != NULL) {
                _id->clear();
            }
            break;
    }
}
JNIType::Type JNIType::getType(void) const {
    return _type;
}
const char *JNIType::getId(void) const {
    switch(_type) {
        case JNIType::T_boolean:
            return "Z";
        case JNIType::T_byte:
            return "B";
        case JNIType::T_char:
            return "C";
        case JNIType::T_short:
            return "S";
        case JNIType::T_int:
            return "I";
        case JNIType::T_long:
            return "J";
        case JNIType::T_float:
            return "F";
        case JNIType::T_double:
            return "D";
        case JNIType::T_void:
            return "V";
        case JNIType::T_object:
        case JNIType::T_array:
            return _id->c_str();
        default:
            return NULL;
    }
}

// ============================================================
JNIString JNIGetMethodSig(
        const JNIType &returnType
        , const JNIParamTypeContainer &paramTypeList
        ) {
    std::vector<JNIType> &p = *(std::vector<JNIType> *)paramTypeList._d;
    JNIString s;
    s += "(";
    for(std::size_t i = 0; i < p.size(); ++i) {
        const JNIType &t = p[i];
        s += t.getId();
    }
    s += ")";
    s += returnType.getId();
    return s;
}

jbyteArray _JNIConvertPointerToJNITypeAction(JNIEnv *jniEnv, void *p) {
    static unsigned int size = sizeof(void *);
    jbyte jByteBuf[32] = {0};
    memcpy(jByteBuf, &p, size);
    jbyteArray ret = jniEnv->NewByteArray(size);
    jniEnv->SetByteArrayRegion(ret, 0, size, jByteBuf);
    return ret;
}
void *_JNIConvertPointerFromJNITypeAction(JNIEnv *jniEnv, jbyteArray d) {
    if(d == NULL) {
        return NULL;
    }
    static unsigned int size = sizeof(void *);
    jbyte jByteBuf[32] = {0};
    jniEnv->GetByteArrayRegion(d, 0, size, jByteBuf);
    void *p = NULL;
    memcpy(&p, jByteBuf, size);
    return p;
}

} // namespace JNIUtil
#endif // #if NEED_JNIUTIL

#undef _JNI_EXPORT


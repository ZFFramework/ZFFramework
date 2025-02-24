
#undef _JNI_EXPORT
#ifdef _WIN32
    #define _JNI_EXPORT __declspec(dllexport)
#else
    #define _JNI_EXPORT __attribute__((visibility("default")))
#endif

#include "JNIUtil.h"
#include <cassert>
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
jsize JNIString::length(void) const {
    return (jsize)((std::string *)_d)->length();
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

void JNIParamTypeContainer::remove(jint i) {
    std::vector<JNIType> &t = *((std::vector<JNIType> *)_d);
    t.erase(t.begin() + (size_t)i);
}
void JNIParamTypeContainer::removeAll(void) {
    ((std::vector<JNIType> *)_d)->clear();
}
jsize JNIParamTypeContainer::count(void) const {
    return (jsize)((std::vector<JNIType> *)_d)->size();
}
const JNIType &JNIParamTypeContainer::get(jint i) const {
    return (*(std::vector<JNIType> *)_d)[i];
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
const JNIType &JNIType::S_boolean(void) {static JNIType v(JNIType::T_boolean); return v;}
const JNIType &JNIType::S_byte(void) {static JNIType v(JNIType::T_byte); return v;}
const JNIType &JNIType::S_char(void) {static JNIType v(JNIType::T_char); return v;}
const JNIType &JNIType::S_short(void) {static JNIType v(JNIType::T_short); return v;}
const JNIType &JNIType::S_int(void) {static JNIType v(JNIType::T_int); return v;}
const JNIType &JNIType::S_long(void) {static JNIType v(JNIType::T_long); return v;}
const JNIType &JNIType::S_float(void) {static JNIType v(JNIType::T_float); return v;}
const JNIType &JNIType::S_double(void) {static JNIType v(JNIType::T_double); return v;}
const JNIType &JNIType::S_void(void) {static JNIType v(JNIType::T_void); return v;}

const JNIType &JNIType::S_object_Class(void) {static JNIType v(JNIType::T_object, "java.lang.Class"); return v;}
const JNIType &JNIType::S_object_Object(void) {static JNIType v(JNIType::T_object, "java.lang.Object"); return v;}
const JNIType &JNIType::S_object_String(void) {static JNIType v(JNIType::T_object, "java.lang.String"); return v;}

const JNIType &JNIType::S_array_Class(void) {static JNIType v(JNIType::T_array, S_object_Class().getId()); return v;}
const JNIType &JNIType::S_array_Object(void) {static JNIType v(JNIType::T_array, S_object_Object().getId()); return v;}
const JNIType &JNIType::S_array_String(void) {static JNIType v(JNIType::T_array, S_object_String().getId()); return v;}

JNIType::JNIType(void) : _type(JNIType::T_boolean), _id(NULL) {
}
JNIType::JNIType(
        JNIType::Type type
        , const char *classNameOrArrayElementTypeId /* = NULL */
        ) : _type(JNIType::T_boolean), _id(NULL) {
    this->setType(type, classNameOrArrayElementTypeId);
}
JNIType::JNIType(const JNIType &ref) : _type(ref._type), _id(NULL) {
    this->operator = (ref);
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
            if(_id != NULL && classNameOrArrayElementTypeId >= _id->c_str() && classNameOrArrayElementTypeId < _id->c_str() + _id->length()) {
                JNIString tmp = classNameOrArrayElementTypeId;
                _id->clear();
                JNIConvertClassNameToClassSig(*_id, tmp);
            }
            else {
                if(_id == NULL) {
                    _id = new JNIString();
                }
                else {
                    _id->clear();
                }
                JNIConvertClassNameToClassSig(*_id, classNameOrArrayElementTypeId);
            }
            break;
        case T_array:
            if(_id != NULL && classNameOrArrayElementTypeId >= _id->c_str() && classNameOrArrayElementTypeId < _id->c_str() + _id->length()) {
                JNIString tmp = classNameOrArrayElementTypeId;
                _id->clear();
                *_id += '[';
                *_id += tmp;
            }
            else {
                if (_id == NULL) {
                    _id = new JNIString();
                } else {
                    _id->clear();
                }
                *_id += '[';
                *_id += classNameOrArrayElementTypeId;
            }
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
        s += p[i].getId();
    }
    s += ")";
    s += returnType.getId();
    return s;
}
JNIString JNIGetMethodSig(
        const JNIType &returnType
        , const JNIType *paramTypeList
        , jsize paramCount
        ) {
    JNIString s;
    s += "(";
    for(jsize i = 0; i < paramCount; ++i) {
        s += paramTypeList[i].getId();
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


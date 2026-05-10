cmake_minimum_required(VERSION 3.16)
project(ZFAppUtil)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFAppUtil.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFAppUtil)
    zfprojConfigBefore_ZFAppUtil(ZFAppUtil ZF_SRC_FILES)
endif()
add_library(ZFAppUtil SHARED ${ZF_SRC_FILES})
target_compile_options(ZFAppUtil PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFAppUtil)
set_target_properties(ZFAppUtil PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFAppUtil PUBLIC ZFLIB_ZFAppUtil=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFAppUtil)
    zfprojConfigAfter_ZFAppUtil(ZFAppUtil)
endif()

target_include_directories(ZFAppUtil PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFAppUtil PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFAppUtil PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFAppUtil PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWidget)
ZFAddLib(ZFUIExt)
ZFAddLib(ZFLua)
ZFAddLib(ZFUIWebKit)
ZFAddLib(ZFNet)


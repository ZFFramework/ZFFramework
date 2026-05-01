cmake_minimum_required(VERSION 3.16)
project(ZFUIKit)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc_ZFUIKit.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIKit)
    zfprojConfigBefore_ZFUIKit(ZFUIKit ZF_SRC_FILES)
endif()
add_library(ZFUIKit SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFUIKit)
set_target_properties(ZFUIKit PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIKit PUBLIC ZFLIB_ZFUIKit=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIKit)
    zfprojConfigAfter_ZFUIKit(ZFUIKit)
endif()

target_include_directories(ZFUIKit PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFUIKit PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIKit PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFUIKit PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)


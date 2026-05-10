cmake_minimum_required(VERSION 3.16)
project(ZFUIKit_impl)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFUIKit_impl.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIKit_impl)
    zfprojConfigBefore_ZFUIKit_impl(ZFUIKit_impl ZF_SRC_FILES)
endif()
add_library(ZFUIKit_impl SHARED ${ZF_SRC_FILES})
target_compile_options(ZFUIKit_impl PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFUIKit_impl)
set_target_properties(ZFUIKit_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIKit_impl PUBLIC ZFLIB_ZFUIKit_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIKit_impl)
    zfprojConfigAfter_ZFUIKit_impl(ZFUIKit_impl)
endif()

target_include_directories(ZFUIKit_impl PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFUIKit_impl PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIKit_impl PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFUIKit_impl PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZF_impl)


cmake_minimum_required(VERSION 3.16)
project(ZFUIWebKit_impl)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc_ZFUIWebKit_impl.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIWebKit_impl)
    zfprojConfigBefore_ZFUIWebKit_impl(ZFUIWebKit_impl ZF_SRC_FILES)
endif()
add_library(ZFUIWebKit_impl SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFUIWebKit_impl)
set_target_properties(ZFUIWebKit_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIWebKit_impl PUBLIC ZFLIB_ZFUIWebKit_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIWebKit_impl)
    zfprojConfigAfter_ZFUIWebKit_impl(ZFUIWebKit_impl)
endif()

target_include_directories(ZFUIWebKit_impl PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFUIWebKit_impl PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIWebKit_impl PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFUIWebKit_impl PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWebKit)
ZFAddLib(ZF_impl)


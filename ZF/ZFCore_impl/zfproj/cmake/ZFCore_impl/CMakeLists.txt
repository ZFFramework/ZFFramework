cmake_minimum_required(VERSION 3.16)
project(ZFCore_impl)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc_ZFCore_impl.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFCore_impl)
    zfprojConfigBefore_ZFCore_impl(ZFCore_impl ZF_SRC_FILES)
endif()
add_library(ZFCore_impl SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFCore_impl)
set_target_properties(ZFCore_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFCore_impl PUBLIC ZFLIB_ZFCore_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFCore_impl)
    zfprojConfigAfter_ZFCore_impl(ZFCore_impl)
endif()

target_include_directories(ZFCore_impl PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFCore_impl PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFCore_impl PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFCore_impl PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZF_impl)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore_impl/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFCore_impl
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore_impl/lib"
    )
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore_impl/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore_impl/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


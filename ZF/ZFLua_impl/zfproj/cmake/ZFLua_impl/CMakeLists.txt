cmake_minimum_required(VERSION 3.16)
project(ZFLua_impl)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc_ZFLua_impl.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFLua_impl)
    zfprojConfigBefore_ZFLua_impl(ZFLua_impl ZF_SRC_FILES)
endif()
add_library(ZFLua_impl SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFLua_impl)
set_target_properties(ZFLua_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFLua_impl PUBLIC ZFLIB_ZFLua_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFLua_impl)
    zfprojConfigAfter_ZFLua_impl(ZFLua_impl)
endif()

target_include_directories(ZFLua_impl PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFLua_impl PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFLua_impl PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFLua_impl PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFLua)
ZFAddLib(ZF_impl)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFLua_impl
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/lib"
    )
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


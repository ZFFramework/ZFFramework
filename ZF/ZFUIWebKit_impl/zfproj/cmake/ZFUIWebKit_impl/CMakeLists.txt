cmake_minimum_required(VERSION 3.16)
project(ZFUIWebKit_impl)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFUIWebKit_impl.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIWebKit_impl)
    zfprojConfigBefore_ZFUIWebKit_impl(ZFUIWebKit_impl ZF_SRC_FILES)
endif()
add_library(ZFUIWebKit_impl SHARED ${ZF_SRC_FILES})
target_compile_options(ZFUIWebKit_impl PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFUIWebKit_impl)
set_target_properties(ZFUIWebKit_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIWebKit_impl PUBLIC ZFLIB_ZFUIWebKit_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIWebKit_impl)
    zfprojConfigAfter_ZFUIWebKit_impl(ZFUIWebKit_impl)
endif()

target_include_directories(ZFUIWebKit_impl PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIWebKit_impl PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFUIWebKit_impl PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFUIWebKit_impl PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWebKit)
ZFAddLib(ZF_impl)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFUIWebKit_impl
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/bin/zfres")


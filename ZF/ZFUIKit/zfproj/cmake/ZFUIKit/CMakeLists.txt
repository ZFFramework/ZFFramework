cmake_minimum_required(VERSION 3.16)
project(ZFUIKit)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFUIKit.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIKit)
    zfprojConfigBefore_ZFUIKit(ZFUIKit ZF_SRC_FILES)
endif()
add_library(ZFUIKit SHARED ${ZF_SRC_FILES})
target_compile_options(ZFUIKit PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFUIKit)
set_target_properties(ZFUIKit PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIKit PUBLIC ZFLIB_ZFUIKit=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIKit)
    zfprojConfigAfter_ZFUIKit(ZFUIKit)
endif()

target_include_directories(ZFUIKit PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIKit PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFUIKit PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFUIKit PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFUIKit
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/zfres")


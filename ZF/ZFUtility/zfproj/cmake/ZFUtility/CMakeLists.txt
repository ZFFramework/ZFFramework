cmake_minimum_required(VERSION 3.16)
project(ZFUtility)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFUtility.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUtility)
    zfprojConfigBefore_ZFUtility(ZFUtility ZF_SRC_FILES)
endif()
add_library(ZFUtility SHARED ${ZF_SRC_FILES})
target_compile_options(ZFUtility PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFUtility)
set_target_properties(ZFUtility PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUtility PUBLIC ZFLIB_ZFUtility=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUtility)
    zfprojConfigAfter_ZFUtility(ZFUtility)
endif()

target_include_directories(ZFUtility PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUtility PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFUtility PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFUtility PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFUtility
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/bin/zfres")


cmake_minimum_required(VERSION 3.16)
project(ZFAlgorithm)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFAlgorithm.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFAlgorithm)
    zfprojConfigBefore_ZFAlgorithm(ZFAlgorithm ZF_SRC_FILES)
endif()
add_library(ZFAlgorithm SHARED ${ZF_SRC_FILES})
target_compile_options(ZFAlgorithm PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFAlgorithm)
set_target_properties(ZFAlgorithm PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFAlgorithm PUBLIC ZFLIB_ZFAlgorithm=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFAlgorithm)
    zfprojConfigAfter_ZFAlgorithm(ZFAlgorithm)
endif()

target_include_directories(ZFAlgorithm PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFAlgorithm PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFAlgorithm PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFAlgorithm PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFAlgorithm
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/zfres")


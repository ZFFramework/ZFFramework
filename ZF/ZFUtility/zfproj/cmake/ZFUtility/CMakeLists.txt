cmake_minimum_required(VERSION 3.16)
set(ZF_PROJ_NAME ZFUtility)
project(${ZF_PROJ_NAME})
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_${ZF_PROJ_NAME}.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigBefore_${ZF_PROJ_NAME} ${ZF_PROJ_NAME} ZF_SRC_FILES)
endif()
add_library(${ZF_PROJ_NAME} SHARED ${ZF_SRC_FILES})
zfprojCxxFlags(${ZF_PROJ_NAME})
zfprojStripFILE(${ZF_PROJ_NAME})
target_compile_definitions(${ZF_PROJ_NAME} PUBLIC ZFLIB_${ZF_PROJ_NAME}=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigAfter_${ZF_PROJ_NAME} ${ZF_PROJ_NAME})
endif()

target_include_directories(${ZF_PROJ_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(${ZF_PROJ_NAME} PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(${ZF_PROJ_NAME} PRIVATE "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(${ZF_PROJ_NAME} PRIVATE ${ZF_LIB_NAME_})
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
    TARGETS ${ZF_PROJ_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/bin/zfres")


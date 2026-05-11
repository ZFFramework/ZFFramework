cmake_minimum_required(VERSION 3.16)
project(ZFUIWidget)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFUIWidget.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIWidget)
    zfprojConfigBefore_ZFUIWidget(ZFUIWidget ZF_SRC_FILES)
endif()
add_library(ZFUIWidget SHARED ${ZF_SRC_FILES})
target_compile_options(ZFUIWidget PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFUIWidget)
set_target_properties(ZFUIWidget PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIWidget PUBLIC ZFLIB_ZFUIWidget=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIWidget)
    zfprojConfigAfter_ZFUIWidget(ZFUIWidget)
endif()

target_include_directories(ZFUIWidget PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIWidget PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFUIWidget PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFUIWidget PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFUIWidget
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )
zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${CMAKE_INSTALL_PREFIX}/zfres")


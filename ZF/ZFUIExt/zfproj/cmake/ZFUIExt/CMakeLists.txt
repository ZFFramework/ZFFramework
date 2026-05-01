cmake_minimum_required(VERSION 3.16)
project(ZFUIExt)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc_ZFUIExt.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
if(COMMAND zfprojConfigBefore_ZFUIExt)
    zfprojConfigBefore_ZFUIExt(ZFUIExt ZF_SRC_FILES)
endif()
add_library(ZFUIExt SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFUIExt)
set_target_properties(ZFUIExt PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFUIExt PUBLIC ZFLIB_ZFUIExt=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFUIExt)
    zfprojConfigAfter_ZFUIExt(ZFUIExt)
endif()

target_include_directories(ZFUIExt PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFUIExt PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")
    target_link_directories(ZFUIExt PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFUIExt PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWidget)
ZFAddLib(ZFNet)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIExt/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFUIExt
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIExt/lib"
    )
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIExt/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIExt/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


cmake_minimum_required(VERSION 3.4.1)
project(ZFAlgorithm)
set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
if(COMMAND zfprojConfigBefore_ZFAlgorithm)
    zfprojConfigBefore_ZFAlgorithm(ZFAlgorithm ZF_SRC_FILES)
endif()
add_library(ZFAlgorithm SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFAlgorithm)
set_target_properties(ZFAlgorithm PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFAlgorithm PUBLIC ZFLIB_ZFAlgorithm=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFAlgorithm)
    zfprojConfigAfter_ZFAlgorithm(ZFAlgorithm)
endif()

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZFAlgorithm PUBLIC ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

ZFAddLib(ZFCore)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(TARGETS ZFAlgorithm EXPORT ZFAlgorithm
    LIBRARY DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/lib"
    ARCHIVE DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/lib"
    RUNTIME DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/bin")
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


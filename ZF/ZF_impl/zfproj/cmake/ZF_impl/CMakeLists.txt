cmake_minimum_required(VERSION 3.4.1)
project(ZF_impl)
set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(COMMAND zfprojConfigGlobal_ZF_impl)
    zfprojConfigGlobal_ZF_impl()
endif()

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
if(COMMAND zfprojConfigBefore_ZF_impl)
    zfprojConfigBefore_ZF_impl(ZF_impl ZF_SRC_FILES)
endif()
add_library(ZF_impl SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZF_impl)
set_target_properties(ZF_impl PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZF_impl PUBLIC ZFLIB_ZF_impl=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZF_impl)
    zfprojConfigAfter_ZF_impl(ZF_impl)
endif()

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZF_impl PUBLIC ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

ZFAddLib(ZFCore)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(TARGETS ZF_impl EXPORT ZF_impl
    LIBRARY DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/lib"
    ARCHIVE DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/lib"
    RUNTIME DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/bin")
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


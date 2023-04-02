cmake_minimum_required(VERSION 3.0.0)
project(ZFLua_impl)
set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)
include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(COMMAND zfprojConfigGlobal_ZFLua_impl)
    zfprojConfigGlobal_ZFLua_impl()
endif()
message("Build with type: ${CMAKE_BUILD_TYPE}, path: ${PROJECT_SOURCE_DIR}")

include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
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

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZFLua_impl PUBLIC ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

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
install(TARGETS ZFLua_impl EXPORT ZFLua_impl
    LIBRARY DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/lib"
    ARCHIVE DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/lib"
    RUNTIME DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/bin")
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfres/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFLua_impl/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


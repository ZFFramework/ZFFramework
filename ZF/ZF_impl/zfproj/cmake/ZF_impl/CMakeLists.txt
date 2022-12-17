cmake_minimum_required(VERSION 3.17)
project(ZF_impl)
message("Build with type: ${CMAKE_BUILD_TYPE}")

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
add_library(ZF_impl STATIC ${srcfiles})
set_target_properties(ZF_impl PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} STATIC IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZF_impl INTERFACE ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

ZFAddLib(ZFCore)


install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfsrc/
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
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfres/
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZF_impl/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


cmake_minimum_required(VERSION 3.17)
project(ZFUtility)
message("Build with type: ${CMAKE_BUILD_TYPE}")

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
add_library(ZFUtility STATIC ${srcfiles})
set_target_properties(ZFUtility PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} STATIC IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZFUtility INTERFACE ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

ZFAddLib(ZFCore)


install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfsrc/
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(TARGETS ZFUtility EXPORT ZFUtility
    LIBRARY DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/lib"
    ARCHIVE DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/lib"
    RUNTIME DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/bin")
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfres/
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFUtility/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


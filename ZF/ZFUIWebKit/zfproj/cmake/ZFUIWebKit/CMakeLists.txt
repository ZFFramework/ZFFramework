cmake_minimum_required(VERSION 3.17)
project(ZFUIWebKit)
message("Build with type: ${CMAKE_BUILD_TYPE}, path: ${PROJECT_SOURCE_DIR}")

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(DEFINED zfprojConfig)
    zfprojConfig(ZFUIWebKit srcfiles)
endif()
add_library(ZFUIWebKit STATIC ${srcfiles})
set_target_properties(ZFUIWebKit PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} STATIC IMPORTED)
    file(GLOB_RECURSE libPath ${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    target_link_libraries(ZFUIWebKit INTERFACE ${ZF_LIB_NAME_})
endfunction(ZFAddLib)

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)


install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfsrc/
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(TARGETS ZFUIWebKit EXPORT ZFUIWebKit
    LIBRARY DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/lib"
    ARCHIVE DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/lib"
    RUNTIME DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/bin")
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../../../zfres/
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/zfres"
    PATTERN "ZF_PUT_RES_FILES_HERE" EXCLUDE
    )

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIWebKit/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )

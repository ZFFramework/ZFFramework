cmake_minimum_required(VERSION 3.17)
project(ZFFramework_test)
message("Build with type: ${CMAKE_BUILD_TYPE}, path: ${PROJECT_SOURCE_DIR}")

set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")

execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/release/release_cmake.sh")

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c
    ${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp
    )
include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(COMMAND zfprojConfigBefore_ZFFramework_test)
    zfprojConfigBefore_ZFFramework_test(ZFFramework_test srcfiles)
endif()
add_executable(ZFFramework_test ${srcfiles})
if(COMMAND zfprojConfigAfter_ZFFramework_test)
    zfprojConfigAfter_ZFFramework_test(ZFFramework_test)
endif()
set_target_properties(ZFFramework_test PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${PROJECT_SOURCE_DIR}/../../../zfsrc)
include_directories(${ZF_ROOT_PATH}/_release/cmake/all/include)

function(ZFAddLib ZF_LIB_TARGETS_ ZF_LIB_NAME_)
    add_library(${ZF_LIB_NAME_} STATIC IMPORTED)
    file(GLOB_RECURSE libPath "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/*${ZF_LIB_NAME_}*")
    if(NOT libPath)
        message(FATAL_ERROR "unable to find lib ${ZF_LIB_NAME_}")
        return()
    endif()
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION ${libPath})
    set(${ZF_LIB_TARGETS_} ${${ZF_LIB_TARGETS_}} ${ZF_LIB_NAME_} PARENT_SCOPE)

    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/util/copy_res.bat" "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    endif()
endfunction(ZFAddLib)

ZFAddLib(ZF_LIB_TARGETS ZFCore)
ZFAddLib(ZF_LIB_TARGETS ZFAlgorithm)
ZFAddLib(ZF_LIB_TARGETS ZFUtility)
ZFAddLib(ZF_LIB_TARGETS ZFUIKit)
ZFAddLib(ZF_LIB_TARGETS ZFUIWidget)
ZFAddLib(ZF_LIB_TARGETS ZFLua)
ZFAddLib(ZF_LIB_TARGETS ZFUIWebKit)
ZFAddLib(ZF_LIB_TARGETS ZFNet)
ZFAddLib(ZF_LIB_TARGETS ZF_impl)
ZFAddLib(ZF_LIB_TARGETS ZFCore_impl)
ZFAddLib(ZF_LIB_TARGETS ZFAlgorithm_impl)
ZFAddLib(ZF_LIB_TARGETS ZFUIKit_impl)
ZFAddLib(ZF_LIB_TARGETS ZFLua_impl)
ZFAddLib(ZF_LIB_TARGETS ZFUIWebKit_impl)
ZFAddLib(ZF_LIB_TARGETS ZFNet_impl)

target_link_libraries(ZFFramework_test PRIVATE ${ZF_LIB_TARGETS})


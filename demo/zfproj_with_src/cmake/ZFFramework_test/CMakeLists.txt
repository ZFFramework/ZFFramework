cmake_minimum_required(VERSION 3.17)
project(ZFFramework_test)
message("Build with type: ${CMAKE_BUILD_TYPE}")

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../ZFFramework)

if(WIN32)
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.bat)
else()
    execute_process(COMMAND ${ZF_ROOT_PATH}/zfsetup.sh)
endif()

function(ZFAddSrc ZF_SRC_FILES_ PROJ_PATH_)
    file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
        ${PROJ_PATH_}/zfsrc/*.c
        ${PROJ_PATH_}/zfsrc/*.cpp
        ${PROJ_PATH_}/zfsrc_ext/*.c
        ${PROJ_PATH_}/zfsrc_ext/*.cpp
        )
    set(${ZF_SRC_FILES_} ${${ZF_SRC_FILES_}} ${srcfiles} PARENT_SCOPE)

    include_directories(${PROJ_PATH_}/zfsrc)

    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/util/copy_res.bat" "${PROJ_PATH_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    else()
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJ_PATH_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    endif()
endfunction(ZFAddSrc)

ZFAddSrc(ZF_SRC_FILES ${PROJECT_SOURCE_DIR}/../../..)

ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUtility)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWidget)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZF_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet_impl)

add_executable(ZFFramework_test ${ZF_SRC_FILES})
set_target_properties(ZFFramework_test PROPERTIES LINKER_LANGUAGE CXX)


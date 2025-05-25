cmake_minimum_required(VERSION 3.4.1)
project(ZFFramework_test)

set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework)
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

function(ZFAddSrc ZF_SRC_FILES_ PROJ_PATH_)
    file(GLOB_RECURSE ZF_SRC_FILES_TMP RELATIVE "${PROJECT_SOURCE_DIR}"
        "${PROJ_PATH_}/zfsrc/*.c"
        "${PROJ_PATH_}/zfsrc/*.cpp"
        "${PROJ_PATH_}/zfsrc_ext/*.c"
        "${PROJ_PATH_}/zfsrc_ext/*.cpp"
        )
    set(${ZF_SRC_FILES_} ${${ZF_SRC_FILES_}} ${ZF_SRC_FILES_TMP} PARENT_SCOPE)

    include_directories("${PROJ_PATH_}/zfsrc")
endfunction()

ZFAddSrc(ZF_SRC_FILES ${PROJECT_SOURCE_DIR}/../../../..)

ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUtility)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWidget)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIExt)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAppUtil)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZF_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit_impl)
ZFAddSrc(ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet_impl)

add_library(ZFFramework_test SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFFramework_test)
set_target_properties(ZFFramework_test PROPERTIES LINKER_LANGUAGE CXX)

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFFramework_test ${android-lib} ${log-lib})


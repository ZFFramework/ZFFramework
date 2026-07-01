cmake_minimum_required(VERSION 3.16)
set(ZF_PROJ_NAME ZFFramework_test)
project(${ZF_PROJ_NAME})
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

zfprojExec("${PROJECT_SOURCE_DIR}/../../../../zfsetup")

set(ZF_SRC_FILES ${ZF_SRC_FILES} "${PROJECT_SOURCE_DIR}/cmake_main.cpp")

function(ZFAddSrc ZF_LIB_NAMES_ LIB_NAME_ ZF_SRC_FILES_ PROJ_PATH_)
    set(${ZF_LIB_NAMES_} ${${ZF_LIB_NAMES_}} ${LIB_NAME_} PARENT_SCOPE)
    if(NOT "${ARGV4}" STREQUAL "")
        include("${ARGV4}/zfprojConfig.cmake" OPTIONAL)
    else()
        include("${PROJ_PATH_}/zfproj/cmake/${LIB_NAME_}/zfprojConfig.cmake" OPTIONAL)
    endif()
    zfprojSrcFiles(
        ZF_SRC_FILES_TMP
        "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_${LIB_NAME_}.cpp"
        "${ZF_ROOT_PATH}"
        "${PROJECT_SOURCE_DIR}"
        "${PROJ_PATH_}"
        )
    set(${ZF_SRC_FILES_} ${${ZF_SRC_FILES_}} ${ZF_SRC_FILES_TMP} PARENT_SCOPE)
    include_directories("${PROJ_PATH_}/zfsrc")
    zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${PROJ_PATH_}/zfres/" "${CMAKE_INSTALL_PREFIX}/bin/zfres")
endfunction()

ZFAddSrc(ZF_LIB_NAMES ${ZF_PROJ_NAME} ZF_SRC_FILES ${PROJECT_SOURCE_DIR}/../../.. ${PROJECT_SOURCE_DIR})

ZFAddSrc(ZF_LIB_NAMES ZFCore ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore)
ZFAddSrc(ZF_LIB_NAMES ZFAlgorithm ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm)
ZFAddSrc(ZF_LIB_NAMES ZFUtility ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUtility)
ZFAddSrc(ZF_LIB_NAMES ZFUIKit ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit)
ZFAddSrc(ZF_LIB_NAMES ZFUIWidget ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWidget)
ZFAddSrc(ZF_LIB_NAMES ZFUIExt ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIExt)
ZFAddSrc(ZF_LIB_NAMES ZFLua ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua)
ZFAddSrc(ZF_LIB_NAMES ZFUIWebKit ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit)
ZFAddSrc(ZF_LIB_NAMES ZFNet ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet)
ZFAddSrc(ZF_LIB_NAMES ZFAppUtil ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAppUtil)
ZFAddSrc(ZF_LIB_NAMES ZF_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZF_impl)
ZFAddSrc(ZF_LIB_NAMES ZFCore_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFCore_impl)
ZFAddSrc(ZF_LIB_NAMES ZFAlgorithm_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFAlgorithm_impl)
ZFAddSrc(ZF_LIB_NAMES ZFUIKit_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIKit_impl)
ZFAddSrc(ZF_LIB_NAMES ZFLua_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFLua_impl)
ZFAddSrc(ZF_LIB_NAMES ZFUIWebKit_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFUIWebKit_impl)
ZFAddSrc(ZF_LIB_NAMES ZFNet_impl ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/ZFNet_impl)

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(COMMAND zfprojConfigBefore_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigBefore_${ZF_PROJ_NAME} ${ZF_PROJ_NAME} ZF_SRC_FILES)
endif()
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    if(COMMAND zfprojConfigBefore_${LIB_NAME_})
        cmake_language(CALL zfprojConfigBefore_${LIB_NAME_} ${ZF_PROJ_NAME} ZF_SRC_FILES)
    endif()
endforeach()
add_executable(${ZF_PROJ_NAME} ${ZF_SRC_FILES})
zfprojCxxFlags(${ZF_PROJ_NAME})
zfprojStripFILE(${ZF_PROJ_NAME})
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    if(COMMAND zfprojConfigAfter_${LIB_NAME_})
        cmake_language(CALL zfprojConfigAfter_${LIB_NAME_} ${ZF_PROJ_NAME})
    endif()
endforeach()
if(COMMAND zfprojConfigAfter_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigAfter_${ZF_PROJ_NAME} ${ZF_PROJ_NAME})
endif()

# dllimport not work for app with all src, disable it explicitly
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    target_compile_definitions(${ZF_PROJ_NAME} PUBLIC ZFLIB_${LIB_NAME_}=)
endforeach()

if(APPLE)
    set_target_properties(${ZF_PROJ_NAME} PROPERTIES INSTALL_RPATH "@loader_path")
elseif(WIN32)
else()
    set_target_properties(${ZF_PROJ_NAME} PROPERTIES INSTALL_RPATH "\\\$ORIGIN;$ORIGIN")
endif()

install(
    TARGETS ${ZF_PROJ_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    )

file(GLOB AppIconPath "${PROJECT_SOURCE_DIR}/AppIcon.png")
if(NOT "${AppIconPath}" STREQUAL "")
    install(FILES ${AppIconPath} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/zfres/ZFUIKit_impl/sys_SDL")
endif()


cmake_minimum_required(VERSION 3.16)
project(ZFFramework_test)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

zfprojExec("${ZF_ROOT_PATH}/tools/release/release_cmake" 1)
include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFFramework_test.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../.."
    )
list(APPEND ZF_SRC_FILES "${PROJECT_SOURCE_DIR}/cmake_main.cpp")
if(COMMAND zfprojConfigBefore_ZFFramework_test)
    zfprojConfigBefore_ZFFramework_test(ZFFramework_test ZF_SRC_FILES)
endif()
add_executable(ZFFramework_test ${ZF_SRC_FILES})
target_compile_options(ZFFramework_test PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFFramework_test)
set_target_properties(ZFFramework_test PROPERTIES LINKER_LANGUAGE CXX)
if(COMMAND zfprojConfigAfter_ZFFramework_test)
    zfprojConfigAfter_ZFFramework_test(ZFFramework_test)
endif()

target_include_directories(ZFFramework_test PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFFramework_test PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories(ZFFramework_test PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFFramework_test PRIVATE ${ZF_LIB_NAME_})

    install(
        DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        PATTERN "*.lib" EXCLUDE
        )
    zfprojExec("${ZF_ROOT_PATH}/tools/util/copy_res" "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/zfres/" "${CMAKE_INSTALL_PREFIX}/bin/zfres")
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWidget)
ZFAddLib(ZFUIExt)
ZFAddLib(ZFLua)
ZFAddLib(ZFUIWebKit)
ZFAddLib(ZFNet)
ZFAddLib(ZFAppUtil)
ZFAddLib(ZF_impl)
ZFAddLib(ZFCore_impl)
ZFAddLib(ZFAlgorithm_impl)
ZFAddLib(ZFUIKit_impl)
ZFAddLib(ZFLua_impl)
ZFAddLib(ZFUIWebKit_impl)
ZFAddLib(ZFNet_impl)

if(APPLE)
    set_target_properties(ZFFramework_test PROPERTIES INSTALL_RPATH "@loader_path")
elseif(WIN32)
else()
    set_target_properties(ZFFramework_test PROPERTIES INSTALL_RPATH "\\\$ORIGIN;$ORIGIN")
endif()

install(
    TARGETS ZFFramework_test
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    )

file(GLOB AppIconPath "${PROJECT_SOURCE_DIR}/AppIcon.png")
if(NOT "${AppIconPath}" STREQUAL "")
    install(FILES ${AppIconPath} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/zfres/ZFUIKit_impl/sys_SDL")
endif()


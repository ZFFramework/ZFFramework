cmake_minimum_required(VERSION 3.16)
project(ZFNet)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp"
    )
if(COMMAND zfprojConfigBefore_ZFNet)
    zfprojConfigBefore_ZFNet(ZFNet ZF_SRC_FILES)
endif()
add_library(ZFNet SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFNet)
set_target_properties(ZFNet PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFNet PUBLIC ZFLIB_ZFNet=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFNet)
    zfprojConfigAfter_ZFNet(ZFNet)
endif()

target_include_directories(ZFNet PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFNet PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories(ZFNet PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFNet ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFNet/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFNet
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFNet/lib"
    )
if(WIN32)
    execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${PROJECT_SOURCE_DIR}\\..\\..\\..\\zfres\\" "${ZF_ROOT_PATH}\\_release\\cmake\\module\\ZFNet\\zfres")
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${ZF_ROOT_PATH}/_release/cmake/module/ZFNet/zfres")
endif()

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFNet/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


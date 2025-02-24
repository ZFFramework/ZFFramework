cmake_minimum_required(VERSION 3.16)
project(ZFAlgorithm)
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
if(COMMAND zfprojConfigBefore_ZFAlgorithm)
    zfprojConfigBefore_ZFAlgorithm(ZFAlgorithm ZF_SRC_FILES)
endif()
add_library(ZFAlgorithm SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFAlgorithm)
set_target_properties(ZFAlgorithm PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFAlgorithm PUBLIC ZFLIB_ZFAlgorithm=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFAlgorithm)
    zfprojConfigAfter_ZFAlgorithm(ZFAlgorithm)
endif()

target_include_directories(ZFAlgorithm PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFAlgorithm PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories(ZFAlgorithm PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFAlgorithm ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFAlgorithm
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/lib"
    )
if(WIN32)
    execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${PROJECT_SOURCE_DIR}\\..\\..\\..\\zfres\\" "${ZF_ROOT_PATH}\\_release\\cmake\\module\\ZFAlgorithm\\zfres")
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/zfres")
endif()

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFAlgorithm/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


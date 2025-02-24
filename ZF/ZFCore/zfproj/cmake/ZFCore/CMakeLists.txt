cmake_minimum_required(VERSION 3.16)
project(ZFCore)
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
if(COMMAND zfprojConfigBefore_ZFCore)
    zfprojConfigBefore_ZFCore(ZFCore ZF_SRC_FILES)
endif()
add_library(ZFCore SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFCore)
set_target_properties(ZFCore PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFCore PUBLIC ZFLIB_ZFCore=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFCore)
    zfprojConfigAfter_ZFCore(ZFCore)
endif()

target_include_directories(ZFCore PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFCore PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories(ZFCore PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFCore ${ZF_LIB_NAME_})
endfunction()


install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/../../../zfsrc/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore/include"
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
    )
install(
    TARGETS ZFCore
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore/lib"
    )
if(WIN32)
    execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${PROJECT_SOURCE_DIR}\\..\\..\\..\\zfres\\" "${ZF_ROOT_PATH}\\_release\\cmake\\module\\ZFCore\\zfres")
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJECT_SOURCE_DIR}/../../../zfres/" "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore/zfres")
endif()

install(
    DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/ZFCore/"
    DESTINATION "${ZF_ROOT_PATH}/_release/cmake/all"
    )


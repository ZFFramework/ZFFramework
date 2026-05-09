cmake_minimum_required(VERSION 3.16)
project(ZFLua)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFLua.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFLua)
    zfprojConfigBefore_ZFLua(ZFLua ZF_SRC_FILES)
endif()
add_library(ZFLua SHARED ${ZF_SRC_FILES})
zfprojStripFILE(ZFLua)
set_target_properties(ZFLua PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFLua PUBLIC ZFLIB_ZFLua=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFLua)
    zfprojConfigAfter_ZFLua(ZFLua)
endif()

target_include_directories(ZFLua PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFLua PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFLua PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFLua PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)


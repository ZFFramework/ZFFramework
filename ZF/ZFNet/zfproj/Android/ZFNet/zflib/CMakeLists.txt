cmake_minimum_required(VERSION 3.16)
project(ZFNet)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_ZFNet.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_ZFNet)
    zfprojConfigBefore_ZFNet(ZFNet ZF_SRC_FILES)
endif()
add_library(ZFNet SHARED ${ZF_SRC_FILES})
target_compile_options(ZFNet PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUG>")
zfprojStripFILE(ZFNet)
set_target_properties(ZFNet PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(ZFNet PUBLIC ZFLIB_ZFNet=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_ZFNet)
    zfprojConfigAfter_ZFNet(ZFNet)
endif()

target_include_directories(ZFNet PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(ZFNet PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFNet PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    add_library(${ZF_LIB_NAME_} SHARED IMPORTED)
    set_target_properties(${ZF_LIB_NAME_} PROPERTIES IMPORTED_LOCATION "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}/lib${ZF_LIB_NAME_}.so")
    target_link_libraries(ZFNet PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)


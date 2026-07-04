cmake_minimum_required(VERSION 3.16)
set(ZF_PROJ_NAME ZFLua_impl)
project(${ZF_PROJ_NAME})
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

zfprojSrcFiles(
    ZF_SRC_FILES
    "${CMAKE_BINARY_DIR}/zfgensrc/zfgensrc_${ZF_PROJ_NAME}.cpp"
    "${ZF_ROOT_PATH}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../../../.."
    )
if(COMMAND zfprojConfigBefore_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigBefore_${ZF_PROJ_NAME} ${ZF_PROJ_NAME} ZF_SRC_FILES)
endif()
add_library(${ZF_PROJ_NAME} SHARED ${ZF_SRC_FILES})
zfprojCxxFlags(${ZF_PROJ_NAME})
zfprojStripFILE(${ZF_PROJ_NAME})
target_compile_definitions(${ZF_PROJ_NAME} PUBLIC ZFLIB_${ZF_PROJ_NAME}=ZF_ENV_EXPORT)
if(COMMAND zfprojConfigAfter_${ZF_PROJ_NAME})
    cmake_language(CALL zfprojConfigAfter_${ZF_PROJ_NAME} ${ZF_PROJ_NAME})
endif()

target_include_directories(${ZF_PROJ_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/../../../../zfsrc")

find_library(log-lib log)
find_library(android-lib android)
target_link_libraries(${ZF_PROJ_NAME} PRIVATE ${android-lib} ${log-lib})

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(${ZF_PROJ_NAME} PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/cpp/include")
    target_link_directories(${ZF_PROJ_NAME} PRIVATE "${ZF_ROOT_PATH}/_release/Android/all/src/main/jniLibs/${ANDROID_ABI}")
    target_link_libraries(${ZF_PROJ_NAME} PRIVATE ${ZF_LIB_NAME_})
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFLua)
ZFAddLib(ZF_impl)


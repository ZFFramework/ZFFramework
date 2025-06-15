cmake_minimum_required(VERSION 3.16)
project({ZFTT_R_proj_name})

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    execute_process(COMMAND "${PROJECT_SOURCE_DIR}\\..\\..\\..\\..\\zfsetup.bat")
else()
    execute_process(COMMAND sh "${PROJECT_SOURCE_DIR}/../../../../zfsetup.sh")
endif()

set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

set(ZF_SRC_FILES ${ZF_SRC_FILES} "${PROJECT_SOURCE_DIR}/cmake_main.cpp")

function(ZFAddSrc ZF_LIB_NAMES_ LIB_NAME_ ZF_SRC_FILES_ PROJ_PATH_)
    set(${ZF_LIB_NAMES_} ${${ZF_LIB_NAMES_}} ${LIB_NAME_} PARENT_SCOPE)
    file(GLOB_RECURSE zfprojConfigPath "${PROJ_PATH_}/zfproj/cmake/${LIB_NAME_}/zfprojConfig.cmake")
    if(NOT "${zfprojConfigPath}" STREQUAL "")
        include("${zfprojConfigPath}")
    endif()

    file(GLOB_RECURSE ZF_SRC_FILES_TMP RELATIVE "${PROJECT_SOURCE_DIR}"
        "${PROJ_PATH_}/zfsrc/*.c"
        "${PROJ_PATH_}/zfsrc/*.cpp"
        "${PROJ_PATH_}/zfsrc_ext/*.c"
        "${PROJ_PATH_}/zfsrc_ext/*.cpp"
        )
    set(${ZF_SRC_FILES_} ${${ZF_SRC_FILES_}} ${ZF_SRC_FILES_TMP} PARENT_SCOPE)
    include_directories("${PROJ_PATH_}/zfsrc")
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${PROJ_PATH_}\\zfres\\" "${PROJECT_BINARY_DIR}\\zfdist\\zfres")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJ_PATH_}/zfres/" "${PROJECT_BINARY_DIR}/zfdist/zfres")
    endif()
endfunction()

ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_proj_name} ZF_SRC_FILES ${PROJECT_SOURCE_DIR}/../../..)
{ZFTT_C_lib_require_0}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_0} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_0}){ZFTT_CE}{ZFTT_C_lib_require_1}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_1} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_1}){ZFTT_CE}{ZFTT_C_lib_require_2}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_2} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_2}){ZFTT_CE}{ZFTT_C_lib_require_3}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_3} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_3}){ZFTT_CE}{ZFTT_C_lib_require_4}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_4} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_4}){ZFTT_CE}{ZFTT_C_lib_require_5}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_5} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_5}){ZFTT_CE}{ZFTT_C_lib_require_6}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_6} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_6}){ZFTT_CE}{ZFTT_C_lib_require_7}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_7} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_7}){ZFTT_CE}{ZFTT_C_lib_require_8}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_8} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_8}){ZFTT_CE}{ZFTT_C_lib_require_9}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_9} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_9}){ZFTT_CE}{ZFTT_C_lib_require_10}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_10} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_10}){ZFTT_CE}{ZFTT_C_lib_require_11}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_11} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_11}){ZFTT_CE}{ZFTT_C_lib_require_12}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_12} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_12}){ZFTT_CE}{ZFTT_C_lib_require_13}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_13} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_13}){ZFTT_CE}{ZFTT_C_lib_require_14}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_14} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_14}){ZFTT_CE}{ZFTT_C_lib_require_15}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_15} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_15}){ZFTT_CE}{ZFTT_C_lib_require_16}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_16} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_16}){ZFTT_CE}{ZFTT_C_lib_require_17}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_17} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_17}){ZFTT_CE}{ZFTT_C_lib_require_18}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_18} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_18}){ZFTT_CE}{ZFTT_C_lib_require_19}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_19} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_19}){ZFTT_CE}{ZFTT_C_lib_require_20}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_20} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_20}){ZFTT_CE}{ZFTT_C_lib_require_21}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_21} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_21}){ZFTT_CE}{ZFTT_C_lib_require_22}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_22} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_22}){ZFTT_CE}{ZFTT_C_lib_require_23}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_23} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_23}){ZFTT_CE}{ZFTT_C_lib_require_24}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_24} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_24}){ZFTT_CE}{ZFTT_C_lib_require_25}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_25} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_25}){ZFTT_CE}{ZFTT_C_lib_require_26}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_26} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_26}){ZFTT_CE}{ZFTT_C_lib_require_27}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_27} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_27}){ZFTT_CE}{ZFTT_C_lib_require_28}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_28} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_28}){ZFTT_CE}{ZFTT_C_lib_require_29}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_29} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_29}){ZFTT_CE}{ZFTT_C_lib_require_30}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_30} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_30}){ZFTT_CE}{ZFTT_C_lib_require_31}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_name_31} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_31}){ZFTT_CE}{ZFTT_C_impl_require_0}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_0} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_0}){ZFTT_CE}{ZFTT_C_impl_require_1}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_1} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_1}){ZFTT_CE}{ZFTT_C_impl_require_2}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_2} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_2}){ZFTT_CE}{ZFTT_C_impl_require_3}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_3} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_3}){ZFTT_CE}{ZFTT_C_impl_require_4}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_4} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_4}){ZFTT_CE}{ZFTT_C_impl_require_5}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_5} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_5}){ZFTT_CE}{ZFTT_C_impl_require_6}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_6} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_6}){ZFTT_CE}{ZFTT_C_impl_require_7}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_7} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_7}){ZFTT_CE}{ZFTT_C_impl_require_8}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_8} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_8}){ZFTT_CE}{ZFTT_C_impl_require_9}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_9} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_9}){ZFTT_CE}{ZFTT_C_impl_require_10}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_10} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_10}){ZFTT_CE}{ZFTT_C_impl_require_11}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_11} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_11}){ZFTT_CE}{ZFTT_C_impl_require_12}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_12} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_12}){ZFTT_CE}{ZFTT_C_impl_require_13}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_13} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_13}){ZFTT_CE}{ZFTT_C_impl_require_14}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_14} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_14}){ZFTT_CE}{ZFTT_C_impl_require_15}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_15} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_15}){ZFTT_CE}{ZFTT_C_impl_require_16}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_16} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_16}){ZFTT_CE}{ZFTT_C_impl_require_17}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_17} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_17}){ZFTT_CE}{ZFTT_C_impl_require_18}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_18} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_18}){ZFTT_CE}{ZFTT_C_impl_require_19}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_19} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_19}){ZFTT_CE}{ZFTT_C_impl_require_20}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_20} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_20}){ZFTT_CE}{ZFTT_C_impl_require_21}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_21} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_21}){ZFTT_CE}{ZFTT_C_impl_require_22}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_22} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_22}){ZFTT_CE}{ZFTT_C_impl_require_23}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_23} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_23}){ZFTT_CE}{ZFTT_C_impl_require_24}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_24} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_24}){ZFTT_CE}{ZFTT_C_impl_require_25}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_25} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_25}){ZFTT_CE}{ZFTT_C_impl_require_26}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_26} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_26}){ZFTT_CE}{ZFTT_C_impl_require_27}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_27} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_27}){ZFTT_CE}{ZFTT_C_impl_require_28}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_28} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_28}){ZFTT_CE}{ZFTT_C_impl_require_29}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_29} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_29}){ZFTT_CE}{ZFTT_C_impl_require_30}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_30} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_30}){ZFTT_CE}{ZFTT_C_impl_require_31}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_name_31} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_31}){ZFTT_CE}{ZFTT_C_lib_ext_require_0}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_0} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_0}){ZFTT_CE}{ZFTT_C_lib_ext_require_1}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_1} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_1}){ZFTT_CE}{ZFTT_C_lib_ext_require_2}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_2} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_2}){ZFTT_CE}{ZFTT_C_lib_ext_require_3}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_3} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_3}){ZFTT_CE}{ZFTT_C_lib_ext_require_4}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_4} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_4}){ZFTT_CE}{ZFTT_C_lib_ext_require_5}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_5} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_5}){ZFTT_CE}{ZFTT_C_lib_ext_require_6}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_6} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_6}){ZFTT_CE}{ZFTT_C_lib_ext_require_7}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_7} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_7}){ZFTT_CE}{ZFTT_C_lib_ext_require_8}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_8} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_8}){ZFTT_CE}{ZFTT_C_lib_ext_require_9}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_9} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_9}){ZFTT_CE}{ZFTT_C_lib_ext_require_10}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_10} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_10}){ZFTT_CE}{ZFTT_C_lib_ext_require_11}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_11} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_11}){ZFTT_CE}{ZFTT_C_lib_ext_require_12}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_12} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_12}){ZFTT_CE}{ZFTT_C_lib_ext_require_13}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_13} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_13}){ZFTT_CE}{ZFTT_C_lib_ext_require_14}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_14} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_14}){ZFTT_CE}{ZFTT_C_lib_ext_require_15}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_15} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_15}){ZFTT_CE}{ZFTT_C_lib_ext_require_16}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_16} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_16}){ZFTT_CE}{ZFTT_C_lib_ext_require_17}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_17} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_17}){ZFTT_CE}{ZFTT_C_lib_ext_require_18}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_18} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_18}){ZFTT_CE}{ZFTT_C_lib_ext_require_19}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_19} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_19}){ZFTT_CE}{ZFTT_C_lib_ext_require_20}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_20} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_20}){ZFTT_CE}{ZFTT_C_lib_ext_require_21}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_21} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_21}){ZFTT_CE}{ZFTT_C_lib_ext_require_22}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_22} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_22}){ZFTT_CE}{ZFTT_C_lib_ext_require_23}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_23} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_23}){ZFTT_CE}{ZFTT_C_lib_ext_require_24}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_24} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_24}){ZFTT_CE}{ZFTT_C_lib_ext_require_25}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_25} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_25}){ZFTT_CE}{ZFTT_C_lib_ext_require_26}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_26} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_26}){ZFTT_CE}{ZFTT_C_lib_ext_require_27}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_27} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_27}){ZFTT_CE}{ZFTT_C_lib_ext_require_28}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_28} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_28}){ZFTT_CE}{ZFTT_C_lib_ext_require_29}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_29} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_29}){ZFTT_CE}{ZFTT_C_lib_ext_require_30}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_30} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_30}){ZFTT_CE}{ZFTT_C_lib_ext_require_31}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_lib_ext_name_31} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_lib_ext_name_31}){ZFTT_CE}{ZFTT_C_impl_ext_require_0}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_0} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_0}){ZFTT_CE}{ZFTT_C_impl_ext_require_1}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_1} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_1}){ZFTT_CE}{ZFTT_C_impl_ext_require_2}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_2} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_2}){ZFTT_CE}{ZFTT_C_impl_ext_require_3}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_3} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_3}){ZFTT_CE}{ZFTT_C_impl_ext_require_4}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_4} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_4}){ZFTT_CE}{ZFTT_C_impl_ext_require_5}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_5} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_5}){ZFTT_CE}{ZFTT_C_impl_ext_require_6}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_6} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_6}){ZFTT_CE}{ZFTT_C_impl_ext_require_7}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_7} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_7}){ZFTT_CE}{ZFTT_C_impl_ext_require_8}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_8} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_8}){ZFTT_CE}{ZFTT_C_impl_ext_require_9}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_9} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_9}){ZFTT_CE}{ZFTT_C_impl_ext_require_10}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_10} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_10}){ZFTT_CE}{ZFTT_C_impl_ext_require_11}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_11} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_11}){ZFTT_CE}{ZFTT_C_impl_ext_require_12}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_12} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_12}){ZFTT_CE}{ZFTT_C_impl_ext_require_13}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_13} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_13}){ZFTT_CE}{ZFTT_C_impl_ext_require_14}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_14} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_14}){ZFTT_CE}{ZFTT_C_impl_ext_require_15}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_15} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_15}){ZFTT_CE}{ZFTT_C_impl_ext_require_16}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_16} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_16}){ZFTT_CE}{ZFTT_C_impl_ext_require_17}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_17} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_17}){ZFTT_CE}{ZFTT_C_impl_ext_require_18}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_18} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_18}){ZFTT_CE}{ZFTT_C_impl_ext_require_19}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_19} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_19}){ZFTT_CE}{ZFTT_C_impl_ext_require_20}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_20} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_20}){ZFTT_CE}{ZFTT_C_impl_ext_require_21}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_21} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_21}){ZFTT_CE}{ZFTT_C_impl_ext_require_22}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_22} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_22}){ZFTT_CE}{ZFTT_C_impl_ext_require_23}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_23} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_23}){ZFTT_CE}{ZFTT_C_impl_ext_require_24}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_24} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_24}){ZFTT_CE}{ZFTT_C_impl_ext_require_25}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_25} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_25}){ZFTT_CE}{ZFTT_C_impl_ext_require_26}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_26} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_26}){ZFTT_CE}{ZFTT_C_impl_ext_require_27}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_27} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_27}){ZFTT_CE}{ZFTT_C_impl_ext_require_28}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_28} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_28}){ZFTT_CE}{ZFTT_C_impl_ext_require_29}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_29} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_29}){ZFTT_CE}{ZFTT_C_impl_ext_require_30}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_30} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_30}){ZFTT_CE}{ZFTT_C_impl_ext_require_31}
ZFAddSrc(ZF_LIB_NAMES {ZFTT_R_impl_ext_name_31} ZF_SRC_FILES ${ZF_ROOT_PATH}/ZFModule/{ZFTT_R_impl_ext_name_31}){ZFTT_CE}

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
if(COMMAND zfprojConfigBefore_{ZFTT_R_proj_name})
    zfprojConfigBefore_{ZFTT_R_proj_name}({ZFTT_R_proj_name} ZF_SRC_FILES)
endif()
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    if(COMMAND zfprojConfigBefore_${LIB_NAME_})
        cmake_language(CALL zfprojConfigBefore_${LIB_NAME_} {ZFTT_R_proj_name} ZF_SRC_FILES)
    endif()
endforeach()
add_executable({ZFTT_R_proj_name} ${ZF_SRC_FILES})
zfprojStripFILE({ZFTT_R_proj_name})
set_target_properties({ZFTT_R_proj_name} PROPERTIES LINKER_LANGUAGE CXX)
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    if(COMMAND zfprojConfigAfter_${LIB_NAME_})
        cmake_language(CALL zfprojConfigAfter_${LIB_NAME_} {ZFTT_R_proj_name})
    endif()
endforeach()
if(COMMAND zfprojConfigAfter_{ZFTT_R_proj_name})
    zfprojConfigAfter_{ZFTT_R_proj_name}({ZFTT_R_proj_name})
endif()

# dllimport not work for app with all src, disable it explicitly
foreach(LIB_NAME_ ${ZF_LIB_NAMES})
    target_compile_definitions({ZFTT_R_proj_name} PUBLIC ZFLIB_${LIB_NAME_}=)
endforeach()

if(APPLE)
    set_target_properties({ZFTT_R_proj_name} PROPERTIES INSTALL_RPATH "@loader_path")
elseif(WIN32)
else()
    set_target_properties({ZFTT_R_proj_name} PROPERTIES INSTALL_RPATH "\\\$ORIGIN;$ORIGIN")
endif()

install(
    TARGETS {ZFTT_R_proj_name}
    DESTINATION "${PROJECT_BINARY_DIR}/zfdist"
    )

file(GLOB_RECURSE SDL_LIBS_
    "${ZF_ROOT_PATH}/_release/cmake/SDL/lib/*SDL3*.*"
    )
foreach(SDL_LIB_ ${SDL_LIBS_})
    if(NOT (SDL_LIB_ MATCHES "_release/cmake/SDL/lib/cmake|_release/cmake/SDL/lib/pkgconfig"))
        if(NOT (SDL_LIB_ MATCHES "SDL3[^/]*\\.a|SDL3[^/]*\\.lib"))
            install(FILES ${SDL_LIB_} DESTINATION "${PROJECT_BINARY_DIR}/zfdist")
        endif()
    endif()
endforeach()

file(GLOB_RECURSE SDL_LIBS_
    "${ZF_ROOT_PATH}/_release/cmake/SDL/bin/*.dll"
    )
foreach(SDL_LIB_ ${SDL_LIBS_})
    install(FILES ${SDL_LIB_} DESTINATION "${PROJECT_BINARY_DIR}/zfdist")
endforeach()


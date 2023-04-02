cmake_minimum_required(VERSION 3.0.0)
set(ZF_ROOT_PATH ${PROJECT_SOURCE_DIR}/../../../../../ZFFramework)
include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)
include("${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zfprojConfig.cmake")
if(COMMAND zfprojConfigGlobal_{ZFTT_R_proj_name})
    zfprojConfigGlobal_{ZFTT_R_proj_name}()
endif()
if(COMMAND zfprojConfigGlobal_ZF_impl)
    zfprojConfigGlobal_ZF_impl()
endif()
project({ZFTT_R_proj_name})
message("Build with type: ${CMAKE_BUILD_TYPE}, path: ${PROJECT_SOURCE_DIR}")

include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")

if(WIN32)
    execute_process(COMMAND "${ZF_ROOT_PATH}/zfsetup.bat")
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/zfsetup.sh")
endif()

function(ZFAddSrc srcfiles PROJ_PATH_)
    file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR}
        ${PROJ_PATH_}/zfsrc/*.c
        ${PROJ_PATH_}/zfsrc/*.cpp
        ${PROJ_PATH_}/zfsrc_ext/*.c
        ${PROJ_PATH_}/zfsrc_ext/*.cpp
        )
    set(${srcfiles} ${${srcfiles}} ${srcfiles} PARENT_SCOPE)

    include_directories(${PROJ_PATH_}/zfsrc)

    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/util/copy_res.bat" "${PROJ_PATH_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${PROJ_PATH_}/zfres/" "${PROJECT_BINARY_DIR}/zfres")
    endif()
endfunction(ZFAddSrc)

ZFAddSrc(srcfiles ${PROJECT_SOURCE_DIR}/../../..)
{ZFTT_C_lib_require_0}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_0}){ZFTT_CE}{ZFTT_C_lib_require_1}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_1}){ZFTT_CE}{ZFTT_C_lib_require_2}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_2}){ZFTT_CE}{ZFTT_C_lib_require_3}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_3}){ZFTT_CE}{ZFTT_C_lib_require_4}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_4}){ZFTT_CE}{ZFTT_C_lib_require_5}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_5}){ZFTT_CE}{ZFTT_C_lib_require_6}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_6}){ZFTT_CE}{ZFTT_C_lib_require_7}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_7}){ZFTT_CE}{ZFTT_C_lib_require_8}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_8}){ZFTT_CE}{ZFTT_C_lib_require_9}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_9}){ZFTT_CE}{ZFTT_C_lib_require_10}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_10}){ZFTT_CE}{ZFTT_C_lib_require_11}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_11}){ZFTT_CE}{ZFTT_C_lib_require_12}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_12}){ZFTT_CE}{ZFTT_C_lib_require_13}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_13}){ZFTT_CE}{ZFTT_C_lib_require_14}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_14}){ZFTT_CE}{ZFTT_C_lib_require_15}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_15}){ZFTT_CE}{ZFTT_C_lib_require_16}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_16}){ZFTT_CE}{ZFTT_C_lib_require_17}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_17}){ZFTT_CE}{ZFTT_C_lib_require_18}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_18}){ZFTT_CE}{ZFTT_C_lib_require_19}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_19}){ZFTT_CE}{ZFTT_C_lib_require_20}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_20}){ZFTT_CE}{ZFTT_C_lib_require_21}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_21}){ZFTT_CE}{ZFTT_C_lib_require_22}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_22}){ZFTT_CE}{ZFTT_C_lib_require_23}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_23}){ZFTT_CE}{ZFTT_C_lib_require_24}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_24}){ZFTT_CE}{ZFTT_C_lib_require_25}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_25}){ZFTT_CE}{ZFTT_C_lib_require_26}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_26}){ZFTT_CE}{ZFTT_C_lib_require_27}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_27}){ZFTT_CE}{ZFTT_C_lib_require_28}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_28}){ZFTT_CE}{ZFTT_C_lib_require_29}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_29}){ZFTT_CE}{ZFTT_C_lib_require_30}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_30}){ZFTT_CE}{ZFTT_C_lib_require_31}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_lib_name_31}){ZFTT_CE}{ZFTT_C_impl_require_0}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_0}){ZFTT_CE}{ZFTT_C_impl_require_1}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_1}){ZFTT_CE}{ZFTT_C_impl_require_2}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_2}){ZFTT_CE}{ZFTT_C_impl_require_3}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_3}){ZFTT_CE}{ZFTT_C_impl_require_4}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_4}){ZFTT_CE}{ZFTT_C_impl_require_5}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_5}){ZFTT_CE}{ZFTT_C_impl_require_6}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_6}){ZFTT_CE}{ZFTT_C_impl_require_7}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_7}){ZFTT_CE}{ZFTT_C_impl_require_8}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_8}){ZFTT_CE}{ZFTT_C_impl_require_9}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_9}){ZFTT_CE}{ZFTT_C_impl_require_10}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_10}){ZFTT_CE}{ZFTT_C_impl_require_11}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_11}){ZFTT_CE}{ZFTT_C_impl_require_12}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_12}){ZFTT_CE}{ZFTT_C_impl_require_13}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_13}){ZFTT_CE}{ZFTT_C_impl_require_14}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_14}){ZFTT_CE}{ZFTT_C_impl_require_15}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_15}){ZFTT_CE}{ZFTT_C_impl_require_16}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_16}){ZFTT_CE}{ZFTT_C_impl_require_17}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_17}){ZFTT_CE}{ZFTT_C_impl_require_18}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_18}){ZFTT_CE}{ZFTT_C_impl_require_19}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_19}){ZFTT_CE}{ZFTT_C_impl_require_20}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_20}){ZFTT_CE}{ZFTT_C_impl_require_21}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_21}){ZFTT_CE}{ZFTT_C_impl_require_22}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_22}){ZFTT_CE}{ZFTT_C_impl_require_23}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_23}){ZFTT_CE}{ZFTT_C_impl_require_24}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_24}){ZFTT_CE}{ZFTT_C_impl_require_25}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_25}){ZFTT_CE}{ZFTT_C_impl_require_26}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_26}){ZFTT_CE}{ZFTT_C_impl_require_27}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_27}){ZFTT_CE}{ZFTT_C_impl_require_28}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_28}){ZFTT_CE}{ZFTT_C_impl_require_29}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_29}){ZFTT_CE}{ZFTT_C_impl_require_30}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_30}){ZFTT_CE}{ZFTT_C_impl_require_31}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/ZF/{ZFTT_R_impl_name_31}){ZFTT_CE}{ZFTT_C_lib_ext_require_0}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_0}){ZFTT_CE}{ZFTT_C_lib_ext_require_1}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_1}){ZFTT_CE}{ZFTT_C_lib_ext_require_2}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_2}){ZFTT_CE}{ZFTT_C_lib_ext_require_3}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_3}){ZFTT_CE}{ZFTT_C_lib_ext_require_4}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_4}){ZFTT_CE}{ZFTT_C_lib_ext_require_5}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_5}){ZFTT_CE}{ZFTT_C_lib_ext_require_6}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_6}){ZFTT_CE}{ZFTT_C_lib_ext_require_7}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_7}){ZFTT_CE}{ZFTT_C_lib_ext_require_8}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_8}){ZFTT_CE}{ZFTT_C_lib_ext_require_9}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_9}){ZFTT_CE}{ZFTT_C_lib_ext_require_10}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_10}){ZFTT_CE}{ZFTT_C_lib_ext_require_11}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_11}){ZFTT_CE}{ZFTT_C_lib_ext_require_12}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_12}){ZFTT_CE}{ZFTT_C_lib_ext_require_13}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_13}){ZFTT_CE}{ZFTT_C_lib_ext_require_14}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_14}){ZFTT_CE}{ZFTT_C_lib_ext_require_15}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_15}){ZFTT_CE}{ZFTT_C_lib_ext_require_16}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_16}){ZFTT_CE}{ZFTT_C_lib_ext_require_17}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_17}){ZFTT_CE}{ZFTT_C_lib_ext_require_18}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_18}){ZFTT_CE}{ZFTT_C_lib_ext_require_19}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_19}){ZFTT_CE}{ZFTT_C_lib_ext_require_20}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_20}){ZFTT_CE}{ZFTT_C_lib_ext_require_21}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_21}){ZFTT_CE}{ZFTT_C_lib_ext_require_22}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_22}){ZFTT_CE}{ZFTT_C_lib_ext_require_23}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_23}){ZFTT_CE}{ZFTT_C_lib_ext_require_24}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_24}){ZFTT_CE}{ZFTT_C_lib_ext_require_25}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_25}){ZFTT_CE}{ZFTT_C_lib_ext_require_26}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_26}){ZFTT_CE}{ZFTT_C_lib_ext_require_27}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_27}){ZFTT_CE}{ZFTT_C_lib_ext_require_28}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_28}){ZFTT_CE}{ZFTT_C_lib_ext_require_29}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_29}){ZFTT_CE}{ZFTT_C_lib_ext_require_30}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_30}){ZFTT_CE}{ZFTT_C_lib_ext_require_31}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_lib_ext_name_31}){ZFTT_CE}{ZFTT_C_impl_ext_require_0}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_0}){ZFTT_CE}{ZFTT_C_impl_ext_require_1}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_1}){ZFTT_CE}{ZFTT_C_impl_ext_require_2}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_2}){ZFTT_CE}{ZFTT_C_impl_ext_require_3}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_3}){ZFTT_CE}{ZFTT_C_impl_ext_require_4}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_4}){ZFTT_CE}{ZFTT_C_impl_ext_require_5}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_5}){ZFTT_CE}{ZFTT_C_impl_ext_require_6}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_6}){ZFTT_CE}{ZFTT_C_impl_ext_require_7}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_7}){ZFTT_CE}{ZFTT_C_impl_ext_require_8}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_8}){ZFTT_CE}{ZFTT_C_impl_ext_require_9}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_9}){ZFTT_CE}{ZFTT_C_impl_ext_require_10}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_10}){ZFTT_CE}{ZFTT_C_impl_ext_require_11}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_11}){ZFTT_CE}{ZFTT_C_impl_ext_require_12}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_12}){ZFTT_CE}{ZFTT_C_impl_ext_require_13}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_13}){ZFTT_CE}{ZFTT_C_impl_ext_require_14}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_14}){ZFTT_CE}{ZFTT_C_impl_ext_require_15}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_15}){ZFTT_CE}{ZFTT_C_impl_ext_require_16}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_16}){ZFTT_CE}{ZFTT_C_impl_ext_require_17}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_17}){ZFTT_CE}{ZFTT_C_impl_ext_require_18}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_18}){ZFTT_CE}{ZFTT_C_impl_ext_require_19}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_19}){ZFTT_CE}{ZFTT_C_impl_ext_require_20}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_20}){ZFTT_CE}{ZFTT_C_impl_ext_require_21}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_21}){ZFTT_CE}{ZFTT_C_impl_ext_require_22}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_22}){ZFTT_CE}{ZFTT_C_impl_ext_require_23}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_23}){ZFTT_CE}{ZFTT_C_impl_ext_require_24}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_24}){ZFTT_CE}{ZFTT_C_impl_ext_require_25}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_25}){ZFTT_CE}{ZFTT_C_impl_ext_require_26}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_26}){ZFTT_CE}{ZFTT_C_impl_ext_require_27}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_27}){ZFTT_CE}{ZFTT_C_impl_ext_require_28}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_28}){ZFTT_CE}{ZFTT_C_impl_ext_require_29}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_29}){ZFTT_CE}{ZFTT_C_impl_ext_require_30}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_30}){ZFTT_CE}{ZFTT_C_impl_ext_require_31}
ZFAddSrc(srcfiles ${ZF_ROOT_PATH}/../ZFModule/ZF/{ZFTT_R_impl_ext_name_31}){ZFTT_CE}

if(COMMAND zfprojConfigBefore_{ZFTT_R_proj_name})
    zfprojConfigBefore_{ZFTT_R_proj_name}({ZFTT_R_proj_name} srcfiles)
endif()
if(COMMAND zfprojConfigBefore_ZF_impl)
    zfprojConfigBefore_ZF_impl({ZFTT_R_proj_name} srcfiles)
endif()
add_executable({ZFTT_R_proj_name} ${srcfiles})
zfprojStripFILE({ZFTT_R_proj_name})
set_target_properties({ZFTT_R_proj_name} PROPERTIES LINKER_LANGUAGE CXX)
if(COMMAND zfprojConfigAfter_ZF_impl)
    zfprojConfigAfter_ZF_impl({ZFTT_R_proj_name})
endif()
if(COMMAND zfprojConfigAfter_{ZFTT_R_proj_name})
    zfprojConfigAfter_{ZFTT_R_proj_name}({ZFTT_R_proj_name})
endif()


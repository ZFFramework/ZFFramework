cmake_minimum_required(VERSION 3.16)
project({ZFTT_R_proj_name})
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\release\\release_cmake.bat" 1)
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/release/release_cmake.sh" 1)
endif()

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/cmake_main.cpp"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp"
    )
if(COMMAND zfprojConfigBefore_{ZFTT_R_proj_name})
    zfprojConfigBefore_{ZFTT_R_proj_name}({ZFTT_R_proj_name} ZF_SRC_FILES)
endif()
add_executable({ZFTT_R_proj_name} ${ZF_SRC_FILES})
zfprojStripFILE({ZFTT_R_proj_name})
set_target_properties({ZFTT_R_proj_name} PROPERTIES LINKER_LANGUAGE CXX)
if(COMMAND zfprojConfigAfter_{ZFTT_R_proj_name})
    zfprojConfigAfter_{ZFTT_R_proj_name}({ZFTT_R_proj_name})
endif()

target_include_directories({ZFTT_R_proj_name} PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories({ZFTT_R_proj_name} PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories({ZFTT_R_proj_name} PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries({ZFTT_R_proj_name} ${ZF_LIB_NAME_})

    install(
        DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/"
        DESTINATION "${PROJECT_BINARY_DIR}/zfdist"
        PATTERN "*.lib" EXCLUDE
        )
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${ZF_ROOT_PATH}\\_release\\cmake\\module\\${ZF_LIB_NAME_}\\zfres\\" "${PROJECT_BINARY_DIR}\\zfdist\\zfres")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/zfres/" "${PROJECT_BINARY_DIR}/zfdist/zfres")
    endif()
endfunction()
{ZFTT_C_lib_require_0}
ZFAddLib({ZFTT_R_lib_name_0}){ZFTT_CE}{ZFTT_C_lib_require_1}
ZFAddLib({ZFTT_R_lib_name_1}){ZFTT_CE}{ZFTT_C_lib_require_2}
ZFAddLib({ZFTT_R_lib_name_2}){ZFTT_CE}{ZFTT_C_lib_require_3}
ZFAddLib({ZFTT_R_lib_name_3}){ZFTT_CE}{ZFTT_C_lib_require_4}
ZFAddLib({ZFTT_R_lib_name_4}){ZFTT_CE}{ZFTT_C_lib_require_5}
ZFAddLib({ZFTT_R_lib_name_5}){ZFTT_CE}{ZFTT_C_lib_require_6}
ZFAddLib({ZFTT_R_lib_name_6}){ZFTT_CE}{ZFTT_C_lib_require_7}
ZFAddLib({ZFTT_R_lib_name_7}){ZFTT_CE}{ZFTT_C_lib_require_8}
ZFAddLib({ZFTT_R_lib_name_8}){ZFTT_CE}{ZFTT_C_lib_require_9}
ZFAddLib({ZFTT_R_lib_name_9}){ZFTT_CE}{ZFTT_C_lib_require_10}
ZFAddLib({ZFTT_R_lib_name_10}){ZFTT_CE}{ZFTT_C_lib_require_11}
ZFAddLib({ZFTT_R_lib_name_11}){ZFTT_CE}{ZFTT_C_lib_require_12}
ZFAddLib({ZFTT_R_lib_name_12}){ZFTT_CE}{ZFTT_C_lib_require_13}
ZFAddLib({ZFTT_R_lib_name_13}){ZFTT_CE}{ZFTT_C_lib_require_14}
ZFAddLib({ZFTT_R_lib_name_14}){ZFTT_CE}{ZFTT_C_lib_require_15}
ZFAddLib({ZFTT_R_lib_name_15}){ZFTT_CE}{ZFTT_C_lib_require_16}
ZFAddLib({ZFTT_R_lib_name_16}){ZFTT_CE}{ZFTT_C_lib_require_17}
ZFAddLib({ZFTT_R_lib_name_17}){ZFTT_CE}{ZFTT_C_lib_require_18}
ZFAddLib({ZFTT_R_lib_name_18}){ZFTT_CE}{ZFTT_C_lib_require_19}
ZFAddLib({ZFTT_R_lib_name_19}){ZFTT_CE}{ZFTT_C_lib_require_20}
ZFAddLib({ZFTT_R_lib_name_20}){ZFTT_CE}{ZFTT_C_lib_require_21}
ZFAddLib({ZFTT_R_lib_name_21}){ZFTT_CE}{ZFTT_C_lib_require_22}
ZFAddLib({ZFTT_R_lib_name_22}){ZFTT_CE}{ZFTT_C_lib_require_23}
ZFAddLib({ZFTT_R_lib_name_23}){ZFTT_CE}{ZFTT_C_lib_require_24}
ZFAddLib({ZFTT_R_lib_name_24}){ZFTT_CE}{ZFTT_C_lib_require_25}
ZFAddLib({ZFTT_R_lib_name_25}){ZFTT_CE}{ZFTT_C_lib_require_26}
ZFAddLib({ZFTT_R_lib_name_26}){ZFTT_CE}{ZFTT_C_lib_require_27}
ZFAddLib({ZFTT_R_lib_name_27}){ZFTT_CE}{ZFTT_C_lib_require_28}
ZFAddLib({ZFTT_R_lib_name_28}){ZFTT_CE}{ZFTT_C_lib_require_29}
ZFAddLib({ZFTT_R_lib_name_29}){ZFTT_CE}{ZFTT_C_lib_require_30}
ZFAddLib({ZFTT_R_lib_name_30}){ZFTT_CE}{ZFTT_C_lib_require_31}
ZFAddLib({ZFTT_R_lib_name_31}){ZFTT_CE}{ZFTT_C_impl_require_0}
ZFAddLib({ZFTT_R_impl_name_0}){ZFTT_CE}{ZFTT_C_impl_require_1}
ZFAddLib({ZFTT_R_impl_name_1}){ZFTT_CE}{ZFTT_C_impl_require_2}
ZFAddLib({ZFTT_R_impl_name_2}){ZFTT_CE}{ZFTT_C_impl_require_3}
ZFAddLib({ZFTT_R_impl_name_3}){ZFTT_CE}{ZFTT_C_impl_require_4}
ZFAddLib({ZFTT_R_impl_name_4}){ZFTT_CE}{ZFTT_C_impl_require_5}
ZFAddLib({ZFTT_R_impl_name_5}){ZFTT_CE}{ZFTT_C_impl_require_6}
ZFAddLib({ZFTT_R_impl_name_6}){ZFTT_CE}{ZFTT_C_impl_require_7}
ZFAddLib({ZFTT_R_impl_name_7}){ZFTT_CE}{ZFTT_C_impl_require_8}
ZFAddLib({ZFTT_R_impl_name_8}){ZFTT_CE}{ZFTT_C_impl_require_9}
ZFAddLib({ZFTT_R_impl_name_9}){ZFTT_CE}{ZFTT_C_impl_require_10}
ZFAddLib({ZFTT_R_impl_name_10}){ZFTT_CE}{ZFTT_C_impl_require_11}
ZFAddLib({ZFTT_R_impl_name_11}){ZFTT_CE}{ZFTT_C_impl_require_12}
ZFAddLib({ZFTT_R_impl_name_12}){ZFTT_CE}{ZFTT_C_impl_require_13}
ZFAddLib({ZFTT_R_impl_name_13}){ZFTT_CE}{ZFTT_C_impl_require_14}
ZFAddLib({ZFTT_R_impl_name_14}){ZFTT_CE}{ZFTT_C_impl_require_15}
ZFAddLib({ZFTT_R_impl_name_15}){ZFTT_CE}{ZFTT_C_impl_require_16}
ZFAddLib({ZFTT_R_impl_name_16}){ZFTT_CE}{ZFTT_C_impl_require_17}
ZFAddLib({ZFTT_R_impl_name_17}){ZFTT_CE}{ZFTT_C_impl_require_18}
ZFAddLib({ZFTT_R_impl_name_18}){ZFTT_CE}{ZFTT_C_impl_require_19}
ZFAddLib({ZFTT_R_impl_name_19}){ZFTT_CE}{ZFTT_C_impl_require_20}
ZFAddLib({ZFTT_R_impl_name_20}){ZFTT_CE}{ZFTT_C_impl_require_21}
ZFAddLib({ZFTT_R_impl_name_21}){ZFTT_CE}{ZFTT_C_impl_require_22}
ZFAddLib({ZFTT_R_impl_name_22}){ZFTT_CE}{ZFTT_C_impl_require_23}
ZFAddLib({ZFTT_R_impl_name_23}){ZFTT_CE}{ZFTT_C_impl_require_24}
ZFAddLib({ZFTT_R_impl_name_24}){ZFTT_CE}{ZFTT_C_impl_require_25}
ZFAddLib({ZFTT_R_impl_name_25}){ZFTT_CE}{ZFTT_C_impl_require_26}
ZFAddLib({ZFTT_R_impl_name_26}){ZFTT_CE}{ZFTT_C_impl_require_27}
ZFAddLib({ZFTT_R_impl_name_27}){ZFTT_CE}{ZFTT_C_impl_require_28}
ZFAddLib({ZFTT_R_impl_name_28}){ZFTT_CE}{ZFTT_C_impl_require_29}
ZFAddLib({ZFTT_R_impl_name_29}){ZFTT_CE}{ZFTT_C_impl_require_30}
ZFAddLib({ZFTT_R_impl_name_30}){ZFTT_CE}{ZFTT_C_impl_require_31}
ZFAddLib({ZFTT_R_impl_name_31}){ZFTT_CE}{ZFTT_C_lib_ext_require_0}
ZFAddLib({ZFTT_R_lib_ext_name_0}){ZFTT_CE}{ZFTT_C_lib_ext_require_1}
ZFAddLib({ZFTT_R_lib_ext_name_1}){ZFTT_CE}{ZFTT_C_lib_ext_require_2}
ZFAddLib({ZFTT_R_lib_ext_name_2}){ZFTT_CE}{ZFTT_C_lib_ext_require_3}
ZFAddLib({ZFTT_R_lib_ext_name_3}){ZFTT_CE}{ZFTT_C_lib_ext_require_4}
ZFAddLib({ZFTT_R_lib_ext_name_4}){ZFTT_CE}{ZFTT_C_lib_ext_require_5}
ZFAddLib({ZFTT_R_lib_ext_name_5}){ZFTT_CE}{ZFTT_C_lib_ext_require_6}
ZFAddLib({ZFTT_R_lib_ext_name_6}){ZFTT_CE}{ZFTT_C_lib_ext_require_7}
ZFAddLib({ZFTT_R_lib_ext_name_7}){ZFTT_CE}{ZFTT_C_lib_ext_require_8}
ZFAddLib({ZFTT_R_lib_ext_name_8}){ZFTT_CE}{ZFTT_C_lib_ext_require_9}
ZFAddLib({ZFTT_R_lib_ext_name_9}){ZFTT_CE}{ZFTT_C_lib_ext_require_10}
ZFAddLib({ZFTT_R_lib_ext_name_10}){ZFTT_CE}{ZFTT_C_lib_ext_require_11}
ZFAddLib({ZFTT_R_lib_ext_name_11}){ZFTT_CE}{ZFTT_C_lib_ext_require_12}
ZFAddLib({ZFTT_R_lib_ext_name_12}){ZFTT_CE}{ZFTT_C_lib_ext_require_13}
ZFAddLib({ZFTT_R_lib_ext_name_13}){ZFTT_CE}{ZFTT_C_lib_ext_require_14}
ZFAddLib({ZFTT_R_lib_ext_name_14}){ZFTT_CE}{ZFTT_C_lib_ext_require_15}
ZFAddLib({ZFTT_R_lib_ext_name_15}){ZFTT_CE}{ZFTT_C_lib_ext_require_16}
ZFAddLib({ZFTT_R_lib_ext_name_16}){ZFTT_CE}{ZFTT_C_lib_ext_require_17}
ZFAddLib({ZFTT_R_lib_ext_name_17}){ZFTT_CE}{ZFTT_C_lib_ext_require_18}
ZFAddLib({ZFTT_R_lib_ext_name_18}){ZFTT_CE}{ZFTT_C_lib_ext_require_19}
ZFAddLib({ZFTT_R_lib_ext_name_19}){ZFTT_CE}{ZFTT_C_lib_ext_require_20}
ZFAddLib({ZFTT_R_lib_ext_name_20}){ZFTT_CE}{ZFTT_C_lib_ext_require_21}
ZFAddLib({ZFTT_R_lib_ext_name_21}){ZFTT_CE}{ZFTT_C_lib_ext_require_22}
ZFAddLib({ZFTT_R_lib_ext_name_22}){ZFTT_CE}{ZFTT_C_lib_ext_require_23}
ZFAddLib({ZFTT_R_lib_ext_name_23}){ZFTT_CE}{ZFTT_C_lib_ext_require_24}
ZFAddLib({ZFTT_R_lib_ext_name_24}){ZFTT_CE}{ZFTT_C_lib_ext_require_25}
ZFAddLib({ZFTT_R_lib_ext_name_25}){ZFTT_CE}{ZFTT_C_lib_ext_require_26}
ZFAddLib({ZFTT_R_lib_ext_name_26}){ZFTT_CE}{ZFTT_C_lib_ext_require_27}
ZFAddLib({ZFTT_R_lib_ext_name_27}){ZFTT_CE}{ZFTT_C_lib_ext_require_28}
ZFAddLib({ZFTT_R_lib_ext_name_28}){ZFTT_CE}{ZFTT_C_lib_ext_require_29}
ZFAddLib({ZFTT_R_lib_ext_name_29}){ZFTT_CE}{ZFTT_C_lib_ext_require_30}
ZFAddLib({ZFTT_R_lib_ext_name_30}){ZFTT_CE}{ZFTT_C_lib_ext_require_31}
ZFAddLib({ZFTT_R_lib_ext_name_31}){ZFTT_CE}{ZFTT_C_impl_ext_require_0}
ZFAddLib({ZFTT_R_impl_ext_name_0}){ZFTT_CE}{ZFTT_C_impl_ext_require_1}
ZFAddLib({ZFTT_R_impl_ext_name_1}){ZFTT_CE}{ZFTT_C_impl_ext_require_2}
ZFAddLib({ZFTT_R_impl_ext_name_2}){ZFTT_CE}{ZFTT_C_impl_ext_require_3}
ZFAddLib({ZFTT_R_impl_ext_name_3}){ZFTT_CE}{ZFTT_C_impl_ext_require_4}
ZFAddLib({ZFTT_R_impl_ext_name_4}){ZFTT_CE}{ZFTT_C_impl_ext_require_5}
ZFAddLib({ZFTT_R_impl_ext_name_5}){ZFTT_CE}{ZFTT_C_impl_ext_require_6}
ZFAddLib({ZFTT_R_impl_ext_name_6}){ZFTT_CE}{ZFTT_C_impl_ext_require_7}
ZFAddLib({ZFTT_R_impl_ext_name_7}){ZFTT_CE}{ZFTT_C_impl_ext_require_8}
ZFAddLib({ZFTT_R_impl_ext_name_8}){ZFTT_CE}{ZFTT_C_impl_ext_require_9}
ZFAddLib({ZFTT_R_impl_ext_name_9}){ZFTT_CE}{ZFTT_C_impl_ext_require_10}
ZFAddLib({ZFTT_R_impl_ext_name_10}){ZFTT_CE}{ZFTT_C_impl_ext_require_11}
ZFAddLib({ZFTT_R_impl_ext_name_11}){ZFTT_CE}{ZFTT_C_impl_ext_require_12}
ZFAddLib({ZFTT_R_impl_ext_name_12}){ZFTT_CE}{ZFTT_C_impl_ext_require_13}
ZFAddLib({ZFTT_R_impl_ext_name_13}){ZFTT_CE}{ZFTT_C_impl_ext_require_14}
ZFAddLib({ZFTT_R_impl_ext_name_14}){ZFTT_CE}{ZFTT_C_impl_ext_require_15}
ZFAddLib({ZFTT_R_impl_ext_name_15}){ZFTT_CE}{ZFTT_C_impl_ext_require_16}
ZFAddLib({ZFTT_R_impl_ext_name_16}){ZFTT_CE}{ZFTT_C_impl_ext_require_17}
ZFAddLib({ZFTT_R_impl_ext_name_17}){ZFTT_CE}{ZFTT_C_impl_ext_require_18}
ZFAddLib({ZFTT_R_impl_ext_name_18}){ZFTT_CE}{ZFTT_C_impl_ext_require_19}
ZFAddLib({ZFTT_R_impl_ext_name_19}){ZFTT_CE}{ZFTT_C_impl_ext_require_20}
ZFAddLib({ZFTT_R_impl_ext_name_20}){ZFTT_CE}{ZFTT_C_impl_ext_require_21}
ZFAddLib({ZFTT_R_impl_ext_name_21}){ZFTT_CE}{ZFTT_C_impl_ext_require_22}
ZFAddLib({ZFTT_R_impl_ext_name_22}){ZFTT_CE}{ZFTT_C_impl_ext_require_23}
ZFAddLib({ZFTT_R_impl_ext_name_23}){ZFTT_CE}{ZFTT_C_impl_ext_require_24}
ZFAddLib({ZFTT_R_impl_ext_name_24}){ZFTT_CE}{ZFTT_C_impl_ext_require_25}
ZFAddLib({ZFTT_R_impl_ext_name_25}){ZFTT_CE}{ZFTT_C_impl_ext_require_26}
ZFAddLib({ZFTT_R_impl_ext_name_26}){ZFTT_CE}{ZFTT_C_impl_ext_require_27}
ZFAddLib({ZFTT_R_impl_ext_name_27}){ZFTT_CE}{ZFTT_C_impl_ext_require_28}
ZFAddLib({ZFTT_R_impl_ext_name_28}){ZFTT_CE}{ZFTT_C_impl_ext_require_29}
ZFAddLib({ZFTT_R_impl_ext_name_29}){ZFTT_CE}{ZFTT_C_impl_ext_require_30}
ZFAddLib({ZFTT_R_impl_ext_name_30}){ZFTT_CE}{ZFTT_C_impl_ext_require_31}
ZFAddLib({ZFTT_R_impl_ext_name_31}){ZFTT_CE}

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


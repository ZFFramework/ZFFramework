
function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
    zfprojExec("${ZF_ROOT_PATH}/tools/common/zfsh" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zf3rd_setup_SDL.zfsh")
endfunction()

function(zfprojConfigAfter_ZF_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL3::SDL3)
        find_package(SDL3 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} PRIVATE SDL3::SDL3)
    endif()
endfunction()



function(zfprojConfigBefore_ZFFramework_test projName ZF_SRC_FILES)
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\common\\zfsh.bat" "${ZF_ROOT_PATH}\\ZF\\ZF_impl\\zfproj\\cmake\\ZF_impl\\zf3rd_setup_SDL.zfsh")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/zfsh.sh" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zf3rd_setup_SDL.zfsh")
    endif()
endfunction()

function(zfprojConfigAfter_ZFFramework_test projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL3::SDL3)
        find_package(SDL3 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL3::SDL3)
    endif()
endfunction()


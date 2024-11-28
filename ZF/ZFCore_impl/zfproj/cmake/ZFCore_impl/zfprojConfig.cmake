
function(zfprojConfigAfter_ZFCore_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT ZFSDL_SDL2)
        set(ZFSDL_SDL2 TRUE)
        add_subdirectory(${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL SDL)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()
endfunction(zfprojConfigAfter_ZFCore_impl)



function(zfprojConfigAfter_ZFCore_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)
    if(NOT ZFSDL_PREFIX)
        set(ZFSDL_PREFIX TRUE)
        list(APPEND CMAKE_PREFIX_PATH "${ZF_ROOT_PATH}/_tmp/SDL")
    endif()

    if(NOT ZFSDL_SDL2)
        set(ZFSDL_SDL2 TRUE)
        find_package(SDL2 REQUIRED)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()
endfunction(zfprojConfigAfter_ZFCore_impl)


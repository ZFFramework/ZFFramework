
function(zfprojConfigAfter_ZFCore_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL3::SDL3)
        find_package(SDL3 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL3::SDL3)
    endif()
endfunction()


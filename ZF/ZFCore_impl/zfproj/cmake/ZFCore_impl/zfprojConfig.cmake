
function(zfprojConfigAfter_ZFCore_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL3::SDL3)
        set(SDL_INSTALL ON CACHE BOOL "ZFSDL" FORCE)
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "ZFSDL" FORCE)
        set(SDL_UNIX_CONSOLE_BUILD ON CACHE BOOL "ZFSDL" FORCE)
        add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL" ${CMAKE_BINARY_DIR}/SDL)
        target_link_libraries(${projName} PRIVATE SDL3::SDL3)
    endif()
endfunction()


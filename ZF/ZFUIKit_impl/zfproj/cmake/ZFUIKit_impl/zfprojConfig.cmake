
function(zfprojConfigAfter_ZFUIKit_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT ZFSDL_SDL2)
        set(ZFSDL_SDL2 TRUE)
        add_subdirectory(${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL SDL)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT ZFSDL_SDL2_image)
        set(ZFSDL_SDL2_image TRUE)
        add_subdirectory(${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_image SDL_image)
        target_link_libraries(${projName} SDL2_image::SDL2_image)
    endif()

    if(NOT ZFSDL_SDL2_mixer)
        set(ZFSDL_SDL2_mixer TRUE)
        set(SDL2MIXER_FLAC OFF CACHE BOOL "ZFDisable" FORCE)
        set(SDL2MIXER_MIDI OFF CACHE BOOL "ZFDisable" FORCE)
        set(SDL2MIXER_MOD OFF CACHE BOOL "ZFDisable" FORCE)
        set(SDL2MIXER_OPUS OFF CACHE BOOL "ZFDisable" FORCE)
        set(SDL2MIXER_WAVPACK OFF CACHE BOOL "ZFDisable" FORCE)
        add_subdirectory(${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_mixer SDL_mixer)
        target_link_libraries(${projName} SDL2_mixer::SDL2_mixer)
    endif()

    if(NOT ZFSDL_SDL2_ttf)
        set(ZFSDL_SDL2_ttf TRUE)
        add_subdirectory(${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_ttf SDL_ttf)
        target_link_libraries(${projName} SDL2_ttf::SDL2_ttf)
    endif()
endfunction(zfprojConfigAfter_ZFUIKit_impl)


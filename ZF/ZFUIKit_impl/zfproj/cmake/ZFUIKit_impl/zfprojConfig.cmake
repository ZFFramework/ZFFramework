
function(zfprojConfigAfter_ZFUIKit_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)
    if(NOT ZFSDL_PREFIX)
        set(ZFSDL_PREFIX TRUE)
        list(APPEND CMAKE_PREFIX_PATH "${ZF_ROOT_PATH}/_release/cmake/SDL")
    endif()

    if(NOT ZFSDL_SDL2)
        set(ZFSDL_SDL2 TRUE)
        find_package(SDL2 REQUIRED)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT ZFSDL_SDL2_image)
        set(ZFSDL_SDL2_image TRUE)
        find_package(SDL2_image REQUIRED)
        target_link_libraries(${projName} SDL2_image::SDL2_image)
    endif()

    if(NOT ZFSDL_SDL2_mixer)
        set(ZFSDL_SDL2_mixer TRUE)
        find_package(SDL2_mixer REQUIRED)
        target_link_libraries(${projName} SDL2_mixer::SDL2_mixer)
    endif()

    if(NOT ZFSDL_SDL2_ttf)
        set(ZFSDL_SDL2_ttf TRUE)
        find_package(SDL2_ttf REQUIRED)
        target_link_libraries(${projName} SDL2_ttf::SDL2_ttf)
    endif()
endfunction(zfprojConfigAfter_ZFUIKit_impl)


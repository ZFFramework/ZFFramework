
function(zfprojConfigAfter_ZFUIKit_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL2::SDL2)
        find_package(SDL2 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT TARGET SDL2_image::SDL2_image)
        find_package(SDL2_image REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_image::SDL2_image)
    endif()

    if(NOT TARGET SDL2_mixer::SDL2_mixer)
        find_package(SDL2_mixer REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_mixer::SDL2_mixer)
    endif()

    if(NOT TARGET SDL2_ttf::SDL2_ttf)
        find_package(SDL2_ttf REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_ttf::SDL2_ttf)
    endif()
endfunction(zfprojConfigAfter_ZFUIKit_impl)


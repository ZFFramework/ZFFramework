
function(zfprojConfigGlobal_ZF_impl)
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/copy_check.bat" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/font/" "${PROJECT_BINARY_DIR}/zfres/ZF_impl/sys_SDL/font/.")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/copy_check.sh" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/font/" "${PROJECT_BINARY_DIR}/zfres/ZF_impl/sys_SDL/font/.")
    endif()
endfunction(zfprojConfigGlobal_ZF_impl)

function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
endfunction(zfprojConfigBefore_ZF_impl)

function(zfprojConfigAfter_ZF_impl projName)
    set(SDL2_SHARED OFF CACHE BOOL "build static" FORCE)
    set(INTERFACE_SDL2_SHARED OFF CACHE BOOL "build static" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "build static" FORCE)


    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL" SDL)
    set_target_properties(SDL2main PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(SDL2-static PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set(SDL2IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "build static" FORCE)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_image" SDL_image)
    set_target_properties(SDL2_image PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set(SDL2TTF_BUILD_SHARED_LIBS OFF CACHE BOOL "build static" FORCE)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_ttf" SDL_ttf)
    set_target_properties(SDL2_ttf PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set(SDL2NET_BUILD_SHARED_LIBS OFF CACHE BOOL "build static" FORCE)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_net" SDL_net)
    set_target_properties(SDL2_net PROPERTIES POSITION_INDEPENDENT_CODE ON)

    target_link_libraries(${projName} PUBLIC
        SDL2main
        SDL2-static
        SDL2_image
        SDL2_ttf
        SDL2_net
        )

    zfprojLoadAllSymbol(${projName})
endfunction(zfprojConfigAfter_ZF_impl)


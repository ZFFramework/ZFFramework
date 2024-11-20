
function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/zfsh.bat" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zf3rd_setup_SDL.zfsh" "${ZF_ROOT_PATH}")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/zfsh.sh" "${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zf3rd_setup_SDL.zfsh" "${ZF_ROOT_PATH}")
    endif()

    # https://open.oppomobile.com/new/developmentDoc/info?id=13223
    set(FONT_URL "https://openfs.oppomobile.com/open/oop/202410/18/62d51f494591f1a9040d83b597745911.zip")

    set(FONT_REPO_FILE "${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/font.zip")
    set(FONT_REPO_PATH "${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/font")
    if(NOT EXISTS ${FONT_REPO_FILE})
        if(WIN32)
            execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/file_download.bat" "${FONT_URL}" "${FONT_REPO_FILE}")
            execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/unzip.bat" "${FONT_REPO_FILE}" "${FONT_REPO_PATH}")
        else()
            execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/file_download.sh" "${FONT_URL}" "${FONT_REPO_FILE}")
            execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/unzip.sh" "${FONT_REPO_FILE}" "${FONT_REPO_PATH}")
        endif()
    endif()
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/copy_check.bat" "${FONT_REPO_PATH}" "${PROJECT_BINARY_DIR}/zfres/ZF_impl/sys_SDL/font")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/copy_check.sh" "${FONT_REPO_PATH}" "${PROJECT_BINARY_DIR}/zfres/ZF_impl/sys_SDL/font")
    endif()
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

    set(SDL2MIXER_FLAC OFF CACHE BOOL "disable sdl mixer feature" FORCE)
    set(SDL2MIXER_MIDI OFF CACHE BOOL "disable sdl mixer feature" FORCE)
    set(SDL2MIXER_MOD OFF CACHE BOOL "disable sdl mixer feature" FORCE)
    set(SDL2MIXER_OPUS OFF CACHE BOOL "disable sdl mixer feature" FORCE)
    set(SDL2MIXER_WAVPACK OFF CACHE BOOL "disable sdl mixer feature" FORCE)
    set(SDL2MIXER_BUILD_SHARED_LIBS OFF CACHE BOOL "build static" FORCE)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL_mixer" SDL_mixer)
    set_target_properties(SDL2_mixer PROPERTIES POSITION_INDEPENDENT_CODE ON)

    target_link_libraries(${projName} PUBLIC
        SDL2main
        SDL2-static
        SDL2_image
        SDL2_ttf
        SDL2_net
        SDL2_mixer
        )
endfunction(zfprojConfigAfter_ZF_impl)


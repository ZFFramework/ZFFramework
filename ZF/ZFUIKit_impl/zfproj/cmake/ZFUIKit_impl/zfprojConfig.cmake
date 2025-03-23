
function(zfprojConfigBefore_ZFUIKit_impl projName ZF_SRC_FILES)
    # https://open.oppomobile.com/new/developmentDoc/info?id=13223
    set(FONT_URL "https://openfs.oppomobile.com/open/oop/202410/18/62d51f494591f1a9040d83b597745911.zip")

    message("ZFSDL: font setup begin")
    set(FONT_REPO_FILE "${ZF_ROOT_PATH}/ZF/ZFUIKit_impl/zf3rd/_repo/font.zip")
    set(FONT_REPO_PATH "${ZF_ROOT_PATH}/ZF/ZFUIKit_impl/zf3rd/_repo/font")
    if(NOT EXISTS ${FONT_REPO_FILE})
        if(WIN32)
            execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/file_download.bat" "${FONT_URL}" "${FONT_REPO_FILE}")
            execute_process(COMMAND "${ZF_ROOT_PATH}/tools/common/unzip.bat" "${FONT_REPO_FILE}" "${FONT_REPO_PATH}")
        else()
            execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/file_download.sh" "${FONT_URL}" "${FONT_REPO_FILE}")
            execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/unzip.sh" "${FONT_REPO_FILE}" "${FONT_REPO_PATH}")
        endif()
    endif()
    if(${projName} STREQUAL "ZFUIKit_impl")
        install(
            DIRECTORY "${FONT_REPO_PATH}/"
            DESTINATION "${ZF_ROOT_PATH}/_release/cmake/module/ZFUIKit_impl/zfres/ZFUIKit_impl/sys_SDL/font"
            )
    else()
        install(
            DIRECTORY "${FONT_REPO_PATH}/"
            DESTINATION "${PROJECT_BINARY_DIR}/zfdist/zfres/ZFUIKit_impl/sys_SDL/font"
            )
    endif()
    message("ZFSDL: font setup end")
endfunction()

function(zfprojConfigAfter_ZFUIKit_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL2::SDL2)
        find_package(SDL2 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT TARGET SDL2_image::SDL2_image)
        set(SDL2IMAGE_VENDORED ON CACHE BOOL "ZFSDL use vendored lib" FORCE)
        find_package(SDL2_image REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_image::SDL2_image)
    endif()

    if(NOT TARGET SDL2_mixer::SDL2_mixer)
        set(SDL2MIXER_VENDORED ON CACHE BOOL "ZFSDL use vendored lib" FORCE)
        find_package(SDL2_mixer REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_mixer::SDL2_mixer)
    endif()

    if(NOT TARGET SDL2_ttf::SDL2_ttf)
        set(SDL2TTF_VENDORED ON CACHE BOOL "ZFSDL use vendored lib" FORCE)
        find_package(SDL2_ttf REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_ttf::SDL2_ttf)
    endif()
endfunction()


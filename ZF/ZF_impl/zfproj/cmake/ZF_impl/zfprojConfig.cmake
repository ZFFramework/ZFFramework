
function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
    # https://open.oppomobile.com/new/developmentDoc/info?id=13223
    set(FONT_URL "https://openfs.oppomobile.com/open/oop/202410/18/62d51f494591f1a9040d83b597745911.zip")

    message("SDL font setup begin")
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
    install(
        DIRECTORY "${FONT_REPO_PATH}"
        DESTINATION "${PROJECT_BINARY_DIR}/zfres/ZF_impl/sys_SDL/font"
        )
    message("SDL font setup end")
endfunction(zfprojConfigBefore_ZF_impl)

function(zfprojConfigAfter_ZF_impl projName)
    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT ZFSDL_SDL2)
        set(ZFSDL_SDL2 TRUE)
        find_package(SDL2 REQUIRED)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT ZFSDL_SDL2_ttf)
        set(ZFSDL_SDL2_ttf TRUE)
        find_package(SDL2_ttf REQUIRED)
        target_link_libraries(${projName} SDL2_ttf::SDL2_ttf)
    endif()

    if(NOT ZFSDL_SDL2_image)
        set(ZFSDL_SDL2_image TRUE)
        find_package(SDL2_image REQUIRED)
        target_link_libraries(${projName} SDL2_image::SDL2_image)
    endif()
endfunction(zfprojConfigAfter_ZF_impl)


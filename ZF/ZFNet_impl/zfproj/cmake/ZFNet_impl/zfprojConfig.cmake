
function(zfprojConfigBefore_ZFNet_impl projName ZF_SRC_FILES)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    find_package(OpenSSL MODULE)
    if(OPENSSL_FOUND)
        include_directories(${OPENSSL_INCLUDE_DIR})
        message("OpenSSL: ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    else()
        message("OpenSSL: NOT FOUND")
    endif()
endfunction()

function(zfprojConfigAfter_ZFNet_impl projName)
    find_package(OpenSSL MODULE)
    if(OPENSSL_FOUND)
        set_target_properties(${projName} PROPERTIES CXX_STANDARD 11)
        target_compile_options(${projName} PUBLIC -DZF_ENV_HTTPS=1)
        target_link_libraries(${projName} ${OPENSSL_LIBRARIES})
    endif()

    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL2::SDL2)
        find_package(SDL2 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2::SDL2)
    endif()

    if(NOT TARGET SDL2_net::SDL2_net)
        find_package(SDL2_net REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} SDL2_net::SDL2_net)
    endif()
endfunction()


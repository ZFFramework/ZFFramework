
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
    set_target_properties(${projName} PROPERTIES CXX_STANDARD 11)
    if(MSVC)
        target_compile_options(${projName} PRIVATE /Zc:__cplusplus)
    endif()

    find_package(OpenSSL MODULE)
    if(OPENSSL_FOUND)
        target_compile_options(${projName} PUBLIC -DZF_ENV_HTTPS=1)
        target_link_libraries(${projName} PRIVATE ${OPENSSL_LIBRARIES})
    endif()

    target_compile_definitions(${projName} PUBLIC ZF_ENV_FORCE_sys_SDL=1)

    if(NOT TARGET SDL3::SDL3)
        find_package(SDL3 REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} PRIVATE SDL3::SDL3)
    endif()

    if(NOT TARGET SDL3_net::SDL3_net)
        find_package(SDL3_net REQUIRED CONFIG HINTS "${ZF_ROOT_PATH}/_release/cmake/SDL" NO_DEFAULT_PATH)
        target_link_libraries(${projName} PRIVATE SDL3_net::SDL3_net)
    endif()

    # cpp-httplib require CoreFoundation and Security on mac
    if(APPLE)
        target_link_libraries(${projName}
            PRIVATE "-framework CoreFoundation"
            PRIVATE "-framework Security"
        )
    endif()
endfunction()



function(zfprojConfigBefore_ZFNet_impl projName ZF_SRC_FILES)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    find_package(OpenSSL MODULE)
    if(OPENSSL_FOUND)
        include_directories(${OPENSSL_INCLUDE_DIR})
        message("OpenSSL: ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    else()
        message("OpenSSL: NOT FOUND")
    endif()
endfunction(zfprojConfigBefore_ZFNet_impl)

function(zfprojConfigAfter_ZFNet_impl projName)
    set_target_properties(${projName} PROPERTIES CXX_STANDARD 11)

    find_package(OpenSSL MODULE)
    if(OPENSSL_FOUND)
        target_compile_options(${projName} PUBLIC -DZF_ENV_HTTPS=1)
        target_link_libraries(${projName} PRIVATE ${OPENSSL_LIBRARIES})
    endif()
endfunction(zfprojConfigAfter_ZFNet_impl)


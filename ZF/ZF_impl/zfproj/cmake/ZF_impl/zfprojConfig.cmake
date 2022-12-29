
function(zfprojConfigAfter_ZF_impl projName)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL" SDL)
    target_link_libraries(${projName} PUBLIC SDL2main SDL2-static)

    if(WIN32)
        set_target_properties(${projName} PROPERTIES
            LINK_FLAGS "/WHOLEARCHIVE"
            )
    elseif(APPLE)
        set_target_properties(${projName} PROPERTIES
            LINK_FLAGS "-Wl,-all_load"
            )
    else()
        set_target_properties(${projName} PROPERTIES
            LINK_FLAGS "-Wl,--whole-archive"
            )
        set_target_properties(${projName} PROPERTIES
            LINK_FLAGS "-Wl,--allow-multiple-definition"
            )
    endif()
endfunction(zfprojConfigAfter_ZF_impl)


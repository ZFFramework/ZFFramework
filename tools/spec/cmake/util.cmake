
function(zfprojStripFILE targetName)
    if(WIN32)
        target_compile_options(${targetName} PUBLIC "/wd4117;")
    else()
        target_compile_options(${targetName} PUBLIC "-Wno-builtin-macro-redefined")
    endif()
    get_target_property(source_files "${targetName}" SOURCES)
    foreach(sourcefile ${source_files})
        get_property(defs SOURCE "${sourcefile}" PROPERTY COMPILE_DEFINITIONS)
        get_filename_component(filename "${sourcefile}" NAME)
        list(APPEND defs "__FILE__=\"${filename}\"")
        set_property(SOURCE "${sourcefile}" PROPERTY COMPILE_DEFINITIONS ${defs})
    endforeach()
endfunction()

function(zfprojLoadAllSymbol targetName)
    if(WIN32)
        set_target_properties(${targetName} PROPERTIES
            LINK_FLAGS "/WHOLEARCHIVE"
            )
    elseif(APPLE)
        set_target_properties(${targetName} PROPERTIES
            LINK_FLAGS "-Wl,-all_load"
            )
    else()
        set_target_properties(${targetName} PROPERTIES
            LINK_FLAGS "-Wl,--whole-archive"
            )
        set_target_properties(${targetName} PROPERTIES
            LINK_FLAGS "-Wl,--allow-multiple-definition"
            )
    endif()
endfunction()


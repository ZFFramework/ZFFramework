
function(zfprojCxxStandard target stdCfg)
    get_property(stdTmp TARGET ${target} PROPERTY CXX_STANDARD)
    if(NOT stdTmp)
        set_property(TARGET ${target} PROPERTY CXX_STANDARD ${stdCfg})
    elseif(stdCfg GREATER stdTmp)
        set_property(TARGET ${target} PROPERTY CXX_STANDARD ${stdCfg})
    endif()
endfunction()

function(zfprojCxxFlags target)
    target_compile_options(${target} PRIVATE
        "$<$<CONFIG:DEBUG>:-DDEBUG>"
    )
    set_target_properties(${target} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        INTERPROCEDURAL_OPTIMIZATION ON
        VISIBILITY_INLINES_HIDDEN ON
    )
endfunction()

# zfprojExec("${ZF_ROOT_PATH}/tools/common/copy_check" "xxx" "xxx")
function(zfprojExec cmd)
    if(CMAKE_HOST_WIN32)
        file(TO_NATIVE_PATH "${cmd}" cmd)
        list(APPEND params "${cmd}.bat")
    else()
        list(APPEND params "sh")
        list(APPEND params "${cmd}.sh")
    endif()
    foreach(arg IN LISTS ARGN)
        if(arg MATCHES "^%")
            string(REGEX REPLACE "^%" "" argTmp "${arg}")
        else()
            file(TO_NATIVE_PATH "${arg}" argTmp)
        endif()
        list(APPEND params "${argTmp}")
    endforeach()
    execute_process(COMMAND ${params})
endfunction()

function(zfprojSrcFiles result unityBuildFile ZF_ROOT_PATH relDir zfsrcBaseDir)
    if("$ENV{ZF_UNITY_BUILD}" STREQUAL "0")
        file(GLOB_RECURSE resultTmp RELATIVE "${relDir}"
            "${zfsrcBaseDir}/zfsrc/*.c"
            "${zfsrcBaseDir}/zfsrc/*.cpp"
            )
    else()
        zfprojExec("${ZF_ROOT_PATH}/tools/common/unity_build" "${unityBuildFile}" "${zfsrcBaseDir}/zfsrc")
        set(resultTmp ${unityBuildFile})
        if(WIN32)
            set_source_files_properties("${unityBuildFile}" PROPERTIES COMPILE_OPTIONS "/bigobj")
        endif()
    endif()
    file(GLOB_RECURSE resultExt RELATIVE "${relDir}"
        "${zfsrcBaseDir}/zfsrc_ext/*.c"
        "${zfsrcBaseDir}/zfsrc_ext/*.cpp"
        )
    list(APPEND resultTmp ${resultExt})
    set(${result} ${resultTmp} PARENT_SCOPE)
endfunction()

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


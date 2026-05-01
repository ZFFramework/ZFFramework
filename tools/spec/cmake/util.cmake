
function(zfprojCxxStandard target stdCfg)
    get_property(stdTmp TARGET ${target} PROPERTY CXX_STANDARD)
    if(NOT stdTmp)
        set_property(TARGET ${target} PROPERTY CXX_STANDARD ${stdCfg})
    elseif(stdCfg GREATER stdTmp)
        set_property(TARGET ${target} PROPERTY CXX_STANDARD ${stdCfg})
    endif()
endfunction()

function(zfprojSrcFiles result unityBuildFile ZF_ROOT_PATH relDir zfsrcBaseDir)
    if("$ENV{ZF_UNITY_BUILD}" STREQUAL "0")
        file(GLOB_RECURSE resultTmp RELATIVE "${relDir}"
            "${zfsrcBaseDir}/zfsrc/*.c"
            "${zfsrcBaseDir}/zfsrc/*.cpp"
            )
    else()
        if(WIN32)
            execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\common\\unity_build.bat" "${unityBuildFile}" "${zfsrcBaseDir}\\zfsrc")
        else()
            execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/common/unity_build.sh" "${unityBuildFile}" "${zfsrcBaseDir}/zfsrc")
        endif()
        set(resultTmp ${unityBuildFile})
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


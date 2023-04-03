
function(zfprojConfigGlobal_ZF_impl)
endfunction(zfprojConfigGlobal_ZF_impl)

function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
endfunction(zfprojConfigBefore_ZF_impl)

function(zfprojConfigAfter_ZF_impl projName)
    add_subdirectory("${ZF_ROOT_PATH}/ZF/ZF_impl/zf3rd/_repo/SDL" SDL)
    target_link_libraries(${projName} PUBLIC SDL2main SDL2-static)
    set_target_properties(SDL2main PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(SDL2-static PROPERTIES POSITION_INDEPENDENT_CODE ON)

    zfprojLoadAllSymbol(${projName})
endfunction(zfprojConfigAfter_ZF_impl)


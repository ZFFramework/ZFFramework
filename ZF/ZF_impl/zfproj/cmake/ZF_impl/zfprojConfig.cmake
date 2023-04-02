
function(zfprojConfigGlobal_ZF_impl)
endfunction(zfprojConfigGlobal_ZF_impl)

function(zfprojConfigBefore_ZF_impl projName ZF_SRC_FILES)
endfunction(zfprojConfigBefore_ZF_impl)

function(zfprojConfigAfter_ZF_impl projName)
    set_target_properties(${projName} PROPERTIES CXX_STANDARD 11)
    set_target_properties(${projName} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endfunction(zfprojConfigAfter_ZF_impl)


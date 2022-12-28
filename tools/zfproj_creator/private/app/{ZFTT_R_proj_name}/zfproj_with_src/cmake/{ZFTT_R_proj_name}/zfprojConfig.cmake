
include("${ZF_ROOT_PATH}/ZF/ZF_impl/zfproj/cmake/ZF_impl/zfprojConfig.cmake")

function(zfprojConfigBefore_{ZFTT_R_proj_name} projName srcfiles)
    if(COMMAND zfprojConfigBefore_ZF_impl)
        zfprojConfigBefore_ZF_impl(${projName} ${srcfiles})
    endif()
endfunction(zfprojConfigBefore_{ZFTT_R_proj_name})

function(zfprojConfigAfter_{ZFTT_R_proj_name} projName)
    if(COMMAND zfprojConfigAfter_ZF_impl)
        zfprojConfigAfter_ZF_impl(${projName})
    endif()
endfunction(zfprojConfigAfter_{ZFTT_R_proj_name})


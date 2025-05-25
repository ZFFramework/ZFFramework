cmake_minimum_required(VERSION 3.16)
project(ZFFramework_test)
set(ZF_ROOT_PATH "${PROJECT_SOURCE_DIR}/../../../../../ZFFramework")
include("${ZF_ROOT_PATH}/tools/spec/cmake/util.cmake")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

include("${PROJECT_SOURCE_DIR}/zfprojConfig.cmake" OPTIONAL)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\release\\release_cmake.bat" 1)
else()
    execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/release/release_cmake.sh" 1)
endif()

file(GLOB_RECURSE ZF_SRC_FILES RELATIVE "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/cmake_main.cpp"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.c"
    "${PROJECT_SOURCE_DIR}/../../../zfsrc_ext/*.cpp"
    )
if(COMMAND zfprojConfigBefore_ZFFramework_test)
    zfprojConfigBefore_ZFFramework_test(ZFFramework_test ZF_SRC_FILES)
endif()
add_executable(ZFFramework_test ${ZF_SRC_FILES})
zfprojStripFILE(ZFFramework_test)
set_target_properties(ZFFramework_test PROPERTIES LINKER_LANGUAGE CXX)
if(COMMAND zfprojConfigAfter_ZFFramework_test)
    zfprojConfigAfter_ZFFramework_test(ZFFramework_test)
endif()

target_include_directories(ZFFramework_test PRIVATE "${PROJECT_SOURCE_DIR}/../../../zfsrc")

function(ZFAddLib ZF_LIB_NAME_)
    target_include_directories(ZFFramework_test PRIVATE "${ZF_ROOT_PATH}/_release/cmake/all/include")

    target_link_directories(ZFFramework_test PUBLIC "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib")
    target_link_libraries(ZFFramework_test ${ZF_LIB_NAME_})

    install(
        DIRECTORY "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/lib/"
        DESTINATION "${PROJECT_BINARY_DIR}/zfdist"
        PATTERN "*.lib" EXCLUDE
        )
    if(WIN32)
        execute_process(COMMAND "${ZF_ROOT_PATH}\\tools\\util\\copy_res.bat" "${ZF_ROOT_PATH}\\_release\\cmake\\module\\${ZF_LIB_NAME_}\\zfres\\" "${PROJECT_BINARY_DIR}\\zfdist\\zfres")
    else()
        execute_process(COMMAND sh "${ZF_ROOT_PATH}/tools/util/copy_res.sh" "${ZF_ROOT_PATH}/_release/cmake/module/${ZF_LIB_NAME_}/zfres/" "${PROJECT_BINARY_DIR}/zfdist/zfres")
    endif()
endfunction()

ZFAddLib(ZFCore)
ZFAddLib(ZFAlgorithm)
ZFAddLib(ZFUtility)
ZFAddLib(ZFUIKit)
ZFAddLib(ZFUIWidget)
ZFAddLib(ZFUIExt)
ZFAddLib(ZFLua)
ZFAddLib(ZFUIWebKit)
ZFAddLib(ZFNet)
ZFAddLib(ZFAppUtil)
ZFAddLib(ZF_impl)
ZFAddLib(ZFCore_impl)
ZFAddLib(ZFAlgorithm_impl)
ZFAddLib(ZFUIKit_impl)
ZFAddLib(ZFLua_impl)
ZFAddLib(ZFUIWebKit_impl)
ZFAddLib(ZFNet_impl)

if(APPLE)
    set_target_properties(ZFFramework_test PROPERTIES INSTALL_RPATH "@loader_path")
elseif(WIN32)
else()
    set_target_properties(ZFFramework_test PROPERTIES INSTALL_RPATH "\\\$ORIGIN;$ORIGIN")
endif()

install(
    TARGETS ZFFramework_test
    DESTINATION "${PROJECT_BINARY_DIR}/zfdist"
    )

file(GLOB_RECURSE SDL_LIBS_
    "${ZF_ROOT_PATH}/_release/cmake/SDL/lib/*SDL2*.*"
    )
foreach(SDL_LIB_ ${SDL_LIBS_})
    if(NOT (SDL_LIB_ MATCHES "_release/cmake/SDL/lib/cmake|_release/cmake/SDL/lib/pkgconfig"))
        if(NOT (SDL_LIB_ MATCHES "SDL2[^/]*\\.a|SDL2[^/]*\\.lib"))
            install(FILES ${SDL_LIB_} DESTINATION "${PROJECT_BINARY_DIR}/zfdist")
        endif()
    endif()
endforeach()

file(GLOB_RECURSE SDL_LIBS_
    "${ZF_ROOT_PATH}/_release/cmake/SDL/bin/*.dll"
    )
foreach(SDL_LIB_ ${SDL_LIBS_})
    install(FILES ${SDL_LIB_} DESTINATION "${PROJECT_BINARY_DIR}/zfdist")
endforeach()


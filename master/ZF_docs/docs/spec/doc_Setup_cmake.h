/**
 * @page DocTag_Setup_cmake cmake
 *
 * @section DocTag_Setup_cmake_Requirement Requirement
 *
 * you must have these settings being prepared:
 * -  cmake
 * -  proper make and compilers
 * -  nasm
 * -  OpenSSL
 * -  for Windows:
 *   -  `Desktop development with C++` via VisualStudioInstaller
 *   -  ninja
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup for quick setup your project files
 *
 * @section DocTag_Setup_cmake_ManualSetup Manual Setup
 *
 * if you really want to setup manually:
 *
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own cmake project (CMakeLists.txt)
 * -# have these lines in your CMakeLists.txt:
 *   @code
 *     add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
 *   @endcode
 * -# specify include path and lib search path in your CMakeLists.txt:
 *   @code
 *     include_directories(path_to_ZFFramework/_release/cmake/all/include)
 *     add_library(your_required_ZF_lib_name SHARED IMPORTED)
 *     set_target_properties(your_required_ZF_lib_name PROPERTIES IMPORTED_LOCATION path_to_ZFFramework/_release/cmake/all/lib/your_required_ZF_lib_name.dll)
 *   @endcode
 * -# copy all res and libs to your product output or proper location according to your platform
 *   -  for example
 *     @code
 *       ~/
 *           YourApp.exe
 *           ZFCore.dll
 *           zfres/
 *     @endcode
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial to code with ZFFramework, enjoy
 */


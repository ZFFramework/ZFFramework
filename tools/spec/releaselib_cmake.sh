WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_cmake.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
_OLD_DIR=$(pwd)

sh "$ZF_ROOT_PATH/tools/common/file_exist.sh" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

cd "$PROJ_PATH/cmake/$PROJ_NAME"
_PROJ_PATH=$(pwd)
cd "$_OLD_DIR"

mkdir -p "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build"
_TMP_PATH=$(pwd)
cd "$_OLD_DIR"

mkdir -p "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist"
_INSTALL_PATH=$(pwd)
cd "$_OLD_DIR"

cd "$_TMP_PATH"
cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX="$_INSTALL_PATH" \
    "$_PROJ_PATH"
make -j4
make install
_RESULT=$?
cd "$_OLD_DIR"

sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$_INSTALL_PATH/include/SDL*"

sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$_INSTALL_PATH/lib/cmake"
sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$_INSTALL_PATH/lib/pkgconfig"
sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$_INSTALL_PATH/lib/*SDL*_test.*"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/include" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/include"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/lib" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/bin" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/bin/zfres" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/zfres"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME" "$ZF_ROOT_PATH/_release/cmake/all"

exit $_RESULT


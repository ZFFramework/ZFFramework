WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_cmake.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..

sh "$ZF_ROOT_PATH/tools/common/file_exist.sh" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)

mkdir -p "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build"
cmake "$PROJ_PATH/cmake/$PROJ_NAME" -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX="$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist"
make -j4
make install
RESULT=$?

cd "$_OLD_DIR"

sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/include/SDL*"

sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/lib/cmake"
sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/lib/pkgconfig"
sh "$ZF_ROOT_PATH/tools/common/zfsh/rm.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/lib/*SDL*_test.*"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/include" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/include"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/lib" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/zfdist/bin/zfres" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/zfres"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME" "$ZF_ROOT_PATH/_release/cmake/all"
exit $RESULT


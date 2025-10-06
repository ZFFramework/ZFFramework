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
cmake "$PROJ_PATH/cmake/$PROJ_NAME" -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j4
make install
RESULT=$?

cd "$_OLD_DIR"

exit $RESULT


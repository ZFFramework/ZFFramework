WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_cmake.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/file_exist.sh" "$ZF_ROOT_PATH/_release/cmake/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)

mkdir -p "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/cmake/$PROJ_NAME/build"
cmake "$PROJ_PATH/zfproj/cmake/$PROJ_NAME"
make -j4
make install

cd "$_OLD_DIR"

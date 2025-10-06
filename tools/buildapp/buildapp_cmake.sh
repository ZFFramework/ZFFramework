WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  buildapp_cmake.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

if test "x-$CMAKE_BUILD_TYPE" = "x-" ; then
    _CONFIG=RelWithDebInfo
else
    _CONFIG=$CMAKE_BUILD_TYPE
fi

ZF_ROOT_PATH=$WORK_DIR/../..
_TMP_PATH=$PROJ_PATH/../../_tmp/cmake/$PROJ_NAME/$_CONFIG

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/cmake/$PROJ_NAME"
_PROJ_PATH=$(pwd)
cd "$_OLD_DIR"

_OLD_DIR=$(pwd)
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cd "$_TMP_PATH"
if test "x-$CMAKE_CXX_FLAGS" = "x-" ; then
    cmake "$_PROJ_PATH" -DCMAKE_BUILD_TYPE=${_CONFIG}
else
    cmake "$_PROJ_PATH" -DCMAKE_BUILD_TYPE=${_CONFIG} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
fi
make -j4
make install
_RESULT="$?"
cd "$_OLD_DIR"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_TMP_PATH/zfdist" "$PROJ_PATH/../../_tmp/cmake/$PROJ_NAME/app"

exit $_RESULT


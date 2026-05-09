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
_INSTALL_PATH=$_TMP_PATH/zfdist

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/cmake/$PROJ_NAME"
_PROJ_PATH=$(pwd)
cd "$_OLD_DIR"

_OLD_DIR=$(pwd)
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cd "$_TMP_PATH"
if test "x-$CMAKE_CXX_FLAGS" = "x-" ; then
    cmake \
        -DCMAKE_BUILD_TYPE=${_CONFIG} \
        -DCMAKE_INSTALL_PREFIX="$_INSTALL_PATH" \
        "$_PROJ_PATH"
else
    cmake \
        -DCMAKE_BUILD_TYPE=${_CONFIG} \
        -DCMAKE_INSTALL_PREFIX="$_INSTALL_PATH" \
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} \
        "$_PROJ_PATH"
fi
make -j4
make install
_RESULT="$?"
cd "$_OLD_DIR"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/bin" "$PROJ_PATH/../../_tmp/cmake/$PROJ_NAME/app"
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$_INSTALL_PATH/lib" "$PROJ_PATH/../../_tmp/cmake/$PROJ_NAME/app"

exit $_RESULT


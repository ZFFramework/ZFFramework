WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  buildapp_Qt.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
_TMP_PATH=$PROJ_PATH/../../_tmp/Qt/$PROJ_NAME/release

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/Qt/$PROJ_NAME"
_PROJ_PATH=$(pwd)
cd "$_OLD_DIR"

_OLD_DIR=$(pwd)
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cd "$_TMP_PATH"
qmake "$_PROJ_PATH/$PROJ_NAME.pro" CONFIG+=release
make -j4
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT


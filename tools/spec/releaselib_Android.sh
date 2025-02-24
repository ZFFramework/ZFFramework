WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_Android.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..

sh "$ZF_ROOT_PATH/tools/common/file_exist.sh" "$ZF_ROOT_PATH/_release/Android/module/$PROJ_NAME/aar" "*.aar"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)

cd "$PROJ_PATH/Android/$PROJ_NAME"
chmod +x gradlew
./gradlew assembleRelease
RESULT=$?

cd "$_OLD_DIR"

exit $RESULT


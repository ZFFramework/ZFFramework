WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  buildapp_Android.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/Android/$PROJ_NAME"
chmod +x gradlew
./gradlew assembleRelease
_RESULT="$?"
cd "$_OLD_DIR"

sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$PROJ_PATH/Android/$PROJ_NAME/zfapp/build/outputs/apk/release/zfapp-release.apk" "$PROJ_PATH/../../_tmp/Android/$PROJ_NAME/app/$PROJ_NAME.apk"

exit $_RESULT


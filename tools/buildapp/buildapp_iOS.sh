WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  buildapp_iOS.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..

_OLD_DIR=$(pwd)
cd "$PROJ_PATH/iOS/$PROJ_NAME"
xcodebuild -showsdks 2>/dev/null | grep iphonesimulator | cut -f3 | xargs xcodebuild build -project $PROJ_NAME.xcodeproj -scheme $PROJ_NAME CODE_SIGNING_REQUIRED=NO
_RESULT="$?"
cd "$_OLD_DIR"

exit $_RESULT


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
pod install
_RESULT="$?"
if test ! "$_RESULT" = "0" ; then
    cd "$_OLD_DIR"
    exit $_RESULT
fi
cd "$_OLD_DIR"

xcodebuild -showsdks 2>/dev/null | grep iphonesimulator | cut -f3 | xargs xcodebuild archive -workspace "$PROJ_PATH/iOS/$PROJ_NAME/$PROJ_NAME.xcworkspace" -scheme $PROJ_NAME -configuration Release -archivePath "$PROJ_PATH/../../_tmp/iOS/$PROJ_NAME/app" -derivedDataPath "$PROJ_PATH/../../_tmp/iOS/$PROJ_NAME/build"
_RESULT="$?"

exit $_RESULT


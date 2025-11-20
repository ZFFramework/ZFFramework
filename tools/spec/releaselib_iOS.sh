WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x-$PROJ_NAME" = "x-" || test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  releaselib_iOS.sh PROJ_NAME PROJ_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
RELEASE_PATH=$ZF_ROOT_PATH/_release/iOS

sh "$ZF_ROOT_PATH/tools/common/file_exist.sh" "$RELEASE_PATH/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi


# build
_OLD_DIR=$(pwd)
cd "$PROJ_PATH/iOS/$PROJ_NAME"
pod install
_RESULT=$?
if test ! "$_RESULT" = "0" ; then
    cd "$_OLD_DIR"
    exit $_RESULT
fi
xcodebuild -workspace "${PROJ_NAME}.xcworkspace" -scheme "${PROJ_NAME}" -configuration "Release" -sdk iphoneos -derivedDataPath "$ZF_ROOT_PATH/_tmp/iOS/$PROJ_NAME/build_iphoneos"
_RESULT=$?
if test ! "$_RESULT" = "0" ; then
    cd "$_OLD_DIR"
    exit $_RESULT
fi
xcodebuild -workspace "${PROJ_NAME}.xcworkspace" -scheme "${PROJ_NAME}" -configuration "Release" -sdk iphonesimulator -derivedDataPath "$ZF_ROOT_PATH/_tmp/iOS/$PROJ_NAME/build_iphonesimulator"
_RESULT=$?
if test ! "$_RESULT" = "0" ; then
    cd "$_OLD_DIR"
    exit $_RESULT
fi
cd "$_OLD_DIR"

# create framework
mkdir -p "$RELEASE_PATH/module/$PROJ_NAME/lib" >/dev/null 2>&1
xcodebuild -create-xcframework \
    -library "$ZF_ROOT_PATH/_tmp/iOS/$PROJ_NAME/build_iphoneos/Build/Products/Release-iphoneos/lib$PROJ_NAME.a" \
    -library "$ZF_ROOT_PATH/_tmp/iOS/$PROJ_NAME/build_iphonesimulator/Build/Products/Release-iphonesimulator/lib$PROJ_NAME.a" \
    -output "$RELEASE_PATH/module/$PROJ_NAME/lib/$PROJ_NAME.xcframework"
_RESULT=$?
if test ! "$_RESULT" = "0" ; then
    exit $_RESULT
fi

# copy headers
sh "$ZF_ROOT_PATH/tools/util/copy_header.sh" "$PROJ_PATH/../zfsrc" "$RELEASE_PATH/module/$PROJ_NAME/include"

# copy res
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$PROJ_PATH/../zfres" "$RELEASE_PATH/module/$PROJ_NAME/zfres"

# install
sh "$ZF_ROOT_PATH/tools/common/copy_check.sh" "$RELEASE_PATH/module/$PROJ_NAME/." "$RELEASE_PATH/all/"

exit 0


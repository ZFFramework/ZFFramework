WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DST_PATH=$1
SRC_PATH=$2
if test "x-$DST_PATH" = "x-" || test "x-$SRC_PATH" = "x-" ; then
    echo "usage:"
    echo "  plist_merge.sh DST_PATH SRC_PATH"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..

if test ! -e "$DST_PATH" ; then
    cp "$WORK_DIR/Info.plist" "$DST_PATH" >/dev/null 2>&1
fi
if test -e "$SRC_PATH" ; then
    /usr/libexec/PlistBuddy -x -c "Merge \"$SRC_PATH\"" "$DST_PATH"
fi


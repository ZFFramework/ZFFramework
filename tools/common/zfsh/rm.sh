WORK_DIR=$(cd "$(dirname "$0")"; pwd)
TARGET_PATH=$1
if test "x-$TARGET_PATH" = "x-" ; then
    echo "usage:"
    echo "  rm.sh TARGET_PATH"
    exit 1
fi

_TARGET_PARENT=$(dirname "$TARGET_PATH")
_TARGET_NAME=$(basename "$TARGET_PATH")
rm -rf "$_TARGET_PARENT"/$_TARGET_NAME >/dev/null 2>&1


WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DST_PATH=$1
SRC_PATH=$2
KEY_NAME=$3
VALUE_FILE_PATH=$4
if test "x-$DST_PATH" = "x-" || test "x-$SRC_PATH" = "x-" || test "x-$KEY_NAME" = "x-" || test "x-$VALUE_FILE_PATH" = "x-" ; then
    echo "usage:"
    echo "  file_replace.sh DST_PATH SRC_PATH KEY_NAME VALUE_FILE_PATH"
    exit 1
fi

_VALUE=`cat "$VALUE_FILE_PATH"`
if test "x-$_VALUE" = "x-" ; then
    echo "no value in: $VALUE_FILE_PATH"
    exit 1
fi

cat "$SRC_PATH" | sed "s/$KEY_NAME/$_VALUE/g" > "$DST_PATH"


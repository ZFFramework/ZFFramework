WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_URL=$1
DST_PATH=$2
if test "x-$SRC_URL" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  file_download.sh SRC_URL DST_PATH"
    exit 1
fi

DST_PARENT=${DST_PATH%[/\\]*}
if ! test -e "$DST_PARENT" ; then
    mkdir -p "$DST_PARENT"
fi

wget -O "$DST_PATH" "$SRC_URL"

exit 0


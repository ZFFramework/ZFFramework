WORK_DIR=$(cd "$(dirname "$0")"; pwd)
SRC_URL=$1
DST_PATH=$2
FORCE=$3
if test "x-$SRC_URL" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  file_download.sh SRC_URL DST_PATH [FORCE]"
    exit 1
fi

if test -e "$DST_PATH" && test ! "x-$FORCE" = "x-1" ; then
    exit 0
fi

DST_PARENT=${DST_PATH%[/\\]*}
if ! test "x-$DST_PARENT" = "x-$DST_PATH" && ! test -e "$DST_PARENT" ; then
    mkdir -p "$DST_PARENT"
fi

wget -O "${DST_PATH}.tmp" "$SRC_URL"
_RESULT="$?"
if test ! "$_RESULT" = "0" ; then
    rm "${DST_PATH}.tmp" >/dev/null 2>&1
    exit $_RESULT
fi
mv "${DST_PATH}.tmp" "$DST_PATH" >/dev/null 2>&1


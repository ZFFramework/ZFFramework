WORK_DIR=$(cd "$(dirname "$0")"; pwd)
DIR_TO_CHECK=$1
TIMESTAMP_FILE_NAME=$2
CHECK_INTERVAL=$3
if test "x-$DIR_TO_CHECK" = "x-" ; then
    echo "usage:"
    echo "  timestamp_check.sh DIR_TO_CHECK [TIMESTAMP_FILE_NAME CHECK_INTERVAL]"
    echo "return by $ ?:"
    echo "  0: not expired"
    echo "  1: expired"
    exit 1
fi

if test "x-$TIMESTAMP_FILE_NAME" = "x-" ; then
    TIMESTAMP_FILE_NAME=_zf_timestamp_
fi
if test "x-$CHECK_INTERVAL" = "x-" ; then
    CHECK_INTERVAL=604800
fi

_CUR_TIME=$(date +%s)
if test -e "$DIR_TO_CHECK/$TIMESTAMP_FILE_NAME" ; then
    read _OLD_TIME < "$DIR_TO_CHECK/$TIMESTAMP_FILE_NAME"
else
    _OLD_TIME=0
fi
_EXPIRE_TIME=$((_OLD_TIME + CHECK_INTERVAL))
if test $_CUR_TIME -ge $_EXPIRE_TIME ; then
    exit 1
fi


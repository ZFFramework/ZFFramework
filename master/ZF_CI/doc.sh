WORK_DIR=$(cd "$(dirname "$0")"; pwd)

if test "x-$ZFCI_TOKEN" = "x-" ; then
    echo "ZFCI_TOKEN not set"
    exit 1
fi

sh "$WORK_DIR/doc_make.sh"
sh "$WORK_DIR/doc_push.sh"


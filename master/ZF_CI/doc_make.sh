WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..

OUTPUT_PATH="$ZF_ROOT_PATH/_tmp/DoxygenDoc"
mkdir -p "$OUTPUT_PATH"

_OLD_DIR=$(pwd)
cd "$ZF_ROOT_PATH/master/ZF_docs/Doxygen"
doxygen
if ! test "$?" = "0" ; then
    exit 1
fi
cd "$_OLD_DIR"

rm -rf "$OUTPUT_PATH/html/index.hhc"
rm -rf "$OUTPUT_PATH/html/index.hhk"
rm -rf "$OUTPUT_PATH/html/index.hhp"


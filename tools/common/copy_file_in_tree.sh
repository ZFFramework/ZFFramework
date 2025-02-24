WORK_DIR=$(cd "$(dirname "$0")"; pwd)
FILE_NAME=$1
FROM_PATH=$2
TO_PATH=$3
if test "x-$FILE_NAME" = "x-" || test "x-$FROM_PATH" = "x-" || test "x-$TO_PATH" = "x-" ; then
    echo "usage:"
    echo "  copy_file_in_tree.sh FILE_NAME FROM_PATH TO_PATH"
    echo ""
    echo "FROM_PATH and TO_PATH must not end with '/'"
    exit 1
fi

find "$FROM_PATH" -name "$FILE_NAME" | sed -e "s#${FROM_PATH}##g" | xargs -I{} sh "${WORK_DIR}/copy_check.sh" "${FROM_PATH}{}" "${TO_PATH}{}"


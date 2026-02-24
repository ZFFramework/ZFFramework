WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x-$PROJ_ROOT" = "x-" || test "x-$PROJ_NAME" = "x-" ; then
    echo "usage:"
    echo "  cleanup_Android.sh PROJ_ROOT PROJ_NAME"
    exit 1
fi

rm -rf "$PROJ_ROOT/$PROJ_NAME/.gradle" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/.idea" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/build" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/gradle-daemon-jvm.properties" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/local.properties" >/dev/null 2>&1
find "$PROJ_ROOT/$PROJ_NAME" -name "*.iml" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1

rm -rf "$PROJ_ROOT/$PROJ_NAME/zflib/.cxx" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zflib/.externalNativeBuild" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zflib/build" >/dev/null 2>&1
find "$PROJ_ROOT/$PROJ_NAME/zflib/" -name "*.iml" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zflib/src/main/assets/zfres" >/dev/null 2>&1

rm -rf "$PROJ_ROOT/$PROJ_NAME/zfapp/.cxx" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfapp/.externalNativeBuild" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfapp/build" >/dev/null 2>&1
find "$PROJ_ROOT/$PROJ_NAME/zfapp/" -name "*.iml" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfapp/src/main/assets/zfres" >/dev/null 2>&1

rm -rf "$PROJ_ROOT/$PROJ_NAME/zfprojConfigHelper/.cxx" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfprojConfigHelper/.externalNativeBuild" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfprojConfigHelper/build" >/dev/null 2>&1
find "$PROJ_ROOT/$PROJ_NAME/zfprojConfigHelper/" -name "*.iml" 2>/dev/null | xargs -I{} rm -rf {} >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/zfprojConfigHelper/src/main/assets/zfres" >/dev/null 2>&1

exit 0


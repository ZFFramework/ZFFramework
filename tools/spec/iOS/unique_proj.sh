WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$1
POD_INSTALL=$2
if test "x-$PROJ_PATH" = "x-" ; then
    echo "usage:"
    echo "  unique_proj.sh PROJ_PATH [POD_INSTALL]"
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..

if test "x-$_PY" = "x-" ; then
    python --version >/dev/null 2>&1 && export _PY=python || export _PY=
fi
if test "x-$_PY" = "x-" ; then
    python3 --version >/dev/null 2>&1 && export _PY=python3 || export _PY=
fi
if test "x-$_PY" = "x-" ; then
    echo "[unique_proj] no python available"
    exit 1
fi

sh "$ZF_ROOT_PATH/tools/common/zfsh.sh" "$WORK_DIR/zf3rd/zfautoscript_zf3rd_setup_xUnique.zfsh"

if test -d "$PROJ_PATH" ; then
    _SRC_PATH="$PROJ_PATH/project.pbxproj"
    _TMP_PATH="$PROJ_PATH/zfxunique_tmp"
    _DST_PATH="$_TMP_PATH/project.pbxproj"
else
    _SRC_PATH="$PROJ_PATH"
    _TMP_PATH="${PROJ_PATH%[/\\]*}/zfxunique_tmp"
    _DST_PATH="$_TMP_PATH/project.pbxproj"
fi

if test "x-$POD_INSTALL" = "x-1" ; then
    _POD_PATH="${PROJ_PATH%[/\\]*}/.."
    if test -e "$_POD_PATH/Podfile" ; then
        _OLD_DIR=$(pwd)
        cd "$_POD_PATH"
        pod install
        cd "$_OLD_DIR"
    fi
fi

rm -rf "$_TMP_PATH" >/dev/null 2>&1
mkdir -p "$_TMP_PATH" >/dev/null 2>&1
cp -r "$_SRC_PATH" "$_DST_PATH" >/dev/null 2>&1
$_PY "$WORK_DIR/zf3rd/_repo/xUnique/xUnique.py" "$_DST_PATH"

if test -e "$_TMP_PATH/project.pbxproj.ubak" ; then
    mv "$_TMP_PATH/project.pbxproj.ubak" "$_TMP_PATH/project.pbxproj"
fi

if test -e "$_TMP_PATH/project.pbxproj.sbak" ; then
    mv "$_TMP_PATH/project.pbxproj.sbak" "$_TMP_PATH/project.pbxproj"
fi

rsync -ruc "$_DST_PATH" "$_SRC_PATH" >/dev/null 2>&1

rm -rf "$_TMP_PATH" >/dev/null 2>&1

exit 0


WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_GIT=$1
GIT_BRANCH=$2
DST_PATH=$3
shift
shift
shift
CLONE_OPTION=$@
if test "x-$PROJ_GIT" = "x-" || test "x-$DST_PATH" = "x-" ; then
    echo "usage:"
    echo "  git_check.sh PROJ_GIT GIT_BRANCH DST_PATH [options]"
    exit 1
fi
if test "x-$CLONE_OPTION" = "x-" ; then
    CLONE_OPTION=--depth=1
fi

_OLD_DIR=$(pwd)
_NEED_PULL=0

_GIT_VALID=0
if test -e "$DST_PATH/.git"; then
    _GIT_PATH_DESIRED="$DST_PATH/.git"
    cd "$_GIT_PATH_DESIRED" >/dev/null 2>&1
    _GIT_PATH_DESIRED=$(pwd -P)
    cd "$_OLD_DIR"

    cd "$DST_PATH" >/dev/null 2>&1
    _GIT_PATH_EXIST=`git rev-parse --show-toplevel 2>/dev/null`/.git
    cd "$_OLD_DIR"
    cd "$_GIT_PATH_EXIST" >/dev/null 2>&1
    _GIT_PATH_EXIST=$(pwd)
    cd "$_OLD_DIR"

    if test "$_GIT_PATH_DESIRED" = "$_GIT_PATH_EXIST" ; then
        _GIT_VALID=1
    fi
fi
if test "$_GIT_VALID" = "1"; then
    sh "$WORK_DIR/timestamp_check.sh" "$DST_PATH/.git"
    if test ! "$?" = "0" ; then
        _NEED_PULL=1
    fi
else
    _NEED_PULL=1
    rm -rf "$DST_PATH/.git_check_tmp" >/dev/null 2>&1
    mkdir -p "$DST_PATH" >/dev/null 2>&1
    _DST_HAS_CONTENT=0
    if test ! "x-$(ls -A "$DST_PATH")" = "x-" ; then
        _DST_HAS_CONTENT=1
    fi
    git clone -b "$GIT_BRANCH" $CLONE_OPTION "$PROJ_GIT" "$DST_PATH/.git_check_tmp"
    _CLONE_SUCCESS="$?"
    if test ! "$_CLONE_SUCCESS" = "0"; then
        rm -rf "$DST_PATH/.git_check_tmp" >/dev/null 2>&1
        exit $_CLONE_SUCCESS
    fi
    rm -rf "$DST_PATH/.git" >/dev/null 2>&1
    mv "$DST_PATH/.git_check_tmp/.git" "$DST_PATH/.git" >/dev/null 2>&1
    rm -rf "$DST_PATH/.git_check_tmp" >/dev/null 2>&1
    if test "x-$_DST_HAS_CONTENT" = "x-0" ; then
        cd "$DST_PATH"
        git checkout .
        cd "$_OLD_DIR"
    fi
fi

if test "$_NEED_PULL" = "1" ; then
    cd "$DST_PATH"
    git stash
    git checkout .
    git reset --hard
    git fetch --all && git pull
    _SUCCESS="$?"
    if test "$?" = "0"; then
        if test -e ".gitmodules"; then
            git submodule update --init --recursive --depth=1
            _SUCCESS="$?"
        fi
    fi
    git checkout "$GIT_BRANCH"
    git stash pop
    cd "$_OLD_DIR"

    if test "$_SUCCESS" = "0"; then
        sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git"
    fi
    exit $_SUCCESS
fi
exit 0


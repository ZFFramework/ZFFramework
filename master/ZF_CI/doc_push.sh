WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..

if test "x-$ZFCI_TOKEN" = "x-" ; then
    echo "ZFCI_TOKEN not set"
    exit 1
fi

OUTPUT_PATH="$ZF_ROOT_PATH/_tmp/DoxygenDoc"

if ! test -e "$OUTPUT_PATH/html/index.html"; then
    echo "doc not exist"
    exit 1
fi

DOC_REPO_PATH="$OUTPUT_PATH/../DoxygenDoc_repo"
mkdir -p "$DOC_REPO_PATH"

sh "$ZF_ROOT_PATH/tools/common/git_check.sh" "https://github.com/ZFFramework/zfframework.github.com" master "$DOC_REPO_PATH"

_OLD_DIR=$(pwd)
cd "$DOC_REPO_PATH"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git checkout .
git reset --hard
git clean -xdf
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch *' --prune-empty --tag-name-filter cat -- --all
cd "$_OLD_DIR"

cp -r "$ZF_ROOT_PATH/master/ZF_docs/doc_repo/." "$DOC_REPO_PATH/"
rm -rf "$DOC_REPO_PATH/doc"
mkdir "$DOC_REPO_PATH/doc"
cp -r "$OUTPUT_PATH/html/." "$DOC_REPO_PATH/doc/"

_OLD_DIR=$(pwd)
cd "$DOC_REPO_PATH"
git add -A
git commit -a -m "update doc"
git push --force "https://ZSaberLv0:$ZFCI_TOKEN@github.com/ZFFramework/zfframework.github.com"
cd "$_OLD_DIR"


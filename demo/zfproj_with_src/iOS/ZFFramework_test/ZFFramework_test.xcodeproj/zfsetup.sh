WORK_DIR=$(cd "$(dirname "$0")"; pwd)
iOS_SRCROOT=$1
iOS_CONFIGURATION_BUILD_DIR=$2
iOS_CONTENTS_FOLDER_PATH=$3
if test "x-$iOS_SRCROOT" = "x-" || test "x-$iOS_CONFIGURATION_BUILD_DIR" = "x-" || test "x-$iOS_CONFIGURATION_BUILD_DIR" = "x-" ; then
    echo "usage:"
    echo "  zfsetup.sh SRCROOT CONFIGURATION_BUILD_DIR CONTENTS_FOLDER_PATH"
    exit 1
fi

sh "${iOS_SRCROOT}/../../../../zfsetup.sh"

ZF_ROOT_PATH=${iOS_SRCROOT}/../../../../../ZFFramework

PLIST_DST_PATH=${iOS_SRCROOT}/Info_tmp.plist
cp -f "${iOS_SRCROOT}/Info_base.plist" "$PLIST_DST_PATH" >/dev/null 2>&1
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFCore/zfproj/iOS/ZFCore/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFCore/zfproj/iOS/ZFCore/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFAlgorithm/zfproj/iOS/ZFAlgorithm/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUtility/zfproj/iOS/ZFUtility/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIKit/zfproj/iOS/ZFUIKit/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIWidget/zfproj/iOS/ZFUIWidget/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIExt/zfproj/iOS/ZFUIExt/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFLua/zfproj/iOS/ZFLua/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIWebKit/zfproj/iOS/ZFUIWebKit/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFNet/zfproj/iOS/ZFNet/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFAppUtil/zfproj/iOS/ZFAppUtil/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZF_impl/zfproj/iOS/ZF_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFCore_impl/zfproj/iOS/ZFCore_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFAlgorithm_impl/zfproj/iOS/ZFAlgorithm_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIKit_impl/zfproj/iOS/ZFUIKit_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFLua_impl/zfproj/iOS/ZFLua_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFUIWebKit_impl/zfproj/iOS/ZFUIWebKit_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "$ZF_ROOT_PATH/ZF/ZFNet_impl/zfproj/iOS/ZFNet_impl/zfprojConfig.plist"
sh "$ZF_ROOT_PATH/tools/spec/iOS/plist_merge.sh" "$PLIST_DST_PATH" "${iOS_SRCROOT}/zfprojConfig.plist"
mv -f "$PLIST_DST_PATH" "${iOS_SRCROOT}/ProjectFiles/Info.plist"

COMPILE_DST_PATH=${iOS_SRCROOT}/ProjectFiles
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFFramework_test.mm" "${iOS_SRCROOT}/../../../zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFCore.mm" "$ZF_ROOT_PATH/ZF/ZFCore/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFAlgorithm.mm" "$ZF_ROOT_PATH/ZF/ZFAlgorithm/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUtility.mm" "$ZF_ROOT_PATH/ZF/ZFUtility/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIKit.mm" "$ZF_ROOT_PATH/ZF/ZFUIKit/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIWidget.mm" "$ZF_ROOT_PATH/ZF/ZFUIWidget/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIExt.mm" "$ZF_ROOT_PATH/ZF/ZFUIExt/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFLua.mm" "$ZF_ROOT_PATH/ZF/ZFLua/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIWebKit.mm" "$ZF_ROOT_PATH/ZF/ZFUIWebKit/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFNet.mm" "$ZF_ROOT_PATH/ZF/ZFNet/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFAppUtil.mm" "$ZF_ROOT_PATH/ZF/ZFAppUtil/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZF_impl.mm" "$ZF_ROOT_PATH/ZF/ZF_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFCore_impl.mm" "$ZF_ROOT_PATH/ZF/ZFCore_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFAlgorithm_impl.mm" "$ZF_ROOT_PATH/ZF/ZFAlgorithm_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIKit_impl.mm" "$ZF_ROOT_PATH/ZF/ZFUIKit_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFLua_impl.mm" "$ZF_ROOT_PATH/ZF/ZFLua_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFUIWebKit_impl.mm" "$ZF_ROOT_PATH/ZF/ZFUIWebKit_impl/zfsrc"
sh "$ZF_ROOT_PATH/tools/common/unity_build.sh" "${COMPILE_DST_PATH}/zfgensrc_ZFNet_impl.mm" "$ZF_ROOT_PATH/ZF/ZFNet_impl/zfsrc"

RES_DST_PATH=${iOS_CONFIGURATION_BUILD_DIR}/${iOS_CONTENTS_FOLDER_PATH}/zfres
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFCore/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFAlgorithm/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUtility/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIKit/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIWidget/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIExt/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFLua/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIWebKit/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFNet/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFAppUtil/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZF_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFCore_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFAlgorithm_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIKit_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFLua_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFUIWebKit_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "$ZF_ROOT_PATH/ZF/ZFNet_impl/zfres" "$RES_DST_PATH"
sh "$ZF_ROOT_PATH/tools/util/copy_res.sh" "${iOS_SRCROOT}/../../../zfres" "$RES_DST_PATH"


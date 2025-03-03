#include "ZFUIListViewCellUpdater.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListViewCellUpdater)

#define _ZFP_ZFUIListViewCellUpdater_cacheKey_separator "_ZFP_ZFUIListViewCellUpdater_cacheKey_separator"
#define _ZFP_ZFUIListViewCellUpdater_tag_separator "_ZFP_ZFUIListViewCellUpdater_tag_separator"
#define _ZFP_ZFUIListViewCellUpdater_tag_separator_head "_ZFP_ZFUIListViewCellUpdater_tag_separator_head"
static ZFUIView *_ZFP_ZFUIListViewCellUpdater_setupSeparator(
        ZF_IN ZFUIListViewCellUpdater *owner
        , ZF_IN const ZFUICellUpdaterParam &updateParam
        ) {
    zfauto separatorViewHolder = owner->itemCacheAccess(_ZFP_ZFUIListViewCellUpdater_cacheKey_separator);
    ZFUIView *separatorView = separatorViewHolder;
    if(separatorView == zfnull) {
        separatorView = zfAlloc(ZFUIView);
        separatorViewHolder = separatorView;
        zfRelease(separatorView);

        zfobj<ZFUILayoutParam> lp;
        lp->sizeParam(ZFUISizeParamFillFill());
        separatorView->layoutParam(lp);

        separatorView->viewUIEnableTree(zffalse);
    }

    updateParam.cell->internalFgViewAdd(separatorView);
    separatorView->bgColor(owner->separatorColor());
    switch(updateParam.orientation) {
        case v_ZFUIOrientation::e_Left:
        case v_ZFUIOrientation::e_Right:
            separatorView->layoutParam()->sizeHint(ZFUISizeCreate(owner->separatorSize(), -1));
            separatorView->layoutParam()->margin(ZFUIMarginCreate(0, owner->separatorMargin().top, 0, owner->separatorMargin().bottom));
            break;
        case v_ZFUIOrientation::e_Top:
        case v_ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->sizeHint(ZFUISizeCreate(-1, owner->separatorSize()));
            separatorView->layoutParam()->margin(ZFUIMarginCreate(owner->separatorMargin().left, 0, owner->separatorMargin().right, 0));
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return zfnull;
    }
    return separatorView;
}
void ZFUIListViewCellUpdater::cellOnUpdate(ZF_IN const ZFUICellUpdaterParam &updateParam) {
    zfsuperI(ZFUICellUpdater)::cellOnUpdate(updateParam);

    if(updateParam.cellIndex == 0) { // first cell
        if(this->separatorIncludingHead()) {
            ZFUIView *separatorView = _ZFP_ZFUIListViewCellUpdater_setupSeparator(this, updateParam);
            updateParam.cell->objectTag(_ZFP_ZFUIListViewCellUpdater_tag_separator_head, separatorView);
            switch(updateParam.orientation) {
                case v_ZFUIOrientation::e_Left:
                    separatorView->layoutParam()->align(v_ZFUIAlign::e_Left);
                    break;
                case v_ZFUIOrientation::e_Right:
                    separatorView->layoutParam()->align(v_ZFUIAlign::e_Right);
                    break;
                case v_ZFUIOrientation::e_Top:
                    separatorView->layoutParam()->align(v_ZFUIAlign::e_Top);
                    break;
                case v_ZFUIOrientation::e_Bottom:
                    separatorView->layoutParam()->align(v_ZFUIAlign::e_Bottom);
                    break;
                default:
                    ZFCoreCriticalShouldNotGoHere();
                    return;
            }
        }
    }
    if(updateParam.cellIndex == updateParam.cellCount - 1) { // last cell
        if(!this->separatorIncludingTail()) {
            return;
        }
    }

    ZFUIView *separatorView = _ZFP_ZFUIListViewCellUpdater_setupSeparator(this, updateParam);
    updateParam.cell->objectTag(_ZFP_ZFUIListViewCellUpdater_tag_separator, separatorView);
    switch(updateParam.orientation) {
        case v_ZFUIOrientation::e_Left:
            separatorView->layoutParam()->align(v_ZFUIAlign::e_Right);
            break;
        case v_ZFUIOrientation::e_Right:
            separatorView->layoutParam()->align(v_ZFUIAlign::e_Left);
            break;
        case v_ZFUIOrientation::e_Top:
            separatorView->layoutParam()->align(v_ZFUIAlign::e_Bottom);
            break;
        case v_ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->align(v_ZFUIAlign::e_Top);
            break;
        default:
            ZFCoreCriticalShouldNotGoHere();
            return;
    }
}
void ZFUIListViewCellUpdater::cellOnRecycle(ZF_IN ZFUICell *cell) {
    zfsuperI(ZFUICellUpdater)::cellOnRecycle(cell);

    ZFUIView *separatorHead = cell->objectTag(_ZFP_ZFUIListViewCellUpdater_tag_separator_head);
    if(separatorHead != zfnull) {
        this->itemCacheRecycle(_ZFP_ZFUIListViewCellUpdater_cacheKey_separator, separatorHead);
        cell->internalFgViewRemove(separatorHead);
        cell->objectTagRemove(_ZFP_ZFUIListViewCellUpdater_tag_separator_head);
    }
    ZFUIView *separatorTail = cell->objectTag(_ZFP_ZFUIListViewCellUpdater_tag_separator);
    if(separatorTail != zfnull) {
        this->itemCacheRecycle(_ZFP_ZFUIListViewCellUpdater_cacheKey_separator, separatorTail);
        cell->internalFgViewRemove(separatorTail);
        cell->objectTagRemove(_ZFP_ZFUIListViewCellUpdater_tag_separator);
    }
}

ZF_NAMESPACE_GLOBAL_END


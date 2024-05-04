#include "ZFUIListCellUpdaterBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIListCellUpdaterBasic)

#define _ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator "_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator"
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator "_ZFP_ZFUIListCellUpdaterBasic_tag_separator"
#define _ZFP_ZFUIListCellUpdaterBasic_tag_separator_head "_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head"
static ZFUIView *_ZFP_ZFUIListCellUpdaterBasic_setupSeparator(
        ZF_IN ZFUIListCellUpdaterBasic *owner
        , ZF_IN const ZFUIListCellUpdaterParam &updateParam
        ) {
    zfauto separatorViewHolder = owner->itemCacheAccess(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator);
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
    separatorView->viewBackgroundColor(owner->separatorColor());
    switch(updateParam.listOrientation) {
        case ZFUIOrientation::e_Left:
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->sizeHint(ZFUISizeMake(owner->separatorSize(), -1));
            separatorView->layoutParam()->layoutMargin(ZFUIMarginMake(0, owner->separatorMargin().top, 0, owner->separatorMargin().bottom));
            break;
        case ZFUIOrientation::e_Top:
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->sizeHint(ZFUISizeMake(-1, owner->separatorSize()));
            separatorView->layoutParam()->layoutMargin(ZFUIMarginMake(owner->separatorMargin().left, 0, owner->separatorMargin().right, 0));
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
    return separatorView;
}
void ZFUIListCellUpdaterBasic::cellOnUpdate(ZF_IN const ZFUIListCellUpdaterParam &updateParam) {
    zfsuperI(ZFUIListCellUpdater)::cellOnUpdate(updateParam);

    if(updateParam.cellIndex == 0) { // first cell
        if(this->separatorIncludingHead()) {
            ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
            updateParam.cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head, separatorView);
            switch(updateParam.listOrientation) {
                case ZFUIOrientation::e_Left:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Left);
                    break;
                case ZFUIOrientation::e_Right:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Right);
                    break;
                case ZFUIOrientation::e_Top:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Top);
                    break;
                case ZFUIOrientation::e_Bottom:
                    separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Bottom);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return;
            }
        }
    }
    if(updateParam.cellIndex == updateParam.cellCount - 1) { // last cell
        if(!this->separatorIncludingTail()) {
            return;
        }
    }

    ZFUIView *separatorView = _ZFP_ZFUIListCellUpdaterBasic_setupSeparator(this, updateParam);
    updateParam.cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator, separatorView);
    switch(updateParam.listOrientation) {
        case ZFUIOrientation::e_Left:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Right);
            break;
        case ZFUIOrientation::e_Right:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Left);
            break;
        case ZFUIOrientation::e_Top:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Bottom);
            break;
        case ZFUIOrientation::e_Bottom:
            separatorView->layoutParam()->layoutAlign(ZFUIAlign::e_Top);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return;
    }
}
void ZFUIListCellUpdaterBasic::cellOnRecycle(ZF_IN ZFUIListCell *cell) {
    zfsuperI(ZFUIListCellUpdater)::cellOnRecycle(cell);

    ZFUIView *separatorHead = cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    if(separatorHead != zfnull) {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorHead);
        cell->internalFgViewRemove(separatorHead);
        cell->objectTagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator_head);
    }
    ZFUIView *separatorTail = cell->objectTag(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    if(separatorTail != zfnull) {
        this->itemCacheRecycle(_ZFP_ZFUIListCellUpdaterBasic_cacheKey_separator, separatorTail);
        cell->internalFgViewRemove(separatorTail);
        cell->objectTagRemove(_ZFP_ZFUIListCellUpdaterBasic_tag_separator);
    }
}

ZF_NAMESPACE_GLOBAL_END


/**
 * @file ZFUIView.h
 * @brief base class of all UI views
 */

#ifndef _ZFI_ZFUIView_h_
#define _ZFI_ZFUIView_h_

#include "ZFUIViewType.h"
#include "ZFUIMouseEvent.h"
#include "ZFUIKeyEvent.h"
#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalImplView "internalImplView"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalBgView "internalBgView"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_internalFgView "internalFgView"

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_child "child"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIView_layoutParam "layoutParam"

// ============================================================
zfclassFwd ZFUIView;
/**
 * @brief see #ZFUIView::nativeImplView
 */
typedef void (*ZFUIViewNativeImplViewDeleteCallback)(
        ZF_IN zfanyT<ZFUIView> const &view
        , ZF_IN void *nativeImplView
        );

// ============================================================
// ZFUIView
zfclassFwd _ZFP_ZFUIViewPrivate;
/**
 * @brief base class of all UI views
 *
 * ZFUIView has these layer of views:
 * -  internal impl view:
 *   for subclass to add views behind native impl view
 * -  internal native view:
 *   for impl to achieve different functions, internal use only
 * -  internal background view:
 *   for subclass to add background views
 * -  normal view:
 *   common children views
 * -  internal foreground view:
 *   for subclass to add foreground views
 *
 * all view layer is implemented internally by simple view management\n
 * \n
 * ZFUIView is serializable and styleable, see #ZFSerializable and #ZFStyleable for more info,
 * all property and normal children views would be serialized and styled automatically,
 * but internal views must be processed by subclass manually\n
 * \n
 * serializable data:
 * @code
 *   <ViewClass>
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalImplView" >
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalBgView" >
 *       </ChildClass>
 *       // optional, see #internalViewAutoSerializeTagAdd
 *       <ChildClass category="internalFgView" >
 *       </ChildClass>
 *
 *       <ChildClass category="child" >
 *           // layout param for parent, optional
 *           <LayoutParamClass category="layoutParam" ... />
 *       </ChildClass>
 *       ... // all children
 *   </ViewClass>
 * @endcode
 * by default, internal views won't be serialized automatically,
 * except matches these condition:
 * -  have the viewId of your internal view been set
 * -  have the same viewId registered by #internalViewAutoSerializeTagAdd
 *
 * \n
 * ADVANCED:\n
 * we allow add native view to ZFUIView environment,
 * for how to, refer to #ZFUINativeViewWrapper\n
 * we also allow add ZFUIView to native view,
 * for how to, refer to #ZFUISysWindow::nativeWindowEmbed
 */
zfclass ZFLIB_ZFUIKit ZFUIView : zfextend ZFStyle {
    ZFOBJECT_DECLARE(ZFUIView, ZFStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #viewTreeInWindow changed
     */
    ZFEVENT(ViewTreeInWindowOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #viewTreeVisible changed
     */
    ZFEVENT(ViewTreeVisibleOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child added or removed or order changed,
     * may be normal child or internal child
     */
    ZFEVENT(ViewChildOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child added to this view,
     * param0 is the child,
     * param1 is #ZFUIViewChildLayer
     */
    ZFEVENT(ViewChildOnAdd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when child removed from this view,
     * param0 is the child,
     * param1 is #ZFUIViewChildLayer
     */
    ZFEVENT(ViewChildOnRemove)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when this view added to parent,
     * param0 is the parent added to
     */
    ZFEVENT(ViewOnAddToParent)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the parent removed from
     */
    ZFEVENT(ViewOnRemoveFromParent)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when this view or parent view's #UIScale or #UIScaleFixed changed
     */
    ZFEVENT(UIScaleOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when view's focus state changed, both obtain or resign
     */
    ZFEVENT(ViewFocusOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is the #ZFUIEvent
     */
    ZFEVENT(ViewOnEvent)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #layoutRequest called
     */
    ZFEVENT(ViewLayoutOnLayoutRequest)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a #ZFUIViewMeasureResult,
     * you may change the measured size to override the measure result
     */
    ZFEVENT(ViewLayoutOnMeasure)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * #viewFrame would be updated before this method,
     * use #viewFramePrev if necessary,
     * you may safely modify children's #layoutParam during this method
     */
    ZFEVENT(ViewLayoutOnLayoutPrepare)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to do actual layout steps\n
     * param0 would be a #v_zfbool init with false,
     * set to true to bypass default layout impl
     */
    ZFEVENT(ViewLayoutOnLayout)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when layout finished,
     * typically you should not modify #layoutParam during this event
     */
    ZFEVENT(ViewLayoutOnLayoutFinish)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #nativeImplViewMarginUpdate and value differs from old
     */
    ZFEVENT(NativeImplViewMarginOnUpdate)

    // ============================================================
    // serialize
public:
    /**
     * @brief store ref layout param for this view for reducing serialization output size
     *
     * if set, while serializing this view's layout param,
     * the ref one would be used as reference object to filter out contents that didn't change
     * (see #ZFSerializable::serializeToData)\n
     * by default, all children would have it's parent's default layout param (#layoutParamCreate)
     * as the ref layout param, during adding to parent
     */
    virtual void serializableRefLayoutParam(ZF_IN ZFUILayoutParam *serializableRefLayoutParam);
    /** @brief see #serializableRefLayoutParam */
    virtual ZFUILayoutParam *serializableRefLayoutParam(void);
protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(
            ZF_IN const ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull
            );
    zfoverride
    virtual zfbool serializableOnSerializeToData(
            ZF_IN_OUT ZFSerializableData &serializableData
            , ZF_OUT_OPT zfstring *outErrorHint = zfnull
            , ZF_IN_OPT ZFSerializable *refOwner = zfnull
            );
    /**
     * @brief whether we should serialize all children
     *
     * by default, #ZFUIView would serialize all normal child views,
     * for some adapter view it may be not necessary,
     * you may override this method to disable the auto serialization of child views
     */
    virtual inline zfbool serializableOnCheckNeedSerializeChildren(void) {
        return zftrue;
    }

public:
    // ============================================================
    // properties
    /**
     * @brief used to identify a view, empty by default
     *
     * this is useful when you want to find a view from a complicated view tree,
     * see #ZFUIView::childFindById
     * @note it's OK that two view have same view id,
     *   however it's recommended to make it unique
     */
    ZFPROPERTY_ASSIGN(zfstring, viewId)

    /**
     * @brief visible or not, zftrue by default
     */
    ZFPROPERTY_ASSIGN(zfbool, visible, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, visible)

    /**
     * @brief true if added to ZFUISysWindow
     *
     * see #E_ViewTreeInWindowOnUpdate
     */
    ZFMETHOD_DECLARE_0(zfbool, viewTreeInWindow)
    /**
     * @brief true only if added to ZFUISysWindow and all of parents are visible
     *
     * see #E_ViewTreeVisibleOnUpdate
     */
    ZFMETHOD_DECLARE_0(zfbool, viewTreeVisible)

    zffinal void _ZFP_ZFUIView_viewTreeInWindow(ZF_IN zfbool viewTreeInWindow);

    /**
     * @brief view's alpha, 1 by default
     */
    ZFPROPERTY_ASSIGN(zffloat, alpha, 1)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, alpha)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, alpha)

    /**
     * @brief whether the view should receive user interaction
     *   (doesn't affect children, see #viewUIEnableTree)
     */
    ZFPROPERTY_ASSIGN(zfbool, viewUIEnable, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, viewUIEnable)

    /**
     * @brief whether the view as well as all its children should receive user interaction,
     *   see #viewUIEnable
     */
    ZFPROPERTY_ASSIGN(zfbool, viewUIEnableTree, zftrue)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, viewUIEnableTree)

    /**
     * @brief true if #viewUIEnable and all of parents are #viewUIEnableTree
     */
    ZFMETHOD_DECLARE_0(zfbool, viewUIEnableFixed)
    /**
     * @brief true if #viewUIEnableTree and all of parents are #viewUIEnableTree
     */
    ZFMETHOD_DECLARE_0(zfbool, viewUIEnableTreeFixed)

    /**
     * @brief whether enable mouse hover event, see #ZFUIView::viewEventOnMouseEvent, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, mouseHoverEnable)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, mouseHoverEnable)

    /**
     * @brief whether the view can be focused, false by default
     */
    ZFPROPERTY_ASSIGN(zfbool, focusable)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, focusable)
    /**
     * @brief whether try to obtain focus when clicked down, true by default
     */
    ZFPROPERTY_ASSIGN(zfbool, focusObtainWhenClick, zftrue)

    /**
     * @brief the view's frame
     *
     * typicall, this property would be updated automatically by parent's #layoutOnLayout,
     * but you may also change this property manually,
     * for example, to achieve custom animation logic,
     * if you do so, the frame would be kept
     * and parent's layout logic would be ignored,
     * until you call #viewFrameReset
     */
    ZFMETHOD_DECLARE_0(ZFUIRect const &, viewFrame)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, viewFrame
            , ZFMP_IN(ZFUIRect const &, viewFrame)
            )

    /** @brief previous #viewFrame */
    ZFMETHOD_DECLARE_0(const ZFUIRect &, viewFramePrev)

    /** @brief true if #viewFrame changed by user, use #viewFrameReset to reset */
    ZFMETHOD_DECLARE_0(zfbool, viewFrameOverrided)

    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(void, viewFrameReset)

    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, x)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, x
            , ZFMP_IN(zffloat const &, propertyValue)
            )
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, y)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, y
            , ZFMP_IN(zffloat const &, propertyValue)
            )
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, width)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, width
            , ZFMP_IN(zffloat const &, propertyValue)
            )
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, height)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, height
            , ZFMP_IN(zffloat const &, propertyValue)
            )
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, centerX)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, centerX
            , ZFMP_IN(zffloat const &, propertyValue)
            )
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_0(zffloat, centerY)
    /** @brief see #viewFrame */
    ZFMETHOD_DECLARE_1(void, centerY
            , ZFMP_IN(zffloat const &, propertyValue)
            )

    /**
     * @brief util method to set fixed size, by setting both of #viewSizeMin and #viewSizeMax
     */
    ZFMETHOD_DECLARE_1(void, viewSizeFixed
            , ZFMP_IN(const ZFUISize &, size)
            )
    /** @brief see #viewSizeFixed */
    ZFMETHOD_DECLARE_2(void, viewSizeFixed
            , ZFMP_IN(zffloat, width)
            , ZFMP_IN(zffloat, height)
            )
    /**
     * @brief min size, #ZFUISizeZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUISize, viewSizeMin)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUISize, viewSizeMin)
    /**
     * @brief max size, negative value means not set, #ZFUISizeInvalid by default
     */
    ZFPROPERTY_ASSIGN(ZFUISize, viewSizeMax, ZFUISizeInvalid())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUISize, viewSizeMax)

    /**
     * @brief background color, #ZFUIColorZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIColor, bgColor, ZFUIColorZero())
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIColor, bgColor)

    // ============================================================
    // transform
    /**
     * @brief whether transform is supported
     *
     * basic 2D transform:
     * -  #translateX
     * -  #translateY
     * -  #scaleX
     * -  #scaleY
     * -  #rotateZ
     *
     * 3D transform:
     * -  #translateX
     * -  #translateY
     * -  #translateZ
     * -  #scaleX
     * -  #scaleY
     * -  #scaleZ
     * -  #rotateX
     * -  #rotateY
     * -  #rotateZ
     *
     * when setting transform which is not supported,
     * it's ensured nothing happen,
     * but the property value is ensured to be updated
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFUITransformFlags, transformAvailable)

    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, translateX, 0)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, translateX)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, translateY, 0)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, translateY)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, translateZ, 0)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, translateZ)

    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, scaleX, 1)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, scaleX)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, scaleY, 1)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, scaleY)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, scaleZ, 1)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, scaleZ)

    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, rotateX, 0)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, rotateX)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, rotateY, 0)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, rotateY)
    /** @brief see #transformAvailable */
    ZFPROPERTY_ASSIGN(zffloat, rotateZ, 0)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, rotateZ)

    // ============================================================
    // util
public:
    /**
     * @brief util method to bind this view to owner's property,
     *   and auto setup #viewId if not set
     *
     * useful in script to build view tree quickly:
     * @code
     *   ZFDynamic()
     *       :classBegin('MyView', 'ZFUIView')
     *       :property('ZFUIView', '_myChild')
     *       :onInit(function(zfargs)
     *           ---@type MyView
     *           local owner = zfargs:sender()
     *           owner
     *               ... // other view tree
     *               :child(ZFUIView():bind(owner, '_myChild')
     *                   ... // other view tree
     *               )
     *
     *           -- now the property can be accessed
     *           owner:_myChild()
     *       end)
     *       :classEnd()
     * @endcode
     */
    ZFMETHOD_DECLARE_2(void, bind
            , ZFMP_IN(ZFObject *, owner)
            , ZFMP_IN(const zfstring &, propertyName)
            )
    /**
     * @brief util method to bind this view's event to owner's method
     *
     * useful in script to build view tree quickly:
     * @code
     *   ZFDynamic()
     *       :classBegin('MyView', 'ZFUIView')
     *       :onInit(function(zfargs)
     *           ---@type MyView
     *           local owner = zfargs:sender()
     *           owner
     *               ... // other view tree
     *               :child(ZFUIButtonBasic():bindEvent('ButtonOnClick', owner, '_myOnClick')
     *                   ... // other view tree
     *               )
     *       end)
     *       :method('void', '_myOnClick', ZFMP(), function(zfargs)
     *           ...
     *       end)
     *       :classEnd()
     * @endcode
     *
     * see #ZFObject::on for event name search logic\n
     * \n
     * the owner's method's return value would be ignored, and params can be
     * -  method with no params
     * -  exactly one param with #v_ZFArgs type, which was the zfargs from the source event
     * -  exactly two param with ZFObject type (or sub class of ZFObject), which was the param0 and param1 from the source event
     *
     * note, when multiple method matches, which one to use would not be ensured,
     * try to prevent that
     */
    ZFMETHOD_DECLARE_3(void, bindEvent
            , ZFMP_IN(const zfstring &, eventName)
            , ZFMP_IN(ZFObject *, owner)
            , ZFMP_IN(const zfstring &, methodName)
            )
    /**
     * @brief util method to bind this view's event to owner's method, see #bindEvent
     */
    ZFMETHOD_DECLARE_3(void, bindEvent
            , ZFMP_IN(zfidentity, eventId)
            , ZFMP_IN(ZFObject *, owner)
            , ZFMP_IN(const zfstring &, methodName)
            )

    // ============================================================
    // init and dealloc
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoImplAppend(ZF_IN_OUT zfstring &ret);

    // ============================================================
public:
    /**
     * @brief native implementation view
     *
     * first child of #nativeView, layout below all other child views,
     * for internal implementation use only,
     * for example, a ImageView's implementation may use native view
     * and set it as internalNativeView\n
     * subclass must not override parent,
     * if really necessary, use #internalBgViewAdd\n
     * use with caution
     * @see nativeView
     */
    ZFMETHOD_DECLARE_0(void *, nativeImplView)
    /**
     * @brief whether the #nativeImplView cost one virtualIndex
     */
    ZFMETHOD_DECLARE_0(zfbool, nativeImplViewRequireVirtualIndex)
    /**
     * @brief inner margin between #ZFUIView and the internal #nativeImplView
     *
     * use #nativeImplViewMarginUpdate to update this value,
     * and it's ensured to be called during #ZFObject::objectOnInitFinish\n
     * subclass should override #nativeImplViewMarginImplUpdate to implement custom margin,
     * and manually call #nativeImplViewMarginUpdate if necessary\n
     * this value can also be controlled by app level code by #nativeImplViewMarginCustom
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, nativeImplViewMargin)
    /**
     * @brief see #nativeImplViewMargin,
     *   #layoutRequest if the final value actually changed
     */
    ZFMETHOD_DECLARE_0(void, nativeImplViewMarginUpdate)
    /**
     * @brief frame of #nativeImplView
     */
    ZFMETHOD_DECLARE_0(const ZFUIRect &, nativeImplViewFrame)

    /**
     * @brief see #nativeImplViewMargin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, nativeImplViewMarginCustom)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIMargin, nativeImplViewMarginCustom)

protected:
    /**
     * @brief see #nativeImplView
     */
    virtual void nativeImplView(
            ZF_IN void *nativeImplView
            , ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback
            , ZF_IN zfbool nativeImplViewRequireVirtualIndex
            );
    /**
     * @brief see #nativeImplViewMargin,
     *   subclass must call super and "append" to existing margin
     */
    virtual inline void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin) {
    }
    /**
     * @brief see #E_NativeImplViewMarginOnUpdate
     */
    virtual inline void nativeImplViewMarginOnUpdate(void) {
        this->observerNotify(ZFUIView::E_NativeImplViewMarginOnUpdate());
    }
    /**
     * @brief called to layout #nativeImplView
     */
    virtual void nativeImplViewOnLayout(
            ZF_OUT ZFUIRect &ret
            , ZF_IN const ZFUIRect &bounds
            , ZF_IN const ZFUIMargin &nativeImplViewMargin
            ) {
        ZFUIRectApplyMarginT(ret, bounds, nativeImplViewMargin);
    }

    // ============================================================
protected:
    /**
     * @brief called to add or remove view to impl
     *
     * subclass may override this method to add child to other container\n
     * to implement this, you must implement all these methods:
     * -  #implChildOnAdd : called to add to impl
     * -  #implChildOnRemove : called to remove from impl
     * -  #implChildOnRemoveAllForDealloc : called to remove from impl during parent dealloc
     */
    virtual void implChildOnAdd(
            ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            );
    /**
     * @brief see #implChildOnAdd, #implChildOnRemoveAllForDealloc
     */
    virtual void implChildOnRemove(
            ZF_IN ZFUIView *child
            , ZF_IN zfindex virtualIndex
            , ZF_IN ZFUIViewChildLayer childLayer
            , ZF_IN zfindex childLayerIndex
            );
    /**
     * @brief called to remove all children during parent dealloc for performance
     *
     * for normal children management,
     * each child would be removed one by one and fire child change event when #childRemoveAll,
     * which is not necessary during parent dealloc
     * (which may cause performance issue),
     * so we use this method to remove all children directly during parent dealloc
     * to improve performance
     */
    virtual void implChildOnRemoveAllForDealloc(void);

public:
    /**
     * @brief native container view
     *
     * the actual type of this is defined by implementation,
     * and it's not recommended to use in your application\n
     * \n
     * for how to add ZFUIView to native view, please refer to #ZFUIView\n
     * for how to add native view to ZFUIView, please refer to #ZFUINativeViewWrapper\n
     * for how to access native implementation, please refer to #nativeImplView
     */
    ZFMETHOD_DECLARE_0(void *, nativeView)
    static void _ZFP_ZFUIView_nativeViewNotifyAdd(
            ZF_IN ZFUIView *view
            , ZF_IN void *nativeParentView
            );
    static void _ZFP_ZFUIView_nativeViewNotifyRemove(ZF_IN ZFUIView *view);

    // ============================================================
    // view focus
public:
    /**
     * @brief whether the view currently focused
     */
    ZFMETHOD_DECLARE_0(zfbool, focused)
    /**
     * @brief request to obtain or resign focus,
     *   result can be checked by #focused
     *
     * only focused view can receive key events
     */
    ZFMETHOD_DECLARE_1(void, focusRequest
            , ZFMP_IN(zfbool, focus)
            )
    zffinal void _ZFP_ZFUIView_focusOnUpdate(void) {
        this->focusOnUpdate();
    }
    /**
     * @brief recursively to find focused child, take care of performance
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, focusFind)
protected:
    /** @brief see #E_ViewFocusOnUpdate */
    virtual inline void focusOnUpdate(void) {
        this->observerNotify(ZFUIView::E_ViewFocusOnUpdate());
    }

    // ============================================================
    // parent
public:
    zffinal void _ZFP_ZFUIView_parentOnUpdate(
            ZF_IN ZFUIView *parentView
            , ZF_IN ZFUILayoutParam *layoutParam
            , ZF_IN ZFUIViewChildLayer viewLayer
            );
    /**
     * @brief parent view or null if none
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUIView>, parent)

    /**
     * @brief remove this view from parent or do nothing if no parent
     *
     * can remove normal child view or internal view
     */
    ZFMETHOD_DECLARE_0(void, removeFromParent)

    // ============================================================
    // scale settings
public:
    /**
     * @brief UI scale for view tree
     *
     * ZFUIView use a special scale logic to adapt various screen sizes,
     * which contain these scale values:
     * -  app scale:\n
     *   app's custom scale, accessed by #UIScale, 1 by default\n
     *   you may change this view to apply scale for entire view tree,
     *   every child view may have different scale setting,
     *   the final scale can be accessed by #UIScaleInherited
     * -  impl scale:\n
     *   accessed by #UIScaleForImpl, it's value depends on impl\n
     *   automatically changed while adding a ZFUIView to native view,
     *   can not be changed by app
     * -  fixed scale:\n
     *   accessed by #UIScaleFixed, always equal to
     *   (#UIScaleInherited * #UIScale * #UIScaleForImpl)\n
     *   all size unit would be applied with this value
     *   before passing to implementations,
     *   can not be changed by app
     * -  impl physical scale:\n
     *   accessed by #UIScaleForPixel, it's value depends on impl\n
     *   to access final pixel size:
     *   view's size * #UIScaleInherited * #UIScale * #UIScaleForPixel
     *
     * in general:
     * -  for app:\n
     *   usually you have no need to worry about scales,
     *   all elements can be assigned as fixed size,
     *   such as 48 for a button's height and 21 for a small textView's height,
     *   it will be scaled automatically while rendering to different size's or DPI's devices\n
     *   you may use #UIScale to control custom scale logic
     * -  for implementation:\n
     *   you have no need to worry about element's logical size,
     *   everything would be scaled to desired size unit
     *   depending on the scale value that impl returned
     *
     * \n
     * since scale may affect impl's pixel size,
     * size-related property should be flushed manually while scale changed,
     * subclass should override #UIScaleOnUpdate to update them,
     * which would be called if #UIScaleFixed really changed
     * \n
     * #UIScale usually used for scale for entire view tree,
     * all layout and touch position would be scaled properly,
     * however, changing #UIScale for a deep view tree may consume much time\n
     * for temporarily scale, typically for animation,
     * use #scaleX instead
     */
    ZFPROPERTY_ASSIGN(zffloat, UIScale, 1)
    ZFPROPERTY_ON_UPDATE_DECLARE(zffloat, UIScale)
    ZFPROPERTY_ON_ATTACH_DECLARE(zffloat, UIScale)
    /** @brief see #UIScale */
    ZFMETHOD_DECLARE_0(zffloat, UIScaleInherited)
    /** @brief see #UIScale */
    ZFMETHOD_DECLARE_0(zffloat, UIScaleForImpl)
    /** @brief see #UIScale */
    ZFMETHOD_DECLARE_0(zffloat, UIScaleForPixel)
    /** @brief see #UIScale */
    ZFMETHOD_DECLARE_0(zffloat, UIScaleFixed)
protected:
    /**
     * @brief see #UIScale, ensured called only when scale value actually changed
     *
     * after this method, #E_UIScaleOnUpdate would be fired
     */
    virtual void UIScaleOnUpdate(void);

    // ============================================================
    // layout logic
public:
    /**
     * @brief create layout param,
     *   calling #layoutParamClass to create instance
     *   and #layoutParamOnUpdate to update
     */
    ZFMETHOD_DECLARE_0(zfautoT<ZFUILayoutParam>, layoutParamCreate)
protected:
    /**
     * @brief see #layoutParamCreate
     *
     * you should override this method to declare your layout param class
     */
    virtual const ZFClass *layoutParamClass(void);
    /**
     * @brief see #layoutParamCreate
     */
    virtual inline void layoutParamOnUpdate(ZF_IN ZFUILayoutParam *layoutParam) {
    }

public:
    /**
     * @brief manually set layout param
     *
     * this method can be called even if this view has no parent,
     * the layout param would be serialized while serializing the view itself\n
     * while adding to another container view with different layout param type,
     * a new layout param would be created and applied style from the existing one
     */
    ZFMETHOD_DECLARE_1(void, layoutParam
            , ZFMP_IN(ZFUILayoutParam *, layoutParam)
            )
    /**
     * @brief get self's layout param, valid only while the view has parent
     *
     * return null if the view has no parent,
     * automatically invoke the view's #layoutRequest if the layout param's property changed
     */
    ZFMETHOD_DECLARE_0(zfanyT<ZFUILayoutParam>, layoutParam)

public:
    /**
     * @brief set need layout
     */
    ZFMETHOD_DECLARE_0(void, layoutRequest)
    /**
     * @brief true if need layout
     */
    ZFMETHOD_DECLARE_0(zfbool, layoutRequested)
    /**
     * @brief true if currently being layouted
     */
    ZFMETHOD_DECLARE_0(zfbool, layouting)
    /**
     * @brief measure the view
     *
     * call #layoutOnMeasure to see the needed size for this view\n
     * note that internal views won't be measured
     * @see layoutMeasuredSize
     */
    ZFMETHOD_DECLARE_2(const ZFUISize &, layoutMeasure
            , ZFMP_IN(const ZFUISize &, sizeHint)
            , ZFMP_IN(const ZFUISizeParam &, sizeParam)
            )
    /**
     * @brief get measured size, invalid if not measured
     */
    ZFMETHOD_DECLARE_0(const ZFUISize &, layoutMeasuredSize)
    /**
     * @brief force to layout if need
     */
    ZFMETHOD_DECLARE_0(void, layoutIfNeed)
    /**
     * @brief get child offset to this view
     *
     * for views that have offset logic (typically scroll views),
     * use this method to access the offset to its parent,
     * child's #viewFrame plus this offset should be the actual
     * offset to parent's edge\n
     * subclass should override #layoutChildOffsetOnUpdate
     * to supply this value
     */
    ZFMETHOD_DECLARE_0(ZFUIPoint, layoutChildOffset)

public:
    void _ZFP_ZFUIView_notifyLayoutView(ZF_IN const ZFUIRect &viewFrame);
protected:
    /**
     * @brief called during #layoutRequest
     */
    virtual void layoutOnLayoutRequest(void);
    /**
     * @brief called by #layoutMeasure to decide the view's size
     *
     * you may override without call super to supply your own layout logic\n
     * \n
     * note that we doesn't ensure layoutOnMeasure would be called during layout steps,
     * only layout that has wrap content features may call #layoutMeasure
     * to calculate children's size\n
     * \n
     * sizeHint means the max size child should reach, see #ZFUISizeType for more info\n
     * \n
     * return a negative value means the view doesn't care about size\n
     * \n
     * @note there's some repeatly work that #layoutMeasure would have done for you,
     *   you should not repeat it again for performance:
     *   -  filter out the case that both sizeParam are fixed or fill parent
     *   -  apply sizeHint by #ZFUILayoutParam::sizeHintApply
     *   -  fix result size in range [viewSizeMin, viewSizeMax]
     * @note for impl views (such as text view),
     *   you should manually take care of #nativeImplViewMargin
     *   during measure and layout steps
     */
    virtual inline void layoutOnMeasure(
            ZF_OUT ZFUISize &ret
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            ) {
    }
    /** @brief see #E_ViewLayoutOnMeasure */
    virtual inline void layoutOnMeasureFinish(
            ZF_IN_OUT ZFUISize &measuredSize
            , ZF_IN const ZFUISize &sizeHint
            , ZF_IN const ZFUISizeParam &sizeParam
            ) {
    }
    /** @brief see #E_ViewLayoutOnLayoutPrepare */
    virtual inline void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds) {
    }
    /**
     * @brief called by #viewFrame to layout the view and children
     *
     * it's valid for subclass to override without calling zfsuper::layoutOnLayout,
     * which means subclass would override all layout steps in parent\n
     * \n
     * note that we doesn't ensure layoutOnMeasure would be called before layoutOnLayout
     */
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
    /** @brief see #E_ViewLayoutOnLayoutFinish */
    virtual inline void layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds) {
    }
    /** @brief see #layoutChildOffset */
    virtual inline void layoutChildOffsetOnUpdate(ZF_IN_OUT ZFUIPoint &ret) {
    }

    // ============================================================
    // children management
public:
    /**
     * @brief find view by viewId, return the view or null if not found
     */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUIView>, childFindById
            , ZFMP_IN(const zfstring &, viewId)
            , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
            , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
            )
    /**
     * @brief find view by class, return the view or null if not found
     */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUIView>, childFindByClass
            , ZFMP_IN(const ZFClass *, cls)
            , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
            , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
            )
    /**
     * @brief call custom impl for each child
     *
     * the impl's sender would be the owner parent view that calling this method,
     * param0 would be each child being checking,
     * impl should set #ZFArgs::eventFiltered if process done,
     * and may set #ZFArgs::result which would return as return value of this method
     */
    ZFMETHOD_DECLARE_3(zfauto, childForEach
            , ZFMP_IN(const ZFListener &, impl)
            , ZFMP_IN_OPT(zfbool, findRecursively, zftrue)
            , ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse)
            )

public:
    /**
     * @brief add view with layout param, param must be created by #layoutParamCreate
     *
     * if layoutParam is null (by default), create new one by #layoutParamCreate\n
     * if layoutParam is type of #layoutParamClass and not null, it would be used directly,
     * otherwise, a new layout param would be created
     * and source layout param would be copied to the newly created layout param
     */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUILayoutParam>, childWithParam
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN(ZFUILayoutParam *, layoutParam)
            , ZFMP_IN_OPT(zfindex, atIndex, zfindexMax())
            )
    /**
     * @brief add child and return child's #layoutParam, see #childWithParam
     */
    zffinal zfanyT<ZFUILayoutParam> child(
            ZF_IN const zfany &view
            , ZF_IN_OPT zfindex atIndex = zfindexMax()
            );
    /**
     * @brief remove view or do nothing if view isn't added to this view
     */
    ZFMETHOD_DECLARE_1(void, childRemove
            , ZFMP_IN(ZFUIView *, view)
            )
    /**
     * @brief remove view at index or assert fail if index out of range
     */
    ZFMETHOD_DECLARE_1(void, childRemoveAt
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief remove all child view
     */
    ZFMETHOD_DECLARE_0(void, childRemoveAll)
    /**
     * @brief move view, make toIndexOrIndexMax as zfindexMax() to move to top most,
     *   and 0 to bottom most, do nothing if index invalid or have no change
     *
     * assert fail if fromIndex out of range, or toIndexOrIndexMax isn't zfindexMax() and out of range\n
     * moving a view would cause #layoutRequest being called
     */
    ZFMETHOD_DECLARE_2(void, childMove
            , ZFMP_IN(zfindex, fromIndex)
            , ZFMP_IN(zfindex, toIndexOrIndexMax)
            )
    /**
     * @brief see #childMove
     */
    ZFMETHOD_DECLARE_2(void, childMove
            , ZFMP_IN(ZFUIView *, child)
            , ZFMP_IN(zfindex, toIndexOrIndexMax)
            )
    /**
     * @brief replace child at index, assert fail if index out of range
     *   or view to replace already has parent
     */
    ZFMETHOD_DECLARE_2(void, childReplaceAt
            , ZFMP_IN(zfindex, atIndex)
            , ZFMP_IN(ZFUIView *, toReplace)
            )
    /**
     * @brief get child view count
     */
    ZFMETHOD_DECLARE_0(zfindex, childCount)
    /**
     * @brief get child view at index or assert fail if out of range
     */
    ZFMETHOD_DECLARE_1(zfanyT<ZFUIView>, childAt
            , ZFMP_IN(zfindex, index)
            )
    /**
     * @brief return index of view or zfindexMax() if not child of this view
     */
    ZFMETHOD_DECLARE_1(zfindex, childFind
            , ZFMP_IN(ZFUIView *, view)
            )
    /**
     * @brief get the child view array
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoT<ZFUIView> >, childArray)

    /**
     * @brief this view belongs to which layer of parent,
     *   valid only if #parent is not null
     *
     * would be #v_ZFUIViewChildLayer::e_Normal if no parent
     */
    ZFMETHOD_DECLARE_0(ZFUIViewChildLayer, viewLayer)
    /**
     * @brief return all children including internal views, see #childArray
     *
     * children are ensured ordered by (impl, bg, normal, fg) views
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoT<ZFUIView> >, childRawArray)

    // ============================================================
    // events
protected:
    /** @brief see #E_ViewTreeInWindowOnUpdate */
    virtual void viewTreeInWindowOnUpdate(void);
    /** @brief see #E_ViewTreeVisibleOnUpdate */
    virtual void viewTreeVisibleOnUpdate(void);
    /** @brief see #E_ViewChildOnUpdate */
    virtual void viewChildOnUpdate(void);
    /** @brief see #E_ViewChildOnAdd */
    virtual void viewChildOnAdd(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayer childLayer
            );
    /** @brief see #E_ViewChildOnRemove */
    virtual void viewChildOnRemove(
            ZF_IN ZFUIView *child
            , ZF_IN ZFUIViewChildLayer childLayer
            );
    /** @brief see #E_ViewOnAddToParent */
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    /** @brief see #E_ViewOnRemoveFromParent */
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

    // ============================================================
    // internal views
public:
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUILayoutParam>, internalImplViewAdd
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
            , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
            )
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalImplViewRemove
            , ZFMP_IN(ZFUIView *, view)
            )
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoT<ZFUIView> >, internalImplViewArray)

public:
    /**
     * @brief internal view which is independent from normal view
     *
     * we have these layers in ZFUIView module:
     * -  internal impl view
     * -  internal background view
     * -  normal view
     * -  internal foreground view
     *
     * each layer is independent and has the same interface to add or remove view\n
     * to make the interfaces cleaner, the internal ones are named with
     * "internalBgView" and "internalFgView" as pre-fix,
     * such as #internalBgViewAdd and #internalBgViewRemove
     * (considering the #child and #childRemove)\n
     * \n
     * internal views has no measure steps, its size always depends on parent's size
     */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUILayoutParam>, internalBgViewAdd
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
            , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
            )
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalBgViewRemove
            , ZFMP_IN(ZFUIView *, view)
            )
    /**
     * @brief usually for debug use only, try to avoid use this in your app for other purpose
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoT<ZFUIView> >, internalBgViewArray)

public:
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_3(zfanyT<ZFUILayoutParam>, internalFgViewAdd
            , ZFMP_IN(ZFUIView *, view)
            , ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull)
            , ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue)
            )
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_1(void, internalFgViewRemove
            , ZFMP_IN(ZFUIView *, view)
            )
    /** @brief see #internalBgViewAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoT<ZFUIView> >, internalFgViewArray)

    // ============================================================
    // other internal view logic
public:
    /**
     * @brief used to apply auto serialize logic to internal views
     *
     * by default, internal views won't be serialized automatically,
     * you may make it available by:
     * -  have internal view's viewId set
     * -  have same id set by this method
     *
     * while serializing, if an internal view with same id already exists,
     * we would copy style from serialize data to the existing one instead
     */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagAdd
            , ZFMP_IN(const zfstring &, tag)
            )
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagRemove
            , ZFMP_IN(const zfstring &, tag)
            )
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_0(void, internalViewAutoSerializeTagRemoveAll)
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_1(void, internalViewAutoSerializeTagGetAllT
            , ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret)
            )
    /** @brief see #internalViewAutoSerializeTagAdd */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfstring>, internalViewAutoSerializeTagGetAll)

protected:
    /**
     * @brief called to check whether the internal view should be layouted using default layout logic,
     *   return true by default
     */
    virtual inline zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView) {
        return zftrue;
    }
    /** @brief see #internalBgViewAdd */
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);

    // ============================================================
    // UI events
public:
    /**
     * @brief directly send a event, use with caution
     */
    ZFMETHOD_DECLARE_1(void, viewEventSend
            , ZFMP_IN(ZFUIEvent *, event)
            )
protected:
    /**
     * @brief notified when a ZFUIEvent occurred
     *
     * default behavior is to dispatch event depends on event type\n
     * you may override without call super's method, to override the event\n
     * you should update #ZFUIEvent::eventResolved if resolved
     */
    virtual void viewEventOnEvent(ZF_IN ZFUIEvent *event);

    // ============================================================
    // events
protected:
    /**
     * @brief called when mouse event occurred
     *
     * due to some limitations, we doesn't support intercept mouse event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * mouse hover event would only be fired if #mouseHoverEnable\n
     * \n
     * by default, this method would simply resolve the event if this view is enabled,
     * you may override without call super's method, to override the event
     */
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);

    /**
     * @brief called when key occurred
     *
     * due to some limitations, we doesn't support intercept key event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * by default, this method would call #viewEventOnKeyEventResolveFocus to achieve focus move,
     * you may override without call super's method, to override the event\n
     * the event would be dispatched from child to parent,
     * util it's resolved
     */
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);
    /**
     * @brief called by #viewEventOnKeyEvent to resolve focus move key event
     *
     * this method would call #ZFUIViewFocusResolveKeyEvent to achieve focus move,
     * you may override without call super's method, to override the event
     */
    virtual void viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent);

    /**
     * @brief called when wheel event occurred
     *
     * due to some limitations, we doesn't support intercept wheel event,
     * event dispatch logic depends on implementation,
     * you may use native view to achieve if necessary\n
     * \n
     * the event would be dispatched from child to parent,
     * util it's resolved
     */
    virtual void viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent);

    // ============================================================
    // override
protected:
    /**
     * @brief for a view, copy style would also copy all of it's children
     */
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFObject *anotherStyleable);

protected:
    zfoverride
    virtual void observerOnAdd(ZF_IN zfidentity eventId);
    zfoverride
    virtual void observerOnRemove(ZF_IN zfidentity eventId);

private:
    _ZFP_ZFUIViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIViewPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIView_h_


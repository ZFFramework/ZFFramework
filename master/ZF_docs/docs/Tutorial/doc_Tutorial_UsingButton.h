/**
 * @page DocTag_Tutorial_UsingButton Using button
 *
 * with observer logic of ZFObject,
 * it's easy to listen on button click event
 * @code
 *   zfblockedAlloc(ZFUIButtonBasic, button);
 *
 *   // declare button click listener
 *   ZFLISTENER(onClickListener) {
 *       zfLog() << zfargs.sender() << " clicked";
 *   } ZFLISTENER_END()
 *
 *   // add as observer
 *   button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener);
 * @endcode
 */


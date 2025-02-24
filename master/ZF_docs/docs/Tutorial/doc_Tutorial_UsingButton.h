/**
 * @page DocTag_Tutorial_UsingButton Using button
 *
 * with observer logic of ZFObject,
 * it's easy to listen on button click event
 * @code
 *   zfobj<ZFUIButtonBasic> button;
 *
 *   // declare button click listener
 *   ZFLISTENER(onClickListener) {
 *       ZFLog() << zfargs.sender() << " clicked";
 *   } ZFLISTENER_END()
 *
 *   // add as observer
 *   button->observerAdd(ZFUIButton::E_ButtonOnClick(), onClickListener);
 * @endcode
 */


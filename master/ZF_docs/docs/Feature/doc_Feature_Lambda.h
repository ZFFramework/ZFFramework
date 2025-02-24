/**
 * @page DocTag_Feature_Lambda Lambda with pure C++03
 *
 * thanks to powerful generic #ZFCallback,
 * we are able to implement lambda feature with pure C++03
 * \n
 * example:
 * @code
 *   ZFUIRect refCapture = ZFUIRectZero();
 *   ZFUIRect valueCapture = ZFUIRectZero();
 *
 *   ZFLISTENER_2(onClick
 *           , ZFUIRect &, refCapture
 *           , ZFUIRect, valueCapture
 *           ) {
 *       refCapture.width = 999;
 *       valueCapture.width = 999;
 *   } ZFLISTENER_END()
 *
 *   zfobj<ZFUIButton> btn;
 *   btn->observerAdd(ZFUIButton::E_ButtonOnClick(), onClick);
 *
 *   ZFLogTrim() << "before invoke: " << refCapture << valueCapture;
 *   btn->simulateClick();
 *   ZFLogTrim() << "after invoke: " << refCapture << valueCapture;
 *
 *   // output:
 *   //   before invoke: (0, 0, 0, 0) (0, 0, 0, 0)
 *   //   after invoke: (0, 0, 999, 0) (0, 0, 0, 0)
 * @endcode
 * \n
 * as you may noticed,
 * we can capture vars outside of the lambda function\n
 * you should take care of the life circle of the captured vars, though
 */


/**
 * @page DocTag_Tutorial_HelloWorldWithUI Hello world with UI
 *
 * time to start a quick hello world with UI elements:
 * @code
 *   // include ZFUIKit.h for UI module of ZFFramework
 *   // you may need to setup your project's include path first
 *   #include "ZFUIKit.h"
 *
 *   ZFMAIN_ENTRY() {
 *       // typically we will start UI by creating a ZFUIWindow
 *       zfobj<ZFUIWindow> window;
 *       // show the window
 *       window->show();
 *
 *       // create a text view and set text content
 *       zfobj<ZFUITextView> textView;
 *       textView->text("hello world");
 *
 *       // add to window
 *       window->child(textView);
 *
 *       // make it center in window
 *       textView->layoutParam()->align(v_ZFUIAlign::e_Center);
 *   }
 * @endcode
 */


/**
 * @page DocTag_Setup_Android Android
 *
 * @section DocTag_Setup_Android_Requirement Requirement
 *
 * you must have these settings being prepared:
 * -  have <a href="https://developer.android.com/ndk/index.html">Android NDK</a> installed and setup properly
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup for quick setup your project files
 *
 * @section DocTag_Setup_Android_ManualSetup Manual Setup
 *
 * if you really want to setup manually:
 *
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own Android project
 * -# merge all src files under _release dir to your project's path,
 *   or use the Android aar file
 * -# set up JNI env like other Android JNI project,
 *   especially to specify header include path and so lib path of ZFFramework,
 *   which located in ~/_release
 * -# in your Android project's AndroidManifest.xml,
 *   add these activity declaration to your \<application\> node:
 *   @code
 *     <activity
 *         android:name="com.ZFFramework.ZF_impl.ZFMainEntry"
 *         android:label="ZFFramework"
 *         android:launchMode="singleTask">
 *     </activity>
 *     <activity
 *         android:name="com.ZFFramework.ZFUIKit_impl.ZFUIRootWindow"
 *         android:configChanges="orientation|keyboardHidden|screenSize"
 *         android:label="ZFFramework"
 *         android:windowSoftInputMode="adjustResize" >
 *     </activity>
 *   @endcode
 *   and these permission:
 *   @code
 *     <uses-permission android:name="android.permission.INTERNET" />
 *   @endcode
 * -# in your Application,
 *   load so library
 *   @code
 *     static {
 *         System.loadLibrary("your_app_lib_name");
 *     }
 *   @endcode
 * -# create a loader activity,
 *   start ZFMainEntry then finish itself:
 *   @code
 *     @Override
 *     protected void onCreate(Bundle savedInstanceState) {
 *         requestWindowFeature(Window.FEATURE_NO_TITLE);
 *         super.onCreate(savedInstanceState);
 *         if (BuildConfig.DEBUG) {
 *             ZFMainEntry.debugMode(true);
 *         }
 *         ZFMainEntry.mainEntryFromLoader(this);
 *     }
 *   @endcode
 * -# ensure all of your resource files put in "src/main/assets/zfres" directory of your Android project,
 *   and ensured all of them have an extra ".mp2" as file name postfix if they are large than 1M bytes
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial to code with ZFFramework, enjoy
 */


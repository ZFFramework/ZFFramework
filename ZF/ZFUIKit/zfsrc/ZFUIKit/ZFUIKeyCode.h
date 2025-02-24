/**
 * @file ZFUIKeyCode.h
 * @brief key code define
 */

#ifndef _ZFI_ZFUIKeyCode_h_
#define _ZFI_ZFUIKeyCode_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief key action for ZFUIKeyCode
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIKeyAction)
    ZFENUM_VALUE(Down)      /**< @brief key down */
    ZFENUM_VALUE(Repeat)    /**< @brief key repeat */
    ZFENUM_VALUE(Up)        /**< @brief key up */
    ZFENUM_VALUE(Cancel)    /**< @brief key cancel */
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(Down)
    ZFENUM_VALUE_REGISTER(Repeat)
    ZFENUM_VALUE_REGISTER(Up)
    ZFENUM_VALUE_REGISTER(Cancel)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIKeyAction)

// ============================================================
/**
 * @brief key code
 */
ZFENUM_BEGIN(ZFLIB_ZFUIKit, ZFUIKeyCode)
    ZFENUM_VALUE(kUnknown = 1)

    ZFENUM_VALUE(kEsc = 100)
    ZFENUM_VALUE(kTab = 101)
    ZFENUM_VALUE(kBackspace = 102)
    ZFENUM_VALUE(kEnter = 103)
    ZFENUM_VALUE(kSpace = 104)
    ZFENUM_VALUE(kCapsLock = 105)
    ZFENUM_VALUE(kShift = 106)
    ZFENUM_VALUE(kCtrl = 107)
    ZFENUM_VALUE(kAlt = 108) /**< @brief Alt under Windows, Option under MacOS */
    ZFENUM_VALUE(kCommand = 109) /**< @brief Windows under Windows, Command under MacOS */
    ZFENUM_VALUE(kContextMenu = 110)

    ZFENUM_VALUE(kF1 = 201)
    ZFENUM_VALUE(kF2 = 202)
    ZFENUM_VALUE(kF3 = 203)
    ZFENUM_VALUE(kF4 = 204)
    ZFENUM_VALUE(kF5 = 205)
    ZFENUM_VALUE(kF6 = 206)
    ZFENUM_VALUE(kF7 = 207)
    ZFENUM_VALUE(kF8 = 208)
    ZFENUM_VALUE(kF9 = 209)
    ZFENUM_VALUE(kF10 = 210)
    ZFENUM_VALUE(kF11 = 211)
    ZFENUM_VALUE(kF12 = 212)

    ZFENUM_VALUE(k0 = 300)
    ZFENUM_VALUE(k1 = 301)
    ZFENUM_VALUE(k2 = 302)
    ZFENUM_VALUE(k3 = 303)
    ZFENUM_VALUE(k4 = 304)
    ZFENUM_VALUE(k5 = 305)
    ZFENUM_VALUE(k6 = 306)
    ZFENUM_VALUE(k7 = 307)
    ZFENUM_VALUE(k8 = 308)
    ZFENUM_VALUE(k9 = 309)

    ZFENUM_VALUE(kA = 401)
    ZFENUM_VALUE(kB = 402)
    ZFENUM_VALUE(kC = 403)
    ZFENUM_VALUE(kD = 404)
    ZFENUM_VALUE(kE = 405)
    ZFENUM_VALUE(kF = 406)
    ZFENUM_VALUE(kG = 407)
    ZFENUM_VALUE(kH = 408)
    ZFENUM_VALUE(kI = 409)
    ZFENUM_VALUE(kJ = 410)
    ZFENUM_VALUE(kK = 411)
    ZFENUM_VALUE(kL = 412)
    ZFENUM_VALUE(kM = 413)
    ZFENUM_VALUE(kN = 414)
    ZFENUM_VALUE(kO = 415)
    ZFENUM_VALUE(kP = 416)
    ZFENUM_VALUE(kQ = 417)
    ZFENUM_VALUE(kR = 418)
    ZFENUM_VALUE(kS = 419)
    ZFENUM_VALUE(kT = 420)
    ZFENUM_VALUE(kU = 421)
    ZFENUM_VALUE(kV = 422)
    ZFENUM_VALUE(kW = 423)
    ZFENUM_VALUE(kX = 424)
    ZFENUM_VALUE(kY = 425)
    ZFENUM_VALUE(kZ = 426)

    ZFENUM_VALUE(kGrave = 501)
    ZFENUM_VALUE(kMinus = 502)
    ZFENUM_VALUE(kEquals = 503)
    ZFENUM_VALUE(kBracketLeft = 504)
    ZFENUM_VALUE(kBracketRight = 505)
    ZFENUM_VALUE(kBackSlash = 506)
    ZFENUM_VALUE(kSemiColon = 507)
    ZFENUM_VALUE(kApostrophe = 508)
    ZFENUM_VALUE(kComma = 509)
    ZFENUM_VALUE(kPeriod = 510)
    ZFENUM_VALUE(kSlash = 511)

    ZFENUM_VALUE(kLeft = 601)
    ZFENUM_VALUE(kUp = 602)
    ZFENUM_VALUE(kRight = 603)
    ZFENUM_VALUE(kDown = 604)
    ZFENUM_VALUE(kHome = 605)
    ZFENUM_VALUE(kEnd = 606)
    ZFENUM_VALUE(kPageUp = 607)
    ZFENUM_VALUE(kPageDown = 608)
    ZFENUM_VALUE(kInsert = 609)
    ZFENUM_VALUE(kDelete = 610)
    ZFENUM_VALUE(kPrintScreen = 611)
    ZFENUM_VALUE(kScrollLock = 612)
    ZFENUM_VALUE(kPauseBreak = 613)

    ZFENUM_VALUE(kNum0 = 700)
    ZFENUM_VALUE(kNum1 = 701)
    ZFENUM_VALUE(kNum2 = 702)
    ZFENUM_VALUE(kNum3 = 703)
    ZFENUM_VALUE(kNum4 = 704)
    ZFENUM_VALUE(kNum5 = 705)
    ZFENUM_VALUE(kNum6 = 706)
    ZFENUM_VALUE(kNum7 = 707)
    ZFENUM_VALUE(kNum8 = 708)
    ZFENUM_VALUE(kNum9 = 709)
    ZFENUM_VALUE(kNumLock = 710)
    ZFENUM_VALUE(kNumAdd = 711)
    ZFENUM_VALUE(kNumMinus = 712)
    ZFENUM_VALUE(kNumMultiply = 713)
    ZFENUM_VALUE(kNumDivide = 714)
    ZFENUM_VALUE(kNumEnter = 715)
    ZFENUM_VALUE(kNumDot = 716)

    ZFENUM_VALUE(kMediaPlay = 801)
    ZFENUM_VALUE(kMediaPause = 802)
    ZFENUM_VALUE(kMediaPlayPause = 803)
    ZFENUM_VALUE(kMediaStop = 804)
    ZFENUM_VALUE(kMediaRecord = 805)
    ZFENUM_VALUE(kMediaPrev = 806)
    ZFENUM_VALUE(kMediaNext = 807)
    ZFENUM_VALUE(kMediaFastForward = 808)
    ZFENUM_VALUE(kMediaFastRewind = 809)

    ZFENUM_VALUE(kVolumeMute = 901)
    ZFENUM_VALUE(kVolumeUp = 902)
    ZFENUM_VALUE(kVolumeDown = 903)

    ZFENUM_VALUE(kPhoneCall = 1001)
    ZFENUM_VALUE(kPhoneCallEnd = 1002)
    ZFENUM_VALUE(kPhoneHome = 1003)
    ZFENUM_VALUE(kPhoneMenu = 1004)
    ZFENUM_VALUE(kPhoneBack = 1005)
    ZFENUM_VALUE(kPhoneSearch = 1006)
    ZFENUM_VALUE(kPhoneCamera = 1007)
ZFENUM_SEPARATOR()
    ZFENUM_VALUE_REGISTER(kUnknown)

    ZFENUM_VALUE_REGISTER(kEsc)
    ZFENUM_VALUE_REGISTER(kTab)
    ZFENUM_VALUE_REGISTER(kBackspace)
    ZFENUM_VALUE_REGISTER(kEnter)
    ZFENUM_VALUE_REGISTER(kSpace)
    ZFENUM_VALUE_REGISTER(kCapsLock)
    ZFENUM_VALUE_REGISTER(kShift)
    ZFENUM_VALUE_REGISTER(kCtrl)
    ZFENUM_VALUE_REGISTER(kAlt)
    ZFENUM_VALUE_REGISTER(kCommand)
    ZFENUM_VALUE_REGISTER(kContextMenu)

    ZFENUM_VALUE_REGISTER(kF1)
    ZFENUM_VALUE_REGISTER(kF2)
    ZFENUM_VALUE_REGISTER(kF3)
    ZFENUM_VALUE_REGISTER(kF4)
    ZFENUM_VALUE_REGISTER(kF5)
    ZFENUM_VALUE_REGISTER(kF6)
    ZFENUM_VALUE_REGISTER(kF7)
    ZFENUM_VALUE_REGISTER(kF8)
    ZFENUM_VALUE_REGISTER(kF9)
    ZFENUM_VALUE_REGISTER(kF10)
    ZFENUM_VALUE_REGISTER(kF11)
    ZFENUM_VALUE_REGISTER(kF12)

    ZFENUM_VALUE_REGISTER(k0)
    ZFENUM_VALUE_REGISTER(k1)
    ZFENUM_VALUE_REGISTER(k2)
    ZFENUM_VALUE_REGISTER(k3)
    ZFENUM_VALUE_REGISTER(k4)
    ZFENUM_VALUE_REGISTER(k5)
    ZFENUM_VALUE_REGISTER(k6)
    ZFENUM_VALUE_REGISTER(k7)
    ZFENUM_VALUE_REGISTER(k8)
    ZFENUM_VALUE_REGISTER(k9)

    ZFENUM_VALUE_REGISTER(kA)
    ZFENUM_VALUE_REGISTER(kB)
    ZFENUM_VALUE_REGISTER(kC)
    ZFENUM_VALUE_REGISTER(kD)
    ZFENUM_VALUE_REGISTER(kE)
    ZFENUM_VALUE_REGISTER(kF)
    ZFENUM_VALUE_REGISTER(kG)
    ZFENUM_VALUE_REGISTER(kH)
    ZFENUM_VALUE_REGISTER(kI)
    ZFENUM_VALUE_REGISTER(kJ)
    ZFENUM_VALUE_REGISTER(kK)
    ZFENUM_VALUE_REGISTER(kL)
    ZFENUM_VALUE_REGISTER(kM)
    ZFENUM_VALUE_REGISTER(kN)
    ZFENUM_VALUE_REGISTER(kO)
    ZFENUM_VALUE_REGISTER(kP)
    ZFENUM_VALUE_REGISTER(kQ)
    ZFENUM_VALUE_REGISTER(kR)
    ZFENUM_VALUE_REGISTER(kS)
    ZFENUM_VALUE_REGISTER(kT)
    ZFENUM_VALUE_REGISTER(kU)
    ZFENUM_VALUE_REGISTER(kV)
    ZFENUM_VALUE_REGISTER(kW)
    ZFENUM_VALUE_REGISTER(kX)
    ZFENUM_VALUE_REGISTER(kY)
    ZFENUM_VALUE_REGISTER(kZ)

    ZFENUM_VALUE_REGISTER(kGrave)
    ZFENUM_VALUE_REGISTER(kMinus)
    ZFENUM_VALUE_REGISTER(kEquals)
    ZFENUM_VALUE_REGISTER(kBracketLeft)
    ZFENUM_VALUE_REGISTER(kBracketRight)
    ZFENUM_VALUE_REGISTER(kBackSlash)
    ZFENUM_VALUE_REGISTER(kSemiColon)
    ZFENUM_VALUE_REGISTER(kApostrophe)
    ZFENUM_VALUE_REGISTER(kComma)
    ZFENUM_VALUE_REGISTER(kPeriod)
    ZFENUM_VALUE_REGISTER(kSlash)

    ZFENUM_VALUE_REGISTER(kLeft)
    ZFENUM_VALUE_REGISTER(kUp)
    ZFENUM_VALUE_REGISTER(kRight)
    ZFENUM_VALUE_REGISTER(kDown)
    ZFENUM_VALUE_REGISTER(kHome)
    ZFENUM_VALUE_REGISTER(kEnd)
    ZFENUM_VALUE_REGISTER(kPageUp)
    ZFENUM_VALUE_REGISTER(kPageDown)
    ZFENUM_VALUE_REGISTER(kInsert)
    ZFENUM_VALUE_REGISTER(kDelete)
    ZFENUM_VALUE_REGISTER(kPrintScreen)
    ZFENUM_VALUE_REGISTER(kScrollLock)
    ZFENUM_VALUE_REGISTER(kPauseBreak)

    ZFENUM_VALUE_REGISTER(kNum0)
    ZFENUM_VALUE_REGISTER(kNum1)
    ZFENUM_VALUE_REGISTER(kNum2)
    ZFENUM_VALUE_REGISTER(kNum3)
    ZFENUM_VALUE_REGISTER(kNum4)
    ZFENUM_VALUE_REGISTER(kNum5)
    ZFENUM_VALUE_REGISTER(kNum6)
    ZFENUM_VALUE_REGISTER(kNum7)
    ZFENUM_VALUE_REGISTER(kNum8)
    ZFENUM_VALUE_REGISTER(kNum9)
    ZFENUM_VALUE_REGISTER(kNumLock)
    ZFENUM_VALUE_REGISTER(kNumAdd)
    ZFENUM_VALUE_REGISTER(kNumMinus)
    ZFENUM_VALUE_REGISTER(kNumMultiply)
    ZFENUM_VALUE_REGISTER(kNumDivide)
    ZFENUM_VALUE_REGISTER(kNumEnter)
    ZFENUM_VALUE_REGISTER(kNumDot)

    ZFENUM_VALUE_REGISTER(kMediaPlay)
    ZFENUM_VALUE_REGISTER(kMediaPause)
    ZFENUM_VALUE_REGISTER(kMediaPlayPause)
    ZFENUM_VALUE_REGISTER(kMediaStop)
    ZFENUM_VALUE_REGISTER(kMediaRecord)
    ZFENUM_VALUE_REGISTER(kMediaPrev)
    ZFENUM_VALUE_REGISTER(kMediaNext)
    ZFENUM_VALUE_REGISTER(kMediaFastForward)
    ZFENUM_VALUE_REGISTER(kMediaFastRewind)

    ZFENUM_VALUE_REGISTER(kVolumeMute)
    ZFENUM_VALUE_REGISTER(kVolumeUp)
    ZFENUM_VALUE_REGISTER(kVolumeDown)

    ZFENUM_VALUE_REGISTER(kPhoneCall)
    ZFENUM_VALUE_REGISTER(kPhoneCallEnd)
    ZFENUM_VALUE_REGISTER(kPhoneHome)
    ZFENUM_VALUE_REGISTER(kPhoneMenu)
    ZFENUM_VALUE_REGISTER(kPhoneBack)
    ZFENUM_VALUE_REGISTER(kPhoneSearch)
    ZFENUM_VALUE_REGISTER(kPhoneCamera)
ZFENUM_END(ZFLIB_ZFUIKit, ZFUIKeyCode)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIKeyCode_h_


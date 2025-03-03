/**
 * @file ZFTestCase.h
 * @brief test case define
 */

#ifndef _ZFI_ZFTestCase_h_
#define _ZFI_ZFTestCase_h_

#include "ZFUtilityDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTestCasePrivate;
/**
 * @brief abstract test case
 *
 * to use, declare a subclass of #ZFTestCase,
 * and override test methods to supply your own test steps,
 * and finally notify end by #stop\n
 * all test cases can be started automatically by invoking #ZFTestCaseRunAllStart,
 * or you may manually start one by creating new instance of a test case,
 * and call #start on it
 * @note automatically test case run depends on #ZFClass's class map,
 *   see #ZFOBJECT_REGISTER for more info
 */
zfabstract ZFLIB_ZFUtility ZFTestCase : zfextend ZFTaskId {
    ZFOBJECT_DECLARE_ABSTRACT_WITH_CUSTOM_CTOR(ZFTestCase, ZFTaskId)

public:
    /**
     * @brief called when output something
     *
     * param0 is a #v_zfstring holds the output,
     * notified when #output called
     */
    ZFEVENT(TestCaseOnOutput)
    /**
     * @brief called when test started
     *
     * called when test case started
     */
    ZFEVENT(TestCaseOnStart)
    /**
     * @brief called when test progress
     *
     * param0 is the param passed from #notifyProgress
     */
    ZFEVENT(TestCaseOnProgress)
    /**
     * @brief called when test stopped
     *
     * param0 is #ZFResultType
     */
    ZFEVENT(TestCaseOnStop)

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    /** @brief see #E_TestCaseOnOutput */
    virtual inline void testCaseOnOutput(ZF_IN const zfchar *info) {
        if(this->observerHasAdd(ZFTestCase::E_TestCaseOnOutput())) {
            this->observerNotify(ZFTestCase::E_TestCaseOnOutput(), zfobj<v_zfstring>(info));
        }
    }
    /** @brief see #E_TestCaseOnStart */
    virtual inline void testCaseOnStart(void) {
        this->observerNotify(ZFTestCase::E_TestCaseOnStart());
    }
    /** @brief see #E_TestCaseOnProgress */
    virtual inline void testCaseOnProgress(ZF_IN ZFObject *progress) {
        this->observerNotify(ZFTestCase::E_TestCaseOnProgress(), progress);
    }
    /** @brief see #E_TestCaseOnStop */
    virtual inline void testCaseOnStop(ZF_IN ZFResultType testCaseResult) {
        this->observerNotify(ZFTestCase::E_TestCaseOnStop(), zfobj<v_ZFResultType>(testCaseResult));
        zfRelease(this);
    }

public:
    /**
     * @brief whether test is running
     */
    ZFMETHOD_DECLARE_0(zfbool, started)

    /**
     * @brief output something for the test case,
     *   used to debug
     */
    zffinal void output(ZF_IN const zfchar *info);
    /**
     * @brief manually start a test case
     *
     * do nothing if already begun,
     * automatically retain this object during test,
     * and release it after #stop
     */
    ZFMETHOD_DECLARE_0(void, start)
    /**
     * @brief called to notify progress,
     *   do nothing if not running
     */
    ZFMETHOD_DECLARE_1(void, notifyProgress
            , ZFMP_IN_OPT(ZFObject *, progress, zfnull)
            )
    zfoverride
    virtual void stop(void) {
        this->stop(v_ZFResultType::e_Success);
    }
    /**
     * @brief must be called to stop test,
     *   do nothing if not running
     *
     * @warning you must not access this object after calling this method
     */
    ZFMETHOD_DECLARE_1(void, stop
            , ZFMP_IN(ZFResultType, testCaseResult)
            )

private:
    zfbool _started;
protected:
    ZFTestCase(void) : _started(zffalse) {}
};

/**
 * @brief util macro to assert within test case
 *
 * upon assert fail, finish the test case by #ZFTestCase::stop
 */
#define ZFTestCaseAssert(cond) \
    ZFTestCaseAssertWithMessage(cond, "test failed: %s", #cond)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertTrim(cond) \
    ZFTestCaseAssertTrimWithMessage(cond, "test failed: %s", #cond)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertDetail(cond, callerInfo) \
    ZFTestCaseAssertDetailWithMessage(cond, callerInfo, "test failed: %s", #cond)

/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessage(cond, fmt, ...) \
    do { \
        if(!(cond)) { \
            this->output(zfstr("%s " fmt, ZFCallerInfoCreate(), ##__VA_ARGS__)); \
            this->stop(v_ZFResultType::e_Fail); \
            return; \
        } \
    } while(zffalse)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessageTrim(cond, fmt, ...) \
    do { \
        if(!(cond)) { \
            this->output(zfstr(fmt, ##__VA_ARGS__)); \
            this->stop(v_ZFResultType::e_Fail); \
            return; \
        } \
    } while(zffalse)
/** @brief see #ZFTestCaseAssert */
#define ZFTestCaseAssertWithMessageDetail(cond, callerInfo, fmt, ...) \
    do { \
        if(!(cond)) { \
            this->output(zfstr("%s " fmt, callerInfo, fmt, ##__VA_ARGS__)); \
            this->stop(v_ZFResultType::e_Fail); \
            return; \
        } \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTestCase_h_


#include "JavaScriptCoreCPP/JSString.h"

#include <string>
#include <iostream>

#include <JavaScriptCore/JavaScript.h>
#include "gtest/gtest.h"

// Tests JSString for std::string interoperability.
TEST(JSStringTests, JSEvaluateScript) {
	// This test is named "JSEvaluateScript", and belongs to the
	// "JSStringTests" test case.
	
    ::JSGlobalContextRef globalContext { JSGlobalContextCreate(nullptr) };
    ::JSObjectRef        globalObject  { JSContextGetGlobalObject(globalContext) };
	
    //JSValueRef JSEvaluateScript(JSContextRef ctx, JSStringRef script, JSObjectRef thisObject, JSStringRef sourceURL, int startingLineNumber, JSValueRef* exception);
    
    ::JSValueRef exceptionValue { nullptr };
    JSString     scriptString   = JSString("2 + 2");
    JSValueRef   resultValue    = JSEvaluateScript(globalContext, JSStringRef(scriptString), globalObject, nullptr, 0, &exceptionValue);
    
    if (exceptionValue) {
        ::JSValueRef exceptionValue2 { nullptr };
        JSString exceptionString { JSValueToStringCopy(globalContext, exceptionValue, &exceptionValue2) };
		EXPECT_FALSE(exceptionValue2);
        std::clog << "MDL: caught exception: " << exceptionString << std::endl;
    } else {
        JSString resultString { JSValueToStringCopy(globalContext, resultValue, &exceptionValue) };
		EXPECT_FALSE(exceptionValue);
        std::clog << "MDL: result = " << resultString << std::endl;
    }
}

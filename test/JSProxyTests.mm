//
//  TiProxyTests.m
//  TiValue
//
//  Created by Matt Langston on 9/9/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.

#import <XCTest/XCTest.h>
#include "JSProxy.h"
#include "JSString.h"
#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <iostream>

@interface JSProxyTests : XCTestCase
@end

@implementation JSProxyTests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testJSClassRef
{
    ::JSGlobalContextRef globalContext { JSGlobalContextCreate(nullptr) };
    ::JSObjectRef        globalObject  { JSContextGetGlobalObject(globalContext) };

    //JSValueRef JSEvaluateScript(JSContextRef ctx, JSStringRef script, JSObjectRef thisObject, JSStringRef sourceURL, int startingLineNumber, JSValueRef* exception);
    
    ::JSValueRef exceptionValue { nullptr };
    JSString     scriptString   = JSString("2 + 2");
    JSValueRef   resultValue    = JSEvaluateScript(globalContext, JSStringRef(scriptString), globalObject, nullptr, 0, &exceptionValue);
    
    if (exceptionValue) {
        ::JSValueRef exceptionValue2 { nullptr };
        JSString exceptionString { JSValueToStringCopy(globalContext, exceptionValue, &exceptionValue2) };
        XCTAssertFalse(exceptionValue2);
        std::clog << "MDL: caught exception: " << exceptionString << std::endl;
    } else {
        JSString resultString { JSValueToStringCopy(globalContext, resultValue, &exceptionValue) };
        XCTAssertFalse(exceptionValue);
        std::clog << "MDL: result = " << resultString << std::endl;
    }

    //context.addPropertyToGlobalObject("Proxy2", Ti::Proxy2::CreateConstructor());
}

@end

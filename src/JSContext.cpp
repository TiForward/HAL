// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSContext.h"
#include <iostream>

#include "JavaScriptCoreCPP/JSValue.h"
#include "JavaScriptCoreCPP/JSString.h"

std::atomic<long> JSContext::ctorCounter_ { 0 };
std::atomic<long> JSContext::dtorCounter_ { 0 };

JSContext::JSContext() :
JSContext(JSVirtualMachine::create())
{
    std::clog << "JSContext: ctor called" << std::endl;
}

JSContext::JSContext(const JSVirtualMachine_ptr_t& virtualMachine_ptr) :
virtualMachine_ptr_(virtualMachine_ptr),
context_(JSGlobalContextCreateInGroup(JSContextGroupRef(*virtualMachine_ptr), 0))
{
    std::clog << "JSContext: ctor called (JSVirtualMachine)" << std::endl;
    ++ctorCounter_;
}

JSContext::~JSContext() {
    std::clog << "JSContext: dtor called" << std::endl;
    JSGlobalContextRelease(context_);
    ++dtorCounter_;
}

// Create a copy of another JSContext.
JSContext::JSContext(const JSContext& rhs) :
virtualMachine_ptr_(rhs.virtualMachine_ptr_),
context_(JSGlobalContextRetain(rhs.context_)),
exception_ptr_(rhs.exception_ptr_),
exceptionHandler_(rhs.exceptionHandler_)
{
    std::clog << "JSContext: copy ctor called" << std::endl;
    ++ctorCounter_;
}

// Create a copy of another JSContext by assignment.
JSContext& JSContext::operator=(const JSContext& rhs) {
    std::clog << "JSContext: operator= called" << std::endl;
    
    if ( this == &rhs ) {
        return *this;
    }
    
    virtualMachine_ptr_ = rhs.virtualMachine_ptr_;
    
    // Release the resource we are replacing.
    JSGlobalContextRelease(context_);
    
    // Retain the resource we are copying.
    context_ = JSGlobalContextRetain(rhs.context_);
    
    exception_ptr_    = rhs.exception_ptr_;
    exceptionHandler_ = rhs.exceptionHandler_;
    
    return *this;
}

JSValue_ptr_t JSContext::globalObject() {
    return JSValue::create(JSContextGetGlobalObject(context_), shared_from_this());
}

JSValue_ptr_t JSContext::evaluateScript(const std::string& script) {
    ::JSValueRef exceptionValue { nullptr };
    JSString scriptJS { script };
    ::JSValueRef result = JSEvaluateScript(context_, JSStringRef(scriptJS), nullptr, nullptr, 0, &exceptionValue);
    
    if (exceptionValue) {
        std::clog << "JSContext: exception during JSEvaluateScript" << std::endl;
        return valueFromNotifyException(exceptionValue);
    }
    
    return JSValue::create(result, shared_from_this());
}

JSValue_ptr_t JSContext::valueFromNotifyException(::JSValueRef exceptionValue) {
    notifyException(exceptionValue);
    // Return undefined, which is created by the JSValue default
    // constructor.
    JSContext_ptr_t context_ptr = JSContext_ptr_t(this, deleter{});
    return JSValue::valueWithUndefinedInContext(context_ptr);
}

void JSContext::notifyException(::JSValueRef value) {
    if (exceptionHandler_) {
        exceptionHandler_(shared_from_this(), JSValue::create(value, shared_from_this()));
    }
}

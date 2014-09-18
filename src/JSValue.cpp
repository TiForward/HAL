// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSValue.h"

std::atomic<long> JSValue::ctorCounter_ { 0 };
std::atomic<long> JSValue::dtorCounter_ { 0 };

// Create the Undefined value.
JSValue::JSValue(const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeUndefined(*context_ptr), context_ptr)
{
    std::clog << "JSValue: ctor called (JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(bool value, const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeBoolean(*context_ptr, value), context_ptr)
{
    std::clog << "JSValue: ctor called (bool, JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(double value, const JSContext_ptr_t& context_ptr) :
JSValue(JSValueMakeNumber(*context_ptr, value), context_ptr)
{
    std::clog << "JSValue: ctor called (double, JSContext_ptr_t)" << std::endl;
}

JSValue::JSValue(::JSValueRef value, const JSContext_ptr_t& context_ptr) :
value_(value),
context_ptr_(context_ptr)
{
    std::clog << "JSValue: ctor called (JSValueRef, JSContext_ptr_t)" << std::endl;
    JSValueProtect(*context_ptr_, value_);
    ++ctorCounter_;
}

JSValue::~JSValue() {
    std::clog << "JSValue: dtor called" << std::endl;
    JSValueUnprotect(*context_ptr_, value_);
    ++dtorCounter_;
}

// Create a copy of another JSValue.
JSValue::JSValue(const JSValue& rhs) :
JSValue(rhs.value_, rhs.context_ptr_)
{
    std::clog << "JSValue: copy ctor called" << std::endl;
}

// Create a copy of another JSValue by assignment.
JSValue& JSValue::operator=(const JSValue& rhs) {
    std::clog << "JSValue: operator= called" << std::endl;
    
    if ( this == &rhs ) {
        return *this;
    }
    
    // Release the resource we are replacing.
    JSValueUnprotect(*context_ptr_, value_);
    
    value_       = rhs.value_;
    context_ptr_ = rhs.context_ptr_;
    
    // Retain the resource we copying.
    JSValueProtect(*context_ptr_, value_);
    
    return *this;
}

JSValue::operator double() const {
    ::JSValueRef exception = 0;
    double result = JSValueToNumber(*context_ptr_, value_, &exception);
    if (exception) {
        context_ptr_ -> notifyException(exception);
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    return result;
}

JSValue::operator JSString() const {
    ::JSValueRef exception = 0;
    ::JSStringRef result = JSValueToStringCopy(*context_ptr_, value_, &exception);
    if (exception) {
        context_ptr_ -> notifyException(exception);
        return JSString("");
    }
    
    return JSString(result);
}


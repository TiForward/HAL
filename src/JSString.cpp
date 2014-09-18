// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSString.h"

std::atomic<long> JSString::ctorCounter_ { 0 };
std::atomic<long> JSString::dtorCounter_ { 0 };

JSString::JSString(JSStringRef string) :
string_(JSStringRetain(string))
{
    std::clog << "JSString: ctor called (JSStringRef)" << std::endl;
    ++ctorCounter_;
}

JSString::JSString(const std::string& string) :
string_(JSStringCreateWithUTF8CString(string.c_str()))
{
    std::clog << "JSString: ctor called (const std::string&)" << std::endl;
    ++ctorCounter_;
}

JSString::~JSString() {
    std::clog << "JSString: dtor called" << std::endl;
    JSStringRelease(string_);
    ++dtorCounter_;
}

// Create a copy of another JSString.
JSString::JSString(const JSString& rhs) :
string_(JSStringRetain(rhs.string_))
{
    std::clog << "JSString: copy ctor called" << std::endl;
    ++ctorCounter_;
}

// Create a copy of another JSProxy by assignment.
JSString& JSString::operator=(const JSString& rhs) {
    std::clog << "JSString: operator= called" << std::endl;
    
    if ( this == &rhs ) {
        return *this;
    }
    
    // Release the resource we are replacing.
    JSStringRelease(string_);
    
    // Retain the resource we are copying.
    string_ = JSStringRetain(rhs.string_);
    
    return *this;
}

JSString::operator std::string() const {
    const size_t maxSize = JSStringGetMaximumUTF8CStringSize(string_);
    char* utf8Buffer = new char[maxSize];
    JSStringGetUTF8CString(string_, utf8Buffer, maxSize);
    std::string string { utf8Buffer };
    delete[] utf8Buffer;
    return string;
}


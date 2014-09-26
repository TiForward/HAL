// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/JSString.h"
#include <codecvt>

std::atomic<long> JSString::ctorCounter_ { 0 };
std::atomic<long> JSString::dtorCounter_ { 0 };

JSString::JSString() :
string_(JSStringCreateWithUTF8CString(nullptr))
{
    std::clog << "JSString: ctor called" << std::endl;
    ++ctorCounter_;
}

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
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
    
    return std::string(converter.to_bytes(static_cast<std::u16string>(*this)));
}

JSString::operator std::u16string() const {
    const JSChar* string_ptr = JSStringGetCharactersPtr(string_);
    return std::u16string(string_ptr, string_ptr + JSStringGetLength(string_));
}

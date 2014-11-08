/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSString.hpp"

#include <cassert>

namespace JavaScriptCoreCPP {
  
  JSString::JSString() noexcept
  : js_string_ref__(JSStringCreateWithUTF8CString(nullptr)) {
  }
  
  JSString::JSString(const char* string) noexcept
  : js_string_ref__(JSStringCreateWithUTF8CString(string)) {
  }
  
  JSString::JSString(const std::string& string) noexcept
  : JSString(string.c_str()) {
  }
  
  const std::size_t JSString::length() const  noexcept{
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    return JSStringGetLength(js_string_ref__);
  }
  
  const std::size_t JSString::size() const noexcept {
    return length();
  }
  
  const bool JSString::empty() const noexcept {
    return length() == 0;
  }
  
  JSString::operator std::string() const noexcept {
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
    return std::string(converter.to_bytes(static_cast<std::u16string>(*this)));
  }
  
  JSString::operator std::u16string() const noexcept {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
    return std::u16string(string_ptr, string_ptr + length());
  }
  
  JSString::~JSString() noexcept {
    JSStringRelease(js_string_ref__);
  }
  
  JSString::JSString(const JSString& rhs) noexcept
  : js_string_ref__(rhs.js_string_ref__) {
    JSStringRetain(js_string_ref__);
  }
  
  JSString::JSString(JSString&& rhs) noexcept
  : js_string_ref__(rhs.js_string_ref__) {
    JSStringRetain(rhs.js_string_ref__);
  }
  
  JSString& JSString::operator=(const JSString& rhs) noexcept {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    JSStringRetain(rhs.js_string_ref__);
    return *this;
  }
  
  JSString& JSString::operator=(JSString&& rhs) noexcept {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    JSStringRetain(rhs.js_string_ref__);
    return *this;
  }
  
  void JSString::swap(JSString& other) noexcept {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_string_ref__, other.js_string_ref__);
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSString::JSString(JSStringRef js_string_ref) noexcept
  : js_string_ref__(js_string_ref) {
    assert(js_string_ref__);
    JSStringRetain(js_string_ref__);
  }
  
  bool operator==(const JSString& lhs, const JSString& rhs) {
    return JSStringIsEqual(lhs, rhs);
  }
  
} // namespace JavaScriptCoreCPP {

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
  
  JSString::JSString() JAVASCRIPTCORECPP_NOEXCEPT
  : js_string_ref__(JSStringCreateWithUTF8CString(nullptr)) {
  }
  
  JSString::JSString(const char* string) JAVASCRIPTCORECPP_NOEXCEPT
  : js_string_ref__(JSStringCreateWithUTF8CString(string)) {
  }
  
  JSString::JSString(const std::string& string) JAVASCRIPTCORECPP_NOEXCEPT
  : JSString(string.c_str()) {
  }
  
  const std::size_t JSString::length() const  JAVASCRIPTCORECPP_NOEXCEPT{
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    return JSStringGetLength(js_string_ref__);
  }
  
  const std::size_t JSString::size() const JAVASCRIPTCORECPP_NOEXCEPT {
    return length();
  }
  
  const bool JSString::empty() const JAVASCRIPTCORECPP_NOEXCEPT {
    return length() == 0;
  }
  
  JSString::operator std::string() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
    auto self = const_cast<JSString*>(this);
    std::call_once(self -> string_once_flag__, [self]() {
      self -> string__ = std::string(converter.to_bytes(static_cast<std::u16string>(*self)));
    });
    
    return string__;
  }
  
  JSString::operator std::u16string() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    auto self = const_cast<JSString*>(this);
    std::call_once(self -> u16string_once_flag__, [self]() {
      const JSChar* string_ptr = JSStringGetCharactersPtr(self -> js_string_ref__);
      self -> u16string__ = std::u16string(string_ptr, string_ptr + self -> length());
    });
    
    return u16string__;
  }
  
  std::size_t JSString::hash_value() const {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    auto self = const_cast<JSString*>(this);
    std::call_once(self -> hash_value_once_flag__, [self]() {
      std::hash<std::string> hash_function = std::hash<std::string>();
      self -> hash_value__ = hash_function(static_cast<std::string>(*self));
    });
    
    return hash_value__;
  }
  
  JSString::~JSString() JAVASCRIPTCORECPP_NOEXCEPT {
    JSStringRelease(js_string_ref__);
  }
  
  JSString::JSString(const JSString& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_string_ref__(rhs.js_string_ref__) {
    JSStringRetain(js_string_ref__);
  }
  
  JSString::JSString(JSString&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_string_ref__(rhs.js_string_ref__) {
    JSStringRetain(rhs.js_string_ref__);
  }
  
  JSString& JSString::operator=(const JSString& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    JSStringRetain(rhs.js_string_ref__);
    return *this;
  }
  
  JSString& JSString::operator=(JSString&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    JSStringRetain(rhs.js_string_ref__);
    return *this;
  }
  
  void JSString::swap(JSString& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_string_ref__, other.js_string_ref__);
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSString::JSString(JSStringRef js_string_ref) JAVASCRIPTCORECPP_NOEXCEPT
  : js_string_ref__(js_string_ref) {
    assert(js_string_ref__);
    JSStringRetain(js_string_ref__);
  }
  
  bool operator==(const JSString& lhs, const JSString& rhs) {
    return JSStringIsEqual(lhs, rhs);
  }
  
} // namespace JavaScriptCoreCPP {

/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSString.hpp"

#include <cassert>

namespace HAL {
  
  JSString::JSString() HAL_NOEXCEPT
  : JSString("") {
    //HAL_LOG_TRACE("JSString::JSString()");
  }
  
  JSString::JSString(const char* string) HAL_NOEXCEPT
  : js_string_ref__(JSStringCreateWithUTF8CString(string))
  , string__(string) {
    HAL_LOG_TRACE("JSString:: ctor 1");
    HAL_LOG_TRACE("JSString:: retain (implicit) ", js_string_ref__);
    const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
    u16string__ = std::u16string(string_ptr, string_ptr + length());
    
    std::hash<std::string> hash_function = std::hash<std::string>();
    hash_value__ = hash_function(static_cast<std::string>(string__));
    //HAL_LOG_TRACE("JSString::JSString(const char*)");
  }
  
  JSString::JSString(const std::string& string) HAL_NOEXCEPT
  : js_string_ref__(JSStringCreateWithUTF8CString(string.c_str()))
  , string__(string) {
    HAL_LOG_TRACE("JSString:: ctor 2");
    HAL_LOG_TRACE("JSString:: retain (implicit) ", js_string_ref__);
    const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
    u16string__ = std::u16string(string_ptr, string_ptr + length());

    std::hash<std::string> hash_function = std::hash<std::string>();
    hash_value__ = hash_function(static_cast<std::string>(string__));
    //HAL_LOG_TRACE("JSString::JSString(const std::string&)");
  }
  
  const std::size_t JSString::length() const  HAL_NOEXCEPT{
    HAL_JSSTRING_LOCK_GUARD;
    return JSStringGetLength(js_string_ref__);
  }
  
  const std::size_t JSString::size() const HAL_NOEXCEPT {
    return length();
  }
  
  const bool JSString::empty() const HAL_NOEXCEPT {
    return length() == 0;
  }
  
  JSString::operator std::string() const HAL_NOEXCEPT {
    return string__;
  }
  
  JSString::operator std::u16string() const HAL_NOEXCEPT {
    return u16string__;
  }
  
  std::size_t JSString::hash_value() const {
    return hash_value__;
  }
  
  JSString::~JSString() HAL_NOEXCEPT {
    HAL_LOG_TRACE("JSString:: dtor");
    HAL_LOG_TRACE("JSString:: release ", js_string_ref__);
    JSStringRelease(js_string_ref__);
  }
  
  JSString::JSString(const JSString& rhs) HAL_NOEXCEPT
  : js_string_ref__(rhs.js_string_ref__)
  , string__(rhs.string__)
  , u16string__(rhs.u16string__)
  , hash_value__(rhs.hash_value__) {
    HAL_LOG_TRACE("JSString:: copy ctor");
    HAL_LOG_TRACE("JSString:: retain ", js_string_ref__);
    JSStringRetain(js_string_ref__);
  }
  
  JSString::JSString(JSString&& rhs) HAL_NOEXCEPT
  : js_string_ref__(rhs.js_string_ref__)
  , string__(std::move(rhs.string__))
  , u16string__(std::move(rhs.u16string__))
  , hash_value__(std::move(rhs.hash_value__)) {
    HAL_LOG_TRACE("JSString:: move ctor");
    HAL_LOG_TRACE("JSString:: retain ", js_string_ref__);
    JSStringRetain(js_string_ref__);
  }
  
  JSString& JSString::operator=(const JSString& rhs) HAL_NOEXCEPT {
    HAL_JSSTRING_LOCK_GUARD;
    HAL_LOG_TRACE("JSString:: copy assignment");
    HAL_LOG_TRACE("JSString:: release ", js_string_ref__);
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    string__        = rhs.string__;
    u16string__     = rhs.u16string__;
    hash_value__    = rhs.hash_value__;
    HAL_LOG_TRACE("JSString:: retain ", js_string_ref__);
    JSStringRetain(js_string_ref__);
    return *this;
  }
  
  JSString& JSString::operator=(JSString&& rhs) HAL_NOEXCEPT {
    HAL_JSSTRING_LOCK_GUARD;
    HAL_LOG_TRACE("JSString:: move assignment");
    HAL_LOG_TRACE("JSString:: release ", js_string_ref__);
    JSStringRelease(js_string_ref__);
    js_string_ref__ = rhs.js_string_ref__;
    string__        = std::move(rhs.string__);
    u16string__     = std::move(rhs.u16string__);
    hash_value__    = std::move(rhs.hash_value__);
    HAL_LOG_TRACE("JSString:: retain ", js_string_ref__);
    JSStringRetain(js_string_ref__);
    return *this;
  }
  
  void JSString::swap(JSString& other) HAL_NOEXCEPT {
    HAL_JSSTRING_LOCK_GUARD;
    HAL_LOG_TRACE("JSString:: swap");
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_string_ref__, other.js_string_ref__);
    swap(u16string__    , other.u16string__);
    swap(string__       , other.string__);
    swap(hash_value__   , other.hash_value__);
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSString::JSString(JSStringRef js_string_ref) HAL_NOEXCEPT
  : js_string_ref__(js_string_ref) {
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
    assert(js_string_ref__);
    JSStringRetain(js_string_ref__);
    HAL_LOG_TRACE("JSString:: ctor 3");
    HAL_LOG_TRACE("JSString:: retain ", js_string_ref__);

    const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
    u16string__ = std::u16string(string_ptr, string_ptr + length());
    string__    = std::string(converter.to_bytes(u16string__));
    
    std::hash<std::string> hash_function = std::hash<std::string>();
    hash_value__ = hash_function(static_cast<std::string>(string__));

    //HAL_LOG_TRACE("JSString::JSString(JSStringRef)");
  }
  
  bool operator==(const JSString& lhs, const JSString& rhs) {
    return JSStringIsEqual(lhs, rhs);
  }
  
} // namespace HAL {

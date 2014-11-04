/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSSTRING_HPP_
#define _JAVASCRIPTCORECPP_JSSTRING_HPP_

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <string>
#include <locale>
#include <codecvt>
#include <cstddef>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

extern "C" struct JSStringRef;

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JSString is an RAII wrapper around a JSStringRef, the
  JavaScriptCore C API representation of a JavaScript string.

  A JSString satisfies satisfies all the requirements for use in all
  STL containers. For example, a JSString can be used as a key in a
  std::unordered_map.

  Specifically, a JSString is comparable with an equivalence relation,
  provides a strict weak ordering, and provides a custom hash
  function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSString final : public detail::JSPerformanceCounter<JSString> {
#else
class JSString final {
#endif
  
 public:
  
  /*!
    @method
    
    @abstract Create an empty JavaScript string with a length of zero.
    
    @result An empty JavaScript string with a length of zero.
  */
  JSString();
  
  /*!
    @method
    
    @abstract Create a JavaScript string from a null-terminated UTF8
    string.
    
    @param string The null-terminated UTF8 string to copy into the new
    JSString.
    
    @result A JSString containing string.
  */
  JSString(const char* string);

  /*!
    @method
    
    @abstract Create a JavaScript string from a null-terminated UTF8
    string.
    
    @param string The null-terminated UTF8 string to copy into the new
    JSString.
    
    @result A JSString containing string.
  */
  explicit JSString(const std::string& string)
      : JSString(string.c_str()) {
  }

  /*!
    @method
    
    @abstract Return the number of Unicode characters in this
    JavaScript string.
    
    @result The number of Unicode characters in this JavaScript
    string.
  */
  const std::size_t length() const;
  
  /*!
    @method
    
    @abstract Return the number of Unicode characters in this
    JavaScript string.
    
    @result The number of Unicode characters in this JavaScript
    string.
  */
  const std::size_t size() const {
    return length();
  }
  
  /*!
    @method
    
    @abstract Return true if this JavaScript string has a length of
    zero.
    
    @result true if this JavaScript string has a length of zero.
  */
  const bool empty() const {
    return length() == 0;
  }
  
  /*!
    @method
    
    @abstract Convert this JavaScript string to a UTF-8 encoded
    std::string.
    
    @result This JavaScript string converted to a UTF-8 encoded
    std::string.
  */
  operator std::string() const {
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
    return std::string(converter.to_bytes(static_cast<std::u16string>(*this)));
  }
  
  /*!
    @method
    
    @abstract Convert this JavaScript string to a UTF-16 encoded
    std::u16string.
    
    @result This JavaScript string converted to a UTF-16 encoded
    std::u16string.
  */
  operator std::u16string() const;


  ~JSString();
  JSString(const JSString&);
  JSString(JSString&&);
  JSString& JSString::operator=(const JSString&) = delete;
  JSString& JSString::operator=(JSString&&) = delete;
  JSString& operator=(JSString);
  void swap(JSString&) noexcept;
  
private:
  
  // These classes and functions need access to operator
  // JSStringRef().
  friend class JSContext;                 // JSEvaluateScript and JSCheckScriptSyntax
  friend class JSObject;                  // HasProperty
  friend class JSPropertyNameArray;       // GetNameAtIndex
  friend class JSPropertyNameAccumulator; // AddName

  friend std::vector<JSStringRef> detail::ToJSStringRefVector(const std::vector<JSString>& js_string_vector);

  // For interoperability with the JavaScriptCore C API.
  operator JSStringRef() const {
    return js_string_ref__;
  }

  // Only the following classes and functions can create a JSString.
  friend class JSValue;
  template<typename T>
  friend class detail::JSNativeClass;  //  static functions
  friend std::vector<JSStringRef> detail::ToJSStringRefVector(const std::vector<JSString>& js_string_vector);

  // For interoperability with the JavaScriptCore C API.
  JSString(const JSStringRef& js_string_ref);

  // Prevent heap based objects.
  static void * operator new(std::size_t);     // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t); // #2: To prevent allocation of array of objects
  
  friend void swap(JSString& first, JSString& second) noexcept;
  friend bool operator==(const JSString& lhs, const JSString& rhs);
  
  JSStringRef js_string_ref__ { nullptr };

#undef JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
                                                              std::recursive_mutex       mutex__;
#define JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

// inline
// std::string to_string(const JSString& js_string) {
//   return static_cast<std::string>(js_string);
// }

// Return true if the two JSStrings are equal.
bool operator==(const JSString& lhs, const JSString& rhs);

// Return true if the two JSStrings are not equal.
inline
bool operator!=(const JSString& lhs, const JSString& rhs) {
  return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSStrings.
inline
bool operator<(const JSString& lhs, const JSString& rhs) {
  return to_string(lhs) < to_string(rhs);
}

inline
bool operator>(const JSString& lhs, const JSString& rhs) {
  return rhs < lhs;
}

inline
bool operator<=(const JSString& lhs, const JSString& rhs) {
  return !(lhs > rhs);
}

inline
bool operator>=(const JSString& lhs, const JSString& rhs) {
  return !(lhs < rhs);
}

inline
std::ostream& operator << (std::ostream& ostream, const JSString& js_string) {
  ostream << to_string(js_string);
  return ostream;
}

} // namespace JavaScriptCoreCPP {

namespace std {

using JavaScriptCoreCPP::JSString;

template<>
void swap<JSString>(JSString& first, JSString& second) noexcept {
  first.swap(second);
}

template<>
struct hash<JSString> {
  using argument_type = JSString;
  using result_type   = std::size_t;
  static const std::hash<std::string> hash_function;
  
  result_type operator()(const argument_type& js_string) const {
    return hash_function(js_string);
  }
};

template<>
std::hash<std::string> hash<JSString>::hash_function = std::hash<std::string>();

}  // namespace std

#endif // _JAVASCRIPTCORECPP_JSSTRING_HPP_
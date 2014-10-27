/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_

#include <locale>
#include <cstddef>
#include <codecvt>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace detail {
class JSPropertyNameArray;
}}

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class JSString
  
  @discussion A JSString is an RAII wrapper around a JSStringRef, the
  JavaScriptCore C API representation of a JavaScript string.

  A JSString satisfies satisfies all the requirements for use in all
  STL containers. For example, a JSString can be used as a key in a
  std::unordered_map.

  Specifically, a JSString is comparable with an equivalence relation,
  provides a strict weak ordering, and provides a custom hash
  function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSString final	: public detail::JSPerformanceCounter<JSString> {
#else
class JSString final	{
#endif
	
 public:
	
	/*!
	  @method
	  
	  @abstract Create an empty JavaScript string with a length of zero.
	  
	  @result An empty JavaScript string with a length of zero.
	*/
	JSString() : js_string_ref_(JSStringCreateWithUTF8CString(nullptr)) {
	}
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript string from a null-terminated UTF8
	  string.
	  
	  @param string The null-terminated UTF8 string to copy into the new
	  JSString.
	  
	  @result A JSString containing string.
	*/
	JSString(const char* string) : js_string_ref_(JSStringCreateWithUTF8CString(string)) {
  }

	/*!
	  @method
	  
	  @abstract Create a JavaScript string from a null-terminated UTF8
	  string.
	  
	  @param string The null-terminated UTF8 string to copy into the new
	  JSString.
	  
	  @result A JSString containing string.
	*/
	JSString(const std::string& string) : JSString(string.c_str()) {
	}

	~JSString() {
		JSStringRelease(js_string_ref_);
	}

	// Copy constructor.
	JSString(const JSString& rhs) : js_string_ref_(rhs.js_string_ref_) {
		JSStringRetain(js_string_ref_);
	}

	// Move constructor.
	JSString(JSString&& rhs) : js_string_ref_(rhs.js_string_ref_) {
		JSStringRetain(js_string_ref_);
	}
	
	// Create a copy of another JSString by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator, X& X::operator=(const X&), and the move assignment
  // operator, X& X::operator=(X&&);
  JSString& operator=(JSString rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSString& first, JSString& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_string_ref_, second.js_string_ref_);
  }

	const std::size_t length() const {
		return JSStringGetLength(js_string_ref_);
	}
	
	const std::size_t size() const {
		return length();
	}
	
	const bool empty() const {
		return length() == 0;
	}
	
	operator std::string() const {
		static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
		return std::string(converter.to_bytes(static_cast<std::u16string>(*this)));
	}

	operator std::u16string() const {
		const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref_);
		return std::u16string(string_ptr, string_ptr + JSStringGetLength(js_string_ref_));
	}

private:

  // For interoperability with the JavaScriptCore C API.
  JSString(const JSStringRef& js_string_ref) : js_string_ref_(js_string_ref) {
	  assert(js_string_ref_);
    JSStringRetain(js_string_ref_);
  }
  
  // For interoperability with the JavaScriptCore C API.
  operator JSStringRef() const {
    return js_string_ref_;
  }

  friend class JSContext;
  friend class JSValue;
  friend class JSObject;
  friend class detail::JSPropertyNameArray;
  friend class JSPropertyNameAccumulator;
  friend class JSFunction;
  
  template<typename T>
	class JSNativeObject;

  // Return true if the two JSStrings are equal.
	friend bool operator==(const JSString& lhs, const JSString& rhs);

	JSStringRef js_string_ref_;
};

// Return true if the two JSStrings are equal.
inline
bool operator==(const JSString& lhs, const JSString& rhs) {
	return JSStringIsEqual(lhs, rhs);
}

// Return true if the two JSStrings are not equal.
inline
bool operator!=(const JSString& lhs, const JSString& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSStrings.
inline
bool operator<(const JSString& lhs, const JSString& rhs) {
	return static_cast<std::string>(lhs) < static_cast<std::string>(rhs);
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
	ostream << static_cast<std::string>(js_string);
	return ostream;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

namespace std {

using namespace JavaScriptCoreCPP::RAII;

template<>
struct hash<JSString> {
	using argument_type = JSString;
	using result_type   = std::size_t;
	const std::hash<std::string> string_hash = std::hash<std::string>();
	
	result_type operator()(const argument_type& js_string) const {
		return string_hash(js_string);
	}
};

}  // namespace std

#endif // _JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_

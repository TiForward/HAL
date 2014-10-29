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


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#undef  JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_TYPE
#define JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME_PREFIX js_context

#undef  JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME
#define JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_TYPE JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME;

#undef  JAVASCRIPTCORECPP_RAII_JSSTRING_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSSTRING_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_TYPE> JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSSTRING_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace detail {
class JSPropertyNameArray;
}}

namespace JavaScriptCoreCPP { namespace RAII {

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
	JSString() : js_string_ref__(JSStringCreateWithUTF8CString(nullptr)) {
	}
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript string from a null-terminated UTF8
	  string.
	  
	  @param string The null-terminated UTF8 string to copy into the new
	  JSString.
	  
	  @result A JSString containing string.
	*/
	JSString(const char* string) : js_string_ref__(JSStringCreateWithUTF8CString(string)) {
  }

	/*!
	  @method
	  
	  @abstract Create a JavaScript string from a null-terminated UTF8
	  string.
	  
	  @param string The null-terminated UTF8 string to copy into the new
	  JSString.
	  
	  @result A JSString containing string.
	*/
	explicit JSString(const std::string& string) : JSString(string.c_str()) {
	}

	~JSString() {
		JSStringRelease(js_string_ref__);
	}

	// Copy constructor.
	JSString(const JSString& rhs) : js_string_ref__(rhs.js_string_ref__) {
		JSStringRetain(js_string_ref__);
	}

	// Move constructor.
	JSString(JSString&& rhs) : js_string_ref__(rhs.js_string_ref__) {
		JSStringRetain(js_string_ref__);
	}
	
#ifdef JAVASCRIPTCORECPP_RAII_MOVE_SEMANTICS_ENABLE
  JSString& JSString::operator=(const JSString&) = default;
  JSString& JSString::operator=(JSString&&) = default;
#endif

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
    swap(first.js_string_ref__, second.js_string_ref__);
  }

	const std::size_t length() const {
		return JSStringGetLength(js_string_ref__);
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
		const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
		return std::u16string(string_ptr, string_ptr + JSStringGetLength(js_string_ref__));
	}

private:

  // For interoperability with the JavaScriptCore C API.
  JSString(const JSStringRef& js_string_ref) : js_string_ref__(js_string_ref) {
	  assert(js_string_ref__);
    JSStringRetain(js_string_ref__);
  }
  
  // For interoperability with the JavaScriptCore C API.
  operator JSStringRef() const {
    return js_string_ref__;
  }

  // JSContext::JSEvaluateScript and JSContext::JSCheckScriptSyntax
  // need access to operator JSStringRef().
  friend class JSContext;

  // The JSValue constructor needs access to operator JSStringRef().
  friend class JSValue;

  // JSObject::HasProperty needs access to operator JSStringRef().
  friend class JSObject;

  // JSPropertyNameArray::GetNameAtIndex needs access to operator
  // JSStringRef().
  friend class detail::JSPropertyNameArray;

  // JSPropertyNameAccumulator::AddName needs access to operator
  // JSStringRef().
  friend class JSPropertyNameAccumulator;

  // The JSFunction constructor needs access to operator
  // JSStringRef().
  friend class JSFunction;

  // The JSNativeClass static functions need access to the JSString
  // constructor.
  template<typename T>
	friend class JSNativeClass;
	
  // template<typename T>
	// friend class JSNativeObject;

  // Return true if the two JSStrings are equal.
	friend bool operator==(const JSString& lhs, const JSString& rhs);

	JSStringRef js_string_ref__;
	JAVASCRIPTCORECPP_RAII_JSSTRING_MUTEX;
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

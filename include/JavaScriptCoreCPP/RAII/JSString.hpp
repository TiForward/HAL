/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_

#include <locale>
#include <cstddef>
#include <codecvt>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

class JSContext;
class JSValue;
class JSObject;

/*!
  @class JSString
  @discussion A JSString is an RAII wrapper around a JSStringRef, the
  JavaScriptCore C API representation of a JavaScript string.
*/
class JSString final	{
	
 public:
	
	// Create an empty JSString with a length of zero characters.
	JSString() : js_string_ref_(JSStringCreateWithUTF8CString(nullptr)) {
	}
	
	/*!
	  @method
	  @abstract     Create a JavaScript string from a null-terminated UTF8 string.
	  @param string The null-terminated UTF8 string to copy into the new JSString.
	  @result       A JSString containing string.
	*/
	JSString(const char* string) : js_string_ref_(JSStringCreateWithUTF8CString(string)) {
  }

	/*!
	  @method
	  @abstract     Create a JavaScript string from a null-terminated UTF8 string.
	  @param string The null-terminated UTF8 string to copy into the new JSString.
	  @result       A JSString containing string.
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

  friend class JSValue;
  friend class JSObject;
  friend class JSPropertyNameArray;
  friend class JSPropertyNameAccumulator;
  friend class JSFunction;
  
	// Return true if the two JSStrings are equal.
	friend bool operator==(const JSString& lhs, const JSString& rhs);

	friend JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSString& source_url, int starting_line_number);
	friend JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSObject& this_object, const JSString& source_url, int starting_line_number);
	friend bool JSCheckScriptSyntax(const JSContext& js_context, const JSString& script, const JSString& source_url, int starting_line_number);

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

inline
std::ostream& operator << (std::ostream& ostream, const JSString& js_string) {
	ostream << static_cast<std::string>(js_string);
	return ostream;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

namespace std {

template<>
struct hash<JavaScriptCoreCPP::RAII::JSString> {
	typedef JavaScriptCoreCPP::RAII::JSString argument_type;
	typedef std::size_t                       result_type;
	const std::hash<std::string> string_hash = std::hash<std::string>();
	
	result_type operator()(const argument_type& js_string) const {
		return string_hash(js_string);
	}
};

}  // namespace std


#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_

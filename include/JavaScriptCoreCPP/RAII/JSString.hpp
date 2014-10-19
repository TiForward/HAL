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

namespace JavaScriptCoreCPP {

// A JSString represents a JavaScript string.
class JSString final	{
	
 public:
	
	// Create an empty JSString with a length of zero characters.
	JSString() : js_string_(JSStringCreateWithUTF8CString(nullptr)) {
	}
	
	JSString(const std::string& string) : js_string_(JSStringCreateWithUTF8CString(string.c_str())) {
	}
	
	JSString(const JSStringRef& js_string) : js_string_(js_string) {
		JSStringRetain(js_string_);
	}
	
	~JSString() {
		JSStringRelease(js_string_);
	}

	// Copy constructor.
	JSString(const JSString& rhs) {
		js_string_ = rhs.js_string_;
		JSStringRetain(js_string_);
	}

  // Move constructor.
  JSString(JSString&& rhs) {
    js_string_ = rhs.js_string_;
    JSStringRetain(js_string_);
  }
  
  // Create a copy of another JSContextGroup by assignment. This is a unified
  // assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
  JSString& operator=(JSString rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSString& first, JSString& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_string_, second.js_string_);
  }

	const std::size_t length() const {
		return JSStringGetLength(js_string_);
	}
	
	operator JSStringRef() const {
		return js_string_;
	}
	
	operator std::string() const {
		static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> converter;
		return std::string(converter.to_bytes(static_cast<std::u16string>(*this)));
	}

	operator std::u16string() const {
		const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_);
		return std::u16string(string_ptr, string_ptr + JSStringGetLength(js_string_));
	}

private:

	// Return true if the two JSStrings are equal.
	friend bool operator==(const JSString& lhs, const JSString& rhs);
	
  // Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSStringRef js_string_;
};

// Return true if the two JSString are equal.
inline
bool operator==(const JSString& lhs, const JSString& rhs) {
	return JSStringIsEqual(lhs, rhs);
}

// Return true if the two JSString are not equal.
inline
bool operator!=(const JSString& lhs, const JSString& rhs) {
	return ! (lhs == rhs);
}

inline
std::ostream& operator << (std::ostream& ostream, const JSString& js_string) {
	ostream << static_cast<std::string>(js_string);
	return ostream;
}

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTRING_HPP_

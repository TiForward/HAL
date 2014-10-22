/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @discussion A JSStaticFunction is an RAII wrapper around the
  JavaScriptCore C API JSStaticFunction, which describes a statically
  declared function property of a JSObject.
*/
class JSStaticFunction final	{
	
 public:

	/*!
	  @method

	  @abstract ...................... Create a statically declared
	                                   function property of a JSObject.
	                               
	  @param function_name ........... A JSString containing the
	                                   function's name. If function_name
	                                   is empty then an anonymous
	                                   function is craeted.
	  
	  @param callget_property_callback A JSObjectGetPropertyCallback to
	                                   invoke when getting the
	                                   property's value.
	                                   
	  
	  @param attributes .............. A logically ORed set of
	                                   JSPropertyAttributes to give to
	                                   the property.
	  
	  @result ........................ An object which describes a
	                                   statically declared function
	                                   property of a JSObject
	*/
	JSStaticFunction(const JSString& function_name, JSObject::CallAsFunctionCallback call_as_function_callback, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());

	JSString get_function_name() const {
		return function_name_;
	}
	
	JSObject::CallAsFunctionCallback get_call_as_function_callback() const {
		return call_as_function_callback_;
	}

	std::set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

	~JSStaticFunction() {
  }

	// Copy constructor.
	JSStaticFunction(const JSStaticFunction& rhs);
	
	// Move constructor.
	JSStaticFunction(JSStaticFunction&& rhs);
  
	// Create a copy of another JSStaticFunction by assignment. This is
	// a unified assignment operator that fuses the copy assignment
	// operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSStaticFunction& operator=(JSStaticFunction rhs) {
		swap(*this, rhs);
		return *this;
  }
  
	friend void swap(JSStaticFunction& first, JSStaticFunction& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.function_name_                       , second.function_name_);
		swap(first.function_name_for_js_static_function_, second.function_name_for_js_static_function_);
		swap(first.call_as_function_callback_           , second.call_as_function_callback_);
		swap(first.attributes_                          , second.attributes_);
    swap(first.js_static_function_                  , second.js_static_function_);
	}

 private:
	
	// For interoperability with the JavaScriptCore C API.
	operator ::JSStaticFunction() const {
		return js_static_function_;
	}
	
	// Return true if the two JSStaticFunctions are equal.
	friend bool operator==(const JSStaticFunction& lhs, const JSStaticFunction& rhs);

	// Define a strict weak ordering for two JSStaticFunctions.
	friend bool operator<(const JSStaticFunction& lhs, const JSStaticFunction& rhs);

	JSString                         function_name_;
	std::string                      function_name_for_js_static_function_;
	JSObject::CallAsFunctionCallback call_as_function_callback_ { nullptr };
	std::set<JSPropertyAttribute>    attributes_;
	::JSStaticFunction               js_static_function_ {0, 0, 0};
};

// Return true if the two JSStaticFunctions are equal.
bool operator==(const JSStaticFunction& lhs, const JSStaticFunction& rhs);

// Return true if the two JSString are not equal.
inline
bool operator!=(const JSStaticFunction& lhs, const JSStaticFunction& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSStaticFunctions.
inline
bool operator<(const JSStaticFunction& lhs, const JSStaticFunction& rhs) {
	if (lhs.function_name_for_js_static_function_ < rhs.function_name_for_js_static_function_) {
		return true;
	}

	return lhs.attributes_ < rhs.attributes_;
}

inline
bool operator>(const JSStaticFunction& lhs, const JSStaticFunction& rhs) {
	return rhs < lhs;
}

inline
bool operator<=(const JSStaticFunction& lhs, const JSStaticFunction& rhs) {
	return !(lhs > rhs);
}

inline
bool operator>=(const JSStaticFunction& lhs, const JSStaticFunction& rhs) {
	return !(lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_

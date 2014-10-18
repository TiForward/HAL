/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_

#include <bitset>
#include <JavaScriptCore/JavaScript.h>
#include "Util/JSStringRefRAII.hpp"

namespace JavaScriptCoreCPP {

enum class PropertyAttributes {	None, ReadOnly, DontEnum, DontDelete };

// A JSStaticValue describes a statically declared value property.

// @struct JSStaticValue
// @abstract This structure describes a statically declared value property.
// @field name A null-terminated UTF8 string containing the property's name.
// @field getProperty A JSObjectGetPropertyCallback to invoke when getting the property's value.
// @field setProperty A JSObjectSetPropertyCallback to invoke when setting the property's value. May be NULL if the ReadOnly attribute is set.
// @field attributes A logically ORed set of JSPropertyAttributes to give to the property.
class JSStaticValue final	{
	
 public:

	JSStaticValue(const std::string& name) : js_static_value_(js_static_value), name_(js_static_value.name) {
		js_static_value_.name = name_.c_str();
	}

	JSStaticValue(const JSStaticValue& js_static_value) : js_static_value_(js_static_value), name_(js_static_value.name) {
		js_static_value_.name = name_.c_str();
	}
	
	~JSStaticValue() {
	}
	
	// Copy constructor.
	JSStaticValue(const JSStaticValue& rhs) : js_static_value_(rhs.js_static_value) , name_(rhs.js_static_value.name)	{
		js_static_value_.name = name_.c_str();
	}
	
	// Create a copy of another JSStaticValue by assignment.
	JSStaticValue& operator=(const JSStaticValue& rhs) {
		if (this == &rhs) {
			return *this;
		}

		js_static_value_      = rhs.js_static_value_;
		name_                 = rhs.name_;
		js_static_value_.name = name_.c_str();
		
		return *this;
	}
	
	operator JSStaticValue() const {
		return js_static_value_;
	}
	
 private:

	// Prevent heap based objects.
	static void* operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void* operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSStaticValue js_static_value_;
	std::string   name_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_

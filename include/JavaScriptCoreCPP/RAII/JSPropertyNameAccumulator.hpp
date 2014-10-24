/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSPROPERTYNAMEACCUMULATOR_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSPROPERTYNAMEACCUMULATOR_HPP_

#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <iostream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

class JSObject;

/*!
  @class
  
  @discussion A JSPropertyNameAccumulator is an RAII wrapper around a
  JSPropertyNameAccumulatorRef, the JavaScriptCore C API
  representation of a JavaScript property name accumulator which is an
  ordered set used to collect the names of a JavaScript object's
  properties
*/
class JSPropertyNameAccumulator	{
	
 public:
	
	JSPropertyNameAccumulator() = default;
	~JSPropertyNameAccumulator() = default;
	
	/*!
	  @method
	  
	  @abstract Adds a property name to a JavaScript property name
	  accumulator.
	  
	  @param property_name The property name to add.
	*/
	void AddName(const JSString& property_name) const {
		JSPropertyNameAccumulatorAddName(js_property_name_accumulator_ref_, property_name);
	}
	
private:

	JSPropertyNameAccumulator(const JSPropertyNameAccumulator& rhs) = delete;
	JSPropertyNameAccumulator(JSPropertyNameAccumulator&& rhs) = delete;

	JSPropertyNameAccumulator& operator=(const JSPropertyNameAccumulator& rhs) = delete;
	JSPropertyNameAccumulator& operator=(JSPropertyNameAccumulator&& rhs) = delete;

	// For interoperability with the JavaScriptCore C API.
	JSPropertyNameAccumulator(const JSPropertyNameAccumulatorRef& js_property_name_accumulator_ref) : js_property_name_accumulator_ref_(js_property_name_accumulator_ref) {
		assert(js_property_name_accumulator_ref_);
	}
	
  // For interoperability with the JavaScriptCore C API.
	operator JSPropertyNameAccumulatorRef() const {
		return js_property_name_accumulator_ref_;
	}
	
	friend class JSObject;

	JSPropertyNameAccumulatorRef js_property_name_accumulator_ref_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSPROPERTYNAMEACCUMULATOR_HPP_
#define _JAVASCRIPTCORECPP_JSPROPERTYNAMEACCUMULATOR_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include <iostream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace detail {
  template<typename T>
  class JSExportPimpl;
}}

namespace JavaScriptCoreCPP {

class JSObject;

/*!
  @class
  
  @discussion A JSPropertyNameAccumulator is an RAII wrapper around a
  JSPropertyNameAccumulatorRef, the JavaScriptCore C API
  representation of a JavaScript property name accumulator which is an
  ordered set used to collect the names of a JavaScript object's
  properties
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSPropertyNameAccumulator	: public detail::JSPerformanceCounter<JSPropertyNameAccumulator> {
#else
class JSPropertyNameAccumulator	{
#endif
	
 public:
	
	JSPropertyNameAccumulator() = delete;
	~JSPropertyNameAccumulator() = default;
	
	/*!
	  @method
	  
	  @abstract Adds a property name to a JavaScript property name
	  accumulator.
	  
	  @param property_name The property name to add.
	*/
	void AddName(const JSString& property_name) const {
		JSPropertyNameAccumulatorAddName(js_property_name_accumulator_ref__, property_name);
	}
	
private:

	JSPropertyNameAccumulator(const JSPropertyNameAccumulator& rhs) = delete;
	JSPropertyNameAccumulator(JSPropertyNameAccumulator&& rhs) = delete;

	JSPropertyNameAccumulator& operator=(const JSPropertyNameAccumulator& rhs) = delete;
	JSPropertyNameAccumulator& operator=(JSPropertyNameAccumulator&& rhs) = delete;

	// Only a JSObject and a JSExportPimpl can create a
	// JSPropertyNameAccumulator.
	friend class JSObject;

  template<typename T>
  friend class detail::JSExportPimpl;

	// For interoperability with the JavaScriptCore C API.
	explicit JSPropertyNameAccumulator(const JSPropertyNameAccumulatorRef& js_property_name_accumulator_ref)
			: js_property_name_accumulator_ref__(js_property_name_accumulator_ref) {
		assert(js_property_name_accumulator_ref__);
	}
	
  // For interoperability with the JavaScriptCore C API.
	operator JSPropertyNameAccumulatorRef() const {
		return js_property_name_accumulator_ref__;
	}

	JSPropertyNameAccumulatorRef js_property_name_accumulator_ref__;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSVALUE_HPP_

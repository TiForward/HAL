/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <vector>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JavaScript object of the Array type.
*/
class JSArray final : public JSObject {
	
 public:

	/*!
	  @method

	  @abstract Create JavaScript Array object.
	  
	  @param js_context The execution context to use.
	  
    @result A JavaScript object that is an Array.
	*/
	JSArray(const JSContext& js_context) : JSArray(std::vector<JSValue>(), js_context) {
	}
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript Array object.
	  
	  @discussion The behavior of this constructor does not exactly
	  match the behavior of the built-in Array constructor in that if
	  the vector of arguments contains one element then the JSArray is
	  also created with on element.
	  
	  @param arguments The JavaScript values to populate the array.
	  
	  @param js_context The execution context to use.
	  
    @result A JavaScript object that is an Array, populated with the
    given JavaScript values.
	*/
	JSArray(const std::vector<JSValue>& arguments, const JSContext& js_context);
	
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_

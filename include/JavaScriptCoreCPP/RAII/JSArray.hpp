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
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @class
  @discussion A JavaScript object of the Array type.
*/
class JSArray final	{
	
 public:

	/*!
	  @method
	  @abstract         Create JavaScript Array object.
	  @param js_context The execution context to use.
    @result           A JavaScript object of the Array.
	  @throws           std::logic_error if the JSArray could not be created.
	*/
	JSArray(const JSContext& js_context) : JSArray(std::vector<JSValue>(), js_context) {
	}
	
	/*!
	  @method
	  @abstract         Create a JavaScript Array object.
	  @discussion       The behavior of this constructor does not exactly match the behavior of the built-in Array constructor. Specifically, if one argument is supplied, this function returns an array with one element.
	  @param js_context The execution context to use.
    @result           A JavaScript object of the Array.
	  @throws           std::logic_error if the JSArray could not be created.
	*/
	JSArray(const std::vector<JSValue>& arguments, const JSContext& js_context) : js_object_(js_context) {
		JSValueRef exception { nullptr };
		JSObjectRef js_object_ref = nullptr;
		if (!arguments.empty()) {
			std::vector<JSValueRef> arguments_array;
			std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
			js_object_ref = JSObjectMakeArray(js_context, arguments_array.size(), &arguments_array[0], &exception);
		} else {
			js_object_ref = JSObjectMakeArray(js_context, 0, nullptr, &exception);
		}
		
		if (exception) {
			// assert(!js_object_ref);
			static const std::string log_prefix { "MDL: JSArray(const std::vector<Value>& arguments, const JSContext& js_context): " };
			const std::string message = static_cast<std::string>(JSValue(exception, js_context));
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::logic_error(message);
		}

		js_object_ = JSObject(js_object_ref, js_context);
		JSValueUnprotect(js_context, js_object_ref);
	}

  /*!
	  @method
	  @abstract Convert a JSArray to a std::vector<JSValue>.
	  @result   A std::vector<JSValue> of the contents of the JSArray.
  */
	// operator std::vector<JSValue>() const {
	// 	return JSValueToBoolean(js_value_, js_value_);
	// }

 private:
	
	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSObject js_object_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_

/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_JSARRAY_HPP_
#define _HAL_JSARRAY_HPP_

#include "HAL/JSObject.hpp"
#include <vector>

namespace HAL {

/*!
  @class
  
  @discussion A JavaScript object of the Array type.
  
  The only way to create a JSArray is by using the
  JSContext::CreateArray member function.
*/
class HAL_EXPORT JSArray final : public JSObject HAL_PERFORMANCE_COUNTER2(JSArray) {
	
 private:

	// Only a JSContext can create a JSArray.
	friend JSContext;
	
	JSArray(const JSContext& js_context, const std::vector<JSValue>& arguments = {});

	static JSObjectRef MakeArray(const JSContext& js_context, const std::vector<JSValue>& arguments);
};

} // namespace HAL {

#endif // _HAL_JSARRAY_HPP_

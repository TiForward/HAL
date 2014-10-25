/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <vector>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JavaScript object of the Array type.
  
  The only way to create a JSArray is by using the
  JSContext::CreateArray member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_ENABLE_PERFORMANCE_COUNTER
class JSArray final : public JSObject, public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSArray> {
#else
class JSArray final : public JSObject {
#endif
	
 private:

	// Only a JSContext can create a JSArray.
	JSArray(const JSContext& js_context) : JSArray(js_context, std::vector<JSValue>()) {
	}
	
	// Only a JSContext can create a JSArray.
	JSArray(const JSContext& js_context, const std::vector<JSValue>& arguments);

	friend JSContext;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSARRAY_HPP_

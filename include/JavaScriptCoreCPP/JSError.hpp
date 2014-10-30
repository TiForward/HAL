/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSERROR_HPP_
#define _JAVASCRIPTCORECPP_JSERROR_HPP_

#include "JavaScriptCoreCPP/JSObject.hpp"

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript object of the Error type.

  The only way to create a JSError is by using the
  JSContext::CreateError member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSError final : public JSObject, public detail::JSPerformanceCounter<JSError> {
#else
class JSError final : public JSObject {
#endif
	
 private:

	JSError(const JSContext& js_context) : JSError(js_context, std::vector<JSValue>()) {
	}
	
	JSError(const JSContext& js_context, const std::vector<JSValue>& arguments);
	
	// Only a JSContext can create a JSError.
	friend JSContext;
};

}} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSERROR_HPP_

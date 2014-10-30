/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSUNDEFINED_HPP_
#define _JAVASCRIPTCORECPP_JSUNDEFINED_HPP_

#include "JavaScriptCoreCPP/JSValue.hpp"

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript value of the undefined type.

  The only way to create a JSUndefined is by using the
  JSContext::CreateUndefined member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSUndefined final : public JSValue, public detail::JSPerformanceCounter<JSUndefined> {
#else
class JSUndefined final : public JSValue {
#endif
	
private:
	
	JSUndefined(const JSContext& js_context) : JSValue(js_context, JSValueMakeUndefined(js_context)) {
	}

	// Only a JSContext can create a JSUndefined.
	friend JSContext;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSUNDEFINED_HPP_

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSNULL_HPP_
#define _JAVASCRIPTCORECPP_JSNULL_HPP_

#include "JavaScriptCoreCPP/JSValue.hpp"

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript value of the null type.

  The only way to create a JSNull is by using the
  JSContext::CreateNull member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNull final : public JSValue, public detail::JSPerformanceCounter<JSNull> {
#else
class JSNull final : public JSValue {
#endif
	
private:
	
	JSNull(const JSContext& js_context) : JSValue(js_context, JSValueMakeNull(js_context)) {
	}

	// Only a JSContext can create a JSNull.
	friend JSContext;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSNULL_HPP_

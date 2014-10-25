/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNULL_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNULL_HPP_

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JavaScript value of the null type.

  The only way to create a JSNull is by using the
  JSContext::CreateNull member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_ENABLE_PERFORMANCE_COUNTER
class JSNull final : public JSValue, public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSNull> {
#else
class JSNull final : public JSValue {
#endif
	
private:
	
	// Only a JSContext can create a JSNull.
	JSNull(const JSContext& js_context) : JSValue(js_context, JSValueMakeNull(js_context)) {
	}

	friend JSContext;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNULL_HPP_

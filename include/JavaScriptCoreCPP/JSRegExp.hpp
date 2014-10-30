/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSREGEXP_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSREGEXP_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JavaScript object of the RegExp type.

  The only way to create a JSRegExp is by using the
  JSContext::CreateRegExp member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSRegExp final : public JSObject, public detail::JSPerformanceCounter<JSRegExp> {
#else
class JSRegExp final : public JSObject {
#endif
	
 private:

	JSRegExp(const JSContext& js_context) : JSRegExp(js_context, std::vector<JSValue>()) {
	}
	
	JSRegExp(const JSContext& js_context, const std::vector<JSValue>& arguments);
	
	// Only a JSContext can create a JSRegExp.
	friend JSContext;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSREGEXP_HPP_

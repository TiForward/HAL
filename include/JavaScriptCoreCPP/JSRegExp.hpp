/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSREGEXP_HPP_
#define _JAVASCRIPTCORECPP_JSREGEXP_HPP_

#include "JavaScriptCoreCPP/JSObject.hpp"

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript object of the RegExp type.

  The only way to create a JSRegExp is by using the
  JSContext::CreateRegExp member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSRegExp final : public JSObject, public detail::JSPerformanceCounter<JSRegExp> {
#else
class JSRegExp final : public JSObject {
#endif
	
 private:
	
	// Only a JSContext can create a JSRegExp.
	friend JSContext;

	JSRegExp(const JSContext& js_context, const std::vector<JSValue>& arguments);
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSREGEXP_HPP_

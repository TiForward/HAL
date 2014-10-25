/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JavaScript object of the Function type whose body is
  given as a string of JavaScript code. Use this class when you want
  to execute a script repeatedly to avoid the cost of re-parsing the
  script before each execution.

  The only way to create a JSFunction is by using the
  JSContext::CreateFunction member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_ENABLE_PERFORMANCE_COUNTER
class JSFunction final : public JSObject, public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSFunction> {
#else
class JSFunction final : public JSObject {
#endif
	
 private:

	// Only a JSContext can create a JSFunction.
	JSFunction(const JSContext& js_context, const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSString& source_url, int starting_line_number);

	friend JSContext;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_

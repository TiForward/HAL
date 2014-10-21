/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JavaScript object of the Function type.
*/
class JSFunction final : public JSObject {
	
 public:

	/*!
	  @method
	  @abstract                   Create a JavaScript function with a given script as its body.
	  @param function_name        A JSString containing the function's name. This will be used when converting the function to string. Pass an empty string to create an anonymous function.
	  @param parameter_names      A JSString array containing the names of the function's parameters.
	  @param body                 A JSString containing the script to use as the function's body.
	  @param js_context           The execution context to use.
	  @param source_url           An optional JSString containing a URL for the script's source file. This is only used when reporting exceptions.
	  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
	  @result                     A JSObject that is a function. The object's prototype will be the default function prototype.
	  @throws                     std::invalid_argument if either body or parameter_names contains a syntax error.
	  @discussion                 Use this method when you want to execute a script repeatedly, to avoid the cost of re-parsing the script before each execution.
	*/
	JSFunction(const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSContext& js_context, const JSString& source_url = JSString(), int starting_line_number = 1);
};


}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSFUNCTION_HPP_

/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSERROR_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSERROR_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JavaScript object of the Error type.
*/
class JSError final : public JSObject {
	
 public:

	/*!
	  @method
	  @abstract         Create JavaScript Error object.
	  @param js_context The execution context to use.
    @result           A JavaScript object of the Error type.
	*/
	JSError(const JSContext& js_context) : JSError(std::vector<JSValue>(), js_context) {
	}
	
	/*!
	  @method
	  @abstract         Create a JavaScript Error object, as if by invoking the built-in Error constructor.
	  @param arguments  The JavaScript values to pass to the Error Constructor.
	  @param js_context The execution context to use.
    @result           A JSObject that is a Error.
	*/
	JSError(const std::vector<JSValue>& arguments, const JSContext& js_context);
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSERROR_HPP_

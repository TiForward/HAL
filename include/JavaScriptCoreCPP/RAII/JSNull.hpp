/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNULL_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNULL_HPP_

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JavaScript value of the null type.
*/
class JSNull final : public JSValue {
	
public:
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the null type.
	  
	  @param js_context The execution context to use.
	  
	  @result The unique null value.
	*/
	JSNull(const JSContext& js_context) : JSValue(js_context, JSValueMakeNull(js_context)) {
	}
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNULL_HPP_

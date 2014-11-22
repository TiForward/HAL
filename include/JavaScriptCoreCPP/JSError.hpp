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
class JAVASCRIPTCORECPP_EXPORT JSError final : public JSObject JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSError) {
	
 private:
	
	// Only a JSContext can create a JSError.
	friend JSContext;
	
	JSError(const JSContext& js_context, const std::vector<JSValue>& arguments = {});

	static JSObjectRef MakeError(const JSContext& js_context, const std::vector<JSValue>& arguments);
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSERROR_HPP_

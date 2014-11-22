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
class JAVASCRIPTCORECPP_EXPORT JSUndefined final : public JSValue JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSUndefined) {
	
private:

	// Only a JSContext can create a JSUndefined.
	friend JSContext;
	
	explicit JSUndefined(const JSContext& js_context)
			: JSValue(js_context, JSValueMakeUndefined(js_context)) {
	}
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSUNDEFINED_HPP_

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASSATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_JSCLASSATTRIBUTE_HPP_

#include <cstddef>
#include <functional>

namespace JavaScriptCoreCPP {

/*!
  @enum
  
  @constant None Specifies that a JSClass has no special attributes.
  
  @constant NoAutomaticPrototype Specifies that a JSClass should not
  automatically generate a shared prototype for its instance
  objects. Use NoAutomaticPrototype in combination with
  JSObject::SetPrototype to manage prototypes manually.
*/
enum class JSClassAttribute : std::uint32_t /* typedef unsigned JSClassAttributes */ {
	None,
	NoAutomaticPrototype
};

} // JavaScriptCoreCPP

#endif // _JAVASCRIPTCORECPP_JSCLASSATTRIBUTE_HPP_

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSNATIVECLASSATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_JSNATIVECLASSATTRIBUTE_HPP_

#include <functional>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @enum
  
  @constant None Specifies that a JSNativeClass has no special
  attributes.
  
  @constant NoAutomaticPrototype Specifies that a JSNativeClass should
  not automatically generate a shared prototype for its instance
  objects. Use NoAutomaticPrototype in combination with
  JSObject::SetPrototype to manage prototypes manually.
*/
enum class JSNativeClassAttribute : JSClassAttributes {
	None,
	NoAutomaticPrototype
};

} // namespace JavaScriptCoreCPP {


// Provide a hash function so that a JSNativeClassAttribute can be
// stored in an unordered container.
namespace std {

using namespace JavaScriptCoreCPP;

template<>
struct hash<JSNativeClassAttribute> {
	using argument_type = JSNativeClassAttribute;
	using result_type   = std::size_t;

	using class_attribute_underlying_type = std::underlying_type<JSNativeClassAttribute>::type;
	
	result_type operator()(const argument_type& class_attribute) const {
		return std::hash<class_attribute_underlying_type>()(static_cast<class_attribute_underlying_type>(class_attribute));
	}
};

}  // namespace std {

#endif // _JAVASCRIPTCORECPP_JSNATIVECLASSATTRIBUTE_HPP_

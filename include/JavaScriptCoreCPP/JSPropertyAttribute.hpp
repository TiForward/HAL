/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSPROPERTYATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_JSPROPERTYATTRIBUTE_HPP_

#include <cstddef>
#include <functional>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @enum

  @constant None Specifies that a property has no special attributes.
  
  @constant ReadOnly Specifies that a property is read-only.
  
  @constant DontEnum Specifies that a property should not be
  enumerated by JSPropertyEnumerators and JavaScript for...in loops.
  
  @constant DontDelete Specifies that the delete operation should fail
  on a property.
*/
enum class JSPropertyAttribute : JSPropertyAttributes {
	None,
	ReadOnly,
	DontEnum,
	DontDelete
};

} // namespace JavaScriptCoreCPP {


// Provide a hash function so that a JSPropertyAttribute can be stored
// in an unordered container.
namespace std {

using namespace JavaScriptCoreCPP;

template<>
struct hash<JSPropertyAttribute> {
	using argument_type = JSPropertyAttribute;
	using result_type   = std::size_t;
	
	using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
	
	result_type operator()(const argument_type& property_attribute) const {
		return std::hash<property_attribute_underlying_type>()(static_cast<property_attribute_underlying_type>(property_attribute));
	}
};

}  // namespace std {

#endif // _JAVASCRIPTCORECPP_JSPROPERTYATTRIBUTE_HPP_

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
enum class JSPropertyAttribute : unsigned /* typedef unsigned JSPropertyAttributes; */ {
	None,
	ReadOnly,
	DontEnum,
	DontDelete
};

} // namespace JavaScriptCoreCPP {


// Provide a hash function so that a JSPropertyAttribute can be stored
// in an unordered container.
namespace std {

using JavaScriptCoreCPP::JSPropertyAttribute;

template<>
struct hash<JSPropertyAttribute> {
	using argument_type   = JSPropertyAttribute;
	using result_type     = std::size_t;
	using underlying_type = std::underlying_type<argument_type>::type;
	static const std::hash<underlying_type> hash_function;
	
	result_type operator()(const argument_type& property_attribute) const {
		return hash_function(static_cast<underlying_type>(property_attribute));
	}
};

template<>
std::hash<hash<JSPropertyAttribute>::underlying_type> hash<JSPropertyAttribute>::hash_function = std::hash<hash<JSPropertyAttribute>::underlying_type>();

}  // namespace std {

#endif // _JAVASCRIPTCORECPP_JSPROPERTYATTRIBUTE_HPP_

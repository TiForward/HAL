/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTATTRIBUTE_HPP_

#include <functional>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @enum
  
  @constant None Specifies that a JSNativeObject has no special
  attributes.
  
  @constant NoAutomaticPrototype Specifies that a JSNativeObject
  should not automatically generate a shared prototype for its
  instance objects. Use NoAutomaticPrototype in combination with
  JSObject::SetPrototype to manage prototypes manually.
*/
enum class JSNativeObjectAttribute {
	None,
	NoAutomaticPrototype
};
}} // namespace JavaScriptCoreCPP { namespace RAII {

// Provide a hash function so that a JSNativeObjectAttribute can be
// stored in an unordered container.
namespace std {

template<>
struct hash<JavaScriptCoreCPP::RAII::JSNativeObjectAttribute> {
	using argument_type = JavaScriptCoreCPP::RAII::JSNativeObjectAttribute;
	using result_type   = std::size_t;

	using property_attribute_underlying_type = std::underlying_type<JavaScriptCoreCPP::RAII::JSNativeObjectAttribute>::type;
	const std::hash<property_attribute_underlying_type> property_attribute_hash = std::hash<property_attribute_underlying_type>();
	
	result_type operator()(const argument_type& property_attribute) const {
		return property_attribute_hash(static_cast<property_attribute_underlying_type>(property_attribute));
	}
};
}  // namespace std {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTATTRIBUTE_HPP_

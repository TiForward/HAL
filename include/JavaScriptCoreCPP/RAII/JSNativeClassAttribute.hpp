/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSATTRIBUTE_HPP_

#include <functional>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @enum
  
  @constant None Specifies that a JSNativeClass has no special
  attributes.
  
  @constant NoAutomaticPrototype Specifies that a JSNativeClass should
  not automatically generate a shared prototype for its instance
  objects. Use NoAutomaticPrototype in combination with
  JSObject::SetPrototype to manage prototypes manually.
*/
enum class JSNativeClassAttribute {
	None,
	NoAutomaticPrototype
};
}} // namespace JavaScriptCoreCPP { namespace RAII {


// Provide a hash function so that a JSNativeClassAttribute can be
// stored in an unordered container.
namespace std {

using namespace JavaScriptCoreCPP::RAII;

template<>
struct hash<JSNativeClassAttribute> {
	using argument_type = JSNativeClassAttribute;
	using result_type   = std::size_t;

	using property_attribute_underlying_type = std::underlying_type<JSNativeClassAttribute>::type;
	const std::hash<property_attribute_underlying_type> property_attribute_hash = std::hash<property_attribute_underlying_type>();
	
	result_type operator()(const argument_type& property_attribute) const {
		return property_attribute_hash(static_cast<property_attribute_underlying_type>(property_attribute));
	}
};
}  // namespace std {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSATTRIBUTE_HPP_

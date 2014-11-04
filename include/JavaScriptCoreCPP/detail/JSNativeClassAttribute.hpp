/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSATTRIBUTE_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSATTRIBUTE_HPP_

#include <cstddef>
#include <functional>

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @enum
  
  @constant None Specifies that a JSNativeClass has no special
  attributes.
  
  @constant NoAutomaticPrototype Specifies that a JSNativeClass should
  not automatically generate a shared prototype for its instance
  objects. Use NoAutomaticPrototype in combination with
  JSObject::SetPrototype to manage prototypes manually.
*/
enum class JSNativeClassAttribute : unsigned /* typedef unsigned JSClassAttributes */ {
	None,
	NoAutomaticPrototype
};

} // JavaScriptCoreCPP { namespace detail {


// Provide a hash function so that a JSNativeClassAttribute can be
// stored in an unordered container.
namespace std {

using namespace JavaScriptCoreCPP::detail::JSNativeClassAttribute;

template<>
struct hash<JSNativeClassAttribute> {
	using argument_type   = JSNativeClassAttribute;
	using result_type     = std::size_t;
	using underlying_type = std::underlying_type<JSNativeClassAttribute>::type;
	static const std::hash<underlying_type> hash_function;
	
	result_type operator()(const argument_type& class_attribute) const {
		return hash_function(static_cast<underlying_type>(class_attribute));
	}
};
	
template<>
std::hash<hash<JSClassAttribute>::underlying_type> hash<JSClassAttribute>::hash_function = std::hash<hash<JSClassAttribute>::underlying_type>();

}  // namespace std {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSATTRIBUTE_HPP_

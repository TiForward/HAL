/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSUTIL_HPP_
#define _JAVASCRIPTCORECPP_JSUTIL_HPP_

#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/JSNativeClassAttribute.hpp"

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace JavaScriptCoreCPP { namespace detail {

using namespace JavaScriptCoreCPP;

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}


// For hash functions for std::unordered_set<JSPropertyAttribute> and
// interoperability with the JavaScriptCore C API.
JSPropertyAttributes ToJSPropertyAttributes(const std::unordered_set<JSPropertyAttribute>& attributes);

// For hash functions for std::unordered_set<JSNativeClassAttribute>
// and interoperability with the JavaScriptCore C API.
JSClassAttributes ToJSClassAttributes(const std::unordered_set<JSNativeClassAttribute>& attributes);

// This in the ToInt32 operation as defined in section 9.5 of the
// ECMA-262 spec. Note that this operation is identical to ToUInt32
// other than to interpretation of the resulting bit-pattern (as such
// this method is also called to implement ToUInt32).
//
// The operation can be descibed as round towards zero, then select
// the 32 least bits of the resulting value in 2s-complement
// representation.
int32_t to_int32_t(double number);

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_JSUTIL_HPP_

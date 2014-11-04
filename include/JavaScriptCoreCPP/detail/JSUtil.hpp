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

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_set>

extern "C" {
  struct JSStringRef;
  struct JSValueRef;
}

namespace JavaScriptCoreCPP {
class JSString;
class JSContext;
class JSValue;
}

namespace JavaScriptCoreCPP { namespace detail {

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

// Log and throw a std::logic_error from an internal component.
void      ThrowLogicError(const std::string& internal_component_name, const std::string& message  );
void      ThrowLogicError(const std::string& internal_component_name, const JSValue&     exception);
void    ThrowRuntimeError(const std::string& internal_component_name, const std::string& message  );
void    ThrowRuntimeError(const std::string& internal_component_name, const JSValue&     exception);
void ThrowInvalidArgument(const std::string& internal_component_name, const std::string& message  );

// For interoperability with the JavaScriptCore C API.
std::vector<JSValue>     to_vector(const JSContext&, size_t count, const JSValueRef[]);
std::vector<JSValue>     to_vector(const JSContext&, const std::vector<JSString>&);
std::vector<JSValueRef>  to_vector(const std::vector<JSValue>& );
std::vector<JSStringRef> to_vector(const std::vector<JSString>&);

// For interoperability with the JavaScriptCore C API.
/* typedef unsigned JSPropertyAttributes */ unsigned ToJSPropertyAttributes(const std::unordered_set<JSPropertyAttribute>& attributes);

// For interoperability with the JavaScriptCore C API.
/* typedef unsigned JSClassAttributes */ unsigned ToJSClassAttribute(JSNativeClassAttribute attribute);

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

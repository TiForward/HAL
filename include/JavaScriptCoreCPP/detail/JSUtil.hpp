/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSUTIL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSUTIL_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/JSClassAttribute.hpp"

#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_set>

#include <JavaScriptCore/JavaScript.h>

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
  JAVASCRIPTCORECPP_EXPORT void      ThrowLogicError(const std::string& internal_component_name, const std::string& message  );
  JAVASCRIPTCORECPP_EXPORT void      ThrowLogicError(const std::string& internal_component_name, const JSValue&     exception);
  JAVASCRIPTCORECPP_EXPORT void    ThrowRuntimeError(const std::string& internal_component_name, const std::string& message  );
  JAVASCRIPTCORECPP_EXPORT void    ThrowRuntimeError(const std::string& internal_component_name, const JSValue&     exception);
  JAVASCRIPTCORECPP_EXPORT void ThrowInvalidArgument(const std::string& internal_component_name, const std::string& message  );
  
  // For interoperability with the JavaScriptCore C API.
  JAVASCRIPTCORECPP_EXPORT std::vector<JSValue>     to_vector(const JSContext&, size_t count, const JSValueRef[]);
  JAVASCRIPTCORECPP_EXPORT std::vector<JSValue>     to_vector(const JSContext&, const std::vector<JSString>&);
  JAVASCRIPTCORECPP_EXPORT std::vector<JSValueRef>  to_vector(const std::vector<JSValue>&);
  JAVASCRIPTCORECPP_EXPORT std::vector<JSStringRef> to_vector(const std::vector<JSString>&);
  
  // For interoperability with the JavaScriptCore C API.
  
  // typedef unsigned JSPropertyAttributes
  JAVASCRIPTCORECPP_EXPORT unsigned ToJSPropertyAttributes(const std::unordered_set<JSPropertyAttribute>& attributes)          JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::unordered_set<JSPropertyAttribute> FromJSPropertyAttributes(::JSPropertyAttributes attributes) JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string(JSPropertyAttribute)                                                          JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string(const std::unordered_set<JSPropertyAttribute>& attributes)                    JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string_JSPropertyAttributes(::JSPropertyAttributes attributes)                       JAVASCRIPTCORECPP_NOEXCEPT;
  
  JAVASCRIPTCORECPP_EXPORT unsigned ToJSClassAttribute(JSClassAttribute attribute)                                             JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::unordered_set<JSClassAttribute> FromJSClassAttributes(::JSClassAttributes attributes)          JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string(JSClassAttribute)                                                             JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string(const std::unordered_set<JSClassAttribute>& attributes)                       JAVASCRIPTCORECPP_NOEXCEPT;
  JAVASCRIPTCORECPP_EXPORT std::string to_string_JSClassAttributes(::JSClassAttributes attributes)                             JAVASCRIPTCORECPP_NOEXCEPT;

  // This in the ToInt32 operation as defined in section 9.5 of the
  // ECMA-262 spec. Note that this operation is identical to ToUInt32
  // other than to interpretation of the resulting bit-pattern (as
  // such this method is also called to implement ToUInt32).
  //
  // The operation can be descibed as round towards zero, then select
  // the 32 least bits of the resulting value in 2s-complement
  // representation.
  JAVASCRIPTCORECPP_EXPORT int32_t to_int32_t(double number);
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSUTIL_HPP_

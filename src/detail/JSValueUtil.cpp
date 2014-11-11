/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSValueUtil.hpp"

namespace JavaScriptCoreCPP { namespace detail {
  
  JSValue::Type ToJSValueType(JSType type) JAVASCRIPTCORECPP_NOEXCEPT {
    switch (type) {
      case kJSTypeUndefined:
        return JSValue::Type::Undefined;
      case kJSTypeNull:
        return JSValue::Type::Null;
      case kJSTypeBoolean:
        return JSValue::Type::Boolean;
      case kJSTypeNumber:
        return JSValue::Type::Number;
      case kJSTypeString:
        return JSValue::Type::String;
      case kJSTypeObject:
        return JSValue::Type::Object;
    }
  }
  
 
}} // namespace JavaScriptCoreCPP { namespace detail {

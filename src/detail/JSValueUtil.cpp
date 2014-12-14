/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/detail/JSValueUtil.hpp"

namespace HAL { namespace detail {
  
  JSValue::Type ToJSValueType(JSType js_type) HAL_NOEXCEPT {
	  auto type = JSValue::Type::Undefined;
    switch (js_type) {
      case kJSTypeUndefined:
        type = JSValue::Type::Undefined;
        break;
      case kJSTypeNull:
        type = JSValue::Type::Null;
        break;
      case kJSTypeBoolean:
        type = JSValue::Type::Boolean;
        break;
      case kJSTypeNumber:
        type = JSValue::Type::Number;
        break;
      case kJSTypeString:
        type = JSValue::Type::String;
        break;
      case kJSTypeObject:
        type = JSValue::Type::Object;
        break;
    }

    return type;
  }
  
 
}} // namespace HAL { namespace detail {

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"

#include <string>

namespace JavaScriptCoreCPP {
  
  JSClass::JSClass() {
    js_class_definition__ = kJSClassDefinitionEmpty;
    js_class_definition__.className = "Default";
    js_class_ref__ = JSClassCreate(&js_class_definition__);
  }
  
  std::string JSClass::get_name() const {
    return js_class_definition__.className;
  }
  
  std::uint32_t JSClass::get_version() const {
    return js_class_definition__.version;
  }
  
  JSClass::operator JSClassRef() const {
    return js_class_ref__;
  }
  
} // namespace JavaScriptCoreCPP {

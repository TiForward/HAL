/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSExport.hpp"
#include "JavaScriptCoreCPP/detail/JSClassBuilder.hpp"

namespace JavaScriptCoreCPP {
  
  //class DefaultJSClass : public JSExport<DefaultJSClass> {
  //  public:
  //
  //  DefaultJSClass(const JSContext& js_context)
  //  : JSExport<DefaultJSClass>(js_context) {
  //  }
  //
  //  static void JSExportInitialize() {
  //    SetClassName("Default");
  //  }
  //
  //};
  
  //JSClass& JSClass::EmptyJSClass() {
  //	static std::once_flag of;
  //  static std::shared_ptr<JSClass> default_js_class_ptr;
  //	std::call_once(of, [] {
  //    detail::JSClassBuilder<DefaultJSClass> builder("Default");
  //    default_js_class_ptr = builder.build();
  //		});
  //
  //	return *default_js_class_ptr;
  //}
  
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

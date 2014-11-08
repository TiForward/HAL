/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <string>
#include <algorithm>

namespace JavaScriptCoreCPP {
  
  JSClassDefinition::JSClassDefinition() noexcept {
    js_class_definition__ = kJSClassDefinitionEmpty;
  }
  
  JSClassDefinition::JSClassDefinition(const ::JSClassDefinition& js_class_definition)
  : name__(js_class_definition.className) {
    
    Initialize(js_class_definition);
    
    js_class_definition__.staticValues    = nullptr;
    js_class_definition__.staticFunctions = nullptr;
    
    auto static_value_ptr = const_cast<::JSStaticValue*>(js_class_definition.staticValues);
    while (static_value_ptr && static_value_ptr -> name) {
      js_value_properties__.push_back(detail::JSStaticValue(*static_value_ptr));
      ++static_value_ptr;
    }

    auto static_function_ptr = const_cast<::JSStaticFunction*>(js_class_definition.staticFunctions);
    while (static_function_ptr && static_function_ptr -> name) {
      js_function_properties__.push_back(detail::JSStaticFunction(*static_function_ptr));
      ++static_function_ptr;
    }
    
    InitializePropertyCallbacks();
  }
  
  JSClassDefinition::~JSClassDefinition() noexcept {
  }
  
  JSClassDefinition::JSClassDefinition(const JSClassDefinition& rhs) noexcept
  : name__(rhs.name__)
  , js_value_properties__(rhs.js_value_properties__)
  , js_function_properties__(rhs.js_function_properties__) {

    Initialize(rhs.js_class_definition__);
    js_class_definition__.className = name__.c_str();
    InitializePropertyCallbacks();
  }
  
  JSClassDefinition::JSClassDefinition(JSClassDefinition&& rhs) noexcept
  : name__(std::move(rhs.name__))
  , js_value_properties__(std::move(rhs.js_value_properties__))
  , js_function_properties__(std::move(rhs.js_function_properties__)) {
    
    Initialize(rhs.js_class_definition__);
    js_class_definition__.className = name__.c_str();
    InitializePropertyCallbacks();
  }
  
  JSClassDefinition& JSClassDefinition::operator=(const JSClassDefinition& rhs) noexcept {
    JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD;

    name__                   = rhs.name__;
    js_value_properties__    = rhs.js_value_properties__;
    js_function_properties__ = rhs.js_function_properties__;
    
    Initialize(rhs.js_class_definition__);
    js_class_definition__.className = name__.c_str();
    InitializePropertyCallbacks();
    
    return *this;
  }
  
  JSClassDefinition& JSClassDefinition::operator=(JSClassDefinition&& rhs) noexcept {
    JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD;

    swap(rhs);
    return *this;
  }
  
  void JSClassDefinition::swap(JSClassDefinition& other) noexcept {
    JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD;
    using std::swap;

    swap(name__                  , other.name__);
    swap(js_value_properties__   , other.js_value_properties__);
    swap(js_function_properties__, other.js_function_properties__);
    swap(static_values__         , other.static_values__);
    swap(static_functions__      , other.static_functions__);
    swap(js_class_definition__   , other.js_class_definition__);
  }

  void JSClassDefinition::JSClassDefinition::Initialize(const ::JSClassDefinition& other) noexcept {
    js_class_definition__.version           = other.version;
    js_class_definition__.attributes        = other.attributes;
    
    js_class_definition__.className         = other.className;
    js_class_definition__.parentClass       = other.parentClass;
    
    js_class_definition__.initialize        = other.initialize;
    js_class_definition__.finalize          = other.finalize;
    js_class_definition__.hasProperty       = other.hasProperty;
    js_class_definition__.getProperty       = other.getProperty;
    js_class_definition__.setProperty       = other.setProperty;
    js_class_definition__.deleteProperty    = other.deleteProperty;
    js_class_definition__.getPropertyNames  = other.getPropertyNames;
    js_class_definition__.callAsFunction    = other.callAsFunction;
    js_class_definition__.callAsConstructor = other.callAsConstructor;
    js_class_definition__.hasInstance       = other.hasInstance;
    js_class_definition__.convertToType     = other.convertToType;
  }
  
  void JSClassDefinition::InitializePropertyCallbacks() noexcept {
    JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD;

    static_values__.clear();
    js_class_definition__.staticValues = nullptr;
    if (!js_value_properties__.empty()) {
      std::transform(js_value_properties__.begin(),
                     js_value_properties__.end(),
                     std::back_inserter(static_values__),
                     [](detail::JSStaticValue js_value_property) {
                       ::JSStaticValue static_value;
                       static_value.name        = js_value_property.get_name().c_str();
                       static_value.getProperty = js_value_property.get_callback();
                       static_value.setProperty = js_value_property.set_callback();
                       static_value.attributes  = detail::ToJSPropertyAttributes(js_value_property.get_attributes());
                       return static_value;
                     });
      
      static_values__.push_back({nullptr, nullptr, nullptr, kJSPropertyAttributeNone});
      js_class_definition__.staticValues = &static_values__[0];
    }
    
    static_functions__.clear();
    js_class_definition__.staticFunctions = nullptr;
    if (!js_function_properties__.empty()) {
      std::transform(js_function_properties__.begin(),
                     js_function_properties__.end(),
                     std::back_inserter(static_functions__),
                     [](detail::JSStaticFunction js_function_property) {
                       ::JSStaticFunction static_function;
                       static_function.name           = js_function_property.get_name().c_str();
                       static_function.callAsFunction = js_function_property.function_callback();
                       static_function.attributes     = detail::ToJSPropertyAttributes(js_function_property.get_attributes());
                       return static_function;
                     });
      
      static_functions__.push_back({nullptr, nullptr, kJSPropertyAttributeNone});
      js_class_definition__.staticFunctions = &static_functions__[0];
    }
  }
  
  std::string JSClassDefinition::get_name() const noexcept {
    return name__;
  }
  
  std::uint32_t JSClassDefinition::get_version() const noexcept {
    return js_class_definition__.version;
  }
  
} // namespace JavaScriptCoreCPP {

/**
 * Javascriptcorecpp
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITION_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITION_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSClassDefinition.hpp"
#include "JavaScriptCoreCPP/JSClassAttribute.hpp"

#include "JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"

#include <string>
#include <unordered_map>

namespace JavaScriptCoreCPP { namespace detail {
  
  template<typename T>
  using JSExportNamedValuePropertyCallbackMap_t    = std::unordered_map<std::string, JSExportNamedValuePropertyCallback<T>>;
  
  template<typename T>
  using JSExportNamedFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSExportNamedFunctionPropertyCallback<T>>;
  
  template<typename T>
  class JSExportClassDefinitionBuilder;
  
  /*!
   @class
   
   @discussion A JSExportClassDefinition defines the properties and
   callbacks of a JavaScript object implemented by a C++ classed
   derived from JSExport.
   
   The only way to create a JSExportClassDefinition is by using a
   JSExportClassDefinitionBuilder.
   
   This class is thread safe and immutable by design.
   */
  template<typename T>
  class JSExportClassDefinition final : public JSClassDefinition JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSExportClassDefinition<T>) {
  public:
    
    JSExportClassDefinition(const JSExportClassDefinitionBuilder<T>& builder);
    
    JSExportNamedValuePropertyCallbackMap_t<T> get_named_value_property_callback_map() {
      return named_value_property_callback_map__;
    }
    
    JSExportNamedFunctionPropertyCallbackMap_t<T> get_named_function_property_callback_map() {
      return named_function_property_callback_map__;
    }

    JSExportClassDefinition()                                          = delete;
    ~JSExportClassDefinition()                                         = default;
    JSExportClassDefinition(const JSExportClassDefinition&)            = default;
    JSExportClassDefinition(JSExportClassDefinition&&)                 = default;
    JSExportClassDefinition& operator=(const JSExportClassDefinition&) = default;
    JSExportClassDefinition& operator=(JSExportClassDefinition&&)      = default;
    
  private:
    
    JSExportNamedValuePropertyCallbackMap_t<T>    named_value_property_callback_map__;
    JSExportNamedFunctionPropertyCallbackMap_t<T> named_function_property_callback_map__;
  };
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITION_HPP_

/**
 * Javascriptcorecpp
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASSDEFINITION_HPP_
#define _JAVASCRIPTCORECPP_JSCLASSDEFINITION_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/detail/JSStaticValue.hpp"
#include "JavaScriptCoreCPP/detail/JSStaticFunction.hpp"

#include <string>
#include <vector>
#include <memory>

namespace JavaScriptCoreCPP { namespace detail {
  template<typename T>
  class JSExportClass;
}}

namespace JavaScriptCoreCPP {
  
  /*!
   @class
   
   @discussion A JSClassDefinition is an RAII wrapper around the
   JavaScriptCore C API struct of the same name (only in a different
   namespace). It defines the properties and callbacks that define a
   type of JavaScript object.
   
   This class is thread safe and immutable by design.
   */
  class JSClassDefinition JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSClassDefinition) {
  public:
    
    JSClassDefinition() noexcept;
    
    explicit JSClassDefinition(const ::JSClassDefinition& js_class_definition);
    
    /*!
     @method
     
     @abstract Return the name of this JSClassDefinition.
     
     @result The name of this JSClassDefinition.
     */
    virtual std::string get_name() const noexcept final;
    
    /*!
     @method
     
     @abstract Return the version number of this JSClassDefinition.
     
     @result The version number of this JSClassDefinition.
     */
    virtual std::uint32_t get_version() const noexcept final;
    
    virtual ~JSClassDefinition() noexcept;
    JSClassDefinition(const JSClassDefinition&) noexcept;
    JSClassDefinition(JSClassDefinition&&) noexcept;
    JSClassDefinition& operator=(const JSClassDefinition&) noexcept;
    JSClassDefinition& operator=(JSClassDefinition&&) noexcept;
    void swap(JSClassDefinition&) noexcept;
    
  private:
    
    virtual void InitializePropertyCallbacks() noexcept final;
    
  protected:
    
    virtual void Initialize(const ::JSClassDefinition& other) noexcept final;
    
    // JSClass and JSExportClass need access to js_class_definition__.
    friend class JSClass;
    
    template<typename T>
    friend class detail::JSExportClass;
    
    std::string                           name__ { "Empty" };
    std::vector<detail::JSStaticValue>    js_value_properties__;
    std::vector<detail::JSStaticFunction> js_function_properties__;
    std::vector<::JSStaticValue>          static_values__;
    std::vector<::JSStaticFunction>       static_functions__;
    ::JSClassDefinition                   js_class_definition__;


#undef  JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
    std::recursive_mutex mutex__;
#define JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_JSCLASSDEFINITION_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
  inline
  void swap(JSClassDefinition& first, JSClassDefinition& second) noexcept {
    first.swap(second);
  }
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASSDEFINITION_HPP_

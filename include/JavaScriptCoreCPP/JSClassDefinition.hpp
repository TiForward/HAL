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
    
    JSClassDefinition() JAVASCRIPTCORECPP_NOEXCEPT;
    
    explicit JSClassDefinition(const ::JSClassDefinition& js_class_definition);
    
    /*!
     @method
     
     @abstract Return the name of this JSClassDefinition.
     
     @result The name of this JSClassDefinition.
     */
    virtual std::string get_name() const JAVASCRIPTCORECPP_NOEXCEPT final;
    
    /*!
     @method
     
     @abstract Return the version number of this JSClassDefinition.
     
     @result The version number of this JSClassDefinition.
     */
    virtual std::uint32_t get_version() const JAVASCRIPTCORECPP_NOEXCEPT final;
    
    virtual ~JSClassDefinition()                           JAVASCRIPTCORECPP_NOEXCEPT;
    JSClassDefinition(const JSClassDefinition&)            JAVASCRIPTCORECPP_NOEXCEPT;
    JSClassDefinition(JSClassDefinition&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
    JSClassDefinition& operator=(const JSClassDefinition&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSClassDefinition& operator=(JSClassDefinition&&)      JAVASCRIPTCORECPP_NOEXCEPT;
    void swap(JSClassDefinition&)                          JAVASCRIPTCORECPP_NOEXCEPT;
    
    virtual void Print() JAVASCRIPTCORECPP_NOEXCEPT final;
    static  void Print(const ::JSClassDefinition& js_class_definition) JAVASCRIPTCORECPP_NOEXCEPT;
    
  private:
    
    virtual void Initialize(const ::JSClassDefinition& other) JAVASCRIPTCORECPP_NOEXCEPT final;
    virtual void InitializePropertyCallbacks() JAVASCRIPTCORECPP_NOEXCEPT final;
    std::vector<detail::JSStaticValue>    js_value_properties__;
    std::vector<detail::JSStaticFunction> js_function_properties__;
    
  protected:
    
    // JSClass and JSExportClass need access to js_class_definition__.
    friend class JSClass;
    
    template<typename T>
    friend class detail::JSExportClass;
    
    std::string                           name__;
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
  void swap(JSClassDefinition& first, JSClassDefinition& second) JAVASCRIPTCORECPP_NOEXCEPT {
    first.swap(second);
  }
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASSDEFINITION_HPP_

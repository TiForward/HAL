/**
 * Javascriptcorecpp
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASS_HPP_
#define _JAVASCRIPTCORECPP_JSCLASS_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSClassDefinition.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>


namespace JavaScriptCoreCPP { namespace detail {
  template<typename T>
  class JSExportClassDefinition;
  
  template<typename T>
  class JSExportClassDefinitionBuilder;
}}

namespace JavaScriptCoreCPP {
  
  /*!
   @class
   
   @discussion A JSClass is an RAII wrapper around a JSClassRef, the
   JavaScriptCore C API representation of a JavaScript class that
   defines JavaScript objects implemented in C.
   
   A JSClass wraps a C++ class and seamlessly integrates it into the
   JavaScriptCore runtime.  The only way to create a JSClass is
   through the use of a JSClassBuilder, so please see that class for
   more details.
   
   An instance of JSClass may be passed to the JSContextGroup
   constructor to create a custom JavaScript global object for all
   contexts in that group.
   */
  class JSClass JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSClass) {
  public:
    
    /*!
     @method
     
     @abstract Return an empty JSClass.
     
     @result An empty JSClass.
     */
    JSClass() JAVASCRIPTCORECPP_NOEXCEPT;
    
    /*!
     @method
     
     @abstract Return a JSClass defined by the given JSClassDefinition.
     
     @result A JSClass defined by the given JSClassDefinition.
     */
    JSClass(const JSClassDefinition& js_class_definition) JAVASCRIPTCORECPP_NOEXCEPT;
    
    /*!
     @method
     
     @abstract Return the name of this JSClass.
     
     @result The name of this JSClass.
     */
    virtual std::string get_name() const JAVASCRIPTCORECPP_NOEXCEPT final {
      return name__;
    }
    
    virtual ~JSClass() JAVASCRIPTCORECPP_NOEXCEPT;
    JSClass(const JSClass&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSClass(JSClass&&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSClass& operator=(const JSClass&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSClass& operator=(JSClass&&) JAVASCRIPTCORECPP_NOEXCEPT;
    void swap(JSClass&) JAVASCRIPTCORECPP_NOEXCEPT;
    
  private:
    
    // These five classes need access to operator JSClassRef().
    friend class JSContext; // for constructor
    friend class JSValue;   // for IsObjectOfClass
    friend class JSObject;  // for constructor
    
    // For setting JSClassDefinition.parentClass
    template<typename T>
    friend class detail::JSExportClassDefinition;
    
    // For setting JSClassDefinition.parentClass
    template<typename T>
    friend class detail::JSExportClassDefinitionBuilder;
    
    operator JSClassRef() const JAVASCRIPTCORECPP_NOEXCEPT {
      return js_class_ref__;
    }
    
    std::string name__ { "Default" };
    JSClassRef  js_class_ref__ { nullptr };
    
  protected:
    
#undef  JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
    std::recursive_mutex mutex__;
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
  inline
  void swap(JSClass& first, JSClass& second) JAVASCRIPTCORECPP_NOEXCEPT {
    first.swap(second);
  }
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASS_HPP_

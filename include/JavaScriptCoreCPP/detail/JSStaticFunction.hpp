/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSSTATICFUNCTION_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSSTATICFUNCTION_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/detail/JSPropertyCallback.hpp"

namespace JavaScriptCoreCPP { namespace detail {
  
  /*!
   @class
   
   @discussion A JSStaticFunction is an RAII wrapper around the
   JavaScriptCore C API JSStaticFunction, which describes a function
   property of a JavaScript object.
   */
  
  class JAVASCRIPTCORECPP_EXPORT JSStaticFunction final : public JSPropertyCallback JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSStaticFunction) {
      
    public:
      
      JSStaticFunction(const ::JSStaticFunction& js_static_function);
      
      JSObjectCallAsFunctionCallback function_callback() const JAVASCRIPTCORECPP_NOEXCEPT {
        return function_callback__;
      }
      
      ~JSStaticFunction() = default;
      JSStaticFunction(const JSStaticFunction&)            JAVASCRIPTCORECPP_NOEXCEPT;
      JSStaticFunction(JSStaticFunction&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
      JSStaticFunction& operator=(const JSStaticFunction&) JAVASCRIPTCORECPP_NOEXCEPT;
      JSStaticFunction& operator=(JSStaticFunction&&)      JAVASCRIPTCORECPP_NOEXCEPT;
      void swap(JSStaticFunction&)                         JAVASCRIPTCORECPP_NOEXCEPT;
      
    private:
      
      friend bool operator==(const JSStaticFunction& lhs, const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT;
      
      JSObjectCallAsFunctionCallback function_callback__ { nullptr };
    };
    
    
    inline
    void swap(JSStaticFunction& first, JSStaticFunction& second) JAVASCRIPTCORECPP_NOEXCEPT {
      first.swap(second);
    }
    
    // Return true if the two JSStaticFunctions are equal.
    bool operator==(const JSStaticFunction& lhs, const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT;
    
    // Return true if the two JSStaticFunctions are not equal.
    inline
    bool operator!=(const JSStaticFunction& lhs, const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
      return ! (lhs == rhs);
    }
    
  }} // namespace JavaScriptCoreCPP { namespace detail

#endif // _JAVASCRIPTCORECPP_DETAIL_JSSTATICFUNCTION_HPP_

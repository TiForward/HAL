/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSSTATICVALUE_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSSTATICVALUE_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/detail/JSPropertyCallback.hpp"

namespace JavaScriptCoreCPP { namespace detail {
  
  /*!
   @class
   
   @discussion A JSStaticValue is an RAII wrapper around the
   JavaScriptCore C API JSStaticValue, which describes a value property
   of a JavaScript object.
   */
  class JSStaticValue final : public JSPropertyCallback JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSStaticValue) {
    
  public:
    
    JSStaticValue(const ::JSStaticValue& js_static_value);
    
    JSObjectGetPropertyCallback get_callback() const JAVASCRIPTCORECPP_NOEXCEPT {
      return get_callback__;
    }
    
    JSObjectSetPropertyCallback set_callback() const JAVASCRIPTCORECPP_NOEXCEPT {
      return set_callback__;
    }
    
    ~JSStaticValue()                               = default;
    JSStaticValue(const JSStaticValue&)            JAVASCRIPTCORECPP_NOEXCEPT;
    JSStaticValue(JSStaticValue&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
    JSStaticValue& operator=(const JSStaticValue&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSStaticValue& operator=(JSStaticValue&&)      JAVASCRIPTCORECPP_NOEXCEPT;
    void swap(JSStaticValue&)                      JAVASCRIPTCORECPP_NOEXCEPT;
    
  private:
    
    friend bool operator==(const JSStaticValue& lhs, const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT;
    
    JSObjectGetPropertyCallback get_callback__ { nullptr };
    JSObjectSetPropertyCallback set_callback__ { nullptr };
  };
  
  inline
  void swap(JSStaticValue& first, JSStaticValue& second) JAVASCRIPTCORECPP_NOEXCEPT {
    first.swap(second);
  }
  
  // Return true if the two JSStaticValues are equal.
  bool operator==(const JSStaticValue& lhs, const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT;
  
  // Return true if the two JSStaticValue are not equal.
  inline
  bool operator!=(const JSStaticValue& lhs, const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    return ! (lhs == rhs);
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSSTATICVALUE_HPP_

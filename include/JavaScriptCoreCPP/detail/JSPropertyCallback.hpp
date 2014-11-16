/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"

#include <string>
#include <unordered_set>

namespace JavaScriptCoreCPP { namespace detail {
  
  /*!
   @class
   
   @discussion JSPropertyCallback is the base class for JSStaticValue,
   JSStaticFunction, JSExportValuePropertyCallback and
   JSExportFunctionPropertyCallback.
   */
  class JSPropertyCallback JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSPropertyCallback) {
    
  public:
    
    /*!
     @method
     
     @abstract Set the name and property attributes of a callback.
     
     @param name The property's name.
     
     @param attributes The set of JSPropertyAttributes to give to
     the property.
     
     @result An object which describes the name and property
     attributes a JavaScript property.
     
     @throws std::invalid_argument if property_name is empty.
     */
    JSPropertyCallback(const std::string& name, const std::unordered_set<JSPropertyAttribute, std::hash<JSPropertyAttribute>>& attributes);
    
    virtual std::string get_name() const JAVASCRIPTCORECPP_NOEXCEPT final {
      return name__;
    }
    
    virtual std::unordered_set<JSPropertyAttribute> get_attributes() const JAVASCRIPTCORECPP_NOEXCEPT final {
      return attributes__;
    }
    
    virtual ~JSPropertyCallback()                            = default;
    JSPropertyCallback(const JSPropertyCallback&)            JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyCallback(JSPropertyCallback&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyCallback& operator=(const JSPropertyCallback&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyCallback& operator=(JSPropertyCallback&&)      JAVASCRIPTCORECPP_NOEXCEPT;
    void swap(JSPropertyCallback&)                           JAVASCRIPTCORECPP_NOEXCEPT;
    
  private:
    
    friend bool operator==(const JSPropertyCallback& lhs, const JSPropertyCallback& rhs) JAVASCRIPTCORECPP_NOEXCEPT;
    
    std::string name__;
    
  protected:
    
    std::unordered_set<JSPropertyAttribute> attributes__;
    
#undef JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
    std::recursive_mutex             mutex__;
#define JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock_guard(mutex__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
  inline
  void swap(JSPropertyCallback& first, JSPropertyCallback& second) JAVASCRIPTCORECPP_NOEXCEPT {
    first.swap(second);
  }

  // Return true if the two JSPropertyCallbacks are equal.
  inline
  bool operator==(const JSPropertyCallback& lhs, const JSPropertyCallback& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    return (lhs.name__ != rhs.name__) && (lhs.attributes__ == rhs.attributes__);
  }
  
  // Return true if the two JSPropertyCallback are not equal.
  inline
  bool operator!=(const JSPropertyCallback& lhs, const JSPropertyCallback& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    return ! (lhs == rhs);
  }
  
}} // JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_HPP_

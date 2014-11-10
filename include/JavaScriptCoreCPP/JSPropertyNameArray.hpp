/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYNAMEARRAY_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYNAMEARRAY_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"

#include <vector>

namespace JavaScriptCoreCPP {
  
  class JSString;
  class JSObject;
  
  /*!
   @class
   
   @discussion A JSPropertyNameArray is an RAII wrapper around a
   JSPropertyNameArrayRef, the JavaScriptCore C API representation of
   an array of JavaScript property names.
   */
  class JSPropertyNameArray final JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSPropertyNameArray) {
    
  public:
    
    /*!
     @method
     
     @abstract Return the number of items in this JavaScript property
     name array.
     
     @result The number of names in this JavaScript property name
     array.
     */
    std::size_t GetCount() const JAVASCRIPTCORECPP_NOEXCEPT;
    
    /*!
     @method
     
     @abstract Gets a property name at a given index in this
     JavaScript property name array.
     
     @param index The index of the property name to retrieve.
     
     @result A JSString containing the property name.
     */
    JSString GetNameAtIndex(std::size_t index) const JAVASCRIPTCORECPP_NOEXCEPT;
    
    /*!
     @method
     
     @abstract Convert this JSPropertyNameArray into a
     std::vector<JSString>.
     
     @result A std::vector<JSString> containing the property names in
     this JSPropertyNameArray.
     */
    operator std::vector<JSString>() const JAVASCRIPTCORECPP_NOEXCEPT;
    
    JSPropertyNameArray()                                      = delete;;
    ~JSPropertyNameArray()                                     JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyNameArray(const JSPropertyNameArray&)            JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyNameArray(JSPropertyNameArray&&)                 JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyNameArray& operator=(const JSPropertyNameArray&) JAVASCRIPTCORECPP_NOEXCEPT;
    JSPropertyNameArray& operator=(JSPropertyNameArray&&)      JAVASCRIPTCORECPP_NOEXCEPT;
    void swap(JSPropertyNameArray&)                            JAVASCRIPTCORECPP_NOEXCEPT;
    
  private:
    
    // Only a JSObject can create a JSPropertyNameArray.
    friend class JSObject;
    
    /*!
     @method
     
     @abstract Gets the names of an object's enumerable properties.
     
     @param object The object whose property names you want to get.
     
     @result A JSPropertyNameArray containing the names of the
     object's enumerable properties.
     */
    explicit JSPropertyNameArray(const JSObject& js_object) JAVASCRIPTCORECPP_NOEXCEPT;
    
    // For interoperability with the JavaScriptCore C API.
    operator JSPropertyNameArrayRef() const JAVASCRIPTCORECPP_NOEXCEPT {
      return js_property_name_array_ref__;
    }
    
    // Prevent heap based objects.
    static void * operator new(std::size_t);       // #1: To prevent allocation of scalar objects
    static void * operator new [] (std::size_t);   // #2: To prevent allocation of array of objects
    
    JSPropertyNameArrayRef js_property_name_array_ref__;
    
#undef  JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
    std::recursive_mutex mutex__;
#define JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
  inline
  void swap(JSPropertyNameArray& first, JSPropertyNameArray& second) JAVASCRIPTCORECPP_NOEXCEPT {
    first.swap(second);
  }
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSPROPERTYNAMEARRAY_HPP_

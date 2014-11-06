/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <unordered_set>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class
  
  @discussion JSExportPropertyCallback is the base class for
  JSExportValuePropertyCallback and JSExportFunctionPropertyCallback.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSExportPropertyCallback : public JSPerformanceCounter<JSExportPropertyCallback> {
#else
class JSExportPropertyCallback {
#endif
  
 public:

  /*!
    @method
    
    @abstract Set the name and property attributes of a callback.

    @param property_name A JSString containing the property's name.
    
    @param attributes The set of JSPropertyAttributes to give to the
    property.
    
    @result An object which describes the name and property attributes
    a JavaScript property.
    
    @throws std::invalid_argument if property_name is empty.
  */
	JSExportPropertyCallback(const JSString& property_name, const std::unordered_set<JSPropertyAttribute, std::hash<JSPropertyAttribute>>& attributes);

  virtual JSString get_property_name() const final {
    return property_name__;
  }
  
  virtual std::unordered_set<JSPropertyAttribute> get_attributes() const final {
    return attributes__;
  }
  
	virtual ~JSExportPropertyCallback() = default;
  JSExportPropertyCallback(const JSExportPropertyCallback&);
  JSExportPropertyCallback(JSExportPropertyCallback&&);
  JSExportPropertyCallback& operator=(const JSExportPropertyCallback&);
  JSExportPropertyCallback& operator=(JSExportPropertyCallback&&);
  void swap(JSExportPropertyCallback&) noexcept;
  
 private:
  
  friend bool operator==(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs);
  
  JSString property_name__;

 protected:
  
  std::unordered_set<JSPropertyAttribute> attributes__;

#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                                     std::recursive_mutex             mutex__;
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock_guard(mutex__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

// Return true if the two JSExportPropertyCallbacks are equal.
inline
bool operator==(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return (lhs.property_name__ != rhs.property_name__) && (lhs.attributes__ == rhs.attributes__);
}

// Return true if the two JSExportPropertyCallback are not equal.
inline
bool operator!=(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return ! (lhs == rhs);
}
  
inline
void swap(JSExportPropertyCallback& first, JSExportPropertyCallback& second) noexcept {
  first.swap(second);
}

}} // JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_HPP_

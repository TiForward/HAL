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
  JSExportPropertyCallback(const JSString& property_name, const std::unordered_set<JSPropertyAttribute>& attributes);

  virtual JSString get_property_name() const final {
    return property_name__;
  }
  
  virtual std::unordered_set<JSPropertyAttribute> get_attributes() const final {
    return attributes__;
  }
  
  virtual ~JSExportPropertyCallback();
  JSExportPropertyCallback(const JSExportPropertyCallback& rhs);
  JSExportPropertyCallback(JSExportPropertyCallback&& rhs);
  JSExportPropertyCallback& JSExportPropertyCallback::operator=(const JSExportPropertyCallback&) = delete;
  JSExportPropertyCallback& JSExportPropertyCallback::operator=(JSExportPropertyCallback&&) = delete;
  JSExportPropertyCallback& operator=(JSExportPropertyCallback rhs);
  void swap(JSExportFunctionPropertyCallback& other) noexcept;
  
 private:
  
  friend bool operator==(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs);
  friend bool operator<(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs);
  
 protected:
  
  const JSString                          property_name__;
  std::unordered_set<JSPropertyAttribute> attributes__;

#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                                           std::recursive_mutex             mutex__
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock_guard(mutex__)
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
  
// Define a strict weak ordering for two JSExportPropertyCallbacks.
inline
bool operator<(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return (lhs.property_name__ < rhs.property_name__) && (lhs.attributes__ < rhs.attributes__);
}

inline
bool operator>(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return rhs < lhs;
}

inline
bool operator<=(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return ! (lhs > rhs);
}

inline
bool operator>=(const JSExportPropertyCallback& lhs, const JSExportPropertyCallback& rhs) {
  return ! (lhs < rhs);
}

}} // JavaScriptCoreCPP { namespace detail {

namespace std {
using JavaScriptCoreCPP::JSExportPropertyCallback;
template<>
void swap<JSExportPropertyCallback>(JSExportPropertyCallback& first, JSExportPropertyCallback& second) noexcept {
  first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_HPP_

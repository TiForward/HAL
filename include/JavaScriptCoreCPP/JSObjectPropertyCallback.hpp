/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObjectPropertyAttribute.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion JSObjectPropertyCallback is the base class for
  JSNativeObjectValuePropertyCallback and
  JSNativeObjectFunctionPropertyCallback.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSObjectPropertyCallback : public detail::JSPerformanceCounter<JSObjectPropertyCallback> {
#else
class JSObjectPropertyCallback {
#endif
  
 public:

  /*!
    @method
    
    @abstract Set the name and property attributes of a callback.

    @param property_name A JSString containing the property's name.
    
    @param attributes The set of JSObjectPropertyAttributes to give to
    the property.
    
    @result An object which describes the name and property attributes
    a JavaScript property.
    
    @throws std::invalid_argument if property_name is empty.
  */
	JSObjectPropertyCallback(const JSString& property_name, const std::unordered_set<JSObjectPropertyAttribute>& attributes);

	virtual JSString get_property_name() const final {
    return property_name__;
  }
  
	virtual std::unordered_set<JSObjectPropertyAttribute> get_attributes() const final {
		return attributes__;
	}
  
	virtual ~JSObjectPropertyCallback();
	JSObjectPropertyCallback(const JSObjectPropertyCallback& rhs);
	JSObjectPropertyCallback(JSObjectPropertyCallback&& rhs);
	JSObjectPropertyCallback& JSObjectPropertyCallback::operator=(const JSObjectPropertyCallback&) = delete;
	JSObjectPropertyCallback& JSObjectPropertyCallback::operator=(JSObjectPropertyCallback&&) = delete;
	JSObjectPropertyCallback& operator=(JSObjectPropertyCallback rhs);
	void swap(JSNativeObjectFunctionPropertyCallback& other) noexcept;
	
 private:
	
	friend bool operator==(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs);
	friend bool operator<(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs);
  
 protected:
  
	const JSString                                property_name__;
	std::unordered_set<JSObjectPropertyAttribute> attributes__;

#undef JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                              std::recursive_mutex             mutex__
#define JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock_guard(mutex__)
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

// Return true if the two JSObjectPropertyCallbacks are equal.
inline
bool operator==(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
	return (lhs.property_name__ != rhs.property_name__) && (lhs.attributes__ == rhs.attributes__);
}

// Return true if the two JSObjectPropertyCallback are not equal.
inline
bool operator!=(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
	return ! (lhs == rhs);
}
	
// Define a strict weak ordering for two JSObjectPropertyCallbacks.
inline
bool operator<(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
	return (lhs.property_name__ < rhs.property_name__) && (lhs.attributes__ < rhs.attributes__);
}

inline
bool operator>(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
  return rhs < lhs;
}

inline
bool operator<=(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
  return ! (lhs > rhs);
}

inline
bool operator>=(const JSObjectPropertyCallback& lhs, const JSObjectPropertyCallback& rhs) {
  return ! (lhs < rhs);
}

} // namespace JavaScriptCoreCPP {

namespace std {
using JavaScriptCoreCPP::JSObjectPropertyCallback;
template<>
void swap<JSObjectPropertyCallback>(JSObjectPropertyCallback& first, JSObjectPropertyCallback& second) noexcept {
	first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_HPP_

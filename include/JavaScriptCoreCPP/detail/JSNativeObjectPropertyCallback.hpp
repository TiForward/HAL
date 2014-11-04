/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class
  
  @discussion JSNativeObjectPropertyCallback is the base class for
  JSNativeObjectValuePropertyCallback and
  JSNativeObjectFunctionPropertyCallback.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeObjectPropertyCallback : public detail::JSPerformanceCounter<JSNativeObjectPropertyCallback> {
#else
class JSNativeObjectPropertyCallback {
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
	JSNativeObjectPropertyCallback(const JSString& property_name, const std::unordered_set<JSPropertyAttribute>& attributes)
			: property_name__(property_name)
			, attributes__(attributes) {
		
		if (property_name__.empty()) {
			ThrowInvalidArgument("JSNativeObjectPropertyCallback", "property_name is missing");
		}
	}

	virtual JSString get_property_name() const final {
    return property_name__;
  }
  
	virtual std::unordered_set<JSPropertyAttribute> get_attributes() const final {
		return attributes__;
	}
  
	virtual ~JSNativeObjectPropertyCallback() {
  }
  
  JSNativeObjectPropertyCallback(const JSNativeObjectPropertyCallback& rhs)
      : property_name__(rhs.property_name__)
      , attributes__(rhs.attributes__) {
  }
  
	JSNativeObjectPropertyCallback(JSNativeObjectPropertyCallback&& rhs)
		  : property_name__(std::move(rhs.property_name__))
		  , attributes__(std::move(rhs.attributes__)) {
  }
  
	JSNativeObjectPropertyCallback& JSNativeObjectPropertyCallback::operator=(const JSNativeObjectPropertyCallback&) = delete;
	JSNativeObjectPropertyCallback& JSNativeObjectPropertyCallback::operator=(JSNativeObjectPropertyCallback&&) = delete;

	// Create a copy of another JSNativeObjectFunctionPropertyCallback
  // by assignment. This is a unified assignment operator that fuses
  // the copy assignment operator
  //
  // X& X::operator=(const X&)
  //
  // and the move assignment operator
  //
  // X& X::operator=(X&&);
  JSNativeObjectPropertyCallback& operator=(JSNativeObjectPropertyCallback rhs) {
    JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
    swap(rhs);
    return *this;
  }

  void swap(JSNativeObjectFunctionPropertyCallback& other) noexcept {
	  JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(property_name__, other.property_name__);
    swap(attributes__   , other.attributes__);
  }

  friend void swap(JSNativeObjectPropertyCallback& first, JSNativeObjectPropertyCallback& second) noexcept {
    first.swap(second);
  }

 private:
  
  friend bool operator==(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs);
  friend bool operator<(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs);
  
	JSString property_name__;

 protected:
  
	std::unordered_set<JSPropertyAttribute> attributes__;

#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                                           std::recursive_mutex             mutex__
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock_guard(mutex__)
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

// Return true if the two JSNativeObjectPropertyCallbacks are equal.
inline
bool operator==(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
	if (lhs.property_name__ != rhs.property_name__) {
		return false;
	}
	
	return lhs.attributes__ == rhs.attributes__;
}

// Return true if the two JSNativeObjectPropertyCallback are not
// equal.
inline
bool operator!=(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
	return ! (lhs == rhs);
}
	
// Define a strict weak ordering for two
// JSNativeObjectPropertyCallbacks.
inline
bool operator<(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
  if (lhs.property_name__ < rhs.property_name__) {
    return true;
  }
  
  return lhs.attributes__ < rhs.attributes__;
}

inline
bool operator>(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
  return rhs < lhs;
}

inline
bool operator<=(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
  return ! (lhs > rhs);
}

inline
bool operator>=(const JSNativeObjectPropertyCallback& lhs, const JSNativeObjectPropertyCallback& rhs) {
  return ! (lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_HPP_

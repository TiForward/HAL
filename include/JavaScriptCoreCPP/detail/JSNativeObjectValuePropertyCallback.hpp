/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectPropertyCallback.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class
  
  @discussion A JSNativeObjectValuePropertyCallback is an RAII wrapper
  around the JavaScriptCore C API JSStaticValue, which describes a
  value property of a JavaScript object.
*/
template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeObjectValuePropertyCallback final : public JSNativeObjectPropertyCallback, public detail::JSPerformanceCounter<JSNativeObjectValuePropertyCallback<T>> {
#else
class JSNativeObjectValuePropertyCallback final : public JSNativeObjectPropertyCallback {
#endif
	
 public:

	/*!
	  @method

	  @abstract Set the callbacks to invoke when getting and setting a
	  property value on a JavaScript object.

	  @param property_name A JSString containing the property's name.
	  
	  @param get_property_callback The callback to invoke when getting a
	  property's value from a JavaScript object.
	  
	  @param set_property_callback The callback to invoke when setting a
	  property's value on a JavaScript object. This may be nullptr, in
	  which case the ReadOnly attribute is automatically set.
	  
	  @param attributes The set of JSPropertyAttributes to give to the
	  value property.
	  
	  @result An object which describes a JavaScript value property.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If property_name is empty or otherwise has a JavaScript syntax
	  error.
	  
	  2. If the ReadOnly attribute is set and the get_property_callback
    is not provided.
	                               
    3. If the ReadOnly attribute is set and the set_property_callback
    is provided.

    4. If both get_property_callback and set_property_callback are
	  missing.
	*/
	JSNativeObjectValuePropertyCallback(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback, const std::unordered_set<JSPropertyAttribute>& attributes);

	GetNamedPropertyCallback<T> get_get_property_callback() const {
		return get_property_callback__;
	}

	SetNamedPropertyCallback<T> get_set_property_callback() const {
		return set_property_callback__;
	}

	~JSNativeObjectValuePropertyCallback() {
	}
	
	JSNativeObjectValuePropertyCallback(const JSNativeObjectValuePropertyCallback& rhs)
			: JSNativeObjectPropertyCallback(rhs)
			, get_property_callback__(rhs.get_property_callback__)
			, set_property_callback__(rhs.set_property_callback__) {
	}
	
	JSNativeObjectValuePropertyCallback(JSNativeObjectValuePropertyCallback&& rhs)
			: JSNativeObjectPropertyCallback(rhs)
			, get_property_callback__(std::move(rhs.get_property_callback__))
			, set_property_callback__(std::move(rhs.set_property_callback__)) {
	}
	
	JSNativeObjectValuePropertyCallback& JSNativeObjectValuePropertyCallback::operator=(const JSNativeObjectValuePropertyCallback&) = delete;
	JSNativeObjectValuePropertyCallback& JSNativeObjectValuePropertyCallback::operator=(JSNativeObjectValuePropertyCallback&&) = delete;

	// Create a copy of another JSNativeObjectFunctionPropertyCallback
  // by assignment. This is a unified assignment operator that fuses
  // the copy assignment operator
  //
  // X& X::operator=(const X&)
  //
  // and the move assignment operator
  //
  // X& X::operator=(X&&);
  JSNativeObjectValuePropertyCallback& operator=(JSNativeObjectValuePropertyCallback rhs) {
    JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
    JSNativeObjectPropertyCallback::operator=(rhs);
    swap(rhs);
    return *this;
  }

  void swap(JSNativeObjectFunctionPropertyCallback& other) noexcept {
	  JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_LOCK_GUARD;
	  using std::swap;
	  
	  // By swapping the members of two classes, the two classes are
	  // effectively swapped.
    swap(get_property_callback__, other.get_property_callback__);
    swap(set_property_callback__, other.set_property_callback__);
  }

  friend void swap(JSNativeObjectValuePropertyCallback& first, JSNativeObjectValuePropertyCallback& second) noexcept {
	  first.swap(second);
  }

private:
	
	template<typename U>
	friend bool operator==(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	template<typename U>
	friend bool operator<(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	GetNamedPropertyCallback<T> get_property_callback__ { nullptr };
	SetNamedPropertyCallback<T> set_property_callback__ { nullptr };
};

template<typename T>
JSNativeObjectValuePropertyCallback<T>::JSNativeObjectValuePropertyCallback(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback, const std::unordered_set<JSPropertyAttribute>& attributes)
		: JSNativeObjectPropertyCallback(property_name, attributes)
		, get_property_callback__(get_property_callback)
		, set_property_callback__(set_property_callback) {
	
	if (!get_property_callback && !set_property_callback) {
		ThrowInvalidArgument("JSNativeObjectValuePropertyCallback", "Both get_property_callback and set_property_callback are missing. At least one callback must be provided");
	}
	
	if (attributes.find(JSPropertyAttribute::ReadOnly) != attributes.end()) {
		if (!get_property_callback) {
			ThrowInvalidArgument("JSNativeObjectValuePropertyCallback", "ReadOnly attribute is set but get_property_callback is missing");
		}
		
		if (set_property_callback) {
			ThrowInvalidArgument("JSNativeObjectValuePropertyCallback", "ReadOnly attribute is set but set_property_callback is provided");
		}
	}
	
	// Force the ReadOnly attribute if only a getter is provided.
	if (get_property_callback && !set_property_callback) {
		attributes__.insert(JSPropertyAttribute::ReadOnly);
	}
}

// Return true if the two JSNativeObjectValuePropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	// get_property_callback__
	if (lhs.get_property_callback__ && !rhs.get_property_callback__) {
		return false;
	}
	
	if (!lhs.get_property_callback__ && rhs.get_property_callback__) {
		return false;
	}
	
	// set_property_callback__
	if (lhs.set_property_callback__ && !rhs.set_property_callback__) {
		return false;
	}
	
	if (!lhs.set_property_callback__ && rhs.set_property_callback__) {
		return false;
	}
	
	return static_cast<JSNativeObjectPropertyCallback>(lhs) == static_cast<JSNativeObjectPropertyCallback>(rhs);
}

// Return true if the two JSNativeObjectValuePropertyCallback are not
// equal.
template<typename T>
bool operator!=(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two
// JSNativeObjectValuePropertyCallbacks.
template<typename T>
bool operator<(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	return static_cast<JSNativeObjectPropertyCallback>(lhs) < static_cast<JSNativeObjectPropertyCallback>(rhs);
}

template<typename T>
bool operator>(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	return rhs < lhs;
}

template<typename T>
bool operator<=(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	return ! (lhs > rhs);
}

template<typename T>
bool operator>=(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	return ! (lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

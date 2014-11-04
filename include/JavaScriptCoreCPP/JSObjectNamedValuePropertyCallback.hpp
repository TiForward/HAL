/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSOBJECTNAMEDVALUEPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_JSOBJECTNAMEDVALUEPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSObjectPropertyCallback.hpp"
#include "JavaScriptCoreCPP/JSObjectCallbacks.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP

/*!
  @class
  
  @discussion A JSObjectNamedValuePropertyCallback is an RAII wrapper
  around the JavaScriptCore C API JSStaticValue, which describes a
  value property of a JavaScript object.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSObjectNamedValuePropertyCallback final : public JSObjectPropertyCallback, public detail::JSPerformanceCounter<JSObjectNamedValuePropertyCallback> {
#else
class JSObjectNamedValuePropertyCallback final : public JSObjectPropertyCallback {
#endif
	
 public:

	/*!
	  @method

	  @abstract Set the callbacks to invoke when getting and setting a
	  property value on a JavaScript object.

	  @param property_name A JSString containing the property's name.
	  
	  @param get_callback The callback to invoke when getting a
	  property's value from a JavaScript object.
	  
	  @param set_callback The callback to invoke when setting a
	  property's value on a JavaScript object. This may be nullptr, in
	  which case the ReadOnly attribute is automatically set.
	  
	  @param attributes The set of JSObjectPropertyAttributes to give to
	  the value property.
	  
	  @result An object which describes a JavaScript value property.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If property_name is empty.
	  
	  2. If the ReadOnly attribute is set and the get_callback is not
    provided.
	                               
    3. If the ReadOnly attribute is set and the set_callback is
    provided.

    4. If both get_callback and set_callback are missing.
	*/
	JSObjectNamedValuePropertyCallback(const JSString& property_name,
	                                   GetNamedValuePropertyCallback get_callback,
	                                   SetNamedValuePropertyCallback set_callback,
	                                   const std::unordered_set<JSObjectPropertyAttribute>& attributes);

	GetNamedValuePropertyCallback get_callback() const {
		return get_callback__;
	}

	SetNamedValuePropertyCallback set_callback() const {
		return set_callback__;
	}

	~JSObjectNamedValuePropertyCallback();
	
	JSObjectNamedValuePropertyCallback(const JSObjectNamedValuePropertyCallback& rhs);
	JSObjectNamedValuePropertyCallback(JSObjectNamedValuePropertyCallback&& rhs);
	JSObjectNamedValuePropertyCallback& JSObjectNamedValuePropertyCallback::operator=(const JSObjectNamedValuePropertyCallback&) = delete;
	JSObjectNamedValuePropertyCallback& JSObjectNamedValuePropertyCallback::operator=(JSObjectNamedValuePropertyCallback&&) = delete;
	JSObjectNamedValuePropertyCallback& operator=(JSObjectNamedValuePropertyCallback rhs);
	void swap(JSObjectFunctionPropertyCallback& other) noexcept;

private:
	
	friend bool operator==(const JSObjectNamedValuePropertyCallback<U>& lhs, const JSObjectNamedValuePropertyCallback<U>& rhs);
	friend bool operator<(const JSObjectNamedValuePropertyCallback<U>& lhs, const JSObjectNamedValuePropertyCallback<U>& rhs);

	GetNamedValuePropertyCallback get_callback__ { nullptr };
	SetNamedValuePropertyCallback set_callback__ { nullptr };
};

// Return true if the two JSObjectNamedValuePropertyCallbacks are equal.
bool operator==(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs);

// Return true if the two JSObjectNamedValuePropertyCallback are not equal.
inline
bool operator!=(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two
// JSObjectNamedValuePropertyCallbacks.
inline
bool operator<(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	return static_cast<JSObjectPropertyCallback>(lhs) < static_cast<JSObjectPropertyCallback>(rhs);
}

inline
bool operator>(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	return rhs < lhs;
}

inline
bool operator<=(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	return ! (lhs > rhs);
}

inline
bool operator>=(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	return ! (lhs < rhs);
}

} // namespace JavaScriptCoreCPP {

namespace std {
using JavaScriptCoreCPP::JSObjectNamedValuePropertyCallback;
template<>
void swap<JSObjectNamedValuePropertyCallback>(JSObjectNamedValuePropertyCallback& first, JSObjectNamedValuePropertyCallback& second) noexcept {
	first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_JSOBJECTNAMEDVALUEPROPERTYCALLBACK_HPP_

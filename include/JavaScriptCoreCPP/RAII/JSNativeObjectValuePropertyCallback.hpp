/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  
  @discussion A JSNativeObjectValuePropertyCallback is an RAII wrapper
  around the JavaScriptCore C API JSStaticValue, which describes a
  value property of a JavaScript object.
*/
template<typename T>
class JSNativeObjectValuePropertyCallback final	{
	
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
	  
	  @param attributes A set of JSPropertyAttributes to give to the
	  property.
	  
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
	JSNativeObjectValuePropertyCallback(const JSString& property_name, GetPropertyCallback<T> get_property_callback, SetPropertyCallback<T> set_property_callback, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());

	JSString get_property_name() const {
		return property_name_;
	}
	
	GetPropertyCallback<T> get_get_property_callback() const {
		return get_property_callback_;
	}

	SetPropertyCallback<T> get_set_property_callback() const {
		return set_property_callback_;
	}

	std::set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

	~JSNativeObjectValuePropertyCallback() {
  }

	// Copy constructor.
	JSNativeObjectValuePropertyCallback(const JSNativeObjectValuePropertyCallback& rhs);
	
	// Move constructor.
	JSNativeObjectValuePropertyCallback(JSNativeObjectValuePropertyCallback&& rhs);
  
	// Create a copy of another JSNativeObjectValuePropertyCallback by assignment. This is a
	// unified assignment operator that fuses the copy assignment
	// operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSNativeObjectValuePropertyCallback& operator=(JSNativeObjectValuePropertyCallback rhs) {
		swap(*this, rhs);
		return *this;
  }
  
	friend void swap(JSNativeObjectValuePropertyCallback& first, JSNativeObjectValuePropertyCallback& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
    // by swapping the members of two classes,
		// the two classes are effectively swapped
    swap(first.property_name_                    , second.property_name_);
    swap(first.property_name_for_js_static_value_, second.property_name_for_js_static_value_);
    swap(first.get_property_callback_            , second.get_property_callback_);
    swap(first.set_property_callback_            , second.set_property_callback_);
    swap(first.attributes_                       , second.attributes_);
	}

private:
	
	// Return true if the two JSNativeObjectValuePropertyCallbacks are equal.
	template<typename U>
	friend bool operator==(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	// Define a strict weak ordering for two
	// JSNativeObjectValuePropertyCallbacks.
	template<typename U>
	friend bool operator<(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	JSString                      property_name_;
	std::string                   property_name_for_js_static_value_;
	GetPropertyCallback<T>        get_property_callback_ { nullptr };
	SetPropertyCallback<T>        set_property_callback_ { nullptr };
	std::set<JSPropertyAttribute> attributes_;
};

template<typename T>
JSNativeObjectValuePropertyCallback<T>::JSNativeObjectValuePropertyCallback(const JSString& property_name, GetPropertyCallback<T> get_property_callback, SetPropertyCallback<T> set_property_callback, const std::set<JSPropertyAttribute> attributes)
		: property_name_(property_name)
		, property_name_for_js_static_value_(property_name)
		, get_property_callback_(get_property_callback)
		, set_property_callback_(set_property_callback)
		, attributes_(attributes) {
	
	static const std::string log_prefix { "MDL: JSNativeObjectValuePropertyCallback: " };
	
	if (property_name_for_js_static_value_.empty()) {
		std::ostringstream os;
		os << "The property_name is empty. A valid JavaScript property name must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	if (!get_property_callback && !set_property_callback) {
		std::ostringstream os;
		os << "Both get_property_callback and set_property_callback are missing. At least one callback must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	if (attributes.find(JSPropertyAttribute::ReadOnly) != attributes.end()) {
		if (!get_property_callback) {
			std::ostringstream os;
			os << "ReadOnly attribute is set but get_property_callback is missing.";
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
		
		if (set_property_callback) {
			std::ostringstream os;
			os << "ReadOnly attribute is set but set_property_callback is provided.";
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
	}

	// Force the ReadOnly attribute if only a getter is provided.
	if (get_property_callback && !set_property_callback) {
		attributes_.insert(JSPropertyAttribute::ReadOnly);
	}

	using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
	std::bitset<4> property_attributes;
	for (auto property_attribute : attributes) {
		const auto bit_position = static_cast<property_attribute_underlying_type>(property_attribute);
		property_attributes.set(bit_position);
	}
}

// Copy constructor.
template<typename T>
JSNativeObjectValuePropertyCallback<T>::JSNativeObjectValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& rhs)
		: property_name_(rhs.property_name_)
		, property_name_for_js_static_value_(rhs.property_name_for_js_static_value_)
		, get_property_callback_(rhs.get_property_callback_)
		, set_property_callback_(rhs.set_property_callback_)
		, attributes_(rhs.attributes_) {
}

// Move constructor.
template<typename T>
JSNativeObjectValuePropertyCallback<T>::JSNativeObjectValuePropertyCallback(JSNativeObjectValuePropertyCallback<T>&& rhs)
		: property_name_(rhs.property_name_)
		, property_name_for_js_static_value_(rhs.property_name_for_js_static_value_)
		, get_property_callback_(rhs.get_property_callback_)
		, set_property_callback_(rhs.set_property_callback_)
		, attributes_(rhs.attributes_) {
}

// Return true if the two JSNativeObjectValuePropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	if (lhs.property_name_ != rhs.property_name_) {
		return false;
	}

	// get_property_callback_
	if (lhs.get_property_callback_ && !rhs.get_property_callback_) {
		return false;
	}
	
	if (!lhs.get_property_callback_ && rhs.get_property_callback_) {
		return false;
	}
	
	// set_property_callback_
	if (lhs.set_property_callback_ && !rhs.set_property_callback_) {
		return false;
	}
	
	if (!lhs.set_property_callback_ && rhs.set_property_callback_) {
		return false;
	}
	
	return lhs.attributes_ == rhs.attributes_;
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
	if (lhs.property_name_for_js_static_value_ < rhs.property_name_for_js_static_value_) {
		return true;
	}
	
	return lhs.attributes_ < rhs.attributes_;
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

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

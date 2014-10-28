/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/RAII/detail/HashUtilities.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"
#include <functional>
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObject;

}} // namespace JavaScriptCoreCPP { namespace RAII {


namespace JavaScriptCoreCPP { namespace detail {

using namespace JavaScriptCoreCPP::RAII;

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

	JSString get_property_name() const {
		return property_name__;
	}
	
	GetNamedPropertyCallback<T> get_get_property_callback() const {
		return get_property_callback__;
	}

	SetNamedPropertyCallback<T> get_set_property_callback() const {
		return set_property_callback__;
	}

	std::unordered_set<JSPropertyAttribute> get_attributes() const {
		return attributes__;
	}

	std::size_t get_hash_value() const {
		return hash_value__;
	}

	~JSNativeObjectValuePropertyCallback() {
  }

	// Copy constructor.
	JSNativeObjectValuePropertyCallback(const JSNativeObjectValuePropertyCallback& rhs)
			: property_name__(rhs.property_name__)
			, get_property_callback__(rhs.get_property_callback__)
			, set_property_callback__(rhs.set_property_callback__)
			, attributes__(rhs.attributes__)
			, hash_value__(hash_val(property_name__, ToJSPropertyAttributes(attributes__))) {
	}
	
	// Move constructor.
	JSNativeObjectValuePropertyCallback(JSNativeObjectValuePropertyCallback&& rhs)
			: property_name__(rhs.property_name__)
			, get_property_callback__(rhs.get_property_callback__)
			, set_property_callback__(rhs.set_property_callback__)
			, attributes__(rhs.attributes__)
			, hash_value__(hash_val(property_name__, ToJSPropertyAttributes(attributes__))) {
	}
	
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
    swap(first.property_name__        , second.property_name__);
    swap(first.get_property_callback__, second.get_property_callback__);
    swap(first.set_property_callback__, second.set_property_callback__);
    swap(first.attributes__           , second.attributes__);
    swap(first.hash_val_             , second.hash_val_);
	}

private:
	
	template<typename U>
	friend bool operator==(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	template<typename U>
	friend bool operator<(const JSNativeObjectValuePropertyCallback<U>& lhs, const JSNativeObjectValuePropertyCallback<U>& rhs);

	JSString                                property_name__;
	GetNamedPropertyCallback<T>             get_property_callback__ { nullptr };
	SetNamedPropertyCallback<T>             set_property_callback__ { nullptr };
	std::unordered_set<JSPropertyAttribute> attributes__;
	                     
	// Precomputed hash value for JSNativeObjectValuePropertyCallback
	// since instances of this class template are immutable.
	std::size_t                             hash_value__;
};

template<typename T>
JSNativeObjectValuePropertyCallback<T>::JSNativeObjectValuePropertyCallback(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback, const std::unordered_set<JSPropertyAttribute>& attributes)
		: property_name__(property_name)
		, get_property_callback__(get_property_callback)
		, set_property_callback__(set_property_callback)
		, attributes__(attributes)
		, hash_value__(hash_val(property_name__, ToJSPropertyAttributes(attributes__))) {
	
	static const std::string log_prefix { "MDL: JSNativeObjectValuePropertyCallback: " };
	
	if (property_name__.empty()) {
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
		attributes__.insert(JSPropertyAttribute::ReadOnly);
	}
}

// Return true if the two JSNativeObjectValuePropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectValuePropertyCallback<T>& lhs, const JSNativeObjectValuePropertyCallback<T>& rhs) {
	if (lhs.property_name__ != rhs.property_name__) {
		return false;
	}

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
	
	return lhs.attributes__ == rhs.attributes__;
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
	if (lhs.property_name__ < rhs.property_name__) {
		return true;
	}
	
	return lhs.attributes__ < rhs.attributes__;
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

template<typename T>
struct hash<JSNativeObjectValuePropertyCallback<T>> {
	using argument_type = JSNativeObjectValuePropertyCallback<T>;
	using result_type   = std::size_t;
	
	result_type operator()(const argument_type& callback) const {
		return callback.get_hash_value();
	}
};

// Hash function for JSNativeObjectValuePropertyCallback so that they
// can be stored in a std::unordered_set.
template<typename T>
using JSNativeObjectValuePropertyCallbackHash = hash<JSNativeObjectValuePropertyCallback<T>>;

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_RAII_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_HPP_

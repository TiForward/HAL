/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JSStaticValue is an RAII wrapper around the
  JavaScriptCore C API JSStaticValue, which describes a statically
  declared value property of a JSObject.
*/
class JSStaticValue final	{
	
 public:

	/*!
	  @method

	  @abstract .................. Create a statically declared value
	                               property of a JSObject.
	                               
	  @param property_name ....... A JSString containing the property's
	                               name.
	  
	  @param get_property_callback A JSObjectGetPropertyCallback to
	                               invoke when getting the property's
	                               value.
	  
	  @param set_property_callback A JSObjectSetPropertyCallback to
	                               invoke when setting the property's
	                               value. May be nullptr if the ReadOnly
	                               attribute is set.
	  
	  @param attributes .......... A logically ORed set of
	                               JSPropertyAttributes to give to the
	                               property.
	  
	  @result .................... An object which describes a
	                               statically declared value property of
	                               a JSObject.
	  
	  @throws .................... std::invalid_argument exception under
	                               these preconditions:

	                               If the ReadOnly attribute is set and
	                               either get_property_callback is not
	                               provided or the set_property_callback
	                               is.
	                               
	                               If both get_property_callback and
	                               set_property_callback are missing.
	                               
	                               If property_name is empty.
	*/
	JSStaticValue(const JSString& property_name, JSObject::GetPropertyCallback get_property_callback, JSObject::SetPropertyCallback set_property_callback, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());

	JSString get_property_name() const {
		return property_name_;
	}
	
	JSObject::GetPropertyCallback get_get_property_callback() const {
		return get_property_callback_;
	}

	JSObject::SetPropertyCallback get_set_property_callback() const {
		return set_property_callback_;
	}

	std::set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

	~JSStaticValue() {
  }

	// Copy constructor.
	JSStaticValue(const JSStaticValue& rhs);
	
	// Move constructor.
	JSStaticValue(JSStaticValue&& rhs);
  
	// Create a copy of another JSStaticValue by assignment. This is a
	// unified assignment operator that fuses the copy assignment
	// operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSStaticValue& operator=(JSStaticValue rhs) {
		swap(*this, rhs);
		return *this;
  }
  
	friend void swap(JSStaticValue& first, JSStaticValue& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
    // by swapping the members of two classes,
		// the two classes are effectively swapped
    swap(first.property_name_                    , second.property_name_);
    swap(first.property_name_for_js_static_value_, second.property_name_for_js_static_value_);
    swap(first.get_property_callback_            , second.get_property_callback_);
    swap(first.set_property_callback_            , second.set_property_callback_);
    swap(first.attributes_                       , second.attributes_);
    swap(first.js_static_value_                  , second.js_static_value_);
	}

private:
	
	// For interoperability with the JavaScriptCore C API.
	operator ::JSStaticValue() const {
		return js_static_value_;
	}
	
	// Return true if the two JSStaticValues are equal.
	friend bool operator==(const JSStaticValue& lhs, const JSStaticValue& rhs);

	// Define a strict weak ordering for two JSStaticValues.
	friend bool operator<(const JSStaticValue& lhs, const JSStaticValue& rhs);

	JSString                      property_name_;
	std::string                   property_name_for_js_static_value_;
	JSObject::GetPropertyCallback get_property_callback_ { nullptr };
	JSObject::SetPropertyCallback set_property_callback_ { nullptr };
	std::set<JSPropertyAttribute> attributes_;
	::JSStaticValue               js_static_value_ {0, 0, 0, 0};
};

// Return true if the two JSStaticValues are equal.
bool operator==(const JSStaticValue& lhs, const JSStaticValue& rhs);

// Return true if the two JSString are not equal.
inline
bool operator!=(const JSStaticValue& lhs, const JSStaticValue& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSStaticValues.
inline
bool operator<(const JSStaticValue& lhs, const JSStaticValue& rhs) {
	if (lhs.property_name_for_js_static_value_ < rhs.property_name_for_js_static_value_) {
		return true;
	}

	return lhs.attributes_ < rhs.attributes_;
}

inline
bool operator>(const JSStaticValue& lhs, const JSStaticValue& rhs) {
	return rhs < lhs;
}

inline
bool operator<=(const JSStaticValue& lhs, const JSStaticValue& rhs) {
	return !(lhs > rhs);
}

inline
bool operator>=(const JSStaticValue& lhs, const JSStaticValue& rhs) {
	return !(lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_

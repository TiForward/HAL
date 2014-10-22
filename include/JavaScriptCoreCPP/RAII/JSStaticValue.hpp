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
	  
	  @result .................... The JSObject result of conversion.
	  
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
	JSStaticValue(const JSString& property_name, JSObjectGetPropertyCallback get_property_callback, JSObjectSetPropertyCallback set_property_callback, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());

	JSString get_property_name() const {
		return property_name_;
	}
	
	JSObjectGetPropertyCallback get_get_property_callback() const {
		return get_property_callback_;
	}

	JSObjectSetPropertyCallback get_set_property_callback() const {
		return set_property_callback_;
	}

	std::set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

private:
	
	// For interoperability with the JavaScriptCore C API.
	operator ::JSStaticValue() const {
		return js_static_value_;
	}
	
	// Prevent heap based objects.
	static void* operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void* operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSString                      property_name_;
	std::string                   property_name_for_js_static_value_;
	JSObjectGetPropertyCallback   get_property_callback_ { nullptr };
	JSObjectSetPropertyCallback   set_property_callback_ { nullptr };
	std::set<JSPropertyAttribute> attributes_;
	::JSStaticValue               js_static_value_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICVALUE_HPP_

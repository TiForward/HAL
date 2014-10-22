/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @discussion A JSStaticFunction is an RAII wrapper around the
  JavaScriptCore C API JSStaticFunction, which describes a statically
  declared function property of a JSObject.
*/
class JSStaticFunction final	{
	
 public:

	/*!
	  @method

	  @abstract ...................... Create a statically declared
	                                   function property of a JSObject.
	                               
	  @param function_name ........... A JSString containing the
	                                   function's name. If function_name
	                                   is empty then an anonymous
	                                   function is craeted.
	  
	  @param callget_property_callback A JSObjectGetPropertyCallback to
	                                   invoke when getting the
	                                   property's value.
	                                   
	  
	  @param attributes .............. A logically ORed set of
	                                   JSPropertyAttributes to give to
	                                   the property.
	  
	  @result ........................ An object which describes a
	                                   statically declared function
	                                   property of a JSObject
	*/
	JSStaticFunction(const JSString& function_name, JSObjectCallAsFunctionCallback call_as_function_callback, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());

	JSString get_function_name() const {
		return function_name_;
	}
	
	JSObjectCallAsFunctionCallback get_call_as_function_callback() const {
		return call_as_function_callback_;
	}

	std::set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

private:
	
	// For interoperability with the JavaScriptCore C API.
	operator ::JSStaticFunction() const {
		return js_static_function_;
	}
	
	// Prevent heap based objects.
	static void* operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void* operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSString                       function_name_;
	std::string                    function_name_for_js_static_value_;
	JSObjectCallAsFunctionCallback call_as_function_callback_ { nullptr };
	std::set<JSPropertyAttribute>  attributes_;
	::JSStaticFunction             js_static_function_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSSTATICFUNCTION_HPP_

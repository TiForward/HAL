/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_

#include <Util/JSGlobalContextRefRAII.hpp>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

class JSValue final	{
	
 public:

	// Create an empty object.
	JSValue(const JSValueRef& js_value) : js_value_(js_value) {
		JSValueProtect(context_, js_object_);
	}
	
	~JSValue() {
		JSValueUnprotect(context_, js_object_);
	}
	
	// Copy constructor.
	JSValue(const JSValue& rhs) {
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSValueUnprotect(context_, js_object_);

		js_object_ = rhs.js_object_;
		context_   = rhs.context_;

		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSValueProtect(context_, js_object_);
	}
	
	// Create a copy of another JSValue by assignment.
	JSValue& operator=(const JSValue& rhs) {
		if (this == &rhs) {
			return *this;
		}
		
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSValueUnprotect(context_, js_object_);
		
		js_object_ = rhs.js_object_;
		context_   = rhs.context_;
		
		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSValueProtect(context_, js_object_);
		
		return *this;
	}
	
	operator JSObjectRef() const {
		return js_object_;
	}
	
 private:

	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSValueRef       js_value_;
	JSContextRefRAII js_context_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_

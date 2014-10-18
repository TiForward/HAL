/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

class JSObject final	{
	
 public:

	// Create an empty object.
	JSObject(JSObjectRef js_object, JSGlobalContextRef context) : js_object_(js_object), context_(context) {
		JSValueProtect(context_, js_object_);
	}
	
	~JSObject() {
		JSValueUnprotect(context_, js_object_);
	}
	
	// Copy constructor.
	JSObject(const JSObject& rhs) {
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
	
	// Create a copy of another JSObject by assignment.
	JSObject& operator=(const JSObject& rhs) {
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
	
	JSObjectRef        js_object_;
	JSGlobalContextRef context_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

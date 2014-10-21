/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

// Standard JavaScript practice calls for storing function objects in
// prototypes, so they can be shared. The default constructor for
// JSClass follows this idiom, instantiating objects with a shared,
// automatically generated prototype containing the class's function
// objects.
//
// The kJSClassAttributeNoAutomaticPrototype attribute specifies that
// a JSClass should not automatically generate such a prototype. The
// resulting JSClass instantiates objects with the default object
// prototype, and gives each instance object its own copy of the
// class's function objects.
class JSClass final	{
	
 public:

	// Create an empty object.
	JSClass(const JSClassDefinition* definition) : js_class_(JSClassCreate(definition)) {
	}
	
	~JSClass() {
		JSClassRelease(js_class_);
	}
	
	// Copy constructor.
	JSClass(const JSClass& rhs) {
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
	
	// Create a copy of another JSClass by assignment.
	JSClass& operator=(const JSClass& rhs) {
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
	
	JSClassDefinition  js_class_definition_;
	JSClassRef         js_class_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

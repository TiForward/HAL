/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_


#include <Util/JSContextGroupRefRAII.hpp>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

// A JSContext allocates a global object and populates it with all the
// built-in JavaScript objects, such as Object, Function, String, and
// Array.
//
// The context is created within a unique context group. Therefore,
// scripts may execute in it concurrently with scripts executing in
// other contexts. However, you may not use values created in the
// context in other contexts.
class JSContext final	{
	
 public:

	// Create a global JavaScript execution context within a unique
	// context group and populate it with all the built-in JavaScript
	// objects, such as Object, Function, String, and Array.
	// JSContext() : JSContext(nullptr) { }
	
	// Create a global JavaScript execution context within a unique
	// context group using the given object class (parameter
	// globalObjectClass). Passing nullptr (the default) for the
	// globalObjectClass will use the default object class. The context
	// is populated with all the built-in JavaScript objects, such as
	// Object, Function, String, and Array.
	JSContext(JSClassRef globalObjectClass = nullptr)
			: js_context_(JSGlobalContextCreate(globalObjectClass))
			, js_context_group_(JSContextGroupRefRAII(JSContextGetGroup(js_context_)))
	{
	}

	// Create a global JavaScript execution context within the given
	// context group using the given object class (parameter
	// globalObjectClass). Passing nullptr (the default) for the
	// globalObjectClass will use the default object class. The context
	// is populated with all the built-in JavaScript objects, such as
	// Object, Function, String, and Array.
	JSGlobalContextRefRAII(const JSContextGroupRefRAII& js_context_group, JSClassRef globalObjectClass = nullptr)
			: js_context_(JSGlobalContextCreateInGroup(js_context_group, globalObjectClass))
			: js_context_group_(js_context_group)
	{
	}

	~JSGlobalContextRefRAII() {
		JSGlobalContextRelease(js_context_group_);
	}
	
	// Copy constructor.
	JSGlobalContextRefRAII(const JSGlobalContextRefRAII& rhs) {
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSGlobalContextRetain(js_context_);

		js_context_     _ = rhs.js_context_;
		js_context_group_ = rhs.js_context_group_;

		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSGlobalContextRelease(js_context_);
	}
	
	// Create a copy of another JSGlobalContextRefRAII by assignment.
	JSGlobalContextRefRAII& operator=(const JSGlobalContextRefRAII& rhs) {
		if (this == &rhs) {
			return *this;
		}
		
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSGlobalContextRetain(js_context_);

		js_context_     _ = rhs.js_context_;
		js_context_group_ = rhs.js_context_group_;

		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSGlobalContextRelease(js_context_);
		
		return *this;
	}

	JSContextGroupRefRAII get_context_group() const {
		return js_context_group_;
	}
	
	JSObjectRefRAII get_global_object() const {
		return JSContextGetGlobalObject(js_context_);
	}

	operator JSContextRef() const {
		return js_context_group_;
	}
	
	operator JSGlobalContextRef() const {
		return js_context_group_;
	}

private:

	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSGlobalContextRef    js_context_;
	JSContextGroupRefRAII js_context_group_;
};

// A JSGlobalContextRef is a JSContext.
using JSContext = JSGlobalContext;

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

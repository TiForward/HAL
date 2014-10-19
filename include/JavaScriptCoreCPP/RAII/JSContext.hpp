/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_


#include "JavaScriptCoreCPP/RAII/JSContextGroup.hpp"
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
			, js_context_group_(JSContextGroup(JSContextGetGroup(js_context_)))
	{
	}

	// Create a global JavaScript execution context within the given
	// context group using the given object class (parameter
	// globalObjectClass). Passing nullptr (the default) for the
	// globalObjectClass will use the default object class. The context
	// is populated with all the built-in JavaScript objects, such as
	// Object, Function, String, and Array.
	JSContext(const JSContextGroup& js_context_group, JSClassRef globalObjectClass = nullptr)
			: js_context_(JSGlobalContextCreateInGroup(js_context_group, globalObjectClass))
			, js_context_group_(js_context_group)
	{
	}

	~JSContext() {
		JSGlobalContextRelease(js_context_);
	}
	
	// Copy constructor.
	JSContext(const JSContext& rhs) {
		js_context_       = rhs.js_context_;
    js_context_group_ = rhs.js_context_group_;
		JSGlobalContextRetain(js_context_);
	}
	
  // Move constructor.
  JSContext(JSContext&& rhs) {
    js_context_ = rhs.js_context_;
    JSGlobalContextRetain(rhs.js_context_);
    js_context_group_ = rhs.js_context_group_;
  }
  
  // Create a copy of another JSContextGroup by assignment. This is a unified
  // assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
  JSContext& operator=(JSContext rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSContext& first, JSContext& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_context_      , second.js_context_      );
    swap(first.js_context_group_, second.js_context_group_);
  }

	JSContextGroup get_context_group() const {
		return js_context_group_;
	}
	
  // TODO: Change JSObjectRef to JSObject
	JSObjectRef get_global_object() const {
		return JSContextGetGlobalObject(js_context_);
	}

	operator JSContextRef() const {
		return js_context_;
	}
	
private:

  // Return true if the two JSContexts are equal.
  friend bool operator==(const JSContext& lhs, const JSContext& rhs);

  // Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSGlobalContextRef js_context_;
	JSContextGroup     js_context_group_;
};

// Return true if the two JSContexts are equal.
inline
bool operator==(const JSContext& lhs, const JSContext& rhs) {
  return (lhs.js_context_ == rhs.js_context_) && (lhs.js_context_group_ == rhs.js_context_group_);
}
  
// Return true if the two JSContextGroups are not equal.
inline
bool operator!=(const JSContext& lhs, const JSContext& rhs) {
  return ! (lhs == rhs);
}


} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

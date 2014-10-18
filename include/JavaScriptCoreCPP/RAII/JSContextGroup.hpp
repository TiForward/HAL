/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

// A JSContextGroup associates JavaScript contexts with one
// another. JSContexts within the same group may share and exchange
// JavaScript objects. Sharing and/or exchanging JavaScript objects
// between contexts in different groups will produce undefined
// behavior.  When objects from the same context group are used in
// multiple threads, explicit synchronization is required.
class JSContextGroup final	{
	
 public:

	JSContextGroup() : js_context_group_(JSContextGroupCreate()) {
	}
	
	JSContextGroup(JSContextGroup js_context_group) : js_context_group_(js_context_group) {
		JSContextGroupRetain(js_context_group_);
	}

	~JSContextGroup() {
		JSContextGroupRelease(js_context_group_);
	}
	
	// Copy constructor.
	JSContextGroup(const JSContextGroup& rhs) {
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSContextGroupRelease(js_context_group_);

		js_context_group_ = rhs.js_context_group_;

		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSContextGroupRetain(js_context_group_);
	}
	
	// Create a copy of another JSContextGroup by assignment.
	JSContextGroup& operator=(const JSContextGroup& rhs) {
		if (this == &rhs) {
			return *this;
		}
		
		// Tell the JavaScriptCore garbage collector that we have no more
		// interest in the object being replaced.
		JSContextGroupRelease(js_context_group_);
		
		js_context_group_ = rhs.js_context_group_;
		
		// However, we must tell the JavaScriptCore garbage collector that
		// we do have an interest in the object that replaced the previous
		// one.
		JSContextGroupRetain(js_context_group_);
		
		return *this;
	}
	
	operator JSContextGroup() const {
		return js_context_group_;
	}
	
 private:

	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSContextGroup js_context_group;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

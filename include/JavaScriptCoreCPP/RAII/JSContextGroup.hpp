/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

#include <utility>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JSContextGroup is an RAII wrapper around a
  JSContextGroupRef, the JavaScriptCore C API representation of a
  group that associates JavaScript contexts with one another.
  
  JSContexts within the same group may share and exchange JavaScript
  objects. Sharing and/or exchanging JavaScript objects between
  contexts in different groups will produce undefined behavior. When
  objects from the same context group are used in multiple threads,
  explicit synchronization is required.
*/
class JSContextGroup final	{
	
 public:

	/*!
	  @method
	  @abstract Create a JavaScript context group.
	  @discussion A JSContextGroup associates JavaScript contexts with
	  one another.  Contexts in the same group may share and exchange
	  JavaScript objects. Sharing and/or exchanging JavaScript objects
	  between contexts in different groups will produce undefined
	  behavior. When objects from the same context group are used in
	  multiple threads, explicit synchronization is required.
	*/
	JSContextGroup() : js_context_group_ref_(JSContextGroupCreate()) {
	}
	
	~JSContextGroup() {
		JSContextGroupRelease(js_context_group_ref_);
	}
	
	// Copy constructor.
	JSContextGroup(const JSContextGroup& rhs) {
		js_context_group_ref_ = rhs.js_context_group_ref_;
		JSContextGroupRetain(js_context_group_ref_);
	}
	
  // Move constructor.
  JSContextGroup(JSContextGroup&& rhs) {
    js_context_group_ref_ = rhs.js_context_group_ref_;
    JSContextGroupRetain(js_context_group_ref_);
  }
  
  // Create a copy of another JSContextGroup by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator, X& X::operator=(const X&), and the move assignment
  // operator, X& X::operator=(X&&);
  JSContextGroup& operator=(JSContextGroup rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSContextGroup& first, JSContextGroup& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_context_group_ref_, second.js_context_group_ref_);
  }

 private:

  friend class JSContext;

  // For interoperability with the JavaScriptCore C API.
	JSContextGroup(JSContextGroupRef js_context_group_ref) : js_context_group_ref_(js_context_group_ref) {
		JSContextGroupRetain(js_context_group_ref_);
	}

  // For interoperability with the JavaScriptCore C API.
	operator JSContextGroupRef() const {
		return js_context_group_ref_;
	}
	
  // Return true if the two JSContextGroups are equal.
  friend bool operator==(const JSContextGroup& lhs, const JSContextGroup& rhs);

  // Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
  JSContextGroupRef js_context_group_ref_ {nullptr};
};

// Return true if the two JSContextGroups are equal.
inline
bool operator==(const JSContextGroup& lhs, const JSContextGroup& rhs) {
  return lhs.js_context_group_ref_ == rhs.js_context_group_ref_;
}
  
// Return true if the two JSContextGroups are not equal.
inline
bool operator!=(const JSContextGroup& lhs, const JSContextGroup& rhs) {
  return ! (lhs == rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

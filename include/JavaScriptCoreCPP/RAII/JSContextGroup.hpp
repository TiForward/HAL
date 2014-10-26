/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

#include "JavaScriptCoreCPP/RAII/detail/JSPerformanceCounter.hpp"
#include <utility>
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

class JSContext;
class JSClass;

/*!
  @class
  
  @discussion A JSContextGroup is an RAII wrapper around a
  JSContextGroupRef, the JavaScriptCore C API representation of a
  group that associates JavaScript contexts with one another.

  JSContexts within the same context group may share and exchange
  JavaScript objects with one another.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSContextGroup final : public detail::JSPerformanceCounter<JSContextGroup> {
#else
class JSContextGroup final	{
#endif
	
 public:

	/*!
	  @method
	  
	  @abstract Create a JavaScript context group. JSContexts within
	  this context group may share and exchange JavaScript objects with
	  one another.
	*/
	JSContextGroup() : js_context_group_ref_(JSContextGroupCreate()) {
	}
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript execution context within this
	  context group with a default global object populated with all of
	  the standard built-in JavaScript objects, such as Object,
	  Function, String, and Array. Scripts may execute in this context
	  concurrently with scripts executing in other contexts.

	  @discussion All JSContexts within this context group may share and
	  exchange JavaScript values with one another.
	  
	  When JavaScript objects from the same context group are used in
	  multiple threads, explicit synchronization is required.
	*/
	JSContext CreateContext() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript execution context within this
	  context group with a global object created from a custom
	  JSClass. Scripts may execute in this context concurrently with
	  scripts executing in other contexts.

	  @discussion All JSContexts within this context group may share and
	  exchange JavaScript values with one another.
	  
	  When JavaScript objects from the same context group are used in
	  multiple threads, explicit synchronization is required.

	  @param global_object_class The JSClass used to create the global
	  object.
	*/
	JSContext CreateContext(JSClass global_object_class) const;

	~JSContextGroup() {
		JSContextGroupRelease(js_context_group_ref_);
	}
	
	// Copy constructor.
	JSContextGroup(const JSContextGroup& rhs) : js_context_group_ref_(rhs.js_context_group_ref_) {
		JSContextGroupRetain(js_context_group_ref_);
	}
	
  // Move constructor.
  JSContextGroup(JSContextGroup&& rhs) : js_context_group_ref_(rhs.js_context_group_ref_) {
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

  // For interoperability with the JavaScriptCore C API.
  explicit JSContextGroup(JSContextGroupRef js_context_group_ref) : js_context_group_ref_(js_context_group_ref) {
		assert(js_context_group_ref_);
		JSContextGroupRetain(js_context_group_ref_);
	}

  // For interoperability with the JavaScriptCore C API.
	operator JSContextGroupRef() const {
		return js_context_group_ref_;
	}
	
	// Prevent heap based objects.
	void* operator new(size_t)     = delete; // #1: To prevent allocation of scalar objects
	void* operator new [] (size_t) = delete; // #2: To prevent allocation of array of objects

  friend class JSContext;

  // Return true if the two JSContextGroups are equal.
  friend bool operator==(const JSContextGroup& lhs, const JSContextGroup& rhs);

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

#endif // _JAVASCRIPTCORECPP_RAII_JSCONTEXTGROUP_HPP_

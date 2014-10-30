/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASS_HPP_
#define _JAVASCRIPTCORECPP_JSCLASS_HPP_

#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#include <utility>
#include <JavaScriptCore/JavaScript.h>


#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>

#unndef JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE std::recursive_mutex

#unndef JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME 
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME js_class

#undef  JAVASCRIPTCORECPP_JSCLASS_MUTEX
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME##_mutex_;


#undef  JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_JSCLASS_MUTEX_TYPE> JAVASCRIPTCORECPP_JSCLASS_MUTEX_NAME##_lock(JAVASCRIPTCORECPP_JSCLASS_MUTEX);

#else
#define JAVASCRIPTCORECPP_JSCLASS_MUTEX
#define JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


namespace JavaScriptCoreCPP {

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class that
  defines JavaScript objects implemented in C.

  An instance of JSClass may be passed to the JSContextGroup
  constructor to create a custom global object for all contexts in
  that group.

  JavaScript objects based on a JSClass may be created by the
  JSContext::CreateObject member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSClass : public detail::JSPerformanceCounter<JSClass> {
#else
class JSClass	{
#endif
	
 public:

	// For interoperability with the JavaScriptCore C API.
	JSClass(const JSClassDefinition* definition = &kJSClassDefinitionEmpty)
			: js_class_ref__(JSClassCreate(definition)) {
	}
	
	virtual ~JSClass() {
		JSClassRelease(js_class_ref__);
	}
	
	// Copy constructor.
	JSClass(const JSClass& rhs) : js_class_ref__(rhs.js_class_ref__) {
		JSClassRetain(js_class_ref__);
	}
	
	// Move constructor.
	JSClass(JSClass&& rhs) : js_class_ref__(rhs.js_class_ref__) {
		JSClassRetain(js_class_ref__);
	}
	
#ifdef JAVASCRIPTCORECPP_MOVE_SEMANTICS_ENABLE
  JSClass& JSClass::operator=(const JSClass&) = default;
  JSClass& JSClass::operator=(JSClass&&) = default;
#endif

	// Create a copy of another JSClass by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSClass& operator=(JSClass rhs) {
		swap(*this, rhs);
		return *this;
	}
	
	friend void swap(JSClass& first, JSClass& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.js_class_ref__, second.js_class_ref__);
	}

 private:

	// For interoperability with the JavaScriptCore C API.
	operator JSClassRef() const {
		return js_class_ref__;
	}

	// The JSContext constructor needs access to operator JSClassRef().
	friend class JSContext;
	
	// JSContext::IsObjectOfClass needs access to operator JSClassRef().
	friend class JSValue;
	
	// The JSObject constructor needs access to operator JSClassRef().
	friend class JSObject;

	// The JSNativeClass constructor access to js_class_ref__ to change
	// its value.
	template<typename T>
	friend class JSNativeClass;

	JSClassRef js_class_ref__{ nullptr };
	JAVASCRIPTCORECPP_JSCLASS_MUTEX;
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASS_HPP_

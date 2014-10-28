/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

#include "JavaScriptCoreCPP/RAII/detail/JSPerformanceCounter.hpp"
#include <utility>
#include <JavaScriptCore/JavaScript.h>


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#ifndef JAVASCRIPTCORECPP_RAII_JSCLASS_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSCLASS_MUTEX std::mutex js_class_mutex_;
#endif

#ifndef JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD std::lock_guard<std::mutex> js_class_lock(js_class_mutex_);
#endif

#else
#define JAVASCRIPTCORECPP_RAII_JSCLASS_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class. An
  instance of JSClass can be passed to the JSObject constructor to
  create JavaScript objects with both value and function properties
  backed by an instance of a C++ class.

  Only JSObject and JSNativeClassBuilder may create a JSClass.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSClass : public detail::JSPerformanceCounter<JSClass> {
#else
class JSClass	{
#endif
	
 public:

	virtual ~JSClass() {
		JSClassRelease(js_class_ref_);
	}
	
	// Copy constructor.
	JSClass(const JSClass& rhs) : js_class_ref_(rhs.js_class_ref_) {
		JSClassRetain(js_class_ref_);
	}
	
	// Move constructor.
	JSClass(JSClass&& rhs) : js_class_ref_(rhs.js_class_ref_) {
		JSClassRetain(js_class_ref_);
	}
	
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
		swap(first.js_class_ref_, second.js_class_ref_);
	}

 private:

	// For interoperability with the JavaScriptCore C API.
	JSClass(const JSClassDefinition* definition = &kJSClassDefinitionEmpty)
			: js_class_ref_(JSClassCreate(definition)) {
	}
	
	// For interoperability with the JavaScriptCore C API.
	operator JSClassRef() const {
		return js_class_ref_;
	}
	
	friend class JSContext;
	friend class JSObject;
	
	template<typename T>
	friend class JSNativeClass;

	JSClassRef js_class_ref_ { nullptr };
	JAVASCRIPTCORECPP_RAII_JSCLASS_MUTEX
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

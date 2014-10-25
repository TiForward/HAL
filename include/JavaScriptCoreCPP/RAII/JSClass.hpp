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

#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#include <utility>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class. An
  instance of JSClass can be passed to the JSObject constructor to
  create JavaScript objects with both value and function properties
  backed by an instance of a C++ class.

  Only JSObject and JSNativeObjectBuilder may create a JSClass.
*/
#ifdef JAVASCRIPTCORECPP_RAII_ENABLE_PERFORMANCE_COUNTER
class JSClass final : public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSClass> {
#else
class JSClass final	{
#endif
	
 public:

	~JSClass() {
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
	JSClass(const JSClassDefinition* definition) : js_class_ref_(JSClassCreate(definition)) {
	}
	
	// For interoperability with the JavaScriptCore C API.
	operator JSClassRef() const {
		return js_class_ref_;
	}
	
	friend class JSContext;
	friend class JSObject;
	
	template<typename T>
	friend class JSNativeObject;

	JSClassRef js_class_ref_ { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

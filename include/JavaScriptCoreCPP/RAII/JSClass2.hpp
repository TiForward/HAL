/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS2_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS2_HPP_

#include "JavaScriptCoreCPP/RAII/JSClassDefinition2.hpp"
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSClassBuilder2;

/*!
  @class

  @discussion A JSClass is an RAII wrapper around a JSClassRef, the
  JavaScriptCore C API representation of a JavaScript class. An
  instance of JSClass can be passed to the JSObject constructor to
  create JavaScript objects with custom behavior.

  The only way to create a JSClass is to use the JSClassBuilder.
*/
class JSClass2 final	{
	
 public:

	template<typename T>
	JSClassDefinition<2> get_js_class_definition() const {
		return js_class_definition_;
	}
	~JSClass2() {
		JSClassRelease(js_class_ref_);
	}
	
	// Copy constructor.
	JSClass2(const JSClass2& rhs)
			: js_class_definition_(rhs.js_class_definition_)
			, js_class_ref_(rhs.js_class_ref_) {
		JSClassRetain(js_class_ref_);
	}
	
	// Move constructor.
	JSClass2(JSClass2&& rhs)
			: js_class_definition_(rhs.js_class_definition_)
			, js_class_ref_(rhs.js_class_ref_) {
		JSClassRetain(js_class_ref_);
	}
	
	// Create a copy of another JSClass by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSClass2& operator=(JSClass2 rhs) {
		swap(*this, rhs);
		return *this;
	}
	
	friend void swap(JSClass2& first, JSClass2& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.js_class_definition_, second.js_class_definition_);
		swap(first.js_class_ref_       , second.js_class_ref_);
	}
	
 private:
	
	// Only the JSClassBuilder can create instances of JSClass.
	template<typename T>
	JSClass2(const JSClassBuilder2<T>& builder);

	// For interoperability with the JavaScriptCore C API.
	operator JSClassRef() const {
		return js_class_ref_;
	}

	friend class JSObject;

	template<typename T>
	friend class JSClassBuilder2;

	template<typename T>
	JSClassDefinition2<T> js_class_definition_;
	JSClassRef            js_class_ref_ { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS2_HPP_

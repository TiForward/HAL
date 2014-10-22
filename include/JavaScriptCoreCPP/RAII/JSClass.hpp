/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

#include "JavaScriptCoreCPP/RAII/JSClassDefinition.hpp"
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
	JSClass(const JSClassDefinition& js_class_definition)
			: js_class_definition_(js_class_definition)
			, js_class_ref_(JSClassCreate(js_class_definition_)) {
	}
	
	~JSClass() {
		JSClassRelease(js_class_ref_);
	}
	
	// Copy constructor.
	JSClass(const JSClass& rhs)
			: js_class_definition_(rhs.js_class_definition_)
			, js_class_ref_(rhs.js_class_ref_) {
		JSClassRetain(js_class_ref_);
	}
	
	// Move constructor.
	JSClass(JSClass&& rhs)
			: js_class_definition_(rhs.js_class_definition_)
			, js_class_ref_(rhs.js_class_ref_) {
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
		swap(first.js_class_definition_, second.js_class_definition_);
		swap(first.js_class_ref_       , second.js_class_ref_);
	}
	
 private:
	
	// // For interoperability with the JavaScriptCore C API.
	// JSClass(const ::JSClassDefinition* definition) {
	// 	assert(definition);
	// }

	// For interoperability with the JavaScriptCore C API.
	operator JSClassRef() const {
		return js_class_ref_;
	}

	JSClassDefinition js_class_definition_;
	JSClassRef        js_class_ref_ { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCLASS_HPP_

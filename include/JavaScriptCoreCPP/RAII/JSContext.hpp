/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

#include "JavaScriptCoreCPP/RAII/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

class JSValue;
class JSObject;

/*!
  @class
  
  @discussion A JSContext is an RAII wrapper around a JSContextRef,
  the JavaScriptCore C API representation of a JavaScript execution
  context that holds the global object and other execution state.

  Scripts may execute concurrently with scripts executing in other
  contexts.
  
  The default constructor creates JSContext in a unique context group
  with a default global object populated with all the standard
  built-in JavaScript objects, such as Object, Function, String, and
  Array.

  A JSContext may also be created in a specific context group so that
  JavaScript objects may be shared and exchanged with other contexts
  within that group.

  When JavaScript objects from the same context group are used in
  multiple threads, explicit synchronization is required.
*/
class JSContext final	{
	
 public:

 public:

	/*!
	  @method
	  
	  @abstract Create a JavaScript execution context in a unique
	  context group with a default global object populated with all the
	  standard built-in JavaScript objects, such as Object, Function,
	  String, and Array. Scripts may execute in this context
	  concurrently with scripts executing in other contexts.
	*/
	JSContext()
			: js_global_context_ref_(JSGlobalContextCreate(nullptr))
			, js_context_group_(JSContextGroup(JSContextGetGroup(js_global_context_ref_))) {
	}

	/*!
	  @method

	  @abstract Create a JavaScript execution context in a specific
	  context group with a default global object populated with all the
	  standard built-in JavaScript objects, such as Object, Function,
	  String, and Array. Scripts may execute in this context
	  concurrently with scripts executing in other contexts.

	  @discussion A JSContext created in a specific context group allows
	  JavaScript objects to be shared and exchanged with other contexts
	  within that group.
	  
	  When JavaScript objects from the same context group are used in
	  multiple threads, explicit synchronization is required.

	  @param js_context_group The context group within which the
	  JSContext is created.
	*/
	JSContext(const JSContextGroup& js_context_group)
			: js_global_context_ref_(JSGlobalContextCreateInGroup(js_context_group, nullptr))
			, js_context_group_(js_context_group)	{
	}

	/*!
	  @method
	  
	  @abstract Create a JavaScript execution context in a unique
	  context group with a global object created from a custom
	  JSClass. Scripts may execute in this context concurrently with
	  scripts executing in other contexts.

	  @param global_object_class The JSClass used to create the global
	  object.
	*/
	JSContext(JSClass global_object_class)
			: js_global_context_ref_(JSGlobalContextCreate(global_object_class))
			, js_context_group_(JSContextGroup(JSContextGetGroup(js_global_context_ref_))) {
	}

	/*!
	  @method
	  
	  @abstract Create a JavaScript execution context in a specific
	  context group with a global object created from a custom
	  JSClass. Scripts may execute in this context concurrently with
	  scripts executing in other contexts.

	  @discussion A JSContext created in a specific context group allows
	  JavaScript objects to be shared and exchanged with other contexts
	  within that group.
	  
	  When JavaScript objects from the same context group are used in
	  multiple threads, explicit synchronization is required.

	  @param js_context_group The context group within which the
	  JSContext is created.
	  
	  @param global_object_class The JSClass used to create the global
	  object.
	*/
	JSContext(const JSContextGroup& js_context_group, JSClass global_object_class)
			: js_global_context_ref_(JSGlobalContextCreateInGroup(js_context_group, global_object_class))
			, js_context_group_(js_context_group)	{
	}

	/* Script Evaluation */
	
	/*!
	  @method
	  
	  @abstract Evaluate a string of JavaScript using the global object
	  as "this.".
	  
	  @param script A JSString containing the script to evaluate.
	  
	  @param source_url An optional JSString containing a URL for the
	  script's source file. This is used by debuggers and when reporting
	  exceptions.
	  
	  @param starting_line_number An optional integer value specifying
	  the script's starting line number in the file located at
	  source_url. This is only used when reporting exceptions. The value
	  is one-based, so the first line is line 1 and invalid values are
	  clamped to 1.
	  
	  @result The JSValue that results from evaluating script.

	  @throws std::runtime_error exception if the evaluated script threw
	  an exception.
	*/
	JSValue JSEvaluateScript(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	/*!
	  @method
	  
	  @abstract Evaluate a string of JavaScript.
	  
	  @param script A JSString containing the script to evaluate.
	  
	  @param this_object The object to use as "this".
	  
	  @param source_url An optional JSString containing a URL for the
	  script's source file. This is used by debuggers and when reporting
	  exceptions.
	  
	  @param starting_line_number An optional integer value specifying
	  the script's starting line number in the file located at
	  source_url. This is only used when reporting exceptions. The value
	  is one-based, so the first line is line 1 and invalid values are
	  clamped to 1.
	  
	  @result The JSValue that results from evaluating script.
	  
	  @throws std::runtime_error exception if the evaluated script threw
	  an exception.
	  
	*/
	JSValue JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	
	/*!
	  @method
	  
	  @abstract Check for syntax errors in a string of JavaScript.
	  
	  @param script A JSString containing the script to check for syntax
	  errors.
	  
	  @param source_url An optional JSString containing a URL for the
	  script's source file. This is used by debuggers and when reporting
	  exceptions.
	  
	  @param starting_line_number An optional integer value specifying
	  the script's starting line number in the file located at
	  source_url. This is only used when reporting exceptions. The value
	  is one-based, so the first line is line 1 and invalid values are
	  clamped to 1.
	  
	  @result true if the script is syntactically correct, otherwise
	  false.
	*/
	bool JSCheckScriptSyntax(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	~JSContext() {
		JSGlobalContextRelease(js_global_context_ref_);
	}
	
	// Copy constructor.
	JSContext(const JSContext& rhs)
			: js_global_context_ref_(rhs.js_global_context_ref_)
			, js_context_group_(rhs.js_context_group_) {
		JSGlobalContextRetain(js_global_context_ref_);
	}
	
  // Move constructor.
  JSContext(JSContext&& rhs)
		  : js_global_context_ref_(rhs.js_global_context_ref_)
		  , js_context_group_(rhs.js_context_group_) {
	  JSGlobalContextRetain(rhs.js_global_context_ref_);
  }
  
  // Create a copy of another JSContext by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator, X& X::operator=(const X&), and the move assignment
  // operator, X& X::operator=(X&&);
  JSContext& operator=(JSContext rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSContext& first, JSContext& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_global_context_ref_, second.js_global_context_ref_);
    swap(first.js_context_group_     , second.js_context_group_);
  }

	JSContextGroup get_context_group() const {
		return js_context_group_;
	}
	
  // TODO: Change JSObjectRef to JSObject
	JSObjectRef get_global_object() const {
		return JSContextGetGlobalObject(js_global_context_ref_);
	}

private:

	// For interoperability with the JavaScriptCore C API.
	JSContext(JSGlobalContextRef js_global_context_ref)
			: js_global_context_ref_(js_global_context_ref)
			, js_context_group_(JSContextGetGroup(js_global_context_ref)) {
		assert(js_global_context_ref_);
		assert(js_context_group_);
		JSGlobalContextRetain(js_global_context_ref);
	}
		
	// For interoperability with the JavaScriptCore C API.
	JSContext(JSContextRef js_context_ref)
			: JSContext(const_cast<JSGlobalContextRef>(js_context_ref)) {
	}

	// For interoperability with the JavaScriptCore C API.
	operator JSContextRef() const {
		return js_global_context_ref_;
	}

	friend class JSValue;
  friend class JSUndefined;
  friend class JSNull;
  friend class JSBoolean;
  friend class JSNumber;
	friend class JSObject;
	friend class JSArray;
  friend class JSDate;
  friend class JSError;
  friend class JSRegExp;
  friend class JSFunction;

	template<typename T>
	friend class JSNativeObject;
	
	// Return true if the two JSContexts are equal.
  friend bool operator==(const JSContext& lhs, const JSContext& rhs);

  // Return true if the two JSValues are equal as compared by the JS == operator.
  friend bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);
  
	JSGlobalContextRef js_global_context_ref_ { nullptr };
	JSContextGroup     js_context_group_;
};

// Return true if the two JSContexts are equal.
inline
bool operator==(const JSContext& lhs, const JSContext& rhs) {
  return (lhs.js_global_context_ref_ == rhs.js_global_context_ref_) && (lhs.js_context_group_ == rhs.js_context_group_);
}
  
// Return true if the two JSContextGroups are not equal.
inline
bool operator!=(const JSContext& lhs, const JSContext& rhs) {
  return ! (lhs == rhs);
}


}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

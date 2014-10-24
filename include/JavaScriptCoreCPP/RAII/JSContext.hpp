/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

#include "JavaScriptCoreCPP/RAII/JSContextGroup.hpp"
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
  
  A JSContext allocates a global object and populates it with all the
  built-in JavaScript objects, such as Object, Function, String, and
  Array.
  
  The context is created within a unique context group. Therefore,
  scripts may execute in it concurrently with scripts executing in
  other contexts. However, you may not use values created in the
  context in other contexts.

  JSContexts within the same group may share and exchange JavaScript
  objects. Sharing and/or exchanging JavaScript objects between
  contexts in different groups will produce undefined behavior. When
  objects from the same context group are used in multiple threads,
  explicit synchronization is required.
*/
class JSContext final	{
	
 public:

	/*!
	  @method
	  @abstract Create a global JavaScript execution context.
	  @discussion Create a global JavaScript execution context within a
	  unique context group using the given object class (parameter
	  global_object_class). Passing nullptr (the default) for the
	  global_object_class will use the default object class.

	  The context is populated with all the built-in JavaScript objects,
	  such as Object, Function, String, and Array.
	*/
	JSContext(JSClassRef global_object_class = nullptr)
			: js_context_ref_(JSGlobalContextCreate(global_object_class))
			, js_context_group_(JSContextGroup(JSContextGetGroup(js_context_ref_))) {
	}

	/*!
	  @method
	  @abstract Create a global JavaScript execution context in the
	  context group provided.
	  @discussion Create a global JavaScript execution context within
	  the given context group using the given object class (parameter
	  global_object_class). Passing nullptr (the default) for the
	  global_object_class will use the default object class.

	  The context is populated with all the built-in JavaScript objects,
	  such as Object, Function, String, and Array.
	*/
	JSContext(const JSContextGroup& js_context_group, JSClassRef global_object_class = nullptr)
			: js_context_ref_(JSGlobalContextCreateInGroup(js_context_group, global_object_class))
			, js_context_group_(js_context_group)	{
	}

	/* Script Evaluation */
	
	/*!
	  @method
	  @abstract                   Evaluate a string of JavaScript using the global object as "this.".
	  @param script               A JSString containing the script to evaluate.
	  @param source_url           An optional JSString containing a URL for the script's source file. This is used by debuggers and when reporting exceptions.
	  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
	  @result                     The JSValue that results from evaluating script.
	  @throws                     std::runtime_error exception if the evaluated script threw an exception.
	*/
	JSValue JSEvaluateScript(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	/*!
	  @method
	  @abstract                   Evaluate a string of JavaScript.
	  @param script               A JSString containing the script to evaluate.
	  @param this_object          The object to use as "this".
	  @param source_url           An optional JSString containing a URL for the script's source file. This is used by debuggers and when reporting exceptions.
	  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
	  @result                     The JSValue that results from evaluating script.
	  @throws                     std::runtime_error exception if the evaluated script threw an exception.
	*/
	JSValue JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	
	/*!
	  @method
	  @abstract                   Check for syntax errors in a string of JavaScript.
	  @param script               A JSString containing the script to check for syntax errors.
	  @param source_url           An optional JSString containing a URL for the script's source file. This is used by debuggers and when reporting exceptions.
	  @param starting_line_number An optional integer value specifying the script's starting line number in the file located at source_url. This is only used when reporting exceptions. The value is one-based, so the first line is line 1 and invalid values are clamped to 1.
	  @result                     true if the script is syntactically correct, otherwise false.
	*/
	bool JSCheckScriptSyntax(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1);
	
	~JSContext() {
		JSGlobalContextRelease(js_context_ref_);
	}
	
	// Copy constructor.
	JSContext(const JSContext& rhs)
			: js_context_ref_(rhs.js_context_ref_)
			, js_context_group_(rhs.js_context_group_) {
		JSGlobalContextRetain(js_context_ref_);
	}
	
  // Move constructor.
  JSContext(JSContext&& rhs)
		  : js_context_ref_(rhs.js_context_ref_)
		  , js_context_group_(rhs.js_context_group_) {
	  JSGlobalContextRetain(rhs.js_context_ref_);
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
    swap(first.js_context_ref_  , second.js_context_ref_  );
    swap(first.js_context_group_, second.js_context_group_);
  }

	JSContextGroup get_context_group() const {
		return js_context_group_;
	}
	
  // TODO: Change JSObjectRef to JSObject
	JSObjectRef get_global_object() const {
		return JSContextGetGlobalObject(js_context_ref_);
	}

private:

	// For interoperability with the JavaScriptCore C API.
	JSContext(JSGlobalContextRef js_context_ref)
			: js_context_ref_(js_context_ref)
			, js_context_group_(JSContextGetGroup(js_context_ref_)) {
		assert(js_context_ref_);
		assert(js_context_group_);
		JSGlobalContextRetain(js_context_ref_);
	}
		
	// For interoperability with the JavaScriptCore C API.
	operator JSGlobalContextRef() const {
		return js_context_ref_;
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
  
	friend JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSString& source_url, int starting_line_number);
	friend JSValue JSEvaluateScript(const JSContext& js_context, const JSString& script, const JSObject& this_object, const JSString& source_url, int starting_line_number);
	friend bool JSCheckScriptSyntax(const JSContext& js_context, const JSString& script, const JSString& source_url, int starting_line_number);

	JSGlobalContextRef js_context_ref_ { nullptr };
	JSContextGroup     js_context_group_;
};

// Return true if the two JSContexts are equal.
inline
bool operator==(const JSContext& lhs, const JSContext& rhs) {
  return (lhs.js_context_ref_ == rhs.js_context_ref_) && (lhs.js_context_group_ == rhs.js_context_group_);
}
  
// Return true if the two JSContextGroups are not equal.
inline
bool operator!=(const JSContext& lhs, const JSContext& rhs) {
  return ! (lhs == rhs);
}


}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

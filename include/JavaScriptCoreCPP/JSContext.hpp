/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCONTEXT_HPP_
#define _JAVASCRIPTCORECPP_JSCONTEXT_HPP_

#include "JavaScriptCoreCPP/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include <vector>
#include <atomic>
#include <cassert>


#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>

#undef  JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_TYPE
#define JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME_PREFIX js_context

#undef  JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME
#define JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_JSCONTEXT_MUTEX
#define JAVASCRIPTCORECPP_JSCONTEXT_MUTEX JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_TYPE JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME;

#undef  JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD
#define JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_TYPE> JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_JSCONTEXT_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_JSCONTEXT_MUTEX
#define JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


#ifdef DEBUG
extern "C" JS_EXPORT void JSSynchronousGarbageCollectForDebugging(JSContextRef);
#endif

namespace JavaScriptCoreCPP { namespace RAII {

class JSValue;
class JSUndefined;
class JSNull;
class JSBoolean;
class JSNumber;
class JSObject;
class JSArray;
class JSDate;
class JSError;
class JSRegExp;
class JSFunction;

/*!
  @class
  
  @discussion A JSContext is an RAII wrapper around a JSContextRef,
  the JavaScriptCore C API representation of a JavaScript execution
  context that holds the global object and other execution state.

  JSContexts are created by the JSContextGroup::CreateContext member
  function.

  Scripts may execute concurrently with scripts executing in other
  contexts, and contexts within the same context group may share and
  exchange their JavaScript objects with one another.
  
  When JavaScript objects within the same context group are used in
  multiple threads, explicit synchronization is required.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSContext final	: public detail::JSPerformanceCounter<JSContext> {
#else
class JSContext final {
#endif
	
 public:
	
	/*!
	  @method
	  
	  @abstract Return the global object of this JavaScript execution
	  context.
	  
	  @result The global object of this JavaScript execution context.
	*/
	JSObject get_global_object() const;
	
	/*!
	  @method
	  
	  @abstract Return the context group of this JavaScript execution
	  context.
	  
	  @result The context group of this JavaScript execution context.
	*/
	JSContextGroup get_context_group() const {
		return js_context_group__;
	}

	/*!
	  @method
	  
	  @abstract Create a JavaScript value from parsing a JSON formatted
	  string.
	  
	  @param string The JSString that contains the JSON data to be
	  parsed.
	  
	  @result A JavaScript value containing the result of parsing the
	  JSON data.
	  
	  @throws std::invalid_argument exception if the string isn't a
	  valid JSON formatted string.
	*/
	JSValue CreateValueFromJSON(const JSString& js_string) const ;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the string type with a
	  length of zero.
	  
	  @result A JSValue of the string type with a length of zero.
	*/
	JSValue CreateString() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the string type.
	  
	  @param string The string to assign to the newly created JSValue.
	  
	  @result A JSValue of the string type that represents the value of
	  string.
	*/
	JSValue CreateString(const JSString& js_string) const;
	JSValue CreateString(const char* string)        const;
	JSValue CreateString(const std::string& string) const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the undefined type.
	  
	  @result The unique undefined value.
	*/
	JSUndefined CreateUndefined() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the null type.
	  
	  @result The unique null value.
	*/
	JSNull CreateNull() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the boolean type.
	  
	  @param boolean The bool to assign to the newly created JSBoolean.
	  
    @result A JavaScript value of the boolean type, representing the
    value of boolean.
  */
	JSBoolean CreateBoolean(bool boolean) const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the number type with the
	  value 0.
	  
	  @result A JavaScript value of the number type, representing the
	  value of 0.
	*/
	JSNumber CreateNumber() const;

	/*!
    @method
    
    @abstract Create a JavaScript value of the number type from a
    double.
    
    @param number The double to assign to the newly created JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	JSNumber CreateNumber(double number) const;

  /*!
    @method
    
    @abstract Create a JavaScript value of the number type an int32_t.
    
    @param number The int32_t to assign to the newly created JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	JSNumber CreateNumber(int32_t number) const;

	/*!
    @method
    
    @abstract Create a JavaScript value of the number type from a
    uint32_t.
    
    @param number The uint32_t to assign to the newly created
    JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	JSNumber CreateNumber(uint32_t number) const;

	/*!
	  @method
	  
	  @abstract Create an empty JavaScript object using the default
	  object class.
	  
	  @discussion The default object class does not allocate storage for
	  private data, so you cannot use the GetPrivate and SetPrivate
	  methods when using this constructor.
	  
	  @result An empty JavaScript object.
	*/
	JSObject CreateObject() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript object in this execution context
	  from a custom JSClass and optional private data.
	  
	  @discussion This constructor allocates storage for private data
	  that you can use the GetPrivate and SetPrivate methods to store
	  private data for callbacks.
	  
	  The private data is set on the created object before the intialize
	  callbacks in its class chain are called. This enables the
	  initialize callbacks to retrieve and manipulate the private data
	  through the GetPrivate method.

	  @param js_class The JSClass used to create this object.
	  
	  @param private_data An optional void* to set as the object's
	  private data. Pass nullptr to specify no private data.
	  
	  @result A JavaScript object created from a custom JSClass and
	  optional private data.
	*/
	JSObject CreateObject(const JSClass& js_class, void* private_data = nullptr) const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript object in this execution context
	  that is implemented by a C++ class.
	  
	  @param T Any class that derives from JSNativeObject.

	  @param constructor_arguments The constructor arguments to pass to
	  T.
	  
	  @result A JavaScript object running in this execution context that
	  is implemented by a C++ class.
	*/
	template<typename T, typename... Us>
	T CreateObject(Us&&... constructor_arguments) const;

	/*!
	  @method
	  
	  @abstract Create JavaScript Array object.
	  
    @result A JavaScript object that is an Array.
	*/
	JSArray CreateArray() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript Array object.
	  
	  @discussion The behavior of this constructor does not exactly
	  match the behavior of the built-in Array constructor in that if
	  the vector of arguments contains one element then the JSArray is
	  also created with on element.
	  
	  @param arguments The JavaScript values to populate the array.
	  
    @result A JavaScript object that is an Array, populated with the
    given JavaScript values.
	*/
	JSArray CreateArray(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  
	  @abstract Create JavaScript Date object.
	  
	  @result A JavaScript object of the Date type.
	*/
	JSDate CreateDate() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript Date object, as if by invoking the
	  built-in Date constructor.
	  
	  @param arguments The JavaScript values to pass to the Date
	  Constructor.
	  
    @result A JSObject that is a Date.
	*/
	JSDate CreateDate(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  
	  @abstract Create JavaScript Error object.
	  
	  @result A JavaScript object of the Error type.
	*/
	JSError CreateError() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript Error object, as if by invoking the
	  built-in Error constructor.
	  
	  @param arguments The JavaScript values to pass to the Error
	  Constructor.
	  
    @result A JSObject that is a Error.
	*/
	JSError CreateError(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript RegExp object.
	  
    @result A JavaScript object of the RegExp type.
	*/
	JSRegExp CreateRegExp() const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript RegExp object, as if by invoking the
	  built-in RegExp constructor.
	  
	  @param arguments The JavaScript values to pass to the RegExp
	  Constructor.
	  
    @result A JSObject that is a RegExp.
	*/
	JSRegExp CreateRegExp(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  
	  @abstract Create a JavaScript function whose body is given as a
	  string of JavaScript code. Use this class when you want to execute
	  a script repeatedly to avoid the cost of re-parsing the script
	  before each execution.

	  @param function_name A JSString containing the function's
	  name. This will be used when converting the function to
	  string. Pass an empty string to create an anonymous function.
	  
	  @param parameter_names A JSString array containing the names of
	  the function's parameters.
	  
	  @param body A JSString containing the script to use as the
	  function's body.
	  
	  @param source_url An optional JSString containing a URL for the
	  script's source file. This is only used when reporting exceptions.
	  
	  @param starting_line_number An optional integer value specifying
	  the script's starting line number in the file located at
	  source_url. This is only used when reporting exceptions. The value
	  is one-based, so the first line is line 1 and invalid values are
	  clamped to 1.
	  
	  @result A JSObject that is a function. The object's prototype will
	  be the default function prototype.
	  
	  @throws std::invalid_argument if either body or parameter_names
	  contains a syntax error.
	*/
	JSFunction CreateFunction(const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSString& source_url = JSString(), int starting_line_number = 1) const;


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
	JSValue JSEvaluateScript(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const;

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
	JSValue JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1) const;

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
	bool JSCheckScriptSyntax(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const;
	

	/*!
	  @method
	  
	  @abstract Perform a JavaScript garbage collection.
	  
	  @discussion JavaScript values that are on the machine stack, in a
	  register, protected by JSValueProtect, set as the global object of
	  an execution context, or reachable from any such value will not be
	  collected.
	  
	  During JavaScript execution, you are not required to call this
	  function; the JavaScript engine will garbage collect as
	  needed. JavaScript values created within a context group are
	  automatically destroyed when the last reference to the context
	  group is released.
	*/
	void GarbageCollect() const {
		JSGarbageCollect(js_context_ref__);
	}
	
	/*!
	  @method
	  
	  @abstract FOR DEBUG PURPOSES ONLY: Perform an immediate JavaScript
	  garbage collection.
	  
	  @discussion JavaScript values that are on the machine stack, in a
	  register, protected by JSValueProtect, set as the global object of
	  an execution context, or reachable from any such value will not be
	  collected.
	*/
#ifdef DEBUG
	void SynchronousGarbageCollectForDebugging() const {
		JSSynchronousGarbageCollectForDebugging(js_context_ref__);
	}
#endif

#ifdef JAVASCRIPTCORECPP_JSCONTEXT_ENABLE_CONTEXT_ID
	/*!
	  @method
	  
	  @abstract Return the unique ID of this JavaScript execution
	  context.
	  
	  @result The unique ID of this JavaScript execution context.
	*/
	long get_context_id() const {
		return js_context_id_;
	}
#endif

	~JSContext() {
		JSGlobalContextRelease(*this);
	}
	
	// Copy constructor.
	JSContext(const JSContext& rhs)
			: js_context_group__(rhs.js_context_group__)
			, js_context_ref__(rhs.js_context_ref__) {
		JSGlobalContextRetain(*this);
	}
	
  // Move constructor.
	JSContext(JSContext&& rhs)
			: js_context_group__(rhs.js_context_group__)
			, js_context_ref__(rhs.js_context_ref__) {
	}
	
#ifdef JAVASCRIPTCORECPP_MOVE_SEMANTICS_ENABLE
  JSContext& JSContext::operator=(const JSContext&) = default;
  JSContext& JSContext::operator=(JSContext&&) = default;
#endif

  // Create a copy of another JSContext by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator, X& X::operator=(const X&), and the move assignment
  // operator, X& X::operator=(X&&);
  JSContext& operator=(JSContext rhs) {
	  JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	  swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSContext& first, JSContext& second) noexcept {
	  JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_context_group__, second.js_context_group__);
    swap(first.js_context_ref__  , second.js_context_ref__);
  }

private:
  
  explicit JSContext(const JSContextGroup& js_context_group, const JSClass& global_object_class = {});
  
  // For interoperability with the JavaScriptCore C API.
  explicit JSContext(JSContextRef js_context_ref);
		
	// For interoperability with the JavaScriptCore C API.
  operator JSContextRef() const {
		return js_context_ref__;
	}

	// For interoperability with the JavaScriptCore C API.
	operator JSGlobalContextRef() const {
		return JSContextGetGlobalContext(js_context_ref__);
	}

  // Only a JSContextGroup can create a JSContext.
	friend class JSContextGroup;

	// JSValue needs access to operator JSContextRef().
	friend class JSValue;
	
	// The JSUndefined constructor needs access to operator
	// JSContextRef().
	friend class JSUndefined;
	
	// The JSNull constructor needs access to operator JSContextRef().
	friend class JSNull;

	// JSBoolean::operator bool() needs access to operator
	// JSContextRef().
	friend class JSBoolean;
	
	// The JSNumber constructor needs access to operator JSContextRef().
	friend class JSNumber;

	// The JSObject constructor needs access to operator JSContextRef().
	friend class JSObject;
	
	// The JSArray constructor needs access to operator JSContextRef().
	friend class JSArray;

	// The JSDate constructor needs access to operator JSContextRef().
	friend class JSDate;

	// The JSError constructor needs access to operator JSContextRef().
	friend class JSError;

	// The JSRegExp constructor needs access to operator JSContextRef().
	friend class JSRegExp;

	// The JSFunction constructor needs access to operator
	// JSContextRef().
	friend class JSFunction;

	// The JSPropertyNameArray constructor needs access to operator
	// JSContextRef().
	friend class detail::JSPropertyNameArray;

	// The JSNativeClass static functions need access to the JSContext
	// constructor.
	template<typename T>
	friend class JSNativeClass;
	
	// JSNativeClass need access to operator JSContextRef().
	template<typename T>
	friend class JSNativeObject;
	
	// Return true if the two JSContexts are equal.
  friend bool operator==(const JSContext& lhs, const JSContext& rhs);

  // This function requires access to operator JSContextRef().
  friend bool operator==(const JSValue& lhs, const JSValue& rhs);


  // Return true if the two JSValues are equal as compared by the JS == operator.
  friend bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);
  
  JSContextGroup js_context_group__;
  JSContextRef   js_context_ref__ { nullptr };
  JAVASCRIPTCORECPP_JSCONTEXT_MUTEX;
};

// Return true if the two JSContexts are equal.
inline
bool operator==(const JSContext& lhs, const JSContext& rhs) {
	return (lhs.js_context_ref__ == rhs.js_context_ref__);
}
  
// Return true if the two JSContextGroups are not equal.
inline
bool operator!=(const JSContext& lhs, const JSContext& rhs) {
  return ! (lhs == rhs);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_JSCONTEXT_HPP_

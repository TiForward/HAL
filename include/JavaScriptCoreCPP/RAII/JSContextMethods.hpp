/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSCONTEXTMETHODS_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSCONTEXTMETHODS_HPP_

#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <vector>
#include <JavaScriptCore/JavaScript.h>

#ifdef DEBUG
extern "C" JS_EXPORT void JSSynchronousGarbageCollectForDebugging(JSContextRef);
#endif

namespace JavaScriptCoreCPP { namespace RAII {

class JSContextGroup;
class JSClass;

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
  
  @discussion A JSContextMethods is an abstract base class the specifies
  the JSValue creation methods of JSContext and JSValue.
*/
class JSContextMethods {
	
 public:

	virtual ~JSContextMethods() {
	}

	/*!
	  @method
	  
	  @abstract Return the global object of this JavaScript execution
	  context.
	  
	  @result The global object of this JavaScript execution context.
	*/
	virtual JSObject get_global_object() const = 0;
	
	/*!
	  @method
	  
	  @abstract Return the context group of this JavaScript execution
	  context.
	  
	  @result The context group of this JavaScript execution context.
	*/
	virtual JSContextGroup get_context_group() const = 0;

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
	virtual JSValue CreateValueFromJSON(const JSString& js_string) const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the string type with a
	  length of zero.
	  
	  @result A JSValue of the string type with a length of zero.
	*/
	virtual JSValue CreateString() const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the string type.
	  
	  @param string The string to assign to the newly created JSValue.
	  
	  @result A JSValue of the string type that represents the value of
	  string.
	*/
	virtual JSValue CreateString(const JSString& js_string) const = 0;
	virtual JSValue CreateString(const char* string)        const = 0;
	virtual JSValue CreateString(const std::string& string) const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the undefined type.
	  
	  @result The unique undefined value.
	*/
	virtual JSUndefined CreateUndefined() const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the null type.
	  
	  @result The unique null value.
	*/
	virtual JSNull CreateNull() const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the boolean type.
	  
	  @param boolean The bool to assign to the newly created JSBoolean.
	  
    @result A JavaScript value of the boolean type, representing the
    value of boolean.
  */
	virtual JSBoolean CreateBoolean(bool boolean) const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript value of the number type with the
	  value 0.
	  
	  @result A JavaScript value of the number type, representing the
	  value of 0.
	*/
	virtual JSNumber CreateNumber() const = 0;

	/*!
    @method
    
    @abstract Create a JavaScript value of the number type from a
    double.
    
    @param number The double to assign to the newly created JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	virtual JSNumber CreateNumber(double number) const = 0;

  /*!
    @method
    
    @abstract Create a JavaScript value of the number type an int32_t.
    
    @param number The int32_t to assign to the newly created JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	virtual JSNumber CreateNumber(int32_t number) const = 0;

  /*!
    @method
    
    @abstract Create a JavaScript value of the number type from a
    uint32_t.
    
    @param number The uint32_t to assign to the newly created
    JSNumber.
    
    @result A JavaScript value of the number type, representing the
    value of number.
  */
	virtual JSNumber CreateNumber(uint32_t number) const = 0;

	/*!
	  @method
	  
	  @abstract Create an empty JavaScript object using the default
	  object class.
	  
	  @discussion The default object class does not allocate storage for
	  private data, so you cannot use the GetPrivate and SetPrivate
	  methods when using this constructor.
	  
	  @result An empty JavaScript object.
	*/
	virtual JSObject CreateObject() const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript object from a custom JSClass and
	  optional private data.
	  
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
	virtual JSObject CreateObject(const JSClass& js_class, void* private_data = nullptr) const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript object defined by a JSNativeClass
	  that is implemented by a C++ object.
	  
	  @param js_native_class The JSNativeClass used to create this
	  object.
	  
	  @param T_constructor_arguments The constructor arguments to use
	  when creating the type T.
	  
	  @result A JavaScript object defined by a JSNativeClass that is
	  implemented by a C++ object.
	*/
	// template<typename T, typename... Us>
	// T CreateObject(const JSNativeClass<T>& js_native_class, Us&&... T_constructor_arguments) const;
	
	/*!
	  @method
	  
	  @abstract Create JavaScript Array object.
	  
    @result A JavaScript object that is an Array.
	*/
	virtual JSArray CreateArray() const = 0;
	
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
	virtual JSArray CreateArray(const std::vector<JSValue>& arguments) const = 0;

	/*!
	  @method
	  
	  @abstract Create JavaScript Date object.
	  
	  @result A JavaScript object of the Date type.
	*/
	virtual JSDate CreateDate() const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript Date object, as if by invoking the
	  built-in Date constructor.
	  
	  @param arguments The JavaScript values to pass to the Date
	  Constructor.
	  
    @result A JSObject that is a Date.
	*/
	virtual JSDate CreateDate(const std::vector<JSValue>& arguments) const = 0;
	
	/*!
	  @method
	  
	  @abstract Create JavaScript Error object.
	  
	  @result A JavaScript object of the Error type.
	*/
	virtual JSError CreateError() const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript Error object, as if by invoking the
	  built-in Error constructor.
	  
	  @param arguments The JavaScript values to pass to the Error
	  Constructor.
	  
    @result A JSObject that is a Error.
	*/
	virtual JSError CreateError(const std::vector<JSValue>& arguments) const = 0;

	/*!
	  @method
	  
	  @abstract Create a JavaScript RegExp object.
	  
    @result A JavaScript object of the RegExp type.
	*/
	virtual JSRegExp CreateRegExp() const = 0;
	
	/*!
	  @method
	  
	  @abstract Create a JavaScript RegExp object, as if by invoking the
	  built-in RegExp constructor.
	  
	  @param arguments The JavaScript values to pass to the RegExp
	  Constructor.
	  
    @result A JSObject that is a RegExp.
	*/
	virtual JSRegExp CreateRegExp(const std::vector<JSValue>& arguments) const = 0;

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
	virtual JSFunction CreateFunction(const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSString& source_url = JSString(), int starting_line_number = 1) const = 0;

	
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
	virtual JSValue JSEvaluateScript(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const = 0;
	
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
	virtual JSValue JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1) const = 0;
	
	
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
	virtual bool JSCheckScriptSyntax(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const = 0;
	
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
	virtual void GarbageCollect() const  = 0;
	
	/*!
	  @method
	  
	  @abstract FOR DEBUG PURPOSES ONLY: Perform an immediate JavaScript
	  garbage collection.
	  
	  @discussion JavaScript values that are on the machine stack, in a
	  register, protected by JSValueProtect, set as the global object of
	  an execution context, or reachable from any such value will not be
	  collected.
	*/
	virtual void SynchronousGarbageCollectForDebugging() const = 0;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSCONTEXTMETHODS_HPP_

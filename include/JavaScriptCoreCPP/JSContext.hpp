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

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSContextGroup.hpp"

#include <vector>

namespace JavaScriptCoreCPP {
  
  class JSClass;
  class JSString;
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
  
  namespace detail {
    template<typename T>
    class JSExportClass;
    
    std::vector<JSValue> to_vector(const JSContext&, size_t, const JSValueRef[]);
  }}

namespace JavaScriptCoreCPP {
  
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
  class JSContext final JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSContext) {
    
  public:
    
    /*!
     @method
     
     @abstract Return the global object of this JavaScript execution
     context.
     
     @result The global object of this JavaScript execution context.
     */
    JSObject get_global_object() const noexcept;
    
    /*!
     @method
     
     @abstract Return the context group of this JavaScript execution
     context.
     
     @result The context group of this JavaScript execution context.
     */
    JSContextGroup get_context_group() const noexcept {
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
    JSValue CreateValueFromJSON(const JSString& js_string) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the string type.
     
     @param string The string to assign to the newly created JSValue.
     
     @result A JSValue of the string type that represents the value of
     string.
     */
    JSValue CreateString()                          const noexcept;
    JSValue CreateString(const JSString& js_string) const noexcept;
    JSValue CreateString(const char*        string) const noexcept;
    JSValue CreateString(const std::string& string) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the undefined type.
     
     @result The unique undefined value.
     */
    JSUndefined CreateUndefined() const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the null type.
     
     @result The unique null value.
     */
    JSNull CreateNull() const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the boolean type.
     
     @param boolean The bool to pass to the JSBoolean constructor.
     
     @result A JavaScript value of the boolean type, representing the
     value of boolean.
     */
    JSBoolean CreateBoolean(bool boolean) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the number type from a
     double.
     
     @param number The double to pass to the JSNumber constructor.
     
     @result A JavaScript value of the number type, representing the
     value of number.
     */
    JSNumber CreateNumber(double number) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the number type an
     int32_t.
     
     @param number The int32_t to pass to the JSNumber constructor.
     
     @result A JavaScript value of the number type, representing the
     value of number.
     */
    JSNumber CreateNumber(int32_t number) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript value of the number type from a
     uint32_t.
     
     @param number The uint32_t to pass to the JSNumber constructor.
     
     @result A JavaScript value of the number type, representing the
     value of number.
     */
    JSNumber CreateNumber(uint32_t number) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript object in this execution
     context. An empty JavaScript object is returned if you do not
     provide a JSClass.
     
     @param js_class An optional custom JSClass to pass to the
     JSObject constructor.
     
     @result A JavaScript object.
     */
    JSObject CreateObject() const noexcept;
    JSObject CreateObject(const JSClass& js_class) const noexcept;
    
    /*!
     @method
     
     @abstract Create a JavaScript object in this execution context
     that is implemented by a C++ class.
     
     @param T Any class that derives from JSNativeObject.
     
     @param constructor_arguments The constructor arguments to pass to
     T.
     
     @result A JavaScript object running in this execution context
     that is implemented by a C++ class.
     */
    template<typename T, typename... Us>
    std::shared_ptr<T> CreateObject(Us&&... constructor_arguments) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript Array object.
     
     @discussion The behavior of this constructor does not exactly
     match the behavior of the built-in Array constructor in that if
     the vector of arguments contains one element then the JSArray is
     also created with on element.
     
     @param arguments Optional JavaScript values to populate the
     array. Otherwise an empty array is created.
     
     @result A JavaScript object that is an Array, populated with the
     given JavaScript values.
     */
    JSArray CreateArray() const noexcept;
    JSArray CreateArray(const std::vector<JSValue>& arguments) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript Date object, as if by invoking the
     built-in Date constructor.
     
     @param arguments Optional JavaScript values to pass to the Date
     Constructor.
     
     @result A JSObject that is a Date.
     */
    JSDate CreateDate() const noexcept;
    JSDate CreateDate(const std::vector<JSValue>& arguments) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript Error object, as if by invoking the
     built-in Error constructor.
     
     @param arguments Optional JavaScript values to pass to the Error
     Constructor.
     
     @result A JSObject that is a Error.
     */
    JSError CreateError() const noexcept;
    JSError CreateError(const std::vector<JSValue>& arguments) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript RegExp object, as if by invoking
     the built-in RegExp constructor.
     
     @param arguments Optional JavaScript values to pass to the RegExp
     Constructor.
     
     @result A JSObject that is a RegExp.
     */
    JSRegExp CreateRegExp() const noexcept;
    JSRegExp CreateRegExp(const std::vector<JSValue>& arguments) const;
    
    /*!
     @method
     
     @abstract Create a JavaScript function whose body is given as a
     string of JavaScript code. Use this method when you want to
     execute a script repeatedly to avoid the cost of re-parsing the
     script before each execution.
     
     @param body A JSString containing the script to use as the
     function's body.
     
     @param parameter_names An optional JSString array containing the
     names of the function's parameters.
     
     @param function_name An optional JSString containing the
     function's name. This will be used when converting the function
     to a string. An empty string creates an anonymous function.
     
     @param source_url An optional JSString containing a URL for the
     script's source file. This is only used when reporting
     exceptions.
     
     @param starting_line_number An optional integer value specifying
     the script's starting line number in the file located at
     source_url. This is only used when reporting exceptions. The
     value is one-based, so the first line is line 1 and invalid
     values are clamped to 1.
     
     @result A JSObject that is a function. The object's prototype
     will be the default function prototype.
     
     @throws std::invalid_argument if either body, function_name or
     parameter_names contains a syntax error.
     */
    JSFunction CreateFunction(const JSString& body) const;
    JSFunction CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names) const;
    JSFunction CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name) const;
    JSFunction CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name, const JSString& source_url, int starting_line_number = 1) const;
    
    
    /* Script Evaluation */
    
    /*!
     @method
     
     @abstract Evaluate a string of JavaScript code.
     
     @param script A JSString containing the script to evaluate.
     
     @param this_object An optional JavaScript object to use as
     "this". The default is the global object.
     
     @param source_url An optional JSString containing a URL for the
     script's source file. This is used by debuggers and when
     reporting exceptions.
     
     @param starting_line_number An optional integer value specifying
     the script's starting line number in the file located at
     source_url. This is only used when reporting exceptions. The
     value is one-based, so the first line is line 1 and invalid
     values are clamped to 1.
     
     @result The JSValue that results from evaluating script.
     
     @throws std::runtime_error exception if the evaluated script
     threw an exception.
     
     */
    JSValue JSEvaluateScript(const JSString& script                                                                                ) const;
    JSValue JSEvaluateScript(const JSString& script, JSObject this_object                                                          ) const;
    JSValue JSEvaluateScript(const JSString& script,                       const JSString& source_url, int starting_line_number = 1) const;
    JSValue JSEvaluateScript(const JSString& script, JSObject this_object, const JSString& source_url, int starting_line_number = 1) const;
    
    /*!
     @method
     
     @abstract Check for syntax errors in a string of JavaScript.
     
     @param script A JSString containing the script to check for
     syntax errors.
     
     @param source_url An optional JSString containing a URL for the
     script's source file. This is used by debuggers and when
     reporting exceptions.
     
     @param starting_line_number An optional integer value specifying
     the script's starting line number in the file located at
     source_url. This is only used when reporting exceptions. The
     value is one-based, so the first line is line 1 and invalid
     values are clamped to 1.
     
     @result true if the script is syntactically correct, otherwise
     false.
     */
    bool JSCheckScriptSyntax(const JSString& script) const noexcept;
    bool JSCheckScriptSyntax(const JSString& script, const JSString& source_url, int starting_line_number = 1) const noexcept;
    
    
    /*!
     @method
     
     @abstract Perform a JavaScript garbage collection.
     
     @discussion JavaScript values that are on the machine stack, in a
     register, protected by JSValueProtect, set as the global object
     of an execution context, or reachable from any such value will
     not be collected.
     
     During JavaScript execution, you are not required to call this
     function; the JavaScript engine will garbage collect as
     needed. JavaScript values created within a context group are
     automatically destroyed when the last reference to the context
     group is released.
     */
    void GarbageCollect() const noexcept;
    
    /*!
     @method
     
     @abstract FOR DEBUG PURPOSES ONLY: Perform an immediate
     JavaScript garbage collection.
     
     @discussion JavaScript values that are on the machine stack, in a
     register, protected by JSValueProtect, set as the global object
     of an execution context, or reachable from any such value will
     not be collected.
     */
#ifdef DEBUG
    void SynchronousGarbageCollectForDebugging() const;
#endif
    
    JSContext() = delete;
    ~JSContext() noexcept;
    JSContext(const JSContext&) noexcept;
    JSContext(JSContext&&) noexcept;
    JSContext& operator=(const JSContext&) noexcept;
    JSContext& operator=(JSContext&&) noexcept;
    void swap(JSContext&) noexcept;
    
  private:
    
    // Only a JSContextGroup create a JSContext using the following
    // constructor.
    friend class JSContextGroup;
    
    JSContext(const JSContextGroup& js_context_group, const JSClass& global_object_class) noexcept;
    
    // These classes and functions need access to operator
    // JSContextRef().
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
    friend class JSPropertyNameArray;
    
    friend bool operator==(const JSValue& lhs, const JSValue& rhs) noexcept;
    
    friend std::vector<JSValue> detail::to_vector(const JSContext&, size_t, const JSValueRef[]);
    
    // For interoperability with the JavaScriptCore C API.
    operator JSContextRef() const noexcept {
      return js_global_context_ref__;
    }
    
    // Only the JSExportClass static functions create a
    // JSContext using the following constructor.
    template<typename T>
    friend class detail::JSExportClass;
    
    explicit JSContext(JSContextRef js_context_ref) noexcept;
    
    // For interoperability with the JavaScriptCore C API.
    explicit JSContext(JSGlobalContextRef js_global_context_ref__) noexcept;
    
    // Prevent heap based objects.
    static void * operator new(std::size_t);       // #1: To prevent allocation of scalar objects
    static void * operator new [] (std::size_t);   // #2: To prevent allocation of array of objects
    
    friend bool operator==(const JSContext& lhs, const JSContext& rhs);
    
    JSContextGroup     js_context_group__;
    JSGlobalContextRef js_global_context_ref__ { nullptr };
    
#undef  JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD
#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
    std::recursive_mutex mutex__;
#define JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
  inline
  void swap(JSContext& first, JSContext& second) noexcept {
    first.swap(second);
  }
  
  // Return true if the two JSContexts are equal.
  inline
  bool operator==(const JSContext& lhs, const JSContext& rhs) {
    return (lhs.js_global_context_ref__ == rhs.js_global_context_ref__);
  }
  
  // Return true if the two JSContextGroups are not equal.
  inline
  bool operator!=(const JSContext& lhs, const JSContext& rhs) {
    return ! (lhs == rhs);
  }
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCONTEXT_HPP_

/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

#include "JavaScriptCoreCPP/RAII/JSContextMethods.hpp"
#include "JavaScriptCoreCPP/RAII/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <atomic>
#include <cassert>


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#undef  JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_TYPE
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME_PREFIX js_context

#undef  JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_TYPE JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME;

#undef  JAVASCRIPTCORECPP_RAII_JSCONTEXT_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_TYPE> JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSCONTEXT_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


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
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSContext final	: public JSContextMethods, public detail::JSPerformanceCounter<JSContext> {
#else
class JSContext final	: public JSContextMethods {
#endif
	
 public:
	
	virtual JSObject get_global_object() const override final;
	
	virtual JSContextGroup get_context_group() const  override final {
		return js_context_group__;
	}

	// Complete documentation of these methods is in
	// JSContextMethods.hpp.
	virtual JSValue     CreateValueFromJSON(const JSString& js_string) const override final;
	virtual JSValue     CreateString() const override final;
	virtual JSValue     CreateString(const JSString& js_string) const override final;
	virtual JSValue     CreateString(const char* string)        const override final;
	virtual JSValue     CreateString(const std::string& string) const override final;
	virtual JSUndefined CreateUndefined() const override final;
	virtual JSNull      CreateNull() const override final;
	virtual JSBoolean   CreateBoolean(bool boolean) const override final;
	virtual JSNumber    CreateNumber() const override final;
	virtual JSNumber    CreateNumber(double number) const override final;
	virtual JSNumber    CreateNumber(int32_t number) const override final;
	virtual JSNumber    CreateNumber(uint32_t number) const override final;
	virtual JSObject    CreateObject() const override final;
	virtual JSObject    CreateObject(const JSClass& js_class, void* private_data = nullptr) const override final;
	virtual JSArray     CreateArray() const override final;
	virtual JSArray     CreateArray(const std::vector<JSValue>& arguments) const override final;
	virtual JSDate      CreateDate() const override final;
	virtual JSDate      CreateDate(const std::vector<JSValue>& arguments) const override final;
	virtual JSError     CreateError() const override final;
	virtual JSError     CreateError(const std::vector<JSValue>& arguments) const override final;
	virtual JSRegExp    CreateRegExp() const override final;
	virtual JSRegExp    CreateRegExp(const std::vector<JSValue>& arguments) const override final;
	virtual JSFunction  CreateFunction(const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSString& source_url = JSString(), int starting_line_number = 1) const override final;
	virtual JSValue     JSEvaluateScript(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const override final;
	virtual JSValue     JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url = JSString(), int starting_line_number = 1) const override final;
	virtual bool        JSCheckScriptSyntax(const JSString& script, const JSString& source_url = JSString(), int starting_line_number = 1) const override final;

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
	template<typename T, typename... Us>
	T CreateObject(const JSNativeClass<T>& js_native_class, Us&&... T_constructor_arguments) const;
	

	virtual void GarbageCollect() const  override final {
		JSGarbageCollect(js_context_ref__);
	}
	
#ifdef DEBUG
	virtual void SynchronousGarbageCollectForDebugging() const {
		JSSynchronousGarbageCollectForDebugging(js_context_ref__);
	}
#endif

#ifdef JAVASCRIPTCORECPP_RAII_JSCONTEXT_ENABLE_CONTEXT_ID
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
	
#ifdef JAVASCRIPTCORECPP_RAII_MOVE_SEMANTICS_ENABLE
  JSContext& JSContext::operator=(const JSContext&) = default;
  JSContext& JSContext::operator=(JSContext&&) = default;
#endif

  // Create a copy of another JSContext by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator, X& X::operator=(const X&), and the move assignment
  // operator, X& X::operator=(X&&);
  JSContext& operator=(JSContext rhs) {
	  JAVASCRIPTCORECPP_RAII_JSCONTEXT_LOCK_GUARD;
	  swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSContext& first, JSContext& second) noexcept {
	  JAVASCRIPTCORECPP_RAII_JSCONTEXT_LOCK_GUARD;
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
  JAVASCRIPTCORECPP_RAII_JSCONTEXT_MUTEX;
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

#endif // _JAVASCRIPTCORECPP_RAII_JSCONTEXT_HPP_

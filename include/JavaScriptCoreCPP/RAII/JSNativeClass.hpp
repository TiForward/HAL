/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_HPP_

#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <limits>
#include <cassert>

#include <JavaScriptCore/JavaScript.h>


#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_THREAD_SAFE


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#unndef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_TYPE
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_TYPE std::recursive_mutex

#unndef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_NAME 
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_NAME js_native_class

#undef  JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_TYPE JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_NAME##_mutex_;


#undef  JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_TYPE> JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX_NAME##_lock(js_native_class_ptr -> JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX);

#else
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace RAII {

using namespace JavaScriptCoreCPP::detail;

template<typename T>
using JSNativeObjectValuePropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>;

template<typename T>
using JSNativeObjectFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>>;


template<typename T>
class JSNativeClass;

template<typename T>
using JSNativeClass_shared_ptr = std::shared_ptr<JSNativeClass<T>>;

template<typename T>
using JSNativeClass_shared_ptr_map_t = std::unordered_map<std::intptr_t, JSNativeClass_shared_ptr<T>>;

template<typename T>
class JSNativeClassBuilder;

/*!
  @class

  @discussion A JSNativeClass is an RAII wrapper around a JSClassRef,
  the JavaScriptCore C API representation of a JavaScript class that
  defines JavaScript objects implemented by a instances of a C++
  class.

  An instance of JSNativeClass may be passed to the JSContextGroup
  constructor to create a custom global object for all contexts in
  that group.

  JavaScript objects based on a JSNativeClass may be created by the
  JSContext::CreateObject member function.

  Only a JSNativeClassBuilder may create instances of a JSNativeClass.
*/
template<typename T>
class JSNativeClass final : public JSClass {
	
 public:
	
	JSNativeClass(const JSNativeClassBuilder<T>& builder);
	
	~JSNativeClass() {
	}

	// Copy constructor.
	JSNativeClass(const JSNativeClass& rhs)
			: JSClass(rhs)
			, class_name_(rhs.class_name_)
			, class_name_for_js_class_definition_(rhs.class_name_for_js_class_definition_)
			, attributes_(rhs.attributes_)
			, parent_(rhs.parent_)
			, value_property_callback_map_(rhs.value_property_callback_map_)
			, function_property_callback_map_(rhs.function_property_callback_map_)
			, initialize_callback_(rhs.initialize_callback_)
			, finalize_callback_(rhs.finalize_callback_)
			, has_property_callback_(rhs.has_property_callback_)
			, get_property_callback_(rhs.get_property_callback_)
			, set_property_callback_(rhs.set_property_callback_)
			, delete_property_callback_(rhs.delete_property_callback_)
			, get_property_names_callback_(rhs.get_property_names_callback_)
			, call_as_function_callback_(rhs.call_as_function_callback_)
			, call_as_constructor_callback_(rhs.call_as_constructor_callback_)
			, has_instance_callback_(rhs.has_instance_callback_)
			, convert_to_type_callback_(rhs.convert_to_type_callback_)
			, js_class_definition_(kJSClassDefinitionEmpty) {
		
		InitializeJSStaticValueVector();
		InitializeJSStaticFunctionVector();
		InitializeJSClassDefinition();

		// Replace the private JSClassRef member in the base class.
		JSClassRelease(js_class_ref__);
		js_class_ref__ = JSClassCreate(&js_class_definition_);
	}
	
	// Move constructor.
	JSNativeClass(JSNativeClass&& rhs)
			: JSClass(rhs)
			, class_name_(rhs.class_name_)
			, class_name_for_js_class_definition_(rhs.class_name_for_js_class_definition_)
			, attributes_(rhs.attributes_)
			, parent_(rhs.parent_)
			, value_property_callback_map_(rhs.value_property_callback_map_)
			, function_property_callback_map_(rhs.function_property_callback_map_)
			, initialize_callback_(rhs.initialize_callback_)
			, finalize_callback_(rhs.finalize_callback_)
			, has_property_callback_(rhs.has_property_callback_)
			, get_property_callback_(rhs.get_property_callback_)
			, set_property_callback_(rhs.set_property_callback_)
			, delete_property_callback_(rhs.delete_property_callback_)
			, get_property_names_callback_(rhs.get_property_names_callback_)
			, call_as_function_callback_(rhs.call_as_function_callback_)
			, call_as_constructor_callback_(rhs.call_as_constructor_callback_)
			, has_instance_callback_(rhs.has_instance_callback_)
			, convert_to_type_callback_(rhs.convert_to_type_callback_)
			, js_static_values_(rhs.js_static_values_)
			, js_static_functions_(rhs.js_static_functions_)
			, js_class_definition_(rhs.js_class_definition_) {
	}
  
#ifdef JAVASCRIPTCORECPP_RAII_MOVE_SEMANTICS_ENABLE
  JSNativeClass& JSNativeClass::operator=(const JSNativeClass&) = default;
  JSNativeClass& JSNativeClass::operator=(JSNativeClass&&) = default;
#endif

  // Create a copy of another JSNativeClass by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
	JSNativeClass& operator=(JSNativeClass rhs) {
		JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD;
		JSClass::operator=(rhs);
    swap(*this, rhs);
    return *this;
  }
  
	friend void swap(JSNativeClass& first, JSNativeClass& second) noexcept {
		JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD;
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(first.class_name_                        , second.class_name_);
    swap(first.class_name_for_js_class_definition_, second.class_name_for_js_class_definition_);
    swap(first.attributes_                        , second.attributes_);
    swap(first.parent_                            , second.parent_);
    swap(first.value_property_callback_map_       , second.value_property_callback_map_);
    swap(first.function_property_callback_map_    , second.function_property_callback_map_);
    swap(first.initialize_callback_               , second.initialize_callback_);
    swap(first.finalize_callback_                 , second.finalize_callback_);
    swap(first.has_property_callback_             , second.has_property_callback_);
    swap(first.get_property_callback_             , second.get_property_callback_);
    swap(first.set_property_callback_             , second.set_property_callback_);
    swap(first.delete_property_callback_          , second.delete_property_callback_);
    swap(first.get_property_names_callback_       , second.get_property_names_callback_);
    swap(first.call_as_function_callback_         , second.call_as_function_callback_);
    swap(first.call_as_constructor_callback_      , second.call_as_constructor_callback_);
    swap(first.has_instance_callback_             , second.has_instance_callback_);
    swap(first.convert_to_type_callback_          , second.convert_to_type_callback_);
    swap(first.js_static_values_                  , second.js_static_values_);
    swap(first.js_static_functions_               , second.js_static_functions_);
    swap(first.js_class_definition_               , second.js_class_definition_);
}

private:

	void InitializeJSStaticValueVector() {
		if (!value_property_callback_map_.empty()) {
			for (const auto& entry : value_property_callback_map_) {
				const auto& property_name           = entry.first;
				const auto& value_property_callback = entry.second;
				JSStaticValue js_static_value;
				js_static_value.name        = property_name.c_str();
				js_static_value.getProperty = JSNativeClass<T>::JSStaticValueGetPropertyCallback;
				js_static_value.setProperty = JSNativeClass<T>::JSStaticValueSetPropertyCallback;
				js_static_value.attributes  = ToJSPropertyAttributes(value_property_callback.get_attributes());
				js_static_values_.push_back(js_static_value);
			}
			js_static_values_.push_back({nullptr, nullptr, nullptr, 0});
		}
	}
	
	void InitializeJSStaticFunctionVector() {
		if (!function_property_callback_map_.empty()) {
			for (const auto& entry : function_property_callback_map_) {
				const auto& function_name              = entry.first;
				const auto& function_property_callback = entry.second;
				JSStaticFunction js_static_function;
				js_static_function.name           = function_name.c_str();
				js_static_function.callAsFunction = JSStaticFunctionCallAsFunctionCallback;
				js_static_function.attributes     = ToJSPropertyAttributes(function_property_callback.get_attributes());
				js_static_functions_.push_back(js_static_function);
			}
			js_static_functions_.push_back({nullptr, nullptr, 0});
		}
	}
	
	void InitializeJSClassDefinition() {
		!class_name_.empty()                     && (js_class_definition_.className         = class_name_for_js_class_definition_.c_str());
		!attributes_.empty()                     && (js_class_definition_.attributes        = ToJSClassAttributes(attributes_));
		parent_                                  && (js_class_definition_.parentClass       = parent_);
		!value_property_callback_map_.empty()    && (js_class_definition_.staticValues      = &js_static_values_[0]);
		!function_property_callback_map_.empty() && (js_class_definition_.staticFunctions   = &js_static_functions_[0]);
		initialize_callback_                     && (js_class_definition_.initialize        = JSNativeClass<T>::JSObjectInitializeCallback);
		finalize_callback_                       && (js_class_definition_.finalize          = JSNativeClass<T>::JSObjectFinalizeCallback);
		// has_property_callback_                   && (js_class_definition_.hasProperty       = JSNativeClass<T>::JSObjectHasPropertyCallback);
		// get_property_callback_                   && (js_class_definition_.getProperty       = JSNativeClass<T>::JSObjectGetPropertyCallback);
		// set_property_callback_                   && (js_class_definition_.setProperty       = JSNativeClass<T>::JSObjectSetPropertyCallback);
		// delete_property_callback_                && (js_class_definition_.deleteProperty    = JSNativeClass<T>::JSObjectDeletePropertyCallback);
		// get_property_names_callback_             && (js_class_definition_.getPropertyNames  = JSNativeClass<T>::JSObjectGetPropertyNamesCallback);
		// call_as_function_callback_               && (js_class_definition_.callAsFunction    = JSNativeClass<T>::JSObjectCallAsFunctionCallback);
		// call_as_constructor_callback_            && (js_class_definition_.callAsConstructor = JSNativeClass<T>::JSObjectCallAsConstructorCallback);
		// has_instance_callback_                   && (js_class_definition_.hasInstance       = JSNativeClass<T>::JSObjectHasInstanceCallback);
		// convert_to_type_callback_                && (js_class_definition_.convertToType     = JSNativeClass<T>::JSObjectConvertToTypeCallback);
	}

	JSString                                       class_name_;
	std::string                                    class_name_for_js_class_definition_;
	std::unordered_set<JSNativeClassAttribute>     attributes_;
	JSClass                                        parent_;
	JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;
	InitializeCallback<T>                          initialize_callback_          { nullptr };
	FinalizeCallback<T>                            finalize_callback_            { nullptr };
	HasPropertyCallback<T>                         has_property_callback_        { nullptr };
	GetPropertyCallback<T>                         get_property_callback_        { nullptr };
	SetPropertyCallback<T>                         set_property_callback_        { nullptr };
	DeletePropertyCallback<T>                      delete_property_callback_     { nullptr };
	GetPropertyNamesCallback<T>                    get_property_names_callback_  { nullptr };
	CallAsFunctionCallback<T>                      call_as_function_callback_    { nullptr };
	CallAsConstructorCallback<T>                   call_as_constructor_callback_ { nullptr };
	HasInstanceCallback<T>                         has_instance_callback_        { nullptr };
	ConvertToTypeCallback<T>                       convert_to_type_callback_     { nullptr };
	
	// For interoperability with the JavaScriptCore C API.
	std::vector<JSStaticValue>                     js_static_values_;
	std::vector<JSStaticFunction>                  js_static_functions_;
	JSClassDefinition                              js_class_definition_;
	std::string                                    js_native_class_identifier_ { "TODO" }; // TODO

	static JSNativeClass_shared_ptr<T> get_JSNativeClass_shared_ptr(const JSObject& js_object);
	
	// For interoperability with the JavaScriptCore C API.
	static JSNativeClass_shared_ptr_map_t<T> native_ptr_to_js_native_class_ptr_map_;
	static std::mutex                        native_ptr_to_js_native_class_ptr_map_mutex_;

	// Support for JSStaticValue
	static JSValueRef  JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool        JSStaticValueSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception);
	
	// Support for JSStaticFunction
	static JSValueRef  JSStaticFunctionCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	
	// Remaining callbacks.
	static void        JSObjectInitializeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref);
	static void        JSObjectFinalizeCallback(JSObjectRef js_object_ref);
	// static bool        JSObjectHasPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name);
	// static JSValueRef  JSObjectGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	// static bool        JSObjectSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception);
	// static bool        JSObjectDeletePropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	// static void        JSObjectGetPropertyNamesCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSPropertyNameAccumulatorRef property_names);
	// static JSValueRef  JSObjectCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	// static JSObjectRef JSObjectCallAsConstructorCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	// static bool        JSObjectHasInstanceCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance, JSValueRef* exception);
	// static JSValueRef  JSObjectConvertToTypeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSType js_type, JSValueRef* exception);
};

template<typename T>
JSNativeClass_shared_ptr_map_t<T> JSNativeClass<T>::native_ptr_to_js_native_class_ptr_map_;

template<typename T>
std::mutex JSNativeClass<T>::native_ptr_to_js_native_class_ptr_map_mutex_;

template<typename T>
JSNativeClass_shared_ptr<T> JSNativeClass<T>::get_JSNativeClass_shared_ptr(const JSObject& js_object) {
	static const std::string log_prefix { "MDL: get_JSNativeClass_shared_ptr:" };
	
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto key               = reinterpret_cast<std::intptr_t>(native_object_ptr);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	const auto js_object_identifier = static_cast<std::string>(js_object);
#endif
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << js_object_identifier
	          << ": look for key "
	          << key
	          << " in JSNativeClass map."
	          << std::endl;
#endif
	
	const auto native_object_ptr_map_position = native_ptr_to_js_native_class_ptr_map_.find(key);
	const bool js_native_class_ptr_found      = native_object_ptr_map_position != native_ptr_to_js_native_class_ptr_map_.end();
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << js_object_identifier
	          << ": found js_native_class_ptr_found = "
	          << std::boolalpha
	          << js_native_class_ptr_found
	          << "."
	          << std::endl;
#endif
	
	// postcondition
	assert(js_native_class_ptr_found);
	
	return native_object_ptr_map_position -> second;
}

// Support for JSStaticValue: getter
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSStaticValueGetPropertyCallback:" };

	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);

	const auto js_native_class_ptr = get_JSNativeClass_shared_ptr(js_object);
	const auto native_object_ptr   = static_cast<T*>(js_object.GetPrivate());
		
	JSString property_name(property_name_ref);

	//JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD;

	const auto& value_property_callback_map = js_native_class_ptr -> value_property_callback_map_;
	const auto callback_position            = value_property_callback_map.find(property_name);
	const bool callback_found               = callback_position != value_property_callback_map.end();
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	const auto js_native_class_identifier = js_native_class_ptr -> js_native_class_identifier_;
#endif

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", property_name = "
	          << property_name
	          << ", callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(callback_found);
	
	const auto callback = (callback_position -> second).get_get_property_callback();
	const auto result   = callback(*native_object_ptr);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", property_name = "
	          << property_name
	          << ", result = "
	          << result
	          << "."
	          << std::endl;
#endif
	
	return result;
}

// Support for JSStaticValue: setter
template<typename T>
bool JSNativeClass<T>::JSStaticValueSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticValueSetPropertyCallback:" };

	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);

	const auto js_native_class_ptr = get_JSNativeClass_shared_ptr(js_object);
	const auto native_object_ptr   = static_cast<T*>(js_object.GetPrivate());

	JSString property_name(property_name_ref);

	//JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD;

	const auto& value_property_callback_map = js_native_class_ptr -> value_property_callback_map_;
	const auto callback_position            = value_property_callback_map.find(property_name);
	const bool callback_found               = callback_position != value_property_callback_map.end();

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	const auto js_native_class_identifier = js_native_class_ptr -> js_native_class_identifier_;
#endif
	
	JSValue js_value(js_context, js_value_ref);

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", property_name = "
	          << property_name
	          << ", js_value = "
	          << js_value
	          << ", callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(callback_found);
	
	const auto callback = (callback_position -> second).get_set_property_callback();
	const auto result   = callback(*native_object_ptr, js_value);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", property_name = "
	          << property_name
	          << ", result = "
	          << result
	          << "."
	          << std::endl;
#endif
	
	return result;
}

// Support for JSStaticFunction
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef js_function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticFunctionCallAsFunctionCallback:" };
	
	JSContext js_context(js_context_ref);
	JSObject  js_function(js_context, js_function_ref);

	const auto js_native_class_ptr = get_JSNativeClass_shared_ptr(js_function);
	const auto native_object_ptr   = static_cast<T*>(js_function.GetPrivate());
	
	assert(js_function.IsFunction());
	JSString function_name; // TODO

	//JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD;

	const auto& function_property_callback_map = js_native_class_ptr -> function_property_callback_map_;
	const auto callback_position               = function_property_callback_map.find(function_name);
	const bool callback_found                  = callback_position != function_property_callback_map.end();

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	const auto js_native_class_identifier = js_native_class_ptr -> js_native_class_identifier_;
#endif
	
	JSObject this_object(js_context, this_object_ref);
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef js_value_ref) { return JSValue(js_context, js_value_ref); });

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", function_name = "
	          << function_name
	          << ", argument count = "
	          << arguments.size()
	          << ", this_object = "
	          << static_cast<std::string>(this_object)
	          << ", callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(callback_found);
	const auto callback = (callback_position -> second).get_call_as_function_callback();
	const auto result   = callback(*native_object_ptr, arguments, this_object);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_class_identifier
	          << ", function_name = "
	          << function_name
	          << ", result = "
	          << result
	          << "."
	          << std::endl;
#endif
	
	return result;
}

template<typename T>
void JSNativeClass<T>::JSObjectInitializeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref) {
	static const std::string log_prefix { "MDL: JSObjectInitializeCallback:" };
	
	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);
	
	const auto js_native_class_ptr = get_JSNativeClass_shared_ptr(js_object);
	const auto native_object_ptr    = static_cast<T*>(js_object.GetPrivate());

	//JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD;
	auto callback = js_native_class_ptr -> initialize_callback_;
	callback(*native_object_ptr);
}

template<typename T>
void JSNativeClass<T>::JSObjectFinalizeCallback(JSObjectRef js_object_ref) {
	static const std::string log_prefix { "MDL: JSObjectInitializeCallback:" };
	
	const auto native_object_ptr = static_cast<T*>(JSObjectGetPrivate(js_object_ref));
	
	JSContext js_context = native_object_ptr -> get_context();
	JSObject  js_object(js_context, js_object_ref);
	
	const auto js_native_class_ptr = get_JSNativeClass_shared_ptr(js_object);
	
	//JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_STATIC_LOCK_GUARD;

	auto callback = js_native_class_ptr -> finalize_callback_;
	callback(*native_object_ptr);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVECLASS_HPP_

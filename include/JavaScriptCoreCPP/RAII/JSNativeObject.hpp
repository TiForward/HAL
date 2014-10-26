/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_

#ifndef DEBUG_JSNATIVEOBJECT
#define DEBUG_JSNATIVEOBJECT
#endif

#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <cassert>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObjectBuilder;

template<typename T>
class JSNativeObject;

template<typename T>
using JSNativeObject_shared_ptr = std::shared_ptr<JSNativeObject<T>>;

template<typename T>
class JSNativeObject final {
	
 public:
	
	JSNativeObject() {
	}

	// JSNativeObjectDefinition<T> get_js_native_object_definition() const {
	// 	return js_native_object_definition_;
	// }
	
 private:

	// Only the JSNativeObjectBuilder can create instances of
	// JSNativeObject.
	JSNativeObject(const JSNativeObjectBuilder<T>& builder);

	template<typename U>
	friend class JSNativeObjectBuilder;
	
	JSContext                           js_context_;
	detail::JSNativeObjectDefinition<T> js_native_object_definition_;
	std::mutex                          js_native_object_mutex_;
	
	static JSNativeObject_shared_ptr<T> get_JSNativeObject_shared_ptr(const JSObject& js_object);

	// For interoperability with the JavaScriptCore C API.
	static std::unordered_map<std::intptr_t, JSNativeObject_shared_ptr<T>> native_ptr_to_js_native_object_ptr_map_;
	static std::mutex                                                      native_ptr_to_js_native_object_ptr_map_mutex_;

	// Support for JSStaticValue
	static JSValueRef JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool JSStaticValueSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception);
	
	// Support for JSStaticFunction
	static JSValueRef JSStaticFunctionCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	
	// Remaining callbacks.
	static void JSObjectInitializeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref);
	static void JSObjectFinalizeCallback(JSObjectRef js_object_ref);
	static bool JSObjectHasPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name);
	static JSValueRef JSObjectGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool JSObjectSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception);
	static bool JSObjectDeletePropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static void JSObjectGetPropertyNamesCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSPropertyNameAccumulatorRef property_names);
	static JSValueRef JSObjectCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	static JSObjectRef JSObjectCallAsConstructorCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	static bool JSObjectHasInstanceCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance, JSValueRef* exception);
	static JSValueRef JSObjectConvertToTypeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSType js_type, JSValueRef* exception);
};

template<typename T>
std::unordered_map<std::intptr_t, JSNativeObject_shared_ptr<T>> JSNativeObject<T>::native_ptr_to_js_native_object_ptr_map_;

template<typename T>
std::mutex JSNativeObject<T>::native_ptr_to_js_native_object_ptr_map_mutex_;

template<typename T>
JSNativeObject_shared_ptr<T> JSNativeObject<T>::get_JSNativeObject_shared_ptr(const JSObject& js_object) {
	static const std::string log_prefix { "MDL: get_JSNativeObject_shared_ptr:" };
	
	const auto native_object_ptr = static_cast<T>(js_object.GetPrivate());
	const auto key               = reinterpret_cast<std::intptr_t>(native_object_ptr);
	
#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_object_identifier = static_cast<std::string>(js_object);
#endif
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << " [DEBUG] "
	          << js_object_identifier
	          << ": look for key "
	          << key
	          << " in JSNativeObject map."
	          << std::endl;
#endif
	
	const auto native_object_ptr_map_position = native_ptr_to_js_native_object_ptr_map_.find(key);
	const bool js_native_object_ptr_found     = native_object_ptr_map_position != native_ptr_to_js_native_object_ptr_map_.end();
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << " [DEBUG] "
	          << js_object_identifier
	          << ": found js_native_object_ptr_found = "
	          << std::boolalpha
	          << js_native_object_ptr_found
	          << "."
	          << std::endl;
#endif
	
	// postcondition
	assert(js_native_object_ptr_found);
	
	return native_object_ptr_map_position -> second;
}

// Support for JSStaticValue: getter
template<typename T>
JSValueRef JSNativeObject<T>::JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticValueGetPropertyCallback:" };

	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);

	const auto js_native_object_ptr = get_JSNativeObject_shared_ptr(js_object);
	const auto native_object_ptr    = static_cast<T>(js_native_object_ptr.get());
		
	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& js_static_value_map = js_native_object_ptr -> js_static_value_map_;
	const auto callback_position    = js_static_value_map.find(property_name);
	const bool callback_found       = callback_position != js_static_value_map.end();

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif

#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
	
	const auto callback = callback_position -> second;
	const auto result   = callback(*native_object_ptr, property_name);
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
bool JSNativeObject<T>::JSStaticValueSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticValueSetPropertyCallback:" };

	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);

	const auto js_native_object_ptr = get_JSNativeObject_shared_ptr(js_object);
	const auto native_object_ptr    = static_cast<T>(js_native_object_ptr.get());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& js_static_value_map = js_native_object_ptr -> js_static_value_map_;
	const auto callback_position    = js_static_value_map.find(property_name);
	const bool callback_found       = callback_position != js_static_value_map.end();

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
	JSValue js_value(js_context, js_value_ref);

#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
	
	const auto callback = callback_position -> second;
	const auto result   = callback(*native_object_ptr, property_name, js_value);
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
JSValueRef JSNativeObject<T>::JSStaticFunctionCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef js_function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticFunctionCallAsFunctionCallback:" };
	
	JSContext js_context(js_context_ref);
	JSObject  js_function(js_context, js_function_ref);

	const auto js_native_object_ptr = get_JSNativeObject_shared_ptr(js_function);
	const auto native_object_ptr    = static_cast<T>(js_native_object_ptr.get());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	assert(js_function.IsFunction());
	JSString function_name; // TODO

	const auto& js_static_function_map = js_native_object_ptr -> js_static_function_map_;
	const auto callback_position       = js_static_function_map.find(function_name);
	const bool callback_found          = callback_position != js_static_function_map.end();

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
	JSObject this_object(js_context, this_object_ref);
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef js_value_ref) { return JSValue(js_context, js_value_ref); });

#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
	const auto callback = callback_position -> second;
	const auto result   = callback(*native_object_ptr, arguments, this_object);
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
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
void JSNativeObject<T>::JSObjectInitializeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref) {
	static const std::string log_prefix { "MDL: JSObjectInitializeCallback:" };
	
	JSContext js_context(js_context_ref);
	JSObject  js_object(js_context, js_object_ref);
	
	const auto js_native_object_ptr = get_JSNativeObject_shared_ptr(js_object);
	const auto native_object_ptr    = static_cast<T>(js_native_object_ptr.get());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	auto callback = js_native_object_ptr -> js_native_object_definition_.get_initialize_callback();
	callback(*native_object_ptr);
}

template<typename T>
void JSNativeObject<T>::JSObjectFinalizeCallback(JSObjectRef js_object_ref) {
}
	
template<typename T>
bool JSNativeObject<T>::JSObjectHasPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name) {
	return false;
}
	
template<typename T>
JSValueRef JSNativeObject<T>::JSObjectGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception) {
	return nullptr;
}
	
template<typename T>
bool JSNativeObject<T>::JSObjectSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception) {
	return false;
}
	
template<typename T>
bool JSNativeObject<T>::JSObjectDeletePropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception) {
	return false;
}
	
template<typename T>
void JSNativeObject<T>::JSObjectGetPropertyNamesCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSPropertyNameAccumulatorRef property_names) {
}
	
template<typename T>
JSValueRef JSNativeObject<T>::JSObjectCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	return nullptr;
}
	
template<typename T>
JSObjectRef JSNativeObject<T>::JSObjectCallAsConstructorCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	return nullptr;
}
	
template<typename T>
bool JSNativeObject<T>::JSObjectHasInstanceCallback(JSContextRef js_context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance, JSValueRef* exception) {
	return false;
}
	
template<typename T>
JSValueRef JSNativeObject<T>::JSObjectConvertToTypeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSType js_type, JSValueRef* exception) {
	return nullptr;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


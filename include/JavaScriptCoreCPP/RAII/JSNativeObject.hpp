/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_

#ifndef DEBUG_JSNATIVEOBJECT
#define DEBUG_JSNATIVEOBJECT
#endif

#include "JavaScriptCoreCPP/RAII/JSNativeObjectDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <cassert>
#include <iostream>
#include <sstream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

template<typename T>
class JSNativeObjectBuilder;

template<typename T>
class JSNativeObject {
	
 public:
	
	virtual ~JSNativeObject() {
	}

	JSNativeObjectDefinition<T> get_js_native_object_definition() const {
		return js_native_object_definition_;
	}
	
 private:

	// Only the JSNativeObjectBuilder can create instances of
	// JSNativeObject.
	JSNativeObject(const JSNativeObjectBuilder<T>& builder);

	template<typename U>
	friend class JSNativeObjectBuilder;
	
	JSContext                   js_context_;
	JSNativeObjectDefinition<T> js_native_object_definition_;
	JSClassDefinition           js_class_definition_ = kJSClassDefinitionEmpty;
	JSClass                     js_class_;
	std::shared_ptr<T>          native_object_ptr_;
	std::string                 js_native_object_identifier_;

	// For interoperability with the JavaScriptCore C API.

	// Support for JSStaticValue
	std::vector<JSStaticValue>                        js_static_value_vector_;
	std::unordered_map<std::string, JSStaticValue>    js_static_value_map_;
	std::mutex                                        js_static_value_map_mutex_;
	
	static JSValueRef JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool JSStaticValueSetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef js_value_ref, JSValueRef* exception);

	// Support for JSStaticFunction
	std::vector<JSStaticFunction>                     js_static_function_vector_;
	std::unordered_map<std::string, JSStaticFunction> js_static_function_map_;
	std::mutex                                        js_static_function_map_mutex_;
	
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

// Support for JSStaticValue: getter
template<typename T>
JSValueRef JSNativeObject<T>::JSStaticValueGetPropertyCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name_ref, JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticValueGetPropertyCallback:" };
	
	JSObject js_object(js_context_ref, js_object_ref);

	auto js_native_object_ptr = static_cast<JSNativeObject<T>>(js_object.GetPrivate());
	
	// Begin critical section.
	std::lock_guard<std::mutex> js_static_value_map_lock(js_native_object_ptr -> js_static_value_map_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& js_static_value_map = js_native_object_ptr -> js_static_value_map_;
	const auto position = js_static_value_map.find(property_name);

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif

#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
	          << ", property_name = "
	          << property_name
	          << ", position != js_static_value_map.end() = "
	          << std::boolalpha
	          << (position != js_static_value_map.end)
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(position != js_static_value_map.end());
	
	const auto callback = position -> second;
	auto native_object_ptr = js_native_object_ptr -> native_object_ptr_;
	auto result = callback(*native_object_ptr, property_name);
	
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
	JSObject js_object(js_context, js_object_ref);
	
	auto js_native_object_ptr = static_cast<JSNativeObject<T>>(js_object.GetPrivate());
	
	// Begin critical section.
	std::lock_guard<std::mutex> js_static_value_map_lock(js_native_object_ptr -> js_static_value_map_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& js_static_value_map = js_native_object_ptr -> js_static_value_map_;
	const auto position = js_static_value_map.find(property_name);

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
	JSValue  js_value(js_context, js_value_ref);

#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
	          << ", property_name = "
	          << property_name
	          << ", js_value = "
	          << js_value
	          << ", position != js_static_value_map.end() = "
	          << std::boolalpha
	          << (position != js_static_value_map.end)
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(position != js_static_value_map.end());
	
	const auto callback = position -> second;
	auto native_object_ptr = js_native_object_ptr -> native_object_ptr_;
	auto result = callback(*native_object_ptr, property_name, js_value);
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
	          << ", property_name = "
	          << property_name
	          << ", js_value = "
	          << js_value
	          << ", result = "
	          << std::boolalpha
	          << result
	          << "."
	          << std::endl;
#endif
	
	return result;
}

// Support for JSStaticFunction
template<typename T>
JSValueRef JSNativeObject<T>::JSStaticFunctionCallAsFunctionCallback(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	static const std::string log_prefix { "MDL: JSStaticFunctionCallAsFunctionCallback:" };
	
	JSContext js_context(js_context_ref);
	JSObject  js_function(js_context, function_ref);

	auto js_native_object_ptr = static_cast<JSNativeObject<T>>(js_function.GetPrivate());
	
	// Begin critical section.
	std::lock_guard<std::mutex> js_static_value_map_lock(js_native_object_ptr -> js_static_value_map_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& js_static_function_map = js_native_object_ptr -> js_static_function_map_;
	const auto position = js_static_function_map.find(property_name);

#ifdef DEBUG_JSNATIVEOBJECT
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
#ifdef DEBUG_JSNATIVEOBJECT
	std::clog << log_prefix
	          << "[DEBUG] "
	          << js_native_object_identifier
	          << ", position != js_static_value_map.end() = "
	          << std::boolalpha
	          << (position != js_static_function_map.end)
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(position != js_static_function_map.end());
	
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef js_value_ref) { return JSValue(js_context, js_value_ref); });

	auto native_object_ptr = js_native_object_ptr -> native_object_ptr_;
	const auto js_native_object_function_property_callback = position -> second;

	if (this_object_ref) {
		JSObject js_this_object(js_context, this_object_ref);

#ifdef DEBUG_JSNATIVEOBJECT
		std::clog << log_prefix
		          << "[DEBUG] "
		          << js_native_object_identifier
		          << " BEFORE: call function "
		          << js_native_object_function_property_callback.get_function_name
		          << " with "
		          << arguments.size()
		          << " arguments"
		          << " (this_object = "
		          << js_this_object
		          << ")."
		          << std::endl;
#endif

		const auto callback = js_native_object_function_property_callback.get_call_as_function_with_this_callback();
		auto result = callback(*native_object_ptr, arguments, js_this_object);

#ifdef DEBUG_JSNATIVEOBJECT
		std::clog << log_prefix
		          << "[DEBUG] "
		          << js_native_object_identifier
		          << " AFTER: called function "
		          << js_native_object_function_property_callback.get_function_name
		          << " with "
		          << arguments.size()
		          << " arguments"
		          << ", result = "
		          << result
		          << " (this_object = "
		          << js_this_object
		          << ")."
		          << std::endl;
#endif
		
		return result;

	} else {

#ifdef DEBUG_JSNATIVEOBJECT
		std::clog << log_prefix
		          << "[DEBUG] "
		          << js_native_object_identifier
		          << " BEFORE: call function "
		          << js_native_object_function_property_callback.get_function_name
		          << " with "
		          << arguments.size()
		          << " arguments"
		          << "."
		          << std::endl;
#endif

		const auto callback = js_native_object_function_property_callback.get_call_as_function_callback();
		auto result = callback(*native_object_ptr, arguments);

#ifdef DEBUG_JSNATIVEOBJECT
		std::clog << log_prefix
		          << "[DEBUG] "
		          << js_native_object_identifier
		          << " AFTER: called function "
		          << js_native_object_function_property_callback.get_function_name
		          << " with "
		          << arguments.size()
		          << " arguments"
		          << ", result = "
		          << result
		          << "."
		          << std::endl;
#endif

		return result;
	}
}


template<typename T>
void JSNativeObject<T>::JSObjectInitializeCallback(JSContextRef js_context_ref, JSObjectRef js_object_ref) {
	JSObject js_object(js_context_ref, js_object);
	auto js_native_object_ptr = static_cast<JSNativeObject<T>>(js_object.GetPrivate());
	auto callback             = js_native_object_ptr -> js_native_object_definition_.get_initialize_callback();
	auto native_object_ptr    = js_native_object_ptr -> native_object_ptr_;
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

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


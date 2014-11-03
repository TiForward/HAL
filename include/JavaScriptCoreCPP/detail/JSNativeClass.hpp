/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_HPP_

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

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

//#include <JavaScriptCore/JavaScript.h>
extern "C" {
	struct JSClassRef;
	struct JSClassDefinition;
	struct JSContextRef;
	struct JSStringRef;
	struct JSValueRef;
	struct JSObjectRef;
	struct JSPropertyNameAccumulatorRef;
}

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
using JSNativeObjectValuePropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>;

template<typename T>
using JSNativeObjectFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>>;

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

	explicit JSNativeClass(const JSClassDefinition* js_class_definition)
			: JSClass(js_class_definition) {
	}
	
	JSNativeClass() = delete;
	~JSNativeClass() = default;
	
	// Copy constructor.
	JSNativeClass(const JSNativeClass& rhs) = default;
	
	// Move constructor.
	JSNativeClass(JSNativeClass&& rhs) = default;
	
	// Copy assignment operator.
	JSNativeClass& operator=(const JSNativeClass&) = default;
	
	// Move assignment operator.
	JSNativeClass& operator=(JSNativeClass&&) = default;
	
private:
	
	static void JSNativeClass<T>::InitializeJSStaticValueVector();
	static void JSNativeClass<T>::InitializeJSStaticFunctionVector();
	
	template<typename U>
	friend class JSNativeClassBuilder;

	static std::string                                    js_class_name_;
	static JSClassDefinition                              js_class_definition_;
	static std::vector<JSStaticValue>                     js_static_values_;
	static std::vector<JSStaticFunction>                  js_static_functions_;
	static InitializeCallback<T>                          initialize_callback_;
	static FinalizeCallback<T>                            finalize_callback_;
	static CallAsFunctionCallback<T>                      call_as_function_callback_;
	static CallAsConstructorCallback<T>                   call_as_constructor_callback_;
	static HasInstanceCallback<T>                         has_instance_callback_;
	static ConvertToTypeCallback<T>                       convert_to_type_callback_;
	static JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	static JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;

#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_TYPE
#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX
#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME
#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX
#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_TYPE        std::recursive_mutex
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX js_native_class_static
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME        JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX##_mutex_
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX             JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_TYPE JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME;
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD        std::lock_guard<JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_TYPE> JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME);
#else
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
	static JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX;
	

	// Support for JSStaticValue
	static JSValueRef  JSStaticValueGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool        JSStaticValueSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception);
	
	// Support for JSStaticFunction
	static JSValueRef  JSStaticFunctionCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	
	// Remaining callbacks.
	static void        JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref);
	static void        JSObjectFinalizeCallback(JSObjectRef object_ref);
	static bool        JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref);
	static JSValueRef  JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static bool        JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception);
	static bool        JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception);
	static void        JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names);
	static JSValueRef  JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	static JSObjectRef JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
	static bool        JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception);
	static JSValueRef  JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception);

	// Helper functions.
	static JSValue::Type        ToJSValueType(JSType type);
	static std::string          LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
	static std::string          LogUnknownException(const std::string& function_name, const JSObject& js_object);
};

template<typename T>
void JSNativeClass<T>::InitializeJSStaticValueVector() {
	if (!value_property_callback_map_.empty()) {
		for (const auto& entry : value_property_callback_map_) {
			const auto& property_name           = entry.first;
			const auto& value_property_callback = entry.second;
			JSStaticValue js_static_value;
			js_static_value.name        = property_name.c_str();
			js_static_value.getProperty = JSStaticValueGetPropertyCallback;
			js_static_value.setProperty = JSStaticValueSetPropertyCallback;
			js_static_value.attributes  = ToJSPropertyAttributes(value_property_callback.get_attributes());
			js_static_values_.push_back(js_static_value);
			JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> added value property ", js_static_values_.back().name);
		}
		js_static_values_.push_back({nullptr, nullptr, nullptr, 0});
	}
}

template<typename T>
void JSNativeClass<T>::InitializeJSStaticFunctionVector() {
	if (!function_property_callback_map_.empty()) {
		for (const auto& entry : function_property_callback_map_) {
			const auto& function_name              = entry.first;
			const auto& function_property_callback = entry.second;
			JSStaticFunction js_static_function;
			js_static_function.name           = function_name.c_str();
			js_static_function.callAsFunction = JSStaticFunctionCallAsFunctionCallback;
			js_static_function.attributes     = ToJSPropertyAttributes(function_property_callback.get_attributes());
			js_static_functions_.push_back(js_static_function);
			JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> added function property ", js_static_functions_.back().name);
		}
		js_static_functions_.push_back({nullptr, nullptr, 0});
	}
}

template<typename T> JSClassDefinition                                        JSNativeClass<T>::js_class_definition_ = kJSClassDefinitionEmpty;
template<typename T> std::vector<JSStaticValue>                               JSNativeClass<T>::js_static_values_;
template<typename T> std::vector<JSStaticFunction>                            JSNativeClass<T>::js_static_functions_;
template<typename T> InitializeCallback<T>                                    JSNativeClass<T>::initialize_callback_          { nullptr };
template<typename T> FinalizeCallback<T>                                      JSNativeClass<T>::finalize_callback_            { nullptr };
template<typename T> CallAsFunctionCallback<T>                                JSNativeClass<T>::call_as_function_callback_    { nullptr };
template<typename T> CallAsConstructorCallback<T>                             JSNativeClass<T>::call_as_constructor_callback_ { nullptr };
template<typename T> HasInstanceCallback<T>                                   JSNativeClass<T>::has_instance_callback_        { nullptr };
template<typename T> ConvertToTypeCallback<T>                                 JSNativeClass<T>::convert_to_type_callback_     { nullptr };
template<typename T> JSNativeObjectValuePropertyCallbackMap_t<T>              JSNativeClass<T>::value_property_callback_map_;
template<typename T> JSNativeObjectFunctionPropertyCallbackMap_t<T>           JSNativeClass<T>::function_property_callback_map_;
template<typename T> JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_TYPE JSNativeClass<T>::JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_MUTEX_NAME;

// Support for JSStaticValue: getter
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticValueGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	JSString property_name(property_name_ref);
	const auto callback_position = value_property_callback_map_.find(property_name);
	const bool callback_found    = callback_position != value_property_callback_map_.end();
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> value property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback_found);

	JSObject js_object(JSContext(context_ref), object_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto callback          = (callback_position -> second).get_get_property_callback();
	const auto result            = callback(*native_object_ptr);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> value property ", to_string(property_name), " = ", to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSStaticValueGetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSStaticValueGetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

// Support for JSStaticValue: setter
template<typename T>
bool JSNativeClass<T>::JSStaticValueSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;

	JSString property_name(property_name_ref);
	const auto callback_position = value_property_callback_map_.find(property_name);
	const bool callback_found    = callback_position != value_property_callback_map_.end();

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> value property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback_found);

	JSContext js_context(context_ref);
	JSObject  js_object(js_context, object_ref);
	JSValue   js_value(js_context, value_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto callback          = (callback_position -> second).get_set_property_callback();
	const auto result            = callback(*native_object_ptr, js_value);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> value property ", to_string(property_name), " set to ", to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSStaticValueSetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSStaticValueSetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

// Support for JSStaticFunction
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;

	JSContext js_context(context_ref);
	JSObject  js_object(js_context, function_ref);
	
	// precondition
	assert(js_object.IsFunction());
	
	JSString function_name; // TODO
	
	const auto callback_position = function_property_callback_map_.find(function_name);
	const bool callback_found    = callback_position != function_property_callback_map_.end();
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> function property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback_found);

	const std::vector<JSValue> arguments = ToJSValueVector(js_context, argument_count, arguments_array);

	JSObject this_object(js_context, this_object_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto callback          = (callback_position -> second).get_call_as_function_callback();
	const auto result            = callback(*native_object_ptr, arguments, this_object);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> ", to_string(this_object), ".", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSStaticFunctionCallAsFunctionCallback", JSObject(js_context, function_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSStaticFunctionCallAsFunctionCallback", JSObject(js_context, function_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
void JSNativeClass<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	auto       callback       = initialize_callback_;
	const bool callback_found = callback != nullptr;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> Initialize callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback);
	
	JSObject  js_object(JSContext(context_ref), object_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	callback(*native_object_ptr);
} catch (const std::exception& e) {
	LogStdException("JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
void JSNativeClass<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	auto       callback       = finalize_callback_;
	const bool callback_found = callback != nullptr;

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> Finalize callback found = ", std::to_string(callback_found));
	
	// precondition
	assert(finalize_callback_);

	// TODO: get context_ref
	// JSContext js_context(context_ref);
	// JSObject  js_object(js_context, object_ref);
	// const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	// callback(*native_object_ptr);
} catch (const std::exception& e) {
	//LogStdException("JSObjectFinalizeCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	//LogUnknownException("JSObjectFinalizeCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
bool JSNativeClass<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
	return JSObject(JSContext(context_ref), object_ref).HasProperty(property_name_ref);
} catch (const std::exception& e) {
	LogStdException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	return JSObject(JSContext(context_ref), object_ref).GetProperty(property_name_ref);
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
	JSContext js_context(context_ref);
	JSObject(js_context, object_ref).SetProperty(property_name_ref, JSValue(js_context, value_ref));
	return true;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectSetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectSetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	return JSObject(JSContext(context_ref), object_ref).DeleteProperty(property_name_ref);
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
void JSNativeClass<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
	return JSObject(JSContext(context_ref), object_ref).GetPropertyNames(JSPropertyNameAccumulator(property_names));
} catch (const std::exception& e) {
	LogStdException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;

	auto       callback       = call_as_function_callback_;
	const bool callback_found = callback != nullptr;

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> Function callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, function_ref);
	
	// precondition
	assert(js_object.IsFunction());
	
	JSObject this_object(js_context, this_object_ref);

	const auto arguments         = ToJSValueVector(js_context, argument_count, arguments_array);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto result            = callback(*native_object_ptr, arguments, this_object);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> ", to_string(this_object), ".", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
JSObjectRef JSNativeClass<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	auto       callback       = call_as_constructor_callback_;
	const bool callback_found = callback != nullptr;

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> Constructor callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, constructor_ref);
	
	// precondition
	assert(js_object.IsConstructor());
	
	const std::vector<JSValue> arguments         = ToJSValueVector(js_context, argument_count, arguments_array);
	const auto                 native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto                 result            = callback(*native_object_ptr, arguments);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> new ", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	auto       callback       = has_instance_callback_;
	const bool callback_found = callback != nullptr;

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> HasInstance callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, constructor_ref);
	JSValue   possible_instance(js_context, possible_instance_ref);

	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto result = callback(*native_object_ptr, possible_instance);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> ", to_string(js_value), " instanceof ", to_string(js_object), " returned ", std::to_string(result));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_STATIC_LOCK_GUARD;
	
	auto       callback       = convert_to_type_callback_;
	const bool callback_found = callback != nullptr;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> ConvertToType callback found = ", std::to_string(callback_found));

	// precondition
	assert(callback);
	
	JSObject js_object(JSContext(context_ref), object_ref);
	JSValue::Type js_type(ToJSValueType(type));
	
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto result = callback(*native_object_ptr, js_type);
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, "> ", to_string(js_object), " converted to ", to_string(js_value));

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
JSValue::Type JSNativeClass<T>::ToJSValueType(JSType type) {
	
	switch (type) {
		case kJSTypeUndefined:
			return JSValue::Type::Undefined;
		case kJSTypeNull:
			return JSValue::Type::Null;
		case kJSTypeBoolean:
			return JSValue::Type::Boolean;
		case kJSTypeNumber:
			return JSValue::Type::Number;
		case kJSTypeString:
			return JSValue::Type::String;
		case kJSTypeObject:
			return JSValue::Type::Object;
		default:
			const std::string message = "Unknown JSType " + std::to_string(type);
			JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name_, "> ", message);
			ThrowLogicError(message);
	}
}

template<typename T>
std::string JSNativeClass<T>::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
	std::ostringstream os;
	os << "JSClassBuilder<"
	   << js_class_name_
	   << ">::"
	   <<  function_name
	   << " for object "
	   << to_string(js_object)
	   << " threw exception: "
	   << exception.what();
	
	const auto message = os.str();
	JAVASCRIPTCORECPP_LOG_ERROR(message);
	return message;
}

template<typename T>
std::string JSNativeClass<T>::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
	std::ostringstream os;
	os << "JSClassBuilder<"
	   << js_class_name_
	   << ">::"
	   <<  function_name
	   << " for object "
	   << to_string(js_object)
	   << " threw unknown exception";
	
	const auto message = os.str();
	JAVASCRIPTCORECPP_LOG_ERROR(message);
	return message;
}

} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_HPP_

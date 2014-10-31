/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSNATIVECLASS_HPP_
#define _JAVASCRIPTCORECPP_JSNATIVECLASS_HPP_

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSNativeObjectCallbacks.hpp"
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

#include <JavaScriptCore/JavaScript.h>


#define JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
#define JAVASCRIPTCORECPP_JSNATIVECLASS_THREAD_SAFE


#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_THREAD_SAFE
#include <mutex>

#undef  JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_TYPE
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX js_native_class

#undef  JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_TYPE JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME;


#undef  JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_TYPE> JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX
#define JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_JSNATIVECLASS_THREAD_SAFE


namespace JavaScriptCoreCPP {

using namespace JavaScriptCoreCPP::detail;

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

	explicit JSNativeClass(const JSString& class_name);
	explicit JSNativeClass(const JSNativeClassBuilder<T>& builder);
	
	JSNativeClass() = delete;
	~JSNativeClass() = default;
	
	// Copy constructor.
	JSNativeClass(const JSNativeClass& rhs) = default;
	
	// Move constructor.
	JSNativeClass(JSNativeClass&& rhs) = default;
	
	// Copy assignment operators.
	JSNativeClass& operator=(const JSNativeClass&) = default;
	JSNativeClass& operator=(JSNativeClass&&) = default;
	
	
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
		!class_attributes_.empty()               && (js_class_definition_.attributes        = ToJSClassAttributes(class_attributes_));
		parent_class_                            && (js_class_definition_.parentClass       = parent_class_);
		!value_property_callback_map_.empty()    && (js_class_definition_.staticValues      = &js_static_values_[0]);
		!function_property_callback_map_.empty() && (js_class_definition_.staticFunctions   = &js_static_functions_[0]);
		initialize_callback_                     && (js_class_definition_.initialize        = JSNativeClass<T>::JSObjectInitializeCallback);
		finalize_callback_                       && (js_class_definition_.finalize          = JSNativeClass<T>::JSObjectFinalizeCallback);
		has_property_callback_                   && (js_class_definition_.hasProperty       = JSNativeClass<T>::JSObjectHasPropertyCallback);
		get_property_callback_                   && (js_class_definition_.getProperty       = JSNativeClass<T>::JSObjectGetPropertyCallback);
		set_property_callback_                   && (js_class_definition_.setProperty       = JSNativeClass<T>::JSObjectSetPropertyCallback);
		delete_property_callback_                && (js_class_definition_.deleteProperty    = JSNativeClass<T>::JSObjectDeletePropertyCallback);
		get_property_names_callback_             && (js_class_definition_.getPropertyNames  = JSNativeClass<T>::JSObjectGetPropertyNamesCallback);
		call_as_function_callback_               && (js_class_definition_.callAsFunction    = JSNativeClass<T>::JSObjectCallAsFunctionCallback);
		call_as_constructor_callback_            && (js_class_definition_.callAsConstructor = JSNativeClass<T>::JSObjectCallAsConstructorCallback);
		has_instance_callback_                   && (js_class_definition_.hasInstance       = JSNativeClass<T>::JSObjectHasInstanceCallback);
		convert_to_type_callback_                && (js_class_definition_.convertToType     = JSNativeClass<T>::JSObjectConvertToTypeCallback);
	}

	template<typename U>
	friend class JSNativeClassBuilder;

	static JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX;
	
	static JSString                                       class_name_;
	static std::string                                    class_name_for_js_class_definition_;
	static std::unordered_set<JSNativeClassAttribute>     class_attributes_;
	static JSClass                                        parent_class_;
	static JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	static JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;
	static InitializeCallback<T>                          initialize_callback_;
	static FinalizeCallback<T>                            finalize_callback_;
	static HasPropertyCallback<T>                         has_property_callback_;
	static GetPropertyCallback<T>                         get_property_callback_;
	static SetPropertyCallback<T>                         set_property_callback_;
	static DeletePropertyCallback<T>                      delete_property_callback_;
	static GetPropertyNamesCallback<T>                    get_property_names_callback_;
	static CallAsFunctionCallback<T>                      call_as_function_callback_;
	static CallAsConstructorCallback<T>                   call_as_constructor_callback_;
	static HasInstanceCallback<T>                         has_instance_callback_;
	static ConvertToTypeCallback<T>                       convert_to_type_callback_;
	
	// For interoperability with the JavaScriptCore C API.
	static std::vector<JSStaticValue>                     js_static_values_;
	static std::vector<JSStaticFunction>                  js_static_functions_;
	static JSClassDefinition                              js_class_definition_;

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
	static std::vector<JSValue> ToJSValueVector(const JSContext& js_context, size_t argument_count, const JSValueRef arguments_array[]);
	static JSValue::Type        ToJSValueType(JSType type);
	static std::string          LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
	static std::string          LogUnknownException(const std::string& function_name, const JSObject& js_object);

};

template<typename T> JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_TYPE JSNativeClass<T>::JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_MUTEX_NAME;

template<typename T> JSString                                       JSNativeClass<T>::class_name_;
template<typename T> std::string                                    JSNativeClass<T>::class_name_for_js_class_definition_;
template<typename T> std::unordered_set<JSNativeClassAttribute>     JSNativeClass<T>::class_attributes_;
template<typename T> JSClass                                        JSNativeClass<T>::parent_class_;
template<typename T> JSNativeObjectValuePropertyCallbackMap_t<T>    JSNativeClass<T>::value_property_callback_map_;
template<typename T> JSNativeObjectFunctionPropertyCallbackMap_t<T> JSNativeClass<T>::function_property_callback_map_;
template<typename T> InitializeCallback<T>                          JSNativeClass<T>::initialize_callback_          { nullptr };
template<typename T> FinalizeCallback<T>                            JSNativeClass<T>::finalize_callback_            { nullptr };
template<typename T> HasPropertyCallback<T>                         JSNativeClass<T>::has_property_callback_        { nullptr };
template<typename T> GetPropertyCallback<T>                         JSNativeClass<T>::get_property_callback_        { nullptr };
template<typename T> SetPropertyCallback<T>                         JSNativeClass<T>::set_property_callback_        { nullptr };
template<typename T> DeletePropertyCallback<T>                      JSNativeClass<T>::delete_property_callback_     { nullptr };
template<typename T> GetPropertyNamesCallback<T>                    JSNativeClass<T>::get_property_names_callback_  { nullptr };
template<typename T> CallAsFunctionCallback<T>                      JSNativeClass<T>::call_as_function_callback_    { nullptr };
template<typename T> CallAsConstructorCallback<T>                   JSNativeClass<T>::call_as_constructor_callback_ { nullptr };
template<typename T> HasInstanceCallback<T>                         JSNativeClass<T>::has_instance_callback_        { nullptr };
template<typename T> ConvertToTypeCallback<T>                       JSNativeClass<T>::convert_to_type_callback_     { nullptr };

template<typename T> std::vector<JSStaticValue>                     JSNativeClass<T>::js_static_values_;
template<typename T> std::vector<JSStaticFunction>                  JSNativeClass<T>::js_static_functions_;
template<typename T> JSClassDefinition                              JSNativeClass<T>::js_class_definition_;


// Support for JSStaticValue: getter
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticValueGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSStaticValueGetPropertyCallback: " };
	
	JSString property_name(property_name_ref);
	const auto callback_position = value_property_callback_map_.find(property_name);
	const bool callback_found    = callback_position != value_property_callback_map_.end();
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << "."
	          << property_name
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback_found);

	JSObject js_object(JSContext(context_ref), object_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto callback          = (callback_position -> second).get_get_property_callback();
	const auto result            = callback(*native_object_ptr);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << ": "
	          << static_cast<std::string>(js_object)
	          << "."
	          << property_name
	          << " = "
	          << static_cast<std::string>(result)
	          << "."
	          << std::endl;
#endif
	
	return result;
} catch (const std::exception& e) {
	*exception = JSValue(JSContext(context_ref), e.what());
} catch (...) {
	*exception = JSValue(JSContext(context_ref), "Unknown C++ exception from JavaScriptCoreCPP.");
}

// Support for JSStaticValue: setter
template<typename T>
bool JSNativeClass<T>::JSStaticValueSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSStaticValueSetPropertyCallback: " };

	JSString property_name(property_name_ref);
	const auto callback_position = value_property_callback_map_.find(property_name);
	const bool callback_found    = callback_position != value_property_callback_map_.end();

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << "."
	          << property_name
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif

	// precondition
	assert(callback_found);

	JSContext js_context(context_ref);
	JSObject  js_object(js_context, object_ref);
	JSValue   js_value(js_context, value_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto callback          = (callback_position -> second).get_set_property_callback();
	const auto result            = callback(*native_object_ptr, js_value);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << ": "
	          << static_cast<std::string>(js_object)
	          << "."
	          << property_name
	          << " = "
	          << js_value
	          << ", result = "
	          << result
	          << "."
	          << std::endl;
#endif
	
	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSStaticValueSetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSStaticValueSetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

// Support for JSStaticFunction
template<typename T>
JSValueRef JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback: " };

	JSContext js_context(context_ref);
	JSObject  js_function(js_context, function_ref);
	
	// precondition
	assert(js_function.IsFunction());
	
	JSString function_name; // TODO
	
	const auto callback_position = function_property_callback_map_.find(function_name);
	const bool callback_found    = callback_position != function_property_callback_map_.end();
	
	// precondition
	assert(callback_found);

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << "."
	          << function_name
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif

	const std::vector<JSValue> arguments = ToJSValueVector(js_context, argument_count, arguments_array);

	JSObject js_this_object(js_context, this_object_ref);
	const auto native_object_ptr = static_cast<T*>(js_function.GetPrivate());
	const auto callback          = (callback_position -> second).get_call_as_function_callback();
	const auto result            = callback(*native_object_ptr, arguments, js_this_object);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << ": "
	          << static_cast<std::string>(js_this_object)
	          << "."
	          << function_name
	          << "(), argument count = "
	          << arguments.size()
	          << ", result = "
	          << static_cast<std::string>(result)
	          << "."
	          << std::endl;
#endif
	
	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback", JSObject(js_context, function_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSStaticFunctionCallAsFunctionCallback", JSObject(js_context, function_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
void JSNativeClass<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectInitializeCallback: " };
	
	auto       callback       = initialize_callback_;
	const bool callback_found = callback != nullptr;
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback);
	
	JSObject  js_object(JSContext(context_ref), object_ref);
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	callback(*native_object_ptr);
} catch (const std::exception& e) {
	LogStdException("JSNativeClass<T>::JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSNativeClass<T>::JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
void JSNativeClass<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectFinalizeCallback: " };
	
	auto       callback       = finalize_callback_;
	const bool callback_found = callback != nullptr;

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(finalize_callback_);

	// TODO: get context_ref
	// JSContext js_context(context_ref);
	// JSObject  js_object(js_context, object_ref);
	// const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	// callback(*native_object_ptr);
} catch (const std::exception& e) {
	//LogStdException("JSNativeClass<T>::JSObjectFinalizeCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	//LogUnknownException("JSNativeClass<T>::JSObjectFinalizeCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
bool JSNativeClass<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	return JSObject(JSContext(context_ref), object_ref).HasProperty(property_name_ref);
} catch (const std::exception& e) {
	LogStdException("JSNativeClass<T>::JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSNativeClass<T>::JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	return JSObject(JSContext(context_ref), object_ref).GetProperty(property_name_ref);
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectGetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectGetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	JSContext js_context(context_ref);
	JSObject(js_context, object_ref).SetProperty(property_name_ref, JSValue(js_context, value_ref));
	return true;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectSetPropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectSetPropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	return JSObject(JSContext(context_ref), object_ref).DeleteProperty(property_name_ref);
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectDeletePropertyCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectDeletePropertyCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
void JSNativeClass<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	return JSObject(JSContext(context_ref), object_ref).GetPropertyNames(JSPropertyNameAccumulator(property_names));
} catch (const std::exception& e) {
	LogStdException("JSNativeClass<T>::JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
	LogUnknownException("JSNativeClass<T>::JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectCallAsFunctionCallback: " };
	
	auto       callback       = call_as_function_callback_;
	const bool callback_found = callback != nullptr;

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, function_ref);
	
	// precondition
	assert(js_object.IsFunction());
	
	JSObject this_object(js_context, this_object_ref);

	const std::vector<JSValue> arguments         = ToJSValueVector(js_context, argument_count, arguments_array);
	const auto                 native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto                 result            = callback(*native_object_ptr, arguments, this_object);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " argument count = "
	          << arguments.size()
	          << ", this_object = "
	          << static_cast<std::string>(this_object)
	          << ", result = "
	          << static_cast<std::string>(result)
	          << "."
	          << std::endl;
#endif

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
JSObjectRef JSNativeClass<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectCallAsConstructorCallback: " };
	
	auto       callback       = call_as_constructor_callback_;
	const bool callback_found = callback != nullptr;

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, constructor_ref);
	
	// precondition
	assert(js_object.IsConstructor());
	
	const std::vector<JSValue> arguments         = ToJSValueVector(js_context, argument_count, arguments_array);
	const auto                 native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto                 result            = callback(*native_object_ptr, arguments);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " argument count = "
	          << arguments.size()
	          << ", result = "
	          << static_cast<std::string>(result)
	          << "."
	          << std::endl;
#endif

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
bool JSNativeClass<T>::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectHasInstanceCallback: " };
	
	auto       callback       = has_instance_callback_;
	const bool callback_found = callback != nullptr;

#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback);
	
	JSContext js_context(context_ref);
	JSObject  js_object(js_context, constructor_ref);
	JSValue   possible_instance(js_context, possible_instance_ref);

	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto result = callback(*native_object_ptr, possible_instance);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " possible_instance = "
	          << static_cast<std::string>(possible_instance)
	          << ", result = "
	          << std::boolalpha
	          << result
	          << "."
	          << std::endl;
#endif

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
JSValueRef JSNativeClass<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_JSNATIVECLASS_STATIC_LOCK_GUARD;
	static const std::string log_prefix { "MDL: JSNativeClass<T>::JSObjectConvertToTypeCallback: " };
	
	auto       callback       = convert_to_type_callback_;
	const bool callback_found = callback != nullptr;
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " callback found = "
	          << std::boolalpha
	          << callback_found
	          << "."
	          << std::endl;
#endif
	
	// precondition
	assert(callback);
	
	JSObject js_object(JSContext(context_ref), object_ref);
	JSValue::Type js_type(ToJSValueType(type));
	
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto result = callback(*native_object_ptr, js_type);
	
#ifdef JAVASCRIPTCORECPP_JSNATIVECLASS_DEBUG
	std::clog << log_prefix
	          << "[DEBUG] "
	          << class_name_for_js_class_definition_
	          << " type = "
	          << to_string(js_type)
	          << ", result = "
	          << std::boolalpha
	          << result
	          << "."
	          << std::endl;
#endif

	return result;
} catch (const std::exception& e) {
	JSContext js_context(context_ref);
	JSString message(LogStdException("JSNativeClass<T>::JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
	*exception = JSValue(js_context, message);
} catch (...) {
	JSContext js_context(context_ref);
	JSString message(LogUnknownException("JSNativeClass<T>::JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
	*exception = JSValue(js_context, message);
}

template<typename T>
std::vector<JSValue> JSNativeClass<T>::ToJSValueVector(const JSContext& js_context, size_t argument_count, const JSValueRef arguments_array[]) {
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef value_ref) { return JSValue(js_context, value_ref); });
	return arguments;
}

template<typename T>
JSValue::Type JSNativeClass<T>::ToJSValueType(JSType type) {
	static const std::string log_prefix { "MDL: JSNativeClass<T>::ToJSValueType: " };
	
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
			const std::string message = "[LOGIC ERROR] Unknown JSType " + std::to_string(type);
			std::clog << log_prefix << message << std::endl;
			throw std::logic_error(message);
	}
}

template<typename T>
std::string JSNativeClass<T>::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
	static const std::string log_prefix { "JavaScriptCoreCPP caught exception in " };
	
	std::ostringstream os;
	os << log_prefix
	   << function_name
	   << " for object "
	   << static_cast<std::string>(js_object)
	   << ", exception text = "
	   << exception.what();
	
	const auto message = os.str();
	
	std::clog << message << std::endl;
	
	return message;
}

template<typename T>
std::string JSNativeClass<T>::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
	static const std::string log_prefix { "JavaScriptCoreCPP caught unknown exception in " };
	
	std::ostringstream os;
	os << log_prefix
	   << function_name
	   << " for object "
	   << static_cast<std::string>(js_object);
	
	const auto message = os.str();
	
	std::clog << message << std::endl;
	
	return message;
}

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSNATIVECLASS_HPP_

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

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSNativeObjectValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"

#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <mutex>
#include <iostream>
#include <sstream>
#include <cassert>

#include <JavaScriptCore/JavaScript.h>

#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG

namespace JavaScriptCoreCPP { namespace RAII {

using namespace JavaScriptCoreCPP::detail;

template<typename T>
using JSNativeObjectValuePropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>;

template<typename T>
using JSNativeObjectFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>>;


template<typename T>
class JSNativeObjectBuilder;

template<typename T>
class JSNativeObject;

template<typename T>
using JSNativeObject_shared_ptr = std::shared_ptr<JSNativeObject<T>>;

/*!
  @class

  @abstract This class is the bridge between a custom JavaScript
  object backed by a C++ class for some or all of its functionality.
*/
template<typename T>
class JSNativeObject final {
	
 public:
	
	JSNativeObject(const JSNativeObjectBuilder<T>& builder);
	
	~JSNativeObject() {
	}

	// Copy constructor.
	JSNativeObject(const JSNativeObject& rhs)
			: js_context_(rhs.js_context_)
			, name_(rhs.name_)
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
			, js_class_(rhs.js_class_)
			, js_class_definition_(kJSClassDefinitionEmpty) {
		
		InitializeJSStaticValueVector();
		InitializeJSStaticFunctionVector();
		InitializeJSClassDefinition();
		js_class_ = JSClass(&js_class_definition_);
	}
	
	// Move constructor.
	JSNativeObject(JSNativeObject&& rhs)
			: js_context_(rhs.js_context_)
			, name_(rhs.name_)
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
			, js_class_(rhs.js_class_)
			, js_class_definition_(rhs.js_class_definition_)
			, js_static_values_(rhs.js_static_values_)
			, js_static_functions_(rhs.js_static_functions_) {
	}
  
  // Create a copy of another JSNativeObject by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
	JSNativeObject& operator=(JSNativeObject rhs) {
    swap(*this, rhs);
    return *this;
  }
  
	friend void swap(JSNativeObject& first, JSNativeObject& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(first.js_context_                        , second.js_context_);
    swap(first.name_                              , second.name_);
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
    swap(first.js_class_                          , second.js_class_);
    swap(first.js_class_definition_               , second.js_class_definition_);
    swap(first.js_static_values_                  , second.js_static_values_);
    swap(first.js_static_functions_               , second.js_static_functions_);
}

private:

	void InitializeJSStaticValueVector() {
		if (!value_property_callback_map_.empty()) {
			for (const auto& entry : value_property_callback_map_) {
				const auto& property_name           = entry.first;
				const auto& value_property_callback = entry.second;
				JSStaticValue js_static_value;
				js_static_value.name        = property_name.c_str();
				js_static_value.getProperty = JSNativeObject<T>::JSStaticValueGetPropertyCallback;
				js_static_value.setProperty = JSNativeObject<T>::JSStaticValueSetPropertyCallback;
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
		!name_.empty()                           && (js_class_definition_.className         = class_name_for_js_class_definition_.c_str());
		!attributes_.empty()                     && (js_class_definition_.attributes        = ToJSClassAttributes(attributes_));
		parent_                                  && (js_class_definition_.parentClass       = js_class_);
		!value_property_callback_map_.empty()    && (js_class_definition_.staticValues      = &js_static_values_[0]);
		!function_property_callback_map_.empty() && (js_class_definition_.staticFunctions   = &js_static_functions_[0]);
		initialize_callback_                     && (js_class_definition_.initialize        = JSObjectInitializeCallback);
		finalize_callback_                       && (js_class_definition_.finalize          = JSObjectFinalizeCallback);
		call_as_constructor_callback_            && (js_class_definition_.callAsConstructor = JSObjectCallAsConstructorCallback);
		has_instance_callback_                   && (js_class_definition_.hasInstance       = JSObjectHasInstanceCallback);
		has_property_callback_                   && (js_class_definition_.hasProperty       = JSObjectHasPropertyCallback);
		get_property_callback_                   && (js_class_definition_.getProperty       = JSObjectGetPropertyCallback);
		set_property_callback_                   && (js_class_definition_.setProperty       = JSObjectSetPropertyCallback);
		delete_property_callback_                && (js_class_definition_.deleteProperty    = JSObjectDeletePropertyCallback);
		get_property_names_callback_             && (js_class_definition_.getPropertyNames  = JSObjectGetPropertyNamesCallback);
		call_as_function_callback_               && (js_class_definition_.callAsFunction    = JSObjectCallAsFunctionCallback);
		convert_to_type_callback_                && (js_class_definition_.convertToType     = JSObjectConvertToTypeCallback);
	}
	
	JSContext                                      js_context_;

	JSString                                       name_;
	std::string                                    class_name_for_js_class_definition_;
	std::unordered_set<JSNativeObjectAttribute>    attributes_;
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
	
	JSClass                                        js_class_;
	
	// For interoperability with the JavaScriptCore C API.
	JSClassDefinition                              js_class_definition_;
	std::vector<JSStaticValue>                     js_static_values_;
	std::vector<JSStaticFunction>                  js_static_functions_;
	
	std::string                                    js_native_object_identifier_ { "TODO" }; // TODO
	std::mutex                                     js_native_object_mutex_;

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
	
	const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
	const auto key               = reinterpret_cast<std::intptr_t>(native_object_ptr);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
	const auto js_object_identifier = static_cast<std::string>(js_object);
#endif
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	const auto native_object_ptr    = static_cast<T*>(js_object.GetPrivate());
		
	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& value_property_callback_map = js_native_object_ptr -> value_property_callback_map_;
	const auto callback_position            = value_property_callback_map.find(property_name);
	const bool callback_found               = callback_position != value_property_callback_map.end();
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	
	const auto callback = (callback_position -> second).get_get_property_callback();
	const auto result   = callback(*native_object_ptr);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	const auto native_object_ptr    = static_cast<T*>(js_object.GetPrivate());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	JSString property_name(property_name_ref);

	const auto& value_property_callback_map = js_native_object_ptr -> value_property_callback_map_;
	const auto callback_position            = value_property_callback_map.find(property_name);
	const bool callback_found               = callback_position != value_property_callback_map.end();

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
	JSValue js_value(js_context, js_value_ref);

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	
	const auto callback = (callback_position -> second).get_set_property_callback();
	const auto result   = callback(*native_object_ptr, js_value);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	const auto native_object_ptr    = static_cast<T*>(js_function.GetPrivate());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	
	assert(js_function.IsFunction());
	JSString function_name; // TODO

	const auto& function_property_callback_map = js_native_object_ptr -> function_property_callback_map_;
	const auto callback_position               = function_property_callback_map.find(function_name);
	const bool callback_found                  = callback_position != function_property_callback_map.end();

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
	const auto js_native_object_identifier = js_native_object_ptr -> js_native_object_identifier_;
#endif
	
	JSObject this_object(js_context, this_object_ref);
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef js_value_ref) { return JSValue(js_context, js_value_ref); });

#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	const auto callback = (callback_position -> second).get_call_as_function_callback();
	const auto result   = callback(*native_object_ptr, arguments, this_object);
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG
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
	const auto native_object_ptr    = static_cast<T*>(js_object.GetPrivate());

	// Begin critical section.
	std::lock_guard<std::mutex> js_native_object_lock(js_native_object_ptr -> js_native_object_mutex_);
	auto callback = js_native_object_ptr -> initialize_callback_;
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


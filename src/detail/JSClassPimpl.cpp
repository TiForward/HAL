/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSClassPimpl.hpp"

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace detail

std::string JSClassPimpl::get_name() const {
	return name__;
}

std::uint32_t JSClassPimpl::get_version() const {
	return version__;
}

JSClassPimpl::operator JSClassRef() const {
	return js_class_ref__;
}

~JSClassPimpl::JSClassPimpl() {
	JSClassRelease(js_class_ref__);
}

JSClassPimpl::JSClassPimpl(const JSClassPimpl& rhs)
		: version__(rhs.version__)
		, class_attribute__(rhs.class_attribute__)
		, name__(rhs.name__)
		, parent__(rhs.parent__)
		, named_value_property_callback_map__(rhs.named_value_property_callback_map__)
		, named_function_property_callback_map__(rhs.named_function_property_callback_map__)
		, initialize_callback__(rhs.initialize_callback__)
		, finalize_callback__(rhs.finalize_callback__)
		, call_as_function_callback__(rhs.call_as_function_callback__)
		, call_as_constructor_callback__(rhs.call_as_constructor_callback__)
		, has_instance_callback__(rhs.has_instance_callback__)
		, convert_to_type_callback__(rhs.convert_to_type_callback__) {
	
	Initialize();
}

JSClassPimpl::JSClassPimpl(JSClassPimpl&& rhs)
		: version__(std::move(rhs.version__))
		, class_attribute__(std::move(rhs.class_attribute__))
		, name__(std::move(rhs.name__))
		, parent__(std::move(rhs.parent__))
		, named_value_property_callback_map__(std::move(rhs.named_value_property_callback_map__))
		, named_function_property_callback_map__(std::move(rhs.named_function_property_callback_map__))
		, initialize_callback__(std::move(rhs.initialize_callback__))
		, finalize_callback__(std::move(rhs.finalize_callback__))
		, call_as_function_callback__(std::move(rhs.call_as_function_callback__))
		, call_as_constructor_callback__(std::move(rhs.call_as_constructor_callback__))
		, has_instance_callback__(std::move(rhs.has_instance_callback__))
		, convert_to_type_callback__(std::move(rhs.convert_to_type_callback__)) {

	Initialize();
}

// Create a copy of another JSClassPimpl by assignment. This is a unified
// assignment operator that fuses the copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSClassPimpl& JSClassPimpl::operator=(JSClassPimpl rhs) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;
	swap(rhs);
	return *this;
}

void JSClassPimpl::swap(JSClassPimpl& other) noexcept {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(version__                             , other.version__);
	swap(class_attribute__                     , other.class_attribute__);
	swap(name__                                , other.name__);
	swap(parent__                              , other.parent__);
	swap(named_value_property_callback_map__   , other.named_value_property_callback_map__);
	swap(named_function_property_callback_map__, other.named_function_property_callback_map__;);
	swap(initialize_callback__                 , other.initialize_callback__);
	swap(finalize_callback__                   , other.finalize_callback__);
	swap(call_as_function_callback__           , other.call_as_function_callback__);
	swap(call_as_constructor_callback__        , other.call_as_constructor_callback__);
	swap(has_instance_callback__               , other.has_instance_callback__);
	swap(convert_to_type_callback__            , other.convert_to_type_callback__);
	swap(js_class_definition__                 , other.js_class_definition__);
	swap(js_class_ref__                        , other.js_class_ref__);
}

void Initialize() {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;

	js_class_definition__             = kJSClassDefinitionEmpty;

	js_class_definition__.version     = js_class_version__;
	js_class_definition__.attributes  = ToJSClassAttribute(js_class_attribute__);

	js_class_definition__.className   = name__.c_str();
	js_class_definition__.parentClass = parent__;

	// Initialize staticValues.
	if (!named_value_property_callback_map__.empty()) {
		for (const auto& entry : named_value_property_callback_map__) {
			const auto& property_name       = entry.first;
			const auto& property_attributes = entry.second;
			JSStaticValue js_static_value;
			js_static_value.name        = property_name.c_str();
			js_static_value.getProperty = JSStaticValueGetPropertyCallback;
			js_static_value.setProperty = JSStaticValueSetPropertyCallback;
			js_static_value.attributes  = ToJSPropertyAttributes(property_attributes);
			js_static_values__.push_back(js_static_value);
			JAVASCRIPTCORECPP_LOG_DEBUG("JSClass<", name__, "> added value property ", js_static_values_.back().name);
		}
		js_static_values__.push_back({nullptr, nullptr, nullptr, 0});
		js_class_definition__.staticValues = &js_static_values__[0];
	}
	
	// Initialize staticFunctions.
	if (!native_function_property_map.empty()) {
		for (const auto& entry : native_function_property_map) {
			const auto& function_name              = entry.first;
			const auto& function_property_callback = entry.second;
			JSStaticFunction js_static_function;
			js_static_function.name           = function_name.c_str();
			js_static_function.callAsFunction = JSStaticFunctionCallAsFunctionCallback;
			js_static_function.attributes     = ToJSPropertyAttributes(function_property_callback.get_attributes());
			js_static_functions__.push_back(js_static_function);
			JAVASCRIPTCORECPP_LOG_DEBUG("JSClass<", name__, "> added function property ", js_static_functions_.back().name);
		}
		js_static_functions__.push_back({nullptr, nullptr, 0});
		js_class_definition__.staticFunctions = &js_static_functions__[0];
	}

	// We provide the following 5 callbacks by simply delegating to the
	// JSObject methods.
	js_class_definition__.hasProperty      = JSObjectHasPropertyCallback;
	js_class_definition__.getProperty      = JSObjectGetPropertyCallback;
	js_class_definition__.setProperty      = JSObjectSetPropertyCallback;
	js_class_definition__.deleteProperty   = JSObjectDeletePropertyCallback;
	js_class_definition__.getPropertyNames = JSObjectGetPropertyNamesCallback;

	// JSClassBuilder<T> sets these after it constructs us because they
	// refer to members of JSNativeClass<T>.
	//
	// initialize
	// finalize
	// callAsFunction
	// callAsConstructor
	// hasInstance
	// convertToType

	js_class_ref__ = JSClassCreate(&js_class_definition__);
}

std::recursive_mutex JSNativeClassPimpl::static_mutex__;

// Support for JSStaticValue: getter

JSValueRef JSNativeClassPimpl::JSStaticValueGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
	
	JSString property_name(property_name_ref);
	const auto callback_position = value_property_callback_map_.find(property_name);
  const bool callback_found    = callback_position != value_property_callback_map_.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> value property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback_found);

  JSObject js_object(JSContext(context_ref), object_ref);
  const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto callback          = (callback_position -> second).get_get_property_callback();
  const auto result            = callback(*native_object_ptr);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> value property ", to_string(property_name), " = ", to_string(result));

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

bool JSNativeClassPimpl::JSStaticValueSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;

  JSString property_name(property_name_ref);
  const auto callback_position = value_property_callback_map_.find(property_name);
  const bool callback_found    = callback_position != value_property_callback_map_.end();

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> value property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback_found);

  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  JSValue   js_value(js_context, value_ref);
  const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto callback          = (callback_position -> second).get_set_property_callback();
  const auto result            = callback(*native_object_ptr, js_value);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> value property ", to_string(property_name), " set to ", to_string(result));

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

JSValueRef JSNativeClassPimpl::JSStaticFunctionCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;

  JSContext js_context(context_ref);
  JSObject  js_object(js_context, function_ref);
  
  // precondition
  assert(js_object.IsFunction());
  
  JSString function_name; // TODO
  
  const auto callback_position = function_property_callback_map_.find(function_name);
  const bool callback_found    = callback_position != function_property_callback_map_.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> function property ", to_string(property_name), " callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback_found);

  const std::vector<JSValue> arguments = ToJSValueVector(js_context, argument_count, arguments_array);

  JSObject this_object(js_context, this_object_ref);
  const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto callback          = (callback_position -> second).get_call_as_function_callback();
  const auto result            = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> ", to_string(this_object), ".", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

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


void JSNativeClassPimpl::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  
  auto       callback       = initialize_callback_;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> Initialize callback found = ", std::to_string(callback_found));

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


void JSNativeClassPimpl::JSObjectFinalizeCallback(JSObjectRef object_ref) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  
  auto       callback       = finalize_callback_;
  const bool callback_found = callback != nullptr;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> Finalize callback found = ", std::to_string(callback_found));
  
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


bool JSNativeClassPimpl::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
  return JSObject(JSContext(context_ref), object_ref).HasProperty(property_name_ref);
} catch (const std::exception& e) {
  LogStdException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
  LogUnknownException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
}


JSValueRef JSNativeClassPimpl::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
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


bool JSNativeClassPimpl::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
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


bool JSNativeClassPimpl::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
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


void JSNativeClassPimpl::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
  return JSObject(JSContext(context_ref), object_ref).GetPropertyNames(JSPropertyNameAccumulator(property_names));
} catch (const std::exception& e) {
  LogStdException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
  LogUnknownException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
}


JSValueRef JSNativeClassPimpl::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;

  auto       callback       = call_as_function_callback_;
  const bool callback_found = callback != nullptr;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> Function callback found = ", std::to_string(callback_found));

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
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> ", to_string(this_object), ".", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

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


JSObjectRef JSNativeClassPimpl::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  
  auto       callback       = call_as_constructor_callback_;
  const bool callback_found = callback != nullptr;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> Constructor callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback);
  
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, constructor_ref);
  
  // precondition
  assert(js_object.IsConstructor());
  
  const std::vector<JSValue> arguments         = ToJSValueVector(js_context, argument_count, arguments_array);
  const auto                 native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto                 result            = callback(*native_object_ptr, arguments);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> new ", to_string(js_object), "(", std::to_string(arguments.size()), "arguments)", " returned ", to_string(result));

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


bool JSNativeClassPimpl::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  
  auto       callback       = has_instance_callback_;
  const bool callback_found = callback != nullptr;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> HasInstance callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback);
  
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, constructor_ref);
  JSValue   possible_instance(js_context, possible_instance_ref);

  const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto result = callback(*native_object_ptr, possible_instance);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> ", to_string(js_value), " instanceof ", to_string(js_object), " returned ", std::to_string(result));

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


JSValueRef JSNativeClassPimpl::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  
  auto       callback       = convert_to_type_callback_;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> ConvertToType callback found = ", std::to_string(callback_found));

  // precondition
  assert(callback);
  
  JSObject js_object(JSContext(context_ref), object_ref);
  JSValue::Type js_type(ToJSValueType(type));
  
  const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
  const auto result = callback(*native_object_ptr, js_type);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, "> ", to_string(js_object), " converted to ", to_string(js_value));

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


JSValue::Type JSNativeClassPimpl::ToJSValueType(JSType type) {
  
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
      JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name__, "> ", message);
      ThrowLogicError(message);
  }
}


std::string JSNativeClassPimpl::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
  std::ostringstream os;
  os << "JSClassBuilder<"
     << js_class_name__
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


std::string JSNativeClassPimpl::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
  std::ostringstream os;
  os << "JSClassBuilder<"
     << js_class_name__
     << ">::"
     <<  function_name
     << " for object "
     << to_string(js_object)
     << " threw unknown exception";
  
  const auto message = os.str();
  JAVASCRIPTCORECPP_LOG_ERROR(message);
  return message;
}

}: // namespace JavaScriptCoreCPP { namespace detail

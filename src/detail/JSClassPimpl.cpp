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
#include "JavaScriptCoreCPP/detail/JSExportCallbackHandler.hpp"
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

void Initialize(const JSObjectNamedValuePropertyCallbackMap_t&    named_value_property_callback_map
                const JSObjectNamedFunctionPropertyCallbackMap_t& named_function_property_callback_map) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;

	js_class_definition__             = kJSClassDefinitionEmpty;

	js_class_definition__.version     = js_class_version__;
	js_class_definition__.attributes  = ToJSClassAttribute(js_class_attribute__);

	js_class_definition__.className   = name__.c_str();
	js_class_definition__.parentClass = parent__;

	// Initialize staticValues.
	if (!named_value_property_callback_map.empty()) {
		for (const auto& entry : named_value_property_callback_map) {
			const auto& property_name       = entry.first;
			const auto& property_attributes = entry.second;
			JSStaticValue js_static_value;
			js_static_value.name        = property_name.c_str();
			js_static_value.getProperty = GetNamedValuePropertyCallback;
			js_static_value.setProperty = SetNamedValuePropertyCallback;
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
			js_static_function.callAsFunction = CallNamedFunctionCallback;
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


	// We delegate these calls to our JSExportCallbackHandler because
	// they only they know the template type T.
	js_class_definition__.initialize        = JSObjectInitializeCallback;
	js_class_definition__.finalize          = JSObjectFinalizeCallback;
	js_class_definition__.callAsFunction    = JSObjectCallAsFunctionCallback;
	js_class_definition__.callAsConstructor = JSObjectCallAsConstructorCallback;
	js_class_definition__.hasInstance       = JSObjectHasPropertyCallback;
	js_class_definition__.convertToType     = JSObjectConvertToTypeCallback;

	js_class_ref__ = JSClassCreate(&js_class_definition__);
}

std::recursive_mutex JSNativeClassPimpl::static_mutex__;

JSValueRef JSNativeClassPimpl::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
	return js_export_callback_handler_ptr__ -> GetNamedProperty(JSObject(JSContext(context_ref), object_ref), JSString(property_name_ref));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

bool JSNativeClassPimpl::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
  return js_export_callback_handler_ptr__ -> SetNamedProperty(JSObject(js_context, object_ref), JSString(property_name_ref), JSValue(js_context, value_ref));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

JSValueRef JSNativeClassPimpl::CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
  return js_export_callback_handler_ptr__ -> CallNamedFunction(JSObject(js_context, object_ref), JSObject(js_context, this_object_ref), ToJSValueVector(js_context, argument_count, arguments_array));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("CallNamedFunctionCallback", JSObject(js_context, function_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("CallNamedFunctionCallback", JSObject(js_context, function_ref)));
  *exception = JSValue(js_context, message);
}

void JSNativeClassPimpl::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  js_export_callback_handler_ptr__ -> Initialize(JSObject(JSContext(context_ref), object_ref));
} catch (const std::exception& e) {
  LogStdException("JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
  LogUnknownException("JSObjectInitializeCallback", JSObject(JSContext(context_ref), object_ref));
}

void JSNativeClassPimpl::JSObjectFinalizeCallback(JSObjectRef object_ref) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  // TODO: get context_ref
  //js_export_callback_handler_ptr__ -> Finalize(JSObject(JSContext(context_ref), object_ref));
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
  JSContext js_context(context_ref);
  return js_export_callback_handler_ptr__ -> CallAsFunction(JSObject(js_context, function_ref), JSObject(js_context, this_object_ref), ToJSValueVector(js_context, argument_count, arguments_array));
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
  JSContext js_context(context_ref);
  return js_export_callback_handler_ptr__ -> CallAsConstructor(JSObject(js_context, constructor_ref), ToJSValueVector(js_context, argument_count, arguments_array));
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
  JSContext js_context(context_ref);
  return js_export_callback_handler_ptr__ -> HasInstance(JSObject(js_context, constructor_ref), JSValue(js_context, possible_instance_ref));
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
  js_export_callback_handler_ptr__ -> ConvertToType(JSObject(JSContext(context_ref), object_ref), ToJSValueType(type));
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
  os << "JSClassPimpl<"
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
  os << "JSClassPimpl<"
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

}} // namespace JavaScriptCoreCPP { namespace detail

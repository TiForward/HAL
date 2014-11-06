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
#include "JavaScriptCoreCPP/JSPropertyNameAccumulator.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace detail {

JSClassPimpl::JSClassPimpl() {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;
	js_class_definition__ = kJSClassDefinitionEmpty;
	js_class_ref__        = JSClassCreate(&js_class_definition__);
}

void JSClassPimpl::Initialize(const JSExportNamedValuePropertyMap_t&    named_value_property_map,
                              const JSExportNamedFunctionPropertyMap_t& named_function_property_map) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD;
	
	js_class_definition__             = kJSClassDefinitionEmpty;

	js_class_definition__.version     = version__;
	js_class_definition__.attributes  = ToJSClassAttribute(class_attribute__);

	js_class_definition__.className   = name__.c_str();
	js_class_definition__.parentClass = parent__;

	// Initialize staticValues.
	if (!named_value_property_map.empty()) {
		for (const auto& entry : named_value_property_map) {
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
	if (!named_function_property_map.empty()) {
		for (const auto& entry : named_function_property_map) {
			const auto& function_name = entry.first;
			const auto& property_attributes = entry.second;
			JSStaticFunction js_static_function;
			js_static_function.name           = function_name.c_str();
			js_static_function.callAsFunction = CallNamedFunctionCallback;
			js_static_function.attributes     = ToJSPropertyAttributes(property_attributes);
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
	// js_class_definition__.callAsConstructor = JSObjectCallAsConstructorCallback;
	// js_class_definition__.hasInstance       = JSObjectHasInstanceCallback;
	// js_class_definition__.convertToType     = JSObjectConvertToTypeCallback;

	js_class_ref__ = JSClassCreate(&js_class_definition__);
}

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
std::recursive_mutex JSClassPimpl::mutex_static__;
#endif

JSExportCallbackHandlerMap_t JSClassPimpl::js_export_callback_handler_map__;

JSExportCallbackHandlerMap_t::key_type JSClassPimpl::get_js_export_callback_handler_map_key(void* data) {
	return reinterpret_cast<JSExportCallbackHandlerMap_t::key_type>(data);
}

JSExportCallbackHandler* JSClassPimpl::get_callback_handler_ptr(void* data) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassPimpl<", name__, ">::get_callback_handler_ptr ", to_string(js_object));

	const auto key      = get_js_export_callback_handler_map_key(data);
	const auto position = js_export_callback_handler_map__.find(key);
	const bool found    = (position != js_export_callback_handler_map__.end());

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassPimpl<", name__, ">::get_callback_handler_ptr ", to_string(js_object), ", found = ", std::to_string(found));

	//assert(found);
	if (found) {
		return (position -> second).get();
	}

	return nullptr;
}

JSExportCallbackHandler* JSClassPimpl::get_callback_handler_ptr(JSObject& js_object) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
	return get_callback_handler_ptr(js_object.GetPrivate());
}

/* The following nine callbacks are delegated to the callback handler. */ 

void JSClassPimpl::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  //return get_callback_handler_ptr(JSObject(JSContext(context_ref), object_ref)) -> Initialize();
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  if (js_object.GetPrivate()) {
	  //return get_callback_handler_ptr(js_object) -> Initialize();
  }
}

void JSClassPimpl::JSObjectFinalizeCallback(JSObjectRef object_ref) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
	void* native_object_ptr = JSObjectGetPrivate(object_ref);
	return get_callback_handler_ptr(native_object_ptr) -> Finalize(native_object_ptr);
}

// JSObjectRef JSClassPimpl::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
//   JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
//   JSContext js_context(context_ref);
//   JSObject  js_object(js_context, constructor_ref);
//   return get_callback_handler_ptr(js_object) -> CallAsConstructor(std::move(js_object), to_vector(js_context, argument_count, arguments_array));
// } catch (const std::exception& e) {
//   JSContext js_context(context_ref);
//   JSString message(LogStdException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref), e));
//   *exception = JSValue(js_context, message);
// } catch (...) {
//   JSContext js_context(context_ref);
//   JSString message(LogUnknownException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref)));
//   *exception = JSValue(js_context, message);
// }

// bool JSClassPimpl::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
//   JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
//   JSContext js_context(context_ref);
// 	JSObject  js_object(js_context, constructor_ref);
// 	return get_callback_handler_ptr(js_object) -> HasInstance(std::move(js_object), JSValue(js_context, possible_instance_ref));
// } catch (const std::exception& e) {
//   JSContext js_context(context_ref);
//   JSString message(LogStdException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
//   *exception = JSValue(js_context, message);
// } catch (...) {
//   JSContext js_context(context_ref);
//   JSString message(LogUnknownException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
//   *exception = JSValue(js_context, message);
// }

JSValueRef JSClassPimpl::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  return get_callback_handler_ptr(js_object) -> GetNamedProperty(std::move(js_object), JSString(property_name_ref));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

bool JSClassPimpl::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
	JSObject  js_object(js_context, object_ref);
	return get_callback_handler_ptr(js_object) -> SetNamedProperty(std::move(js_object), JSString(property_name_ref), JSValue(js_context, value_ref));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

JSValueRef JSClassPimpl::CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
	JSObject  js_object(js_context, function_ref);

	// TODO: Get function's name.
	//return get_callback_handler_ptr(js_object) -> CallNamedFunction(std::move(js_object), to_vector(js_context, argument_count, arguments_array), JSObject(js_context, this_object_ref));
	return nullptr;
	
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("CallNamedFunctionCallback", JSObject(js_context, function_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("CallNamedFunctionCallback", JSObject(js_context, function_ref)));
  *exception = JSValue(js_context, message);
}

JSValueRef JSClassPimpl::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
  JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
  JSContext js_context(context_ref);
	JSObject  js_object(js_context, function_ref);
	return get_callback_handler_ptr(js_object) -> CallAsFunction(std::move(js_object), to_vector(js_context, argument_count, arguments_array), JSObject(js_context, this_object_ref));
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref)));
  *exception = JSValue(js_context, message);
}

// JSValueRef JSClassPimpl::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
//   JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
//   JSContext js_context(context_ref);
//   JSObject  js_object(js_context, object_ref);
//   return get_callback_handler_ptr(js_object) -> ConvertToType(std::move(js_object), ToJSValueType(type));
// } catch (const std::exception& e) {
//   JSContext js_context(context_ref);
//   JSString message(LogStdException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
//   *exception = JSValue(js_context, message);
// } catch (...) {
//   JSContext js_context(context_ref);
//   JSString message(LogUnknownException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
//   *exception = JSValue(js_context, message);
// }


/* All of the following callbacks are simply delegated to JSObject.  */

bool JSClassPimpl::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  //return js_object.HasProperty(property_name_ref);
  return false;
} catch (const std::exception& e) {
  LogStdException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
  LogUnknownException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
}

JSValueRef JSClassPimpl::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  return js_object.GetProperty(property_name_ref);
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

bool JSClassPimpl::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  js_object.SetProperty(property_name_ref, JSValue(js_context, value_ref));
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

bool JSClassPimpl::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  return js_object.DeleteProperty(property_name_ref);
} catch (const std::exception& e) {
  JSContext js_context(context_ref);
  JSString message(LogStdException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref), e));
  *exception = JSValue(js_context, message);
} catch (...) {
  JSContext js_context(context_ref);
  JSString message(LogUnknownException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref)));
  *exception = JSValue(js_context, message);
}

void JSClassPimpl::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
  JSContext js_context(context_ref);
  JSObject  js_object(js_context, object_ref);
  return js_object.GetPropertyNames(JSPropertyNameAccumulator(property_names));
} catch (const std::exception& e) {
  LogStdException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
} catch (...) {
  LogUnknownException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
}

JSValue::Type JSClassPimpl::ToJSValueType(JSType type) {
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
			const std::string internal_component_name = "JSClassPimpl";
			const std::string message = "Unknown JSType " + std::to_string(type);
			ThrowLogicError(internal_component_name, message);
	}
}

std::string JSClassPimpl::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
  std::ostringstream os;
  os << "JSClassPimpl:: "
     <<  function_name
     << " for object "
     << to_string(js_object)
     << " threw exception: "
     << exception.what();
  
  const auto message = os.str();
  JAVASCRIPTCORECPP_LOG_ERROR(message);
  return message;
}

std::string JSClassPimpl::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
  std::ostringstream os;
  os << "JSClassPimpl:: "
     <<  function_name
     << " for object "
     << to_string(js_object)
     << " threw unknown exception";
  
  const auto message = os.str();
  JAVASCRIPTCORECPP_LOG_ERROR(message);
  return message;
}

}} // namespace JavaScriptCoreCPP { namespace detail

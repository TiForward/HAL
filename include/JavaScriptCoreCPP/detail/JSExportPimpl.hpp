/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"

#include "JavaScriptCoreCPP/detail/JSExportCallbackHandler.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <memory>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSExportPimpl final : public JSExportCallbackHandler, public detail::JSPerformanceCounter<JSExportPimpl> {
#else
class JSExportPimpl final : public JSExportCallbackHandler {
#endif

 public:

	/* Implement the JSExportCallbackHandler interface. */
	
	virtual JSValue  GetNamedProperty(JSObject&& object, JSString&& property_name)                                    const override;
	virtual bool     SetNamedProperty(JSObject&& object, JSString&& property_name, JSValue&& value)                         override;
	virtual JSValue  CallNamedFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)       override;
	
	virtual void     Initialize(JSObject&& object)                                                                          override;
	virtual void     Finalize(JSObject&& object)                                                                            override;
	virtual JSValue  CallAsFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)          override;
	virtual JSObject CallAsConstructor(JSObject&& constructor, std::vector<JSValue>&& arguments)                            override;
	virtual bool     HasInstance(JSObject&& constructor, JSValue&& possible_instance)                                 const override;
	virtual JSValue  ConvertToType(JSObject&& object, JSValue::JSType&& type)                                         const override;

	JSExportPimpl()                                = delete;
	~JSExportPimpl()                               = default;
	JSExportPimpl(const JSExportPimpl& rhs)        = delete;
	JSExportPimpl(JSExportPimpl&& rhs)             = delete;
	JSExportPimpl& operator=(const JSExportPimpl&) = delete;
	JSExportPimpl& operator=(JSExportPimpl&&)      = delete;

 private:

	// Only a JSClassBuilder can create us.
	template<typename U>
	friend class JSClassBuilder;

	template<typename U>
  JSExportPimpl(const JSClassBuilder<T>& builder);
	
	JSObjectNamedValuePropertyCallbackMap_t    named_value_property_callback_map__;
	JSObjectNamedFunctionPropertyCallbackMap_t named_function_property_callback_map__;

	InitializeCallback                         initialize_callback__             { nullptr };
	FinalizeCallback                           finalize_callback__               { nullptr };
	CallAsFunctionCallback                     call_as_function_callback__       { nullptr };
	CallAsConstructorCallback                  call_as_constructor_callback__    { nullptr };
	HasInstanceCallback                        has_instance_callback__           { nullptr };
	ConvertToTypeCallback                      convert_to_type_callback__        { nullptr };
};

template<typename T>
JSValue JSExportPimpl<T>::GetNamedProperty(JSObject&& object, JSString&& property_name) const {
  const auto callback_position = named_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != named_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::GetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
  
  // precondition
  assert(callback_found);
  
  const auto native_object_ptr = static_cast<JSExport*>(object.GetPrivate());
  const auto callback          = (callback_position -> second).get_get_property_callback();
  const auto result            = callback(*native_object_ptr);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::GetNamedProperty: result = ", to_string(result), " for ", to_string(object), ".", to_string(property_name));

  return result;
};
  
template<typename T>
bool JSExportPimpl<T>::SetNamedProperty(JSObject&& object, JSString&& property_name, JSValue&& value) {
  const auto callback_position = named_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != named_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::SetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));

  // precondition
  assert(callback_found);
  
  const auto native_object_ptr = static_cast<JSExport*>(object.GetPrivate());
  const auto callback          = (callback_position -> second).get_set_property_callback();
  const auto result            = callback(*native_object_ptr, value);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::SetNamedProperty: result = ", std::to_string(result), " for ", to_string(object), ".", to_string(property_name), " = ", to_string(value));
  
  return result;
};

template<typename T>
JSValue JSExportPimpl<T>::CallNamedFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  const auto callback_position = named_function_property_callback_map__.find(static_cast<JSString>(function));
  const bool callback_found    = callback_position != named_function_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallNamedFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  const auto native_object_ptr = static_cast<JSExport*>(function.GetPrivate());
  const auto callback          = (callback_position -> second).get_call_as_function_callback();
  const auto result            = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallNamedFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};
  
template<typename T>
void JSExportPimpl<T>::Initialize(JSObject&& object) {
  auto       callback       = initialize_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::Initialize: callback found = ", std::to_string(callback_found), " for ", to_string(object));
  
  // precondition
  assert(callback);
  
  const auto native_object_ptr = static_cast<JSExport*>(object.GetPrivate());
  callback(*native_object_ptr);
};
  
template<typename T>
void JSExportPimpl<T>::Finalize(JSObject&& object) {
  auto       callback       = finalize_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::Finalize: callback found = ", std::to_string(callback_found));
  
  // precondition
  assert(finalize_callback__);

  const auto native_object_ptr = static_cast<JSExport*>(object.GetPrivate());
  callback(*native_object_ptr);
};
  
template<typename T>
JSValue JSExportPimpl<T>::CallAsFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  auto       callback          = call_as_function_callback__;
  const bool callback_found    = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallAsFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  const auto native_object_ptr = static_cast<JSExport*>(function.GetPrivate());
  const auto callback          = (callback_position -> second).get_call_as_function_callback();
  const auto result            = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};

template<typename T>
JSObject JSExportPimpl<T>::CallAsConstructor(JSObject&& constructor, std::vector<JSValue>&& arguments) {
  auto       callback       = call_as_constructor_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallAsConstructor: callback found = ", std::to_string(callback_found), " for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");
  
  // precondition
  assert(callback);
  
  // precondition
  assert(constructor.IsConstructor());
  
  const auto native_object_ptr = static_cast<JSExport*>(constructor.GetPrivate());
  const auto result            = callback(*native_object_ptr, arguments);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::CallAsConstructor: result = ", to_string(result), " for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};
  
template<typename T>
bool JSExportPimpl<T>::HasInstance(JSObject&& constructor, JSValue&& possible_instance) const {
	// precondition
	try {
		static_cast<void>(dynamic_cast<const T&>(const));
		result = true;
	} catch (const ...) {
		detail::ThrowLogicError("JSExportPimpl<T>::HasInstance", to_string(constructor) + " is not in out inheritance hierarchy.");
	}
	
	bool result = false;
	try {
		static_cast<void>(dynamic_cast<const T&>(possible_instance));
		result = true;
	} catch (const ...) {
	}
	
	// auto       callback       = has_instance_callback__;
  // const bool callback_found = callback != nullptr;
  
	// JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::HasInstance: callback found = ", std::to_string(callback_found), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));

  // // precondition
  // assert(callback);
  
  // const auto native_object_ptr = static_cast<JSExport*>(constructor.GetPrivate());
  // const auto result = callback(*native_object_ptr, possible_instance);
  
	JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::HasInstance: result = ", std::to_string(result), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));

	return result;
};

template<typename T>
JSValue JSExportPimpl<T>::ConvertToType(JSObject&& object, JSValue::JSType&& type) const {
  auto       callback       = convert_to_type_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::ConvertToType: callback found = ", std::to_string(callback_found), " for ", to_string(object));

  // precondition
  assert(callback);
  
  const auto native_object_ptr = static_cast<JSExport*>(object.GetPrivate());
  const auto result = callback(*native_object_ptr, type);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", js_class_name__, ">::ConvertToType: result = ", to_string(result), " for converting ", to_string(object), " to ", to_string(type));

  return result;
};
	
	
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_HPP_

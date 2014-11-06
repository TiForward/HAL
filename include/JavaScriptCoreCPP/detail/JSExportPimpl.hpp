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

#include "JavaScriptCoreCPP/detail/JSExportCallbackHandler.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"
#include "JavaScriptCoreCPP/detail/JSClassPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <unordered_map>
#include <memory>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
using JSExportNamedValuePropertyCallbackMap_t    = std::unordered_map<std::string, JSExportNamedValuePropertyCallback<T>>;

template<typename T>
using JSExportNamedFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSExportNamedFunctionPropertyCallback<T>>;

template<typename U>
class JSClassBuilder;

template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSExportPimpl final : public JSExportCallbackHandler, public detail::JSPerformanceCounter<JSExportPimpl> {
#else
class JSExportPimpl final : public JSExportCallbackHandler {
#endif

 public:

	/* Implement the JSExportCallbackHandler interface. */
	
	virtual void     Initialize()                                                                                           override;
	virtual void     Finalize(void* native_object_ptr)                                                                      override;
	virtual JSObject CallAsConstructor(JSObject&& constructor, std::vector<JSValue>&& arguments)                            override;
	virtual bool     HasInstance(JSObject&& constructor, JSValue&& possible_instance)                                 const override;
	virtual JSValue  GetNamedProperty(JSObject&& object, JSString&& property_name)                                    const override;
	virtual bool     SetNamedProperty(JSObject&& object, JSString&& property_name, JSValue&& value)                         override;
	virtual JSValue  CallNamedFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)       override;
	virtual JSValue  CallAsFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)          override;
	virtual JSValue  ConvertToType(JSObject&& object, JSValue::Type&& type)                                           const override;
	

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

  JSExportPimpl(const JSClassBuilder<T>& builder);
	
	std::string                                   name__;
	JSExportNamedValuePropertyCallbackMap_t<T>    named_value_property_callback_map__;
	JSExportNamedFunctionPropertyCallbackMap_t<T> named_function_property_callback_map__;
	CallAsFunctionCallback<T>                     call_as_function_callback__    { nullptr };
	ConvertToTypeCallback<T>                      convert_to_type_callback__     { nullptr };
	JSExportCallbackHandlerMap_t::key_type        callback_handler_key__;

#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                          std::recursive_mutex       mutex__;
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

template<typename T>
void JSExportPimpl<T>::Initialize() {
JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::Initialize callback handler ", std::to_string(callback_handler_key__));
}

template<typename T>
void JSExportPimpl<T>::Finalize(void* native_object_ptr) {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;

	const auto callback_handler_key          = reinterpret_cast<JSExportCallbackHandlerMap_t::key_type>(native_object_ptr);
	const auto callback_handler_erase_result = JSClassPimpl::js_export_callback_handler_map__.erase(callback_handler_key);
	const bool callback_handler_unregistered = (callback_handler_erase_result == 1);
	JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::Finalize: native object deleted = ", std::to_string(callback_handler_unregistered));
	assert(callback_handler_unregistered);
	
	delete reinterpret_cast<T*>(native_object_ptr);
}

template<typename T>
JSObject JSExportPimpl<T>::CallAsConstructor(JSObject&& constructor, std::vector<JSValue>&& arguments) {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsConstructor for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");
	auto native_object_ptr = new T(arguments);
	constructor.SetPrivate(static_cast<void*>(native_object_ptr));
	return *native_object_ptr;
};
  
template<typename T>
bool JSExportPimpl<T>::HasInstance(JSObject&& constructor, JSValue&& possible_instance) const {
	if (possible_instance.IsObject()) {
		return false;
	}
	
	bool result = false;
	try {
		static_cast<void>(dynamic_cast<T*>(constructor.GetPrivate()));
		result = true;
	} catch (...) {
	}
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::HasInstance: result = ", std::to_string(result), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));

	return result;
};

template<typename T>
JSValue JSExportPimpl<T>::GetNamedProperty(JSObject&& object, JSString&& property_name) const {
  const auto callback_position = named_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != named_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::GetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
  
  // precondition
  assert(callback_found);

  auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
  const auto callback    = (callback_position -> second).get_get_property_callback();
  const auto result      = callback(*native_object_ptr);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::GetNamedProperty: result = ", to_string(result), " for ", to_string(object), ".", to_string(property_name));

  return result;
};
  
template<typename T>
bool JSExportPimpl<T>::SetNamedProperty(JSObject&& object, JSString&& property_name, JSValue&& value) {
  const auto callback_position = named_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != named_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::SetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));

  // precondition
  assert(callback_found);
  
  auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
  const auto callback    = (callback_position -> second).get_set_property_callback();
  const auto result      = callback(*native_object_ptr, value);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::SetNamedProperty: result = ", std::to_string(result), " for ", to_string(object), ".", to_string(property_name), " = ", to_string(value));
  
  return result;
};

template<typename T>
JSValue JSExportPimpl<T>::CallNamedFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  const auto callback_position = named_function_property_callback_map__.find(static_cast<JSString>(function));
  const bool callback_found    = callback_position != named_function_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallNamedFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
  const auto callback    = (callback_position -> second).get_call_as_function_callback();
  const auto result      = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallNamedFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};
  
template<typename T>
JSValue JSExportPimpl<T>::CallAsFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  auto       callback          = call_as_function_callback__;
  const bool callback_found    = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
  const auto result      = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};

template<typename T>
JSValue JSExportPimpl<T>::ConvertToType(JSObject&& object, JSValue::Type&& type) const {
  auto       callback       = convert_to_type_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::ConvertToType: callback found = ", std::to_string(callback_found), " for ", to_string(object));

  // precondition
  assert(callback_found);
  
  auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
  const auto result      = callback(*native_object_ptr, type);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::ConvertToType: result = ", to_string(result), " for converting ", to_string(object), " to ", to_string(type));

  return result;
};
	
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_HPP_

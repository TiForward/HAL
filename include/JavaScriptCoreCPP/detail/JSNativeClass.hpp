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
#include "JavaScriptCoreCPP/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"

#include "JavaScriptCoreCPP/detail/JSNativeClassPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectCallbackHandler.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectFunctionPropertyCallback.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <unordered_map>
#include <cassert>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

namespace JavaScriptCoreCPP { namespace detail {

template<typename T>
using JSNativeObjectValuePropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>;

template<typename T>
using JSNativeObjectFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>>;

template<typename T>
class JSClassBuilder;

/*!
  @class

  @discussion A JSNativeClass is an RAII wrapper around a JSClassRef,
  the JavaScriptCore C API representation of a JavaScript class that
  defines JavaScript objects implemented by instances of a C++ class.

  An instance of JSNativeClass may be passed to the JSContextGroup
  constructor to create a custom global object for all contexts in
  that group.

  JavaScript objects based on a JSNativeClass may be created by the
  JSContext::CreateObject member function.

  Only a JSClassBuilder may create instances of a JSNativeClass.

  This class is thread-safe and lock-free by design.
*/
template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeClass final : public JSClass, public JSNativeObjectCallbackHandler, public detail::JSPerformanceCounter<JSNativeClass> {
#else
class JSNativeClass final : public JSClass, public JSNativeObjectCallbackHandler {
#endif

 public:

  explicit JSNativeClass(JSNativeClassPimpl                                    js_native_class_pimpl,
                         const JSNativeObjectValuePropertyCallbackMap_t<T>&    native_value_property_callback_map
                         const JSNativeObjectFunctionPropertyCallbackMap_t<T>& native_function_property_callback_map)
      : JSClass(js_native_class_pimpl.js_class_definition__)
      , js_native_class_pimpl__(js_native_class_pimpl)
      , js_class_name__(js_native_class_pimpl.js_class_name__)
      , native_value_property_callback_map__(native_value_property_callback_map)
      , native_function_property_callback_map__(native_function_property_callback_map)
      );
  
  JSNativeClass()                                = delete;
  ~JSNativeClass()                               = default;
  JSNativeClass(const JSNativeClass& rhs)        = default;
  JSNativeClass(JSNativeClass&& rhs)             = default;
  JSNativeClass& operator=(const JSNativeClass&) = default;
  JSNativeClass& operator=(JSNativeClass&&)      = default;


  /* Implement the JSNativeObjectCallbackHandler interface. */
  
  virtual JSValue  GetNativeProperty(const JSObject& object, const JSString& property_name)                             const override final;
  virtual bool     SetNativeProperty(JSObject object, const JSString& property_name, const JSValue& value)                    override final;
  virtual JSValue  CallAsNativeFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments)       override final;

  virtual void     Initialize(JSObject object)                                                                                override final;
  virtual void     Finalize(JSObject object)                                                                                  override final;
  virtual JSValue  CallAsFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments)             override final;
  virtual JSObject CallAsConstructor(JSObject constructor, const std::vector<JSValue>& arguments)                             override final;
  virtual bool     HasInstance(const JSObject& constructor, const JSValue& possible_instance)                           const override final;
  virtual JSValue  ConvertToType(const JSObject& object, const JSValue::JSType& type)                                   const override final;

 private:
  
  JSNativeClassPimpl                             js_native_class_pimpl__;
  std::string                                    js_class_name__;
  JSNativeObjectValuePropertyCallbackMap_t<T>    native_value_property_callback_map__;
  JSNativeObjectFunctionPropertyCallbackMap_t<T> native_function_property_callback_map__;

  // The following six callbacks are set directly by
  // JSClassBuilder<T>. They must be static and set by the builder
  // before we are constructed since they are used in initializing the
  // JavaScriptCore C API JSClassDefinition. They are thread safe and
  // require no synchronization since they are only member function
  // addresses.
  template<typename U>
  friend class JSClassBuilder;
  
  static InitializeCallback<T>        initialize_callback__;
  static FinalizeCallback<T>          finalize_callback__;
  static CallAsFunctionCallback<T>    call_as_function_callback__;
  static CallAsConstructorCallback<T> call_as_constructor_callback__;
  static HasInstanceCallback<T>       has_instance_callback__;
  static ConvertToTypeCallback<T>     convert_to_type_callback__;
};

template<typename T> InitializeCallback<T>        JSNativeClass<T>::initialize_callback__          { nullptr };
template<typename T> FinalizeCallback<T>          JSNativeClass<T>::finalize_callback__            { nullptr };
template<typename T> CallAsFunctionCallback<T>    JSNativeClass<T>::call_as_function_callback__    { nullptr };
template<typename T> CallAsConstructorCallback<T> JSNativeClass<T>::call_as_constructor_callback__ { nullptr };
template<typename T> HasInstanceCallback<T>       JSNativeClass<T>::has_instance_callback__        { nullptr };
template<typename T> ConvertToTypeCallback<T>     JSNativeClass<T>::convert_to_type_callback__     { nullptr };

template<typename T>
JSValue JSNativeClass<T>::GetNativeProperty(const JSObject& object, const JSString& property_name) const {
  const auto callback_position = native_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != native_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::GetNativeProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
  
  // precondition
  assert(callback_found);
  
  const auto native_object_ptr = static_cast<T*>(object.GetPrivate());
  const auto callback          = (callback_position -> second).get_get_property_callback();
  const auto result            = callback(*native_object_ptr);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::GetNativeProperty: result = ", to_string(result), " for ", to_string(object), ".", to_string(property_name));

  return result;
};
  
template<typename T>
bool JSNativeClass<T>::SetNativeProperty(JSObject object, const JSString& property_name, const JSValue& value) {
  const auto callback_position = native_value_property_callback_map__.find(property_name);
  const bool callback_found    = callback_position != native_value_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::SetNativeProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));

  // precondition
  assert(callback_found);
  
  const auto native_object_ptr = static_cast<T*>(object.GetPrivate());
  const auto callback          = (callback_position -> second).get_set_property_callback();
  const auto result            = callback(*native_object_ptr, value);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::SetNativeProperty: result = ", std::to_string(result), " for ", to_string(object), ".", to_string(property_name), " = ", to_string(value));
  
  return result;
};

template<typename T>
JSValue JSNativeClass<T>::CallAsNativeFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  const auto callback_position = native_function_property_callback_map__.find(static_cast<JSString>(function));
  const bool callback_found    = callback_position != native_function_property_callback_map__.end();
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsNativeFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  const auto native_object_ptr = static_cast<T*>(function.GetPrivate());
  const auto callback          = (callback_position -> second).get_call_as_function_callback();
  const auto result            = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsNativeFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};
  
template<typename T>
void JSNativeClass<T>::Initialize(JSObject object) {
  auto       callback       = initialize_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::Initialize: callback found = ", std::to_string(callback_found), " for ", to_string(object));
  
  // precondition
  assert(callback);
  
  const auto native_object_ptr = static_cast<T*>(object.GetPrivate());
  callback(*native_object_ptr);
};
  
template<typename T>
void JSNativeClass<T>::Finalize(JSObject object) {
  auto       callback       = finalize_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::Finalize: callback found = ", std::to_string(callback_found));
  
  // precondition
  assert(finalize_callback__);

  const auto native_object_ptr = static_cast<T*>(object.GetPrivate());
  callback(*native_object_ptr);
};
  
template<typename T>
JSValue JSNativeClass<T>::CallAsFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments) {
  // precondition
  assert(function.IsFunction());
  
  // Will this work?
  auto       callback          = call_as_function_callback__;
  const bool callback_found    = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  // precondition
  assert(callback_found);

  const auto native_object_ptr = static_cast<T*>(function.GetPrivate());
  const auto callback          = (callback_position -> second).get_call_as_function_callback();
  const auto result            = callback(*native_object_ptr, arguments, this_object);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};

template<typename T>
JSObject JSNativeClass<T>::CallAsConstructor(JSObject constructor, const std::vector<JSValue>& arguments) {
  auto       callback       = call_as_constructor_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsConstructor: callback found = ", std::to_string(callback_found), " for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");
  
  // precondition
  assert(callback);
  
  // precondition
  assert(constructor.IsConstructor());
  
  const auto native_object_ptr = static_cast<T*>(constructor.GetPrivate());
  const auto result            = callback(*native_object_ptr, arguments);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::CallAsConstructor: result = ", to_string(result), " for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");

  return result;
};
  
template<typename T>
bool JSNativeClass<T>::HasInstance(const JSObject& constructor, const JSValue& possible_instance) const {
  auto       callback       = has_instance_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::HasInstance: callback found = ", std::to_string(callback_found), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));

  // precondition
  assert(callback);
  
  const auto native_object_ptr = static_cast<T*>(constructor.GetPrivate());
  const auto result = callback(*native_object_ptr, possible_instance);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::HasInstance: result = ", std::to_string(result), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));

  return result;
};

template<typename T>
JSValue JSNativeClass<T>::ConvertToType(const JSObject& object, const JSValue::JSType& type) const {
  auto       callback       = convert_to_type_callback__;
  const bool callback_found = callback != nullptr;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::ConvertToType: callback found = ", std::to_string(callback_found), " for ", to_string(object));

  // precondition
  assert(callback);
  
  const auto native_object_ptr = static_cast<T*>(object.GetPrivate());
  const auto result = callback(*native_object_ptr, type);
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">::ConvertToType: result = ", to_string(result), " for converting ", to_string(object), " to ", to_string(type));

  return result;
};

} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASS_HPP_

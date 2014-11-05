/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_HPP_

#include "JavaScriptCoreCPP/JSClassAttribute.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

extern "C" {
  struct JSClassDefinition;
	struct JSStaticValue;
	struct JSStaticFunction;
	struct JSContextRef;
	struct JSObjectRef;
	struct JSStringRef;
	struct JSValueRef;
	struct JSPropertyNameAccumulatorRef;
	// JSType
}

namespace JavaScriptCoreCPP { namespace detail {

using JSObjectNamedValuePropertyCallbackMap_t    = std::unordered_map<std::string, JSObjectNamedValuePropertyCallback>;
using JSObjectNamedFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSObjectNamedFunctionPropertyCallback>;

class JSString;
class Object;

class JSClassPimpl final {
  
 public:

	JSClassPimpl()                               = delete;
	~JSClassPimpl();
	JSClassPimpl(const JSClassPimpl&)            = delete;
	JSClassPimpl(JSClassPimpl&&)                 = delete;
	JSClassPimpl& operator=(const JSClassPimpl&) = delete;
	JSClassPimpl& operator=(JSClassPimpl&&)      = delete;
  
 private:

  // Only a JSClassBuilder can create us..
  template<typename T>
  friend class JSClassBuilder;

  template<typename T>
  JSClassPimpl(const JSClassBuilder<T>& builder);

  void Initialize(const JSObjectNamedValuePropertyCallbackMap_t&    named_value_property_callback_map
                  const JSObjectNamedFunctionPropertyCallbackMap_t& named_function_property_callback_map);

  // JSClass needs access to the following three methods.
  friend JSClass;
  
  std::string get_name() const;
	std::uint32_t get_version() const;

  // For interoperability with the JavaScriptCore C API.
  operator JSClassRef() const;

  std::uint32_t                            version__         { 0 };
	JSClassAttribute                         class_attribute__ { JSClassAttribute::None };
	JSString                                 name__;
	JSClass                                  parent__;
  std::vector<JSStaticValue>               js_static_values__;
  std::vector<JSStaticFunction>            js_static_functions__;
  std::unique_ptr<JSExportCallbackHandler> js_export_callback_handler_ptr__;
  JSClassDefinition                        js_class_definition__;
	JSClassRef                               js_class_ref__ { nullptr };

	// Support for JSStaticValue
  static JSValueRef  GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception);
  static bool        SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception);
  
  // Support for JSStaticFunction
  static JSValueRef  CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
  
  // JavaScriptCore C API callback interface.
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
  static JSValue::Type ToJSValueType(JSType type);

  static JSClass                                  default_js_class_;
  static std::unordered_map<std::string, JSClass> js_class_map_;


  
#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD
#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                                std::recursive_mutex       mutex__
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD        std::lock_guard<std::recursive_mutex> lock(mutex__)
static                                                                          std::recursive_mutex              mutex_static__;
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(mutex_static__);
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

}} // namespace JavaScriptCoreCPP { namespace detail {

namespace std {
using JavaScriptCoreCPP::detail::JSClassPimpl;
template<>
void swap<JSClassPimpl>(JSClassPimpl& first, JSClassPimpl& second) noexcept {
  first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_HPP_

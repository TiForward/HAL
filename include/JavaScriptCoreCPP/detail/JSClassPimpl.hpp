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

#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/JSClassAttribute.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbackHandler.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <string>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {
// class JSString;
class JSClass;
// class JSObject;
}

namespace JavaScriptCoreCPP { namespace detail {

using JSExportNamedValuePropertyMap_t    = std::unordered_map<std::string, std::unordered_set<JSPropertyAttribute>>;
using JSExportNamedFunctionPropertyMap_t = std::unordered_map<std::string, std::unordered_set<JSPropertyAttribute>>;
using JSExportCallbackHandlerMap_t       = std::unordered_map<std::intptr_t, std::shared_ptr<JSExportCallbackHandler>>;

template<typename T>
class JSClassBuilder;

template<typename T>
class JSExportPimpl;

class JSClassPimpl final {
  
 public:

  // Default constructor for implementing JSClass::EmptyJSClass.
  JSClassPimpl();

  ~JSClassPimpl() {
		JSClassRelease(js_class_ref__);
	}
	
	JSClassPimpl(const JSClassPimpl&)            = delete;
	JSClassPimpl(JSClassPimpl&&)                 = delete;
	JSClassPimpl& operator=(const JSClassPimpl&) = delete;
	JSClassPimpl& operator=(JSClassPimpl&&)      = delete;
  
  std::string get_name() const {
	  return name__;
  }
  
  std::uint32_t get_version() const {
	  return version__;
  }
  
  // For interoperability with the JavaScriptCore C API.
  operator JSClassRef() const {
	  return js_class_ref__;
  }

 private:

	// Only a JSClassBuilder can create us.
  template<typename T>
  friend class JSClassBuilder;

  template<typename T>
  JSClassPimpl(const JSClassBuilder<T>& builder);

  void Initialize(const JSExportNamedValuePropertyMap_t&    named_value_property_map,
                  const JSExportNamedFunctionPropertyMap_t& named_function_property_map);

  std::uint32_t                            version__         { 0 };
	JSClassAttribute                         class_attribute__ { JSClassAttribute::None };
 	std::string                              name__;
	JSClassRef                               parent__;
  std::vector<JSStaticValue>               js_static_values__;
  std::vector<JSStaticFunction>            js_static_functions__;
  JSClassDefinition                        js_class_definition__;
	JSClassRef                               js_class_ref__ { nullptr };
	JSExportCallbackHandlerMap_t::key_type   callback_handler_key__;

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
  static std::string   LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
  static std::string   LogUnknownException(const std::string& function_name, const JSObject& js_object);

  static JSExportCallbackHandlerMap_t::key_type get_js_export_callback_handler_map_key(void* data);
  static JSExportCallbackHandler*               get_callback_handler_ptr(void* data);
  static JSExportCallbackHandler*               get_callback_handler_ptr(JSObject& js_object);

  // JSExportPimpl needs access to the
  // js_export_callback_handler_map__.
  template<typename T>
	friend class JSExportPimpl;

  static JSExportCallbackHandlerMap_t js_export_callback_handler_map__;

#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD
#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
                                                                                std::recursive_mutex       mutex__;
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD        std::lock_guard<std::recursive_mutex> lock(mutex__)
static                                                                          std::recursive_mutex                            mutex_static__;
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(JSClassPimpl::mutex_static__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_HPP_

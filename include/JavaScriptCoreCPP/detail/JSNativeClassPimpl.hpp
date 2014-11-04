/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSPIMPL_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSPIMPL_HPP_

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeObjectCallbackHandler.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeClassAttribute.hpp"

#include <vector>
#include <memory>
#include <cstddef>

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

class JSNativeClassPimpl final {
  
 public:

	JSNativeClassPimpl()                                     = delete;
  ~JSNativeClassPimpl()                                    = default;
  JSNativeClassPimpl(const JSNativeClassPimpl&)            = default;
  JSNativeClassPimpl(JSNativeClassPimpl&&)                 = default;
  JSNativeClassPimpl& operator=(const JSNativeClassPimpl&) = default;
  JSNativeClassPimpl& operator=(JSNativeClassPimpl&&)      = default;
  
private:

  explicit JSNativeClassPimpl(const std::string                 js_class_name,
                              uint32_t                          js_class_version,
                              JSClassAttribute                  js_class_attribute,
                              const JSClass&                    js_class_parent,
                              JSNativeObjectStaticPropertyMap_t native_value_property_map,
                              JSNativeObjectStaticPropertyMap_t native_function_property_map);
  
  // JSClassBuilder<T> needs access to take the address of our private
  // static functions, call our constructor and set our callback
  // handler.
  template<typename T>
  friend class JSClassBuilder;

  // The JSNativeClass<T> constructor needs access to our
  // js_class_definition__ to pass to its JSClass base class
  // constructor.
  template<typename T>
  friend class JSNativeClass;

  std::string                       js_class_name__;
  JSNativeObjectStaticPropertyMap_t native_value_property_map__;
  JSNativeObjectStaticPropertyMap_t native_function_property_map__;
  std::vector<JSStaticValue>        js_static_values__;
  std::vector<JSStaticFunction>     js_static_functions__;
	JSClassDefinition                 js_class_definition__;

	std::weak_ptr<JSNativeObjectCallbackHandler> callback_handler_weak_ptr__;

	// Support for JSStaticValue
  static JSValueRef  JSStaticValueGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception);
  static bool        JSStaticValueSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception);
  
  // Support for JSStaticFunction
  static JSValueRef  JSStaticFunctionCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception);
  
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

#undef JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSPIMPL_STATIC_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
  static                                                                              std::recursive_mutex              static_mutex__;
#define JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSPIMPL_STATIC_LOCK_GUARD std::lock_guard<std::recursive_mutex> static_lock(static_mutex__);
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
};

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVECLASSPIMPL_HPP_

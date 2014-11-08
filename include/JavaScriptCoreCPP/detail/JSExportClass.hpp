/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/detail/JSExportClassDefinition.hpp"

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSUndefined.hpp"

//#include "JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp"
//#include "JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp"
//#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"
//
//#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
//#include "JavaScriptCoreCPP/JSClassAttribute.hpp"

#include "JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp"

#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>

namespace JavaScriptCoreCPP { namespace detail {
  
  template<typename T>
  class JSExportClass;
  
  template<typename T>
  using JSExportCallbackMap_t = std::unordered_map<std::intptr_t, JSExportClass<T>*>;
  
  template<typename T>
  class JSExportClassDefinitionBuilder;
  
  template<typename T>
  class JSExportClassDefinition;
  
  template<typename T>
  class JSExportClass : public JSClass JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSExportClass<T>) {
    
  public:
    
    JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) noexcept;
    
    JSExportClass()                                = default;
    ~JSExportClass()                               = default;
    JSExportClass(const JSExportClass& rhs)        = default;
    JSExportClass(JSExportClass&& rhs)             = default;
    JSExportClass& operator=(const JSExportClass&) = default;
    JSExportClass& operator=(JSExportClass&&)      = default;
    
  private:
    
    void     Initialize(JSObject&& object);
    JSValue  GetNamedProperty(JSObject&& object, const JSString&& property_name) const;
    bool     SetNamedProperty(JSObject&& object, const JSString&& property_name, const JSValue&& value);
    JSValue  CallNamedFunction(JSObject&& function, const JSString&& function_name, const std::vector<JSValue>&& arguments, JSObject&& this_object);
    JSValue  CallAsFunction(JSObject&& function, const std::vector<JSValue>&& arguments, JSObject&& this_object);
    JSObject CallAsConstructor(JSObject&& constructor, const std::vector<JSValue>&& arguments);
    bool     HasInstance(JSObject&& constructor, const JSValue&& possible_instance) const;
    // JSValue  ConvertToType(JSObject&& object, const JSValue::Type&& type) const final;
    
    JSExportNamedValuePropertyCallbackMap_t<T>    named_value_property_callback_map__;
    JSExportNamedFunctionPropertyCallbackMap_t<T> named_function_property_callback_map__;
    
    // JSExportClassDefinitionBuilder needs access in order to take
    // the address of our non-static functions that implement the
    // JSExport callbacks.
    template<typename U>
    friend class JSExportClassDefinition;
    
    // JSExportClassDefinitionBuilder needs access in order to take
    // the address of our static functions that implement the
    // JavaScriptCore C API callbacks.
    template<typename U>
    friend class JSExportClassDefinitionBuilder;
    
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
    // static JSValueRef  JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception);
    
    // Helper functions.
    static JSExportClass* get_callback_ptr(void* data);
    static JSExportClass* get_callback_ptr(JSObject& js_object);
    static JSValue::Type  ToJSValueType(JSType type);
    static std::string    LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
    static std::string    LogUnknownException(const std::string& function_name, const JSObject& js_object);
    
    static JSExportCallbackMap_t<T> js_export_callback_map__;
    
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
    
    std::recursive_mutex mutex__;
    static std::recursive_mutex mutex_static__;
    
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD        std::lock_guard<std::recursive_mutex> lock(mutex__)
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(JSExportClass::mutex_static__)
    
#else
    
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
    
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
  template<typename T>
  std::recursive_mutex JSExportClass<T>::mutex_static__;
#endif
  
  template<typename T>
  JSExportCallbackMap_t<T> JSExportClass<T>::js_export_callback_map__;
  
  template<typename T>
  JSExportClass<T>::JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) noexcept
  : JSClass(js_export_class_definition)
  , named_value_property_callback_map__(js_export_class_definition.get_named_value_property_callback_map())
  , named_function_property_callback_map__(js_export_class_definition.get_named_function_property_callback_map()) {
    // We are callback 0 so that we can intercept all Initialize callbacks.
    const auto key                      = reinterpret_cast<typename JSExportCallbackMap_t<T>::key_type>(nullptr);
    const auto callback_insert_result   = js_export_callback_map__.emplace(key, this);
    const bool callback_registered      = callback_insert_result.second;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">: callback ", std::to_string(key), " registered = ", std::to_string(callback_registered));
    
    assert(callback_registered);
  }
  
  template<typename T>
  void JSExportClass<T>::Initialize(JSObject&& object) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    object.SetPrivate(new T(object.get_context()));
    
    const auto key                    = reinterpret_cast<typename JSExportCallbackMap_t<T>::key_type>(object.GetPrivate());
    const auto callback_insert_result = js_export_callback_map__.emplace(key, this);
    const bool callback_registered    = callback_insert_result.second;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::Initialize: callback ", std::to_string(key), " registered = ", std::to_string(callback_registered));
    assert(callback_registered);
  }
  
  template<typename T>
  JSValue JSExportClass<T>::GetNamedProperty(JSObject&& object, const JSString&& property_name) const {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    const auto callback_position = named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != named_value_property_callback_map__.end();
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::GetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
    const auto callback    = (callback_position -> second).get_callback();
    const auto result      = callback(*native_object_ptr);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::GetNamedProperty: result = ", to_string(result), " for ", to_string(object), ".", to_string(property_name));
    
    return result;
  };
  
  template<typename T>
  bool JSExportClass<T>::SetNamedProperty(JSObject&& object, const JSString&& property_name, const JSValue&& value) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    const auto callback_position = named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != named_value_property_callback_map__.end();
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::SetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
    const auto callback    = (callback_position -> second).set_callback();
    const auto result      = callback(*native_object_ptr, std::move(value));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::SetNamedProperty: result = ", std::to_string(result), " for ", to_string(object), ".", to_string(property_name), " = ", to_string(value));
    
    return result;
  };
  
  template<typename T>
  JSValue JSExportClass<T>::CallNamedFunction(JSObject&& function, const JSString&& function_name, const std::vector<JSValue>&& arguments, JSObject&& this_object) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    // precondition
    assert(function.IsFunction());
    
    const auto callback_position = named_function_property_callback_map__.find(static_cast<JSString>(function_name));
    const bool callback_found    = callback_position != named_function_property_callback_map__.end();
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::CallNamedFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
    const auto callback    = (callback_position -> second).function_callback();
    const auto result      = callback(*native_object_ptr, std::move(arguments), std::move(this_object));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::CallNamedFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
    
    return result;
  };
  
  template<typename T>
  JSValue JSExportClass<T>::CallAsFunction(JSObject&& function, const std::vector<JSValue>&& arguments, JSObject&& this_object) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    // precondition
    assert(function.IsFunction());
    
    auto       callback       = js_class_definition__.js_class_definition__.callAsFunction;
    const bool callback_found = callback != nullptr;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::CallAsFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
    const auto result      = callback(*native_object_ptr, std::move(arguments), std::move(this_object));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
    
    return result;
  };
  
  template<typename T>
  JSObject JSExportClass<T>::CallAsConstructor(JSObject&& constructor, const std::vector<JSValue>&& arguments) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::CallAsConstructor for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");
    
    // TODO
    return constructor.get_context().CreateUndefined();
  };
  
  template<typename T>
  bool JSExportClass<T>::HasInstance(JSObject&& constructor, const JSValue&& possible_instance) const {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
    
    if (possible_instance.IsObject()) {
      return false;
    }
    
    bool result = false;
    try {
      auto native_object_ptr = dynamic_cast<T*>(reinterpret_cast<T*>(constructor.GetPrivate()));
      static_cast<void>(native_object_ptr);
      result = true;
    } catch (...) {
    }
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::HasInstance: result = ", std::to_string(result), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));
    
    return result;
  };
  
  //    template<typename T>
  //    JSValue JSExportClass<T>::ConvertToType(JSObject&& object, const JSValue::Type&& type) const {
  //      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD;
  //
  //      auto       callback       = convert_to_type_callback__;
  //      const bool callback_found = callback != nullptr;
  //
  //      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::ConvertToType: callback found = ", std::to_string(callback_found), " for ", to_string(object));
  //
  //      // precondition
  //      assert(callback_found);
  //
  //      auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
  //      const auto result      = callback(*native_object_ptr, type);
  //
  //      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass<", get_name(), ">::ConvertToType: result = ", to_string(result), " for converting ", to_string(object), " to ", to_string(type));
  //
  //      return result;
  //    };
  
  
  /* Static functions begin here. */
  
  template<typename T>
  JSExportClass<T>* JSExportClass<T>::get_callback_ptr(void* data) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    
    auto key      = reinterpret_cast<typename JSExportCallbackMap_t<T>::key_type>(data);
    auto position = js_export_callback_map__.find(key);
    bool found    = (position != js_export_callback_map__.end());
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::get_callback_ptr callback ", std::to_string(key), " found = ", std::to_string(found));
    
    if (found) {
      return position -> second;
    }
    
    // Fallback to callback 0.
    key      = reinterpret_cast<typename JSExportCallbackMap_t<T>::key_type>(nullptr);
    position = js_export_callback_map__.find(key);
    found    = (position != js_export_callback_map__.end());
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::get_callback_ptr callback ", std::to_string(key), " found = ", std::to_string(found));
    
    assert(found);
    return position -> second;
  }
  
  template<typename T>
  JSExportClass<T>* JSExportClass<T>::get_callback_ptr(JSObject& js_object) {
    return get_callback_ptr(js_object.GetPrivate());
  }
  
  
  /* Implementations of the JavaScriptCore C API callbacks begin here. */
  
  template<typename T>
  void JSExportClass<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectInitializeCallback");
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    
    get_callback_ptr(js_object) -> Initialize(std::move(js_object));
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    
    auto       native_object_ptr = JSObjectGetPrivate(object_ref);
    const auto key               = reinterpret_cast<typename JSExportCallbackMap_t<T>::key_type>(native_object_ptr);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: delete native object ", std::to_string(key));
    delete reinterpret_cast<T*>(native_object_ptr);
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: native object ", std::to_string(key), "deleted");
    
    const auto callback_erase_result = js_export_callback_map__.erase(key);
    const bool callback_unregistered = (callback_erase_result == 1);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: callback ", std::to_string(key), "un-registered = ", std::to_string(callback_unregistered));
    
    assert(callback_unregistered);
  }
  
  
  /* FIXME: ALl of the following are broken. */
  
  template<typename T>
  JSValueRef JSExportClass<T>::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    return get_callback_ptr(js_object) -> GetNamedProperty(std::move(js_object), JSString(property_name_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("GetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    return get_callback_ptr(js_object) -> SetNamedProperty(std::move(js_object), JSString(property_name_ref), JSValue(js_context, value_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("SetNamedValuePropertyCallback", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, function_ref);
    
    // TODO:
    // const JSString&& function_name = ...
    // return get_callback_ptr(js_object) -> CallNamedFunction(std::move(js_object), function_name, to_vector(js_context, argument_count, arguments_array), JSObject(js_context, this_object_ref));
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
  
  template<typename T>
  bool JSExportClass<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    
    return js_object.HasProperty(JSString(property_name_ref));
    return false;
    
  } catch (const std::exception& e) {
    LogStdException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
  } catch (...) {
    LogUnknownException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    return js_object.GetProperty(JSString(property_name_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectGetPropertyCallback", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    js_object.SetProperty(JSString(property_name_ref), JSValue(js_context, value_ref));
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
  
  template<typename T>
  bool JSExportClass<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    return js_object.DeleteProperty(JSString(property_name_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectDeletePropertyCallback", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    return js_object.GetPropertyNames(JSPropertyNameAccumulator(property_names));
  } catch (const std::exception& e) {
    LogStdException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
  } catch (...) {
    LogUnknownException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, function_ref);
    return get_callback_ptr(js_object) -> CallAsFunction(std::move(js_object), to_vector(js_context, argument_count, arguments_array), JSObject(js_context, this_object_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectCallAsFunctionCallback", JSObject(js_context, function_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  JSObjectRef JSExportClass<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    return get_callback_ptr(js_object) -> CallAsConstructor(std::move(js_object), to_vector(js_context, argument_count, arguments_array));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    return get_callback_ptr(js_object) -> HasInstance(std::move(js_object), JSValue(js_context, possible_instance_ref));
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
    *exception = JSValue(js_context, message);
  }
  
  //  template<typename T>
  //  JSValueRef JSExportClass<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
  //    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
  //    JSContext js_context(context_ref);
  //    JSObject  js_object(js_context, object_ref);
  //    return get_callback_ptr(js_object) -> ConvertToType(std::move(js_object), ToJSValueType(type));
  //  } catch (const std::exception& e) {
  //    JSContext js_context(context_ref);
  //    JSString message(LogStdException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
  //    *exception = JSValue(js_context, message);
  //  } catch (...) {
  //    JSContext js_context(context_ref);
  //    JSString message(LogUnknownException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
  //    *exception = JSValue(js_context, message);
  //  }
  
  template<typename T>
  JSValue::Type JSExportClass<T>::ToJSValueType(JSType type) {
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
        const std::string internal_component_name = "JSExportClass<T>";
        const std::string message = "Unknown JSType " + std::to_string(type);
        ThrowLogicError(internal_component_name, message);
    }
  }
  
  template<typename T>
  std::string JSExportClass<T>::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
    std::ostringstream os;
    os << "JSExportClass<T>:: "
    <<  function_name
    << " for object "
    << to_string(js_object)
    << " threw exception: "
    << exception.what();
    
    const auto message = os.str();
    JAVASCRIPTCORECPP_LOG_ERROR(message);
    return message;
  }
  
  template<typename T>
  std::string JSExportClass<T>::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
    std::ostringstream os;
    os << "JSExportClass<T>:: "
    <<  function_name
    << " for object "
    << to_string(js_object)
    << " threw unknown exception";
    
    const auto message = os.str();
    JAVASCRIPTCORECPP_LOG_ERROR(message);
    return message;
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_HPP_

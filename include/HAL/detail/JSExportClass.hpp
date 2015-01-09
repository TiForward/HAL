/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_DETAIL_JSEXPORTCLASS_HPP_
#define _HAL_DETAIL_JSEXPORTCLASS_HPP_

#include "HAL/detail/JSBase.hpp"
#include "HAL/JSClass.hpp"
#include "HAL/detail/JSExportClassDefinition.hpp"

#include "HAL/JSString.hpp"
#include "HAL/JSObject.hpp"
#include "HAL/JSUndefined.hpp"

#include "HAL/detail/JSPropertyNameAccumulator.hpp"
#include "HAL/detail/JSUtil.hpp"
#include "HAL/detail/JSValueUtil.hpp"

#include <string>
#include <regex>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>
#include <typeinfo>
#include <typeindex>

namespace HAL {
  template<typename T>
  class JSExport;
}

namespace HAL { namespace detail {
  
  
  template<typename T>
  class JSExportClassDefinitionBuilder;
  
  template<typename T>
  class JSExportClassDefinition;
  
  /*!
   @class
   
   @discussion A JSExportClass is an RAII wrapper around a JSClassRef,
   the JavaScriptCore C API representation of a JavaScript class. A
   JSExportClass defines JavaScript objects implemented by a C++
   class.
   
   This class is thread-safe and lock-free by design.
   */
  template<typename T>
  class JSExportClass final : public JSClass HAL_PERFORMANCE_COUNTER2(JSExportClass<T>) {
    
  public:
    
    JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) HAL_NOEXCEPT;
    
    JSExportClass()                                HAL_NOEXCEPT;//= default;
    ~JSExportClass()                               HAL_NOEXCEPT;//= default;
    JSExportClass(const JSExportClass&)            = default;
    JSExportClass& operator=(const JSExportClass&) = default;
    
#ifdef HAL_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
    JSExportClass(JSExportClass&&)                 = default;
    JSExportClass& operator=(JSExportClass&&)      = default;
#endif
    
  private:
    
    void Print() const;
    
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
    static JSValueRef  JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception);
    
    // Helper functions.
    static std::string LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
    static std::string LogUnknownException(const std::string& function_name, const JSObject& js_object);
    
    static JSExportClassDefinition<T> js_export_class_definition__;
    
#undef HAL_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
#ifdef HAL_THREAD_SAFE
    static std::recursive_mutex mutex_static__;
#define HAL_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(JSExportClass::mutex_static__)
#else
#define HAL_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
#endif  // HAL_THREAD_SAFE
  };
  
#ifdef HAL_THREAD_SAFE
  template<typename T>
  std::recursive_mutex JSExportClass<T>::mutex_static__;
#endif
  
  template<typename T>
  JSExportClassDefinition<T> JSExportClass<T>::js_export_class_definition__;
  
  template<typename T>
  JSExportClass<T>::JSExportClass() HAL_NOEXCEPT {
	  HAL_LOG_TRACE("JSExportClass<", typeid(T).name(), ">:: ctor 1 ", this);
  }

  template<typename T>
  JSExportClass<T>::JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) HAL_NOEXCEPT
  : JSClass(js_export_class_definition) {
    HAL_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    HAL_LOG_TRACE("JSExportClass<", typeid(T).name(), ">:: ctor 2 ", this);
    js_export_class_definition__ = js_export_class_definition;
    //js_export_class_definition__.Print();
  }
  
  template<typename T>
  JSExportClass<T>::~JSExportClass() HAL_NOEXCEPT {
	  HAL_LOG_TRACE("JSExportClass<", typeid(T).name(), ">:: dtor ", this);
  }
  

  template<typename T>
  void JSExportClass<T>::Print() const {
    HAL_JSCLASS_LOCK_GUARD;
    for (const auto& entry : js_export_class_definition__.named_value_property_callback_map__) {
      const auto& name       = entry.first;
      const auto& attributes = entry.second.get_attributes();
      HAL_LOG_DEBUG("JSExportClass: has value property callback ", name, " with attributes ", to_string(attributes));
    }
    
    for (const auto& entry : js_export_class_definition__.named_function_property_callback_map__) {
      const auto& name       = entry.first;
      const auto& attributes = entry.second.get_attributes();
      HAL_LOG_DEBUG("JSExportClass: has function property callback ", name, " with attributes ", to_string(attributes));
    }
  }
  
  // The static functions that implement the JavaScriptCore C API
  // callbacks begin here.
  
  template<typename T>
  void JSExportClass<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) {
    
	  HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::Initialize");
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    
    const auto previous_native_object_ptr = static_cast<JSExport<T>*>(js_object.GetPrivate());
    const auto native_object_ptr          = new T(js_context);
    
    if (previous_native_object_ptr != nullptr) {
      HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::Initialize: replace ", previous_native_object_ptr, " with ", native_object_ptr, " for ", object_ref);
      delete previous_native_object_ptr;
    }
    
    const bool result = js_object.SetPrivate(native_object_ptr);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::Initialize: private data set to ", js_object.GetPrivate(), " for ", object_ref);
    assert(result);
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) {
    HAL_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    
    auto native_object_ptr = static_cast<JSExport<T>*>(JSObjectGetPrivate(object_ref));
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::Finalize: delete native object ", native_object_ptr, " for ", object_ref);
    if (native_object_ptr) {
      //delete reinterpret_cast<T*>(native_object_ptr);
      delete native_object_ptr;
      JSObjectSetPrivate(object_ref, nullptr);
    }
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    
    const std::string property_name = JSString(property_name_ref);
    
    const auto callback_position = js_export_class_definition__.named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != js_export_class_definition__.named_value_property_callback_map__.end();
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::GetNamedProperty: callback found = ", callback_found, " for ", to_string(js_object), ".", property_name);
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = static_cast<const T*>(js_object.GetPrivate());
    const auto callback          = (callback_position -> second).get_callback();
    const auto result            = callback(*native_object_ptr);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::GetNamedProperty: result = ", to_string(result), " for ", to_string(js_object), ".", property_name);
    
    return static_cast<JSValueRef>(result);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("GetNamedProperty", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("GetNamedProperty", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  bool JSExportClass<T>::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSValue   js_value(js_context, value_ref);
    
    const std::string property_name = JSString(property_name_ref);
    
    const auto callback_position = js_export_class_definition__.named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != js_export_class_definition__.named_value_property_callback_map__.end();
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::SetNamedProperty: callback found = ", callback_found, " for ", to_string(js_object), ".", property_name);
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    const auto callback    = (callback_position -> second).set_callback();
    const auto result      = callback(*native_object_ptr, js_value);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::SetNamedProperty: result = ", result, " for ", to_string(js_object), ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("SetNamedProperty", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("SetNamedProperty", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    // to_string(js_object) produces this text:
    //
    // function sayHello() {
    //     [native code]
    // }
    //
    // So this is the regular expression we use to determing the
    // function's name for lookup.
    static std::regex regex("^function\\s+([^(]+)\\(\\)(.|\\n)*$");
    
    JSContext         js_context(context_ref);
    JSObject          js_object(js_context, function_ref);
    JSObject          this_object(js_context, this_object_ref);
    const std::string js_object_string = to_string(js_object);
    std::smatch       match_results;
    const bool        found = std::regex_match(js_object_string, match_results, regex);

    static_cast<void>(found);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallNamedFunction: function name found = ", found, ", match_results.size() = ", match_results.size(), ", input = ", js_object_string);
    
    // precondition
    // The size of the match results should be 3:
    // match_results[0] == the whole string.
    // match_results[1] == The function's name
    // match_results[2] == Everything after the function's name.
    assert(match_results.size() == 3);
    const std::string function_name = match_results[1];
    
    // precondition
    assert(js_object.IsFunction());
    
    const auto callback_position = js_export_class_definition__.named_function_property_callback_map__.find(function_name);
    const bool callback_found    = callback_position != js_export_class_definition__.named_function_property_callback_map__.end();
    const auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    const auto native_this_ptr   = static_cast<T*>(this_object.GetPrivate());

    static_cast<void>(native_object_ptr);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallNamedFunction: callback found = ", callback_found, " for this[", native_this_ptr, "].", function_name, "(...)");
    
    // precondition
    assert(callback_found);
    
    const auto callback = (callback_position -> second).function_callback();
    const auto result   = callback(*native_this_ptr, to_vector(js_context, argument_count, arguments_array), this_object);
    
#ifdef HAL_LOGGING_ENABLE
    std::string js_value_str;
    if (result.IsObject()) {
      JSObject js_object = static_cast<JSObject>(result);
      const auto native_object_ptr = reinterpret_cast<std::intptr_t>(js_object.GetPrivate());
      js_value_str = std::to_string(native_object_ptr);
    }
    else {
      js_value_str = to_string(result);
    }
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallNamedFunction: result = ", js_value_str, " for this[", native_this_ptr, "].", function_name, "(...)");
#endif
    
    return static_cast<JSValueRef>(result);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("CallNamedFunction", JSObject(js_context, function_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("CallNamedFunction", JSObject(js_context, function_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.has_property_callback__;
    const bool callback_found = callback != nullptr;

    const auto native_object_ptr = static_cast<const T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::HasProperty: callback found = ", callback_found, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    // precondition
    assert(callback_found);
    
    const bool result = callback(*native_object_ptr, property_name);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::HasProperty: result = ", result, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    return result;
    
  } catch (const std::exception& e) {
	  LogStdException("HasProperty", JSObject(JSContext(context_ref), object_ref), e);
    return false;
  } catch (...) {
	  LogUnknownException("HasProperty", JSObject(JSContext(context_ref), object_ref));
    return false;
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.get_property_callback__;
    const bool callback_found = callback != nullptr;
    
    const auto native_object_ptr = static_cast<const T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::GetProperty: callback found = ", callback_found, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    // precondition
    assert(callback_found);
    
    const auto result = callback(*native_object_ptr, property_name);
    
#ifdef HAL_LOGGING_ENABLE
    std::string js_value_str;
    if (result.IsObject()) {
      JSObject js_object = static_cast<JSObject>(result);
      const auto native_object_ptr = reinterpret_cast<std::intptr_t>(js_object.GetPrivate());
      js_value_str = std::to_string(native_object_ptr);
    }
    else {
      js_value_str = to_string(result);
    }
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::GetProperty: result = ", js_value_str, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
#endif
    
    return static_cast<JSValueRef>(result);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("GetProperty", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("GetProperty", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    ;
    
    auto       callback       = js_export_class_definition__.set_property_callback__;
    const bool callback_found = callback != nullptr;
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::SetProperty: callback found = ", callback_found, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    // precondition
    assert(callback_found);
    
    const auto result = callback(*native_object_ptr, property_name, JSValue(js_context, value_ref));
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::SetProperty: result = ", result, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("SetProperty", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("SetProperty", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.delete_property_callback__;
    const bool callback_found = callback != nullptr;
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::DeleteProperty: callback found = ", callback_found, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    // precondition
    assert(callback_found);
    
    const auto result = callback(*native_object_ptr, property_name);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::DeleteProperty: result = ", result, " for this[", native_object_ptr, "].", static_cast<std::string>(property_name));
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("DeleteProperty", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("DeleteProperty", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
    
    JSContext                 js_context(context_ref);
    JSObject                  js_object(js_context, object_ref);
    JSPropertyNameAccumulator js_property_name_accumulator(property_names);
    
    auto       callback       = js_export_class_definition__.get_property_names_callback__;
    const bool callback_found = callback != nullptr;
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::GetPropertyNames: callback found = ", callback_found, " for this[", native_object_ptr, "]");
    
    // precondition
    assert(callback_found);
    
    callback(*native_object_ptr, js_property_name_accumulator);
    
  } catch (const std::exception& e) {
	  LogStdException("GetPropertyNames", JSObject(JSContext(context_ref), object_ref), e);
  } catch (...) {
	  LogUnknownException("GetPropertyNames", JSObject(JSContext(context_ref), object_ref));
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, function_ref);
    JSObject  this_object(js_context, this_object_ref);
    
    // precondition
    assert(js_object.IsFunction());
    
    auto       callback       = js_export_class_definition__.call_as_function_callback__;
    const bool callback_found = callback != nullptr;
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    auto native_this_ptr   = static_cast<T*>(this_object.GetPrivate());
    static_cast<void>(native_this_ptr);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallAsFunction: callback found = ", callback_found, " for this[", native_this_ptr, "].this[", native_object_ptr, "](...)");
    
    // precondition
    assert(callback_found);
    
    const auto result = callback(*native_object_ptr, to_vector(js_context, argument_count, arguments_array), this_object);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallAsFunction: result = ", to_string(result), " for this[", native_this_ptr, "].this[", native_object_ptr, "](...)");
    
    return static_cast<JSValueRef>(result);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("CallAsFunction", JSObject(js_context, function_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("CallAsFunction", JSObject(js_context, function_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  JSObjectRef JSExportClass<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    static_cast<void>(native_object_ptr);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallAsConstructor: new this[", native_object_ptr, "]");
    
//    auto new_object   = js_context.CreateObject(JSExport<T>::Class());
//    const bool result = new_object.SetPrivate(new T(*native_object_ptr, to_vector(js_context, argument_count, arguments_array)));
    
//    const auto new_native_new_object_ptr = new T(*native_object_ptr, to_vector(js_context, argument_count, arguments_array));
//    auto new_object = JSObject(js_context, JSExport<T>::Class(), new_native_new_object_ptr);

    auto       new_object                 = js_context.CreateObject<T>();
    const auto previous_native_object_ptr = static_cast<T*>(new_object.GetPrivate());
    const auto new_native_new_object_ptr  = new T(js_context, to_vector(js_context, argument_count, arguments_array));
    const bool result                     = new_object.SetPrivate(new_native_new_object_ptr);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallAsConstructor: replace ", previous_native_object_ptr, " with ", new_native_new_object_ptr, " for ", JSObjectRef(new_object));
    
    // postconditions
    assert(result);
    assert(previous_native_object_ptr);
    
    delete previous_native_object_ptr;

    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::CallAsConstructor: new this[", native_object_ptr, "] = this[", new_native_new_object_ptr, "] for ", JSObjectRef(new_object));
    
    
    return static_cast<JSObjectRef>(new_object);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectCallAsConstructorCallback", JSObject(js_context, constructor_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    JSValue   possible_instance(js_context, possible_instance_ref);

    bool result = false;
    if (possible_instance.IsObject()) {
      JSObject possible_object = static_cast<JSObject>(possible_instance);
      if (possible_object.GetPrivate() != nullptr) {
        auto possible_js_export_ptr     = static_cast<JSExport<T>*>(possible_object.GetPrivate());
        auto possible_native_object_ptr = dynamic_cast<T*>(possible_js_export_ptr);
        if (possible_native_object_ptr != nullptr) {
          result = true;
        }
      }
    }
    
    auto native_object_ptr = static_cast<T*>(js_object.GetPrivate());
    static_cast<void>(native_object_ptr);
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::HasInstance: result = ", result, " for ", to_string(possible_instance), " instanceof this[", native_object_ptr, "]");
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return false;
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSValue::Type js_value_type = ToJSValueType(type);
    
    auto       callback       = js_export_class_definition__.convert_to_type_callback__;
    const bool callback_found = callback != nullptr;
    
    const auto native_object_ptr = static_cast<const T*>(js_object.GetPrivate());
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::ConvertToType: callback found = ", callback_found, " for this[", native_object_ptr, "]");
    
    // precondition
    assert(callback_found);
    
    const auto result = callback(*native_object_ptr, js_value_type);
    
    HAL_LOG_DEBUG("JSExportClass<", typeid(T).name(), ">::ConvertToType: result = ", to_string(result), " for converting this[", native_object_ptr, "] to ", to_string(js_value_type));
    
    return static_cast<JSValueRef>(result);
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
    *exception = static_cast<JSValueRef>(js_context.CreateString(message));
    return nullptr;
  }
  
  template<typename T>
  std::string JSExportClass<T>::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
    std::ostringstream os;
    os << "JSExportClass<"
    << typeid(T).name()
    << ">:: "
    <<  function_name
    << " for object "
    << to_string(js_object)
    << " threw exception: "
    << exception.what();
    
    const auto message = os.str();
    HAL_LOG_ERROR(message);
    return message;
  }
  
  template<typename T>
  std::string JSExportClass<T>::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
    std::ostringstream os;
    os << "JSExportClass<"
    << typeid(T).name()
    << ">:: "
    <<  function_name
    << " for object "
    << to_string(js_object)
    << " threw unknown exception";
    
    const auto message = os.str();
    HAL_LOG_ERROR(message);
    return message;
  }
  
}} // namespace HAL { namespace detail {

#endif // _HAL_DETAIL_JSEXPORTCLASS_HPP_

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

#include "JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"
#include "JavaScriptCoreCPP/detail/JSValueUtil.hpp"

#include <string>
#include <regex>
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
  class JSExportClass final : public JSClass JAVASCRIPTCORECPP_PERFORMANCE_COUNTER2(JSExportClass<T>) {
    
  public:
    
    JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) JAVASCRIPTCORECPP_NOEXCEPT;
    
    JSExportClass()                                = default;
    ~JSExportClass()                               = default;
    JSExportClass(const JSExportClass&)            = default;
    JSExportClass& operator=(const JSExportClass&) = default;
    
#ifdef JAVASCRIPTCORECPP_MOVE_CTOR_AND_ASSIGN_DEFAULT_ENABLE
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
    
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
    static std::recursive_mutex mutex_static__;
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(JSExportClass::mutex_static__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
  };
  
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
  template<typename T>
  std::recursive_mutex JSExportClass<T>::mutex_static__;
#endif
  
  template<typename T>
  JSExportClassDefinition<T> JSExportClass<T>::js_export_class_definition__;
  
  template<typename T>
  JSExportClass<T>::JSExportClass(const JSExportClassDefinition<T>& js_export_class_definition) JAVASCRIPTCORECPP_NOEXCEPT
  : JSClass(js_export_class_definition) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    js_export_class_definition__ = js_export_class_definition;
    //js_export_class_definition__.Print();
  }
  
  template<typename T>
  void JSExportClass<T>::Print() const {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    for (const auto& entry : js_export_class_definition__.named_value_property_callback_map__) {
      const auto& name       = entry.first;
      const auto& attributes = entry.second.get_attributes();
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass: has value property callback ", name, " with attributes ", to_string(attributes));
    }
    
    for (const auto& entry : js_export_class_definition__.named_function_property_callback_map__) {
      const auto& name       = entry.first;
      const auto& attributes = entry.second.get_attributes();
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass: has function property callback ", name, " with attributes ", to_string(attributes));
    }
  }
  
  // The static functions that implement the JavaScriptCore C API
  // callbacks begin here.
  
  template<typename T>
  void JSExportClass<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) {
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::Initialize");
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    const bool result = js_object.SetPrivate(new T(js_context));

    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::Initialize: private data set to = ", js_object.GetPrivate());
    assert(result);
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASS_LOCK_GUARD_STATIC;
    
    auto native_object_ptr = JSObjectGetPrivate(object_ref);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::Finalize: delete native object ", native_object_ptr);
    delete reinterpret_cast<T*>(native_object_ptr);
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    
    const std::string property_name = JSString(property_name_ref);
    
    const auto callback_position = js_export_class_definition__.named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != js_export_class_definition__.named_value_property_callback_map__.end();
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::GetNamedProperty: callback found = ", callback_found, " for ", to_string(js_object), ".", property_name);
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    const auto callback          = (callback_position -> second).get_callback();
    const auto result            = callback(*native_object_ptr);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::GetNamedProperty: result = ", to_string(result), " for ", to_string(js_object), ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::GetNamedProperty", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::GetNamedProperty", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSValue   js_value(js_context, value_ref);
    
    const std::string property_name = JSString(property_name_ref);
    
    const auto callback_position = js_export_class_definition__.named_value_property_callback_map__.find(property_name);
    const bool callback_found    = callback_position != js_export_class_definition__.named_value_property_callback_map__.end();
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::SetNamedProperty: callback found = ", callback_found, " for ", to_string(js_object), ".", property_name);
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(js_object.GetPrivate());
    const auto callback    = (callback_position -> second).set_callback();
    const auto result      = callback(*native_object_ptr, js_value);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::SetNamedProperty: result = ", result, " for ", to_string(js_object), ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::SetNamedProperty", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::SetNamedProperty", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
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
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallNamedFunction: function name found = ", found, ", match_results.size() = ", match_results.size(), ", input = ", js_object_string);
    
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
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string this_object_str = to_string(this_object);
    const std::string this_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));

    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallNamedFunction: callback found = ", callback_found, " for ", this_object_str, ".", function_name, "(...)");
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<T*>(this_object.GetPrivate());
    const auto callback          = (callback_position -> second).function_callback();
    const auto result            = callback(*native_object_ptr, to_vector(js_context, argument_count, arguments_array));
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    std::string js_value_str;
    if (result.IsObject()) {
      JSObject js_object = result;
      js_value_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));
    }
    else {
      js_value_str = to_string(result);
    }
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallNamedFunction: result = ", js_value_str, " for ", this_object_str, ".", function_name, "(...)");
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::CallNamedFunction", JSObject(js_context, function_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::CallNamedFunction", JSObject(js_context, function_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.has_property_callback__;
    const bool callback_found = callback != nullptr;

    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string js_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::HasProperty: callback found = ", callback_found, " for ", js_object_str, ".", property_name);
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    const auto result            = callback(*native_object_ptr, property_name);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::HasProperty: result = ", result, " for ", js_object_str, ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    LogStdException("JSExportClass::HasProperty", JSObject(JSContext(context_ref), object_ref), e);
  } catch (...) {
    LogUnknownException("JSExportClass::HasProperty", JSObject(JSContext(context_ref), object_ref));
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.get_property_callback__;
    const bool callback_found = callback != nullptr;
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string js_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::GetProperty: callback found = ", callback_found, " for ", js_object_str, ".", property_name);
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    const auto result            = callback(*native_object_ptr, property_name);
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    std::string js_value_str;
    if (result.IsObject()) {
      JSObject js_object = result;
      js_value_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));
    }
    else {
      js_value_str = to_string(result);
    }
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::GetProperty: result = ", js_value_str, " for ", js_object_str, ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::GetProperty", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::GetProperty", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    ;
    
    auto       callback       = js_export_class_definition__.set_property_callback__;
    const bool callback_found = callback != nullptr;
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string js_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));

    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::SetProperty: callback found = ", callback_found, " for ", js_object_str, ".", property_name);
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(js_object.GetPrivate());
    const auto result      = callback(*native_object_ptr, property_name, JSValue(js_context, value_ref));
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::SetProperty: result = ", result, " for ", js_object_str, ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::SetProperty", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::SetProperty", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  bool JSExportClass<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSString  property_name(property_name_ref);
    
    auto       callback       = js_export_class_definition__.delete_property_callback__;
    const bool callback_found = callback != nullptr;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::DeleteProperty: callback found = ", callback_found, " for ", to_string(js_object), ".", property_name);
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(js_object.GetPrivate());
    const auto result      = callback(*native_object_ptr, property_name);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::DeleteProperty: result = ", result, " for ", to_string(js_object), ".", property_name);
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::DeleteProperty", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::DeleteProperty", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  void JSExportClass<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
    
    JSContext                 js_context(context_ref);
    JSObject                  js_object(js_context, object_ref);
    JSPropertyNameAccumulator js_property_name_accumulator(property_names);
    
    auto       callback       = js_export_class_definition__.get_property_names_callback__;
    const bool callback_found = callback != nullptr;
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string js_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));

    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::GetPropertyNames: callback found = ", callback_found, " for ", js_object_str);
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    callback(*native_object_ptr, js_property_name_accumulator);
    
  } catch (const std::exception& e) {
    LogStdException("JSExportClass::GetPropertyNames", JSObject(JSContext(context_ref), object_ref), e);
  } catch (...) {
    LogUnknownException("JSExportClass::GetPropertyNames", JSObject(JSContext(context_ref), object_ref));
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
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallAsFunction: callback found = ", callback_found, " for ", to_string(this_object), ".", to_string(js_object), "(...)");
    
    // precondition
    assert(callback_found);
    
    auto native_object_ptr = reinterpret_cast<T*>(js_object.GetPrivate());
    const auto result      = callback(*native_object_ptr, to_vector(js_context, argument_count, arguments_array), this_object);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(js_object), "(...)");
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSExportClass::CallAsFunction", JSObject(js_context, function_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSExportClass::CallAsFunction", JSObject(js_context, function_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  JSObjectRef JSExportClass<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
    
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string js_object_str = std::to_string(reinterpret_cast<intptr_t>(js_object.GetPrivate()));
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallAsConstructor: new ", js_object_str);
    
    auto new_object = js_context.CreateObject(JSExport<T>::Class());
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    const bool result            = new_object.SetPrivate(new T(*native_object_ptr, to_vector(js_context, argument_count, arguments_array)));
    
    // FIXME: Why does to_string(js_object) cause an
    // 'EXC_BAD_ACCESS(code=2, address=...)
    // in JSValueToStringCopy?
    //const std::string js_object_str = to_string(js_object);
    const std::string new_object_str = std::to_string(reinterpret_cast<intptr_t>(new_object.GetPrivate()));
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::CallAsConstructor: result = ", new_object_str, "(private data set to ", new_object.GetPrivate(), ") for new ", js_object_str);
    assert(result);
    
    return new_object;
    
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
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, constructor_ref);
    JSValue   possible_instance(js_context, possible_instance_ref);

    bool result = false;
    if (possible_instance.IsObject()) {
      JSObject possible_object = possible_instance;
      if (possible_object.GetPrivate() != nullptr) {
        auto possible_js_export_ptr     = reinterpret_cast<JSExport<T>*>(possible_object.GetPrivate());
        auto possible_native_object_ptr = dynamic_cast<T*>(possible_js_export_ptr);
        if (possible_native_object_ptr != nullptr) {
          result = true;
        }
      }
    }
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::HasInstance: result = ", result, " for ", to_string(possible_instance), " instanceof ", to_string(js_object));
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectHasInstanceCallback", JSObject(js_context, constructor_ref)));
    *exception = JSValue(js_context, message);
  }
  
  template<typename T>
  JSValueRef JSExportClass<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
    JSContext js_context(context_ref);
    JSObject  js_object(js_context, object_ref);
    JSValue::Type js_value_type = ToJSValueType(type);
    
    auto       callback       = js_export_class_definition__.convert_to_type_callback__;
    const bool callback_found = callback != nullptr;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::ConvertToType: callback found = ", callback_found, " for ", to_string(js_object));
    
    // precondition
    assert(callback_found);
    
    const auto native_object_ptr = reinterpret_cast<const T*>(js_object.GetPrivate());
    const auto result            = callback(*native_object_ptr, js_value_type);
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClass::ConvertToType: result = ", to_string(result), " for converting ", to_string(js_object), " to ", to_string(js_value_type));
    
    return result;
    
  } catch (const std::exception& e) {
    JSContext js_context(context_ref);
    JSString message(LogStdException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref), e));
    *exception = JSValue(js_context, message);
  } catch (...) {
    JSContext js_context(context_ref);
    JSString message(LogUnknownException("JSObjectConvertToTypeCallback", JSObject(js_context, object_ref)));
    *exception = JSValue(js_context, message);
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

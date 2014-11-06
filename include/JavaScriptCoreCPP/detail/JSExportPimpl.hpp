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

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"

#include "JavaScriptCoreCPP/JSLogger.hpp"

#include "JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"

#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/JSClassAttribute.hpp"

#include "JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp"

#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>

#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#endif

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace detail {
  
  template<typename T>
  using JSExportNamedValuePropertyCallbackMap_t    = std::unordered_map<std::string, JSExportNamedValuePropertyCallback<T>>;
  
  template<typename T>
  using JSExportNamedFunctionPropertyCallbackMap_t = std::unordered_map<std::string, JSExportNamedFunctionPropertyCallback<T>>;
  
  
  template<typename T>
  class JSExportPimpl;
  
  template<typename T>
  using CallbackMap_t = std::unordered_map<std::intptr_t, JSExportPimpl<T>*>;
  
  template<typename T>
  class JSClassBuilder;
  
  template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
  class JSExportPimpl : public detail::JSPerformanceCounter<JSExportPimpl> {
#else
    class JSExportPimpl : public JSClass {
#endif
      
    public:
      
      /*!
       @method
       
       @abstract Return the name of this JSClass.
       
       @result The name of this JSClass.
       */
      std::string get_name() const {
        return name__;
      }
      
      /*!
       @method
       
       @abstract Return the version number of this JSClass.
       
       @result The version number of this JSClass.
       */
      std::uint32_t get_version() const {
        return version__;
      }
      
      JSExportPimpl() {
        js_class_definition__ = kJSClassDefinitionEmpty;
        js_class_ref__        = JSClassCreate(&js_class_definition__);
      }
      
      JSExportPimpl(const JSClassBuilder<T>& builder);
      
      ~JSExportPimpl() {
        JSClassRelease(js_class_ref__);
      }
      
      JSExportPimpl(const JSExportPimpl& rhs)        = delete;
      JSExportPimpl(JSExportPimpl&& rhs)             = delete;
      JSExportPimpl& operator=(const JSExportPimpl&) = delete;
      JSExportPimpl& operator=(JSExportPimpl&&)      = delete;
      
      
      virtual void     Initialize(JSObject&& object) final;
      virtual JSObject CallAsConstructor(JSObject&& constructor, const std::vector<JSValue>&& arguments) final;
      virtual bool     HasInstance(JSObject&& constructor, const JSValue&& possible_instance) const final;
      virtual JSValue  GetNamedProperty(JSObject&& object, const JSString&& property_name) const final;
      virtual bool     SetNamedProperty(JSObject&& object, const JSString&& property_name, const JSValue&& value) final;
      virtual JSValue  CallNamedFunction(JSObject&& function, const JSString&& function_name, const std::vector<JSValue>&& arguments, JSObject&& this_object) final;
      virtual JSValue  CallAsFunction(JSObject&& function, const std::vector<JSValue>&& arguments, JSObject&& this_object)                                          ;
      // virtual JSValue  ConvertToType(JSObject&& object, const JSValue::Type&& type) const final;
      
      // For interoperability with the JavaScriptCore C API.
      operator JSClassRef() const {
        return js_class_ref__;
      }
      
      std::string                                   name__;
      std::uint32_t                                 version__ { 0 };
      
      // staticValues
      JSExportNamedValuePropertyCallbackMap_t<T>    named_value_property_callback_map__;
      std::vector<JSStaticValue>                    js_static_values__;
      
      // staticFunctions
      JSExportNamedFunctionPropertyCallbackMap_t<T> named_function_property_callback_map__;
      std::vector<JSStaticFunction>                 js_static_functions__;
      
      CallAsFunctionCallback<T>                     call_as_function_callback__;
      
      JSClassDefinition                             js_class_definition__;
      JSClassRef                                    js_class_ref__ { nullptr };
      
    private:
      
      // ConvertToTypeCallback<T>                      convert_to_type_callback__     { nullptr };
      
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
      static JSExportPimpl* get_callback_ptr(void* data);
      static JSExportPimpl* get_callback_ptr(JSObject& js_object);
      static JSValue::Type  ToJSValueType(JSType type);
      static std::string    LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception);
      static std::string    LogUnknownException(const std::string& function_name, const JSObject& js_object);
      
      static CallbackMap_t<T> callback_map__;
      
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
      
      std::recursive_mutex mutex__;
      static std::recursive_mutex mutex_static__;
      
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD        std::lock_guard<std::recursive_mutex> lock(mutex__)
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC std::lock_guard<std::recursive_mutex> lock_static(JSExportPimpl::mutex_static__)
      
#else
      
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC
      
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE
    };
    
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
    template<typename T>
    std::recursive_mutex JSExportPimpl<T>::mutex_static__;
#endif
    
    template<typename T>
    CallbackMap_t<T> JSExportPimpl<T>::callback_map__;
    
    template<typename T>
    void JSExportPimpl<T>::Initialize(JSObject&& object) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      object.SetPrivate(new T(object.get_context()));
      
      const auto key                    = reinterpret_cast<typename CallbackMap_t<T>::key_type>(object.GetPrivate());
      const auto callback_insert_result = callback_map__.emplace(key, this);
      const bool callback_registered    = callback_insert_result.second;
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::Initialize: callback ", std::to_string(key), " registered = ", std::to_string(callback_registered));
      assert(callback_registered);
    }
    
    template<typename T>
    JSValue JSExportPimpl<T>::GetNamedProperty(JSObject&& object, const JSString&& property_name) const {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      const auto callback_position = named_value_property_callback_map__.find(property_name);
      const bool callback_found    = callback_position != named_value_property_callback_map__.end();
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::GetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
      
      // precondition
      assert(callback_found);
      
      auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
      const auto callback    = (callback_position -> second).get_callback();
      const auto result      = callback(*native_object_ptr);
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::GetNamedProperty: result = ", to_string(result), " for ", to_string(object), ".", to_string(property_name));
      
      return result;
    };
    
    template<typename T>
    bool JSExportPimpl<T>::SetNamedProperty(JSObject&& object, const JSString&& property_name, const JSValue&& value) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      const auto callback_position = named_value_property_callback_map__.find(property_name);
      const bool callback_found    = callback_position != named_value_property_callback_map__.end();
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::SetNamedProperty: callback found = ", std::to_string(callback_found), " for ", to_string(object), ".", to_string(property_name));
      
      // precondition
      assert(callback_found);
      
      auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
      const auto callback    = (callback_position -> second).set_callback();
      const auto result      = callback(*native_object_ptr, std::move(value));
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::SetNamedProperty: result = ", std::to_string(result), " for ", to_string(object), ".", to_string(property_name), " = ", to_string(value));
      
      return result;
    };
    
    template<typename T>
    JSValue JSExportPimpl<T>::CallNamedFunction(JSObject&& function, const JSString&& function_name, const std::vector<JSValue>&& arguments, JSObject&& this_object) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      // precondition
      assert(function.IsFunction());
      
      const auto callback_position = named_function_property_callback_map__.find(static_cast<JSString>(function_name));
      const bool callback_found    = callback_position != named_function_property_callback_map__.end();
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallNamedFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
      
      // precondition
      assert(callback_found);
      
      auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
      const auto callback    = (callback_position -> second).function_callback();
      const auto result      = callback(*native_object_ptr, std::move(arguments), std::move(this_object));
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallNamedFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
      
      return result;
    };
    
    template<typename T>
    JSValue JSExportPimpl<T>::CallAsFunction(JSObject&& function, const std::vector<JSValue>&& arguments, JSObject&& this_object) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      // precondition
      assert(function.IsFunction());
      
      auto       callback       = call_as_function_callback__;
      const bool callback_found = callback != nullptr;
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsFunction: callback found = ", std::to_string(callback_found), " for ", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
      
      // precondition
      assert(callback_found);
      
      auto native_object_ptr = reinterpret_cast<T*>(function.GetPrivate());
      const auto result      = callback(*native_object_ptr, std::move(arguments), std::move(this_object));
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsFunction: result = ", to_string(result), " for ", to_string(this_object), ".", to_string(function), "(", std::to_string(arguments.size()), "arguments)");
      
      return result;
    };
    
    template<typename T>
    JSObject JSExportPimpl<T>::CallAsConstructor(JSObject&& constructor, const std::vector<JSValue>&& arguments) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::CallAsConstructor for ", to_string(constructor), "(", std::to_string(arguments.size()), "arguments)");
      
      // FIXME:
      //      auto native_object_ptr = new T(arguments);
      //      constructor.SetPrivate(static_cast<void*>(native_object_ptr));
      //      return *native_object_ptr;
      
      return constructor;
    };
    
    template<typename T>
    bool JSExportPimpl<T>::HasInstance(JSObject&& constructor, const JSValue&& possible_instance) const {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
      
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
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::HasInstance: result = ", std::to_string(result), " for ", to_string(possible_instance), " instanceof ", to_string(constructor));
      
      return result;
    };
    
    //    template<typename T>
    //    JSValue JSExportPimpl<T>::ConvertToType(JSObject&& object, const JSValue::Type&& type) const {
    //      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD;
    //
    //      auto       callback       = convert_to_type_callback__;
    //      const bool callback_found = callback != nullptr;
    //
    //      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::ConvertToType: callback found = ", std::to_string(callback_found), " for ", to_string(object));
    //
    //      // precondition
    //      assert(callback_found);
    //
    //      auto native_object_ptr = reinterpret_cast<T*>(object.GetPrivate());
    //      const auto result      = callback(*native_object_ptr, type);
    //
    //      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport<", name__, ">::ConvertToType: result = ", to_string(result), " for converting ", to_string(object), " to ", to_string(type));
    //
    //      return result;
    //    };
    
    template<typename T>
    JSExportPimpl<T>* JSExportPimpl<T>::get_callback_ptr(void* data) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      
      auto key      = reinterpret_cast<typename CallbackMap_t<T>::key_type>(data);
      auto position = callback_map__.find(key);
      bool found    = (position != callback_map__.end());
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportPimpl::get_callback_ptr callback ", std::to_string(key), " found = ", std::to_string(found));
      
      if (found) {
        return position -> second;
      }
      
      // Fallback to callback 0.
      key      = reinterpret_cast<typename CallbackMap_t<T>::key_type>(nullptr);
      position = callback_map__.find(key);
      found    = (position != callback_map__.end());
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExportPimpl::get_callback_ptr callback ", std::to_string(key), " found = ", std::to_string(found));
      
      assert(found);
      return position -> second;
    }
    
    template<typename T>
    JSExportPimpl<T>* JSExportPimpl<T>::get_callback_ptr(JSObject& js_object) {
      return get_callback_ptr(js_object.GetPrivate());
    }
    
    template<typename T>
    void JSExportPimpl<T>::JSObjectInitializeCallback(JSContextRef context_ref, JSObjectRef object_ref) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectInitializeCallback");
      
      JSContext js_context(context_ref);
      JSObject  js_object(js_context, object_ref);
      
      get_callback_ptr(js_object) -> Initialize(std::move(js_object));
    }
    
    template<typename T>
    void JSExportPimpl<T>::JSObjectFinalizeCallback(JSObjectRef object_ref) {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      
      auto       native_object_ptr = JSObjectGetPrivate(object_ref);
      const auto key               = reinterpret_cast<typename CallbackMap_t<T>::key_type>(native_object_ptr);
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: delete native object ", std::to_string(key));
      delete reinterpret_cast<T*>(native_object_ptr);
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: native object ", std::to_string(key), "deleted");
      
      const auto callback_erase_result = callback_map__.erase(key);
      const bool callback_unregistered = (callback_erase_result == 1);
      
      JAVASCRIPTCORECPP_LOG_DEBUG("JSExport::JSObjectFinalizeCallback: callback ", std::to_string(key), "un-registered = ", std::to_string(callback_unregistered));
      
      assert(callback_unregistered);
    }
    
    template<typename T>
    JSValueRef JSExportPimpl<T>::GetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    bool JSExportPimpl<T>::SetNamedValuePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    JSValueRef JSExportPimpl<T>::CallNamedFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      JSContext js_context(context_ref);
      JSObject  js_object(js_context, function_ref);
      
      // TODO: Get function's name.
      //return get_callback_ptr(js_object) -> CallNamedFunction(std::move(js_object), to_vector(js_context, argument_count, arguments_array), JSObject(js_context, this_object_ref));
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
    bool JSExportPimpl<T>::JSObjectHasPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      JSContext js_context(context_ref);
      JSObject  js_object(js_context, object_ref);
      
      // FIXME.
      //return js_object.HasProperty(property_name_ref);
      return false;
      
    } catch (const std::exception& e) {
      LogStdException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref), e);
    } catch (...) {
      LogUnknownException("JSObjectHasPropertyCallback", JSObject(JSContext(context_ref), object_ref));
    }
    
    template<typename T>
    JSValueRef JSExportPimpl<T>::JSObjectGetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    
    template<typename T>
    bool JSExportPimpl<T>::JSObjectSetPropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef value_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    
    template<typename T>
    bool JSExportPimpl<T>::JSObjectDeletePropertyCallback(JSContextRef context_ref, JSObjectRef object_ref, JSStringRef property_name_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    
    template<typename T>
    void JSExportPimpl<T>::JSObjectGetPropertyNamesCallback(JSContextRef context_ref, JSObjectRef object_ref, JSPropertyNameAccumulatorRef property_names) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      JSContext js_context(context_ref);
      JSObject  js_object(js_context, object_ref);
      return js_object.GetPropertyNames(JSPropertyNameAccumulator(property_names));
    } catch (const std::exception& e) {
      LogStdException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref), e);
    } catch (...) {
      LogUnknownException("JSObjectGetPropertyNamesCallback", JSObject(JSContext(context_ref), object_ref));
    }
    
    template<typename T>
    JSValueRef JSExportPimpl<T>::JSObjectCallAsFunctionCallback(JSContextRef context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    JSObjectRef JSExportPimpl<T>::JSObjectCallAsConstructorCallback(JSContextRef context_ref, JSObjectRef constructor_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    bool JSExportPimpl<T>::JSObjectHasInstanceCallback(JSContextRef context_ref, JSObjectRef constructor_ref, JSValueRef possible_instance_ref, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
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
    
    template<typename T>
    JSValueRef JSExportPimpl<T>::JSObjectConvertToTypeCallback(JSContextRef context_ref, JSObjectRef object_ref, JSType type, JSValueRef* exception) try {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_LOCK_GUARD_STATIC;
      JSContext js_context(context_ref);
      JSObject  js_object(js_context, object_ref);
      return get_callback_ptr(js_object) -> ConvertToType(std::move(js_object), ToJSValueType(type));
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
    JSValue::Type JSExportPimpl<T>::ToJSValueType(JSType type) {
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
          const std::string internal_component_name = "JSExportPimpl<T>";
          const std::string message = "Unknown JSType " + std::to_string(type);
          ThrowLogicError(internal_component_name, message);
      }
    }
    
    template<typename T>
    std::string JSExportPimpl<T>::LogStdException(const std::string& function_name, const JSObject& js_object, const std::exception& exception) {
      std::ostringstream os;
      os << "JSExportPimpl<T>:: "
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
    std::string JSExportPimpl<T>::LogUnknownException(const std::string& function_name, const JSObject& js_object) {
      std::ostringstream os;
      os << "JSExportPimpl<T>:: "
      <<  function_name
      << " for object "
      << to_string(js_object)
      << " threw unknown exception";
      
      const auto message = os.str();
      JAVASCRIPTCORECPP_LOG_ERROR(message);
      return message;
    }
    
  }} // namespace JavaScriptCoreCPP { namespace detail {
  
#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTPIMPL_HPP_

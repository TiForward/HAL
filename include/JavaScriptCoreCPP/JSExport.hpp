/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSEXPORT_HPP_
#define _JAVASCRIPTCORECPP_JSEXPORT_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include "JavaScriptCoreCPP/detail/JSExportClassDefinitionBuilder.hpp"

#include <string>
#include <memory>
#include <mutex>

namespace JavaScriptCoreCPP {
  
  /*!
   @class
   
   @abstract This class is the bridge between a custom JavaScript
   object backed by a C++ class for some or all of its functionality.
   */
  template<typename T>
  class JSExport JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSExport<T>) {
    
  public:
    
    /*!
     @method
     
     @abstract Return the JSClass for the C++ class T.
     
     @throws std::invalid_argument if the C++ class T didn't at least
     provide a name for the JSClass.
     */
    static JSClass get_js_class();

    JSExport()                           = delete;
    virtual ~JSExport()                  = default;
    JSExport(const JSExport&)            = default;
    JSExport(JSExport&&)                 = default;
    JSExport& operator=(const JSExport&) = default;
    JSExport& operator=(JSExport&&)      = default;
    
  protected:
    
    JSExport(const JSContext& js_context);
    
    JSContext get_context() const noexcept;
    
    /*!
     @method
     
     @abstract Set the name of your JSClass.
     */
    static void SetClassName(const JSString& class_name);
    
    /*!
     @method
     
     @abstract Set the version of your JSClass.
     */
    static void SetClassVersion(std::uint32_t class_version);
    
    /*!
     @method
     
     @abstract Set the JSClassAttribute of your JSClass.
     
     @discussion Standard JavaScript practice calls for storing
     JavaScript function objects in prototypes so that they can be
     shared with JavaScript objects with that prototype. By default
     your JSClass follows this idiom, instantiating your JavaScript
     object with a shared, automatically generated prototype containing
     your JSClass's JavaScript function objects.
     
     To override this behavior set the JSClassAttribute to
     'NoAutomaticPrototype', which specifies that your JSClass should
     not automatically generate such a prototype. The resulting JSClass
     will then instantiate your JavaScript objects with the default
     JavaScript object prototype, and give each JavaScript object its
     own copy of your JSClass's JavaScript function objects.
     */
    static void SetClassAttribute(JSClassAttribute class_attribute);
    
    /*!
     @method
     
     @abstract Set the parent of your JSClass. By default your JSClass
     will have the default JavaScript object class.
     */
    static void SetParent(const JSClass& parent);
    
    /*!
     @method
     
     @abstract Add callbacks to invoke when getting and/or setting a
     value property on your JavaScript object. The property will always
     have the 'DontDelete' attribute. If a setter callback is not
     provided then the property will also have the 'ReadOnly'
     attribute. By default the property is enumerable unless you
     specify otherwise.
     
     @discussion For example, given this class definition:
     
     class Foo {
     JSValue GetName() const;
     bool SetName(JSValue&& value);
     };
     
     You would call AddValueProperty like this:
     
     AddValueProperty("name", &Foo::GetName, &Foo::SetName);
     
     If you wanted the property ReadOnly, then you would call
     AddValueProperty like this:
     
     AddValueProperty("name", &Foo::GetName);
     
     @param property_name A JSString containing your property's name.
     
     @param get_callback The callback to invoke when getting your
     property's value.
     
     @param set_callback The callback to invoke when setting your
     property's value. This may be nullptr, in which case the
     'ReadOnly' attribute for your property is automatically set. If
     this callback returns false, then this indicates that the value
     was not set.
     
     @param enumerable An optional property attribute that specifies
     whether your property is enumerable. The default value is true,
     which means the property is enumerable.
     
     @throws std::invalid_argument exception under these preconditions:
     
     1. If property_name is empty.
     
     2. If both get_callback and set_callback are missing.
     
     3. You have already added a property with the same property_name.
     */
    static void AddValueProperty(const JSString& property_name,
                                 detail::GetNamedValuePropertyCallback<T> get_callback,
                                 detail::SetNamedValuePropertyCallback<T> set_callback = nullptr,
                                 bool enumerable = true);
    
    /*!
     @method
     
     @abstract Add a function property to your JavaScript object with
     the 'DontDelete' and 'ReadOnly' attributes. By default the
     property is enumerable unless you specify otherwise.
     
     @discussion For example, given this class definition:
     
     class Foo {
     JSValue Hello(std::vector<JSValue>&& arguments, JSObject&& this_object);
     };
     
     You would call AddFunctionProperty like this:
     
     AddFunctionProperty("hello", &Foo::Hello);
     
     @param function_name A JSString containing your function's name.
     
     @param function_callback The callback to invoke when calling your
     JavaScript object as a function.
     
     @param enumerable An optional property attribute that specifies
     whether your property is enumerable. The default value is true,
     which means the property is enumerable.
     
     @throws std::invalid_argument exception under these preconditions:
     
     1. If function_name is empty.
     
     2. If function_callback is not provided.
     
     3. You have already added a property with the same property_name.
     */
    static void AddFunctionProperty(const JSString& function_name, detail::CallNamedFunctionCallback<T> function_callback, bool enumerable = true);
    
    /*!
     @method
     
     @abstract Set the callback to invoke when your JavaScript object
     is called as a function.
     
     @discussion If this callback does not exist, then calling your
     object as a function will throw a JavaScript exception.
     
     For example, given this class definition:
     
     class Foo {
     JSValue DoSomething(std::vector<JSValue>&& arguments, JSObject&& this_object);
     };
     
     You would call FunctionCallback like this:
     
     FunctionCallback(&Foo::DoSomething);
     
     In the JavaScript expression 'myObject.myFunction()', then
     'myFunction' is the instance of Foo being called, and this_object
     would be set to 'myObject'.
     
     In the JavaScript expression 'myFunction()', then both
     'myFunction' and 'myObject' is the instance of Foo being called.
     */
    static void FunctionCallback(const detail::CallAsFunctionCallback<T>& call_as_function_callback);
    
    /*!
     @method
     
     @abstract Set the callback to invoke when converting your
     JavaScript object to another JavaScript type. This function is
     only invoked when converting your object to a number or a
     string. An object converted to boolean is 'true.' An object
     converted to object is itself.
     
     @discussion If this function returns JSUndefined, then the
     conversion request forwards the reqeust to your JSClass' parent
     class chain, then your JavaScript object's prototype chain.
     
     For example, given this class definition:
     
     class Foo {
     JSValue ConvertToType(JSValue::Type&& type) const;
     };
     
     You would call ConvertToTypeCallback like this:
     
     ConvertToTypeCallback(&Foo::ConvertToType);
     */
    // static void ConvertToTypeCallback(const detail::ConvertToTypeCallback<T>& convert_to_type_callback);
    
  private:
    
    friend class JSObject;
    
    JSContext js_context__;
    static detail::JSExportClassDefinitionBuilder<T> builder__;
  };
  
  template<typename T>
  void JSExport<T>::SetClassName(const JSString& class_name) {
    builder__.ClassName(class_name);
  }
  
  template<typename T>
  void JSExport<T>::SetClassVersion(uint32_t class_version) {
    builder__.Version(class_version);
  }
  
  template<typename T>
  void JSExport<T>::SetClassAttribute(JSClassAttribute class_attribute) {
    builder__.ClassAttribute(class_attribute);
  }
  
  template<typename T>
  void JSExport<T>::SetParent(const JSClass& parent) {
    builder__.Parent(parent);
  }
  
  template<typename T>
  void JSExport<T>::AddValueProperty(const JSString& property_name, detail::GetNamedValuePropertyCallback<T> get_callback, detail::SetNamedValuePropertyCallback<T> set_callback, bool enumerable) {
    builder__.AddValueProperty(property_name, get_callback, set_callback);
  }
  
  template<typename T>
  void JSExport<T>::AddFunctionProperty(const JSString& function_name, detail::CallNamedFunctionCallback<T> function_callback, bool enumerable) {
    builder__.AddFunctionProperty(function_name, function_callback, enumerable);
  }
  
  template<typename T>
  void JSExport<T>::FunctionCallback(const detail::CallAsFunctionCallback<T>& call_as_function_callback) {
    builder__.Function(call_as_function_callback);
  }
  
  // template<typename T>
  // void JSExport<T>::ConvertToTypeCallback(const detail::ConvertToTypeCallback<T>& convert_to_type_callback) {
  //  builder__.ConvertToType(convert_to_type_callback);
  // }
  
  
  template<typename T>
  detail::JSExportClassDefinitionBuilder<T> JSExport<T>::builder__ = detail::JSExportClassDefinitionBuilder<T>("NotSet");
  
  template<typename T>
  JSClass JSExport<T>::get_js_class() {
    static JSClass js_class;
    std::once_flag of;
    std::call_once(of, [] {
      T::JSExportInitialize();
      const std::string class_name = builder__.ClassName();
      if (class_name.empty() || class_name == "NotSet") {
        detail::ThrowInvalidArgument("JSExport", "You must provide a JSClass name");
      }
      js_class = builder__.build();
    });
    
    return js_class;
  }
  
  template<typename T>
  JSExport<T>::JSExport(const JSContext& js_context)
  : js_context__(js_context) {
  }
  
  template<typename T>
  JSContext JSExport<T>::get_context() const noexcept {
    return js_context__;
  }

  template<typename T, typename... Us>
  std::shared_ptr<T> JSContext::CreateObject(Us&&... constructor_arguments) const {
    auto native_object_ptr = std::make_shared<T>(*this);
    return native_object_ptr;
  }
  
  
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSEXPORT_HPP_

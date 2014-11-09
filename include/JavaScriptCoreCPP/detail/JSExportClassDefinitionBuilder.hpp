/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_HPP_

#include "JavaScriptCoreCPP/detail/JSBase.hpp"
#include "JavaScriptCoreCPP/detail/JSExportClassDefinition.hpp"
#include "JavaScriptCoreCPP/detail/JSExportClass.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <string>
#include <cstdint>

#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_MUTEX
#undef JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_MUTEX                      std::recursive_mutex       mutex__
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_MUTEX
#define JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace detail {
  
  /*!
   @class
   
   @abstract This is a builder that creates a JSExportClassDefinition
   for defining a JSClass backed by a C++ class.
   
   @discussion All properties on this builder except "ClassName" are
   optional, and all callbacks may be nullptr. By default the
   JSClassVersion number is initialized to 0, the Parent JSClass is
   initialized to the default JavaScript object class, The
   JSClassAttribute defaults to 'AutomaticPrototype', and all
   callbacks are initialized to nullptr.
   
   Using the AddValueProperty and AddFunctionProperty methods are the
   simplest and most efficient means for vending custom properties
   since they autmatically service requests like GetProperty,
   SetProperty and GetPropertyNames. The other property access
   callbacks are required only to implement unusual properties, like
   array indexes, whose names are not known at compile-time.
   
   Standard JavaScript practice calls for storing JavaScript function
   objects in prototypes so that they can be shared with JavaScript
   objects having that prototype. The default JSExportClassDefinition
   created by a JSExportClassDefinitionBuilder follows this idiom,
   instantiating JavaScript objects with a shared, automatically
   generated prototype containing the JSClass's JavaScript function
   objects.
   
   To override this behavior set the JSClassAttribute to
   'NoAutomaticPrototype', which specifies that a JSClass should not
   automatically generate such a prototype. The resulting JSClass will
   then instantiate JavaScript objects with the default JavaScript
   object prototype, and give each JavaScript object its own copy of
   the JSClass's JavaScript function objects.
   
   Setting any callback to nullptr specifies that the default object
   callback should substitute, except in the case of HasProperty,
   where it specifies that GetProperty should substitute.
   */
  
  template<typename T>
  using JSClassMap_t = std::unordered_map<std::string, std::shared_ptr<JSClass>>;
  
  template<typename T>
  class JSExportClassDefinitionBuilder final JAVASCRIPTCORECPP_PERFORMANCE_COUNTER1(JSExportClassDefinitionBuilder<T>) {
    
  public:
    
    /*!
     @method
     
     @abstract Create a builder with the given JSClass name. The
     JSClassVersion number is initialized to 0, the Parent is
     initialized to the default JavaScript object class, the
     JSClassAttribute defaults to 'AutomaticPrototype' and all
     callbacks are initialized to nullptr.
     */
    explicit JSExportClassDefinitionBuilder(const std::string& class_name) noexcept
    : name__(class_name) {
      js_class_definition__ = kJSClassDefinitionEmpty;
    }
    
    JSExportClassDefinitionBuilder()                                                           = delete;
    ~JSExportClassDefinitionBuilder()                                                          = default;
    JSExportClassDefinitionBuilder(const JSExportClassDefinitionBuilder<T>& rhs)               = default;
    JSExportClassDefinitionBuilder(JSExportClassDefinitionBuilder<T>&& rhs)                    = default;
    JSExportClassDefinitionBuilder<T>& operator=(const JSExportClassDefinitionBuilder<T>& rhs) = default;
    JSExportClassDefinitionBuilder<T>& operator=(JSExportClassDefinitionBuilder<T>&& rhs)      = default;
    
    /*!
     @method
     
     @abstract Return the JSClass's name.
     
     @result The JSClass's name.
     */
    std::string ClassName() const noexcept {
      return name__;
    }
    
    /*!
     @method
     
     @abstract Set the JSClass's name.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& ClassName(const std::string& class_name) noexcept {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      name__ = class_name;
      return *this;
    }
    
    /*!
     @method
     
     @abstract Return the JSClass's version.
     
     @result The JSClass's version.
     */
    std::uint32_t Version() const noexcept {
      return js_class_definition__.version;
    }
    
    /*!
     @method
     
     @abstract Set the JSClass's version.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& Version(std::uint32_t class_version) noexcept {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      js_class_definition__.version = class_version;
      return *this;
    }
    
    /*!
     @method
     
     @abstract Return the JSClass's JSClassAttribute.
     
     @result The JSClass's JSClassAttribute.
     */
    JSClassAttribute ClassAttribute() const noexcept {
      return FromJSClassAttributes(js_class_definition__.attributes);
    }
    
    /*!
     @method
     
     @abstract Set the JSClass's JSClassAttribute.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& ClassAttribute(JSClassAttribute class_attribute) noexcept {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      js_class_definition__.attributes = ToJSClassAttribute(class_attribute);
      return *this;
    }
    
    /*!
     @method
     
     @abstract Return the parent of the JSClass created by this
     builder.
     
     @result The parent of the JSClass created by this builder.
     */
    JSClass Parent() const noexcept {
      return parent__;
    }
    
    /*!
     @method
     
     @abstract Set the parent of the JSClass created by this builder.
     The default value is the default JavaScript object class.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& Parent(const JSClass& parent) noexcept {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      parent__ = parent;
      return *this;
    }
    
    /*!
     @method
     
     @abstract Add callbacks to invoke when getting and/or setting a
     value property on a JavaScript object. The property will always
     have the 'DontDelete' attribute. If a setter callback is not
     provided then the property will also have the 'ReadOnly'
     attribute. By default the property is enumerable unless you
     specify otherwise.
     
     @discussion For example, given this class definition:
     
     class Foo {
     JSValue GetName() const;
     bool SetName(JSValue&& value);
     };
     
     You would call the builer like this:
     
     JSExportClassDefinitionBuilder<Foo> builder("Foo");
     builder.AddValueProperty("name", &Foo::GetName, &Foo::SetName);
     
     If you wanted the property ReadOnly, then you would call the
     builder like this:
     
     builder.AddValueProperty("name", &Foo::GetName);
     
     @param property_name A JSString containing the property's name.
     
     @param get_callback The callback to invoke when getting a
     property's value from a JavaScript object.
     
     @param set_callback The callback to invoke when setting a
     property's value on a JavaScript object. This may be nullptr, in
     which case the 'ReadOnly' attribute for this property is
     automatically set. If this callback returns false, then this
     indicates that the value was not set.
     
     @param enumerable An optional property attribute that specifies
     whether the property is enumerable. The default value is true,
     which means the property is enumerable.
     
     @throws std::invalid_argument exception under these preconditions:
     
     1. If property_name is empty.
     
     2. If both get_callback and set_callback are missing.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& AddValueProperty(const JSString& property_name, GetNamedValuePropertyCallback<T> get_callback, SetNamedValuePropertyCallback<T> set_callback = nullptr, bool enumerable = true) {
      std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete };
      static_cast<void>(!enumerable   && attributes.insert(JSPropertyAttribute::DontEnum).second);
      static_cast<void>(!set_callback && attributes.insert(JSPropertyAttribute::ReadOnly).second);
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      AddValuePropertyCallback(JSExportNamedValuePropertyCallback<T>(property_name, get_callback, set_callback, attributes));
      return *this;
    }
    
    /*!
     @method
     
     @abstract Add a function property to a JavaScript object with the
     'DontDelete' and 'ReadOnly' attributes. By default the property is
     enumerable unless you specify otherwise.
     
     @discussion For example, given this class definition:
     
     class Foo {
     JSValue Hello(std::vector<JSValue>&& arguments, JSObject&& this_object);
     };
     
     You would call the builer like this:
     
     JSExportClassDefinitionBuilder<Foo> builder("Foo");
     builder.AddFunctionProperty("hello", &Foo::Hello);
     
     @param function_name A JSString containing the function's name.
     
     @param function_callback The callback to invoke when calling the
     JavaScript object as a function.
     
     @param enumerable An optional property attribute that specifies
     whether the property is enumerable. The default value is true,
     which means the property is enumerable.
     
     @throws std::invalid_argument exception under these preconditions:
     
     1. If function_name is empty.
     
     2. If function_callback is not provided.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& AddFunctionProperty(const JSString& function_name, CallNamedFunctionCallback<T> function_callback, bool enumerable = true) {
      std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete, JSPropertyAttribute::ReadOnly };
      static_cast<void>(!enumerable && attributes.insert(JSPropertyAttribute::DontEnum).second);
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      AddFunctionPropertyCallback(JSExportNamedFunctionPropertyCallback<T>(function_name, function_callback, attributes));
      return *this;
    }
    
    /*!
     @method
     
     @abstract Return the callback to invoke when a JavaScript object
     is called as a function.
     
     @result The callback to invoke when a JavaScript object is called
     as a function.
     */
    CallAsFunctionCallback<T> Function() const noexcept {
      return call_as_function_callback__;
    }
    
    /*!
     @method
     
     @abstract Set the callback to invoke when a JavaScript object is
     called as a function.
     
     @discussion If this callback does not exist, then calling your
     object as a function will throw a JavaScript exception.
     
     For example, given this class definition:
     
     class Foo {
     JSValue DoSomething(std::vector<JSValue>&& arguments, JSObject&& this_object);
     };
     
     You would call the builer like this:
     
     JSExportClassDefinitionBuilder<Foo> builder("Foo");
     builder.Function(&Foo::DoSomething);
     
     In the JavaScript expression 'myObject.myFunction()', then
     'myFunction' is the instance of Foo being called, and this_object
     would be set to 'myObject'.
     
     In the JavaScript expression 'myFunction()', then both
     'myFunction' and 'myObject' is the instance of Foo being called.
     
     @result A reference to the builder for chaining.
     */
    JSExportClassDefinitionBuilder<T>& Function(const CallAsFunctionCallback<T>& call_as_function_callback) noexcept {
      JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
      call_as_function_callback__ = call_as_function_callback;
      return *this;
    }
    
    /*!
     @method
     
     @abstract Return the callback to invoke when converting a
     JavaScript object another JavaScript type.
     
     @result The callback to invoke when converting a JavaScript object
     to another JavaScript type.
     */
    // ConvertToTypeCallback<T> ConvertToType() const noexcept {
    //   return convert_to_type_callback__;
    // }
    
    /*!
     @method
     
     @abstract Set the callback to invoke when converting a JavaScript
     object to another JavaScript type. This function is only invoked
     when converting an object to a number or a string. An object
     converted to boolean is 'true.' An object converted to object is
     itself.
     
     @discussion If this function returns JSUndefined, then the
     conversion request forwards the reqeust to the JSClass' parent
     class chain, then the JavaScript object's prototype chain.
     
     For example, given this class definition:
     
     class Foo {
     JSValue ConvertToType(JSValue::Type&& type) const;
     };
     
     You would call the builer like this:
     
     JSExportClassDefinitionBuilder<Foo> builder("Foo");
     builder.ConvertToType(&Foo::ConvertToType);
     
     @result A reference to the builder for chaining.
     */
    // JSExportClassDefinitionBuilder<T>& ConvertToType(const ConvertToTypeCallback<T>& convert_to_type_callback) noexcept {
    //   JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
    //   convert_to_type_callback__ = convert_to_type_callback;
    //   return *this;
    // }
    
    /*!
     @method
     
     @abstract Create and return a JSClass instance with all of the
     properties and callbacks specified in this builder.
     
     @result A JSClass instance with all of the properties and
     callbacks specified in this builder.
     */
    JSExportClassDefinition<T> build();
    
  private:
    
    void AddValuePropertyCallback(const JSExportNamedValuePropertyCallback<T>& value_property_callback);
    void AddFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback<T>& function_property_callback);
    
    // JSExportClassDefinition needs access to js_class_definition__ in
    // accordance with the Builder Pattern.
    template<typename U>
    friend class JSExportClassDefinition;
    
    ::JSClassDefinition                           js_class_definition__;
    std::string                                   name__;
    JSClass                                       parent__;
    JSExportNamedValuePropertyCallbackMap_t<T>    named_value_property_callback_map__;
    JSExportNamedFunctionPropertyCallbackMap_t<T> named_function_property_callback_map__;
    CallAsFunctionCallback<T>                     call_as_function_callback__ { nullptr };
    // ConvertToTypeCallback<T>                        convert_to_type_callback__  { nullptr };
    std::shared_ptr<JSExportPimpl<T>>             js_export_pimpl_ptr__;
    
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_MUTEX;
  };
  
  template<typename T>
  void JSExportClassDefinitionBuilder<T>::AddValuePropertyCallback(const JSExportNamedValuePropertyCallback<T>& value_property_callback) {
    const auto property_name = value_property_callback.get_name();
    const auto position      = named_value_property_callback_map__.find(property_name);
    const bool found         = position != named_value_property_callback_map__.end();
    
    if (found) {
      const std::string message = "Value property " + static_cast<std::string>(property_name) + " already added.";
      JAVASCRIPTCORECPP_LOG_ERROR("JSExportClassDefinitionBuilder<", name__, ">: ", message);
      ThrowInvalidArgument("JSExportClassDefinitionBuilder", message);
    }
    
    const auto callback_insert_result = named_value_property_callback_map__.emplace(property_name, value_property_callback);
    const bool callback_inserted      = callback_insert_result.second;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClassDefinitionBuilder<", name__, ">: AddValuePropertyCallback: callback ", property_name, ", inserted = ", std::to_string(callback_inserted));
    assert(callback_inserted);
  }
  
  template<typename T>
  void JSExportClassDefinitionBuilder<T>::AddFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback<T>& function_property_callback) {
    const auto property_name = function_property_callback.get_name();
    const auto position      = named_function_property_callback_map__.find(property_name);
    const bool found         = position != named_function_property_callback_map__.end();
    
    if (found) {
      const std::string message = "Function property " + static_cast<std::string>(property_name) + " already added.";
      JAVASCRIPTCORECPP_LOG_ERROR("JSExportClassDefinitionBuilder<", name__, ">: ", message);
      ThrowInvalidArgument("JSExportClassDefinitionBuilder", message);
    }
    
    const auto callback_insert_result = named_function_property_callback_map__.emplace(property_name, function_property_callback);
    const bool callback_inserted      = callback_insert_result.second;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportClassDefinitionBuilder<", name__, ">: AddFunctionPropertyCallback: callback ", property_name, ", inserted = ", std::to_string(callback_inserted));
    assert(callback_inserted);
  }
  
  template<typename T>
  JSExportClassDefinition<T> JSExportClassDefinitionBuilder<T>::build() {
    JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_LOCK_GUARD;
    
    js_class_definition__.className          = name__.c_str();
    js_class_definition__.parentClass        = parent__;
    js_class_definition__.initialize         = JSExportClass<T>::JSObjectInitializeCallback;
    js_class_definition__.finalize           = JSExportClass<T>::JSObjectFinalizeCallback;
    // js_class_definition__.hasProperty       = JSExportClass<T>::JSObjectHasPropertyCallback;
    // js_class_definition__.getProperty       = JSExportClass<T>::JSObjectGetPropertyCallback;
    // js_class_definition__.setProperty       = JSExportClass<T>::JSObjectSetPropertyCallback;
    // js_class_definition__.deleteProperty    = JSExportClass<T>::JSObjectDeletePropertyCallback;
    // js_class_definition__.getPropertyNames  = JSExportClass<T>::JSObjectGetPropertyNamesCallback;
    // js_class_definition__.callAsFunction    = JSExportClass<T>::JSObjectCallAsFunctionCallback;
    // js_class_definition__.callAsConstructor = JSExportClass<T>::JSObjectCallAsConstructorCallback;
    // js_class_definition__.hasInstance       = JSExportClass<T>::JSObjectHasInstanceCallback;
    // js_class_definition__.convertToType     = JSExportClass<T>::JSObjectConvertToTypeCallback;
    
    return JSExportClassDefinition<T>(*this);
  }
  
  template<typename T>
  JSExportClassDefinition<T>::JSExportClassDefinition(const JSExportClassDefinitionBuilder<T>& builder)
  : JSClassDefinition(builder.js_class_definition__)
  , named_value_property_callback_map__(builder.named_value_property_callback_map__)
  , named_function_property_callback_map__(builder.named_function_property_callback_map__) {
    InitializeNamedPropertyCallbacks();
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCLASSDEFINITIONBUILDER_HPP_

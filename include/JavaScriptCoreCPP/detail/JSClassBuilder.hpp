/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_HPP_

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"
#include "JavaScriptCoreCPP/detail/JSClassPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSExportPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <string>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_MUTEX
#undef JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_MUTEX                      std::recursive_mutex       mutex__
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#else
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_MUTEX
#define JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class

  @abstract This is a builder that creates a JSClass from a C++ class.

  @discussion All properties on this builder except "ClassName" are
  optional, and all callbacks may be nullptr. By default the
  JSClassVersion number is initialized to 0, the Parent JSClass is
  initialized to the default JavaScript object class, The
  JSClassAttribute defaults to 'AutomaticPrototype', and all callbacks
  are initialized to nullptr.

  Using the AddValueProperty and AddFunctionProperty methods are the
  simplest and most efficient means for vending custom properties
  since they autmatically service requests like GetProperty,
  SetProperty and GetPropertyNames. The other property access
  callbacks are required only to implement unusual properties, like
  array indexes, whose names are not known at compile-time.
  
  Standard JavaScript practice calls for storing JavaScript function
  objects in prototypes so that they can be shared with JavaScript
  objects with that prototype. The default JSClass created by a
  JSClassBuilder follows this idiom, instantiating JavaScript objects
  with a shared, automatically generated prototype containing the
  JSClass's JavaScript function objects.

  To override this behavior set the JSClassAttribute to
  'NoAutomaticPrototype', which specifies that a JSClass should not
  automatically generate such a prototype. The resulting JSClass will
  then instantiate JavaScript objects with the default JavaScript
  object prototype, and give each JavaScript object its own copy of
  the JSClass's JavaScript function objects.

  Setting any callback to nullptr specifies that the default object
  callback should substitute, except in the case of HasProperty, where
  it specifies that GetProperty should substitute.
*/

template<typename T>
using JSClassMap_t = std::unordered_map<std::string, std::shared_ptr<JSClass>>;

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
template<typename T>
class JSClassBuilder final : public JSPerformanceCounter<JSClassBuilder<T>> {
#else
template<typename T>
class JSClassBuilder final {
#endif

 public:
  
  /*!
    @method
    
    @abstract Create a builder with the given JSClass name. The
    JSClassVersion number is initialized to 0, the Parent is
    initialized to the default JavaScript object class, the
    JSClassAttribute defaults to 'AutomaticPrototype' and all
    callbacks are initialized to nullptr.
  */
	explicit JSClassBuilder(const JSString& class_name)
			: name__(class_name) {
	}
  
  JSClassBuilder() = delete;
  ~JSClassBuilder() = default;
  
  JSClassBuilder(const JSClassBuilder<T>& rhs) = default;
  JSClassBuilder(JSClassBuilder<T>&& rhs) = default;
  
  JSClassBuilder<T>& operator=(const JSClassBuilder<T>& rhs) = default;
  JSClassBuilder<T>& operator=(JSClassBuilder<T>&& rhs) = default;

  /*!
    @method
    
    @abstract Return the JSClass's name.
    
    @result The JSClass's name.
  */
  JSString ClassName() const {
    return name__;
  }
  
  /*!
    @method
    
    @abstract Set the JSClass's name.
    
    @result A reference to the builder for chaining.
  */
  JSClassBuilder<T>& ClassName(const JSString& class_name) {
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
    name__ = class_name;
    return *this;
  }

  /*!
    @method
    
    @abstract Return the JSClass's version.
    
    @result The JSClass's version.
  */
  std::uint32_t Version() const {
    return version__;
  }
  
  /*!
    @method
    
    @abstract Set the JSClass's version.
    
    @result A reference to the builder for chaining.
  */
  JSClassBuilder<T>& Version(std::uint32_t class_version) {
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
    version__ = class_version;
    return *this;
  }

  /*!
    @method
    
    @abstract Return the JSClass's JSClassAttribute.
    
    @result The JSClass's JSClassAttribute.
  */
  JSClassAttribute ClassAttribute() const {
    return class_attribute__;
  }
  
  /*!
    @method
    
    @abstract Set the JSClass's JSClassAttribute.
    
    @result A reference to the builder for chaining.
  */
  JSClassBuilder<T>& ClassAttribute(JSClassAttribute class_attribute) {
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
    class_attribute__ = class_attribute;
    return *this;
  }

  /*!
    @method
    
    @abstract Return the parent of the JSClass created by this
    builder.
    
    @result The parent of the JSClass created by this builder.
  */
  JSClass Parent() const {
    return parent__;
  }

  /*!
    @method
    
    @abstract Set the parent of the JSClass created by this builder.
    The default value is the default JavaScript object class.
    
    @result A reference to the builder for chaining.
  */
  JSClassBuilder<T>& Parent(const JSClass& parent) {
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
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
    
    JSClassBuilder<Foo> builder("Foo");
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
  JSClassBuilder<T>& AddValueProperty(const JSString& property_name, GetNamedValuePropertyCallback<T> get_callback, SetNamedValuePropertyCallback<T> set_callback = nullptr, bool enumerable = true) {
    std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete };
    static_cast<void>(!enumerable   && attributes.insert(JSPropertyAttribute::DontEnum).second);
    static_cast<void>(!set_callback && attributes.insert(JSPropertyAttribute::ReadOnly).second);
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
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
    
    JSClassBuilder<Foo> builder("Foo");
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
  JSClassBuilder<T>& AddFunctionProperty(const JSString& function_name, CallNamedFunctionCallback<T> function_callback, bool enumerable = true) {
    std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete, JSPropertyAttribute::ReadOnly };
    static_cast<void>(!enumerable && attributes.insert(JSPropertyAttribute::DontEnum).second);
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
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
  CallAsFunctionCallback<T> Function() const {
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
    
    JSClassBuilder<Foo> builder("Foo");
    builder.Function(&Foo::DoSomething);
    
    In the JavaScript expression 'myObject.myFunction()', then
    'myFunction' is the instance of Foo being called, and this_object
    would be set to 'myObject'.
    
    In the JavaScript expression 'myFunction()', then both
    'myFunction' and 'myObject' is the instance of Foo being called.

    @result A reference to the builder for chaining.
  */
  JSClassBuilder<T>& Function(const CallAsFunctionCallback<T>& call_as_function_callback) {
    JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
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
  // ConvertToTypeCallback<T> ConvertToType() const {
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
    
    JSClassBuilder<Foo> builder("Foo");
    builder.ConvertToType(&Foo::ConvertToType);

    @result A reference to the builder for chaining.
  */
  // JSClassBuilder<T>& ConvertToType(const ConvertToTypeCallback<T>& convert_to_type_callback) {
  //   JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
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
  std::shared_ptr<JSClass> build();

 private:

  // The builder pattern requires friendship by these two
  // collaborating classes.
  template<typename U>
  friend class JSExportPimpl;

  friend JSClassPimpl;

  void AddValuePropertyCallback(const JSExportNamedValuePropertyCallback<T>& value_property_callback);
  void AddFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback<T>& function_property_callback);
  
  std::shared_ptr<JSExportPimpl<T>>               js_export_pimpl_ptr__;
  // JSExportCallbackHandlerMap_t::key_type          callback_handler_key__;
  std::uint32_t                                   version__         { 0 };
  JSClassAttribute                                class_attribute__ { JSClassAttribute::None };
  
  JSString                                        name__;
  JSClass                                         parent__ { JSClass::EmptyJSClass() };
  
  JSExportNamedValuePropertyCallbackMap_t<T>      named_value_property_callback_map__;
  JSExportNamedValuePropertyMap_t                 named_value_property_map__;

  JSExportNamedFunctionPropertyCallbackMap_t<T>   named_function_property_callback_map__;
  JSExportNamedFunctionPropertyMap_t              named_function_property_map__;
  
  CallAsFunctionCallback<T>                       call_as_function_callback__ { nullptr };
  // ConvertToTypeCallback<T>                        convert_to_type_callback__  { nullptr };
  JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_MUTEX;

  static JSClassMap_t<T> js_class_map__;
};

template<typename T>
JSClassMap_t<T> JSClassBuilder<T>::js_class_map__;

template<typename T>
void JSClassBuilder<T>::AddValuePropertyCallback(const JSExportNamedValuePropertyCallback<T>& value_property_callback) {
  const auto property_name = value_property_callback.get_property_name();
  const auto position      = named_value_property_callback_map__.find(property_name);
  const bool found         = position != named_value_property_callback_map__.end();
  
  if (found) {
	  const std::string message = "Value property " + static_cast<std::string>(property_name) + " already added.";
    JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", name__, ">: ", message);
    ThrowInvalidArgument("JSClassBuilder", message);
  }
  
  const auto callback_insert_result = named_value_property_callback_map__.emplace(property_name, value_property_callback);
  const bool callback_inserted      = callback_insert_result.second;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">: AddValuePropertyCallback: callback ", property_name, ", inserted = ", std::to_string(callback_inserted));

  const auto attributes_insert_result = named_value_property_map__.emplace(property_name, value_property_callback.get_attributes());
  const bool attributes_inserted      = attributes_insert_result.second;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">: AddValuePropertyCallback: attributes ", property_name, ", inserted = ", std::to_string(attributes_inserted));

  // postcondition: The callback and attributes were added to their
  // respective maps.
  assert(callback_inserted);
  assert(attributes_inserted);
}

template<typename T>
void JSClassBuilder<T>::AddFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback<T>& function_property_callback) {
  const auto property_name = function_property_callback.get_property_name();
  const auto position      = named_function_property_callback_map__.find(property_name);
  const bool found         = position != named_function_property_callback_map__.end();
  
  if (found) {
	  const std::string message = "Function property " + static_cast<std::string>(property_name) + " already added.";
    JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", name__, ">: ", message);
    ThrowInvalidArgument("JSClassBuilder", message);
  }

  const auto callback_insert_result = named_function_property_callback_map__.emplace(property_name, function_property_callback);
  const bool callback_inserted      = callback_insert_result.second;
  
  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">: AddFunctionPropertyCallback: callback ", property_name, ", inserted = ", std::to_string(callback_inserted));

  const auto attributes_insert_result = named_function_property_map__.emplace(property_name, function_property_callback.get_attributes());
  const bool attributes_inserted      = attributes_insert_result.second;

  JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">: AddFunctionPropertyCallback: attributes ", property_name, ", inserted = ", std::to_string(attributes_inserted));

  // postcondition: The callback and attributes were added to their
  // respective maps.
  assert(callback_inserted);
  assert(attributes_inserted);
}

template<typename T>
std::shared_ptr<JSClass> JSClassBuilder<T>::build() {
	JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_LOCK_GUARD;
	JAVASCRIPTCORECPP_DETAIL_JSCLASSPIMPL_LOCK_GUARD_STATIC;


	// Check to see if we already exist in the JSClass map. If we do
	// then the only thing we need to do is create the callback handler.
	const auto position = js_class_map__.find(name__);
	const bool found    = (position != js_class_map__.end());

	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">:: found = ", std::to_string(found));

	if (found) {
		return position -> second;
	}
	
	// Insert the callback handler (i.e. JSExportPimpl) into the
	// dispatch table (i.e. JSClassPimpl's callback map).
	js_export_pimpl_ptr__                     = std::make_shared<JSExportPimpl<T>>(*this);
	// callback_handler_key__                    = reinterpret_cast<JSExportCallbackHandlerMap_t::key_type>(js_export_pimpl_ptr__.get());
	// const auto callback_handler_insert_result = JSClassPimpl::js_export_callback_handler_map__.emplace(callback_handler_key__, js_export_pimpl_ptr__);
	// const bool callback_handler_registered    = callback_handler_insert_result.second;
	
	// JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">:: callback handler registered = ", std::to_string(callback_handler_registered));
	
	// assert(callback_handler_registered);

	auto js_class_pimpl_ptr = std::make_shared<JSClassPimpl>(*this);
	auto js_class_ptr       = std::make_shared<JSClass>(js_class_pimpl_ptr);

	const auto js_class_ptr_insert_result = js_class_map__.emplace(name__, js_class_ptr);
	const bool js_class_ptr_inserted      = js_class_ptr_insert_result.second;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", name__, ">: Created JSClass = ", std::to_string(js_class_ptr_inserted));

	assert(js_class_ptr_inserted);

	return js_class_ptr;
}

template<typename T>
JSExportPimpl<T>::JSExportPimpl(const JSClassBuilder<T>& builder)
		: name__(builder.name__)
		, named_value_property_callback_map__(builder.named_value_property_callback_map__)
		, named_function_property_callback_map__(builder.named_function_property_callback_map__)
		, call_as_function_callback__(builder.call_as_function_callback__) {
		// , convert_to_type_callback__(builder.convert_to_type_callback__)
		// , callback_handler_key__(builder.callback_handler_key__) {
}

template<typename T>
    JSClassPimpl::JSClassPimpl(const JSClassBuilder<T>& builder)
    : version__(builder.version__)
    , class_attribute__(builder.class_attribute__)
    , name__(builder.name__)
		, parent__(builder.parent__) {
		// , js_export_pimpl_ptr__(builder.js_export_pimpl_ptr__) {
  
  Initialize(builder.named_value_property_map__,
             builder.named_function_property_map__);
}

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSCLASSBUILDER_HPP_

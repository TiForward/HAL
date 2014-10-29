/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeClass.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"
#include <unordered_set>
#include <memory>
#include <iostream>
#include <sstream>
#include <JavaScriptCore/JavaScript.h>

//#define JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_DEBUG

namespace JavaScriptCoreCPP { namespace RAII {

using namespace JavaScriptCoreCPP::detail;

/*!
  @class

  @abstract This is a builder that creates JSNativeClass instances
  that defines JavaScript objects implemented by a instances of a C++
  class. All properties except "class_name" on this builder are
  optional, and all callbacks may be nullptr.

  @discussion Using the AddValuePropertyCallback and
  AddFunctionPropertyCallback methods are the simplest and most
  efficient means for vending custom properties since they
  autmatically service requests like get_property_callback,
  set_property_callback, and get_property_names_callback. The other
  property access callbacks are required only to implement unusual
  properties, like array indexes, whose names are not known at
  compile-time.
  
  Standard JavaScript practice calls for storing function objects in
  prototypes so that they can be shared. The instances of
  JSNativeClass created by a JSNativeClassBuilder follows this idiom,
  instantiating objects with a shared, automatically generated
  prototype containing the class's properties.

  To override this, the JSNativeClassAttributes::NoAutomaticPrototype
  attribute can be set which specifies that a JSNativeClass should not
  automatically generate such a prototype so that an instance has only
  the default object prototype and a copy of the properties.
  
  Setting any callback to nullptr specifies that the default object
  callback should substitute, except in the case of
  has_property_callback, where it specifies that
  get_property_names_callback should substitute.
*/
template<typename T>
class JSNativeClassBuilder final {

 public:
	
	/*!
	  @method
	  
	  @abstract Create a builder with the given class with all callbacks
	  initialized to nullptr.
	*/
	JSNativeClassBuilder(const JSString& class_name)
			: class_name_(class_name)
			, parent_(&kJSClassDefinitionEmpty) {
	}
	
	/*!
	  @method
	  
	  @abstract Create a builder that is initialized from an existing
	  JSNativeClass.
	*/
	JSNativeClassBuilder(const JSNativeClass<T>& js_native_class);

	JSNativeClassBuilder() = delete;;
	~JSNativeClassBuilder() = default;
	
	JSNativeClassBuilder(const JSNativeClassBuilder<T>& rhs) = default;
	JSNativeClassBuilder(JSNativeClassBuilder<T>&& rhs) = default;
	
	JSNativeClassBuilder<T>& operator=(const JSNativeClassBuilder<T>& rhs) = default;
	JSNativeClassBuilder<T>& operator=(JSNativeClassBuilder<T>&& rhs) = default;

	/*!
	  @method
	  
	  @abstract Return the JSNativeClass's name.
	  
	  @result The JSNativeClass's name.
	*/
	JSString class_name() const {
		return class_name_;
	}
	
	/*!
	  @method
	  
	  @abstract Set the JSNativeClass's name.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& class_name(const JSString& class_name) {
		class_name_ = class_name;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the JSNativeClassAttributes that describe the
	  characteristics of the JSNativeClass.
	  
	  @result The JSNativeClassAttributes that describe the
	  characteristics of the JSNativeClass.
	*/
	std::unordered_set<JSNativeClassAttribute> attributes() const {
		return attributes_;
	}

	/*!
	  @method
	  
	  @abstract Set the JSNativeClassAttributes that describe the
	  characteristics of the JSNativeClass.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& attributes(const std::unordered_set<JSNativeClassAttribute>& attributes) {
		attributes_ = attributes;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the parent of the JSNativeClass. The default
	  value is the default object class.
	  
	  @result The parent of the the JSNativeClass.
	*/
	JSClass parent() const {
		return parent_;
	}

	/*!
	  @method
	  
	  @abstract Set the parent of the JSNativeClass.  The default value
	  is the default object class.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& parent(const JSClass& parent) {
		parent_ = parent;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when a JavaScript object
	  is first created.
	  
	  @result The callback to invoke when a JavaScript object is first
	  created.
	*/
	InitializeCallback<T> initialize_callback() const {
		return initialize_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when a JavaScript object is
	  first created. Unlike the other object callbacks, the initialize
	  callback is called on the least derived object (the parent object)
	  first, and the most derived object last, analogous to the way C++
	  constructors work in a class hierarchy.

	  @discussion For example, given this class definition:

	  class Foo {
	    void Initialize();
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.InitializeCallback(&Foo::Initialize);
  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& initialize_callback(const InitializeCallback<T>& initialize_callback) {
		initialize_callback_ = initialize_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when a JavaScript object
	  is finalized (prepared for garbage collection).
  	  
	  @result The callback to invoke when a JavaScript object is
	  finalized (prepared for garbage collection).
	*/
	FinalizeCallback<T> finalize_callback() const {
		return finalize_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when a JavaScript object is
	  finalized (prepared for garbage collection). This callback is
	  invoked immediately before your C++ class destructor. An object
	  may be finalized on any thread.

	  @discussion The finalize callback is called on the most derived
	  object first, and the least derived object (the parent object)
	  last, analogous to that way C++ destructors work in a class
	  hierarchy.
	  
	  You must not call any function that may cause a garbage collection
	  or an allocation of a garbage collected object from within a
	  FinalizeCallback. This basically means don't create any object
	  whose class name begins with JS (e.g. JSString, JSValue, JSObject,
	  etc.)  and don't call any methods on such objects that you may
	  already have a reference to.
	  
	  For example, given this class definition:

	  class Foo {
	    void Finalize();
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.FinalizeCallback(&Foo::Finalize);

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& finalize_callback(const FinalizeCallback<T>& finalize_callback) {
		finalize_callback_ = finalize_callback;
		return *this;
	}

	/*!
	  @method
	  
	  @abstract Return the callback to invoke when a JavaScript object
	  is used as a constructor in a 'new' expression. If you provide
	  this callback then you must also provide the HasInstanceCallback
	  as well.
  
	  @result The callback to invoke when an object is used as a
	  constructor in a 'new' expression.
	*/
	CallAsConstructorCallback<T> call_as_constructor_callback() const {
		return call_as_constructor_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when a JavaScript object is
	  used as a constructor in a 'new' expression. If you provide this
	  callback then you must also provide the HasInstanceCallback as
	  well.

	  @discussion If this callback doest not exist, then using your
	  object as a constructor in a 'new' expression will throw a
	  JavaScript exception.
	  
	  For example, given this class definition:
	  
	  class Foo {
	    JSObject Constructor(const std::vector<JSValue>& arguments);
	  };
	  
	  You would call the builer like this:
	  
	  JSNativeClassBuilder<Foo> builder;
	  builder.CallAsConstructorCallback(&Foo::Constructor);

	  If your callback were invoked by the JavaScript expression
	  'new myConstructor()', then 'myConstructor' is the instance of Foo
	  being called.

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& call_as_constructor_callback(const CallAsConstructorCallback<T>& call_as_constructor_callback) {
		call_as_constructor_callback_ = call_as_constructor_callback;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the callback to invoke when a JavaScript object
	  is used as the target of an 'instanceof' expression. If you
	  provide this callback then you must also provide the
	  CallAsConstructorCallback as well.

	  @result The callback to invoke when an object is used as the
	  target of an 'instanceof' expression.
	*/
	HasInstanceCallback<T> has_instance_callback() const {
		return has_instance_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when a JavaScript object is
	  used as the target of an 'instanceof' expression. If you provide
	  this callback then you must also provide the
	  CallAsConstructorCallback as well.

	  @discussion If this callback does not exist, then 'instanceof'
	  expressions that target your object will return false.

	  For example, given this class definition:
	  
	  class Foo {
	    bool HasInstance(const JSValue& possible_instance) const;
	  };

	  You would call the builer like this:
	  
	  JSNativeClassBuilder<Foo> builder;
	  builder.HasInstanceCallback(&Foo::HasInstance);

	  If your callback were invoked by the JavaScript expression
	  'someValue instanceof myObject', then 'myObject' is the instanceof
	  of Foo being called and 'someValue' is the possible_instance
	  parameter.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& has_instance_callback(const HasInstanceCallback<T>& has_instance_callback) {
		has_instance_callback_ = has_instance_callback;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Add callbacks to invoke when getting and/or setting a
	  property value on a JavaScript object.
	  
	  @param property_name A JSString containing the property's name.
	  
	  @param get_property_callback The callback to invoke when getting a
	  property's value from a JavaScript object.
	  
	  @param set_property_callback The callback to invoke when setting a
	  property's value on a JavaScript object. This may be nullptr, in
	  which case the ReadOnly attribute for this property is
	  automatically set. If this callback returns false, then this
	  indicates that the value was not set. This could happen if the
	  property's "DontDelete" attribute is set, or any other reason you
	  choose for rejecting this specific value.
	  
	  @param attributes An optional set of JSPropertyAttributes to give
	  to the function property. The default is
	  JSPropertyAttribute::DontDelete.
	  
	  @result An object which describes a JavaScript value property.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If property_name is empty or otherwise has a JavaScript syntax
	  error.
	  
	  2. If the ReadOnly attribute is set and the get_property_callback
    is not provided.
	                               
    3. If the ReadOnly attribute is set and the set_property_callback
    is provided.

    4. If both get_property_callback and set_property_callback are
	  missing.

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& AddValuePropertyCallback(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback = nullptr, const std::unordered_set<JSPropertyAttribute>& attributes = {JSPropertyAttribute::DontDelete}) {
		return AddValuePropertyCallback(JSNativeObjectValuePropertyCallback<T>(property_name, get_property_callback, set_property_callback, attributes));
	}
	
	/*!
	  @method

	  @abstract Remove all callbacks added by the
	  AddValuePropertyCallback method.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& RemoveAllValuePropertyCallbacks();

	/*!
	  @method

	  @abstract Return the callback to invoke when getting a property's
	  value from a JavaScript object.
  	  
	  @result The callback to invoke when getting a property's value
	  from a JavaScript object.
	*/
	GetPropertyCallback<T> get_property_callback() const {
		return get_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when getting a property's
	  value from a JavaScript object.
	  
	  @discussion If this function returns JSUndefined, the get request
	  forwards to properties added by the AddValuePropertyCallback
	  method (if any), properties vended by the class' parent class
	  chain, then properties belonging to the JavaScript object's
	  prototype chain.

	  For example, given this class definition:

	  class Foo {
	    JSValue GetProperty(const JSString& property_name) const;
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.GetPropertyCallback(&Foo::GetProperty);
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& get_property_callback(const GetPropertyCallback<T>& get_property_callback) {
		get_property_callback_ = get_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when setting a property's
	  value on a JavaScript object.
  	  
	  @result The callback to invoke when setting a property's value on
	  a JavaScript object.
	*/
	SetPropertyCallback<T> set_property_callback() const {
		return set_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when setting a property's
	  value on a JavaScript object.

	  @discussion If this callback returns false then the request
	  forwards to properties added by the AddValuePropertyCallback
	  method (if any), then properties vended by the class' parent class
	  chain, then properties belonging to the JavaScript object's
	  prototype chain.

	  For example, given this class definition:

	  class Foo {
	    bool SetProperty(const JSString& property_name, const JSValue& value);
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.SetPropertyCallback(&Foo::SetProperty);
  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& set_property_callback(const SetPropertyCallback<T>& set_property_callback) {
		set_property_callback_ = set_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when deleting a property
	  from a JavaScript object.
  	  
	  @result The callback to invoke when deleting a property from a
	  JavaScript object.
	*/
	DeletePropertyCallback<T> delete_property_callback() const {
		return delete_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when deleting a property from
	  a JavaScript object.
  	  
	  @discussion If this function returns false then the request
	  forwards to properties added by the AddValuePropertyCallback
	  method (if any), then properties vended by the class' parent class
	  chain, then properties belonging to the JavaScript object's
	  prototype chain.

	  For example, given this class definition:

	  class Foo {
	    bool DeleteProperty(const JSString& property_name);
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.DeletePropertyCallback(&Foo::DeleteProperty);

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& delete_property_callback(const DeletePropertyCallback<T>& delete_property_callback) {
		delete_property_callback_ = delete_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when collecting the names
	  of a JavaScript object's properties.
  	  
	  @result The callback to invoke when collecting the names of a
	  JavaScript object's properties
	*/
	GetPropertyNamesCallback<T> get_property_names_callback() const {
		return get_property_names_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when collecting the names of
	  a JavaScript object's properties.
	  
	  @discussion The GetPropertyNamesCallback only needs to provide the
	  property names provided by the GetPropertyCallback and/or
	  SetPropertyCallback callbacks (if any). Other property names are
	  automatically added from properties provided by the
	  AddValuePropertyCallback method (if any), then properties vended
	  by the class' parent class chain, then properties belonging to the
	  JavaScript object's prototype chain.
	  
	  For example, given this class definition:

	  class Foo {
	    void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const;
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.GetPropertyNamesCallback(&Foo::GetPropertyNames);
	  
	  Property name accumulators are used by JavaScript for...in loops.
	  Use JSPropertyNameAccumulator::AddName to add property names to
	  accumulator.

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& get_property_names_callback(const GetPropertyNamesCallback<T>& get_property_names_callback) {
		get_property_names_callback_ = get_property_names_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when determining whether a
	  JavaScript object has a property.
  
	  @result The callback to invoke when determining whether a
	  JavaScript object has a property.
	*/
	HasPropertyCallback<T> has_property_callback() const {
		return has_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when determining whether a
	  JavaScript object has a property. If this callback is missing then
	  the object will delegate to the GetPropertyCallback.
  
	  @discussion The HasPropertyCallback enables optimization in cases
	  where only a property's existence needs to be known, not its
	  value, and computing its value is expensive. If the
	  HasPropertyCallback doesn't exist, then the GetPropertyCallback
	  will be used instead.

	  If this function returns false then the reqeust forwards to
	  properties added by the AddValuePropertyCallback method (if any),
	  then properties vended by the class' parent class chain, then
	  properties belonging to the JavaScript object's prototype chain.

	  For example, given this class definition:

	  class Foo {
	    bool HasProperty(const JSString& property_name) const;
	  };

	  You would call the builer like this:

	  JSNativeClassBuilder<Foo> builder;
	  builder.HasPropertyCallback(&Foo::HasProperty);

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& has_property_callback(const HasPropertyCallback<T>& has_property_callback) {
		has_property_callback_ = has_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Add a function property to a JavaScript object that is
	  invoked when called as a function on that JavaScript object.

	  @param function_name A JSString containing the function's name.
	  
	  @param call_as_function_callback The callback to invoke when
	  calling the JavaScript object as a function.
	  
	  @param attributes The set of JSPropertyAttributes to give to the
	  function property.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If function_name is empty or otherwise has a JavaScript syntax
	  error.
	  
	  2. If the call_as_function_callback is not provided.

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& AddFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, const std::unordered_set<JSPropertyAttribute>& attributes = {JSPropertyAttribute::DontDelete}) {
		return AddFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback<T>(function_name, call_as_function_callback, attributes));
	}
	
	/*!
	  @method

	  @abstract Remove all callbacks added by the
	  AddFunctionPropertyCallback method..
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& RemoveAllFunctionPropertyCallbacks();

	/*!
	  @method

	  @abstract Return the callback to invoke when a JavaScript object
	  is called as a function.
  
	  @result The callback to invoke when a JavaScript object is called
	  as a function.
	*/
	CallAsFunctionCallback<T> call_as_function_callback() const {
		return call_as_function_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when a JavaScript object is
	  called as a function.

	  @discussion If this callback does not exist, then calling your
	  object as a function will throw a JavaScript exception.

	  For example, given this class definition:
	  
	  class Foo {
	    JSValue Bar(const std::vector<JSValue>& arguments, const JSObject& this_object);
	  };

	  You would call the builer like this:
	  
	  JSNativeClassBuilder<Foo> builder;
	  builder.CallAsFunctionCallback(&Foo::Bar);
	  
	  In the JavaScript expression 'myObject.myFunction()', then
	  'myFunction' is the instance of Foo being called, and this_object
	  would be set to 'myObject'.
	  
	  In the JavaScript expression 'myFunction()', then 'myFunction' is
	  the instance of Foo being called, and this_object would be set to
	  the global object.

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& call_as_function_callback(const CallAsFunctionCallback<T>& call_as_function_callback) {
		call_as_function_callback_ = call_as_function_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when converting a
	  JavaScript object another JavaScript type.

	  @result The callback to invoke when converting a JavaScript object
	  to another JavaScript type.
	*/
	ConvertToTypeCallback<T> convert_to_type_callback() const {
		return convert_to_type_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when converting a JavaScript
	  object to another JavaScript type. This function is only invoked
	  when converting an object to a number or a string. An object
	  converted to boolean is 'true.' An object converted to object is
	  itself.

	  @discussion If this function returns JSUndefined, then the
	  conversion request forwards the reqeust to the class' parent class
	  chain, then the JavaScript object's prototype chain.

	  For example, given this class definition:
	  
	  class Foo {
	    JSValue ConvertToType(const JSValue::Type& type) const;
	  };

	  You would call the builer like this:
	  
	  JSNativeClassBuilder<Foo> builder;
	  builder.ConvertToTypeCallback(&Foo::ConvertToType);

	  @result A reference to the builder for chaining.
	*/
	JSNativeClassBuilder<T>& convert_to_type_callback(const ConvertToTypeCallback<T>& convert_to_type_callback) {
		convert_to_type_callback_ = convert_to_type_callback;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Create and return a JSClass instance with all of the
	  properties and callbacks specified in the builder.

	  @result A JSClass instance with all of the properties and
	  callbacks specified in the builder.
	*/
	JSNativeClass<T> build() const {
		return JSNativeClass<T>(*this);
	}
	
 private:

	JSNativeClassBuilder<T>& AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback);
	JSNativeClassBuilder<T>& AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback);

	template<typename U>
	friend class JSNativeClass;

	// Optional parameters - initialized to default values
	JSString                                       class_name_;
	std::unordered_set<JSNativeClassAttribute>     attributes_;
	JSClass                                        parent_;
	JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;
	InitializeCallback<T>                          initialize_callback_             { nullptr };
	FinalizeCallback<T>                            finalize_callback_               { nullptr };
	HasPropertyCallback<T>                         has_property_callback_           { nullptr };
	GetPropertyCallback<T>                         get_property_callback_           { nullptr };
	SetPropertyCallback<T>                         set_property_callback_           { nullptr };
	DeletePropertyCallback<T>                      delete_property_callback_        { nullptr };
	GetPropertyNamesCallback<T>                    get_property_names_callback_     { nullptr };
	CallAsFunctionCallback<T>                      call_as_function_callback_       { nullptr };
	CallAsConstructorCallback<T>                   call_as_constructor_callback_    { nullptr };
	HasInstanceCallback<T>                         has_instance_callback_           { nullptr };
	ConvertToTypeCallback<T>                       convert_to_type_callback_        { nullptr };
};

template<typename T>
JSNativeClassBuilder<T>::JSNativeClassBuilder(const JSNativeClass<T>& js_native_class)
		: class_name_(js_native_class.class_name_)
		, attributes_(js_native_class.attributes_)
		, parent_(js_native_class.parent_)
		, value_property_callback_map_(js_native_class.value_property_callback_map_)
		, function_property_callback_map_(js_native_class.function_property_callback_map_)
		, initialize_callback_(js_native_class.initialize_callback_)
		, finalize_callback_(js_native_class.finalize_callback_)
		, has_property_callback_(js_native_class.has_property_callback_)
	  , get_property_callback_(js_native_class.get_property_callback_)
    , set_property_callback_(js_native_class.set_property_callback_)
    , delete_property_callback_(js_native_class.delete_property_callback_)
    , get_property_names_callback_(js_native_class.get_property_names_callback_)
    , call_as_function_callback_(js_native_class.call_as_function_callback_)
		, call_as_constructor_callback_(js_native_class.call_as_constructor_callback_)
		, has_instance_callback_(js_native_class.has_instance_callback_)
		, convert_to_type_callback_(js_native_class.convert_to_type_callback_) {
}


template<typename T>
JSNativeClassBuilder<T>& JSNativeClassBuilder<T>::AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback) {
	static const std::string log_prefix { "MDL: JSNativeClassBuilder::AddValuePropertyCallback:" };
	
	const auto property_name = value_property_callback.get_property_name();
	const auto position      = value_property_callback_map_.find(property_name);
	const bool found         = position != value_property_callback_map_.end();
	
	if (found) {
		// A propery with this name is already in the map, so replace
		// it.
		const auto number_of_elements_removed = value_property_callback_map_.erase(property_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove previous property "
		          << property_name
		          << ", removed = "
		          << std::boolalpha
		          << removed
		          << "."
		          << std::endl;
#endif
		
		// postcondition: the callback was removed from the map.
		assert(removed);
	}
	
	const auto insert_result = value_property_callback_map_.emplace(property_name, value_property_callback);
	const bool inserted      = insert_result.second;
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << "insert property "
	          << property_name
	          << ", inserted = "
	          << std::boolalpha
	          << inserted
	          << "."
	          << std::endl;
#endif
	// postcondition: The callbak was added to the map.
	assert(inserted);
	
	return *this;
}

template<typename T>
JSNativeClassBuilder<T>& JSNativeClassBuilder<T>::RemoveAllValuePropertyCallbacks() {
	value_property_callback_map_.clear();
	return *this;
}

template<typename T>
JSNativeClassBuilder<T>& JSNativeClassBuilder<T>::AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback) {
	static const std::string log_prefix { "MDL: JSNativeClassBuilder::AddFunctionPropertyCallback:" };
	
	const auto function_name = function_property_callback.get_function_name();
	const auto position      = function_property_callback_map_.find(function_name);
	const bool found         = position != function_property_callback_map_.end();
	
	if (found) {
		// A propery with this name is already in the map, so replace
		// it.
		const auto number_of_elements_removed = function_property_callback_map_.erase(function_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove previous function "
		          << function_name
		          << ", removed = "
		          << std::boolalpha
		          << removed
		          << "."
		          << std::endl;
#endif
		
		// postcondition: the callback was removed from the map.
		assert(removed);
	}
	
	const auto insert_result = function_property_callback_map_.emplace(function_name, function_property_callback);
	const bool inserted      = insert_result.second;
	
#ifdef JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << "insert function "
	          << function_name
	          << ", inserted = "
	          << std::boolalpha
	          << inserted
	          << "."
	          << std::endl;
#endif
	// postcondition: The callbak was added to the map.
	assert(inserted);
	
	return *this;
}

template<typename T>
JSNativeClassBuilder<T>& JSNativeClassBuilder<T>::RemoveAllFunctionPropertyCallbacks() {
	function_property_callback_map_.clear();
	return *this;
}


/* JSNativeClass constructor */

template<typename T>
JSNativeClass<T>::JSNativeClass(const JSNativeClassBuilder<T>& builder)
		: JSClass()
		, class_name_(builder.class_name_)
		, class_name_for_js_class_definition_(class_name_)
		, attributes_(builder.attributes_)
		, parent_(builder.parent_)
		, value_property_callback_map_(builder.value_property_callback_map_)
		, function_property_callback_map_(builder.function_property_callback_map_)
		, initialize_callback_(builder.initialize_callback_)
		, finalize_callback_(builder.finalize_callback_)
		, has_property_callback_(builder.has_property_callback_)
	  , get_property_callback_(builder.get_property_callback_)
    , set_property_callback_(builder.set_property_callback_)
    , delete_property_callback_(builder.delete_property_callback_)
    , get_property_names_callback_(builder.get_property_names_callback_)
    , call_as_function_callback_(builder.call_as_function_callback_)
		, call_as_constructor_callback_(builder.call_as_constructor_callback_)
		, has_instance_callback_(builder.has_instance_callback_)
		, convert_to_type_callback_(builder.convert_to_type_callback_)
	  , js_class_definition_(kJSClassDefinitionEmpty) {

	JAVASCRIPTCORECPP_RAII_JSCLASS_LOCK_GUARD;
	InitializeJSStaticValueVector();
	InitializeJSStaticFunctionVector();
	InitializeJSClassDefinition();

	// Replace the private JSClassRef member in the base class.
	JSClassRelease(js_class_ref__);
	js_class_ref__ = JSClassCreate(&js_class_definition_);
}


}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVECLASSBUILDER_HPP_

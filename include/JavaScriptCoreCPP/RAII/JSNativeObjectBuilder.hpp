/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSNativeObjectDefinition.hpp"
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"
#include <unordered_set>
#include <memory>
#include <iostream>
#include <sstream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class

  @abstract This is a builder that creates JSNativeObject instances
  that represent a custom JavaScript object backed by a C++ class for
  some of its functionality. All properties on this builder are
  optional, and all callbacks may be nullptr.

  @discussion Adding JSNativeObjectFunctionPropertyCallback and
  JSNativeObjectValuePropertyCallback instances to a
  JSNativeObjectBuilder are the simplest and most efficient means for
  vending custom properties since they autmatically service requests
  like get_property_callback, set_property_callback, and
  get_property_names_callback. The other property access callbacks are
  required only to implement unusual properties, like array indexes,
  whose names are not known at compile-time.
  
  Standard JavaScript practice calls for storing function objects in
  prototypes so that they can be shared. The instances of
  JSNativeObject created by a JSNativeObjectBuilder follows this
  idiom, instantiating objects with a shared, automatically generated
  prototype containing the class's properties.

  To override this, the JSNativeObjectAttributes::NoAutomaticPrototype
  attribute can be set which specifies that a JSNativeObject should
  not automatically generate such a prototype so that an instance has
  only the default object prototype and a copy of the properties.
  
  Setting any callback to nullptr specifies that the default object
  callback should substitute, except in the case of
  has_property_callback, where it specifies that
  get_property_names_callback should substitute.
*/
template<typename T>
class JSNativeObjectBuilder final {

 public:
	
	JSNativeObjectBuilder(const JSContext& js_context) : js_context_(js_context) {
	}
	
	JSNativeObjectBuilder() = delete;;
	~JSNativeObjectBuilder() = default;
	
	JSNativeObjectBuilder(const JSNativeObjectBuilder<T>& rhs) = default;
	JSNativeObjectBuilder(JSNativeObjectBuilder<T>&& rhs) = default;
	
	JSNativeObjectBuilder<T>& operator=(const JSNativeObjectBuilder<T>& rhs) = default;
	JSNativeObjectBuilder<T>& operator=(JSNativeObjectBuilder<T>&& rhs) = default;

	/*!
	  @method
	  
	  @abstract Return the JSContext in which the JSNativeObject is
	  created.
	  
	  @result The JSContext in which the JSNativeObject is created.
	*/
	JSContext jscontext() const {
		return js_context_;
	}
	
	/*!
	  @method
	  
	  @abstract Set the JSContext in which the JSNativeObject is
	  created.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& js_context(const JSContext& js_context) {
		js_context_ = js_context;
		return *this;
	}

	/*!
	  @method
	  
	  @abstract Return the JSNativeObject's name.
	  
	  @result The JSNativeObject's name.
	*/
	JSString name() const {
		return name_;
	}
	
	/*!
	  @method
	  
	  @abstract Set the JSNativeObject's name.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& name(const JSString& name) {
		name_ = name;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the JSNativeObjectAttributes that describe the
	  characteristics of the JSNativeObject.
	  
	  @result The JSNativeObjectAttributes that describe the
	  characteristics of the JSNativeObject.
	*/
	std::unordered_set<JSNativeObjectAttribute> attributes() const {
		return attributes_;
	}

	/*!
	  @method
	  
	  @abstract Set the JSNativeObjectAttributes that describe the
	  characteristics of the JSNativeObject.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& attributes(const std::unordered_set<JSNativeObjectAttribute>& attributes) {
		attributes_ = attributes;
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the parent of the JSNativeObject. A nullptr means
	  to use the default object class.
	  
	  @result The parent of the the JSNativeObject.
	*/
	std::shared_ptr<JSNativeObject<T>> parent() const {
		return parent_ptr_;
	}

	/*!
	  @method
	  
	  @abstract Set the parent of the JSNativeObject. A nullptr means to
	  use the default object class.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& parent(const std::shared_ptr<JSNativeObject<T>>& parent_ptr) {
		parent_ptr_ = parent_ptr;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the JSNativeObjectValuePropertyCallbacks that
	  describe the JSNativeObject's value properties.
  
	  @result The JSNativeObjectValuePropertyCallbacks that describe the
	  JSNativeObject's value properties.
	*/
	std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>> value_property_callback_map() const {
		return value_property_callback_map_;
	}
	
	/*!
	  @method

	  @abstract Add a JSNativeObjectValuePropertyCallback.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback);
	
	/*!
	  @method

	  @abstract Remove a JSNativeObjectValuePropertyCallback.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback);

	/*!
	  @method

	  @abstract Remove a JSNativeObjectValuePropertyCallback with the
	  given property name.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveValuePropertyCallback(const std::string& property_name);

	/*!
	  @method

	  @abstract Remove all JSNativeObjectValuePropertyCallbacks.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveAllValuePropertyCallbacks();

	/*!
	  @method

	  @abstract Return the JSNativeObjectFunctionPropertyCallbacks that
	  describe the JSNativeObject's function properties.

	  @result The JSNativeObjectFunctionPropertyCallbacks that describe
	  the JSNativeObject's function properties.
	*/
	std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>> JSNativeObjectFunctionPropertyCallbacks() const {
		return function_property_callback_map_;
	}
	
	/*!
	  @method

	  @abstract Add a JSNativeObjectFunctionPropertyCallback.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback);
	
	/*!
	  @method

	  @abstract Remove a JSNativeObjectFunctionPropertyCallback.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback);

	/*!
	  @method

	  @abstract Remove a JSNativeObjectFunctionPropertyCallback with the
	  given property name.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveFunctionPropertyCallback(const std::string& property_name);

	/*!
	  @method

	  @abstract Remove all JSNativeObjectFunctionPropertyCallbacks.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& RemoveAllFunctionPropertyCallbacks();

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
	  first created. Use this callback to perform any additional
	  initialization in your native C++ object that wasn't done in its
	  constructor.
	  
	  @discussion It is recommended that you perform your object's
	  initialization in your C++ class constructor instead of using the
	  InitializeCallback. The InitializeCallback is of limited use and
	  is provided only for parity with the JavaScriptCore C API.

	  Unlike the other object callbacks, the initialize callback is
	  called on the least derived object (the parent object) first, and
	  the most derived object last, analogous to that way C++
	  constructors work in a class hierarchy.

	  For example, given this class definition:

	  class Foo {
	    void Initialize();
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.InitializeCallback(&Foo::Initialize);
  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& initialize_callback(const InitializeCallback<T>& initialize_callback) {
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

	  @discussion It is recommended that you release your object's
	  resources and perform other cleanup in your object's destructor
	  instead of using this callback. The FinalizeCallback is of limited
	  use and is provided only for parity with the JavaScriptCore C API.
	  
	  The finalize callback is called on the most derived object first,
	  and the least derived object (the parent object) last, analogous
	  to that way C++ destructors work in a class hierarchy.
	  
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

	  JSNativeObjectBuilder<Foo> builder;
	  builder.FinalizeCallback(&Foo::Finalize);

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& finalize_callback(const FinalizeCallback<T>& finalize_callback) {
		finalize_callback_ = finalize_callback;
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
	  the object will delegate to GetPropertyCallback.
  
	  @discussion The HasPropertyCallback enables optimization in cases
	  where only a property's existence needs to be known, not its
	  value, and computing its value is expensive. If the
	  HasPropertyCallback doesn't exist, then the GetPropertyCallback
	  will be used instead.

	  If this function returns false, the hasProperty request forwards
	  to the native object's JSNativeObjectPropertyCallback (if any),
	  then its parent JSNativeObject chain (which includes the default
	  object class), then its prototype chain.

	  For example, given this class definition:

	  class Foo {
	    bool HasProperty(const JSString& property_name) const;
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.HasPropertyCallback(&Foo::HasProperty);

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& has_property_callback(const HasPropertyCallback<T>& has_property_callback) {
		has_property_callback_ = has_property_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when getting a property's
	  value from a JavaScript object.
  	  
	  @result The callback to invoke when getting a property's value
	  from a JavaScript object.
	*/
	GetPropertyCallback<T> get_property_names_callback() const {
		return get_property_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when getting a property's
	  value from a JavaScript object.
	  
	  @discussion If this function returns JSUndefined, the get request
	  forwards to the native object's JSNativeObjectPropertyCallback (if
	  any), then its parent JSNativeObject chain (which includes the
	  default object class), then its prototype chain.

	  For example, given this class definition:

	  class Foo {
	    JSValue GetProperty(const JSString& property_name) const;
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.GetPropertyCallback(&Foo::GetProperty);
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& get_property_callback(const GetPropertyCallback<T>& get_property_callback) {
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

	  @discussion If this function returns false, the set request
	  forwards to the native object's JSNativeObjectPropertyCallback (if
	  any), then its parent JSNativeObject chain (which includes the
	  default object class), then its prototype chain.
  
	  For example, given this class definition:

	  class Foo {
	    bool SetProperty(const JSString& property_name, const JSValue& value);
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.SetPropertyCallback(&Foo::SetProperty);
  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& set_property_callback(const SetPropertyCallback<T>& set_property_callback) {
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
  	  
	  @discussion If this function returns false, the delete request
	  forwards to the native object's JSNativeObjectPropertyCallback (if
	  any), then its parent JSNativeObject chain (which includes the
	  default object class), then its prototype chain.

	  For example, given this class definition:

	  class Foo {
	    bool DeleteProperty(const JSString& property_name);
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.DeletePropertyCallback(&Foo::DeleteProperty);

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& delete_property_callback(const DeletePropertyCallback<T>& delete_property_callback) {
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
	GetPropertyNamesCallback<T> get_property_callback() const {
		return get_property_names_callback_;
	}

	/*!
	  @method

	  @abstract Set the callback to invoke when collecting the names of
	  a JavaScript object's properties.
	  
	  @discussion The GetPropertyNamesCallback only needs to provide the
	  names of properties that the native object manages through
	  GetPropertyNamesCallback or SetPropertyCallback. Other properties,
	  including the native object's set of
	  JSNativeObjectPropertyCallback (if any), properties vended by
	  other objects, and properties belonging to object's prototype, are
	  added independently.

	  For example, given this class definition:

	  class Foo {
	    void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const;
	  };

	  You would call the builer like this:

	  JSNativeObjectBuilder<Foo> builder;
	  builder.GetPropertyNamesCallback(&Foo::GetPropertyNames);
	  
	  Property name accumulators are used by JavaScript for...in loops.
	  Use JSPropertyNameAccumulator::AddName to add property names to
	  accumulator.

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& get_property_names_callback(const GetPropertyNamesCallback<T>& get_property_names_callback) {
		get_property_names_callback_ = get_property_names_callback;
		return *this;
	}

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
	  object as a function will throw an exception.

	  For example, given this class definition:
	  
	  class Foo {
	  JSValue Bar(const std::vector<JSValue>& arguments, const JSObject& this_object);
	  };

	  You would call the builer like this:
	  
	  JSNativeObjectBuilder<Foo> builder;
	  builder.CallAsFunctionCallback(&Foo::Bar);
	  
	  In the JavaScript expression 'myObject.myFunction()', then
	  'myFunction' is the instance of Foo being called, and this_object
	  would be set to 'myObject'.
	  
	  In the JavaScript expression 'myFunction()', then 'myFunction' is
	  the instance of Foo being called, and this_object would be set to
	  the global object.

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& call_as_function_callback(const CallAsFunctionCallback<T>& call_as_function_callback) {
		call_as_function_callback_ = call_as_function_callback;
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
	  object as a constructor in a 'new' expression will throw an
	  exception.
	  
	  For example, given this class definition:
	  
	  class Foo {
	    JSObject Constructor(const std::vector<JSValue>& arguments);
	  };
	  
	  You would call the builer like this:
	  
	  JSNativeObjectBuilder<Foo> builder;
	  builder.CallAsConstructorCallback(&Foo::Constructor);

	  If your callback were invoked by the JavaScript expression
	  'new myConstructor()', then 'myConstructor' is the instance of Foo
	  being called.

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& call_as_constructor_callback(const CallAsConstructorCallback<T>& call_as_constructor_callback) {
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
	  
	  JSNativeObjectBuilder<Foo> builder;
	  builder.HasInstanceCallback(&Foo::HasInstance);

	  If your callback were invoked by the JavaScript expression
	  'someValue instanceof myObject', then 'myObject' is the instanceof
	  of Foo being called and 'someValue' is the possible_instance
	  parameter.
	  
	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& has_instance_callback(const HasInstanceCallback<T>& has_instance_callback) {
		has_instance_callback_ = has_instance_callback;
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
	  object another JavaScript type.

	  @discussion If this function returns JSUndefined, the conversion
	  request forwards to JSNativeObject's parent chain (which includes
	  the default object class).
  
	  This function is only invoked when converting an object to number
	  or string. An object converted to boolean is 'true.' An object
	  converted to object is itself.

	  For example, given this class definition:
	  
	  class Foo {
	    JSValue ConvertToType(const JSValue::Type& type) const;
	  };

	  You would call the builer like this:
	  
	  JSNativeObjectBuilder<Foo> builder;
	  builder.ConvertToTypeCallback(&Foo::ConvertToType);

	  @result A reference to the builder for chaining.
	*/
	JSNativeObjectBuilder<T>& convert_to_type_callback(const ConvertToTypeCallback<T>& convert_to_type_callback) {
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
	JSNativeObject<T> build() const {
		return JSNativeObject<T>(*this);
	}
	
 private:

	// Require parameters
	JSContext js_context_;
	
	// Optional parameters - initialized to default values
	JSString                                                                   name_;
	std::unordered_set<JSNativeObjectAttribute>                                attributes_;
	std::shared_ptr<JSNativeObject<T>>                                         parent_ptr_                          { nullptr };
	std::unordered_map<std::string, JSNativeObjectValuePropertyCallback<T>>    value_property_callback_map_;
	std::unordered_map<std::string, JSNativeObjectFunctionPropertyCallback<T>> function_property_callback_map_;
	InitializeCallback<T>                                                      initialize_callback_                 { nullptr };
	FinalizeCallback<T>                                                        finalize_callback_                   { nullptr };
	HasPropertyCallback<T>                                                     has_property_callback_               { nullptr };
	GetPropertyCallback<T>                                                     get_property_callback_               { nullptr };
	SetPropertyCallback<T>                                                     set_property_callback_               { nullptr };
	DeletePropertyCallback<T>                                                  delete_property_callback_            { nullptr };
	GetPropertyNamesCallback<T>                                                get_property_names_callback_         { nullptr };
	CallAsFunctionCallback<T>                                                  call_as_function_callback_           { nullptr };
	CallAsConstructorCallback<T>                                               call_as_constructor_callback_        { nullptr };
	HasInstanceCallback<T>                                                     has_instance_callback_               { nullptr };
	ConvertToTypeCallback<T>                                                   convert_to_type_callback_            { nullptr };
};

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback) {
	static const std::string log_prefix { "MDL: JSNativeObjectBuilder::AddValuePropertyCallback:" };
	
	const auto property_name = value_property_callback.get_property_name();
	const auto position      = value_property_callback_map_.find(property_name);
	const bool found         = position != value_property_callback_map_.end();
	
	if (found) {
		// A propery with this name is already in the map, so replace
		// it.
		const auto number_of_elements_removed = value_property_callback_map_.erase(property_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove previous property"
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
	const bool inserted      = (insert_result -> second);
	
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << "insert property"
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
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback) {
	return RemoveValuePropertyCallback(value_property_callback.get_property_name());
}

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveValuePropertyCallback(const std::string& property_name) {
	static const std::string log_prefix { "MDL: JSNativeObjectBuilder::RemoveValuePropertyCallback:" };
	
	const auto position = value_property_callback_map_.find(property_name);
	const bool found = position != value_property_callback_map_.end();
	
	if (found) {
		const auto number_of_elements_removed = value_property_callback_map_.erase(property_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove property"
		          << property_name
		          << ", removed = "
		          << std::boolalpha
		          << removed
		          << "."
		          << std::endl;
#endif
		
		// postcondition: the callback was removed from the map.
		assert(removed);

#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
	} else {
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "property"
		          << property_name
		          << " already removed."
		          << std::endl;
#endif
	}
	
	return *this;
}

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveAllValuePropertyCallbacks() {
	value_property_callback_map_.clear();
	return *this;
}

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback) {
	static const std::string log_prefix { "MDL: JSNativeObjectBuilder::AddFunctionPropertyCallback:" };
	
	const auto property_name = function_property_callback.get_property_name();
	const auto position      = function_property_callback_map_.find(property_name);
	const bool found         = position != function_property_callback_map_.end();
	
	if (found) {
		// A propery with this name is already in the map, so replace
		// it.
		const auto number_of_elements_removed = function_property_callback_map_.erase(property_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove previous property"
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
	
	const auto insert_result = function_property_callback_map_.emplace(property_name, function_property_callback);
	const bool inserted      = (insert_result -> second);
	
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
	std::clog << log_prefix
	          << " [DEBUG] "
	          << "insert property"
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
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback) {
	return RemoveFunctionPropertyCallback(function_property_callback.get_property_name());
}

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveFunctionPropertyCallback(const std::string& property_name) {
	static const std::string log_prefix { "MDL: JSNativeObjectBuilder::RemoveFunctionPropertyCallback:" };
	
	const auto position = function_property_callback_map_.find(property_name);
	const bool found = position != function_property_callback_map_.end();
	
	if (found) {
		const auto number_of_elements_removed = function_property_callback_map_.erase(property_name);
		const bool removed                    = (number_of_elements_removed == 1);
		
#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "remove property"
		          << property_name
		          << ", removed = "
		          << std::boolalpha
		          << removed
		          << "."
		          << std::endl;
#endif
		
		// postcondition: the callback was removed from the map.
		assert(removed);

#if JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_DEBUG
	} else {
		std::clog << log_prefix
		          << " [DEBUG] "
		          << "property"
		          << property_name
		          << " already removed."
		          << std::endl;
#endif
	}
	
	return *this;
}

template<typename T>
JSNativeObjectBuilder<T>& JSNativeObjectBuilder<T>::RemoveAllFunctionPropertyCallbacks() {
	function_property_callback_map_.clear();
	return *this;
}


/* JSNativeObjectDefinition constructor */

template<typename T>
JSNativeObjectDefinition<T>::JSNativeObjectDefinition(const JSNativeObjectBuilder<T>& builder)
		: name_(builder.name_)
		, class_name_for_js_class_definition_(name_)
		, attributes_(builder.attributes_)
		, parent_ptr_(builder.parent_ptr_)
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


	// Use kJSClassAttributeNoAutomaticPrototype in combination with
	// JSObjectSetPrototype to manage prototypes manually

	if (!attributes_.empty()) {
		js_class_definition_.attributes = detail::ToJSClassAttributes(attributes_);
	}
	
	js_class_definition_.className  = class_name_for_js_class_definition_.c_str();
	
	if (parent_ptr_) {
		js_class_definition_.parentClass = parent_ptr_ -> js_native_object_definition_.js_class_;
	}

	if (!value_property_callback_map_.empty()) {
		js_static_values_ = detail::make_unique<std::vector<JSStaticValue>>();
		for (const auto& value_property_callback : value_property_callback_map_) {
			JSStaticValue js_static_value;
			js_static_value.name        = value_property_callback.property_name_for_js_static_value_;
			js_static_value.getProperty = JSNativeObject<T>::JSStaticValueGetPropertyCallback;
			js_static_value.setProperty = JSNativeObject<T>::JSStaticValueSetPropertyCallback;
			js_static_value.attributes  = value_property_callback.js_property_attributes_;
			js_static_values_ -> push_back(js_static_value);
		}
		js_static_values_ -> emplace_back(nullptr, nullptr, nullptr, 0);
		js_class_definition_.staticValues = &(*js_static_values_)[0];
	}
	
	if (!function_property_callback_map_.empty()) {
		js_static_functions_ = detail::make_unique<std::vector<JSStaticFunction>>();
		for (const auto& function_property_callback : function_property_callback_map_) {
			JSStaticFunction js_static_function;
			js_static_function.name           = function_property_callback.function_name_for_js_static_function_;
			js_static_function.callAsFunction = JSNativeObject<T>::JSStaticFunctionCallAsFunctionCallback;
			js_static_function.attributes     = function_property_callback.js_property_attributes_;
			js_static_functions_ -> push_back(js_static_function);
		}
		js_static_functions_ -> emplace_back(nullptr, nullptr, 0);
		js_class_definition_.staticFunctions = &(*js_static_functions_)[0];
	}

	initialize_callback_          && (js_class_definition_.initialize        = JSNativeObject<T>::JSObjectInitializeCallback);
	finalize_callback_            && (js_class_definition_.finalize          = JSNativeObject<T>::JSObjectFinalizeCallback);
	has_property_callback_        && (js_class_definition_.hasProperty       = JSNativeObject<T>::JSObjectHasPropertyCallback);
	get_property_callback_        && (js_class_definition_.getProperty       = JSNativeObject<T>::JSObjectGetPropertyCallback);
	set_property_callback_        && (js_class_definition_.setProperty       = JSNativeObject<T>::JSObjectSetPropertyCallback);
	delete_property_callback_     && (js_class_definition_.deleteProperty    = JSNativeObject<T>::JSObjectDeletePropertyCallback);
	get_property_names_callback_  && (js_class_definition_.getPropertyNames  = JSNativeObject<T>::JSObjectGetPropertyNamesCallback);
	call_as_function_callback_    && (js_class_definition_.callAsFunction    = JSNativeObject<T>::JSObjectCallAsFunctionCallback);
	call_as_constructor_callback_ && (js_class_definition_.callAsConstructor = JSNativeObject<T>::JSObjectCallAsConstructorCallback);
	has_property_callback_        && (js_class_definition_.hasInstance       = JSNativeObject<T>::JSObjectHasPropertyCallback);
	convert_to_type_callback_     && (js_class_definition_.convertToType     = JSNativeObject<T>::JSObjectCallAsConstructorCallback);
	
	js_class_ = JSClass(&js_class_definition_);
}


/* JSNativeObject constructor */

template<typename T>
JSNativeObject<T>::JSNativeObject(const JSNativeObjectBuilder<T>& builder)
		: js_context_(builder.js_context_)
		, js_native_object_definition_(JSNativeObjectDefinition<T>(builder)) {
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTBUILDER_HPP_

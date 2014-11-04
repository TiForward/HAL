/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSCLASSBUILDER_HPP_
#define _JAVASCRIPTCORECPP_JSCLASSBUILDER_HPP_

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSClassPimpl.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

#include <string>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#undef JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX
#undef JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD
#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX                      std::recursive_mutex       mutex__
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD std::lock_guard<std::recursive_mutex> lock(mutex__)
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


namespace JavaScriptCoreCPP {

//using namespace JavaScriptCoreCPP::detail;

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
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
template<typename T>
class JSClassBuilder final : public detail::JSPerformanceCounter<JSClassBuilder<T>> {
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
	  
	  @abstract Add callbacks to invoke when getting and/or setting a
	  value property on a JavaScript object. The property will always
	  have the 'DontDelete' attribute. If a setter callback is not
	  provided then the property will also have the 'ReadOnly'
	  attribute. By default the property is enumerable unless you
	  specify otherwise.
	  
	  @discussion For example, given this class definition:
	  
	  class Foo {
	    JSValue GetName() const;
	    bool SetName(const JSValue& value);
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
	  
    4. If both get_callback and set_callback are missing.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& AddValueProperty(const JSString& property_name, GetNamedPropertyCallback get_callback, SetNamedPropertyCallback set_callback = nullptr, bool enumerable = true) {
		std::unordered_set<JSObjectPropertyAttribute> attributes { JSObjectPropertyAttribute::DontDelete };
		static_cast<void>(!enumerable   && attributes.insert(JSObjectPropertyAttribute::DontEnum).second);
		static_cast<void>(!set_callback && attributes.insert(JSObjectPropertyAttribute::ReadOnly).second);
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		AddValuePropertyCallback(JSObjectNamedValuePropertyCallback(property_name, get_callback, set_callback, attributes));
		return *this;
	}
	
	/*!
	  @method

	  @abstract Add a function property to a JavaScript object with the
	  'DontDelete' and 'ReadOnly' attributes. By default the property is
	  enumerable unless you specify otherwise.

	  @discussion For example, given this class definition:
	  
	  class Foo {
	    JSValue Hello(const std::vector<JSValue>& arguments, JSObject& this_object);
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
	JSClassBuilder<T>& AddFunctionProperty(const JSString& function_name, CallAsFunctionCallback function_callback, bool enumerable = true) {
		std::unordered_set<JSObjectPropertyAttribute> attributes { JSObjectPropertyAttribute::DontDelete, JSObjectPropertyAttribute::ReadOnly };
		static_cast<void>(!enumerable && attributes.insert(JSObjectPropertyAttribute::DontEnum).second);
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		AddFunctionPropertyCallback(JSObjectNamedValuePropertyCallback(function_name, function_callback, attributes));
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the JSClass's version.
	  
	  @result The JSClass's version.
	*/
	uint32_t Version() const {
		return version__;
	}
	
	/*!
	  @method
	  
	  @abstract Set the JSClass's version.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Version(uint32_t class_version) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
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
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
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
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		parent__ = parent;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when a JavaScript object
	  is first created.
	  
	  @result The callback to invoke when a JavaScript object is first
	  created.
	*/
	InitializeCallback<T> Initialize() const {
		return initialize_callback__;
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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.Initialize(&Foo::Initialize);
  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Initialize(const InitializeCallback<T>& initialize_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		initialize_callback__ = initialize_callback;
		return *this;
	}

	/*!
	  @method

	  @abstract Return the callback to invoke when a JavaScript object
	  is finalized (prepared for garbage collection).
  	  
	  @result The callback to invoke when a JavaScript object is
	  finalized (prepared for garbage collection).
	*/
	FinalizeCallback<T> Finalize() const {
		return finalize_callback__;
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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.Finalize(&Foo::Finalize);

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Finalize(const FinalizeCallback<T>& finalize_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		finalize_callback__ = finalize_callback;
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
	CallAsConstructorCallback<T> Constructor() const {
		return call_as_constructor_callback__;
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.ConstructorCallback(&Foo::Constructor);

	  If your callback were invoked by the JavaScript expression
	  'new myConstructor()', then 'myConstructor' is the instance of Foo
	  being called.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Constructor(const CallAsConstructorCallback<T>& call_as_constructor_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		call_as_constructor_callback__ = call_as_constructor_callback;
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
	HasInstanceCallback<T> HasInstance() const {
		return has_instance_callback__;
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.HasInstance(&Foo::HasInstance);

	  If your callback were invoked by the JavaScript expression
	  'someValue instanceof myObject', then 'myObject' is the instanceof
	  of Foo being called and 'someValue' is the possible_instance
	  parameter.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& HasInstance(const HasInstanceCallback<T>& has_instance_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		has_instance_callback__ = has_instance_callback;
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
	    JSValue DoSomething(const std::vector<JSValue>& arguments, JSObject& this_object);
	  };

	  You would call the builer like this:
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.Function(&Foo::DoSomething);
	  
	  In the JavaScript expression 'myObject.myFunction()', then
	  'myFunction' is the instance of Foo being called, and this_object
	  would be set to 'myObject'.
	  
	  In the JavaScript expression 'myFunction()', then 'myFunction' is
	  the instance of Foo being called, and this_object would be set to
	  the global object.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Function(const CallAsFunctionCallback<T>& call_as_function_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
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
	ConvertToTypeCallback<T> ConvertToType() const {
		return convert_to_type_callback__;
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.ConvertToType(&Foo::ConvertToType);

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& ConvertToType(const ConvertToTypeCallback<T>& convert_to_type_callback) {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		convert_to_type_callback__ = convert_to_type_callback;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Create and return a JSClass instance with all of the
	  properties and callbacks specified in this builder.

	  @result A JSClass instance with all of the properties and
	  callbacks specified in this builder.
	*/
	JSClass build() const;

 private:

	void AddValuePropertyCallback(const JSObjectNamedValuePropertyCallback& value_property_callback);
	void AddFunctionPropertyCallback(const JSObjectNamedFunctionPropertyCallback& function_property_callback);
	
	std::uint32_t                              version__         { 0 };
	JSClassAttribute                           class_attribute__ { JSClassAttribute::None };
	
	JSString                                   name__;
	JSClass                                    parent__;
	
	JSObjectNamedValuePropertyCallbackMap_t    named_value_property_callback_map__;
	JSObjectNamedFunctionPropertyCallbackMap_t named_function_property_callback_map__;
	
	InitializeCallback                         initialize_callback__             { nullptr };
	FinalizeCallback                           finalize_callback__               { nullptr };
	CallAsFunctionCallback                     call_as_function_callback__       { nullptr };
	CallAsConstructorCallback                  call_as_constructor_callback__    { nullptr };
	HasInstanceCallback                        has_instance_callback__           { nullptr };
	ConvertToTypeCallback                      convert_to_type_callback__        { nullptr };
	JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX;
};

template<typename T>
void JSClassBuilder<T>::AddValuePropertyCallback(const JSObjectNamedValuePropertyCallback& value_property_callback) {
	const auto property_name = value_property_callback.get_property_name();
	const auto position      = native_value_property_callback_map__.find(property_name);
	const bool found         = position != native_value_property_callback_map__.end();
	
	if (found) {
		const std::string message = "Value property " + property_name + " already added.";
		JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name__, ">: ", message);
		ThrowRuntimeError("JSClassBuilder", message);
	}
	
	const auto insert_result = native_value_property_callback_map__.emplace(property_name, value_property_callback);
	const bool inserted      = insert_result.second;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">: insert property ", property_name, ", inserted = ", std::to_string(inserted));

	// postcondition: The callbak was added to the map.
	assert(inserted);
}

template<typename T>
void JSClassBuilder<T>::AddFunctionPropertyCallback(const JSObjectNamedFunctionPropertyCallback& function_property_callback) {
	const auto function_name = function_property_callback.get_function_name();
	const auto position      = native_function_property_callback_map__.find(function_name);
	const bool found         = position != native_function_property_callback_map__.end();
	
	if (found) {
		const std::string message = "Function property " + function_name + " already added.";
		JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name__, ">: ", message);
		ThrowRuntimeError("JSClassBuilder", message);
	}
	
	const auto insert_result = native_function_property_callback_map__.emplace(function_name, function_property_callback);
	const bool inserted      = insert_result.second;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name__, ">: insert function property ", function_name, ", inserted = ", std::to_string(inserted));

	// postcondition: The callbak was added to the map.
	assert(inserted);
}

template<typename T>
JSClass JSClassBuilder<T>::build() const {
	JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
	return JSClass(std::make_shared<JSClassPimpl>(*this));
}

template<typename T>
detail::JSClassPimpl(const JSClassBuilder<T>& builder)
		: version__(builder.version__)
		, class_attribute__(builder.class_attribute__)
		, name__(builder.name__)
		, parent__(builder.parent__)
		, named_value_property_callback_map__(builder.named_value_property_callback_map__)
		, named_function_property_callback_map__(builder.named_function_property_callback_map__)
		, initialize_callback__(builder.initialize_callback__)
		, finalize_callback__(builder.finalize_callback__)
		, call_as_function_callback__(builder.call_as_function_callback__)
		, call_as_constructor_callback__(builder.call_as_constructor_callback__)
		, has_instance_callback__(builder.has_instance_callback__)
		, convert_to_type_callback__(builder.convert_to_type_callback__) {
	Initialize();
}

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASSBUILDER_HPP_

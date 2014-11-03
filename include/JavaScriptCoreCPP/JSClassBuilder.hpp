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

#include "JavaScriptCoreCPP/JSLogger.hpp"
#include "JavaScriptCoreCPP/detail/JSNativeClass.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <JavaScriptCore/JavaScript.h>


#ifdef JAVASCRIPTCORECPP_THREAD_SAFE
#include <mutex>

#unndef JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_TYPE
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_TYPE std::recursive_mutex

#unndef JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_NAME 
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_NAME js_class_builder

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_TYPE JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_NAME##_mutex_;

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_TYPE> JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_NAME##_lock(JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX);


#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_TYPE
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME_PREFIX js_class_builder_static

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_TYPE JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME;

#undef  JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_LOCK_GUARD
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_TYPE> JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX
#define JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE


namespace JavaScriptCoreCPP {

using namespace JavaScriptCoreCPP::detail;

/*!
  @class

  @abstract This is a builder that creates JSClass instances backed by
  a C++ class.

  @discussion All properties on this builder except "ClassName" are
  optional, and all callbacks may be nullptr. By default the
  JSClassVersion number is initialized to 0, the Parent JSClass is
  initialized to the default JavaScript object class,
  AutomaticPrototype is initialized to true and all callbacks are
  initialized to nullptr.

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

  To override this behavior call the AutomaticPrototype method with a
  value of false, which specifies that a JSClass should not
  automatically generate such a prototype. The resulting JSClass will
  then instantiate JavaScript objects with the default JavaScript
  object prototype, and give each JavaScript object its own copy of
  the JSClass's JavaScript function objects.

  Setting any callback to nullptr specifies that the default object
  callback should substitute, except in the case of HasProperty, where
  it specifies that GetProperty should substitute.
*/
template<typename T>
class JSClassBuilder final {

 public:
	
	/*!
	  @method
	  
	  @abstract Create a builder with the given JSClass name. The
	  JSClassVersion number is initialized to 0, the Parent is
	  initialized to the default JavaScript object class,
	  AutomaticPrototype is initialized to true and all callbacks are
	  initialized to nullptr.
	*/
	explicit JSClassBuilder(const JSString& js_class_name)
			: js_class_name_(js_class_name) {

		ThrowRuntimeErrorIfJSClassAlreadyExists();
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
	JSString JSClassName() const {
		return js_class_name_;
	}
	
	/*!
	  @method
	  
	  @abstract Add callbacks to invoke when getting and/or setting a
	  property value on a JavaScript object. The property will always
	  have the JSPropertyAttribute::DontDelete property attribute. If a
	  setter callback is not provided then the property will also have
	  the JSPropertyAttribute::ReadOnly property attribute. By default
	  the property is enumerable unless you specify otherwise.
	  
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
	  
	  @param get_property_callback The callback to invoke when getting a
	  property's value from a JavaScript object.
	  
	  @param set_property_callback The callback to invoke when setting a
	  property's value on a JavaScript object. This may be nullptr, in
	  which case the ReadOnly attribute for this property is
	  automatically set. If this callback returns false, then this
	  indicates that the value was not set.
	  
	  @param enumerable An optional property attribute that specifies
	  whether the property is enumerable. The default value is true,
	  which means the property is enumerable.

	  @result An object which describes a JavaScript value property.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If property_name is empty or otherwise has a JavaScript syntax
	  error.
	  
    4. If both get_property_callback and set_property_callback are
	  missing.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& AddValueProperty(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback = nullptr, bool enumerable = true) {
		std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete };
		static_cast<void>(!enumerable && attributes.insert(JSPropertyAttribute::DontEnum).second);
		static_cast<void>(!set_property_callback && attributes.insert(JSPropertyAttribute::ReadOnly).second);
		return AddValuePropertyCallback(JSNativeObjectValuePropertyCallback<T>(property_name, get_property_callback, set_property_callback, attributes));
	}
	
	/*!
	  @method

	  @abstract Add a function property to a JavaScript object that is
	  invoked when called as a function on that JavaScript object. The
	  property will always have the JSPropertyAttribute::DontDelete and
	  JSPropertyAttribute::ReadOnly property attributes. By default the
	  property is enumerable unless you specify otherwise.

	  @discussion For example, given this class definition:
	  
	  class Foo {
	    JSValue SayHello(const std::vector<JSValue>& arguments, JSObject& this_object);
	  };

	  You would call the builer like this:
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.AddFunctionProperty("sayHello", &Foo::SayHello);

	  @param function_name A JSString containing the function's name.
	  
	  @param call_as_function_callback The callback to invoke when
	  calling the JavaScript object as a function.
	  
	  @param enumerable An optional property attribute that specifies
	  whether the property is enumerable. The default value is true,
	  which means the property is enumerable.
	  
	  @throws std::invalid_argument exception under these preconditions:

	  1. If function_name is empty or otherwise has a JavaScript syntax
	  error.
	  
	  2. If the call_as_function_callback is not provided.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& AddFunctionProperty(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, bool enumerable = true) {
		std::unordered_set<JSPropertyAttribute> attributes { JSPropertyAttribute::DontDelete, JSPropertyAttribute::ReadOnly };
		static_cast<void>(!enumerable && attributes.insert(JSPropertyAttribute::DontEnum).second);
		return AddFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback<T>(function_name, call_as_function_callback, attributes));
	}
	
	/*!
	  @method
	  
	  @abstract Return the JSClass's version.
	  
	  @result The JSClass's version.
	*/
	uint32_t JSClassVersion() const {
		return js_class_version_;
	}
	
	/*!
	  @method
	  
	  @abstract Set the JSClass's version.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& JSClassVersion(uint32_t js_class_version) {
		js_class_version_ = js_class_version;
		return *this;
	}

	/*!
	  @method
	  
	  @abstract Return whether the JSClass created by this builder will
	  instantiate JavaScript objects with an automatically generated
	  prototype containing the JSClass's JavaScript function objects.
	  
	  @result Whether the JSClass created by this builder will
	  instantiate JavaScript objects with an automatically generated
	  prototype containing the JSClass's JavaScript function objects.
	*/
	bool AutomaticPrototype() const {
		return js_class_attributes_ == kJSClassAttributeNone;
	}

	/*!
	  @method

	  @abstract Set whether the JSClass created by this builder will
	  instantiate JavaScript objects with an automatically generated
	  prototype containing the JSClass's JavaScript function objects.
	  
	  @discussion Standard JavaScript practice calls for storing
	  JavaScript function objects in prototypes so that they can be
	  shared with JavaScript objects with that prototype. The default
	  JSClass created by a JSClassBuilder follows this idiom,
	  instantiating JavaScript objects with a shared, automatically
	  generated prototype containing the JSClass's JavaScript function
	  objects.
	  
	  To override this behavior call this method with a value of false,
	  which specifies that the JSClass created by this builder should
	  not automatically generate such a prototype. The resulting JSClass
	  will then instantiate JavaScript objects with the default
	  JavaScript object prototype, and give each JavaScript object its
	  own copy of the JSClass's JavaScript function objects.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& AutomaticPrototype(bool automatic_prototype) {
		if (automatic_prototype) {
			js_class_attributes_ == kJSClassAttributeNone;
		} else {
			js_class_attributes_ == kJSClassAttributeNoAutomaticPrototype;
		}
		
		return *this;
	}
	
	/*!
	  @method
	  
	  @abstract Return the parent of the JSClass created by this
	  builder.
	  
	  @result The parent of the JSClass created by this builder.
	*/
	JSClass Parent() const {
		return js_class_parent_;
	}

	/*!
	  @method
	  
	  @abstract Set the parent of the JSClass created by this builder.
	  The default value is the default JavaScript object class.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Parent(const JSClass& js_class_parent) {
		js_class_parent_ = js_class_parent;
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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.Initialize(&Foo::Initialize);
  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Initialize(const InitializeCallback<T>& initialize_callback) {
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
	FinalizeCallback<T> Finalize() const {
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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.Finalize(&Foo::Finalize);

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Finalize(const FinalizeCallback<T>& finalize_callback) {
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
	CallAsConstructorCallback<T> Constructor() const {
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.ConstructorCallback(&Foo::Constructor);

	  If your callback were invoked by the JavaScript expression
	  'new myConstructor()', then 'myConstructor' is the instance of Foo
	  being called.

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& Constructor(const CallAsConstructorCallback<T>& call_as_constructor_callback) {
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
	HasInstanceCallback<T> HasInstance() const {
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.HasInstance(&Foo::HasInstance);

	  If your callback were invoked by the JavaScript expression
	  'someValue instanceof myObject', then 'myObject' is the instanceof
	  of Foo being called and 'someValue' is the possible_instance
	  parameter.
	  
	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& HasInstance(const HasInstanceCallback<T>& has_instance_callback) {
		has_instance_callback_ = has_instance_callback;
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
	ConvertToTypeCallback<T> ConvertToType() const {
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
	  
	  JSClassBuilder<Foo> builder("Foo");
	  builder.ConvertToType(&Foo::ConvertToType);

	  @result A reference to the builder for chaining.
	*/
	JSClassBuilder<T>& ConvertToType(const ConvertToTypeCallback<T>& convert_to_type_callback) {
		convert_to_type_callback_ = convert_to_type_callback;
		return *this;
	}
	
	/*!
	  @method

	  @abstract Create and return a JSClass instance with all of the
	  properties and callbacks specified in this builder.

	  @result A JSClass instance with all of the properties and
	  callbacks specified in this builder.
	*/
	detail::JSNativeClass<T> build() const {
		JAVASCRIPTCORECPP_JSCLASSBUILDER_LOCK_GUARD;
		
		JSClass::ThrowRuntimeErrorIfJSClassAlreadyExists(js_class_name_);
		
		InitializeJSClassDefinition(this);
		
		using detail::JSNativeClass<T>;
		return JSNativeClass<T>(JSNativeClass<T>::js_class_definition_);
	}

 private:

	JSClassBuilder<T>& AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback);
	JSClassBuilder<T>& AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback);
	
	uint32_t                                       js_class_version_;
	JSClassAttributes                              js_class_attributes_;
	JSString                                       js_class_name_;
	JSClass                                        js_class_parent_;
	JSNativeObjectValuePropertyCallbackMap_t<T>    value_property_callback_map_;
	JSNativeObjectFunctionPropertyCallbackMap_t<T> function_property_callback_map_;
	InitializeCallback<T>                          initialize_callback_             { nullptr };
	FinalizeCallback<T>                            finalize_callback_               { nullptr };
	CallAsFunctionCallback<T>                      call_as_function_callback_       { nullptr };
	CallAsConstructorCallback<T>                   call_as_constructor_callback_    { nullptr };
	HasInstanceCallback<T>                         has_instance_callback_           { nullptr };
	ConvertToTypeCallback<T>                       convert_to_type_callback_        { nullptr };
	JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX;
	
	template<typename U>
	static void InitializeJSClassDefinition(const JSClassBuilder<U>& builder);

	static JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX;
};

template<typename T> JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_TYPE JSClassBuilder<T>::JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_MUTEX_NAME;

template<typename T>
JSClassBuilder<T>& JSClassBuilder<T>::AddValuePropertyCallback(const JSNativeObjectValuePropertyCallback<T>& value_property_callback) {
	const auto property_name = value_property_callback.get_property_name();
	const auto position      = value_property_callback_map_.find(property_name);
	const bool found         = position != value_property_callback_map_.end();
	
	if (found) {
		const std::string message = "Value property " + to_string(property_name) + " already added.";
		JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name_, ">: ", message);
		ThrowRuntimeError("JSClassBuilder", message);
	}
	
	const auto insert_result = value_property_callback_map_.emplace(property_name, value_property_callback);
	const bool inserted      = insert_result.second;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, ">: insert property ", property_name, ", inserted = ", std::to_string(inserted));

	// postcondition: The callbak was added to the map.
	assert(inserted);
	
	return *this;
}

template<typename T>
JSClassBuilder<T>& JSClassBuilder<T>::AddFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& function_property_callback) {
	const auto function_name = function_property_callback.get_function_name();
	const auto position      = function_property_callback_map_.find(function_name);
	const bool found         = position != function_property_callback_map_.end();
	
	if (found) {
		const std::string message = "Function property " + to_string(function_name) + " already added.";
		JAVASCRIPTCORECPP_LOG_ERROR("JSClassBuilder<", js_class_name_, ">: ", message);
		ThrowRuntimeError("JSClassBuilder", message);
	}
	
	const auto insert_result = function_property_callback_map_.emplace(function_name, function_property_callback);
	const bool inserted      = insert_result.second;
	
	JAVASCRIPTCORECPP_LOG_DEBUG("JSClassBuilder<", js_class_name_, ">: insert function property ", function_name, ", inserted = ", std::to_string(inserted));

	// postcondition: The callbak was added to the map.
	assert(inserted);
	
	return *this;
}

template<typename T>
void JSClassBuilder<T>::InitializeJSClassDefinition(const JSClassBuilder<T>& builder) {
	JAVASCRIPTCORECPP_JSCLASSBUILDER_STATIC_LOCK_GUARD;
	std::lock_guard<JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX_TYPE> builder_lock(builder.JAVASCRIPTCORECPP_JSCLASSBUILDER_MUTEX);
	
	using detail::JSNativeClass<T>;
	
	JSNativeClass<T>::value_property_callback_map_           = builder.value_property_callback_map_;
	JSNativeClass<T>::function_property_callback_map_        = builder.function_property_callback_map_;

	JSNativeClass<T>::js_class_name_                         = to_string(builder.js_class_name_);

	JSNativeClass<T>::js_class_definition_.version           = builder.js_class_version_;
	JSNativeClass<T>::js_class_definition_.attributes        = builder.js_class_attributes_;
	JSNativeClass<T>::js_class_definition_.className         = JSNativeClass<T>::js_js_class_name_.c_str();
	JSNativeClass<T>::js_class_definition_.parentClass       = builder.js_class_parent_;

	JSNativeClass<T>::InitializeJSStaticValueVector();
	JSNativeClass<T>::InitializeJSStaticFunctionVector();
	
	JSNativeClass<T>::js_class_definition_.initialize        = builder.initialize_callback_;
	JSNativeClass<T>::js_class_definition_.finalize          = builder.finalize_callback_;
	
	// JSNativeClass provides the following 5 callbacks by simply
	// delegating to the equivalent JSObject methods.
	JSNativeClass<T>::js_class_definition_.hasProperty       = JSNativeClass<T>::JSObjectHasPropertyCallback;
	JSNativeClass<T>::js_class_definition_.getProperty       = JSNativeClass<T>::JSObjectGetPropertyCallback;
	JSNativeClass<T>::js_class_definition_.setProperty       = JSNativeClass<T>::JSObjectSetPropertyCallback;
	JSNativeClass<T>::js_class_definition_.deleteProperty    = JSNativeClass<T>::JSObjectDeletePropertyCallback;
	JSNativeClass<T>::js_class_definition_.getPropertyNames  = JSNativeClass<T>::JSObjectGetPropertyNamesCallback;

	JSNativeClass<T>::js_class_definition_.callAsFunction    = builder.call_as_function_callback_;
	JSNativeClass<T>::js_class_definition_.callAsConstructor = builder.call_as_constructor_callback_;
	JSNativeClass<T>::js_class_definition_.hasInstance       = builder.has_instance_callback_;
	JSNativeClass<T>::js_class_definition_.convertToType     = builder.convert_to_type_callback_;
}

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSCLASSBUILDER_HPP_


	/*!
	  @method

	  @abstract Remove all callbacks added by the AddValueProperty
	  method.
	  
	  @result A reference to the builder for chaining.
	JSClassBuilder<T>& RemoveAllValueProperties() {
		value_property_callback_map_.clear();
		return *this;
	}
	*/

	/*!
	  @method

	  @abstract Remove all callbacks added by the AddFunctionProperty
	  method.
	  
	  @result A reference to the builder for chaining.
	JSClassBuilder<T>& RemoveAllFunctionProperties() {
		function_property_callback_map_.clear();
		return *this;
	}
	*/

	/*!
	  @method

	  @abstract Return the callback to invoke when getting a property's
	  value from a JavaScript object.
  	  
	  @result The callback to invoke when getting a property's value
	  from a JavaScript object.

	GetPropertyCallback<T> GetProperty() const {
		return get_property_callback_;
	}
	*/

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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.GetProperty(&Foo::GetProperty);
	  
	  @result A reference to the builder for chaining.

	JSClassBuilder<T>& GetProperty(const GetPropertyCallback<T>& get_property_callback) {
		get_property_callback_ = get_property_callback;
		return *this;
	}
	*/
	
	/*!
	  @method

	  @abstract Return the callback to invoke when setting a property's
	  value on a JavaScript object.
  	  
	  @result The callback to invoke when setting a property's value on
	  a JavaScript object.

	SetPropertyCallback<T> SetProperty() const {
		return set_property_callback_;
	}
	*/

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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.SetProperty(&Foo::SetProperty);
  
	  @result A reference to the builder for chaining.

	JSClassBuilder<T>& SetProperty(const SetPropertyCallback<T>& set_property_callback) {
		set_property_callback_ = set_property_callback;
		return *this;
	}
	*/

	/*!
	  @method

	  @abstract Return the callback to invoke when deleting a property
	  from a JavaScript object.
  	  
	  @result The callback to invoke when deleting a property from a
	  JavaScript object.

	DeletePropertyCallback<T> DeleteProperty() const {
		return delete_property_callback_;
	}
	*/

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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.DeleteProperty(&Foo::DeleteProperty);

	  @result A reference to the builder for chaining.

	JSClassBuilder<T>& DeleteProperty(const DeletePropertyCallback<T>& delete_property_callback) {
		delete_property_callback_ = delete_property_callback;
		return *this;
	}
	*/

	/*!
	  @method

	  @abstract Return the callback to invoke when collecting the names
	  of a JavaScript object's properties.
  	  
	  @result The callback to invoke when collecting the names of a
	  JavaScript object's properties

	GetPropertyNamesCallback<T> GetPropertyNames() const {
		return get_property_names_callback_;
	}
	*/

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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.GetPropertyNames(&Foo::GetPropertyNames);
	  
	  Property name accumulators are used by JavaScript for...in loops.
	  Use JSPropertyNameAccumulator::AddName to add property names to
	  accumulator.

	  @result A reference to the builder for chaining.

	JSClassBuilder<T>& GetPropertyNames(const GetPropertyNamesCallback<T>& get_property_names_callback) {
		get_property_names_callback_ = get_property_names_callback;
		return *this;
	}
	*/

	/*!
	  @method

	  @abstract Return the callback to invoke when determining whether a
	  JavaScript object has a property.
  
	  @result The callback to invoke when determining whether a
	  JavaScript object has a property.

	HasPropertyCallback<T> HasProperty() const {
		return has_property_callback_;
	}
	*/

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

	  JSClassBuilder<Foo> builder("Foo");
	  builder.HasProperty(&Foo::HasProperty);

	  @result A reference to the builder for chaining.

	JSClassBuilder<T>& HasProperty(const HasPropertyCallback<T>& has_property_callback) {
		has_property_callback_ = has_property_callback;
		return *this;
	}
	*/

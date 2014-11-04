/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSNATIVEOBJECT_HPP_
#define _JAVASCRIPTCORECPP_JSNATIVEOBJECT_HPP_

//#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include "JavaScriptCoreCPP/JSNativeClass.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

//#include <JavaScriptCore/JavaScript.h>


namespace JavaScriptCoreCPP {

using namespace JavaScriptCoreCPP::detail;

/*!
  @class

  @abstract This class is the bridge between a custom JavaScript
  object backed by a C++ class for some or all of its functionality.
*/
template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeObject : public JSObject, public detail::JSPerformanceCounter<JSNativeObject> {
#else
class JSNativeObject : public JSObject {
#endif
	
 public:

	/*!
	  @method
	  
	  @abstract Return the JSNativeClass's name that defines this
	  JavaScript object.
	  
	  @result The JSNativeClass's name that defines this.
	*/
	JSString get_class_name() const {
		return class_name_;
	}
	
	virtual ~JSNativeObject() {
	}
	
	JSNativeObject(const JSNativeObject& rhs)
			: JSObject(rhs)
			, js_native_class__(rhs.js_native_class__) {
	}
	
	JSNativeObject(JSNativeObject&& rhs)
			: JSObject(rhs)
			, js_native_class__(std::move(rhs.js_native_class__)) {
	}
	
	JSNativeObject& JSNativeObject::operator=(const JSNativeObject&) = delete;
	JSNativeObject& JSNativeObject::operator=(JSNativeObject&&) = delete;
	
  // Create a copy of another JSNativeObject by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator
  //
  // X& X::operator=(const X&)
  //
  // and the move assignment operator
  //
  // X& X::operator=(X&&);
  JSNativeObject& operator=(JSNativeObject rhs) {
	  JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	  JSObject::operator=(rhs);
    swap(rhs);
    return *this;
  }

  void swap(JSNativeObject& other) noexcept {
	  JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	  using std::swap;
	  
	  // By swapping the members of two classes, the two classes are
	  // effectively swapped.
	  swap(js_native_class__            , other.js_native_class__);
	  swap(attach_to_context_once_flag__, other.attach_to_context_once_flag__);
	  swap(attached_to_context__        , other.attached_to_context__);
  }

	friend void swap(JSNativeObject& first, JSNativeObject& second) noexcept {
		first.swap(second);
	}

	virtual JSObject CallAsConstructor(const std::vector<JSValue>& arguments) override final {
		assert(attached_to_context__);
		return JSObject::CallAsConstructor(arguments);
	}

	virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, JSObject this_object) override final {
		assert(attached_to_context__);
		return JSObject::CallAsFunction(arguments, this_object);
	}
	
	virtual bool HasInstance(const JSObject& constructor, const JSValue& possible_instance) const final {
		bool result = false;
		try {
			static_cast<void>(dynamic_cast<const T&>(possible_instance));
			result = true;
		} catch (...) {
		}
		return result;
	}

 private:

	// Only a JSContext can attach a JSNativeClass to itself.
	friend JSContext;

	void AttachToContext() const {
		auto self = const_cast<JSNativeObject*>(this);
		std::call_once(self -> attach_to_context_once_flag__, [self] {
				// Replace the JSObjectRef of our base class with one based on
				// our JSNativeClass that has its private data set to a
				// pointer to ourselves.
				// JSValueUnprotect(self -> get_context(), self -> js_object_ref__);
				// self -> js_object_ref__ = JSObject(self -> get_context(), self -> js_native_class__, self);
				// self -> attached_to_context__ = true;
				self -> JSObject::SetPrivate(self);
			});
	}
	
	virtual void* GetPrivate() const override final {
		assert(attached_to_context__);
		return nullptr;
	}
	
	virtual bool SetPrivate(void* data) override final {
		assert(attached_to_context__);
		return false;
	}

	JSNativeClass<T> js_native_class__;
	std::once_flag   attach_to_context_once_flag__;
	bool             attached_to_context__ { false };

 protected:
	
	JSNativeObject(const JSContext& js_context, const JSNativeClass<T>& js_native_class)
			: JSObject(js_context, js_native_class)
			, js_native_class__(JSClassBuilder<T>(js_native_class).HasInstance(&JSNativeObject<T>::HasInstance).build()) {
		assert(JSClassBuilder<T>(js_native_class__).HasInstance());
		assert(JSClassBuilder<T>(js_native_class__).Constructor());
	}

	/*!
	  @method
	  
	  @abstract Set the JSClass's name.
	  
	  @result void
	*/
	static void JSClassName(const JSString js_class_name) const {
		// TODO
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
	static void AddValueProperty(const JSString& property_name, GetNamedPropertyCallback<T> get_property_callback, SetNamedPropertyCallback<T> set_property_callback = nullptr, bool enumerable = true) {
		// TODO
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
	static void AddFunctionProperty(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, bool enumerable = true) {
		// TODO
	}

	/*!
	  @method
	  
	  @abstract Set the JSClass's version.
	  
	  @result A reference to the builder for chaining.
	*/
	static void JSClassVersion(uint32_t js_class_version) {
		// TODO
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
	static void AutomaticPrototype(bool automatic_prototype) {
		// TODO
	}

	/*!
	  @method
	  
	  @abstract Set the parent of the JSClass created by this builder.
	  The default value is the default JavaScript object class.
	  
	  @result A reference to the builder for chaining.
	*/
	static void Parent(const JSClass& js_class_parent) {
		// TODO
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
	static void Initialize(const InitializeCallback<T>& initialize_callback) {
		// TODO
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
	static void Finalize(const FinalizeCallback<T>& finalize_callback) {
		// TODO
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
	static void Constructor(const CallAsConstructorCallback<T>& call_as_constructor_callback) {
		// TODO
	}
};

template<typename T>
JSContext JSContextGroup::CreateContext(const JSNativeClass<T>& global_object_class) const {
	return JSContext(*this, global_object_class);
}

template<typename T, typename... Us>
T JSContext::CreateObject(Us&&... constructor_arguments) const {
	const auto object = T(*this, std::forward<Us>(constructor_arguments)...);
	object.AttachToContext();
	return object;
}

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSNATIVEOBJECT_HPP_

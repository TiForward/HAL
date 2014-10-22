/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

namespace JavaScriptCoreCPP { namespace RAII {

	/*!
	  @enum JSPropertyAttribute
	  @constant None         Specifies that a property has no special attributes.
	  @constant ReadOnly     Specifies that a property is read-only.
	  @constant DontEnum     Specifies that a property should not be enumerated by JSPropertyEnumerators and JavaScript for...in loops.
	  @constant DontDelete   Specifies that the delete operation should fail on a property.
	*/
enum class JSPropertyAttribute : ::JSPropertyAttributes {
	None,      // kJSPropertyAttributeNone,
	ReadOnly,  // kJSPropertyAttributeReadOnly,
	DontEnum,  // kJSPropertyAttributeDontEnum,
	DontDelete // kJSPropertyAttributeDontDelete
};

class JSPropertyNameAccumulator;

/*!
  @class
  @discussion A JSObject is an RAII wrapper around a JSObjectRef, the
  JavaScriptCore C API representation of a JavaScript object. A
  JSObject is a JSValue.
*/
class JSObject : public JSValue {
	
 public:

	/*! 
	  @typedef      InitializeCallback
	  @abstract     The callback invoked when an object is first created.
	  @param object The JSObject being created.
	  @discussion    If you named your function Initialize, you would declare it like this:
	  
	  void Initialize(const JSObject& object);
	  
	  Unlike the other object callbacks, the initialize callback is
	  called on the least derived class (the parent class) first, and
	  the most derived class last.
	*/
	using InitializeCallback = std::function<void(const JSObject&)>;
	
	/*! 
	  @typedef       FinalizeCallback
	  @abstract      The callback invoked when an object is finalized (prepared for garbage collection). An object may be finalized on any thread.
	  @param object  The JSObject being finalized.
	  @discussion    If you named your function Finalize, you would declare it like this:
	  
	  void Finalize(const JSObject& object);
	  
	  The finalize callback is called on the most derived class first,
	  and the least derived class (the parent class) last.
	  
	  You must not call any function that may cause a garbage collection
	  or an allocation of a garbage collected object from within a
	  JSObjectFinalizeCallback. This includes all methods on all objects
	  that take a JSContext in their constructor parameter list.
	*/
	using FinalizeCallback = std::function<void(const JSObject&)>;
	
	/*! 
	  @typedef             HasPropertyCallback
	  @abstract            The callback invoked when determining whether an object has a property.
	  @param object        The JSObject to search for the property.
	  @param property_name A JSString containing the name of the property to look up.
	  @result              true if object has the property, otherwise false.
	  @discussion          If you named your function HasProperty, you would declare it like this:
	  
	  bool HasProperty(const JSObject& object, const JSString& property_name);
	  
	  If this function returns false, the hasProperty request forwards
	  to object's statically declared properties, then its parent class
	  chain (which includes the default object class), then its
	  prototype chain.
	  
	  This callback enables optimization in cases where only a
	  property's existence needs to be known, not its value, and
	  computing its value would be expensive.
	  
	  If this callback doesn't exist, then the
	  JSObjectGetPropertyCallback callback will be used instead.
	*/
	using HasPropertyCallback = std::function<bool(const JSObject&, const JSString&)>;
	
	/*! 
	  @typedef             GetPropertyCallback
	  @abstract            The callback invoked when getting a property's value.
	  @param object        The JSObject to search for the property.
	  @param property_name A JSString containing the name of the property to get.
	  @result              The property's value if object has the property, otherwise JSUndefined.
	  @discussion          If you named your function GetProperty, you would declare it like this:
	  
	  JSValue GetProperty(const JSObject& object, const JSString& property_name);
	  
	  If this function returns JSUndefined, the get request forwards to
	  object's statically declared properties, then its parent class
	  chain (which includes the default object class), then its
	  prototype chain.
	*/
	using GetPropertyCallback = std::function<JSValue(const JSObject&, const JSString&)>;
	
	/*! 
	  @typedef             SetPropertyCallback
	  @abstract            The callback invoked when setting a property's value.
	  @param object        The JSObject on which to set the property's value.
	  @param property_name A JSString containing the name of the property to set.
	  @param value         A JSValue to use as the property's value.
	  @result              true if the property was set, otherwise false.
	  @discussion          If you named your function SetProperty, you would declare it like this:
	  
	  bool SetProperty(const JSObject& object, const JSString& property_name, const JSValue& value);
	  
	  If this function returns false, the set request forwards to
	  object's statically declared properties, then its parent class
	  chain (which includes the default object class).
	*/
	using SetPropertyCallback = std::function<bool(const JSObject&, const JSString&, const JSValue&)>;
	
	/*! 
	  @typedef             DeletePropertyCallback
	  @abstract            The callback invoked when deleting a property.
	  @param object        The JSObject in which to delete the property.
	  @param property_name A JSString containing the name of the property to delete.
	  @result              true if propertyName was successfully deleted, otherwise false.
	  @discussion          If you named your function DeleteProperty, you would declare it like this:
	  
	  bool DeleteProperty(const JSObject& object, const JSString& property)name);
	  
	  If this function returns false, the delete request forwards to
	  object's statically declared properties, then its parent class
	  chain (which includes the default object class).
	*/
	using DeletePropertyCallback = std::function<bool(const JSObject&, const JSString&)>;
	
	/*! 
	  @typedef           GetPropertyNamesCallback
	  @abstract          The callback invoked when collecting the names of an object's properties.
	  @param object      The JSObject whose property names are being collected.
	  @param accumulator A JavaScript property name accumulator in which to accumulate the names of object's properties.
	  @discussion        If you named your function GetPropertyNames, you would declare it like this:
	  
	  void GetPropertyNames(const JSObject& object, const JSPropertyNameAccumulator& accumulator);
	  
	  Property name accumulators are used by JSObject::CopyPropertyNames
	  and JavaScript for...in loops.
	  
	  Use JSPropertyNameAccumulator::AddName to add property names to
	  accumulator. A class's getPropertyNames callback only needs to
	  provide the names of properties that the class vends through a
	  custom getProperty or setProperty callback. Other properties,
	  including statically declared properties, properties vended by
	  other classes, and properties belonging to object's prototype, are
	  added independently.
	*/
	using GetPropertyNamesCallback = std::function<void(const JSObject&, const JSPropertyNameAccumulator&)>;
	
	/*! 
	  @typedef           CallAsFunctionCallback
	  @abstract          The callback invoked when an object is called as a function.
	  @param function    A JSObject that is the function being called.
	  @param arguments   A JSValue array of arguments to pass to the function.
	  @param this_object The object to use as "this".
	  @result            A JSValue that is the function's return value.
	  @discussion        If you named your function CallAsFunction, you would declare it like this:
	  
	  JSValue CallAsFunction(const JSObject& object, const std::vector<JSValue>& arguments, const JSObject& this_object);
	  
	  If your callback were invoked by the JavaScript expression
	  'myObject.myFunction()', function would be set to myFunction, and
	  thisObject would be set to myObject.
	  
	  If this callback is does not exist, then calling your object as a
	  function will throw an exception.
	*/
	using CallAsFunctionCallback = std::function<JSValue(const JSObject&, const std::vector<JSValue>&, const JSObject&)>;
	
	/*! 
	  @typedef           CallAsConstructorCallback
	  @abstract          The callback invoked when an object is used as a constructor in a 'new' expression.
	  @param constructor A JSObject that is the constructor being called.
	  @param arguments   A JSValue array of arguments to pass to the constructor.
	  @result            A JSObject that is the constructor's return value.
	  @discussion        If you named your function CallAsConstructor, you would declare it like this:
	  
	  JSObject CallAsConstructor(const JSObject& constructor, const std::vector<JSValue>& arguments);
	  
	  If your callback were invoked by the JavaScript expression 'new
	  myConstructor()', constructor would be set to myConstructor.
	  
	  If this callback is doest not exist, using your object as a
	  constructor in a 'new' expression will throw an exception.
	*/
	using CallAsConstructorCallback = std::function<JSObject(const JSObject&, const std::vector<JSValue>&)>;
	
	/*! 
	  @typedef                 HasInstanceCallback
	  @abstract                The callback invoked when an object is used as the target of an 'instanceof' expression.
	  @param constructor       The JSObject that is the target of the 'instanceof' expression.
	  @param possible_instance The JSValue being tested to determine if it is an instance of constructor.
	  @result                  true if possible_instance is an instance of constructor, otherwise false.
	  @discussion If you named your function HasInstance, you would declare it like this:
	  
	  bool HasInstance(const JSObject& constructor, const JSValue possible_instance);
	  
	  If your callback were invoked by the JavaScript expression
	  'someValue instanceof myObject', constructor would be set to
	  myObject and possible_instance would be set to someValue.
	  
	  If this callback is does not exist, 'instanceof' expressions that
	  target your object will return false.
	  
	  Standard JavaScript practice calls for objects that implement the
	  callAsConstructor callback to implement the
	  JSObjectHasInstanceCallback callback as well.
	*/
	using HasInstanceCallback = std::function<bool(const JSObject&, const JSValue&)>;
	
	/*! 
	  @typedef       ConvertToTypeCallback
	  @abstract      The callback invoked when converting an object to a particular JavaScript type.
	  @param object  The JSObject to convert.
	  @param type    A JSValue::Type specifying the JavaScript type to convert to.
	  @result        The objects's converted value, or JSUndefined if the object was not converted.
	  @discussion    If you named your function ConvertToType, you would declare it like this:
	  
	  JSValue ConvertToType(const JSObject& object, JSValue::Type type);
	  
	  If this function returns JSUndefined, the conversion request
	  forwards to object's parent class chain (which includes the
	  default object class).
	  
	  This function is only invoked when converting an object to number
	  or string. An object converted to boolean is 'true.' An object
	  converted to object is itself.
	*/
	using ConvertToTypeCallback = std::function<JSValue(const JSObject&, JSValue::Type)>;
	
	/*!
	  @method
	  @abstract       Create a JavaScript object from the given JavaScript value.
	  @param js_value The JSValue to convert.
	  @result         The JSObject result of conversion.
	  @throws         std::invalid_argument if the given JavaScript value could not be converted to a JavaScript object.
	*/
	//JSObject(const JSValue& js_value);

	/*!
	  @method
	  @abstract         Create a an empty JavaScript object.
	  @param js_context The execution context to use.
	  @result           An empty JavaScript object.
	*/
	JSObject(const JSContext& js_context) : JSObject(js_context, JSObjectMake(js_context, nullptr, nullptr)) {
	}

	/*!
	  @method
	  @abstract Return this object's prototype.
	  @result   A JSValue that is this object's prototype.
	*/
	JSValue GetPrototype() const {
		return JSValue(js_context_, JSObjectGetPrototype(js_context_, js_object_ref_));
	}

	/*!
	  @method
	  @abstract Sets this object's prototype.
	  @param    value A JSValue to set as this object's prototype.
	*/
	void SetPrototype(const JSValue& js_value) {
		JSObjectSetPrototype(js_context_, js_object_ref_, js_value);
	}

	/*!
	  @method
	  @abstract Gets this object's private data.
	  @result   A void* that is this object's private data, if the object has private data, otherwise NULL.
	*/
	void* GetPrivate() const {
		return JSObjectGetPrivate(js_object_ref_);
	}
	
	/*!
	  @method
	  @abstract    Sets a pointer to private data on this object.
	  @param data  A void* to set as this object's private data.
	  @result true if this object can store private data, otherwise false.
	  @discussion  The default object class does not allocate storage for private data. Only objects created with a non-NULL JSClass can store private data.
	*/
	bool SetPrivate(void* data) {
		return JSObjectSetPrivate(js_object_ref_, data);
	}

	/*!
	  @method
	  @abstract Return a std::vector<JSString> of the names of this object's enumerable properties.
	  @result   A std::vector<JSString> containing the names of this object's enumerable properties.
	*/
	std::vector<JSString> GetPropertyNames() const;
			
	/*!
	  @method
	  @abstract Return a std::unordered_map<JSString, JSValue> of this object's enumerable properties.
	  @result   A std::unordered_map<JSString, JSValue> of this object's enumerable properties.
	*/
	std::unordered_map<JSString, JSValue> GetProperties() const;

	/*!
	  @method
	  @abstract            Determine if this object has a property with the given name.
	  @param property_name A JSString containing the property's name.
	  @result              true if this object has a property whose name matches propertyName, otherwise false.
	*/
	bool HasProperty(const JSString& property_name) const {
		return JSObjectHasProperty(js_context_, js_object_ref_, property_name);
	}

	/*!
	  @method
	  @abstract            Get a property's value from this object.
	  @param property_name A JSString containing the property's name.
	  @result              The property's value if this object has the property, otherwise the undefined value.
	*/
	JSValue GetProperty(const JSString& property_name) const;
	
	/*!
	  @method
	  @abstract             Gets a property from this object by numeric index.
	  @param property_index An integer value that is the property's name.
	  @result               The property's value if this object has the property, otherwise the undefined value.

	  @discussion Calling GetPropertyAtIndex is equivalent to calling
	  GetProperty with a string containing property_index, but
	  GetPropertyAtIndex provides optimized access to numeric
	  properties.
	*/
	JSValue GetPropertyAtIndex(unsigned property_index) const;
	
	/*!
	  @method
	  @abstract             Sets a property on this object.
	  @param property_name  A JSString containing the property's name.
	  @param property_value A JSValue to use as the property's value.
	  @param attributes     An optional set of JSPropertyAttributes that describe the characteristics of this property.
	*/
	void SetProperty(const JSString& property_name, const JSValue& property_value, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>());
	
	/*!
	  @method
	  @abstract             Sets a property on this object by numeric index.
	  @param property_index An integer value that is the property's name.
	  @param value          A JSValue to use as the property's value.

	  @discussion Calling SetPropertyAtIndex is equivalent to calling
	  SetProperty with a string containing property_index, but
	  SetPropertyAtIndex provides optimized access to numeric
	  properties.
	*/
	void SetPropertyAtIndex(unsigned property_index, const JSValue& property_value);
	
/*!
	  @method
	  @abstract            Delete a property from an object.
	  @param property_name A JSString containing the property's name.
	  @result              true if the delete operation succeeds, otherwise false (for example, if the property has the DontDelete attribute set).
	*/
	bool DeleteProperty(const JSString& property_name) const;
	
	/*!
	  @method
	  @abstract Determine whether this object can be called as a function.
	  @result   true if this object can be called as a function, otherwise false.
	*/
	bool IsFunction() const {
		return JSObjectIsFunction(js_context_, js_object_ref_);
	}

	/*!
	  @method
	  @abstract Call this object as a function using the global object as "this."
	  @result   The JSValue that results from calling this object as a function
	  @throws   std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction() const {
		return CallAsFunction(std::vector<JSValue>());
	}

	/*!
	  @method
	  @abstract Call this object as a function using the global object as "this."
	  @result   The JSValue that results from calling this object as a function
	  @throws   std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()() const {
		return CallAsFunction();
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSValue>(), this_object);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const JSObject& this_object) const {
		return CallAsFunction(this_object);
	}

	/*!
	  @method
	  @abstract       Call this object as a function using the global object as "this."
	  @param argument A JSValue to pass as the sole argument to the function.
	  @result         The JSValue that results from calling this object as a function
	  @throws         std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSValue& argument) const {
		return CallAsFunction(std::vector<JSValue>{argument});
	}

	/*!
	  @method
	  @abstract       Call this object as a function using the global object as "this."
	  @param argument A JSValue to pass as the sole argument to the function.
	  @result         The JSValue that results from calling this object as a function
	  @throws         std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const JSValue& argument) const {
		return CallAsFunction(argument);
	}

	/*!
	  @method
	  @abstract       Call this object as a function using the global object as "this."
	  @param argument A JSString to pass as the sole argument to the function.
	  @result         The JSValue that results from calling this object as a function
	  @throws         std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSString& argument) const {
		return CallAsFunction(std::vector<JSString>{argument});
	}

	/*!
	  @method
	  @abstract       Call this object as a function using the global object as "this."
	  @param argument A JSString to pass as the sole argument to the function.
	  @result         The JSValue that results from calling this object as a function
	  @throws         std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const JSString& argument) const {
		return CallAsFunction(argument);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param argument    A JSValue to pass as the sole argument to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSValue& argument, const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSValue>{argument}, this_object);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param argument    A JSValue to pass as the sole argument to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const JSValue& argument, const JSObject& this_object) const {
		return CallAsFunction(argument, this_object);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param argument    A JSString to pass as the sole argument to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSString& argument, const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSString>{argument}, this_object);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param argument    A JSString to pass as the sole argument to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const JSString& argument, const JSObject& this_object) const {
		return CallAsFunction(argument, this_object);
	}

	/*!
	  @method
	  @abstract        Call this object as a function using the global object as "this."
	  @param arguments A JSValue array of arguments to pass to the function.
	  @result          The JSValue that results from calling this object as a function
	  @throws          std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  @abstract        Call this object as a function using the global object as "this."
	  @param arguments A JSValue array of arguments to pass to the function.
	  @result          The JSValue that results from calling this object as a function
	  @throws          std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSValue>& arguments) const {
		return CallAsFunction(arguments);
	}

	/*!
	  @method
	  @abstract        Call this object as a function using the global object as "this."
	  @param arguments A JSString array of arguments to pass to the function.
	  @result          The JSValue that results from calling this object as a function
	  @throws          std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSString>& arguments) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [this](const JSString& js_string) { return JSValue(js_string, js_context_); });
		return CallAsFunction(arguments_array);
	}

	/*!
	  @method
	  @abstract        Call this object as a function using the global object as "this."
	  @param arguments A JSString array of arguments to pass to the function.
	  @result          The JSValue that results from calling this object as a function
	  @throws          std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSString>& arguments) const {
		return CallAsFunction(arguments);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the given JSObject as "this".
	  @param arguments   A JSValue array of arguments to pass to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function.
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const;

	/*!
	  @method
	  @abstract          Call this object as a function using the given JSObject as "this".
	  @param arguments   A JSValue array of arguments to pass to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function.
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSValue>& arguments, const JSObject& this_object) const {
		return CallAsFunction(arguments, this_object);
	}

	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param arguments   A JSString array of arguments to pass to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSString>& arguments, const JSObject& this_object) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [this](const JSString& js_string) { return JSValue(js_string, js_context_); });
		return CallAsFunction(arguments_array, this_object);
	}

	
	/*!
	  @method
	  @abstract          Call this object as a function using the global object as "this."
	  @param arguments   A JSString array of arguments to pass to the function.
	  @param this_object The object to use as "this".
	  @result            The JSValue that results from calling this object as a function
	  @throws            std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSString>& arguments, const JSObject& this_object) const {
		return CallAsFunction(arguments, this_object);
	}

	/*!
	  @method
	  @abstract Determine whether this object can be called as a constructor.
	  @result   true if this object can be called as a constructor, otherwise false.
	*/
	bool IsConstructor() const {
		return JSObjectIsConstructor(js_context_, js_object_ref_);
	}

	/*!
	  @method
	  @abstract Calls this object as a constructor.
	  @result   The JSObject that results from calling this object as a constructor
	  @throws   std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor() const {
		return CallAsConstructor(std::vector<JSValue>());
	}

	/*!
	  @method
	  @abstract       Calls this object as a constructor.
	  @param argument A JSValue to pass as the sole argument to the constructor.
	  @result         The JSObject that results from calling this object as a constructor
	  @throws         std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const JSValue& argument) const {
		return CallAsConstructor(std::vector<JSValue>{argument});
	}

	/*!
	  @method
	  @abstract       Calls this object as a constructor.
	  @param argument A JSString to pass as the sole argument to the constructor.
	  @result         The JSObject that results from calling this object as a constructor
	  @throws         std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const JSString& argument) const {
		return CallAsConstructor(std::vector<JSString>{argument});
	}

	/*!
	  @method
	  @abstract        Calls this object as a constructor.
	  @param arguments A JSValue array of arguments to pass to the constructor.
	  @result          The JSObject that results from calling this object as a constructor
	  @throws          std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  @abstract        Calls this object as a constructor.
	  @param arguments A JSString array of arguments to pass to the constructor.
	  @result          The JSObject that results from calling this object as a constructor
	  @throws          std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSString>& arguments) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [this](const JSString& js_string) { return JSValue(js_string, js_context_); });
		return CallAsConstructor(arguments_array);
	}


	/*!
	  @method
	  @abstract Tests whether a JavaScript value is an object with a given class in its class chain.
	  @param    jsClass The JSClass to test against.
	  @result   true if value is an object and has jsClass in its class chain, otherwise false.
	*/
  // bool isObjectOfClass(const JSClass& js_class) {
	//   return JSValueIsObjectOfClass(js_context_, js_object_ref_, js_class);
	// }


	virtual ~JSObject() {
		JSValueUnprotect(js_context_, js_object_ref_);
	}
	
	// Copy constructor.
	JSObject(const JSObject& rhs) : JSValue(rhs), js_context_(rhs.js_context_), js_object_ref_(rhs.js_object_ref_) {
		JSValueProtect(js_context_, js_object_ref_);
	}
	
	// Move constructor.
	JSObject(JSObject&& rhs) : JSValue(rhs), js_context_(rhs.js_context_), js_object_ref_(rhs.js_object_ref_) {
		JSValueProtect(js_context_, js_object_ref_);
	}
	
	// Create a copy of another JSObject by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSObject& operator=(JSObject rhs) {
		JSValue::operator=(rhs);
		swap(*this, rhs);
		return *this;
	}
	
	friend void swap(JSObject& first, JSObject& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.js_context_   , second.js_context_);
		swap(first.js_object_ref_, second.js_object_ref_);
	}
	
 private:
	
	// For interoperability with the JavaScriptCore C API.
	JSObject(JSGlobalContextRef js_context_ref, JSObjectRef js_object_ref);
	
	// For interoperability with the JavaScriptCore C API.
	operator JSObjectRef() const {
		return js_object_ref_;
	}
	
	friend class JSValue;
  friend class JSPropertyNameArray;
  friend class JSArray;
  friend class JSDate;
  friend class JSError;
  friend class JSRegExp;
  friend class JSFunction;
  friend class JSStaticValue;

	JSContext   js_context_;
	JSObjectRef js_object_ref_ { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

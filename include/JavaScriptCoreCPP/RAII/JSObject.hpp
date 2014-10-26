/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyAttribute.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace JavaScriptCoreCPP { namespace detail {
class JSPropertyNameArray;
}}

namespace JavaScriptCoreCPP { namespace RAII {

class JSPropertyNameAccumulator;
class JSClass;

/*!
  @class
  
  @discussion A JSObject is an RAII wrapper around a JSObjectRef, the
  JavaScriptCore C API representation of a JavaScript object. A
  JSObject is a JSValue.

  The only way to create a JSObject is by using the
  JSContext::CreateObject member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSObject : public JSValue, public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSObject> {
#else
class JSObject : public JSValue {
#endif
	
 public:

	/*!
	  @method
	  
	  @abstract Return this object's prototype.
	  
	  @result A JSValue that is this object's prototype.
	*/
	JSValue GetPrototype() const {
		return JSValue(js_context_, JSObjectGetPrototype(js_context_, js_object_ref_));
	}

	/*!
	  @method
	  
	  @abstract Sets this object's prototype.
	  
	  @param value A JSValue to set as this object's prototype.
	*/
	void SetPrototype(const JSValue& js_value) {
		JSObjectSetPrototype(js_context_, js_object_ref_, js_value);
	}

	/*!
	  @method
	  
	  @abstract Gets this object's private data.
	  
	  @result A void* that is this object's private data, if the object
	  has private data, otherwise nullptr.
	*/
	void* GetPrivate() const {
		return JSObjectGetPrivate(js_object_ref_);
	}
	
	/*!
	  @method
	  
	  @abstract Set this object's private data. Note that you must use
	  the JSObject constructor taking a custom JSClass in order to use
	  this method because the default JSObject constructor does not
	  allocate storage for private data.

	  @param data A void* to set as this object's private data.
	  
	  @result true if this object can store private data, otherwise
	  false.
	*/
	bool SetPrivate(void* data) {
		return JSObjectSetPrivate(js_object_ref_, data);
	}

	/*!
	  @method
	  
	  @abstract Return a std::vector<JSString> of the names of this
	  object's enumerable properties.
	  
	  @result A std::vector<JSString> containing the names of this
	  object's enumerable properties.
	*/
	std::vector<JSString> GetPropertyNames() const;
			
	/*!
	  @method
	  
	  @abstract Return a std::unordered_map<JSString, JSValue> of this
	  object's enumerable properties.
	  
	  @result A std::unordered_map<JSString, JSValue> of this object's
	  enumerable properties.
	*/
	std::unordered_map<JSString, JSValue> GetProperties() const;

	/*!
	  @method
	  
	  @abstract Determine if this object has a property with the given
	  name.
	  
	  @param property_name A JSString containing the property's name.
	  
	  @result true if this object has a property whose name matches
	  propertyName, otherwise false.
	*/
	bool HasProperty(const JSString& property_name) const {
		return JSObjectHasProperty(js_context_, js_object_ref_, property_name);
	}

	/*!
	  @method
	  
	  @abstract Get a property's value from this object.
	  
	  @param property_name A JSString containing the property's name.
	  
	  @result The property's value if this object has the property,
	  otherwise the undefined value.
	*/
	JSValue GetProperty(const JSString& property_name) const;
	
	/*!
	  @method
	  
	  @abstract Gets a property from this object by numeric index. This
	  method is equivalent to calling GetProperty with a string
	  containing the numeric index, but GetPropertyAtIndex provides
	  optimized access for numeric properties.

	  @param property_index An integer value that is the property's
	  name.
	  
	  @result The property's value if this object has the property,
	  otherwise the undefined value.
	*/
	JSValue GetPropertyAtIndex(unsigned property_index) const;
	
	/*!
	  @method
	  
	  @abstract Set a property on this object.
	  
	  @param property_name A JSString containing the property's name.
	  
	  @param property_value A JSValue to use as the property's value.
	  
	  @param attributes An optional set of JSPropertyAttributes that
	  describe the characteristics of this property.
	*/
	void SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute> attributes = std::unordered_set<JSPropertyAttribute>());
	
	/*!
	  @method
	  
	  @abstract Set a property on this object by numeric index. This
	  method is equivalent to calling SetProperty with a string
	  containing the numeric index, but SetPropertyAtIndex provides
	  optimized access for numeric properties.

	  @param property_index An integer value that is the property's
	  name.
	  
	  @param value A JSValue to use as the property's value.
	*/
	void SetPropertyAtIndex(unsigned property_index, const JSValue& property_value);
	
/*!
	  @method
	  
	  @abstract Delete a property from an object.
	  
	  @param property_name A JSString containing the property's name.
	  
	  @result true if the delete operation succeeds, otherwise false
	  (for example, if the property has the DontDelete attribute set).
	*/
	bool DeleteProperty(const JSString& property_name) const;
	
	/*!
	  @method
	  
	  @abstract Determine whether this object can be called as a
	  function.
	  
	  @result true if this object can be called as a function, otherwise
	  false.
	*/
	bool IsFunction() const {
		return JSObjectIsFunction(js_context_, js_object_ref_);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction() const {
		return CallAsFunction(std::vector<JSValue>());
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()() const {
		return CallAsFunction();
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSValue>(), this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const JSObject& this_object) const {
		return CallAsFunction(this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSValue to pass as the sole argument to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSValue& argument) const {
		return CallAsFunction(std::vector<JSValue>{argument});
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSValue to pass as the sole argument to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const JSValue& argument) const {
		return CallAsFunction(argument);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSString to pass as the sole argument to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSString& argument) const {
		return CallAsFunction(std::vector<JSString>{argument});
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSString to pass as the sole argument to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const JSString& argument) const {
		return CallAsFunction(argument);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSValue to pass as the sole argument to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSValue& argument, const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSValue>{argument}, this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSValue to pass as the sole argument to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const JSValue& argument, const JSObject& this_object) const {
		return CallAsFunction(argument, this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSString to pass as the sole argument to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const JSString& argument, const JSObject& this_object) const {
		return CallAsFunction(std::vector<JSString>{argument}, this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param argument A JSString to pass as the sole argument to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const JSString& argument, const JSObject& this_object) const {
		return CallAsFunction(argument, this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSValue array of arguments to pass to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments) const {
		return CallAsFunction(arguments, js_context_.get_global_object());
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSValue array of arguments to pass to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSValue>& arguments) const {
		return CallAsFunction(arguments);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSString array of arguments to pass to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSString>& arguments) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return JSValue(js_context_.CreateString(js_string)); });
		return CallAsFunction(arguments_array);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSString array of arguments to pass to the
	  function.
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSString>& arguments) const {
		return CallAsFunction(arguments);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the given JSObject
	  as "this".
	  
	  @param arguments A JSValue array of arguments to pass to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const;

	/*!
	  @method
	  
	  @abstract Call this object as a function using the given JSObject
	  as "this".
	  
	  @param arguments A JSValue array of arguments to pass to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSValue>& arguments, const JSObject& this_object) const {
		return CallAsFunction(arguments, this_object);
	}

	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSString array of arguments to pass to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSString>& arguments, const JSObject& this_object) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return js_context_.CreateString(js_string); });
		return CallAsFunction(arguments_array, this_object);
	}

	
	/*!
	  @method
	  
	  @abstract Call this object as a function using the global object
	  as "this."
	  
	  @param arguments A JSString array of arguments to pass to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result The JSValue that results from calling this object as a
	  function.
	  
	  @throws std::runtime_error exception if the called function threw
	  an exception, or object is not a function.
	*/
	JSValue operator()(const std::vector<JSString>& arguments, const JSObject& this_object) const {
		return CallAsFunction(arguments, this_object);
	}

	/*!
	  @method
	  
	  @abstract Determine whether this object can be called as a
	  constructor.
	  
	  @result true if this object can be called as a constructor,
	  otherwise false.
	*/
	bool IsConstructor() const {
		return JSObjectIsConstructor(js_context_, js_object_ref_);
	}

	/*!
	  @method
	  
	  @abstract Calls this object as a constructor.
	  
	  @result The JSObject that results from calling this object as a
	  constructor.
	  
	  @throws std::runtime_error exception if the called constructor
	  threw an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor() const {
		return CallAsConstructor(std::vector<JSValue>());
	}

	/*!
	  @method
	  
	  @abstract Calls this object as a constructor.
	  
	  @param argument A JSValue to pass as the sole argument to the
	  constructor.
	  
	  @result The JSObject that results from calling this object as a
	  constructor.
	  
	  @throws std::runtime_error exception if the called constructor
	  threw an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const JSValue& argument) const {
		return CallAsConstructor(std::vector<JSValue>{argument});
	}

	/*!
	  @method
	  
	  @abstract Calls this object as a constructor.
	  
	  @param argument A JSString to pass as the sole argument to the
	  constructor.
	  
	  @result The JSObject that results from calling this object as a
	  constructor.
	  
	  @throws std::runtime_error exception if the called constructor
	  threw an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const JSString& argument) const {
		return CallAsConstructor(std::vector<JSString>{argument});
	}

	/*!
	  @method
	  
	  @abstract Calls this object as a constructor.
	  
	  @param arguments A JSValue array of arguments to pass to the
	  constructor.
	  
	  @result The JSObject that results from calling this object as a
	  constructor.
	  
	  @throws std::runtime_error exception if the called constructor
	  threw an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  
	  @abstract Calls this object as a constructor.
	  
	  @param arguments A JSString array of arguments to pass to the
	  constructor.
	  
	  @result The JSObject that results from calling this object as a
	  constructor.
	  
	  @throws std::runtime_error exception if the called constructor
	  threw an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSString>& arguments) const {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return js_context_.CreateString(js_string); });
		return CallAsConstructor(arguments_array);
	}


	/*!
	  @method
	  
	  @abstract Determine whether this JavaScript object is an object
	  with a given class in its class chain.
	  
	  @param jsClass The JSClass to test against.
	  
	  @result true if value is an object and has jsClass in its class
	  chain, otherwise false.
	*/
	bool IsObjectOfClass(const JSClass& js_class) {
		return JSValueIsObjectOfClass(js_context_, js_object_ref_, js_class);
	}


	virtual ~JSObject() {
		JSValueUnprotect(js_context_, js_object_ref_);
	}
	
	// Copy constructor.
	JSObject(const JSObject& rhs)
			: JSValue(rhs)
			, js_object_ref_(rhs.js_object_ref_) {
		JSValueProtect(js_context_, js_object_ref_);
	}
	
	// Move constructor.
	JSObject(JSObject&& rhs)
			: JSValue(rhs)
			, js_object_ref_(rhs.js_object_ref_) {
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
		swap(first.js_object_ref_, second.js_object_ref_);
	}

 protected:

	// Only a JSContext can create a JSObject.
	JSObject(const JSContext& js_context) : JSObject(js_context, JSObjectMake(js_context, nullptr, nullptr)) {
	}

	// Only a JSContext can create a JSObject.
	JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data = nullptr);
	
 private:
	
	JSObject(const JSContext& js_context, JSObjectRef js_object_ref);

	// // For interoperability with the JavaScriptCore C API.
	// JSObject(JSContextRef js_context_ref, JSObjectRef js_object_ref);
	
	// For interoperability with the JavaScriptCore C API.
	operator JSObjectRef() const {
		return js_object_ref_;
	}
	
  friend class JSContext;
	friend class JSValue;
	friend class detail::JSPropertyNameArray;
  friend class JSArray;
  friend class JSDate;
  friend class JSError;
  friend class JSRegExp;
  friend class JSFunction;

	template<typename T>
	friend class JSNativeObject;

	JSObjectRef js_object_ref_ { nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

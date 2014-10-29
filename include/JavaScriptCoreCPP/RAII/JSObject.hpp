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

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyAttribute.hpp"
#include <vector>
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
  JSContext::CreateObject member functions.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSObject : public JSValue, public detail::JSPerformanceCounter<JSObject> {
#else
class JSObject : public JSValue {
#endif
	
 public:

	/*!
	  @method
	  
	  @abstract Determine whether this object can be called as a
	  constructor.
	  
	  @result true if this object can be called as a constructor.
	*/
	virtual bool IsConstructor() const {
		return JSObjectIsConstructor(get_context(), js_object_ref__);
	}

	/*!
	  @method
	  
	  @abstract Return the result when this JavaScript object is used as
	  the target of an 'instanceof' expression. This method returns
	  false by default.
	  
	  @param possible_instance The JSValue being tested to determine if
	  it is an instance of this JavaScript object.
	  
	  @result Return true if this JavaScript object is an 'instanceof'
	  possible_instance.
	*/
	virtual bool HasInstance(const JSValue& possible_instance) const {
		return false;
	}

	/*!
	  @method
	  
	  @abstract Call this JavaScript object as a constructor as if in a
	  'new' expression.
	  
	  @param arguments The JSValue argument(s) to pass to the function.
	  
	  @result The JavaScript object of the constructor's return value.
	  
	  @throws std::runtime_error if either this JavaScript object can't
	  be called as a constructor, or calling the constructor itself
	  threw a JavaScript exception.
	*/
	virtual JSObject CallAsConstructor(const std::vector<JSValue>&  arguments);
	virtual JSObject CallAsConstructor(                                      ) final { return CallAsConstructor(std::vector<JSValue>()         ); }
	virtual JSObject CallAsConstructor(const JSValue&               argument ) final { return CallAsConstructor(std::vector<JSValue> {argument}); }
	virtual JSObject CallAsConstructor(const JSString&              argument ) final { return CallAsConstructor(std::vector<JSString>{argument}); }
	virtual JSObject CallAsConstructor(const std::vector<JSString>& arguments) final {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return get_context().CreateString(js_string); });
		return CallAsConstructor(arguments_array);
	}

	/*!
	  @method
	  
	  @abstract Determine whether this object can be called as a
	  function.
	  
	  @result true if this object can be called as a function.
	*/
	virtual bool IsFunction() const {
		return JSObjectIsFunction(get_context(), js_object_ref__);
	}
	
	/*!
	  @method
	  
	  @abstract Call this JavaScript object as a function. A
	  std::runtime_error exception is thrown if this JavaScript object
	  can't be called as a function.
	  
	  @discussion In the JavaScript expression 'myObject.myFunction()',
	  the "this_object" parameter will be set to 'myObject' and this
	  JavaScript object is 'myFunction'.

	  @param arguments The JSValue argument(s) to pass to the function.
	  
	  @param this_object An optional JavaScript object to use as "this".
	  
	  @result Return the function's return value.

	  @throws std::runtime_error if either this JavaScript object can't
	  be called as a function, or calling the function itself threw a
	  JavaScript exception.
	*/
	virtual JSValue operator()(                                                                     ) final { return CallAsFunction(                      ); }
	virtual JSValue operator()(                                          const JSObject& this_object) final { return CallAsFunction(           this_object); }
	virtual JSValue operator()(const JSValue&               argument                                ) final { return CallAsFunction(argument              ); }
	virtual JSValue operator()(const JSString&              argument                                ) final { return CallAsFunction(argument              ); }
	virtual JSValue operator()(const JSValue&               argument ,   const JSObject& this_object) final { return CallAsFunction(argument , this_object); }
	virtual JSValue operator()(const JSString&              argument ,   const JSObject& this_object) final { return CallAsFunction(argument , this_object); }
	virtual JSValue operator()(const std::vector<JSValue>&  arguments                               ) final { return CallAsFunction(arguments             ); }
	virtual JSValue operator()(const std::vector<JSString>& arguments                               ) final { return CallAsFunction(arguments             ); }
	virtual JSValue operator()(const std::vector<JSValue>&  arguments,   const JSObject& this_object) final { return CallAsFunction(arguments, this_object); }
	virtual JSValue operator()(const std::vector<JSString>& arguments,   const JSObject& this_object) final { return CallAsFunction(arguments, this_object); }

	/*!
	  @method
	  
	  @abstract Return a property of this JavaScript object.
	  
	  @param property_name The name of the property to get.

	  @result The property's value.

	  @throws std::runtime_error if getting the property threw a
	  JavaScript exception.
	*/
	virtual JSValue GetProperty(const JSString& property_name) const;

	/*!
	  @method
	  
	  @abstract Return a property of this JavaScript object by numeric
	  index. This method is equivalent to calling GetProperty with a
	  string containing the numeric index, but provides optimized access
	  for numeric properties.
	  
	  @param property_index An integer value that is the property's
	  name.
	  
	  @result The property's value.
	  
	  @throws std::runtime_error if getting the property threw a
	  JavaScript exception.
	*/
	virtual JSValue GetProperty(unsigned property_index) const;

	/*!
	  @method
	  
	  @abstract Set a property on this JavaScript object with an
	  optional set of attributes.
	  
	  @param property_name The name of the property to set.
	  
	  @param value The value of the the property to set.
	  
	  @param attributes An optional set of property attributes to give
	  to the property.

	  @result true if the the property was set.
	  
	  @throws std::runtime_error if setting the property threw a
	  JavaScript exception.
	*/
	virtual void SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute>& attributes = {});

	/*!
	  @method
	  
	  @abstract Set a property on this JavaScript object by numeric
	  index. This method is equivalent to calling SetProperty with a
	  string containing the numeric index, but provides optimized access
	  for numeric properties.
	  
	  @param property_index An integer value that is the property's
	  name.
	  
	  @param value The value of the the property to set.
	  
	  @throws std::runtime_error if setting the property threw a
	  JavaScript exception.
	*/
	virtual void SetProperty(unsigned property_index, const JSValue& property_value);

	/*!
	  @method
	  
	  @abstract Determine whether this JavaScript object has a property.

	  @param property_name The name of the property to set.
	  
	  @result true if this JavaScript object has the property.
	*/
	virtual bool HasProperty(const JSString& property_name) const {
		return JSObjectHasProperty(get_context(), js_object_ref__, property_name);
	}

	/*!
	  @method
	  
	  @abstract Delete a property from this JavaScript object.

	  @param property_name The name of the property to delete.
	  
	  @result true if the property was deleted.
	  
	  @throws std::runtime_error if deleting the property threw a
	  JavaScript exception.
	*/
	virtual bool DeleteProperty(const JSString& property_name);

	/*! 
	  @method
	  
	  @abstract Convert this JavaScript object to another JavaScript
	  type. An object converted to boolean is 'true.' An object
	  converted to object is itself.
	  
	  @result The objects's converted value.
	*/
	virtual operator JSUndefined() const;
	virtual operator JSNull()      const;
	virtual operator JSBoolean()   const;
	virtual operator JSNumber()    const;
  virtual operator JSString()    const{
	  return static_cast<JSString>(*this);
  }

	/*!
	  @method
	  
	  @abstract Return this JavaScript object's prototype.
	  
	  @result This JavaScript object's prototype.
	*/
	virtual JSValue GetPrototype() const {
		return JSValue(get_context(), JSObjectGetPrototype(get_context(), js_object_ref__));
	}
	
	/*!
	  @method
	  
	  @abstract Sets this JavaScript object's prototype.
	  
	  @param value The value to set as this JavaScript object's
	  prototype.
	*/
	virtual void SetPrototype(const JSValue& js_value) {
		JSObjectSetPrototype(get_context(), js_object_ref__, js_value);
	}

	/*!
	  @method
	  
	  @abstract Gets this object's private data.
	  
	  @result A void* that is this object's private data, if the object
	  has private data, otherwise nullptr.
	*/
	virtual void* GetPrivate() const {
		return JSObjectGetPrivate(js_object_ref__);
	}
	
	/*!
	  @method
	  
	  @abstract Set this object's private data. Note that you must use
	  the JSObject constructor taking a custom JSClass in order to use
	  this method because the default JSObject constructor does not
	  allocate storage for private data.

	  @param data A void* to set as this object's private data.
	  
	  @result true if this object can store private data.
	*/
	virtual bool SetPrivate(void* data) {
		return JSObjectSetPrivate(js_object_ref__, data);
	}

	virtual ~JSObject() {
		JSValueUnprotect(get_context(), js_object_ref__);
	}
	
	// Copy constructor.
	JSObject(const JSObject& rhs)
			: JSValue(rhs)
			, js_object_ref__(rhs.js_object_ref__) {
		JSValueProtect(get_context(), js_object_ref__);
	}
	
	// Move constructor.
	JSObject(JSObject&& rhs)
			: JSValue(rhs)
			, js_object_ref__(rhs.js_object_ref__) {
		JSValueProtect(get_context(), js_object_ref__);
	}
	
#ifdef JAVASCRIPTCORECPP_RAII_MOVE_SEMANTICS_ENABLE
  JSObject& JSObject::operator=(const JSObject&) = default;
  JSObject& JSObject::operator=(JSObject&&) = default;
#endif

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
		swap(first.js_object_ref__, second.js_object_ref__);
	}

 protected:
	
	JSObject(const JSContext& js_context) : JSObject(js_context, JSObjectMake(js_context, nullptr, nullptr)) {
	}

	JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data = nullptr);
	
	/*!
	  @method
	  
	  @abstract Call this JavaScript object as a function. If derived
	  classes don't provide an implementation of this method then the
	  default behavior is to throw a std::runtime_error exception.

	  @param arguments A JSValue array of arguments to pass to the
	  function.
	  
	  @param this_object The object to use as "this".
	  
	  @result Return the function's return value.
	*/
	virtual JSValue CallAsFunction(const std::vector<JSValue>&  arguments, const JSObject& this_object);

	/*!
	  @method
	  
	  @abstract Collect the names of this JavaScript object's
	  properties.
	  
	  @discussion Derived classes should provide only the names of
	  properties that this JavaScript object provides through the
	  GetProperty and SetProperty methods. Other property names are
	  automatically added from properties vended by the JavaScript
	  object's parent class chain and properties belonging to the
	  JavaScript object's prototype chain.
	  
	  @param accumulator A JavaScript property name accumulator in which
	  to accumulate the names of this JavaScript object's
	  properties. Use JSPropertyNameAccumulator::AddName to add property
	  names to the accumulator. Property name accumulators are used by
	  JavaScript for...in loops.
	*/
	virtual void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const {
		return;
	}
	
 private:

	virtual JSValue CallAsFunction(                                                                   ) final { return CallAsFunction(std::vector<JSValue>()                                            ); }
	virtual JSValue CallAsFunction(                                        const JSObject& this_object) final { return CallAsFunction(std::vector<JSValue>()         , this_object                      ); }
	virtual JSValue CallAsFunction(const JSValue&               argument                              ) final { return CallAsFunction(std::vector<JSValue> {argument}                                   ); }
	virtual JSValue CallAsFunction(const JSString&              argument                              ) final { return CallAsFunction(std::vector<JSString>{argument}                                   ); }
	virtual JSValue CallAsFunction(const JSValue&               argument , const JSObject& this_object) final { return CallAsFunction(std::vector<JSValue> {argument}, this_object                      ); }
	virtual JSValue CallAsFunction(const JSString&              argument , const JSObject& this_object) final { return CallAsFunction(std::vector<JSString>{argument}, this_object                      ); }
	virtual JSValue CallAsFunction(const std::vector<JSValue>&  arguments                             ) final { return CallAsFunction(arguments                      , get_context().get_global_object()); }
	virtual JSValue CallAsFunction(const std::vector<JSString>& arguments                             ) final {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return JSValue(get_context().CreateString(js_string)); });
		return CallAsFunction(arguments_array);
	}

	virtual JSValue CallAsFunction(const std::vector<JSString>& arguments, const JSObject& this_object) final {
		std::vector<JSValue> arguments_array;
		std::transform(arguments.begin(),
		               arguments.end(),
		               std::back_inserter(arguments_array),
		               [this](const JSString& js_string) { return get_context().CreateString(js_string); });
		return CallAsFunction(arguments_array, this_object);
	}

	// For interoperability with the JavaScriptCore C API.
	JSObject(const JSContext& js_context, JSObjectRef js_object_ref);

	// For interoperability with the JavaScriptCore C API.
	operator JSObjectRef() const {
		return js_object_ref__;
	}
	
	// Only a JSContext can create a JSObject.
	friend class JSContext;

	// JSValue::operator JSObject() needs access to operator
	// JSObjectRef().
	friend class JSValue;
	
	// The JSPropertyNameArray constructor needs access to operator
	// JSObjectRef().
	friend class detail::JSPropertyNameArray;


	// JSArray needs access to js_object_ref__ to change its value.
	friend class JSArray;
	
	// JSDate needs access to js_object_ref__ to change its value.
	friend class JSDate;
	
	// JSError needs access to js_object_ref__ to change its value.
	friend class JSError;

	// JSRegExp needs access to js_object_ref__ to change its value.
	friend class JSRegExp;

	// JSDate needs access to js_object_ref__ to change its value.
	friend class JSFunction;

	// The JSNativeClass static functions need access to the JSObject
	// constructor.
	template<typename T>
	friend class JSNativeClass;

	// template<typename T>
	// friend class JSNativeObject;

	JSObjectRef js_object_ref__{ nullptr };
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

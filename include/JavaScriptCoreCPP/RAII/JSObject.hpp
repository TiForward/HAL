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
#include "JavaScriptCoreCPP/RAII/JSUndefined.hpp"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <bitset>
#include <type_traits>
#include <string>
#include <sstream>
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

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

/*!
  @class
  @discussion A JSObject is an RAII wrapper around a JSObjectRef, the
  JavaScriptCore C API representation of a JavaScript object. A
  JSObject is a JSValue.
*/
class JSObject : public JSValue {
	
 public:

	/*!
	  @method
	  @abstract       Create a JavaScript object from the given JavaScript value.
	  @param js_value The JSValue to convert.
	  @result         The JSObject result of conversion.
	  @throws         std::invalid_argument if the given JavaScript value could not be converted to a JavaScript object.
	*/
	JSObject(const JSValue& js_value);

	/*!
	  @method
	  @abstract         Create a an empty JavaScript object.
	  @param js_context The execution context to use.
	  @result           An empty JavaScript object.
	*/
	JSObject(const JSContext& js_context) : JSObject(JSObjectMake(js_context, nullptr, nullptr), js_context) {
	}

	/*!
	  @method
	  @abstract Return this object's prototype.
	  @result   A JSValue that is this object's prototype.
	*/
	JSValue GetPrototype() const {
		return JSValue(JSObjectGetPrototype(js_context_, js_object_ref_), js_context_);
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
	  @param attributes     A logically ORed set of JSPropertyAttributes to give to the property.
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
	  @abstract Calls this object as a function using the global object as "this."
	  @result   The JSValue that results from calling this object as a function
	  @throws   std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments) const;

	/*!
	  @method
	  @abstract           Calls this object as a function using the given JSObject as "this".
	  @param this_object  The object to use as "this".
	  @result             The JSValue that results from calling this object as a function.
	  @throws             std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const;

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
	  @result   The JSObject that results from calling this object as a constructor.
	  @throws   std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) const;

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
	JSObject(const JSObject& rhs) : JSValue(rhs) {
		js_object_ref_ = rhs.js_object_ref_;
		JSValueProtect(js_context_, js_object_ref_);
	}
	
	// Move constructor.
	JSObject(JSObject&& rhs) : JSValue(rhs) {
		js_object_ref_ = rhs.js_object_ref_;
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
	
 private:
	
	// For interoperability with the JavaScriptCore C API.
	JSObject(JSObjectRef js_object_ref, const JSContext& js_context);

	// For interoperability with the JavaScriptCore C API.
	operator JSObjectRef() const {
		return js_object_ref_;
	}
	
	friend class JSValue;
  friend class JSArray;
  friend class JSPropertyNameArray;
		
  // Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSObjectRef js_object_ref_;
	JSContext   js_context_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

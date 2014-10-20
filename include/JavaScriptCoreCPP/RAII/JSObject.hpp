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
class JSObject final	{
	
 public:

	/*!
	  @method
	  @abstract         Create a an empty JavaScript object.
	  @param js_context The execution context to use.
	  @result           An empty JavaScript object.
	*/
	JSObject(const JSContext& js_context) : js_object_ref_(JSObjectMake(js_context, nullptr, nullptr)), js_context_(js_context) {
	}

	/*!
	  @method
	  @abstract Return this object's prototype.
	  @result   A JSValue that is this object's prototype.
	*/
	JSValue get_prototype() const {
		return JSValue(JSObjectGetPrototype(js_context_, js_object_ref_), js_context_);
	}

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
	JSValue GetProperty(const JSString& property_name) const {
		JSValueRef exception { nullptr };
		JSValueRef js_value_ref = JSObjectGetProperty(js_context_, js_object_ref_, property_name, &exception);
		if (exception) {
			// assert(!js_value_ref);
			static const std::string log_prefix { "MDL: JSObject::GetProperty: " };
			std::ostringstream os;
			os << "Caught exception getting property with name \"" << property_name << "\": " << JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
		}

		JSValue js_value(js_value_ref, js_context_);
		if (js_value_ref) {
			js_value = JSValue(js_value_ref, js_context_);
			JSValueUnprotect(js_value, js_value_ref);
		}
		
		return js_value;
	}

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
	JSValue GetPropertyAtIndex(unsigned property_index) const {
		JSValueRef exception { nullptr };
		JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(js_context_, js_object_ref_, property_index, &exception);
		if (exception) {
			// assert(!js_value_ref);
			static const std::string log_prefix { "MDL: JSObject::GetPropertyAtIndex: " };
			std::ostringstream os;
			os << "Caught exception getting property at index \"" << property_index << "\": " << JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
		}
		
		JSValue js_value(js_value_ref, js_context_);
		if (js_value_ref) {
			js_value = JSValue(js_value_ref, js_context_);
			JSValueUnprotect(js_value, js_value_ref);
		}
		
		return js_value;
	}
	
	/*!
	  @method
	  @abstract             Sets a property on this object.
	  @param property_name  A JSString containing the property's name.
	  @param property_value A JSValue to use as the property's value.
	  @param attributes     A logically ORed set of JSPropertyAttributes to give to the property.
	*/
	void SetProperty(const JSString& property_name, const JSValue& property_value, const std::set<JSPropertyAttribute> attributes = std::set<JSPropertyAttribute>()) {
		using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
		std::bitset<4> property_attributes;
		for (auto property_attribute : attributes) {
			const auto bit_position = static_cast<property_attribute_underlying_type>(property_attribute);
			property_attributes.set(bit_position);
		}
		JSValueRef exception { nullptr };
		JSObjectSetProperty(js_context_, js_object_ref_, property_name, property_value, static_cast<property_attribute_underlying_type>(property_attributes.to_ulong()), &exception);
		if (exception) {
			// assert(!js_value_ref);
			static const std::string log_prefix { "MDL: JSObject::SetProperty: " };
			std::ostringstream os;
			os << "Caught exception setting property with name \""
			   << property_name
			   << "\" to value \""
			   << property_value
			   << "\": "
			   << JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
		}
	}

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
	void SetPropertyAtIndex(unsigned property_index, const JSValue& property_value) {
		JSValueRef exception { nullptr };
		JSObjectSetPropertyAtIndex(js_context_, js_object_ref_, property_index, property_value, &exception);
		if (exception) {
			// assert(!js_value_ref);
			static const std::string log_prefix { "MDL: JSObject::SetPropertyAtIndex: " };
			std::ostringstream os;
			os << "Caught exception setting property with index \""
			   << property_index
			   << "\" to value \""
			   << property_value
			   << "\": "
			   << JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
		}
	}
	
/*!
	  @method
	  @abstract            Delete a property from an object.
	  @param property_name A JSString containing the property's name.
	  @result              true if the delete operation succeeds, otherwise false (for example, if the property has the DontDelete attribute set).
	*/
	bool DeleteProperty(const JSString& property_name) const {
		JSValueRef exception { nullptr };
		const bool result = JSObjectDeleteProperty(js_context_, js_object_ref_, property_name, &exception);
		if (exception) {
			static const std::string log_prefix { "MDL: JSObject::DeleteProperty: " };
			std::ostringstream os;
			os << "Caught exception deleting property with name \"" << property_name << "\": " << JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [WARN] " << message << std::endl;
		}

		return result;
	}
	
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
	JSValue CallAsFunction(const std::vector<JSValue>& arguments) const {
		static const std::string log_prefix { "MDL: JSObject::CallAsFunction: " };

		if (!IsFunction()) {
			const std::string message = "This object is not a function.";
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}
		
		JSValueRef exception { nullptr };
		JSValueRef js_value_ref = nullptr;
		const JSObjectRef this_object = nullptr;
		if (!arguments.empty()) {
			std::vector<JSValueRef> arguments_array;
			std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
			js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, arguments_array.size(), &arguments_array[0], &exception);
		} else {
			js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, 0, nullptr, &exception);
		}

		if (exception) {
			// assert(!js_value_ref);
			const std::string message = JSValue(exception, js_context_);
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}

		assert(js_value_ref);
		JSValue js_value(js_value_ref, js_context_);
		JSValueUnprotect(js_context_, js_value_ref);
		
		return js_value;
	}

	/*!
	  @method
	  @abstract           Calls this object as a function using the given JSObject as "this".
	  @param this_object  The object to use as "this".
	  @result             The JSValue that results from calling this object as a function.
	  @throws             std::runtime_error exception if the called function through an exception, or object is not a function.
	*/
	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const {
		static const std::string log_prefix { "MDL: JSObject::CallAsFunction: " };

		if (!IsFunction()) {
			const std::string message = "This object is not a function.";
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}
		
		JSValueRef exception { nullptr };
		JSValueRef js_value_ref = nullptr;
		if (!arguments.empty()) {
			std::vector<JSValueRef> arguments_array;
			std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
			js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, arguments_array.size(), &arguments_array[0], &exception);
		} else {
			js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, 0, nullptr, &exception);
		}

		if (exception) {
			// assert(!js_value_ref);
			const std::string message = JSValue(exception, js_context_);
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}

		assert(js_value_ref);
		JSValue js_value(js_value_ref, js_context_);
		JSValueUnprotect(js_context_, js_value_ref);
		
		return js_value;
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
	  @result   The JSObject that results from calling this object as a constructor.
	  @throws   std::runtime_error exception if the called constructor through an exception, or object is not a constructor.
	*/
	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) const {
		static const std::string log_prefix { "MDL: JSObject::CallAsConstructor: " };

		if (!IsConstructor()) {
			const std::string message = "This object is not a constructor.";
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}
		
		JSValueRef exception { nullptr };
		JSObjectRef js_object_ref = nullptr;
		if (!arguments.empty()) {
			std::vector<JSValueRef> arguments_array;
			std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
			js_object_ref = JSObjectCallAsConstructor(js_context_, js_object_ref_, arguments_array.size(), &arguments_array[0], &exception);
		} else {
			js_object_ref = JSObjectCallAsConstructor(js_context_, js_object_ref_, 0, nullptr, &exception);
		}

		if (exception) {
			// assert(!js_object_ref);
			const std::string message = JSValue(exception, js_context_);
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::runtime_error(message);
		}

		assert(js_object_ref);
		JSObject js_object(js_object_ref, js_context_);
		JSValueUnprotect(js_context_, js_object_ref);
		
		return js_object;
	}

	/*!
    @method
    @abstract Tests whether a JavaScript value is an object with a given class in its class chain.
    @param    jsClass The JSClass to test against.
    @result   true if value is an object and has jsClass in its class chain, otherwise false.
  */
  // bool isObjectOfClass(const JSClass& js_class) {
  //   return JSValueIsObjectOfClass(js_context_, js_value_ref_, js_class);
  // }
  
  

	/*!
	  @method
	  @abstract          Determined whether this JSObject was constructed by the given constructor, as compared by the JS instanceof operator.
	  @param constructor The constructor to test against.
	  @result            true if this object was constructed by the given constructor, as compared by the JS instanceof operator, otherwise false.
	*/
	// bool IsInstanceOfConstructor(const JSObject& constructor) {
	// 	JSValueRef exception { nullptr };
	// 	const bool result = JSValueIsInstanceOfConstructor(js_context_, js_value_ref_, constructor, &exception);
	// 	if (exception) {
	// 		static const std::string log_prefix { "MDL: JSValue::IsInstanceOfConstructor:" };
	// 		std::ostringstream os;
	// 		os << "JSValue caught exception during JSValueIsInstanceOfConstructor: " << JSValue(exception, js_context_);
	// 		const std::string message = os.str();
	// 		std::clog << log_prefix << " [DEBUG] " << message << std::endl;
	// 		assert(result == false);
	// 	}
		
	// 	return result;
	// }

  ~JSObject() {
		JSValueUnprotect(js_context_, js_object_ref_);
	}
	
	// Copy constructor.
	JSObject(const JSObject& rhs) {
		js_object_ref_ = rhs.js_object_ref_;
		js_context_    = rhs.js_context_;
		JSValueProtect(js_context_, js_object_ref_);
	}

  // Move constructor.
  JSObject(JSObject&& rhs) {
	  js_object_ref_ = rhs.js_object_ref_;
	  js_context_    = rhs.js_context_;
	  JSValueProtect(js_context_, js_object_ref_);
  }
  
  // Create a copy of another JSContextGroup by assignment. This is a unified
  // assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
  JSObject& operator=(JSObject rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSObject& first, JSObject& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_object_ref_, second.js_object_ref_);
    swap(first.js_context_   , second.js_context_);
  }

 private:

  // For interoperability with the JavaScriptCore C API.
  JSObject(JSObjectRef js_object_ref, const JSContext& js_context) : js_object_ref_(js_object_ref), js_context_(js_context) {
	  JSValueProtect(js_context_, js_object_ref_);
	}
	
  // For interoperability with the JavaScriptCore C API.
	operator JSObjectRef() const {
		return js_object_ref_;
	}
	
	// For interoperability with the JavaScriptCore C API.
	operator JSContextRef() const {
	  return js_context_;
  }

  friend class JSArray;

  // Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSObjectRef js_object_ref_;
	JSContext   js_context_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSOBJECT_HPP_

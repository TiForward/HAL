/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_

//#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"

#include "JavaScriptCoreCPP/RAII/JSNativeClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"

#include <JavaScriptCore/JavaScript.h>


#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_DEBUG


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#ifndef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX std::mutex js_native_object_mutex_;
#endif

#ifndef JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_LOCK_GUARD std::lock_guard<std::mutex> js_native_object_lock(js_native_object_mutex_);
#endif

#else
#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace RAII {

using namespace JavaScriptCoreCPP::detail;

/*!
  @class

  @abstract This class is the bridge between a custom JavaScript
  object backed by a C++ class for some or all of its functionality.
*/
template<typename T>
class JSNativeObject final : public JSObject {
	
 public:
	
	// virtual bool IsConstructor() const;
	// virtual bool HasInstance(const JSValue& possible_instance) const;
	// virtual JSObject CallAsConstructor(const std::vector<JSValue>& arguments);
	// virtual bool IsFunction() const;
	// virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const;
	// virtual JSValue GetProperty(const JSString& property_name) const;
	// virtual JSValue GetProperty(unsigned property_index) const;
	// virtual bool SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute> attributes);
	// virtual bool SetProperty(unsigned property_index, const JSValue& property_value);
	// virtual bool HasProperty(const JSString& property_name) const;
	// virtual bool DeleteProperty(const JSString& property_name) const;
	// virtual JSValue ConvertToType(JSType type) const;
	// virtual operator JSUndefined() const;
	// virtual operator JSNull() const;
	// virtual operator JSBoolean() const;
	// virtual operator JSNumber() const;
	// virtual operator JSString() const;
	// virtual operator JSObject() const;
	// virtual JSValue GetPrototype() const;
	// virtual JSValue SetPrototype(const JSValue& js_value) const;
	// virtual void* GetPrivate() const;
	// virtual bool SetPrivate(void* data) const;
	
	virtual ~JSNativeObject() {
	}

	// Copy constructor.
	JSNativeObject(const JSNativeObject<T>& rhs)
			: JSObject(rhs.get_context(), rhs.js_native_class__)
			, js_native_class__(rhs.js_native_class__) {
	}
	
	// Move constructor.
	JSNativeObject(JSNativeObject<T>&& rhs)
			: JSObject(rhs.get_context(), rhs.js_native_class__)
			, js_native_class__(rhs.js_native_class__) {
	}
  
  // Create a copy of another JSNativeObject by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
	JSNativeObject& operator=(JSNativeObject<T> rhs) {
		JSObject::operator=(rhs);
    swap(*this, rhs);
    return *this;
  }
  
	friend void swap(JSNativeObject<T>& first, JSNativeObject<T>& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(first.js_native_class__, second.js_native_class__);
}

private:

	JSNativeObject(const JSContext& js_context, const JSNativeClass<T>& js_native_class)
			: JSObject(const JSContext& js_context, const JSClass& js_class, nullptr)
			, js_native_class__(js_native_class) {
		// new T
		// page 95.
		SetPrivate(new T());
	}

	// Only a JSContext can create a JSNativeObject.
	friend class JSContext;
	
	JSNativeClass<T> js_native_class__;
	JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX;
	JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_STATIC_MUTEX;
};

template<typename T>
JSContext CreateContext(const JSNativeClass<T>& global_object_class) const {
	return JSContext(*this, global_object_class);
}

template<typename T>
JSNativeObject<T> JSContextRef::CreateObject(const JSNativeClass<T>& js_native_class) const {
	return JSNativeClass<T>(get_context(), js_native_class);
}

/*
template<typename T>
virtual bool JSNativeObject<T>::IsConstructor() const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::IsConstructor: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual bool JSNativeObject<T>::HasInstance(const JSValue& possible_instance) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::HasInstance: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual JSObject JSNativeObject<T>::CallAsConstructor(const std::vector<JSValue>& arguments) {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::CallAsConstructor: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual bool JSNativeObject<T>::IsFunction() const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::IsFunction: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual JSValue JSNativeObject<T>::CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::CallAsFunction: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual JSValue JSNativeObject<T>::GetProperty(const JSString& property_name) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::GetProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual JSValue JSNativeObject<T>::GetProperty(unsigned property_index) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::GetProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual bool JSNativeObject<T>::SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute> attributes) {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::SetProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual bool JSNativeObject<T>::SetProperty(unsigned property_index, const JSValue& property_value) {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::SetProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual bool JSNativeObject<T>::HasProperty(const JSString& property_name) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::HasProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual bool JSNativeObject<T>::DeleteProperty(const JSString& property_name) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::DeleteProperty: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}

template<typename T>
virtual JSValue JSNativeObject<T>::ConvertToType(JSType type) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::ConvertToType: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual operator JSNativeObject<T>::JSUndefined() const {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSUndefined(): " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
} 

template<typename T>
virtual operator JSNativeObject<T>::JSNull() const {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSNull(): " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateNull();
}

template<typename T>
virtual operator JSNativeObject<T>::JSBoolean() const {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSBoolean(): " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateBoolean(false);
}

template<typename T>
virtual operator JSNativeObject<T>::JSNumber() const {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSNumber(): " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateNumber(std::numeric_limits<double>::quiet_NaN());
}

template<typename T>
virtual operator JSNativeObject<T>::JSString() const  {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSString(): " };
	return get_context().CreateString(log_prefix);
}

template<typename T>
virtual operator JSNativeObject<T>::JSObject() const {
	static const std::string log_prefix { "MDL: operator JSNativeObject<T>::JSObject(): " };
	return get_context().CreateUndefined();
}

template<typename T>
virtual JSValue JSNativeObject<T>::GetPrototype() const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::GetPrototype: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return get_context().CreateUndefined();
}

template<typename T>
virtual void JSNativeObject<T>::SetPrototype(const JSValue& js_value) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::SetPrototype: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
}

template<typename T>
virtual void* JSNativeObject<T>::GetPrivate() const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::GetPrivate: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return nullptr;
}

template<typename T>
virtual bool JSNativeObject<T>::SetPrivate(void* data) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::SetPrivate: " };
	std::clog << log_prefix << "[DEBUG] Unimplemented" << std::endl;
	return false;
}
*/
}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


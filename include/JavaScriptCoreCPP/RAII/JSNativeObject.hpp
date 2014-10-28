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
#include "JavaScriptCoreCPP/RAII/JSContext.hpp"

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
class JSNativeObject : public JSObject {
	
 protected:

	// This constructor is the one invoked by the T derived class.
	JSNativeObject(const JSContext& js_context) : JSObject(js_context) {
	}
	
	virtual bool IsConstructor() const override final {
		return JSObject::IsConstructor();
	}

	virtual bool HasInstance(const JSValue& possible_instance) const override final {
		return JSObject::HasInstance();
	}

	virtual JSObject CallAsConstructor(const std::vector<JSValue>& arguments) override final {
		return JSObject::HasInstance(arguments);
	}

	virtual bool IsFunction() const override final {
		return JSObject::IsFunction();
	}

	virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const override final {
		return JSObject::CallAsFunction(arguments, this_object);
	}
	
	virtual JSValue GetProperty(const JSString& property_name) const override final {
		return JSObject::GetProperty(property_name);
	}
	
	virtual JSValue GetProperty(unsigned property_index) const override final {
		return JSObject::GetProperty(property_index);
	}
	
	virtual bool SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute> attributes) override final {
		return JSObject::SetProperty(property_name, property_value, attributes);
	}
	
	virtual bool SetProperty(unsigned property_index, const JSValue& property_value) override final {
		return JSObject::SetProperty(property_index, property_value);
	}
	
	virtual bool HasProperty(const JSString& property_name) const override final {
		return JSObject::HasProperty(property_name);
	}
	
	virtual bool DeleteProperty(const JSString& property_name) const override final {
		return JSObject::DeleteProperty(property_name);
	}
	
	virtual operator JSUndefined() const override final {
		return JSObject::operator JSUndefined();
	}
	
	virtual operator JSNull() const override final {
		return JSObject::operator JSNull();
	}
	
	virtual operator JSBoolean() const override final {
		return JSObject::operator JSBoolean();
	}
	
	virtual operator JSNumber() const override final {
		return JSObject::operator JSNumber();
	}
	
	virtual operator JSString() const override final {
		return JSObject::operator JSString();
	}
	
	virtual JSValue GetPrototype() const override final {
		return JSObject::GetPrototype();
	}
	
	virtual JSValue SetPrototype(const JSValue& js_value) const override final {
		return JSObject::SetPrototype(js_value);
	}

	virtual void* GetPrivate() const override final {
		return nullptr;
	}
	
	virtual bool SetPrivate(void* data) const override final {
		return false;
	}
	
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

	void Initialize(const JSNativeClass<T>& js_native_class, const T* native_object_ptr) {
		js_native_class__          = js_native_class;
		native_object_shared_ptr__ = native_object_shared_ptr;

		JSObjectRef js_object_ref = JSObjectMake(get_context(), js_native_class, native_object_ptr);

		// Replace the JSObjectRef in our base class.
		JSValueUnprotect(get_context(), js_object_ref__);
		js_object_ref__ = js_object_ref;
	}

	// Only a JSContext can call Initialize.
	friend class JSContext;
	
	JSNativeClass<T>   js_native_class__;
	JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX;
};

template<typename T>
JSContext JSContextGroup::CreateContext(const JSNativeClass<T>& global_object_class) const {
	return JSContext(*this, global_object_class);
}

template<typename T, typename... Us>
JSNativeObject<T> JSContext::CreateObject(const JSNativeClass<T>& js_native_class, Us&&... T_constructor_arguments) const {
	auto js_native_object = JSNativeClass<T>(*this);
	js_native_object.Initialize(js_native_class, new T(std::forward<Us>(T_constructor_arguments)...));
	return js_native_object;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


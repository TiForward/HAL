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
			: JSObject(rhs.js_context_, rhs.js_native_class_)
			, js_native_class_(rhs.js_native_class_) {
	}
	
	// Move constructor.
	JSNativeObject(JSNativeObject<T>&& rhs)
			: JSObject(rhs.js_context_, rhs.js_native_class_)
			, js_native_class_(rhs.js_native_class_) {
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
    swap(first.js_native_class_, second.js_native_class_);
}

private:

	// Only a JSContext can create a JSNativeObject.
	JSNativeObject(const JSContext& js_context, const JSNativeClass<T>& js_native_class)
			: JSObject(const JSContext& js_context, const JSClass& js_class, nullptr)
			, js_native_class_(js_native_class) {
		// new T
		// page 95.
		SetPrivate(new T());
	}

	friend class JSContext;
	
	template<typename U>
	friend class JSNativeClass;
	
	JSNativeClass<T> js_native_class_;
	JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_MUTEX;
	JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_STATIC_MUTEX;
};


}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


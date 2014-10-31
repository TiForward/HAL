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

#include <JavaScriptCore/JavaScript.h>


#define JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG


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

	virtual ~JSNativeObject() {
	}
	
	// Copy constructor.
	JSNativeObject(const JSNativeObject& rhs)
			: JSObject(rhs)
			, js_native_class__(rhs.js_native_class__) {
	}
	
	// Move constructor.
	JSNativeObject(JSNativeObject&& rhs)
			: JSObject(std::move(rhs))
			, js_native_class__(rhs.js_native_class__) {
	}
	
#ifdef JAVASCRIPTCORECPP_MOVE_SEMANTICS_ENABLE
	JSNativeObject& JSNativeObject::operator=(const JSNativeObject&) = default;
	JSNativeObject& JSNativeObject::operator=(JSNativeObject&&) = default;
#endif
	
	// Create a copy of another JSNativeObject by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSNativeObject& operator=(JSNativeObject rhs) {
		JSObject::operator=(rhs);
		swap(*this, rhs);
		return *this;
	}
	
	friend void swap(JSNativeObject& first, JSNativeObject& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.js_native_class__, second.js_native_class__);
	}
	
	virtual JSObject CallAsConstructor(const std::vector<JSValue>& arguments) override final {
		assert(attached_to_context__);
		return JSObject::CallAsConstructor(arguments);
	}

	virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, JSObject this_object) override final {
		assert(attached_to_context__);
		return JSObject::CallAsFunction(arguments, this_object);
	}
	
 protected:
	
	JSNativeObject(const JSContext& js_context, const JSNativeClass<T>& js_native_class)
			: JSObject(js_context)
			  //, js_native_class__(JSNativeClassBuilder<T>(js_native_class).build()) {
			, js_native_class__(JSNativeClassBuilder<T>(js_native_class).HasInstance(&JSNativeObject<T>::HasInstance).build()) {
		assert(JSNativeClassBuilder<T>(js_native_class__).HasInstance());
	}

 private:

	// Only a JSContext can attach a JSNativeClass to itself.
	friend JSContext;

	virtual void AttachToContext() const final {
		auto self = const_cast<JSNativeObject*>(this);
		std::call_once(self -> attach_to_context_once_flag__, [self] {
				// Replace the JSObjectRef of our base class with one based on
				// our JSNativeClass that has its private data set to a
				// pointer to ourselves.
				JSValueUnprotect(self -> get_context(), self -> js_object_ref__);
				self -> js_object_ref__ = JSObject(self -> get_context(), self -> js_native_class__, self);
				self -> attached_to_context__ = true;
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

	bool HasInstance(const JSValue& possible_instance) const;

	JSNativeClass<T> js_native_class__;
	std::once_flag   attach_to_context_once_flag__;
	bool             attached_to_context__ { false };
};

template<typename T>
bool JSNativeObject<T>::HasInstance(const JSValue& possible_instance) const {
	static const std::string log_prefix { "MDL: JSNativeObject<T>::HasInstance:" };
	try {
		static_cast<void>(dynamic_cast<const T&>(possible_instance));
		return true;
	} catch (const std::bad_cast& exception) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated std::bad_cast exception: "
		          << exception.what()
		          << "."
		          << std::endl;
#endif
	} catch (const std::exception& exception) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated std::exception: "
		          << exception.what()
		          << "."
		          << std::endl;
#endif
	} catch (...) {
#ifdef JAVASCRIPTCORECPP_JSNATIVEOBJECT_DEBUG
		std::clog << log_prefix
		          << "[DEBUG] possible_instance "
		          << possible_instance
		          << " generated an unknown exception."
		          << std::endl;
#endif
	}
	return false;
}
	
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

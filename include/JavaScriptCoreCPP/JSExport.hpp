/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSEXPORT_HPP_
#define _JAVASCRIPTCORECPP_JSEXPORT_HPP_

//#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbackHandler.hpp"
#include "JavaScriptCoreCPP/detail/JSExportPimpl.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP {

/*!
  @class

  @abstract This class is the bridge between a custom JavaScript
  object backed by a C++ class for some or all of its functionality.
*/
template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSExport : public JSObject, public detail::JSExportCallbackHandler, public detail::JSPerformanceCounter<JSExport> {
#else
class JSExport : public JSObject, public detail::JSExportCallbackHandler {
#endif
	
 public:

	virtual ~JSExport() {
	}
	
	JSExport(const JSExport& rhs)
			: JSObject(rhs)
			, js_native_class__(rhs.js_native_class__) {
	}
	
	JSExport(JSExport&& rhs)
			: JSObject(rhs)
			, js_native_class__(std::move(rhs.js_native_class__)) {
	}
	
	JSExport& JSExport::operator=(const JSExport&) = delete;
	JSExport& JSExport::operator=(JSExport&&) = delete;
	
	// Create a copy of another JSExport by assignment. This is a
  // unified assignment operator that fuses the copy assignment
  // operator
  //
  // X& X::operator=(const X&)
  //
  // and the move assignment operator
  //
  // X& X::operator=(X&&);
  JSExport& operator=(JSExport rhs) {
	  JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	  JSObject::operator=(rhs);
    swap(rhs);
    return *this;
  }

  void swap(JSExport& other) noexcept {
	  JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	  using std::swap;
	  
	  // By swapping the members of two classes, the two classes are
	  // effectively swapped.
	  swap(js_native_class__            , other.js_native_class__);
	  swap(attach_to_context_once_flag__, other.attach_to_context_once_flag__);
	  swap(attached_to_context__        , other.attached_to_context__);
  }

	friend void swap(JSExport& first, JSExport& second) noexcept {
		first.swap(second);
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
		auto self = const_cast<JSExport*>(this);
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
	
	JSExport(const JSContext& js_context, const JSNativeClass<T>& js_native_class)
			: JSObject(js_context, js_native_class)
			, js_native_class__(JSClassBuilder<T>(js_native_class).HasInstance(&JSExport<T>::HasInstance).build()) {
		assert(JSClassBuilder<T>(js_native_class__).HasInstance());
		assert(JSClassBuilder<T>(js_native_class__).Constructor());
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

#endif // _JAVASCRIPTCORECPP_JSEXPORT_HPP_

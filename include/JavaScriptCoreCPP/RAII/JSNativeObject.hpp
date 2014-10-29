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
	JSNativeObject(const JSContext& js_context, const JSNativeClass<T>& js_native_class) : JSObject(js_context, js_native_class, this) {
	}
	
	virtual ~JSNativeObject() = default;

	// Copy constructor.
	JSNativeObject(const JSNativeObject<T>& rhs) = default;
	
	// Move constructor.
	JSNativeObject(JSNativeObject<T>&& rhs) = default;

	// Copy assignment operators.
	JSNativeObject& operator=(const JSNativeObject<T>&) = default;
	JSNativeObject& operator=(JSNativeObject<T>&&) = default;
	
	virtual bool IsConstructor() const override final {
		return JSObject::IsConstructor();
	}

	virtual bool HasInstance(const JSValue& possible_instance) const override final {
		return JSObject::HasInstance(possible_instance);
	}

	virtual JSObject CallAsConstructor(const std::vector<JSValue>& arguments) override final {
		return JSObject::CallAsConstructor(arguments);
	}

	virtual bool IsFunction() const override final {
		return JSObject::IsFunction();
	}

	virtual JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) override final {
		return JSObject::CallAsFunction(arguments, this_object);
	}
	
	virtual JSValue GetProperty(const JSString& property_name) const override final {
		return JSObject::GetProperty(property_name);
	}
	
	virtual JSValue GetProperty(unsigned property_index) const override final {
		return JSObject::GetProperty(property_index);
	}
	
	virtual void SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute>& attributes = {}) override final {
		return JSObject::SetProperty(property_name, property_value, attributes);
	}
	
	virtual void SetProperty(unsigned property_index, const JSValue& property_value) override final {
		return JSObject::SetProperty(property_index, property_value);
	}
	
	virtual bool HasProperty(const JSString& property_name) const override final {
		return JSObject::HasProperty(property_name);
	}
	
	virtual bool DeleteProperty(const JSString& property_name) override final {
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
	
	virtual void SetPrototype(const JSValue& js_value) override final {
		JSObject::SetPrototype(js_value);
	}

	virtual void* GetPrivate() const override final {
		return nullptr;
	}
	
	virtual bool SetPrivate(void* data) override final {
		return false;
	}
};

template<typename T>
JSContext JSContextGroup::CreateContext(const JSNativeClass<T>& global_object_class) const {
	return JSContext(*this, global_object_class);
}

// template<typename T, typename... Us>
// T JSContext::CreateObject(const JSNativeClass<T>& js_native_class, Us&&... T_constructor_arguments) const {
// 	return T(*this, std::forward<Us>(T_constructor_arguments)...));
// }

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECT_HPP_


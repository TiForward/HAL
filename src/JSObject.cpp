/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSObject.hpp"

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSPropertyNameArray.hpp"

#include "JavaScriptCoreCPP/JSUndefined.hpp"
#include "JavaScriptCoreCPP/JSNull.hpp"
#include "JavaScriptCoreCPP/JSBoolean.hpp"
#include "JavaScriptCoreCPP/JSNumber.hpp"

#include "JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <algorithm>
#include <type_traits>
#include <sstream>
#include <limits>

namespace JavaScriptCoreCPP {

JSObject::JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data)
		: JSObject(js_context, JSObjectMake(js_context, js_class, private_data)) {
}

bool JSObject::HasProperty(const JSString& property_name) const {
	return JSObjectHasProperty(get_context(), *this, property_name);
}

JSValue JSObject::GetProperty(const JSString& property_name) const {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetProperty(get_context(), *this, property_name, &exception);
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_value_ref.
		assert(!js_value_ref);
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
	
	assert(js_value_ref);
	return JSValue(get_context(), js_value_ref);
}

JSValue JSObject::GetProperty(unsigned property_index) const {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(get_context(), *this, property_index, &exception);
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_value_ref.
		assert(!js_value_ref);
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
	
	assert(js_value_ref);
	return JSValue(get_context(), js_value_ref);
}

void JSObject::SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute>& attributes) {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;

	JSValueRef exception { nullptr };
	JSObjectSetProperty(get_context(), *this, property_name, property_value, detail::ToJSPropertyAttributes(attributes), &exception);
	if (exception) {
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
}

void JSObject::SetProperty(unsigned property_index, const JSValue& property_value) {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;

	JSValueRef exception { nullptr };
	JSObjectSetPropertyAtIndex(get_context(), *this, property_index, property_value, &exception);
	if (exception) {
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
}

bool JSObject::DeleteProperty(const JSString& property_name) {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	
	JSValueRef exception { nullptr };
	const bool result = JSObjectDeleteProperty(get_context(), *this, property_name, &exception);
	if (exception) {
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
	
	return result;
}

JSPropertyNameArray JSObject::CopyPropertyNames() const {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	return JSPropertyNameArray(*this);
}

bool JSObject::IsConstructor() const {
	return JSObjectIsConstructor(get_context(), *this);
}

JSObject JSObject::CallAsConstructor(                                      ) { return CallAsConstructor(std::vector<JSValue>  {}        ); }
JSObject JSObject::CallAsConstructor(const JSValue&               argument ) { return CallAsConstructor(std::vector<JSValue>  {argument}); }
JSObject JSObject::CallAsConstructor(const JSString&              argument ) { return CallAsConstructor(std::vector<JSString> {argument}); }
JSObject JSObject::CallAsConstructor(const std::vector<JSString>& arguments) { return CallAsConstructor(detail::to_vector(get_context(), arguments)); }
JSObject JSObject::CallAsConstructor(const std::vector<JSValue>&  arguments) {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	
	if (!IsConstructor()) {
		detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a constructor.");
	}
	
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		const auto arguments_array = detail::to_vector(arguments);
		js_object_ref = JSObjectCallAsConstructor(get_context(), *this, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectCallAsConstructor(get_context(), *this, 0, nullptr, &exception);
	}
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_object_ref.
		assert(!js_object_ref);
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}

	// postcondition
	assert(js_object_ref);
	return JSObject(get_context(), js_object_ref);
}

bool JSObject::IsFunction() const {
	return JSObjectIsFunction(get_context(), *this);
}

JSValue JSObject::operator()(                                                            ) { return CallAsFunction(std::vector<JSValue>()                      , *this      ); }
JSValue JSObject::operator()(const JSValue&               argument                       ) { return CallAsFunction({argument}                                  , *this      ); }
JSValue JSObject::operator()(const JSString&              argument                       ) { return CallAsFunction(detail::to_vector(get_context(), {argument}), *this      ); }
JSValue JSObject::operator()(const std::vector<JSValue>&  arguments                      ) { return CallAsFunction(arguments                                   , *this      ); }
JSValue JSObject::operator()(const std::vector<JSString>& arguments                      ) { return CallAsFunction(detail::to_vector(get_context(), arguments) , *this      ); }
JSValue JSObject::operator()(                                        JSObject this_object) { return CallAsFunction(std::vector<JSValue>()                      , this_object); }
JSValue JSObject::operator()(const JSValue&               argument , JSObject this_object) { return CallAsFunction({argument}                                  , this_object); }
JSValue JSObject::operator()(const JSString&              argument , JSObject this_object) { return CallAsFunction(detail::to_vector(get_context(), {argument}), this_object); }
JSValue JSObject::operator()(const std::vector<JSValue>&  arguments, JSObject this_object) { return CallAsFunction(arguments                                   , this_object); }
JSValue JSObject::operator()(const std::vector<JSString>& arguments, JSObject this_object) { return CallAsFunction(detail::to_vector(get_context(), arguments) , this_object); }

JSValue JSObject::CallAsFunction(const std::vector<JSValue>&  arguments, JSObject this_object) {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	
	if (!IsFunction()) {
		detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a function.");
	}
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref { nullptr };
	if (!arguments.empty()) {
		const auto arguments_array = detail::to_vector(arguments);
		js_value_ref = JSObjectCallAsFunction(get_context(), *this, this_object, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_value_ref = JSObjectCallAsFunction(get_context(), *this, this_object, 0, nullptr, &exception);
	}
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_value_ref.
		assert(!js_value_ref);
		detail::ThrowRuntimeError("JSObject", JSValue(get_context(), exception));
	}
	
	assert(js_value_ref);
	return JSValue(get_context(), js_value_ref);
}

JSValue JSObject::GetPrototype() const {
	return JSValue(get_context(), JSObjectGetPrototype(get_context(), *this));
}

void JSObject::SetPrototype(const JSValue& js_value) {
	JSObjectSetPrototype(get_context(), *this, js_value);
}

void* JSObject::GetPrivate() {
	return JSObjectGetPrivate(*this);
}

bool JSObject::SetPrivate(void* data) {
	return JSObjectSetPrivate(*this, data);
}

void JSObject::GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const {
	JAVASCRIPTCORECPP_JSVALUE_LOCK_GUARD;
	for (const auto& property_name : static_cast<std::vector<JSString>>(CopyPropertyNames())) {
		accumulator.AddName(property_name);
	}
}

} // namespace JavaScriptCoreCPP {
